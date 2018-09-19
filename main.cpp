// CPSC 323 Lexer.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>
#include <map>
#include <fstream>
#include "Lexer.h"

using namespace std;

int main()
{
    Lexer lexer;
    ifstream fin;
    fin.open("asdsad");
    if (!fin)
    {
        cout << "Couldn't open file =(\n";

        // Something is wrong -- exit code 1
        exit(1);
    }

    map<char *, Lexer::State> *lexedData;

    lexedData = lexer.lex(fin);
    if (lexedData)
    {
        cout << "success!\n";
    }

    return 0;
}