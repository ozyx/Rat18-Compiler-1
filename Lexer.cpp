#include "Lexer.h"

Lexer::Lexer() {}

Lexer::~Lexer() {}

std::string Lexer::stateToString(Lexer::State state)
{
    std::string stateStr = "";
    switch (state)
    {
    case SPACE:
        stateStr = "Space";
        break;
    case SEPARATOR:
        stateStr = "Separator";
        break;
    case OPERATOR:
        stateStr = "Operator";
        break;
    case BOOLEAN:
        stateStr = "Boolean";
        break;
    case NUMBER:
        stateStr = "Number";
        break;
    case REAL:
        stateStr = "Real";
        break;
    case KEYWORD:
        stateStr = "Keyword";
        break;
    case IDENTIFIER:
        stateStr = "Identifier";
        break;
    }

    return stateStr;
}

std::map<char *, Lexer::State> *Lexer::lex(std::ifstream &Rat18)
{
    std::map<char *, State> *lex = new std::map<char *, State>();

    char unit;

    char *word;
    int length = 0;

    State prevstate = SPACE; // previous state
    State curstate = SPACE;  // current space

    while (!Rat18.eof())
    {
        Rat18.get(unit);
        if (unit == '\n')
            continue; // skip tests - ignor new line
        if (unit == '\0')
            continue; // skip tests - ignor string terminator
        if (unit == ' ' && prevstate == SPACE)
            continue; // skip test - ignore more than one space
        //space
        if (unit == ' ')
        {
            curstate = SPACE;
        }
        //Separator
        else if (unit == '(' | unit == ')' | unit == '{' | unit == '}')
        {
            curstate = SEPARATOR;
        }
        //operator
        else if (unit == '+' | unit == '-' | unit == '/' | unit == '*' | unit == '%')
        {
            curstate = OPERATOR;
        }
        //boolean op
        else if ((unit == '=' | unit == '<' | unit == '>') && prevstate == OPERATOR)
        {
            curstate = BOOLEAN;
        }
        //number
        else if (unit > 47 && unit < 58 && prevstate != IDENTIFIER)
        {
            curstate = NUMBER;
        }
        //real
        else if (unit == '.' && prevstate == NUMBER)
        {
            curstate = REAL;
        }
        // word           uppercase                       lowercase                     digit             did not come from num   did not come from real
        else if ((unit > 64 && unit < 91) | (unit > 96 && unit < 123) | (unit > 47 && unit < 58) && prevstate != NUMBER && prevstate != REAL)
        {
            curstate = IDENTIFIER;
        }
        // breaks grammar
        else
        {
            return nullptr;
        }

        //build word
        if (curstate == IDENTIFIER)
        {
            *word = unit;
            word++;
            length++;
        }
        //build num or real
        if (curstate == NUMBER)
        {
        }
        //build separator
        if (curstate == SEPARATOR)
        {
            word = &unit;
            lex->insert({word, SEPARATOR});
        }
        // add token and lexeme to lex vector
        if (curstate == SPACE && prevstate == IDENTIFIER)
        {
            word = nullptr;
            for (int i = length; i > 0; i--)
            {
                word--;
            }
            if (isKeyword(word))
            {
                lex->insert({word, KEYWORD});
            }
            else
            {
                lex->insert({word, IDENTIFIER});
            }
        }

        prevstate = curstate;
    }

    return lex;
}

// check if word is keyword or ID
bool Lexer::isKeyword(char *word)
{
    if(word == nullptr) { throw new std::runtime_error("word cannot be null"); }
    return strcmp(word, "if") == 0 | strcmp(word, "ifend") == 0 | strcmp(word, "while") == 0 | 
    strcmp(word, "whileend") == 0 | strcmp(word, "else") == 0 | strcmp(word, "elseend") == 0 | 
    strcmp(word, "switch") == 0 | strcmp(word, "case") == 0 | strcmp(word, "break") == 0;
}

// step through map and print
void Lexer::printLex(std::map<char *, Lexer::State> lexRat)
{
    std::cout << "Output:\n";
    std::cout << "token\tlexeme\n";
    std::cout << "___________________________\n";

    std::map<char *, Lexer::State>::iterator it = lexRat.begin();

    while (it != lexRat.end())
    {
        std::cout << (*it).first << "\t" << stateToString((*it).second) << "\n";
        ++it;
    }
}