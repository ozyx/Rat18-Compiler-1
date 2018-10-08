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
        // Check if we are inside of a multiline comment,
        // or at the beginning of a new comment range.
        if (comment | (c == '[' && buffer.peek() == '*'))
        {
            // Iterate until we see a '*]'
            while (c != '*' | buffer.peek() != ']')
            {
                // If we hit the end of the line, set
                // comment switch to "true" and reset
                // the current character so it gets ignored.
                if (buffer.eof())
                {
                    comment = true;
                    c = ' ';
                    break;
                }

                buffer.get(c);
            }

            // If we haven't reached the end of the file,
            // and the current character is a '*', we know
            // we have reached the end of the comment section.
            if (!buffer.eof() && c == '*')
            {
                comment = false;

                // Get both characters '*]' out of the stream
                buffer.get(c).get(c);
            }
        }

        // Get the character type (transition)
        transition = getTransition(c);

        // Update state
        currState = Lexer::stateTable[currState][transition];

        // Terminating state
        if (currState == TRM)
        {
            tokenStr = stateToString(prevState);

            if (tokenStr != "Reject")
            {

                if (tokenStr == "Identifier")
                {
                    // Check if this identifier is a keyword
                    if (isKeyword(lexeme))
                    {
                        tokenStr = "Keyword";
                    }
                }
                else if (tokenStr == "Operator")
                {
                    if (lexeme.length() > 1 && !isValidOperator(lexeme))
                    {
                        lexeme.pop_back();
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
            else
            {
                // reset state machine
                currState = NS;
                lexeme.clear();
                tokenStr.clear();
            }
        }
        else
        {
            if (!isspace(c))
            {
                lexeme.push_back(c);
            }
        }

        prevState = currState;
    }

    // Grab the last token
    tokenStr = stateToString(prevState);

    // Evaluate the last token
    if (tokenStr != "Reject")
    {
        if (tokenStr == "Identifier")
        {
            // Check if this identifier is a keyword
            if (isKeyword(lexeme))
            {
                tokenStr = "Keyword";
            }
        }
        else if (tokenStr == "Operator")
        {
            if (lexeme.length() > 1 && !isValidOperator(lexeme))
            {
                lexeme.pop_back();
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
    int transition = REJECT;

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
    else if (isValidOperator(c))
    {
        transition = OPERATOR;
    }
    else if (isValidSeparator(c))
    {
        transition = SEPARATOR;
    }
    else if (c == '$')
    {
        transition = FUNC_SEPARATOR;
    }

    return transition;
}

std::string Lexer::stateToString(int state) const
{
    std::string stateStr = "Reject";

    switch (state)
    {
    case S1:
    case S2:
        stateStr = "Identifier";
        break;
    case S3:
        stateStr = "Reject";
        break;
    case S4:
        stateStr = "Integer";
        break;
    case S5:
        stateStr = "Reject";
    case S6:
        stateStr = "Real";
        break;
    case S7:
    case S8:
        stateStr = "Operator";
        break;
    case S9:
        stateStr = "Separator";
        break;
    case S11:
        stateStr = "Separator";
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
