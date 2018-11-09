#include "SyntaxAnalyzer.h"

SyntaxAnalyzer::SyntaxAnalyzer(std::vector<Lexer::Token> tokens, bool print) : it(tokens.begin()), currentToken(*(it))
{
    this->print = print;
    this->tokens = tokens;
}

SyntaxAnalyzer::~SyntaxAnalyzer()
{
}

void SyntaxAnalyzer::getNextToken()
{
    // Increment iterator
    ++it;

    if (it == this->tokens.end())
    {
        // No more tokens!
        // Throw an error
    }

    this->currentToken = *(it);
}

void SyntaxAnalyzer::Rat18F()
{
    OptFunctionDefinitions();

    getNextToken();

    if (currentToken.lexeme == "$$")
    {
        OptDeclarationList();
        StatementList();
    }

    getNextToken();

    if (currentToken.lexeme != "$$")
    {
        // error?
    }
}

void SyntaxAnalyzer::Function()
{
    getNextToken();
    Identifier();

    getNextToken();
    if (currentToken.lexeme != "(")
    {
        // TODO: throw error
    }

    OptParameterList();

    getNextToken();
    if (currentToken.lexeme != ")")
    {
        // TODO: throw error
    }

    OptDeclarationList();
    Body();
}

void SyntaxAnalyzer::OptFunctionDefinitions()
{
    getNextToken();

    if (currentToken.lexeme == "function")
    {
        FunctionDefinitions();
    }
    else
    {
        Empty();
    }
}

void SyntaxAnalyzer::OptDeclarationList()
{
    getNextToken();

    if (currentToken.lexeme == "real" | currentToken.lexeme == "boolean" | currentToken.lexeme == "int")
    {
        DeclarationList();
    }
    else
    {
        Empty();
    }
}

void SyntaxAnalyzer::DeclarationList()
{
    Declaration();

    getNextToken();

    if (currentToken.lexeme == ";")
    {
        getNextToken();
        if (currentToken.lexeme == "real" | currentToken.lexeme == "boolean" | currentToken.lexeme == "int")
        {
            DeclarationList();
        }
    }
}

void SyntaxAnalyzer::Declaration()
{
    Qualifier();

    getNextToken();

    if (currentToken.token == "Identifier")
    {
        IDs();
    }
}

void SyntaxAnalyzer::Qualifier()
{
    // print stuff
}

void SyntaxAnalyzer::IDs()
{
    Identifier();
    getNextToken();
    if (currentToken.lexeme == ",")
    {
        getNextToken();
        if (currentToken.token == "Identifier")
        {
            IDs();
        }
        else
        {
            // TODO: error
        }
    }
}

void SyntaxAnalyzer::Identifier()
{
    // TODO: print stuff
}

void SyntaxAnalyzer::StatementList()
{
    Statement();

    getNextToken();
    if (currentToken.lexeme == "get" | currentToken.lexeme == "put" | currentToken.lexeme == "while" | currentToken.lexeme == "if" |
        currentToken.lexeme == "return" | currentToken.token == "Identifier")
    {
        StatementList();
    }
}

void SyntaxAnalyzer::Statement()
{
    getNextToken();

    if (currentToken.lexeme == "{")
    {
        Compound();
    }
    else if (currentToken.token == "Identifier")
    {
        Assign();
    }
    else if (currentToken.lexeme == "if")
    {
        If();
    }
    else if (currentToken.lexeme == "return")
    {
        Return();
    }
    else if (currentToken.lexeme == "put")
    {
        Print();
    }
    else if (currentToken.lexeme == "get")
    {
        Scan();
    }
    else if (currentToken.lexeme == "while")
    {
        While();
    }
    else
    {
        // TODO: throw error
    }
}

void SyntaxAnalyzer::Compound()
{
    StatementList();

    getNextToken();

    if (currentToken.lexeme != "}")
    {
        // TODO: throw error
    }
}

void SyntaxAnalyzer::Assign()
{
    Identifier();

    getNextToken();

    if (currentToken.lexeme != "=")
    {
        // TODO: throw error
    }

    Expression();
}

void SyntaxAnalyzer::Expression()
{
    Term();
    ExpressionPrime();
}

void SyntaxAnalyzer::ExpressionPrime()
{
    getNextToken();
    if (currentToken.lexeme == "+" | currentToken.lexeme == "-")
    {
        Term();
        ExpressionPrime();
    }
    else
    {
        Empty();
    }
}

void SyntaxAnalyzer::Term()
{
    Factor();
    TermPrime();
}

void SyntaxAnalyzer::Factor()
{
    getNextToken();

    if (currentToken.lexeme == "-")
    {
        getNextToken();
    }

    Primary();
}

void SyntaxAnalyzer::Primary()
{
    if (currentToken.token == "Identifier")
    {
        Identifier();

        getNextToken();
        if (currentToken.lexeme == "(")
        {
            IDs();
            getNextToken();

            if (currentToken.lexeme != ")")
            {
                // TODO: throw error
            }
        }
    }
    else if (currentToken.token == "Integer")
    {
        Integer();
    }
    else if (currentToken.lexeme == "(")
    {
        Expression();

        getNextToken();

        if (currentToken.lexeme != ")")
        {
            // TODO: throw error
        }
    }
    else if (currentToken.token == "Real")
    {
        Real();
    }
    else if (currentToken.lexeme == "true")
    {
        // TODO:
    }
    else if (currentToken.lexeme == "false")
    {
        // TODO:
    }
}

void SyntaxAnalyzer::Integer()
{
    // TODO: print
}

void SyntaxAnalyzer::Real()
{
    // TODO: print
}

void SyntaxAnalyzer::Return()
{
    getNextToken();

    if (currentToken.lexeme != ";")
    {
        Expression();
    }
}

void SyntaxAnalyzer::If()
{
    getNextToken();

    if (currentToken.lexeme != "(")
    {
        // TODO: throw error
    }

    Condition();

    getNextToken();

    if (currentToken.lexeme != ")")
    {
        // TODO: throw error
    }

    Statement();

    getNextToken();

    if (currentToken.lexeme == "else")
    {
        Statement();
    }

    getNextToken();

    if (currentToken.lexeme != "ifend")
    {
        // TODO: throw error
    }
}

void SyntaxAnalyzer::Condition()
{
    getNextToken();
    Expression();

    Relop();

    getNextToken();
    Expression();
}

void SyntaxAnalyzer::Relop()
{
    if (currentToken.lexeme != "==" && currentToken.lexeme != "^=" && currentToken.lexeme != ">" && currentToken.lexeme != "<" && currentToken.lexeme != "=>" && currentToken.lexeme != "=<")
    {
        // TODO: throw error
    }
}

void SyntaxAnalyzer::Empty()
{
    // TODO: print
}

void SyntaxAnalyzer::Body()
{
    getNextToken();
    if (currentToken.lexeme != "{")
    {
        // TODO: throw error
    }

    StatementList();

    if (currentToken.lexeme != "}")
    {
        // TODO: throw error
    }
}

void SyntaxAnalyzer::FunctionDefinitions()
{
    Function();

    getNextToken();
    if (currentToken.lexeme == "function")
    {
        FunctionDefinitions();
    }
}

void SyntaxAnalyzer::Print()
{
    getNextToken();
    if (currentToken.lexeme != "(")
    {
        // TODO: throw error
    }

    getNextToken();
    Expression();

    getNextToken();
    if (currentToken.lexeme != ")")
    {
        // TODO: throw error
    }

    getNextToken();
    if (currentToken.lexeme != ";")
    {
        // TODO: throw error
    }
}

void SyntaxAnalyzer::Scan()
{
    getNextToken();
    if (currentToken.lexeme != "(")
    {
        // TODO: throw error
    }

    getNextToken();
    IDs();

    getNextToken();
    if (currentToken.lexeme != ")")
    {
        // TODO: throw error
    }

    getNextToken();
    if (currentToken.lexeme != ";")
    {
        // TODO: throw error
    }
}

void SyntaxAnalyzer::TermPrime()
{
    getNextToken();

    if (currentToken.lexeme == "*" | currentToken.lexeme == "/")
    {
        Factor();
        TermPrime();
    }
}
