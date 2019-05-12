#include <iostream>
#include <fstream>
#include "parsing.h"
#include <string>
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
int* constants = new int[20]; //�洢����
int constantCount = 0; //��¼����������
int identifierCount = 0; //��¼��ʶ��������
int line = 1; //��¼��ǰ�����ĵ��ʵ�����

char buff[1000]; //�洢Ԥ�����ĵ��ʷ���
int i = 0;
string identifier = ""; //��¼��ǰδ����ı�ʶ��
int value = 0;
int type = 0;
int emptyFlag = 0; //��� �������ʽ���Ƿ������� ��
ofstream out2;        //�ļ���� �﷨����

struct IdentifiersTable      //�洢��ʶ��
{
	int idType = 0;               //��ʶ��������
	string idName = ""; //��ʶ��������
	char* idValue = NULL;//��ʶ����ֵ
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

int isIdentifier(string temp)
{
	for (int id = 0; id < identifierCount; id++)
	{
		if (temp == IDTable[id].idName)
		{
			return IDTable[id].idType;
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

	for (int i = 0; temp[i] != '\0'; i++)
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
					//
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
			else
			{
				buff[j++] = temp[i-1];
			}
			continue;
		}

		buff[j++] = temp[i];
	}
	//buff[j++] = '#';
	buff[j] = '\0';
}


/*��ȡһ�����ʷ���
����0����ʾδ�����ʶ�������һ����⣬������ identifer ��
����1���ؼ��֣�����valueֵ
����2�������������valueֵ
����3�����޷�������valueֵ
����4���Ѷ����ʶ��������idvalueֵ�� Ϊ����(int, bool)
����5�����֣����� constants �У�����valueֵ
����6���ʷ��������ͣ���valueֵ
*/
int GetNext(int& value)
{
	int j = 0;
	char token[MaxPerToken];

	/*ofstream out("LexicalAnalysisOutput.text");
	if (!out.is_open())
	{
		out2 << "Error opening file" << endl;
		exit(0);
	}*/

	for (i; buff[i] != '\0'; i++)
	{
		while (buff[i] == ' ' || buff[i] == '\n')
		{
			if (buff[i] == '\n')
			{
				line++;
			}
			i++;
		}

		for (int m = 0; m < MaxPerToken; m++)   //����
		{
			token[m] = '\0';
		}
		j = 0;

		if (isCharacter(buff[i]))
		{
			while (isCharacter(buff[i]) || isDigit(buff[i]) || buff[i] == '_')
			{
				token[j++] = buff[i++];
			}
			i--;
			token[j] = '\0';
			if (int key = isKeyWord(token))
			{

				//out << "(" << TypeKeyWord << ", " << key << ")" << endl;

				value = isKeyWord(token);
				i++;
				return TypeKeyWord;
			}

			else if (isIdentifier(token))
			{

				//out << "(" << TypeIdentifier << ", " << token << ")" << endl;
				identifier = token;
				value = isIdentifier(token);
				i++;
				return TypeIdentifier;
			}
			else
			{
				//out << "(" << TypeIdentifier << ", " << token << ")" << endl;
				identifier = token;
				i++;
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
			//int key = isKeyWord(token);
			if (int key = isKeyWord(token))
			{
				value = isKeyWord(token);
				//out << "(" << TypeKeyWord << ", " << key << ")" << endl;
				return TypeKeyWord;
			}
			else if (isIdentifier(token))
			{
				value = isIdentifier(token);
				identifier = token;
				//out << "(" << TypeIdentifier << ", " << token << ")" << endl;
				return TypeIdentifier;
			}
			else
			{
				identifier = token;
				//out << "(" << TypeIdentifier << ", " << token << ")" << endl;
				return 0;
			}
		}
		else if (isDigit(buff[i]))
		{
			int flag = 1;
			while (isDigit(buff[i]) || isCharacter(buff[i]))
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
					//out << "(" << TypeError << ", " << token << ")" << endl;

					return TypeError;
				}
			}
			if (flag)
			{
				//out << "(" << TypeConstant << ", " << token << ")" << endl;

				constants[constantCount] = atoi(token);
				value = constantCount++;
				i++;
				return TypeConstant;
			}
		}
		else if (int key1 = isOperator(buff[i]))
		{
			int j = i++;
			int key2 = isOperator(buff[++j]);
			switch (key1)
			{
			case 1:
				switch (key2)
				{
				case 7:
					value = 12; i++; 
					//out << "(" << TypeOperator << ", " << value << ")" << endl;
					return TypeOperator;
				case 1:
					value = 23; i++; 
					//out << "(" << TypeOperator << ", " << value << ")" << endl;
					return TypeOperator;
				default:
					value = key1;
					//out << "(" << TypeOperator << ", " << value << ")" << endl;
					return TypeOperator;
				}
			case 2:
				switch (key2)
				{
				case 7:
					value = 13; i++; 
					//out << "(" << TypeOperator << ", " << value << ")" << endl;
					return TypeOperator;
				case 2:
					value = 24; i++; 
					//out << "(" << TypeOperator << ", " << value << ")" << endl;
					return TypeOperator;
				default:
					value = key1;
					//out << "(" << TypeOperator << ", " << value << ")" << endl;
					return TypeOperator;
				}
			case 3:
				if (key2 == 7)
				{
					value = 14; i++; 
					//out << "(" << TypeOperator << ", " << value << ")" << endl;
					return TypeOperator;
				}
				else
				{
					value = key1;
					//out << "(" << TypeOperator << ", " << value << ")" << endl;
					return TypeOperator;
				}
			case 4:
				if (key2 == 7)
				{
					value = 15; i++; 
					//out << "(" << TypeOperator << ", " << value << ")" << endl;
					return TypeOperator;
				}
				else
				{
					value = key1;
					//out << "(" << TypeOperator << ", " << value << ")" << endl;
					return TypeOperator;
				}
			case 5:
				if (key2 == 7)
				{
					value = 16; i++; 
					//out << "(" << TypeOperator << ", " << value << ")" << endl;
					return TypeOperator;
				}
				else
				{
					value = key1;
					//out << "(" << TypeOperator << ", " << value << ")" << endl;
					return TypeOperator;
				}
			case 6:
				if (key2 == 7)
				{
					value = 17; i++; 
					//out << "(" << TypeOperator << ", " << value << ")" << endl;
					return TypeOperator;
				}
				else
				{
					value = key1;
					//out << "(" << TypeOperator << ", " << value << ")" << endl;
					return TypeOperator;
				}
			case 7:
				if (key2 == 7)
				{
					value = 18; i++; 
					//out << "(" << TypeOperator << ", " << value << ")" << endl;
					return TypeOperator;
				}
				else
				{
					value = key1;
					//out << "(" << TypeOperator << ", " << value << ")" << endl;
					return TypeOperator;
				}
			case 8:
				if (key2 == 7)
				{
					value = 19; i++; 
					//out << "(" << TypeOperator << ", " << value << ")" << endl;
					return TypeOperator;
				}
				else
				{
					value = key1;
					//out << "(" << TypeOperator << ", " << value << ")" << endl;
					return TypeOperator;
				}
			case 9:
				if (key2 == 7)
				{
					value = 20; i++; 
					//out << "(" << TypeOperator << ", " << value << ")" << endl;
					return TypeOperator;
				}
				else
				{
					value = key1;
					//out << "(" << TypeOperator << ", " << value << ")" << endl;
					return TypeOperator;
				}
			case 10:
				if (key2 == key1)
				{
					value = 21; i++; 
					//out << "(" << TypeOperator << ", " << value << ")" << endl;
					return TypeOperator;
				}
				else
				{
					value = key1;
					//out << "(" << TypeOperator << ", " << value << ")" << endl;
					return TypeOperator;
				}
			case 11:
				if (key2 == key1)
				{
					value = 22; i++; 
					//out << "(" << TypeOperator << ", " << value << ")" << endl;
					return TypeOperator;
				}
				else
				{
					value = key1;
					//out << "(" << TypeOperator << ", " << value << ")" << endl;
					return TypeOperator;
				}
			}
		}
		else if (int key2 = isBoundary(buff[i]))
		{
			token[j] = buff[i++];
			//out << "(" << TypeBoundrary << ", " << key << ")" << endl;
			value = key2;
			return TypeBoundrary;
		}
		else
		{
			i++;
			return TypeError;
		}
	}
	//out.close();
	return -1;
}

/*���������ʽ�ķ�ʵ��           ��ʼӦΪ���ʽ��һ������
E->TE��
E��->+TE��|-TE��|��
T->FT��
T��->*FT��|/FT��|��
F->(E)|i
*/
//*************************���������ʽ************************************
void myExpE()
{
	myExpT();
	myExpE1();
}
void myExpT()
{
	myExpF();
	myExpT1();
}
void myExpE1()
{
	if (type == TypeOperator && value == 1)
	{
		type = GetNext(value);
		myExpT();
		myExpE1();
		//return;
	}
	else if (type == TypeOperator && value == 2)
	{
		type = GetNext(value);
		myExpT();
		myExpE1();
	}
}
void myExpT1()
{
	
	if (type == TypeOperator && value == 3)
	{
		type = GetNext(value);
		myExpF();
		myExpT1();
		//return;
	}
	else if (type == TypeOperator && value == 4)
	{
		type = GetNext(value);
		myExpF();
		myExpT1();
	}
}
void myExpF()
{
	if (type == TypeBoundrary && value == 3)   // (
	{
		type = GetNext(value);
		myExpE();
		// )
		if (type == TypeBoundrary && value == 4)  // )
		{
			type = GetNext(value);
		}
		else
		{
			out2 << "erro: line " << line << ", ��ֵ����ȱ��')'" << endl;
		}
	}
	else if (type == TypeIdentifier || type == TypeConstant)
	{
		type = GetNext(value);
	}
	else
	{
		out2 << "erro: line " << line << ", ���ʽ����" << endl;
	}
}
//*************************���������ʽ************************************


void myCondition()
{
	type = GetNext(value);
	if ((type == TypeKeyWord && value == 9) || (type == TypeKeyWord && value == 10)) // Ϊ true false
	{
		return;
	}
	else if (type == TypeOperator && value == 6)     //Ϊ !
	{
		type = GetNext(value);
		myExpE();
	}
	else
	{
		myExpE();

		if (type == TypeOperator && (value == 8 || value == 9 || (value >= 17 && value <= 22)))
		{
			type = GetNext(value);
			myExpE();
		}
		else
		{
			return;
		}
	}
}


void myWhile()
{
	type = GetNext(value);
	if (type == TypeBoundrary && value == 3)  // (
	{
		myCondition();
		if (type == TypeBoundrary && value == 4)  // )
		{
			type = GetNext(value);
			if (type == TypeBoundrary && value == 5)  // {
			{
				type = GetNext(value);
				myFunctionBody();
				if (type == TypeBoundrary && value == 6)   //  }
				{
					return;
				}
				else
				{
					out2 << "erro: line " << line << "��while���ȱ'}'" << endl;
				}
			}
			else
			{
				out2 << "erro: line " << line << "��while���ȱ'{'" << endl;
			}
		}
		else
		{
			out2 << "erro: line " << line << "��while���ȱ')'" << endl;
		}
	}
	else
	{
		out2 << "erro: line " << line << "��while���ȱ'('" << endl;
	}
}

void myDeclare(int typetmp, int valuetmp)
{
	type = GetNext(value);
	if (type == 0) //��δ�����ʶ��
	{
		IDTable[identifierCount].idType = valuetmp;
		IDTable[identifierCount].idName = identifier;
		identifierCount+=1;

		type = GetNext(value);
		// = 
		if (type == 2 && value == 7)  //Ϊ =
		{
			type = GetNext(value);
			myExpE();
			if (type == TypeBoundrary && value == 2)
			{
				return;
			}
			else if (type == TypeBoundrary && value == 1)      //Ϊ ,
			{
				myDeclare(type, value);
				return;
			}
			else
			{
				out2 << "erro: line " << line << ", ��ֵ����" << endl;
			}
			return;
		}
		// ;
		else if (type == TypeBoundrary && value == 2)  //Ϊ ;
		{
			return;
		}
		// ,
		else if (type == TypeBoundrary && value == 1)      //Ϊ ,
		{
			myDeclare(type, value);
		}
		else
		{
			out2 << "erro: line " << line << ", �������" << endl;
		}
	}
	else
	{
		out2 << "erro: line " << line << ", �������󣨱�ʶ���Ѷ��壩" << endl;
	}
}

void myAssignmentStmt()     //��ֵ
{
	type = GetNext(value);
	if (type == TypeOperator &&
		(value == 7 || value == 12 || value == 13 || value == 14 || value == 15 || value == 16))
	{
		type = GetNext(value);
		if (type == 1 && (value == 9 || value == 10))
		{
			type = GetNext(value);
			
		}
		else
		{
			myExpE();
		}
		if (type = TypeBoundrary && value == 2)
		{
			return;
		}
		else
		{
			out2 << "erro: line " << line << ", ��ֵ����ȱ��';'" << endl;
		}
	}
	else
	{
		out2 << "erro: line " << line << ", ��ֵ����" << endl;
	}
}

void myIf()
{
	type = GetNext(value);
	if (type == TypeBoundrary && value == 3)  // (
	{
		myCondition();
		if (type == TypeBoundrary && value == 4)  // )
		{
			type = GetNext(value);
			if (type == TypeBoundrary && value == 5)  // {
			{
				type = GetNext(value);
				myFunctionBody();
				if (type == TypeBoundrary && value == 6)  // }
				{
					type = GetNext(value);
					if (type == TypeKeyWord && value == 5)   // else
					{
						myElse();
					}
					else
					{
						myFunctionStmt();
					}
				}
				else
				{
					out2 << "erro: line " << line << "��if���ȱ'}'" << endl;
				}
			}
			else
			{
				out2 << "erro: line " << line << "��if���ȱ'{'" << endl;
			}
		}
		else
		{
			out2 << "erro: line " << line << "��if���ȱ')'" << endl;
		}
	}
	else
	{
		out2 << "erro: line " << line << "��if���ȱ'('" << endl;
	}
}

void myElse()
{
	type = GetNext(value);
	if (type == TypeKeyWord && value == 4)
	{
		myIf();
		return;
	}
	else if (type == TypeBoundrary && value == 5)
	{
		type = GetNext(value);
		myFunctionBody();
		if (type == TypeBoundrary && value == 6)
		{
			return;
		}
		else
		{
			out2 << "erro: line " << line << "��else���ȱ'}'" << endl;
		}
	}
	else
	{
		out2 << "erro: line " << line << "��else���ȱ'{'" << endl;
	}
}

void myFunctionStmt()      //����ʱΪ��ǰ�ж��ַ�
{
	if (type == TypeKeyWord && (value == 1 || value == 8))
	{
		myDeclare(type, value);
		return;
	}
	else if (type == TypeIdentifier && (value == 1 || value == 8))
	{
		myAssignmentStmt();
		return;
	}
	else if (type == TypeKeyWord && value == 4) //if stmt
	{
		myIf();
		return;
	}
	else if (type == TypeKeyWord && value == 6)  // while stmt
	{
		myWhile();
		return;
	}
	else if (type == TypeKeyWord && value == 10) // return stmt
	{
		//myReturn();
		return;
	}
	else if (type == TypeBoundrary && value == 2)
	{
		return;
	}
	else if (type == 0)
	{
		out2 << "erro: line " << line << "��δ�����ʶ��" << endl;
	}
	else
	{
		//return;
		out2 << "erro: line " << line << "��������δʶ���Ӧ������ʽ��" << endl;
	}
}
void myFunctionBody()  //����ʱΪ��ǰ�ж��ַ�
{
	while (!(type == 3 && value == 6))  ///������û����
	{
		myFunctionStmt();
		type = GetNext(value);
	}
}

void parse()
{
	type = GetNext(value);
	while (-1 != type)
	{
		
		myFunctionStmt();
		type = GetNext(value);
	}
	
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
	temp[num] = '\0';
	in.close();

	string flie2 = "ParsingResult.txt";
	
	out2.open(flie2, ios::out);
	if (!out2.is_open())
	{
		return NULL;
	}
	preTreatment(temp);
	parse();

	out2.close();

	return 0;
}
