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
bool SymbolTable::insert(Lexer::Token t, std::string type)
{
    bool success = false;

    if (!lookup(t))
    {
        Symbol *s = new Symbol(t, this->memaddress, type);
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

    os << std::left << std::setw(COL_WIDTH) << "Identifier" << std::setw(COL_WIDTH) << "Type"
       << "Memory Address" << std::endl;
	os << std::setfill('-') << std::setw(COL_WIDTH * 2 + 14) << '-' << std::setfill(' ') << std::endl;

    for (std::vector<Symbol>::const_iterator it = this->table.begin(); it != this->table.end(); ++it)
    {
        os << std::setw(COL_WIDTH) << it->token.lexeme << std::setw(COL_WIDTH) << it->type << it->address << std::endl;
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

    os << std::left << std::setw(COL_WIDTH) << "Address" << std::setw(COL_WIDTH) << "OpCode"
       << "Operand" << std::endl;
	os << std::setfill('-') << std::setw(COL_WIDTH * 2 + 7) << '-' << std::setfill(' ') << std::endl;

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

/**
 * @brief Push a value onto the typestack
 * 
 * @param type the type
 */
void SymbolTable::push_typestack(std::string type)
{
    this->typestack.push(type);
}

/**
 * @brief Pop a value from the typestack
 * 
 * @return true if successful
 * @return false if stack is empty
 */
bool SymbolTable::pop_typestack()
{
    bool success = false;

    if(!this->typestack.empty())
    {
        success = true;
        this->typestack.pop();
    }

    return success;
}

/**
 * @brief Retrieve the top element on the stack
 * 
 * @return std::string the type
 */
std::string SymbolTable::top_typestack() const
{
    return this->typestack.top();
}

/**
 * @brief Get the type of the given Token
 * 
 * @param token 
 * @return std::string 
 */
std::string SymbolTable::get_type(Lexer::Token token) const
{
    std::string type = "";

    for(Symbol s : this->table)
    {
        if(s.token.lexeme == token.lexeme)
        {
            type = s.type;
            break;
        }
    }

	if (token.token == "Integer")
	{
		type = "int";
	}

    return type;
}

/**
 * @brief Check if typestack is empty
 * 
 * @return true if empty
 * @return false if not empty
 */
bool SymbolTable::typestack_empty() const
{
    return this->typestack.empty();
}