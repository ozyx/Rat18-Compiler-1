#ifndef LEXER_H
#define LEXER_H
#include <fstream>
#include <vector>
#include "stdafx.h"

class Lexer
{
public:

      enum TransitionType
    {
        REJECT = 0,
        INTEGER,
        REAL,
        IDENTIFIER,
        UNKNOWN
    };

    // State table
     int dfsm[5][5] = {{0, INTEGER, REAL, IDENTIFIER, UNKNOWN},
                     {INTEGER, INTEGER, REAL, REJECT, REJECT},
                     {REAL, REAL, REJECT, REJECT, REJECT},
                     {IDENTIFIER, REJECT, REJECT, IDENTIFIER, REJECT},
                     {UNKNOWN, REJECT, REJECT, REJECT, REJECT}};

    struct Token
    {
        std::string token;
        int lexeme;
        std::string lexemeName;
    };

  // Constructor
  Lexer();

  // Destructor
  ~Lexer();

  std::vector<Token> lex(std::string expression);

private:
  TransitionType getTransition(char tokenChar);

  std::string stateToString(int state);
};

#endif // LEXER_H