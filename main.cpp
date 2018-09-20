#include <iostream>
#include <fstream>
#include "Lexer.h"


int main() {

	std::ifstream rat18;
	rat18.open("input.txt");
	if (!rat18)
	{
		std::cout << "Coult Not Open File\n";

		std::exit(0);
	}
	Lexer tom;

	tom.lex(rat18);
	tom.printLex;


	return 0;
}