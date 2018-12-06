#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "Lexer.h"
#include "Globals.h"
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <assert.h>

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
  std::string list_instr();

  int get_address(Lexer::Token token);
  int get_mem();
  
  // Mutators
  bool insert(Lexer::Token t);

  bool remove(std::string id);

  void gen_instr(std::string op, int operand);
  void push_jumpstack(int address);
  void back_patch(int jump_addr)
  {
	  const int addr = jumpstack.back();
	  jumpstack.pop_back();

	  for (Instr instr : instructions)
	  {
		  if (instr.address == addr)
		  {
			  instr.operand = jump_addr;
			  return;
		  }
	  }
	  assert("SOMETHING WENT WRONG SymbolTable.h Line 63");
  }
private:
  void incrementMem();

  std::vector<Symbol> table;
  std::vector<Instr> instructions;
  std::vector<int> jumpstack;
  int memaddress;
};

#endif // SYMBOLTABLE_H