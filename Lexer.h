#ifndef LEXER_H
#define LEXER_H
#include <fstream>
#include <vector>
#include <iostream>
#include "stdafx.h"

class Lexer
{
public:
  enum TransitionType
  {
      IDENTIFIER,
      INTEGER,
      REAL,
      UNKNOWN
  };

  // State table
  int stateTable[8][4] = {{1, 4, 7, 7},
                          {2, 3, 7, 7}, //ACCEPTABLE ID
                          {2, 3, 7, 7}, //ACCEPTABLE ID
                          {2, 3, 7, 7},
                          {7, 4, 5, 7}, //ACCEPTABLE INT
                          {7, 6, 7, 7},
                          {7, 6, 7, 7}, //ACCEPTABLE REAL
                          {7, 7, 7, 7}};

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
  std::ifstream *_fin;
};

#endif // LEXER_H