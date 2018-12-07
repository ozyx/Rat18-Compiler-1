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
	instr_address = 0;
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

	if (!lookup(t.lexeme))
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
int SymbolTable::lookup(std::string id)
{
	std::vector<Symbol>::iterator it = this->table.begin();
	bool found = false;

	while (!found && it != this->table.end())
	{
		if (it->token.lexeme == id)
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
bool SymbolTable::remove(std::string id)
{
	bool success = false;
	int pos = 0;

	if (lookup(id))
	{
		std::vector<Symbol>::const_iterator it = this->table.begin();
		while (!success && it != this->table.end())
		{
			if (it->token.lexeme == id)
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

		if (it->operand == NIL)
		{
			os << "NIL";
		}
		else
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
 * @brief Get a particular token's memory address
 * 
 * @param token the token
 * @return int the token's address. 0 if unsuccessful.
 */
int SymbolTable::get_address(Lexer::Token token)
{
	return lookup(token.lexeme);
}

/**
 * @brief Get the current memory address
 * 
 * @return int the memory address
 */
int SymbolTable::get_mem()
{
	return this->memaddress;
}
