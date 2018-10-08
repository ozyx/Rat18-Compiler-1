#ifndef LEXER_H
#define LEXER_H

#include <sstream>
#include <vector>
#include <iostream>
#include <unordered_set>
#include "stdafx.h"

class Lexer
{
public:
  enum State
  {
    NS = 0, // NULL STATE
    S1,     //ACCEPTABLE ID
    S2,     //ACCEPTABLE ID
    S3,
    S4,     //ACCEPTABLE INT
    S5,
    S6,     //ACCEPTABLE REAL
    S7,
    S8,
    S9,
    S10,
    S11,    // ACCEPTABLE '$$'
    TRM     // TERMINATING
  };

  enum TransitionType
  {
    IDENTIFIER = 0,
    INTEGER,
    REAL,
    OPERATOR,
    SEPARATOR,
    FUNC_SEPARATOR,
    REJECT
  };

  // State table
  int stateTable[13][7] = {{S1,  S4,  TRM, S7,  S9,  S10, TRM},  // INITIAL STATE
                           {S2,  S3,  TRM, TRM, TRM, TRM, TRM},  // ACCEPTABLE ID
                           {S2,  S3,  TRM, TRM, TRM, TRM, TRM},  // ACCEPTABLE ID
                           {S2,  S3,  TRM, TRM, TRM, TRM, TRM},
                           {TRM, S4,  S5,  TRM, TRM, TRM, TRM},  // ACCEPTABLE INT
                           {TRM, S6,  TRM, TRM, TRM, TRM, TRM},
                           {TRM, S6,  TRM, TRM, TRM, TRM, TRM},  // ACCEPTABLE REAL
                           {TRM, TRM, TRM, S8,  TRM, TRM, TRM},  // ACCEPTABLE 1-OP
                           {TRM, TRM, TRM, TRM, TRM, TRM, TRM},  // ACCEPTABLE 2-OP
                           {TRM, TRM, TRM, TRM, TRM, TRM, TRM},  // ACCEPTABLE SEPARATOR
                           {TRM, TRM, TRM, TRM, TRM, S11, TRM},
                           {TRM, TRM, TRM, TRM, TRM, TRM, TRM},  // ACCEPTABLE '$$'
                           {TRM, TRM, TRM, TRM, TRM, TRM, TRM}}; // TERMINATING

  std::unordered_set<std::string> keywords = {"while", "whileend", "int", "function", "if", "ifend", "return", "get", "put", "true", "false"};
  std::unordered_set<char> separators = {'(',')','{','}',',',':', ';'};
  std::unordered_set<char> operators = {'+','-','*','/','<','>','=','^'};
  std::unordered_set<std::string> double_operators = {"==", "^=", "=>", "=<"};

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

  std::vector<Token> lex(std::stringstream &buffer);

private:
  bool comment;

  int getTransition(char tokenChar) const;

  std::string stateToString(int state) const;

  // Use for single operators
  bool isValidOperator(char c) const;

  // Use for double operators
  bool isValidOperator(std::string s) const;

  bool isValidSeparator(char c) const;

  bool isKeyword(std::string token) const;
};

#endif // LEXER_H