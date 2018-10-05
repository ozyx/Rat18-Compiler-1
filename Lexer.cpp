#include "Lexer.h"

Lexer::Lexer() {}

Lexer::~Lexer() {}

std::vector<Lexer::Token> Lexer::lex(std::ifstream &fin)
{
    std::vector<Token> tokens;
    Token *token;
    char c;
    int transition;
    std::string lexeme = "";
    std::string tokenStr = "";
    int prevState = 0;
    int currState = 0;

    while (fin.get(c))
    {
        //check for and ignore comments
        if (c == '[')
        {
            do
            {
                fin.get(c);
            } while (c != ']');

            // Get the next token after ']'
            fin.get(c);
        }

        //get transition type
        transition = getTransition(c);

        //update state
        currState = Lexer::stateTable[currState][transition];

        // Terminating state
        if (currState == S7)
        {
            tokenStr = stateToString(prevState);

            // Create token and add to list of tokens
            token = new Token(tokenStr, lexeme);
            tokens.push_back(*token);

            // reset state machine
            currState = NS;
            lexeme.clear();
            tokenStr.clear();
        }
        else
        {
            lexeme.push_back(c);
        }

        prevState = currState;
    }

    return tokens;
}

int Lexer::getTransition(char c)
{
    if (isdigit(c))
    {
        return INTEGER;
    }
    else if (isalpha(c))
    {
        return IDENTIFIER;
    }
    else if (c == '.')
    {
        return REAL;
    }
    else
    {
        return UNKNOWN;
    }

    return 0;
}

std::string Lexer::stateToString(int state)
{
    std::string stateStr = "";

    switch (state)
    {
    case S1:
    case S2:
        stateStr = "Identifier";
        break;
    case S4:
        stateStr = "Integer";
        break;
    case S6:
        stateStr = "Real";
        break;
    default:
        stateStr = "Unknown";
        break;
    }

    return stateStr;
}
