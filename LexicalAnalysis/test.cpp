#include <fstream>  
#include <cassert>
#include<iostream>
using namespace std;

#define MaxPerToken 25

const char* keyWords[] = { "int", "void", "main", "if", "else", "while", "for", "bool" , "true", "false", "return" };
char boundaries[] = { ',', ';', '(', ')', '{', '}' };
char operators[] = { '+', '-', '*', '/', '%', '!', '=', '<', '>', '&', '|' };

int isDigit(char temp)
{
	if (temp >= '0' && temp <= '9')
	{
		return 1;
	}
	else
	{
		return 0;
	}
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
	
	for (int i = 0; i < sizeof(keyWords)/sizeof(keyWords[0]); i++)
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
	for (int i = 0; i < sizeof(boundaries)/sizeof(boundaries[0]); i++)
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
	for (int i = 0; i < sizeof(operators)/sizeof(operators[0]); i++)
	{
		if (temp == operators[i])
		{
			return (01 + i);
		}
	}
	return 0;
}

int isIdentifier(char* identifiers[], char temp[])
{
	for (int i = 0; i < sizeof(identifiers)/sizeof(identifiers[0]); i++)  
	{
		if (identifiers[i] == temp)
		{
			return 1;
		}
	}
	return 0;
}

void preTreatment(char temp[], int num, char buff[])
{
	int j = 0;

	for (int i = 0; i < num; i++)
	{

		if (temp[i] == '\n' || temp[i] == '\t' || temp[i] == '\r')
		{
			continue;
		}
		if (temp[i] == ' ')
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
			}
			else if (temp[i] == '*')
			{
				i++;
				while (!(temp[i] == '*' && temp[i + 1] == '/'))
				{
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

void lexicalAnalysis(char buff[])
{
	char token[MaxPerToken];
	int j = 0;
	char* identifiers[MaxPerToken];

	ofstream out("output.text");
	if (!out.is_open())
	{
		cout << "Error opening file" << endl;
		exit(0);
	}

	for (int i = 0; buff[i] != '#'; i++)
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

		if (isCharacter(buff[i]))
		{
			int m = i + 1;
			int n = i + 1;
			int p = i + 1;
			int q = i + 1;
			if ((buff[m] == '+' && buff[++m] == '+'))
			{
				out << "(4, " << buff[i] << ")" << endl;
				out << "(2, $1$1)" << endl;
				i = i + 2;
			}
			else if (buff[n] == '-' && buff[++n] == '-') 
			{
				out << "(4, " << buff[i] << ")" << endl;
				out << "(2, $2$2)" << endl;
				i = i + 2;
			}
			else if (isOperator(buff[p]) || isBoundary(buff[p]))
			{
				out << "(4, " << buff[i] << ")" << endl;
			}
			else
			{
				while (isCharacter(buff[i]) || isDigit(buff[i]) || buff[i] == '_')
				{
					token[j++] = buff[i++];
				}
				token[j] = '\0';
				i--;
				int key = isKeyWord(token);
				if (key)
				{
					out << "(1, $" << key << ")" << endl;
				}
				else
				{
					out << "(4, " << token << ")" << endl;
				}
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
			int key = isKeyWord(token);
			if (key)
			{
				//string str = token;
				out << "(1, $" << key << ")" << endl;
			}
			else
			{
				out << "(4, " << token << ")" << endl;
			}
			/*else if (isIdentifier(identifiers, token))
			{
				print("Identifier", token);
				out << "(Identifier, " << token << ")" << endl;
			*/
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
					out << "(6, " << token << ")" << endl;
					break;
				}
			}
			if (flag)
			{
				out << "(5, " << token << ")" << endl;
			}
		}
		else if (int key1 = isOperator(buff[i]))
		{
			int j = i;
			int key2 = isOperator(buff[++j]);
			if ((key2 == 7 && key1 <= 9 && key1 >= 1)
				|| (key1 == 10 && key2 == 10) || (key1 == 11 && key2 == 11)
				|| (key1 == 1 && key2 == 1) || (key1 == 2 && key2 == 2))
			{
				out << "(2, $" << key1 << "$" << key2 << ")" << endl;
			}
			else
			{
				out << "(2, $" << key1 << ")" << endl;
			}

		}
		else if (int key = isBoundary(buff[i]))
		{
			//token[j] = buff[i];
			out << "(3, $" << key << ")" << endl;
		}
		else
		{
			token[j] = buff[i];
			out << "(6, " << token << ")" << endl;
		}
	}
	out.close();
}

void outTables()
{
	ofstream out1("CodeTable.text");
	if (!out1.is_open())
	{
		cout << "Error opening file" << endl;
		exit(0);
	}

	out1 << "**************************关键字*****************************"<< endl;
	out1 << "CLASS\t\t" << "NAME\t\t" << "VALUE" << endl;
	for (int i = 0; i < sizeof(keyWords) / sizeof(keyWords[0]); i++)
	{
		out1 << "1\t\t" << keyWords[i] << "\t\t$" << i + 1 << endl;
	}
	out1 << "*******************************************************\n\n" << endl;


	out1 << "**************************运算符*****************************" << endl;
	out1 << "CLASS\t\t" << "NAME\t\t" << "VALUE" << endl;
	for (int i = 0; i < sizeof(operators) / sizeof(operators[0]); i++)
	{
		out1 << "2\t\t" << operators[i] << "\t\t$" << i + 1 << endl;
	}
	out1 << "*******************************************************\n\n" << endl;


	out1 << "**************************界限符*****************************" << endl;
	out1 << "CLASS\t\t" << "NAME\t\t" << "VALUE" << endl;
	for (int i = 0; i < sizeof(boundaries) / sizeof(boundaries[0]); i++)
	{
		out1 << "3\t\t" << boundaries[i] << "\t\t$" << i + 1 << endl;
	}
	out1 << "*******************************************************\n\n" << endl;

	out1 << "**************************其他*****************************" << endl;
	out1 << "CLASS\t\t" << "NAME\t\t" << "VALUE" << endl;
	out1 << "4\t\t" << "标识符\t\t" << "字符串值" << endl;
	out1 << "5\t\t" << "整数\t\t" << "数值" << endl;
	out1 << "6\t\t" << "出错\t\t" << "出粗字符串值" << endl;
	out1 << "*******************************************************\n\n" << endl;
}

int main() 
{
	int num = 0;
	char temp[1000];
	char buff[1000];

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

	preTreatment(temp, num, buff);
	lexicalAnalysis(buff);
	outTables();

	return 0;
}