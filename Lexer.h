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
    S1 = 1, //ACCEPTABLE ID
    S2 = 2, //ACCEPTABLE ID
    S3 = 3,
    S4 = 4, //ACCEPTABLE INT
    S5 = 5,
    S6 = 6, //ACCEPTABLE REAL
    S7 = 7, //
    S8 = 8,
    S9 = 9,
    S10 = 10
  };

  enum TransitionType
  {
    IDENTIFIER,
    INTEGER,
    REAL,
    OPERATOR,
    SEPARATOR,
    REJECT
  };

  // State table
  int stateTable[11][6] = {{S1,  S4,  S10, S7,  S9,  S10},
                           {S2,  S3,  S10, S10, S10, S10},    // ACCEPTABLE ID
                           {S2,  S3,  S10, S10, S10, S10},    // ACCEPTABLE ID
                           {S2,  S3,  S10, S10, S10, S10},
                           {S10, S4,  S5,  S10, S10, S10},    // ACCEPTABLE INT
                           {S10, S6,  S10, S10, S10, S10},
                           {S10, S6,  S10, S10, S10, S10},   // ACCEPTABLE REAL
                           {S10, S10, S10, S8,  S10, S10},   // ACCEPTABLE 1-OP
                           {S10, S10, S10, S10, S10, S10},  // ACCEPTABLE 2-OP
                           {S10, S10, S10, S10, S10, S10},  // ACCEPTABLE SEPARATOR
                           {S10, S10, S10, S10, S10, S10}}; // TERMINATING

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

  bool isAccepting(int state) const;
};

#endif // LEXER_H