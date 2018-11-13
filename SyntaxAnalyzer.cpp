#include "SyntaxAnalyzer.h"

SyntaxAnalyzer::SyntaxAnalyzer(std::vector<Lexer::Token> tokens, bool print) : it(tokens.begin()), currentToken(*(it))
{
    this->print = print;
    this->tokens = tokens;
}

SyntaxAnalyzer::~SyntaxAnalyzer()
{
}

/**
 * Get the next token in the list of tokens
 * Increments iterator to current token
 */
void SyntaxAnalyzer::getNextToken()
{
    // Increment iterator
    ++it;

    if (it == this->tokens.end())
    {
        --it;
        throw SyntaxError("Unexpected end of file", currentToken.lineNumber);
    }

    this->currentToken = *(it);

    if (print)
    {
        printCurrentToken();
    }
}

void SyntaxAnalyzer::Rat18F()
{
    if (print)
    {
        printCurrentToken();
        std::cout << "\t<Rat18F> -> <Opt Function Definitions> $$ <Opt Declaration List> <Statement List>" << std::endl;
    }

    OptFunctionDefinitions();

    if (currentToken.lexeme == "$$")
    {
        getNextToken();
        OptDeclarationList();
        StatementList();
    }

    if (currentToken.lexeme != "$$")
    {
        throw SyntaxError("Expected '$$'.", currentToken.lineNumber);
    }
}

void SyntaxAnalyzer::Parameter()
{
    if (print)
    {
        std::cout << "\t<Parameter> -> <IDs> : <Qualifier>" << std::endl;
    }

    IDs();

    if (currentToken.lexeme != ":")
    {
        throw SyntaxError("Expected ':'", currentToken.lineNumber);
    }

    getNextToken();
    Qualifier();
}

void SyntaxAnalyzer::Function()
{
    if (print)
    {
        std::cout << "\t<Function> ->  function <Identifier> ( <Opt Parameter List> ) <Opt Declaration List> <Body>" << std::endl;
    }

    Identifier();

    getNextToken();
    if (currentToken.lexeme != "(")
    {
        throw SyntaxError("Expected '('", currentToken.lineNumber);
    }

    getNextToken();

    OptParameterList();

    if (currentToken.lexeme != ")")
    {
        throw SyntaxError("Expected ')'", currentToken.lineNumber);
    }

    getNextToken();
    OptDeclarationList();
    Body();
}

void SyntaxAnalyzer::OptFunctionDefinitions()
{
    if (print)
    {
        std::cout << "\t<Opt Function Definitions> ->  <Function Definitions> | <Empty>" << std::endl;
    }

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
    if (print)
    {
        std::cout << "\t<Opt Declaration List> -> <Declaration List> | <Empty>" << std::endl;
    }

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
    if (print)
    {
        std::cout << "\t<Declaration List> -> <Declaration>; | <Declaration>; <Declaration List>\n";
    }

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
    if (print)
    {
        std::cout << "\t<Declaration> -> <Qualifier> <IDs>" << std::endl;
    }

    Qualifier();
    getNextToken();

    if (currentToken.token == "Identifier")
    {
        IDs();
    }
}

void SyntaxAnalyzer::Qualifier()
{
    if (print)
    {
        std::cout << "\t<Qualifier> ->  int | boolean | real" << std::endl;
    }
}

void SyntaxAnalyzer::IDs()
{
    if (print)
    {
        std::cout << "\t<IDs> -> <Identifier> | <Identifier>, <IDs>" << std::endl;
    }

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
            throw SyntaxError("Expected identifier", currentToken.lineNumber);
        }
    }
}

void SyntaxAnalyzer::Identifier()
{
    if (print)
    {
        std::cout << "\t<Identifier>" << std::endl;
    }
}

void SyntaxAnalyzer::StatementList()
{
    if (print)
    {
        std::cout << "\t<Statement List> -> <Statement> | <Statement> <Statement List>" << std::endl;
    }

    Statement();

    if (currentToken.lexeme == "get" | currentToken.lexeme == "put" | currentToken.lexeme == "while" | currentToken.lexeme == "if" |
        currentToken.lexeme == "return" | currentToken.token == "Identifier")
    {
        StatementList();
    }
}

void SyntaxAnalyzer::Statement()
{
    if (print)
    {
        std::cout << "\t<Statement> -> <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>" << std::endl;
    }

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
        throw SyntaxError("Expected '{', identifier or keyword", currentToken.lineNumber);
    }
}

void SyntaxAnalyzer::Compound()
{
    if (print)
    {
        std::cout << "\t<Compound> -> { <Statement List> }" << std::endl;
    }

    StatementList();

    if (currentToken.lexeme != "}")
    {
        throw SyntaxError("Expected '}'", currentToken.lineNumber);
    }

    getNextToken();
}

void SyntaxAnalyzer::Assign()
{
    if (print)
    {
        std::cout << "\t<Assign> -> <Identifier> = <Expression>;" << std::endl;
    }

    Identifier();

    getNextToken();

    if (currentToken.lexeme != "=")
    {
        throw SyntaxError("Expected '='", currentToken.lineNumber);
    }

    getNextToken();
    Expression();

    if (currentToken.lexeme != ";")
    {
        throw SyntaxError("Expected ';'", currentToken.lineNumber);
    }
    getNextToken();
}

void SyntaxAnalyzer::Expression()
{
    if (print)
    {
        std::cout << "\t<Expression> -> <Term> <ExpressionPrime>" << std::endl;
    }

    Term();
    ExpressionPrime();
}

void SyntaxAnalyzer::ExpressionPrime()
{
    if (print)
    {
        std::cout << "\t<ExpressionPrime> -> + <Term> <ExpressionPrime> | - <Term> <ExpressionPrime> | <Empty>" << std::endl;
    }

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
    if (print)
    {
        std::cout << "\t<Term> -> <Factor> <TermPrime>" << std::endl;
    }

    Factor();
    TermPrime();
}

void SyntaxAnalyzer::Factor()
{
    if (print)
    {
        std::cout << "\t<Factor> -> - <Primary> | <Primary>" << std::endl;
    }

    if (currentToken.lexeme == "-")
    {
        getNextToken();
    }

    Primary();
}

void SyntaxAnalyzer::Primary()
{
    if (print)
    {
        std::cout << "\t<Primary> -> <Identifier> | <Integer> | <Identifier> ( <IDs> ) | ( <Expression> ) | <Real> | true | false" << std::endl;
    }

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
                throw SyntaxError("Expected ')'", currentToken.lineNumber);
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
            throw SyntaxError("Expected ')'", currentToken.lineNumber);
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
        std::cout << "\ttrue" << std::endl;
        getNextToken();
    }
    else if (currentToken.lexeme == "false")
    {
        std::cout << "\tfalse" << std::endl;
        getNextToken();
    }
}

void SyntaxAnalyzer::Integer()
{
    if (print)
    {
        std::cout << "\t<Integer>" << std::endl;
    }
}

void SyntaxAnalyzer::Real()
{
    if (print)
    {
        std::cout << "\t<Real>" << std::endl;
    }
}

void SyntaxAnalyzer::Return()
{
    if (print)
    {
        std::cout << "\t<Return> -> return; |  return <Expression>;" << std::endl;
    }

    if (currentToken.lexeme != ";")
    {
        Expression();
    }
    getNextToken();
}

void SyntaxAnalyzer::If()
{
    if (print)
    {
        std::cout << "\t<If> -> if ( <Condition> ) <Statement> endif | if ( <Condition> ) <Statement> else <Statement> endif" << std::endl;
    }

    if (currentToken.lexeme != "(")
    {
        throw SyntaxError("Expected '('", currentToken.lineNumber);
    }

    Condition();

    getNextToken();

    if (currentToken.lexeme != ")")
    {
        throw SyntaxError("Expected ')'", currentToken.lineNumber);
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
        throw SyntaxError("Expected 'ifend' keyword", currentToken.lineNumber);
    }
}

void SyntaxAnalyzer::Condition()
{
    if (print)
    {
        std::cout << "\t<Condition> -> <Expression>  <Relop>  <Expression>" << std::endl;
    }

    Expression();

    Relop();

    getNextToken();
    Expression();
}

void SyntaxAnalyzer::Relop()
{
    if (currentToken.lexeme != "==" && currentToken.lexeme != "^=" && currentToken.lexeme != ">" && currentToken.lexeme != "<" && currentToken.lexeme != "=>" && currentToken.lexeme != "=<")
    {
        throw SyntaxError("Expected relational operator", currentToken.lineNumber);
    }

    if (print)
    {
        std::cout << "\t<Relop> -> " << currentToken.lexeme << std::endl;
    }
}

void SyntaxAnalyzer::Empty()
{
    if (print)
    {
        std::cout << "\t<Empty> -> ε" << std::endl;
    }
}

void SyntaxAnalyzer::Body()
{
    if (print)
    {
        std::cout << "\t<Body> -> { <Statement List> }" << std::endl;
    }

    if (currentToken.lexeme != "{")
    {
        throw SyntaxError("Expected '{'", currentToken.lineNumber);
    }

    getNextToken();

    StatementList();

    if (currentToken.lexeme != "}")
    {
        throw SyntaxError("Expected '}'", currentToken.lineNumber);
    }

    getNextToken();
}

void SyntaxAnalyzer::FunctionDefinitions()
{
    if (print)
    {
        std::cout << "\t<Function Definitions> -> <Function> | <Function> <Function Definitions>" << std::endl;
    }

    Function();

    if (currentToken.lexeme == "function")
    {
        getNextToken();
        FunctionDefinitions();
    }
}

void SyntaxAnalyzer::Print()
{
    if (print)
    {
        std::cout << "\t<Print> -> put ( <Expression> );" << std::endl;
    }

    if (currentToken.lexeme != "(")
    {
        throw SyntaxError("Expected '('", currentToken.lineNumber);
    }

    getNextToken();
    Expression();

    if (currentToken.lexeme != ")")
    {
        throw SyntaxError("Expected ')'", currentToken.lineNumber);
    }
    getNextToken();

    if (currentToken.lexeme != ";")
    {
        throw SyntaxError("Expected ';'", currentToken.lineNumber);
    }

    getNextToken();
}

void SyntaxAnalyzer::Scan()
{
    if (print)
    {
        std::cout << "\t<Scan> -> get ( <IDs> );" << std::endl;
    }

    if (currentToken.lexeme != "(")
    {
        throw SyntaxError("Expected '('", currentToken.lineNumber);
    }

    getNextToken();
    IDs();

    if (currentToken.lexeme != ")")
    {
        throw SyntaxError("Expected ')'", currentToken.lineNumber);
    }

    getNextToken();
    if (currentToken.lexeme != ";")
    {
        throw SyntaxError("Expected ';'", currentToken.lineNumber);
    }

    getNextToken();
}

void SyntaxAnalyzer::TermPrime()
{
    if (print)
    {
        std::cout << "\t<TermPrime> -> * <Factor> <TermPrime> | / <Factor> <TermPrime> | <Empty>" << std::endl;
    }

    if (currentToken.lexeme == "*" | currentToken.lexeme == "/")
    {
        getNextToken();

        Factor();
        TermPrime();
    }
}

/**
 * Attempt to syntactically analyze a list of
 * Lexer tokens
 */
void SyntaxAnalyzer::Analyze()
{
    Rat18F();
    std::cout << "Syntax Analysis Successful." << std::endl;
}

void SyntaxAnalyzer::OptParameterList()
{
    if (print)
    {
        std::cout << "\t<Opt Parameter List> -> <Parameter List> | <Empty>" << std::endl;
    }

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
        throw SyntaxError("Expected ')' or identifier", currentToken.lineNumber);
    }
}

void SyntaxAnalyzer::ParameterList()
{
    if (print)
    {
        std::cout << "\t<Parameter List> -> <Parameter> | <Parameter> , <Parameter List>" << std::endl;
    }

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
    if (print)
    {
        std::cout << "\t<While> -> while ( <Condition> )  <Statement>" << std::endl;
    }

    if (currentToken.lexeme != "(")
    {
        throw SyntaxError("Expected '('", currentToken.lineNumber);
    }
    getNextToken();

    Condition();

    if (currentToken.lexeme != ")")
    {
        throw SyntaxError("Expected ')'", currentToken.lineNumber);
    }
    getNextToken();
    Statement();

    if (currentToken.lexeme != "whileend")
    {
        throw SyntaxError("Expected 'whileend' keyword", currentToken.lineNumber);
    }
    getNextToken();
}

void SyntaxAnalyzer::printCurrentToken()
{
    std::cout << std::left << std::endl
              << std::setw(8) << "Token:" << std::setw(16) << currentToken.token << std::setw(8) << "Lexeme:" << currentToken.lexeme << std::endl
              << std::endl;
}

SyntaxError::SyntaxError(std::string message, int lineNumber)
{
    this->message = message;
    this->lineNumber = lineNumber;
}

SyntaxError::~SyntaxError() {}

std::string SyntaxError::getMessage() const
{
    return (this->message + " Line: " + std::to_string(this->lineNumber));
}