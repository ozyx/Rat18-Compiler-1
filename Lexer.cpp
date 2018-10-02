#include "Lexer.h"

Lexer::Lexer() {}

Lexer::~Lexer() {}

std::vector<Lexer::Token> Lexer::lex(std::string expression)
{
    Token token;
    std::vector<Token> tokens;
    char current = ' ';
    int col = REJECT;
    int currentState = REJECT;
    int prevState = REJECT;
    std::string currentToken = "";
    int i = 0;

    while (i < expression.length())
    {
        current = expression[i];

        col = getTransition(current);

        currentState = dfsm[currentState][col];

        if (currentState == REJECT)
        {
            if (prevState != REJECT)
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

    if (currentState != REJECT && currentToken != "")
    {
        token.token = currentToken;
        token.lexeme = currentState;
        token.lexemeName = getTransition(token.lexeme);
        tokens.push_back(token);
    }

    return tokens;
}

Lexer::TransitionType Lexer::getTransition(char tokenChar)
{
    TransitionType transition = UNKNOWN;

    if (isspace(tokenChar))
    {
        transition = REJECT;
    }
    else if (isdigit(tokenChar))
    {
        transition = INTEGER;
    }
    else if (tokenChar == '.')
    {
        transition = REAL;
    }
    else if (isalpha(tokenChar))
    {
        transition = IDENTIFIER;
    }

    return transition;
}

std::string Lexer::stateToString(int state)
{
    std::string stateStr = "";

    switch (state)
    {
    case INTEGER:
        stateStr = "Integer";
        break;
    case REAL:
        stateStr = "Real";
        break;
    case IDENTIFIER:
        stateStr = "Identifier";
        break;
    case UNKNOWN:
        stateStr = "Unknown";
        break;
    case REJECT:
        stateStr = "Reject";
        break;
    }

    return stateStr;
}