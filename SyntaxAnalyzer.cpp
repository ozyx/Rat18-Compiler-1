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
        throw new SyntaxError("Expected '$$'.", currentToken.lineNumber);
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
            // TODO: error
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
        // TODO: throw error
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
        // TODO: throw error
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
        // TODO: throw error
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

void SyntaxAnalyzer::Analyze()
{
    Rat18F();
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
        // TODO: throw error
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
    if(print)
    {
        std::cout << "\t<While> -> while ( <Condition> )  <Statement>" << std::endl;
    }

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

void SyntaxAnalyzer::printCurrentToken()
{
    std::cout << std::endl
              << std::setw(8) << "Token: " << currentToken.token << std::endl
              << std::setw(8) << "Lexeme: " << currentToken.lexeme << std::endl
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