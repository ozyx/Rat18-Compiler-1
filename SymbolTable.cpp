#include "SymbolTable.h"

/**
 * @brief Construct a new SymbolTable object
 */
SymbolTable::SymbolTable() : memaddress(5000) {}

/**
 * @brief Destroy the SymbolTable object
 */
SymbolTable::~SymbolTable()
{
	instr_address = 1;
}

/**
 * @brief Increment the current memory address by one
 */
void SymbolTable::incrementMem()
{
	++this->memaddress;
}

/**
 * @brief Insert a Symbol into the symbol table
 *
 * @param t The token object
 * @return true if successful
 * @return false if unsuccessful
 */
bool SymbolTable::insert(Lexer::Token t)
{
	bool success = false;

	if (!lookup(t))
	{
		Symbol *s = new Symbol(t, this->memaddress);
		this->table.push_back(*s);
		incrementMem();
		success = true;
	}

	return success;
}

/**
 * @brief Check to see if an identifier already exists
 * in the symbol table.
 *
 * @param id The identifier (lexeme)
 * @return address if existing, 0 if not
 */
int SymbolTable::lookup(Lexer::Token t)
{
	std::vector<Symbol>::iterator it = this->table.begin();
	bool found = false;

	while (!found && it != this->table.end())
	{
		if (it->token.lexeme == t.lexeme && it->token.token == t.token)
		{
			found = true;
		}
		else
		{
			++it;
		}
	}

	return found ? it->address : 0;
}

/**
 * @brief Remove a Symbol from the symbol table.
 *
 * @param id The identifier (lexeme)
 * @return true if successful
 * @return false if unsuccessful
 */
bool SymbolTable::remove(Lexer::Token t)
{
	bool success = false;
	int pos = 0;

	if (lookup(t))
	{
		std::vector<Symbol>::const_iterator it = this->table.begin();
		while (!success && it != this->table.end())
		{
			if (it->token.lexeme == t.lexeme)
			{
				this->table.erase(this->table.begin() + pos);
				success = true;
			}
			else
			{
				++pos;
				++it;
			}
		}
	}

	return success;
}

/**
 * @brief Return a string representation of the
 * current symbol table.
 *
 * @return std::string
 */
std::string SymbolTable::list()
{
	std::ostringstream os;
	const int COL_WIDTH = 15;

	os << std::left << std::setw(COL_WIDTH) << "Identifier" << std::setw(COL_WIDTH) << "Type" << "Memory Address" << std::endl;

	for (std::vector<Symbol>::const_iterator it = this->table.begin(); it != this->table.end(); ++it)
	{
		os << std::setw(COL_WIDTH) << it->token.lexeme << std::setw(COL_WIDTH) << it->token.token << it->address << std::endl;
	}

	return os.str();
}

/**
 * @brief List all of the instructions
 * in the instruction table
 * 
 * @return std::string A formatted list of instructions
 */
std::string SymbolTable::list_instr()
{
	std::ostringstream os;
	const int COL_WIDTH = 15;

	os << std::left << std::setw(COL_WIDTH) << "address" << std::setw(COL_WIDTH) << "op" << "operand" << std::endl;

	for (std::vector<Instr>::const_iterator it = this->instructions.begin(); it != this->instructions.end(); ++it)
	{
		os << std::setw(COL_WIDTH) << it->address << std::setw(COL_WIDTH) << it->op;

		if (it->operand != NIL)
		{
			os << it->operand;
		}

		os << std::endl;
	}

	return os.str();
}

/**
 * @brief Generate a new instruction and
 * add it to the instruction table.
 * 
 * @param op The op (ADD, SUB, EQU, etc...)
 * @param operand The operand (an integer, memory address, etc...)
 */
void SymbolTable::gen_instr(std::string op, int operand)
{
	Instr *instr = new Instr(op, operand);

	this->instructions.push_back(*instr);
}

/**
 * @brief Push an address onto the jumpstack
 * 
 * @param address the address
 */
void SymbolTable::push_jumpstack(int address)
{
	this->jumpstack.push_back(address);
}

/**
 * @brief Used to close off JUMP instructions.
 * Find the previous JUMP instruction and fill out its
 * operand with the current instruction address.
 * 
 * @param jump_addr The address of the previous JUMP instruction
 */
void SymbolTable::back_patch(int jump_addr)
{
	const int addr = jumpstack.back();
	jumpstack.pop_back();

	if (this->instructions.size() >= addr)
	{
		this->instructions.at(addr - 1).operand = jump_addr;
	}
	else
	{
		// TODO: ERROR
	}
}

/**
 * @brief Get a particular token's memory address (e.g: 500, 5001...)
 * 
 * @param token the token
 * @return int the token's address. 0 if unsuccessful.
 */
int SymbolTable::get_address(Lexer::Token token)
{
	return lookup(token);
}

/**
 * @brief Get the current memory address (e.g: 5000, 5001...)
 * 
 * @return int the memory address
 */
int SymbolTable::get_mem()
{
	return this->memaddress;
}

/**
 * @brief Get the current instruction address (i.e: 1, 2, 3...)
 * 
 * @return int The instruction address
 */
int SymbolTable::get_instr_address() const
{
	return instr_address;
}
