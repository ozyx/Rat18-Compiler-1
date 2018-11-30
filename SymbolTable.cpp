#include "SymbolTable.h"

/**
 * @brief Construct a new SymbolTable object
 */
SymbolTable::SymbolTable() : memaddress(5000) {}

/**
 * @brief Destroy the SymbolTable object
 */
SymbolTable::~SymbolTable() {}

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
 * @return true if already existing
 * @return false if not existing
 */
bool SymbolTable::lookup(std::string id)
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

    return found;
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

    if (lookup(id))
    {
        std::vector<Symbol>::const_iterator it = this->table.begin();
        while (!success && it != this->table.end())
        {
            if (it->token.lexeme == id)
            {
                this->table.erase(it);
                success = true;
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
std::string list()
{
    //TODO: Make this return a formatted string
}
