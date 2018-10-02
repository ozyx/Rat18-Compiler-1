#include "Lexer.h"

Lexer::Lexer() {}

Lexer::~Lexer() {}

std::vector<Token> Lexer::lex(std::string expression)
{
    Token token;
    std::vector<Token> tokens;
    char current = ' ';
    int col = Rat18::REJECT;
    int currentState = Rat18::REJECT;
    int prevState = Rat18::REJECT;
    std::string currentToken = "";
    int i = 0;

    while (i < expression.length())
    {
        current = expression[i];

        col = getTransition(current);

        currentState = Rat18::dfsm[currentState][col];

        if (currentState == Rat18::REJECT)
        {
            if (prevState != Rat18::REJECT)
            {
                token.token = current;
                token.lexeme = prevState;
                token.lexemeName = stateToString(token.lexeme);
            }
        }
        else
        {
            currentToken += current;
            ++i;
        }
        prevState = currentState;
    }

    if (currentState != Rat18::REJECT && currentToken != "")
    {
        token.token = currentToken;
        token.lexeme = currentState;
        token.lexemeName = getTransition(token.lexeme);
        tokens.push_back(token);
    }

    return tokens;
}

Rat18::TransitionType Lexer::getTransition(char tokenChar)
{
    Rat18::TransitionType transition = Rat18::UNKNOWN;

    if (isspace(tokenChar))
    {
        transition = Rat18::REJECT;
    }
    else if (isdigit(tokenChar))
    {
        transition = Rat18::INTEGER;
    }
    else if (tokenChar == '.')
    {
        transition = Rat18::REAL;
    }
    else if (isalpha(tokenChar))
    {
        transition = Rat18::IDENTIFIER;
    }

    return transition;
}

std::string Lexer::stateToString(int state)
{
    std::string stateStr = "";

    switch (state)
    {
    case Rat18::INTEGER:
        stateStr = "Integer";
        break;
    case Rat18::REAL:
        stateStr = "Real";
        break;
    case Rat18::IDENTIFIER:
        stateStr = "Identifier";
        break;
    case Rat18::UNKNOWN:
        stateStr = "Unknown";
        break;
    case Rat18::REJECT:
        stateStr = "Reject";
        break;
    }

    return stateStr;
}