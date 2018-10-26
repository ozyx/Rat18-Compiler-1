#include "SyntaxAnalyzer.h"

SyntaxAnalyzer::SyntaxAnalyzer(std::vector<Lexer::Token> tokens, bool print): it(tokens.begin()), currentToken(*(it))
{
    this->print = print;
    this->tokens = tokens;
}

SyntaxAnalyzer::~SyntaxAnalyzer()
{

}

void SyntaxAnalyzer::getNextToken()
{
    ++it;
    if(it == this->tokens.end())
    {
        // No more tokens!
    }

    this->currentToken = *(it);
}