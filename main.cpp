// CPSC 323 Lexer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <string>
#include "header.h"

using namespace std;

map<char *, State> *lexer(ifstream &Rat18);
bool isKeyword(char *word);
void printLex(vector<string, char *> lexRat);

int main()
{
    ifstream fin;
    fin.open("asdsad");
    if (!fin)
    {
        cout << "Couldn't open file =(\n";

        // Something is wrong -- exit code 1
        exit(1);
    }

    map<char*, State> *lexedData;

    lexedData = lexer(fin);
    if (lexedData)
        cout << "success!\n";

    return 0;
}

map<char *, State> *lexer(ifstream &Rat18)
{
    map<char *, State> *lex;

    char unit;

    char *word;
    int length = 0;

    State prevstate = SPACE; // previous state
    State curstate = SPACE;  // current space

    while (!Rat18.eof())
    {
        Rat18.get(unit);
        if (unit == '\n')
            continue; // skip tests - ignor new line
        if (unit == '\0')
            continue; // skip tests - ignor string terminator
        if (unit == ' ' && prevstate == SPACE)
            continue; // skip test - ignore more than one space
        //space
        if (unit == ' ')
            curstate = SPACE;
        //Separator
        else if (unit == '(' | unit == ')' | unit == '{' | unit == '}')
            curstate = SEPARATOR;
        //operator
        else if (unit == '+' | unit == '-' | unit == '/' | unit == '*' | unit == '%')
            curstate = OPERATOR;
        //boolean op
        else if ((unit == '=' | unit == '<' | unit == '>') && prevstate == OPERATOR)
            curstate = BOOLEAN;
        //number
        else if (unit > 47 && unit < 58 && prevstate != IDENTIFIER)
            curstate = NUMBER;
        //real
        else if (unit == '.' && prevstate == NUMBER)
            curstate = REAL;
        // word           uppercase                       lowercase                     digit             did not come from num   did not come from real
        else if (((int)unit > 64 && unit < 91) | (unit > 96 && unit < 123) | (unit > 47 && unit < 58) && prevstate != NUMBER && prevstate != REAL)
            curstate = IDENTIFIER;
        // breaks grammar
        else
        {
            return nullptr;
        }

        //build word
        if (curstate == IDENTIFIER)
        {
            *word = unit;
            word++;
            length++;
        }
        //build num or real
        if (curstate == NUMBER)
        {
        }
        //build separator
        if (curstate == SEPARATOR)
        {
            word = &unit;
            lex->insert({word, SEPARATOR});
        }
        // add token and lexeme to lex vector
        if (curstate == SPACE && prevstate == IDENTIFIER)
        {
            word = nullptr;
            for (int i = length; length > 0; i--)
            {
                word--;
            }
            if (isKeyword(word))
            {
                lex->insert({word, KEYWORD});
            }
            else
                lex->insert({word, IDENTIFIER});
        }

        prevstate = curstate;
    }

    return lex;
}
// step through vector and print
void printLex(map<char*, State> lexRat)
{
    cout << "Output:\n";
    cout << "token\tlexeme\n";
    cout << "___________________________\n";

    map<char*, State>::iterator it = lexRat.begin();

    while(it != lexRat.end())
    {
        cout << (*it).first << "\t" << stateToString((*it).second) << "\n";
        ++it;
    }
}

// check if word is keyword or ID
bool isKeyword(char *word)
{
    return strcmp(word, "if") == 0 | strcmp(word, "ifend") == 0 | strcmp(word, "while") == 0
    | strcmp(word, "whileend") == 0 | strcmp(word, "else") == 0 | strcmp(word, "elseend") == 0
    | strcmp(word, "switch") == 0 | strcmp(word, "case") == 0 | strcmp(word, "break") == 0;
}
