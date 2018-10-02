#ifndef LEXER_H
#define LEXER_H
#include <fstream>
#include <vector>
#include "stdafx.h"
struct Token
{
  std::string token;
  int lexeme;
  std::string lexemeName;
};

class Lexer
{
public:
  // Constructor
  Lexer();

  // Destructor
  ~Lexer();

  std::vector<Token> lex(std::string expression);

private:
  Rat18::TransitionType getTransition(char tokenChar);

  std::string stateToString(int state);
};

#endif // LEXER_H