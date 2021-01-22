#include "lexical.h"
#include <iostream>

vector<keyType> Key = {{"auto", 1}, {"break", 2}, {"case", 3}, {"char", 4}, {"const", 5}, {"continue", 6}, {"default", 7}, {"do", 8}, {"double", 9}, {"else", 10}, {"enum", 11}, {"extern", 12}, {"float", 13}, {"for", 14}, {"goto", 15}, {"if", 16}, {"int", 17}, {"long", 18}, {"register", 19}, {"return", 20}, {"short", 21}, {"signed", 22}, {"sizeof", 23}, {"static", 24}, {"struct", 25}, {"switch", 26}, {"typedef", 27}, {"union", 28}, {"unsigned", 29}, {"void", 30}, {"volatile", 31}, {"while", 32}};

vector<string> OperatorOrDelimiter = {
    "+", "-", "*", "/", "<", "<=", ">", ">=", "=", "==",
    "!=", ";", "(", ")", "^", ",", "\"", "\'", "#", "&",
    "&&", "|", "||", "%", "~", "<<", ">>", "[", "]", "{",
    "}", "\\", ".", "\?", ":", "!"};

char getChar(string &str)
{
	if (str.empty())
		return '\0';
	char res = str[0];
	str.erase(str.begin());
	return res;
}

char getBchar(string &str)
{
	if (str.empty())
	{
		cout << "error: buffer is empty!" << endl;
		return '\0';
	}
	int i = 0;
	int len = str.size();
	while (str[i] == '\0' || str[i] == '\n' || str[i] == ' ')
		i++;
	char ch = str[i];
	str.erase(str.begin(), str.begin() + i + 1);
	return ch;
}

void retract(string &str, char ch)
{
	str.insert(str.begin(), ch);
}

keyType ReturnWord(string &str)
{
	string strToken;
	int value;
	keyType tempkey;
	char ch = getBchar(str);
	// 标识符检测入口
	if (isalpha(ch))
	{
		strToken.push_back(ch);
		ch = getChar(str);
		while (isalpha(ch) || isdigit(ch))
		{
			strToken.push_back(ch);
			ch = getChar(str);
		}
		retract(str, ch);
		value = reserve(strToken, Key);
		tempkey.keyname = strToken;
		if (value == 0)
			tempkey.value = 71;
		else
			tempkey.value = Key[value - 1].value;
	}
	// 整数检测入口
	else if (isdigit(ch))
	{
		strToken.push_back(ch);
		ch = getChar(str);
		while (isdigit(ch))
		{
			strToken.push_back(ch);
			ch = getChar(str);
		}
		retract(str, ch);
		tempkey.keyname = strToken;
		tempkey.value = 72;
	}
	// 运算符和界限符检测入口
	else if (ch != '\0')
	{
		strToken.push_back(ch);
		tempkey.value = search(ch, str, strToken, OperatorOrDelimiter);
		tempkey.keyname = strToken;
	}
	else
		tempkey.value = -1;
	return tempkey;
}

int reserve(string &strToken, vector<keyType> &table)
{
	for (keyType x : table)
		if (strToken == x.keyname)
			return x.value;
	return 0;
}

int search(char ch, string &buffer, string &strToken, vector<string> &op)
{
	int syn;
	if (ch == '*' || ch == '/' || ch == ';' || ch == '(' || ch == ')' || ch == '^' || ch == ',' || ch == '\"' || ch == '\'' || ch == '~' || ch == '#' || ch == '%' || ch == '[' || ch == ']' || ch == '{' || ch == '}' || ch == '\\' || ch == '.' || ch == '\?' || ch == ':')
	{
		for (int i = 0; i < 36; i++)
		{
			if (strToken == OperatorOrDelimiter[i])
			{
				syn = 33 + i;
				return syn;
			}
		}
	}
	else if (ch == '<')
	{
		// < <= <<
		ch = getChar(buffer); //后移，超前搜索
		if (ch == '=')
		{
			strToken.push_back(ch);
			syn = 38;
		}
		else if (ch == '<')
		{ //左移
			strToken.push_back(ch);
			syn = 58;
		}
		else
		{
			retract(buffer, ch);
			syn = 37;
		}
		return syn;
	}
	else if (ch == '>')
	{ //>,>=,>>
		ch = getChar(buffer);
		if (ch == '=')
		{
			strToken.push_back(ch);
			syn = 40;
		}
		else if (ch == '>')
		{
			strToken.push_back(ch);
			syn = 59;
		}
		else
		{
			retract(buffer, ch);
			syn = 39;
		}
		return syn;
	}
	else if (ch == '=')
	{ //=.==
		ch = getChar(buffer);
		if (ch == '=')
		{
			strToken.push_back(ch);
			syn = 42;
		}
		else
		{
			retract(buffer, ch);
			syn = 41;
		}
		return syn;
	}
	else if (ch == '+')
	{ //+.++
		ch = getChar(buffer);
		if (ch == '+')
		{
			strToken.push_back(ch);
			syn = 69;
		}
		else
		{
			retract(buffer, ch);
			syn = 33;
		}
		return syn;
	}
	else if (ch == '-')
	{ //-.--
		ch = getChar(buffer);
		if (ch == '-')
		{
			strToken.push_back(ch);
			syn = 70;
		}
		else
		{
			retract(buffer, ch);
			syn = 34;
		}
		return syn;
	}
	else if (ch == '!')
	{ //!,!=
		ch = getChar(buffer);
		if (ch == '=')
		{
			strToken.push_back(ch);
			syn = 43;
		}
		else
		{
			syn = 68;
			retract(buffer, ch);
		}
		return syn;
	}
	else if (ch == '&')
	{ //&,&&
		ch = getChar(buffer);
		if (ch == '&')
		{
			strToken.push_back(ch);
			syn = 53;
		}
		else
		{
			retract(buffer, ch);
			syn = 52;
		}
		return syn;
	}
	else if (ch == '|')
	{ //|,||
		ch = getChar(buffer);
		if (ch == '|')
		{
			strToken.push_back(ch);
			syn = 55;
		}
		else
		{
			retract(buffer, ch);
			syn = 54;
		}
		return syn;
	}

	else
	{ //不能被以上词法分析识别，则出错。
		cout << "error：there is no \"" << ch << "\"" << endl;
		exit(0);
		return -1;
	}
}

void filterbuffer(string &str)
{
    string tempString;
    int len = str.size();
    for (int i = 0; i < len; i++)
    {
        // 去除单行注释
        if (str[i] == '/' && str[i + 1] == '/')
        {
            i += 2;
            while (str[i] != '\n')
                i++;
        }
        // 去除多行注释
        if (str[i] == '/' && str[i + 1] == '*')
        {
            i += 2;
            while (str[i] != '*' || str[i + 1] != '/')
                i++; //继续扫描
            i += 2;  //跨过“*/”
        }
        if (str[i] != '\n' && str[i] != '\t' && str[i] != '\v' && str[i] != '\r')
            tempString.push_back(str[i]);
    }
    str = tempString;
}