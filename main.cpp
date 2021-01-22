#include <iostream>
#include <fstream>
#include <cctype>
#include "lexical.h"

using namespace std;

extern const int N = 47;
extern int point;
extern char ch;
extern string strToken; //存放构成单词符号的字符串
extern string buffer; //字符缓冲区

#include <string>
#include <fstream>
using namespace std;

int main()
{
    ifstream in("test.txt", ios::in);
    istreambuf_iterator<char> beg(in), end;
    string buffer(beg, end);
    filterbuffer(buffer);
    in.close();

    ofstream out;
    keyType temp;
    while (buffer.size())
	{
        temp = ReturnWord(buffer);
        if (temp.value != -1)
            cout << "(" << temp.value << ", " << temp.keyname << ")" << endl;
	}
	cout << "The End." << endl;
    return 0;
}