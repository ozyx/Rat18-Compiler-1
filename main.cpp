
#include "Lexer.h"
#include <iostream>
#include <fstream>
#include <iomanip>

int main()
{
    std::ifstream fin;                // Input file stream
    std::string inFile;               // Input file name
    std::vector<Lexer::Token> tokens; // List of tokens
    std::stringstream *buffer;
    std::string line;

    // Get file name from user
    std::cout << "Which file would you like to open?: ";
    std::getline(std::cin, inFile);

    // Open the file
    fin.open(inFile.c_str());

    if (!fin)
    {
        std::cout << "input file did not open" << std::endl;
        exit(1);
    }

    // File has opened, instantiate the lexer.
    Lexer *lexer = new Lexer();

    // Output header
    std::cout << std::left;
    std::cout << std::setw(11) << "LEXEME"
              << "TOKEN" << std::endl;

    while (getline(fin, line))
    {
        buffer = new std::stringstream(line);
        tokens = lexer->lex(*buffer);

        // Output token list
        for (std::vector<Lexer::Token>::const_iterator it = tokens.begin(); it != tokens.end(); ++it)
        {
            std::cout << std::setw(11) << (*it).lexeme << (*it).token << std::endl;
        }
    }

    return 0;
}