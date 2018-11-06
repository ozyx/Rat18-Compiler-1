#ifndef SYNTAXANALYZER_H
#define SYNTAXANALYZER_H

#include "Lexer.h"

class SyntaxAnalyzer
{
  public:
    SyntaxAnalyzer(std::vector<Lexer::Token> tokens, bool print = false);
    ~SyntaxAnalyzer();

  private:

    void Rat18F();
    void OptFunctionDefinitions();
    void FunctionDefinitions();
    void Function();
    void OptParameterList();
    void ParameterList();
    void Parameter();
    void Qualifier();
    void Body();
    void OptDeclarationList();
    void DeclarationList();
    void Declaration();
    void IDs();
    void StatementList();
    void Statement();
    void Compound();
    void Assign();
    void If();
    void Return();
    void Print();
    void Scan();
    void While();
    void Condition();
    void Relop();
    void Expression();
    void Term();
    void Factor();
    void Primary();
    void Empty();
    void ExpressionPrime();
    void TermPrime();
    void Identifier();
    
    void getNextToken();

    std::vector<Lexer::Token> tokens;
    std::vector<Lexer::Token>::iterator it;
    Lexer::Token currentToken;
    bool print;
};

#endif // SYNTAXANALYZER_H