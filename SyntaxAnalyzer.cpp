#include "SyntaxAnalyzer.h"

SyntaxAnalyzer::SyntaxAnalyzer(const std::vector<Lexer::Token> &tokens, std::ofstream &output, bool print) : tokens(tokens), it(tokens.begin()), currentToken(*(it)), output(output), save(nullptr)
{
	this->print = print;
	this->save = new Lexer::Token();
	this->errCount = 0;
	this->isDeclaration = false;
	this->assign = false;
}

SyntaxAnalyzer::~SyntaxAnalyzer()
{
}

void SyntaxAnalyzer::error(ErrorType errorType, int lineNumber, std::string expected)
{
	errCount++;
	err << "[ERR] (Line " << lineNumber << ") ";
	switch (errorType)
	{
	case TYPE_MISMATCH:
	{
		err << "TYPE MISMATCH";
		if (expected != "")
		{
			err << ". Expected \"" << expected << "\"";
		}
		break;
	}
	case DUPLICATE_SYMBOL:
	{
		err << "DUPLICATE SYMBOL";
		if (expected != "")
		{
			err << " \"" << expected << "\"";
		}
		break;
	}
	case UNDECLARED_VARIABLE:
	{
		err << "UNDECLARED VARIABLE";
		if (expected != "")
		{
			err << " \"" << expected << "\"";
		}

		break;
	}
	}
	err << std::endl;
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

	if (this->currentToken.token == "Illegal")
	{
		throw SyntaxError("Illegal symbol \'" + this->currentToken.lexeme + "\'", this->currentToken.lineNumber);
	}
}

// The root of the top-down parser
void SyntaxAnalyzer::Rat18F()
{
	if (print)
	{
		printCurrentToken();
		output << "\t<Rat18F> -> <Opt Function Definitions> $$ <Opt Declaration List> <Statement List>" << std::endl;
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
		output << "\t<Parameter> -> <IDs> : <Qualifier>" << std::endl;
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
		output << "\t<Function> ->  function <Identifier> ( <Opt Parameter List> ) <Opt Declaration List> <Body>" << std::endl;
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
		output << "\t<Opt Function Definitions> ->  <Function Definitions> | <Empty>" << std::endl;
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
		output << "\t<Opt Declaration List> -> <Declaration List> | <Empty>" << std::endl;
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
		output << "\t<Declaration List> -> <Declaration>; | <Declaration>; <Declaration List>\n";
	}

	// Save variable type
	savedType = new std::string(currentToken.lexeme);

	this->isDeclaration = true;

	Declaration();

	if (currentToken.lexeme == ";")
	{
		// Done with declaration, pop type stack
		symbolTable.pop_typestack();

		getNextToken();
		if (currentToken.lexeme == "real" | currentToken.lexeme == "boolean" | currentToken.lexeme == "int")
		{
			DeclarationList();
		}
	}

	this->isDeclaration = false;
}

void SyntaxAnalyzer::Declaration()
{
	if (print)
	{
		output << "\t<Declaration> -> <Qualifier> <IDs>" << std::endl;
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
		output << "\t<Qualifier> ->  int | boolean | real" << std::endl;
	}
}

void SyntaxAnalyzer::IDs()
{
	if (print)
	{
		output << "\t<IDs> -> <Identifier> | <Identifier>, <IDs>" << std::endl;
	}

	if (isDeclaration)
	{
		if (!symbolTable.lookup(currentToken))
		{
			symbolTable.insert(currentToken, *savedType);
		}
		else
		{
			error(DUPLICATE_SYMBOL, currentToken.lineNumber, currentToken.lexeme);
		}
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
		output << "\t<Identifier>" << std::endl;
	}
}

void SyntaxAnalyzer::StatementList()
{
	if (print)
	{
		output << "\t<Statement List> -> <Statement> | <Statement> <Statement List>" << std::endl;
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
		output << "\t<Statement> -> <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>" << std::endl;
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
		output << "\t<Compound> -> { <Statement List> }" << std::endl;
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
		output << "\t<Assign> -> <Identifier> = <Expression>;" << std::endl;
	}

	Identifier();

	// Save the value of the current token to gen instruction later
	*save = currentToken;
	std::string type = symbolTable.get_type(*save);
	if (type == "")
	{
		error(UNDECLARED_VARIABLE, currentToken.lineNumber, currentToken.lexeme);
		this->assign = true;
	}
	else
	{
		symbolTable.push_typestack(symbolTable.get_type(*save));
	}

	getNextToken();

	if (currentToken.lexeme != "=")
	{
		throw SyntaxError("Expected '='", currentToken.lineNumber);
	}

	getNextToken();
	Expression();

	symbolTable.gen_instr("POPM", symbolTable.get_address(*save));

	if (currentToken.lexeme != ";")
	{
		throw SyntaxError("Expected ';'", currentToken.lineNumber);
	}

	symbolTable.pop_typestack();

	getNextToken();
}

void SyntaxAnalyzer::Expression()
{
	if (print)
	{
		output << "\t<Expression> -> <Term> <ExpressionPrime>" << std::endl;
	}

	Term();
	ExpressionPrime();
}

void SyntaxAnalyzer::ExpressionPrime()
{
	if (print)
	{
		output << "\t<ExpressionPrime> -> + <Term> <ExpressionPrime> | - <Term> <ExpressionPrime> | <Empty>" << std::endl;
	}

	if (currentToken.lexeme == "+" | currentToken.lexeme == "-")
	{
		std::string op = currentToken.lexeme;
		getNextToken();

		Term();

		if (op == "+")
		{
			symbolTable.gen_instr("ADD", NIL);
		}
		else
		{
			symbolTable.gen_instr("SUB", NIL);
		}
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
		output << "\t<Term> -> <Factor> <TermPrime>" << std::endl;
	}

	Factor();
	TermPrime();
}

void SyntaxAnalyzer::Factor()
{
	if (print)
	{
		output << "\t<Factor> -> - <Primary> | <Primary>" << std::endl;
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
		output << "\t<Primary> -> <Identifier> | <Integer> | <Identifier> ( <IDs> ) | ( <Expression> ) | <Real> | true | false" << std::endl;
	}

	// If the last symbol was undeclared, clear the typestack
	if (!symbolTable.typestack_empty() && symbolTable.top_typestack() == "")
	{
		symbolTable.pop_typestack();
	}

	if (currentToken.token == "Identifier")
	{
		if (symbolTable.get_type(currentToken) == "")
		{
			if (!this->assign)
			{
				error(UNDECLARED_VARIABLE, currentToken.lineNumber, currentToken.lexeme);
			}
			this->assign = false;
		}

		// If typestack is empty, we should be within a Condition.
		// Push the current Identifier's type onto the stack to compare with
		// the next one we see.
		if (symbolTable.typestack_empty())
		{
			if (symbolTable.get_type(currentToken) != "")
			{
				symbolTable.push_typestack(*savedType);
			}
		}
		// If the Identifier doesn't have a type, it isn't in the symbol table.
		else if (symbolTable.get_type(currentToken) == "")
		{
			// error(UNDECLARED_VARIABLE, currentToken.lineNumber, currentToken.lexeme);
		}
		// Error TYPE MISMATCH
		else if ((symbolTable.get_type(currentToken) != symbolTable.top_typestack()) && symbolTable.top_typestack() != "")
		{
			error(TYPE_MISMATCH, currentToken.lineNumber, symbolTable.top_typestack());
		}

		Identifier();
		symbolTable.gen_instr("PUSHM", symbolTable.lookup(currentToken));

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
		// ERROR: TYPE MISMATCH
		if (!symbolTable.typestack_empty() && symbolTable.top_typestack() != "int")
		{
			error(TYPE_MISMATCH, currentToken.lineNumber, symbolTable.top_typestack());
		}

		Integer();
		symbolTable.gen_instr("PUSHI", stoi(currentToken.lexeme));
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
		// Error TYPE MISMATCH
		if (!symbolTable.typestack_empty() && symbolTable.top_typestack() != "real")
		{
			error(TYPE_MISMATCH, currentToken.lineNumber, symbolTable.top_typestack());
		}

		Real();
		getNextToken();
	}
	else if (currentToken.lexeme == "true")
	{
		// Error TYPE MISMATCH
		if (!symbolTable.typestack_empty() && symbolTable.top_typestack() != "boolean")
		{
			error(TYPE_MISMATCH, currentToken.lineNumber, symbolTable.top_typestack());
		}

		if (print)
		{
			output << "\ttrue" << std::endl;
		}

		symbolTable.gen_instr("PUSHI", 1);
		getNextToken();
	}
	else if (currentToken.lexeme == "false")
	{
		// Error TYPE MISMATCH
		if (!symbolTable.typestack_empty() && symbolTable.top_typestack() != "boolean")
		{
			error(TYPE_MISMATCH, currentToken.lineNumber, symbolTable.top_typestack());
		}

		if (print)
		{
			output << "\tfalse" << std::endl;
		}
		symbolTable.gen_instr("PUSHI", 0);
		getNextToken();
	}
}

void SyntaxAnalyzer::Integer()
{
	if (print)
	{
		output << "\t<Integer>" << std::endl;
	}
}

void SyntaxAnalyzer::Real()
{
	if (print)
	{
		output << "\t<Real>" << std::endl;
	}
}

void SyntaxAnalyzer::Return()
{
	if (print)
	{
		output << "\t<Return> -> return; |  return <Expression>;" << std::endl;
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
		output << "\t<If> -> if ( <Condition> ) <Statement> endif | if ( <Condition> ) <Statement> else <Statement> endif" << std::endl;
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

	if (currentToken.lexeme == "else")
	{
		Statement();
	}

	if (currentToken.lexeme != "ifend")
	{
		throw SyntaxError("Expected 'ifend' keyword", currentToken.lineNumber);
	}

	symbolTable.back_patch(symbolTable.get_instr_address());
	getNextToken();
}

void SyntaxAnalyzer::Condition()
{
	if (print)
	{
		output << "\t<Condition> -> <Expression>  <Relop>  <Expression>" << std::endl;
	}

	// Save variable type to push to typestack later
	savedType = new std::string(symbolTable.get_type(currentToken));

	if (*savedType == "")
	{
		error(UNDECLARED_VARIABLE, currentToken.lineNumber, currentToken.lexeme);
	}

	Expression();

	Relop();

	getNextToken();
	Expression();

	if (*savedOp == "<")
	{
		symbolTable.gen_instr("LES", NIL);
	}
	else if (*savedOp == ">")
	{
		symbolTable.gen_instr("GRT", NIL);
	}
	else if (*savedOp == "==")
	{
		symbolTable.gen_instr("EQU", NIL);
	}
	else if (*savedOp == "^=")
	{
		symbolTable.gen_instr("NEQ", NIL);
	}
	else if (*savedOp == "=>")
	{
		symbolTable.gen_instr("GEQ", NIL);
	}
	else if (*savedOp == "=<")
	{
		symbolTable.gen_instr("LEQ", NIL);
	}

	symbolTable.push_jumpstack(symbolTable.get_instr_address());
	symbolTable.gen_instr("JUMPZ", NIL);
}

void SyntaxAnalyzer::Relop()
{
	if (currentToken.lexeme != "==" && currentToken.lexeme != "^=" && currentToken.lexeme != ">" && currentToken.lexeme != "<" && currentToken.lexeme != "=>" && currentToken.lexeme != "=<")
	{
		throw SyntaxError("Expected relational operator", currentToken.lineNumber);
	}

	this->savedOp = new std::string(currentToken.lexeme);

	if (print)
	{
		output << "\t<Relop> -> " << currentToken.lexeme << std::endl;
	}
}

void SyntaxAnalyzer::Empty()
{
	if (print)
	{
		output << "\t<Empty> -> ε" << std::endl;
	}
}

void SyntaxAnalyzer::Body()
{
	if (print)
	{
		output << "\t<Body> -> { <Statement List> }" << std::endl;
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
		output << "\t<Function Definitions> -> <Function> | <Function> <Function Definitions>" << std::endl;
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
		output << "\t<Print> -> put ( <Expression> );" << std::endl;
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

	symbolTable.gen_instr("STDOUT", NIL);

	getNextToken();
}

void SyntaxAnalyzer::Scan()
{
	if (print)
	{
		output << "\t<Scan> -> get ( <IDs> );" << std::endl;
	}

	if (currentToken.lexeme != "(")
	{
		throw SyntaxError("Expected '('", currentToken.lineNumber);
	}

	getNextToken();

	symbolTable.gen_instr("STDIN", NIL);
	int addr = symbolTable.get_address(currentToken);
	symbolTable.gen_instr("POPM", addr);

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
		output << "\t<TermPrime> -> * <Factor> <TermPrime> | / <Factor> <TermPrime> | <Empty>" << std::endl;
	}

	if (currentToken.lexeme == "*" | currentToken.lexeme == "/")
	{
		std::string op = currentToken.lexeme;

		getNextToken();

		Factor();

		if (op == "*")
		{
			symbolTable.gen_instr("MUL", NIL);
		}
		else
		{
			symbolTable.gen_instr("DIV", NIL);
		}

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
	output << "Syntax Analysis Successful." << std::endl
		   << std::endl;
}

void SyntaxAnalyzer::OptParameterList()
{
	if (print)
	{
		output << "\t<Opt Parameter List> -> <Parameter List> | <Empty>" << std::endl;
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
		output << "\t<Parameter List> -> <Parameter> | <Parameter> , <Parameter List>" << std::endl;
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
		output << "\t<While> -> while ( <Condition> )  <Statement>" << std::endl;
	}

	int addr = symbolTable.get_instr_address();
	symbolTable.gen_instr("LABEL", NIL);

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
	symbolTable.gen_instr("JUMP", addr);
	symbolTable.back_patch(symbolTable.get_instr_address());

	getNextToken();
}

void SyntaxAnalyzer::printCurrentToken()
{
	output << std::left << std::endl
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

std::string SyntaxAnalyzer::PrintAll()
{
	std::ostringstream out;
	out << this->symbolTable.list();
	out << std::endl;
	out << this->symbolTable.list_instr();
	if (this->errCount > 0)
	{
		out << std::endl;
		out << errCount << " ERROR" << ((errCount > 1) ? "S" : "");
		out << " FOUND" << std::endl;
		out << std::setfill('-') << std::setw(15) << '-' << std::setfill(' ') << std::endl;
		out << err.str();
	}
	else
	{
		out << std::endl
			<< "3AC Code Generated Successfully!" << std::endl;
	}
	out << std::endl;

	return out.str();
}