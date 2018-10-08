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
    S12     // TERMINATING
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
  int stateTable[13][7] = {{S1,  S4,  S12, S7,  S9,  S10, S12},  // INITIAL STATE
                           {S2,  S3,  S12, S12, S12, S12, S12},  // ACCEPTABLE ID
                           {S2,  S3,  S12, S12, S12, S12, S12},  // ACCEPTABLE ID
                           {S2,  S3,  S12, S12, S12, S12, S12},
                           {S12, S4,  S5,  S12, S12, S12, S12},  // ACCEPTABLE INT
                           {S12, S6,  S12, S12, S12, S12, S12},
                           {S12, S6,  S12, S12, S12, S12, S12},  // ACCEPTABLE REAL
                           {S12, S12, S12, S8,  S12, S12, S12},  // ACCEPTABLE 1-OP
                           {S12, S12, S12, S12, S12, S12, S12},  // ACCEPTABLE 2-OP
                           {S12, S12, S12, S12, S12, S12, S12},  // ACCEPTABLE SEPARATOR
                           {S12, S12, S12, S12, S12, S11, S12},
                           {S12, S12, S12, S12, S12, S12, S12},  // ACCEPTABLE '$$'
                           {S12, S12, S12, S12, S12, S12, S12}}; // TERMINATING

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