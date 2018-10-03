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
        SPACE,
        UNKNOWN
    };

    // State table
     int dfsm[6][6] = {{0, INTEGER, REAL, IDENTIFIER, SPACE, UNKNOWN},
                     {INTEGER, INTEGER, REAL, REJECT, REJECT, REJECT},
                     {REAL, REAL, REJECT, REJECT, REJECT, REJECT},
                     {IDENTIFIER, REJECT, REJECT, IDENTIFIER, REJECT, REJECT},
                     {SPACE, INTEGER, REJECT, IDENTIFIER, SPACE, REJECT},
                     {UNKNOWN, REJECT, REJECT, REJECT, REJECT, REJECT}};

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