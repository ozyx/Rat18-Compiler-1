#include "SyntaxAnalyzer.h"

SyntaxAnalyzer::SyntaxAnalyzer(std::vector<Lexer::Token> tokens, bool print) : it(tokens.begin()), currentToken(*(it))
{
    this->print = print;
    this->tokens = tokens;
}

SyntaxAnalyzer::~SyntaxAnalyzer()
{
}

void SyntaxAnalyzer::getNextToken()
{
    // Increment iterator
    ++it;

    if (it == this->tokens.end())
    {
        // No more tokens!
        // Throw an error
    }

    this->currentToken = *(it);
}

void SyntaxAnalyzer::Rat18F()
{
    OptFunctionDefinitions();

    getNextToken();

    if (currentToken.lexeme == "$$")
    {
        OptDeclarationList();
        StatementList();
    }

    getNextToken();

    if (currentToken.lexeme != "$$")
    {
        // error?
    }
}

void SyntaxAnalyzer::OptFunctionDefinitions()
{
    getNextToken();

    if (currentToken.lexeme == "function")
    {
        FunctionDefinitions();
    }
    else
    {
        Empty();
    }
}

void SyntaxAnalyzer::OptDeclarationList()
{
    getNextToken();

    if (currentToken.lexeme == "real" | currentToken.lexeme == "boolean" | currentToken.lexeme == "int")
    {
        DeclarationList();
    }
    else
    {
        Empty();
    }
}

void SyntaxAnalyzer::DeclarationList()
{
    Declaration();

    getNextToken();

    if (currentToken.lexeme == ";")
    {
        getNextToken();
        if (currentToken.lexeme == "real" | currentToken.lexeme == "boolean" | currentToken.lexeme == "int")
        {
            DeclarationList();
        }
    }
}

void SyntaxAnalyzer::Declaration()
{
    Qualifier();
    if (currentToken.token == "Identifier")
    {
        IDs();
    }
}

void SyntaxAnalyzer::Qualifier()
{
    // print stuff
    getNextToken();
}

void SyntaxAnalyzer::IDs()
{
    Identifier();
    getNextToken();
    if (currentToken.lexeme == ",")
    {
        getNextToken();
        if (currentToken.token == "Identifier")
        {
            IDs();
        }
        else
        {
            // TODO: error
        }
    }
}

void SyntaxAnalyzer::Identifier()
{
    // TODO: print stuff
}

void SyntaxAnalyzer::StatementList()
{
}