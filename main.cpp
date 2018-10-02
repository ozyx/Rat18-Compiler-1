// CPSC 323 Lexer.cpp : Defines the entry point for the console application.
//
#include "Lexer.h"
#include <iostream>

int main()
{

    Lexer lexer;
    // declare variables
    std::ifstream infile;
    std::string fileName = "";
    std::string expression = "";
    std::vector<Lexer::Token> tokens;

    // get data from user
    std::cout << "nPlease enter the name of the file: ";
    std::getline(std::cin, fileName);

    infile.open(fileName.c_str());

    if (infile.fail())
    {
        std::cout << "n** ERROR - the file " << fileName << " cannot be found!nn";
        exit(1);
    }

    // use a loop to scan each line in the file
    while (getline(infile, expression))
    {
        // use the "Lexer" function to isolate integer, real, operator,
        // string, and unknown tokens
        tokens = lexer.lex(expression);

        // display the tokens to the screen
        for (unsigned x = 0; x < tokens.size(); ++x)
        {
            std::cout << tokens[x].lexemeName << "\t"
                      << tokens[x].token << std::endl;
        }
    }

    infile.close();

    return 0;
}