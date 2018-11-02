
#include "Lexer.h"
#include <iostream>
#include <fstream>
#include <iomanip>

int main()
{
    const int COL_SIZE = 20;

    std::ifstream fin;                // Input file stream
    std::string inFile;               // Input file name
    std::vector<Lexer::Token> tokens; // List of tokens
    std::stringstream *buffer;
    std::string line;

    std::vector<std::string> files = {"testCase1.txt", "testCase2.txt", "testCase3.txt"};

    for (std::string file : files)
    {

        // Open the file
        fin.open(file.c_str());

        if (!fin)
        {
            std::cout << "input file did not open" << std::endl;
            exit(1);
        }

        std::cout << std::endl << "RUNNING TEST CASE FILE \"" << file << "\"" << std::endl << std::endl;

        // File has opened, instantiate the lexer.
        Lexer *lexer = new Lexer();

        // Output header
        std::cout << std::left;
        std::cout << std::setw(COL_SIZE) << "LEXEME"
                  << std::setw(COL_SIZE) << "TOKEN" 
                  << "LINE NUMBER" << std::endl;
        std::cout << std::setfill('-') << std::setw(COL_SIZE) << "-"
                  << std::setw(COL_SIZE) << '-'
                  << "-----" << std::setfill(' ') << std::endl;
        
        int lineNumber = 1;

        while (getline(fin, line))
        {
            buffer = new std::stringstream(line);
            tokens = lexer->lex(*buffer, lineNumber);

            // Output token list
            for (std::vector<Lexer::Token>::const_iterator it = tokens.begin(); it != tokens.end(); ++it)
            {
                std::cout << std::setw(COL_SIZE) << (*it).lexeme << std::setw(COL_SIZE) << (*it).token << (*it).lineNumber << std::endl;
            }

            lineNumber++;
        }
        fin.close();
    }

    std::cout << std::endl << "EXECUTION HAS COMPLETED." << std::endl;
    std::cout << "Press enter to continue. . ." << std::endl;
    // std::cin.get();

    return 0;
}