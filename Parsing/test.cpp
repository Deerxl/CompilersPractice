#include <iostream>
#include <fstream>
//#include <fstream>
using namespace std;

#define MaxPerToken 25
#define TypeKeyWord 1
#define TypeOperator 2
#define TypeBoundrary 3
#define TypeIdentifier 4
#define TypeConstant 5
#define TypeError 6

const char* keyWords[] = { "int", "void", "main", "if", "else", "while", "for", "bool" , "true", "false", "return" };
char boundaries[] = { ',', ';', '(', ')', '{', '}' };
char operators[] = { '+', '-', '*', '/', '%', '!', '=', '<', '>', '&', '|' };
char* constants[]; //存储常数
int constantCount = 0; //记录常数的数量
int identifierCount = 0; //记录标识符的数量
int line = 0; //记录当前分析的单词的行数

char buff[1000]; //存储预处理后的单词符号
int i = 0;
char* identifier; //记录当前未定义的标识符
int value = 0;
int type = 0;
int emptyFlag = 0; //标记 算术表达式中是否遇到了 ε

struct Identifiers      //存储标识符
{
	int idType;               //标识符的类型
	char idName[25]; //标识符的名称
	char idValue[25];//标识符的值
}IDTable[100];



int isDigit(char temp)
{
	if (temp >= '0' && temp <= '9')
	{
		return 1;
	}
	return 0;
}

int isCharacter(char temp)
{
	if ((temp >= 'a' && temp <= 'z') || (temp >= 'A' && temp <= 'Z'))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int isKeyWord(char temp[])
{
	string str = temp;

	for (int i = 0; i < sizeof(keyWords) / sizeof(keyWords[0]); i++)
	{
		if (str == keyWords[i])
		{
			return (i + 1);
		}
	}
	return 0;
}

int isBoundary(char temp)
{
	for (int i = 0; i < sizeof(boundaries) / sizeof(boundaries[0]); i++)
	{
		if (temp == boundaries[i])
		{
			return (i + 1);
		}
	}
	return 0;
}

int isOperator(char temp)
{
	for (int i = 0; i < sizeof(operators) / sizeof(operators[0]); i++)
	{
		if (temp == operators[i])
		{
			return (01 + i);
		}
	}
	return 0;
}

int isIdentifier(char temp[])
{
	for (int id = 0; id < identifierCount; id++)
	{
		if (temp == IDTable[i].idName)
		{
			return IDTable[i].idType;
		}
	}
	return 0;
}

int searchIdByName(char* identifier)
{
	for (int id = 0; id < identifierCount; id++)
	{
		if (identifier == IDTable[i].idName)
		{
			return id;
		}
	}
	return -1;
}

void preTreatment(char temp[])
{
	int j = 0;

	for (int i = 0; temp[i] != '#'; i++)
	{

		if (temp[i] == '\t' || temp[i] == '\r')
		{
			continue;
		}
		if (temp[i] == '\n' || temp[i] == ' ')
		{
			buff[j++] = temp[i];
			continue;
		}
		if (temp[i] == '/')
		{
			i++;

			if (temp[i] == '/')
			{
				i++;
				while (temp[i] != '\n')
				{
					i++;
				}
				buff[j++] = temp[i];
			}
			else if (temp[i] == '*')
			{
				i++;
				while (!(temp[i] == '*' && temp[i + 1] == '/'))
				{
					if (temp[i] == '\n')
					{
						buff[j++] = temp[i];
					}
					i++;
				}
				i++;
			}
			continue;
		}

		buff[j++] = temp[i];
	}
	buff[j] = '\0';
} 


/*获取一个单词符号
返回0，表示未定义标识符，需进一步检测，并存入 identifer 中
返回1，关键字，返回value值
返回2，运算符，返回value值
返回3，界限符，返回value值
返回4，已定义标识符，返回idvalue值， 为类型(int, bool)
返回5，数字，存入 constants 中，返回value值
返回6，词法错误类型，无value值
*/
int GetNext(int value)
{
	int j = 0;
	char token[MaxPerToken];

	ofstream out("LexicalAnalysisOutput.text");
	if (!out.is_open())
	{
		cout << "Error opening file" << endl;
		exit(0);
	}

	for (i; buff[i] != '#'; i++)
	{
		while (buff[i] == ' ')
		{
			i++;
		}

		for (int m = 0; m < MaxPerToken; m++)   //清零
		{
			token[m] = '\0';
		}
		j = 0;
		if (buff[i] == '\n')
		{
			line++;
			i++;
		}
		if (isCharacter(buff[i]))
		{
			while (isCharacter(buff[i]) || isDigit(buff[i]) || buff[i] == '_')
			{
				token[j++] = buff[i++];
			}
			token[j] = '\0';
			i--;
			if (int key = isKeyWord(token))
			{

				out << "(" << TypeKeyWord << ", " << key << ")" << endl;

				value = isKeyWord(token);
				return TypeKeyWord;
			}
			else if (isIdentifier(token))
			{

				out << "(" << TypeIdentifier << ", " << token << ")" << endl;
				identifier = token;
				value = isIdentifier(token);
				return TypeIdentifier;
			}
			else
			{
				out << "(" << TypeIdentifier << ", " << token << ")" << endl;
				identifier = token;
				return 0;
			}
		}
		else if (buff[i] == '_')
		{
			while (isCharacter(buff[i]) || isDigit(buff[i]) || buff[i] == '_')
			{
				token[j++] = buff[i++];
			}
			token[j] = '\0';
			i--;	
			//int key = isKeyWord(token);
			if (int key = isKeyWord(token))
			{
				value = isKeyWord(token);
				out << "(" << TypeKeyWord << ", " << key << ")" << endl;
				return TypeKeyWord;
			}
			else if (isIdentifier(token))
			{
				value = isIdentifier(token);
				identifier = token;
				out << "(" << TypeIdentifier << ", " << token << ")" << endl;
				return TypeIdentifier;
			}
			else
			{
				identifier = token;
				out << "(" << TypeIdentifier << ", " << token << ")" << endl;
				return 0;
			}
		}
		else if (isDigit(buff[i]))
		{
			int flag = 1;
			while ( isDigit(buff[i]) || isCharacter(buff[i]))
			{
				token[j++] = buff[i++];
			}
			token[j] = '\0';
			i--;
			for (int k = 0; k < j; k++)
			{
				if (isCharacter(buff[i]))
				{
					flag = 0;
					out << "(" << TypeError << ", " << token << ")" << endl;

					return TypeError;
				}
			}
			if (flag)
			{
				out << "(" << TypeConstant << ", " << token << ")" << endl;

				constants[constantCount] = token;
				value = constantCount++;				
				return TypeConstant;
			}
		}
		else if (int key1 = isOperator(buff[i]))
		{
			int j = i;
			int key2 = isOperator(buff[++j]);

			switch (key1)
			{
			case 1:
				switch (key2)
				{
				case 7:
					value = 12;
					out << "(" << TypeOperator << ", " << value << ")" << endl;
					return TypeOperator;
				case 1:
					value = 23;
					out << "(" << TypeOperator << ", " << value << ")" << endl;
					return TypeOperator;
				default:
					value = key1;
					out << "(" << TypeOperator << ", " << value << ")" << endl;
					return TypeOperator;
				}
			case 2:
				switch (key2)
				{
				case 7:
					value = 13;
					out << "(" << TypeOperator << ", " << value << ")" << endl;
					return TypeOperator;
				case 2:
					value = 24;
					out << "(" << TypeOperator << ", " << value << ")" << endl;
					return TypeOperator;
				default:
					value = key1;
					out << "(" << TypeOperator << ", " << value << ")" << endl;
					return TypeOperator;
				}
			case 3:
				if (key2 == 7)
				{
					value = 14;
					out << "(" << TypeOperator << ", " << value << ")" << endl;
					return TypeOperator;
				}
				else
				{
					value = key1;
					out << "(" << TypeOperator << ", " << value << ")" << endl;
					return TypeOperator;
				}
			case 4:
				if (key2 == 7)
				{
					value = 15;
					out << "(" << TypeOperator << ", " << value << ")" << endl;
					return TypeOperator;
				}
				else
				{
					value = key1;
					out << "(" << TypeOperator << ", " << value << ")" << endl;
					return TypeOperator;
				}
			case 5:
				if (key2 == 7)
				{
					value = 16;
					out << "(" << TypeOperator << ", " << value << ")" << endl;
					return TypeOperator;
				}
				else
				{
					value = key1;
					out << "(" << TypeOperator << ", " << value << ")" << endl;
					return TypeOperator;
				}
			case 6:
				if (key2 == 7)
				{
					value = 17;
					out << "(" << TypeOperator << ", " << value << ")" << endl;
					return TypeOperator;
				}
				else
				{
					value = key1;
					out << "(" << TypeOperator << ", " << value << ")" << endl;
					return TypeOperator;
				}
			case 7:
				if (key2 == 7)
				{
					value = 18;
					out << "(" << TypeOperator << ", " << value << ")" << endl;
					return TypeOperator;
				}
				else
				{
					value = key1;
					out << "(" << TypeOperator << ", " << value << ")" << endl;
					return TypeOperator;
				}
			case 8:
				if (key2 == 7)
				{
					value = 19;
					out << "(" << TypeOperator << ", " << value << ")" << endl;
					return TypeOperator;
				}
				else
				{
					value = key1;
					out << "(" << TypeOperator << ", " << value << ")" << endl;
					return TypeOperator;
				}
			case 9:
				if (key2 == 7)
				{
					value = 20;
					out << "(" << TypeOperator << ", " << value << ")" << endl;
					return TypeOperator;
				}
				else
				{
					value = key1;
					out << "(" << TypeOperator << ", " << value << ")" << endl;
					return TypeOperator;
				}
			case 10:
				if (key2 == key1)
				{
					value = 21;
					out << "(" << TypeOperator << ", " << value << ")" << endl;
					return TypeOperator;
				}
				else
				{
					value = key1;
					out << "(" << TypeOperator << ", " << value << ")" << endl;
					return TypeOperator;
				}
			case 11:
				if (key2 == key1)
				{
					value = 22;
					out << "(" << TypeOperator << ", " << value << ")" << endl;
					return TypeOperator;
				}
				else
				{
					value = key1;
					out << "(" << TypeOperator << ", " << value << ")" << endl;
					return TypeOperator;
				}
			}
		}
		else if (int key = isBoundary(buff[i]))
		{
			token[j] = buff[i];
			out << "(" << TypeBoundrary << ", " << key << ")" << endl;
			value = key;
			return TypeBoundrary;
		}
		else
		{
			out << "(" << TypeError << ", " << buff[i] << ")" << endl;
			return TypeError;
		}
	}
	out.close();
}

void myExpE();
void myExpE1();
void myExpT();
void myExpT1();
void myExpF();

void myCondition();
void myWhile();
void myDeclare(int typetmp, int valuetmp);
void myAssignmentStmt();
void myIf();
void myFunctionStmt(int typetmp, int valuetmp);
void myFunctionBody();

/*简单算术表达式文法实现
E->TE’
E’->+TE’|-TE’|ε
T->FT’
T’->*FT’|/FT’|ε
F->(E)|i
*/
//*************************简单算术表达式************************************
void myExpE()
{
	myExpT();
	myExpE1();
	return;
}
void myExpT()
{
	myExpF();
	myExpT1();
	return;
}
void myExpE1()
{
	if (type == TypeOperator && value == 1)
	{
		myExpT();
		myExpE1();
		return;
	}
	else if (type == TypeOperator && value == 2)
	{
		myExpT();
		myExpE1();
		return;
	}
	else
	{
		emptyFlag = 1;
		return;
	}
}
void myExpT1()
{
	type = GetNext(value);
	if (type == TypeOperator && value == 3)
	{
		myExpF();
		myExpT1();
		return;
	}
	else if (type == TypeOperator && value == 4)
	{
		myExpF();
		myExpT1();
		return;
	}
	else
	{
		emptyFlag = 1;
		return;
	}
}
void myExpF()
{
	type = GetNext(value);
	if (type == TypeBoundrary && value == 3)
	{
		myExpE();
		type = GetNext(value);
		if (type == TypeBoundrary && value == 4)
		{
			return;
		}
		else
		{
			cout << "erro: line " << line << ", 赋值错误，缺少')'" << endl;
			return;
		}
	}
	else if (type == TypeIdentifier || type == TypeConstant)
	{
		return;
	}
	else
	{
		cout << "erro: line " << line << ", 赋值错误" << endl;
		return;
	}
}
//*************************简单算术表达式************************************


void myCondition()
{
	type = GetNext(value);
	if ((type == 1 && value == 8) || (type == 1 && value == 9) // 为 true false 0 1
		|| (type == 5 && '0' == *constants[value]) || (type == 5 && '1' == *constants[value]))
	{
		return;
	}
	else if ((type == 5 && constants[value]) || (isIdentifier(identifier)))    //为表达式
	{
		type = GetNext(value);
		if (type == 2 && ((value >= 17 && value <= 22) || value == 8 || value == 9))
		{
			myExpE();
		}
		else
		{
			cout << "erro: line " << line << ", 条件语句错误" << endl;
		}
	}
	else if (type == 2 && value == 6)     //为 !
	{
		type = GetNext(value);
		if ((type == 5 && constants[value]) || isIdentifier(identifier))
		{
			return;
		}
		else
		{
			cout << "erro: line " << line << ", 条件语句错误" << endl;
		}
	}
	else
	{
		cout << "erro: line " << line << ", 条件语句错误" << endl;
	}
}


void myWhile()
{
	type = GetNext(value);
	if (type == TypeBoundrary && value == 3)
	{
		myCondition();
		if (!emptyFlag)
		{
			type = GetNext(value);
		}
		if (type == TypeBoundrary && value == 4)
		{
			type = GetNext(value);
			if (type == TypeBoundrary && value == 5)
			{
				myFunctionBody();
				//pointBack(); //回溯到上一个单词
				if (type == TypeBoundrary && value == 6)
				{
					return;
				}
				else
				{
					cout << "erro: line " << line << "，while语句缺'}'" << endl;
				}
			}
			else
			{
				cout << "erro: line " << line << "，while语句缺'{'" << endl;
			}
		}
		else
		{
			cout << "erro: line " << line << "，while语句缺')'" << endl;
		}
	}
	else
	{
		cout << "erro: line " << line << "，while语句缺'('" << endl;
	}
}

void myDeclare(int typetmp, int valuetmp)
{
	type = GetNext(value);
	if (type == 0) //是未定义标识符
	{
		IDTable[identifierCount].idType = valuetmp;
		IDTable[identifierCount].idName = identifier;
		//IDTable[identifierCount].idName = identifier;
		//IDTable[identifierCount].ds = identifier;
		//strcpy(IDTable[identifierCount].idName, identifier);
		identifierCount++;
		
		if (type == 2 && value == 7)  //为 =
		{
			myExpE();

			if (!emptyFlag)
			{
				type = GetNext(value);
				emptyFlag = 0;
			}
			if (type = TypeOperator && value == 2)
			{
				return;
			}
			else
			{
				cout << "erro: line " << line << ", 赋值错误" << endl;
			}
			return;
		}
		else if (type == TypeBoundrary && value == 2)  //为 ;
		{
			return;
		}
		else if (type == TypeBoundrary && value == 1)      //为 ,
		{
			myDeclare(type, value);
			return;
		}
		else
		{
			cout << "erro: line " << line << ", 定义错误" << endl;
			return;
		}
	}
	else
	{
		cout << "erro: line " << line << ", 变量声明错误" << endl;
		return;
	}
}

void myAssignmentStmt()     //赋值
{
	type = GetNext(value);
	if (type == TypeOperator && 
		(value == 7 || value == 12 || value == 13 || value == 14 || value == 15 || value == 16)) 
	{
		myExpE();
		
		if (!emptyFlag)
		{
			type = GetNext(value);
			emptyFlag = 0;
		}
		if (type = TypeBoundrary && value == 2)
		{
			return;
		}
		else
		{
			cout << "erro: line " << line << ", 赋值错误，缺少';'" << endl;
		}
	}
	else
	{
		cout << "erro: line " << line << ", 赋值错误" << endl;
	}
	/*else if ((type == TypeOperator && value == 23) || (type == TypeOperator && value == 24))
	{
		type = GetNext(value);
		if()
		return;
	}*/
}


void myIf()
{
	type = GetNext(value);
	if (type == TypeBoundrary && value == 3)
	{
		myCondition();
		type = GetNext(value);
		if (type == TypeBoundrary && value == 4)
		{
			type = GetNext(value);
			if (type == TypeBoundrary && value == 5)
			{
				myFunctionBody();
				if (type == TypeBoundrary && value == 6)
				{
					return;
				}
				else
				{
					cout << "erro: line " << line << "，if语句缺'}'" << endl;
				}
			}
			else
			{
				cout << "erro: line " << line << "，if语句缺'{'" << endl;
			}
		}
		else
		{
			cout << "erro: line " << line << "，if语句缺')'" << endl;
		}
	}
	else
	{
		cout << "erro: line " << line << "，if语句缺'('" << endl;
	}
}

void myFunctionStmt(int typetmp, int valuetmp)
{
	if (typetmp == TypeKeyWord && (valuetmp == 1 || valuetmp == 7))
	{
		myDeclare(typetmp, valuetmp);
		return;
	}
	else if (typetmp == TypeIdentifier && (valuetmp == 1 || valuetmp == 7))
	{
		myAssignmentStmt();
		return;
	}
	else if (typetmp == TypeKeyWord && valuetmp == 4) //if stmt
	{
		myIf();
		return;
	}
	else if (typetmp == TypeKeyWord && valuetmp == 6)  // while stmt
	{
		myWhile();
		return;
	}
	else if (typetmp == TypeKeyWord && valuetmp == 10) // return stmt
	{
		//myReturn();
		return;
	}
}

void myFunctionBody()
{
	while (type != 3 && value != 6)  ///函数体没结束
	{
		myFunctionStmt(type, value);
		type = GetNext(value);
	}
}

void parse()
{
	type = GetNext(value);
	myFunctionStmt(type, value);
}


int main()
{
	char temp[1000];
	int num = 0;
	//char buff[1000];

	string flie = "inputSample.cpp";
	ifstream in;
	in.open(flie, ios::in);
	if (!in.is_open())
	{
		return NULL;
	}

	while (!in.eof())
	{
		temp[num++] = in.get();
	}
	num--;
	in.close();

	preTreatment(temp);
	parse();
	
	return 0;
}