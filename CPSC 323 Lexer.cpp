// CPSC 323 Lexer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <string>

using namespace std;


vector<string ,char*> *lexer(ifstream Rat18);
bool isKeyword(char* word);
void printLex(vector<string,char*> LexRat);




int main()
{
	ifstream fin;
	fin.open("asdsad");
	if (fin);
	else {
		cout << "Couldn't open file =(\n";
		exit(0);
	}

	vector<string,char*> *lexedData;

	lexedData = lexer(fin);
	if (lexedData) cout << "success!\n";

    return 0;
}

vector<string, char*> *lexer(ifstream Rat18) {
	vector<string,char*> *lex;
	char unit;

	char* thing;
	int length = 0;

	int prevstate = 0; // previous state
	int curstate = 0; // current space

	int space = 0;
	int separator = 1;
	int oper = 2;
	int boo = 3;
	int number = 4;
	int real = 5;
	int word = 6;


	while ( !Rat18.eof()) {
		Rat18.get(unit);
		if (unit == '\n') continue; // skip tests - ignor new line
		if (unit == '\0') continue; // skip tests - ignor string terminator
		if (unit == ' ' && prevstate = space) continue; // skip test - ignore more than one space
		//space
		if (unit == ' ') curstate = space;
		//Separator
		else if (unit == '(' | unit == ')' | unit == '{' | unit == '}')curstate = separator;
		//operator
		else if (unit == '+' | unit == '-' | unit == '/' | unit == '*' | unit == '%' ) curstate = oper;
		//boolean op
		else if ( (unit == '=' | unit == '<' | unit == '>') && prevstate = oper) curstate = boo;
		//number
		else if (unit > 47 && unit < 58 && prevstate != word) curstate = number;
		//real
		else if (unit == '.' && prevstate == number) curstate = real;
		// word           uppercase                       lowercase                     digit             did not come from num   did not come from real          
		else if (( (int)unit > 64 && unit < 91) | (unit > 96 && unit < 123) | (unit > 47 && unit < 58) && prevstate != number && prevstate != real) curstate = word;
		// breaks grammar
		else {
			return lex = nullptr;
		}

		//build word
		if (curstate = word) {
			*thing = unit;
			thing++;
		}
		//build num or real
		if (curstate = number) {

		}
		//build separator
		if (curstate = separator) {
			*lex->push_back("Separator", unit);
		}
		// add token and lexeme to lex vector
		if (curstate = space && prevstate == word) {
			thing = '\0';
			for (int i = length; length > 0; i--) thing--;
			if (isKeyword(thing)) {
				*lex->push_back("keyword", thing);
			}
			else *lex->push_back("ID", thing);
		}





		prevstate = curstate;
		}

	return lex;
			

	}
// step through vector and print
void printLex(vector<string, char*> LexRat) {

	cout << "Output:\n";
	cout << "token\tlexeme\n";
	cout << "___________________________\n";
	for (int i = 0; i < LexRat.size(); i++) {
		cout << LexRat.at(i) << "\t" << LexRat.at(i).at(i) << "\n";
	}

}

// check if word is keyword or ID
bool isKeyword(char* word) {

	if (strcmp(word, "if") == 0) return 1;
	else if (strcmp(word, "ifend") == 0) return 1;
	else if (strcmp(word, "while") == 0) return 1;
	else if (strcmp(word, "whileend") == 0) return 1;
	else if (strcmp(word, "else") == 0) return 1;
	else if (strcmp(word, "elseend") == 0) return 1;
	else if (strcmp(word, "switch") == 0) return 1;
	else if (strcmp(word, "case") == 0) return 1;
	else if (strcmp(word, "break") == 0) return 1;
	return 0;
}


