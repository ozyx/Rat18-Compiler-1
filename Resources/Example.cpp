/*
* lexical..c
*/

#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <map>
#include <stdexcept>
#include <cmath>

using std::string;
using std::vector;
using std::cout;
using std::cin;
using std::getline;
using std::endl;

vector<string> lex(string s);

std::map<string, int> variables;

using it = vector<string>::iterator;

struct expt {
	virtual int eval() { return 0; }

	virtual void print() = 0;

	virtual expt* replace(string, int) = 0;

};

struct exp_num : public expt {
	exp_num(int v) {
		value = v;
	}

	int eval() { return value; }

	void print() { cout << value; }

	expt* replace(string, int) { return this; }

	float value;
};

struct exp_var : public expt {
	exp_var(string n) {
		name = n;
	}

	int eval() {
		try {
			return variables[name];
		}
		catch (...) {
			return 0;
		}
	}

	expt* replace(string n, int v) {
		if (name == n)
			return new exp_num{ v };
		else
			return this;
	}

	void print() { cout << name; }

	string name;
};

struct exp_paren : public expt {
	exp_paren(expt* i) {
		inner = i;
	}

	int eval() { return inner->eval(); }
	void print() {
		cout << "(";
		inner->print();
		cout << ")";
	}

	expt* inner;
};

struct exp_minus : public expt {
	exp_minus(expt* i) {
		inner = i;
	}

	int eval() { return -inner->eval(); }

	void print() {
		cout << "-(";
		inner->print();
		cout << ")";
	}

	expt* replace(string n, int v) {
		return new exp_minus{ inner->replace(n,v) };
	}

	expt* inner;
};

int op_prec(string op) {
	if (op == "+" || op == "-")
		return 50;
	else if (op == "*" || op == "/")
		return 75;
	else
		return 0;
}

struct exp_op : public expt {
	exp_op(string op, expt* l, expt* r) {
		oper = op;
		left = l;
		right = r;
	}

	int eval() {
		if (oper == "+")
			return left->eval() + right->eval();
		else if (oper == "-")
			return left->eval() - right->eval();
		else if (oper == "*")
			return left->eval() * right->eval();
		else if (oper == "/")
			return left->eval() / right->eval();
		else
			return 0;
	}

	void print() {
		left->print();
		cout << oper;
		right->print();
	}

	expt* replace(string n, int v) {
		return new exp_op{ oper, left->replace(n,v), right->replace(n,v) };
	}

	string oper;
	expt* left;
	expt* right;
};

struct function_def {
	string var;
	expt* body;
};

std::map<string, function_def> functions;

struct exp_func : public expt {
	exp_func(string n, expt* a) {
		name = n;
		arg = a;
	}

	void print() {
		cout << name << "(";
		arg->print();
		cout << ")";
	}

	expt* replace(string n, int v) {
		return new exp_func{ name, arg->replace(n, v) };
	}

	int eval() {
		if (name == "sin")
			return int(std::sin(arg->eval()) * 100);
		else if (name == "sqrt")
			return int(std::sqrt(arg->eval()));
		else if (name == "exp")
			return int(std::exp(arg->eval()));
		else {
			expt* body = functions[name].body;
			string v = functions[name].var;

			expt* expansion = body->replace(v, arg->eval());
			return expansion->eval();
		}

	}

	string name;
	expt* arg;
};

struct exp_assign : public expt {
	exp_assign(string n, expt* r) {
		name = n;
		rhs = r;
	}

	void print() {
		cout << name << " = ";
		rhs->print();
	}

	int eval() {
		int rv = rhs->eval();
		variables[name] = rv;
		return rv;
	}

	expt* replace(string, int) {
		return this;
	}

	string name;
	expt* rhs;
};

bool is_expr(it start, it finish);
expt* parse_exp(it start, it finish);



bool is_var(string s) {
	return (s.at(0) >= 'a' && s.at(0) <= 'z') || s.at(0) == '_';
}

int main() {

	while (true) {
		string input;
		cout << "> ";
		getline(cin, input);
		vector<string> words = lex(input); // Split into words

										   // Print the individual words
		for (string s : words)
			cout << "\"" << s << "\",";
		cout << endl;

		// Function definitions
		if (words.size() >= 6 &&
			is_var(words[0]) && words[1] == "(" && is_var(words[2]) && words[3] == ")") {
			expt* body = parse_exp(words.begin() + 5, words.end() - 1);
			if (body)
				functions[words[0]] = function_def{ words[2], body };
		}

		expt* p = parse_exp(words.begin(), words.end() - 1);
		if (p) {
			cout << "Value: " << p->eval() << endl;
			p->print(); cout << endl;
		}
	}

	return 0;
}

bool is_num(string s) {
	try {
		stoi(s);
		return true;
	}
	catch (std::invalid_argument& e) {
		return false;
	}
}

bool is_op(string s) {
	return s == "+" || s == "*" || s == "/" || s == "-";
}

expt* parse_exp(it start, it finish);
expt* parse_assign(it start, it finish);
expt* parse_sum(it start, it finish);
expt* parse_prod(it start, it finish);
expt* parse_neg(it start, it finish);
expt* parse_unit(it start, it finish);

expt* parse_exp(it start, it finish) {
	return parse_assign(start, finish);
}

expt* parse_assign(it start, it finish) {
	if (finish - start >= 2 && is_var(*start) && *(start + 1) == "=") {
		expt* rhs = parse_assign(start + 2, finish);
		if (rhs)
			return new exp_assign{ *start, rhs };
	}

	return parse_sum(start, finish);
}

expt* parse_sum(it start, it finish) {
	//cout << "Parse sum\n";
	for (it op = start + 1; op <= finish - 1; ++op) {
		if (*op == "+" || *op == "-") {
			expt* left = parse_sum(start, op - 1);
			expt* right = parse_prod(op + 1, finish);
			if (left && right)
				return new exp_op{ *op, left, right };
		}
	}

	return parse_prod(start, finish);
}

expt* parse_prod(it start, it finish) {
	//cout << "Parse prod\n";
	for (it op = start + 1; op <= finish - 1; ++op) {
		if (*op == "*" || *op == "/") {
			expt* left = parse_prod(start, op - 1);
			expt* right = parse_neg(op + 1, finish);
			if (left && right)
				return new exp_op{ *op, left, right };
		}
	}

	return parse_neg(start, finish);
}

expt* parse_neg(it start, it finish) {
	//cout << "Parse neg\n";
	if (*start == "-") {
		expt* e = parse_unit(start + 1, finish);
		if (e)
			return new exp_minus{ e };
	}

	return parse_unit(start, finish);
}

expt* parse_unit(it start, it finish) {
	//cout << "Parsing unit" << endl;

	if (start > finish)
		return nullptr;
	else if (start == finish) {
		if (is_num(*start))
			return new exp_num{ stoi(*start) };
		else if (is_var(*start))
			return new exp_var{ *start };
		else
			return nullptr;
	}
	else {
		// Parenthesized expressions
		if (*start == "(" && *finish == ")") {
			expt* e = parse_exp(start + 1, finish - 1);
			if (e)
				return e;
		}

		// Function calls
		if (is_var(*start) && *(start + 1) == "(" && *finish == ")") {
			expt* e = parse_exp(start + 2, finish - 1);

			if (e)
				return new exp_func{ *start, e };
		}
	}

	return nullptr;
}

bool is_expr(it start, it finish) {
	if (start > finish)
		return false;
	else if (start == finish) {
		if (is_num(*start) || is_var(*start))
			return true;
		else
			return false;
	}
	else {
		if (*start == "-" && is_expr(start + 1, finish))
			return true;

		if (*start == "(" && *finish == ")" &&
			is_expr(start + 1, finish - 1))
			return true;

		for (it op = start + 1; op <= finish - 1; ++op) {
			if (is_op(*op) &&
				is_expr(start, op - 1) &&
				is_expr(op + 1, finish))
				return true;
		}

		return false;
	}
}

vector<string> lex(string s) {
	const int SPACE = 0;
	const int NUM = 1;
	const int DEC = 2;
	const int VAR = 3;
	const int OP = 4;
	const int PAREN = 5;

	int state = SPACE;
	vector<string> words;
	string word;

	for (char c : s) {
		if (state == SPACE) {
			if (c == ' ') {
				continue; // Ignore, stay in SPACE
			}
			else if (c >= '0' && c <= '9')
				state = NUM;
			else if ((c >= 'a' && c <= 'z') || c == '_')
				state = VAR;
			else if (c == '(' || c == ')')
				state = PAREN;
			else if (c == '+' || c == '*' || c == '-' || c == '/' || c == '=')
				state = OP;

			word.push_back(c);
		}
		else {
			// Not in SPACE
			int newstate = state;
			if (c == ' ')
				newstate = SPACE;
			else if (c >= '0' && c <= '9')
				newstate = NUM;
			else if ((c >= 'a' && c <= 'z') || c == '_')
				newstate = VAR;
			else if (c == '(' || c == ')')
				newstate = PAREN;
			else if (c == '+' || c == '*' || c == '-' || c == '/' || c == '=')
				newstate = OP;

			if (newstate == SPACE) {
				words.push_back(word);
				word.clear();
				state = SPACE;
				continue;
			}

			if ((newstate == state && state != PAREN && state != OP) ||
				(state == NUM && newstate == DEC))
				word.push_back(c);
			else if (newstate != state || state == PAREN || state == OP) {
				words.push_back(word);
				word.clear();
				word.push_back(c);
			}

			state = newstate;
		}
	}

	if (!word.empty()) // state != SPACE
		words.push_back(word);

	return words;
}
