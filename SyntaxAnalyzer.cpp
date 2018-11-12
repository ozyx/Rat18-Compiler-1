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

    if (currentToken.lexeme == "$$")
    {
        getNextToken();
        OptDeclarationList();
        StatementList();
    }

    if (currentToken.lexeme != "$$")
    {
        // error?
    }
}

void SyntaxAnalyzer::Parameter()
{
    IDs();

    if (currentToken.lexeme != ":")
    {
        // TODO: throw error
    }

    getNextToken();
    Qualifier();
}

void SyntaxAnalyzer::Function()
{
    Identifier();

    getNextToken();
    if (currentToken.lexeme != "(")
    {
        // TODO: throw error
    }

    getNextToken();

    OptParameterList();

    if (currentToken.lexeme != ")")
    {
        // TODO: throw error
    }

    getNextToken();
    OptDeclarationList();
    Body();
}

void SyntaxAnalyzer::OptFunctionDefinitions()
{
    if (currentToken.lexeme == "function")
    {
        getNextToken();
        FunctionDefinitions();
    }
    else
    {
        Empty();
    }
}

void SyntaxAnalyzer::OptDeclarationList()
{
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

    if (currentToken.lexeme == "get" | currentToken.lexeme == "put" | currentToken.lexeme == "while" | currentToken.lexeme == "if" |
        currentToken.lexeme == "return" | currentToken.token == "Identifier")
    {
        StatementList();
    }
}

void SyntaxAnalyzer::Statement()
{

    if (currentToken.lexeme == "{")
    {
        getNextToken();
        Compound();
    }
    else if (currentToken.token == "Identifier")
    {
        Assign();
    }
    else if (currentToken.lexeme == "if")
    {
        getNextToken();
        If();
    }
    else if (currentToken.lexeme == "return")
    {
        getNextToken();
        Return();
    }
    else if (currentToken.lexeme == "put")
    {
        getNextToken();
        Print();
    }
    else if (currentToken.lexeme == "get")
    {
        getNextToken();
        Scan();
    }
    else if (currentToken.lexeme == "while")
    {
        getNextToken();
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

    if (currentToken.lexeme != "}")
    {
        // TODO: throw error
    }

    getNextToken();
}

void SyntaxAnalyzer::Assign()
{
    Identifier();

    getNextToken();

    if (currentToken.lexeme != "=")
    {
        // TODO: throw error
    }

    getNextToken();
    Expression();

    if (currentToken.lexeme != ";")
    {
        // TODO: throw error
    }
    getNextToken();
}

void SyntaxAnalyzer::Expression()
{
    Term();
    ExpressionPrime();
}

void SyntaxAnalyzer::ExpressionPrime()
{
    if (currentToken.lexeme == "+" | currentToken.lexeme == "-")
    {
        getNextToken();

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
            getNextToken();
            IDs();

            if (currentToken.lexeme != ")")
            {
                // TODO: throw error
            }

            getNextToken();
        }
    }
    else if (currentToken.token == "Integer")
    {
        Integer();
        getNextToken();
    }
    else if (currentToken.lexeme == "(")
    {
        getNextToken();

        Expression();

        if (currentToken.lexeme != ")")
        {
            // TODO: throw error
        }
        getNextToken();
    }
    else if (currentToken.token == "Real")
    {
        Real();
        getNextToken();
    }
    else if (currentToken.lexeme == "true")
    {
        // TODO:
        getNextToken();
    }
    else if (currentToken.lexeme == "false")
    {
        // TODO:
        getNextToken();
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
    if (currentToken.lexeme != ";")
    {
        Expression();
    }
    getNextToken();
}

void SyntaxAnalyzer::If()
{
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
    if (currentToken.lexeme != "{")
    {
        // TODO: throw error
    }

    getNextToken();

    StatementList();

    if (currentToken.lexeme != "}")
    {
        // TODO: throw error
    }

    getNextToken();
}

void SyntaxAnalyzer::FunctionDefinitions()
{
    Function();

    if (currentToken.lexeme == "function")
    {
        getNextToken();
        FunctionDefinitions();
    }
}

void SyntaxAnalyzer::Print()
{
    if (currentToken.lexeme != "(")
    {
        // TODO: throw error
    }

    getNextToken();
    Expression();

    if (currentToken.lexeme != ")")
    {
        // TODO: throw error
    }
    getNextToken();

    if (currentToken.lexeme != ";")
    {
        // TODO: throw error
    }

    getNextToken();
}

void SyntaxAnalyzer::Scan()
{
    if (currentToken.lexeme != "(")
    {
        // TODO: throw error
    }

    getNextToken();
    IDs();

    if (currentToken.lexeme != ")")
    {
        // TODO: throw error
    }

    getNextToken();
    if (currentToken.lexeme != ";")
    {
        // TODO: throw error
    }

    getNextToken();
}

void SyntaxAnalyzer::TermPrime()
{
    if (currentToken.lexeme == "*" | currentToken.lexeme == "/")
    {
        getNextToken();

        Factor();
        TermPrime();
    }
}

void SyntaxAnalyzer::Analyze()
{
    Rat18F();
}

void SyntaxAnalyzer::OptParameterList()
{
    if (currentToken.lexeme == ")")
    {
        Empty();
    }
    else if (currentToken.token == "Identifier")
    {
        ParameterList();
    }
    else
    {
        // TODO: throw error
    }
}

void SyntaxAnalyzer::ParameterList()
{
    Parameter();

    getNextToken();

    if (currentToken.lexeme == ",")
    {
        getNextToken();
        ParameterList();
    }
}

void SyntaxAnalyzer::While()
{

    if (currentToken.lexeme != "(")
    {
        // TODO: throw error
    }
    getNextToken();

    Condition();

    if (currentToken.lexeme != ")")
    {
        // TODO: throw error
    }
    getNextToken();
    Statement();

    if (currentToken.lexeme != "whileend")
    {
        // TODO: throw error
    }
    getNextToken();
}

SyntaxError::SyntaxError(std::string message, int lineNumber)
{
    this->message = message;
    this->lineNumber = lineNumber;
}

SyntaxError::~SyntaxError() {}

std::string SyntaxError::getMessage()
{
    return (this->message + " Line: " + std::to_string(this->lineNumber));
}