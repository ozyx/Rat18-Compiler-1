#ifndef HEADER_H
#define HEADER_H

#include <string>

enum State
{
    SPACE = 0,
    SEPARATOR = 1,
    OPERATOR = 2,
    BOOLEAN = 3,
    NUMBER = 4,
    REAL = 5,
    KEYWORD = 6,
    IDENTIFIER = 7
};

std::string stateToString(State state)
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

#endif // HEADER_H