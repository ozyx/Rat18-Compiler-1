#include "Lexer.h"

Lexer::Lexer() : comment(false) {}

Lexer::~Lexer() {}

std::vector<Lexer::Token> Lexer::lex(std::stringstream &buffer)
{
    std::vector<Token> tokens;
    Token *token;
    char c;
    int transition;
    std::string lexeme = "";
    std::string tokenStr = "";
    int prevState = 0;
    int currState = 0;

    while (buffer.get(c))
    {
        //check for and ignore comments
        if ((c == '[') | comment)
        {
            do
            {
                buffer.get(c);

                // Handling for multi-line comments
            } while (c != ']' && !buffer.eof());

            // comment == true if we are in a multi-line comment
            this->comment = c != ']';

            if (this->comment)
            {
                // We are currently in a multi-line comment, and we
                // have reached the end of the line.
                // Just set the character to a space so can ignore it.
                c = ' ';
            }
            else
            {
                // Get the next token after ']'
                buffer.get(c);
            }
        }

        //get transition type
        transition = getTransition(c);

        //update state
        currState = Lexer::stateTable[currState][transition];

        // Terminating state
        if (currState == S7)
        {
            tokenStr = stateToString(prevState);

            if (tokenStr != "Unknown")
            {
                if (tokenStr == "Identifier")
                {
                    // Check if this identifier is a keyword
                    if (isKeyword(lexeme))
                    {
                        tokenStr = "Keyword";
                    }
                }
                // Create token and add to list of tokens
                token = new Token(tokenStr, lexeme);
                tokens.push_back(*token);

                // reset state machine
                currState = NS;
                lexeme.clear();
                tokenStr.clear();

                // If we reached the terminating state by anything other
                // than whitespace, we need to put it back and re-examine
                // the character on the next iteration.
                if (!isspace(c))
                {
                    buffer.putback(c);
                }
            }
        }
        else
        {
            if (!isspace(c))
            {
                lexeme.push_back(c);

                if (isValidOperator(c))
                {
                    // Check for double operators
                    buffer.get(c);
                    if (isValidOperator(c))
                    {
                        // Check if this becomes a valid double-operator (looking ahead)
                        lexeme.push_back(c);

                        // If it does not, remove from lexeme and put the char back.
                        if (!isValidOperator(lexeme))
                        {
                            lexeme.pop_back();
                            buffer.putback(c);
                        }
                    }
                    else
                    {
                        // If it isn't an operator, just put it back.
                        buffer.putback(c);
                    }

                    token = new Token("Operator", lexeme);
                    tokens.push_back(*token);
                    lexeme.clear();
                }
                else if (isValidSeparator(c))
                {
                    token = new Token("Separator", lexeme);
                    tokens.push_back(*token);
                    lexeme.clear();
                }
            }
        }

        prevState = currState;
    }

    // Grab the last token
    tokenStr = stateToString(prevState);

    // Evaluate the last token
    if (tokenStr != "Unknown")
    {
        if (tokenStr == "Identifier")
        {
            if (isKeyword(lexeme))
            {
                tokenStr = "Keyword";
            }
        }
        // Create token and add to list of tokens
        token = new Token(tokenStr, lexeme);
        tokens.push_back(*token);
    }

    return tokens;
}

int Lexer::getTransition(char c) const
{
    int  transition = UNKNOWN;

    if (isdigit(c))
    {
        transition = INTEGER;
    }
    else if (isalpha(c))
    {
        transition = IDENTIFIER;
    }
    else if (c == '.')
    {
        transition = REAL;
    }

    return transition;
}

std::string Lexer::stateToString(int state) const
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

bool Lexer::isValidOperator(char c) const
{
    return operators.count(c);
}

bool Lexer::isValidOperator(std::string s) const
{
    return double_operators.count(s);
}

bool Lexer::isValidSeparator(char c) const
{
    return separators.count(c);
}

bool Lexer::isKeyword(std::string token) const
{
    return keywords.count(token);
}