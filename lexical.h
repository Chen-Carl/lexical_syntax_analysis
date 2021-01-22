#ifndef LEXICAL_H
#define LEXICAL_H

#include <vector>
#include <string>

using namespace std;
struct keyType
{
    string keyname;
    int value;
};

char getChar(string &str);
char getBchar(string &str);
void retract(string &str, char ch);
keyType ReturnWord(string &str);
int reserve(string &strToken, vector<keyType> &table);
int search(char ch, string &buffer, string &str, vector<string> &op);
void filterbuffer(string &str);

#endif
