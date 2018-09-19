#ifndef LEXER_H
#define LEXER_H

#include <fstream>
#include <map>
#include <iostream>

class Lexer
{
  public:

    enum State
    {
        SPACE = 0,
        SEPARATOR = 1,
        OPERATOR = 2,
        BOOLEAN = 3,
        NUMBER = 4,
        REAL = 5,
        KEYWORD = 6,
        IDENTIFIER = 7
    };

    // Constructor
    Lexer();

    // Destructor
    ~Lexer();

    std::string stateToString(State state);

    std::map<char *, State> *lex(std::ifstream &fin);

    void printLex(std::map<char*, Lexer::State> lexRat);

  private:

  bool isKeyword(char *word);
};

#endif // LEXER_H