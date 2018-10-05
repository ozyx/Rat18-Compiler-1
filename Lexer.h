#ifndef LEXER_H
#define LEXER_H

#include <fstream>
#include <vector>
#include <iostream>
#include "stdafx.h"

class Lexer
{
public:

  enum State
  {
    NS = 0, // NULL STATE
    S1 = 1,
    S2 = 2, //ACCEPTABLE ID
    S3 = 3, //ACCEPTABLE ID
    S4 = 4,
    S5 = 5, //ACCEPTABLE INT
    S6 = 6,
    S7 = 7 //ACCEPTABLE REAL
  };

  enum TransitionType
  {
    IDENTIFIER,
    INTEGER,
    REAL,
    UNKNOWN
  };

  // State table
  int stateTable[8][4] = {{S1, S4, S7, NS},
                          {S2, S3, S7, S7}, // ACCEPTABLE ID
                          {S2, S3, S7, S7}, // ACCEPTABLE ID
                          {S2, S3, S7, S7},
                          {S7, S4, S5, S7}, // ACCEPTABLE INT
                          {S7, S6, S7, S7},
                          {S7, S6, S7, S7}, // ACCEPTABLE REAL
                          {S7, S7, S7, S7}};

  struct Token
  {
    Token(std::string token, std::string lexeme)
    {
      this->token = token;
      this->lexeme = lexeme;
    }

    std::string token;
    std::string lexeme;
  };

  // Constructor
  Lexer();

  // Destructor
  ~Lexer();

  std::vector<Token> lex(std::ifstream &fin);

private:
  int getTransition(char tokenChar);

  std::string stateToString(int state);

  bool isValidOperator(char c);

  bool isValidSeparator(char c);
};

#endif // LEXER_H