#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "Lexer.h"
#include "Globals.h"
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

class SymbolTable
{
public:
  struct Instr
  {
    int address;
    std::string op;
    int operand;

    Instr(std::string op, int operand) : address(instr_address++), op(op), operand(operand) {}
  };

  struct Symbol
  {
    Lexer::Token token;
    int address;

    Symbol(Lexer::Token t, int address) : token(t), address(address) {}
  };

  SymbolTable();

  ~SymbolTable();

  // Accessors
  int lookup(std::string id);

  std::string list();

  // Mutators
  bool insert(Lexer::Token t);

  bool remove(std::string id);

  int get_address(Lexer::Token token);

  void gen_instr(std::string op, int operand);

private:
  void incrementMem();

  std::vector<Symbol> table;
  std::vector<Instr> instructions;
  int memaddress;
};

#endif // SYMBOLTABLE_H