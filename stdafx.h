#ifndef STDAFX_H
#define STDAFX_H
#include <string>

struct Rat18
{
    enum TransitionType
    {
        REJECT = 0,
        INTEGER,
        REAL,
        IDENTIFIER,
        UNKNOWN
    };

    // State table
     int dfsm[5][5] = {{0, INTEGER, REAL, IDENTIFIER, UNKNOWN},
                     {INTEGER, INTEGER, REAL, REJECT, REJECT},
                     {REAL, REAL, REJECT, REJECT, REJECT},
                     {IDENTIFIER, REJECT, REJECT, IDENTIFIER, REJECT},
                     {UNKNOWN, REJECT, REJECT, REJECT, REJECT}};

    struct Token
    {
        std::string token;
        int lexeme;
        std::string lexemeName;
    };
};

#endif // STDAFX_H