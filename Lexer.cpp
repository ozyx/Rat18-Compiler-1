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
        // Get one character
        current = expression[i];

        // Get the transition type given the character
        col = getTransition(current);

        // Record previous state and get new state
        prevState = currentState;
        currentState = dfsm[currentState][col];

        if (currentState == REJECT)
        {
            // If the previous state was not reject, then we know
            // we have a token.
            if (prevState != REJECT)
            {
                // TODO: reexamine this l
                token.token = current;
                token.lexeme = prevState;
                token.lexemeName = stateToString(token.lexeme);
            }
        }
        // Otherwise, build our next token
        else
        {
            if (currentState != SPACE)
            {
                currentToken += current;
            }

            // Iterate
            ++i;
        }

        // TODO: reexamine this logic
        if (currentState == REJECT && currentToken != "")
        {
            token.token = currentToken;
            token.lexeme = prevState;
            token.lexemeName = stateToString(token.lexeme);
            tokens.push_back(token);
            currentToken = "";
        }
    }
    if (currentState != REJECT && currentToken != "")
    {
        token.token = currentToken;
        token.lexeme = prevState;
        token.lexemeName = stateToString(token.lexeme);
        tokens.push_back(token);
        currentToken = "";
    }
    return tokens;
}

Lexer::TransitionType Lexer::getTransition(char tokenChar)
{
    TransitionType transition = UNKNOWN;

    if (isspace(tokenChar))
    {
        transition = SPACE;
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
    else
    {
        transition = REJECT;
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