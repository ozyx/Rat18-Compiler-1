#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "Lexer.h"
#include <string>
#include <vector>

class SymbolTable
{
public:
  struct Symbol
  {
    Lexer::Token token;
    int address;

    Symbol(Lexer::Token t, int address) : token(t), address(address) {}
  };

  SymbolTable();

  ~SymbolTable();

  // Accessors
  bool lookup(std::string id);

  std::string list();

  // Mutators
  bool insert(Lexer::Token t);

  bool remove(std::string id);

private:
  void incrementMem();

  std::vector<Symbol> table;
  int memaddress;
};

#endif // SYMBOLTABLE_H