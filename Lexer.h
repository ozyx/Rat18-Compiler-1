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

  std::unordered_set<std::string> keywords = {"while", "whileend", "int", "function", "if", "ifend", "return", "get", "put"};
  std::unordered_set<char> separators = {'(',')','{','}',',',':', ';'};
  std::unordered_set<char> operators = {'+','-','*','/','<','>','=','!'};
  std::unordered_set<std::string> double_operators = {"++", "--", "==", "!=", "<=", ">="};

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