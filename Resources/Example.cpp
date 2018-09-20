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


/******************************************************************************
file: assign6.cc
this file is for printing(with flexible imputs)

David Williams-Haven
5-1-2017

structures
tree vitueal print struct
tree_num
tree_range
tree_qual
tree_op

functions
vector<string> lex(string s)
tree* parse_tree(it start, it finish)




******************************************************************************/
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <map>
#include <cassert>



using namespace std;
vector<string> lex(string s);
using it = vector<string>::iterator;



struct tree {
	virtual void print() = 0;
	virtual int eval() { return 0; }
	virtual string valu() { return 0; }
};

struct tree_num : public tree {
	tree_num(string v) {
		value = v;
	}
	int eval() { return 100; }
	string valu() { return value; }
	void print() { cout << value; }

	string value;
};
struct tree_op : public tree {

	tree_op(string v, tree*l, tree* r) {
		operand = v;
		left = l;
		right = r;
	}
	void print()
	{
		if (operand == ",")
		{
			left->print();
			//cout << operand;
			right->print();
		}
	
	}
	int eval() { return 90; }
	string operand;
	tree* right;
	tree* left;
};

struct tree_range : public tree {
	tree_range(string v, tree*l, tree* r, string boo) 
	{
		cout << " tree_range Constructor" << endl;
		int i = stoi(l->valu());
		if ((boo).compare("even") == 0 || (boo).compare("odd") == 0) {
			int x = stoi((r+1)->valu());
			for (; i <= x; i++) {
				leafs.push_back(new tree_num(to_string(i)));
			}
		}

	}
	tree_range(string v, tree*l, tree* r) {
		int i = stoi(l->valu());
		int x = stoi((r + 1)->valu());
		for (; i <= x; i++) {
			leafs.push_back(new tree_num(to_string(i)));
		}
	}
	
	void print() 
	{
		if (leafs.back()->valu().compare("even") == 0) 
		{
			for (tree* x : leafs) 
			{
				if ((stoi(x->valu()) % 2) == 0)
					cout << x->valu();
			}
		}
		else if (leafs.back()->valu().compare("even") == 0)
		{
			for (tree* x : leafs) 
			{
				if ((stoi(x->valu()) % 2) == 1)
					cout << x->valu();
			}
		}
		else {
			for (tree* x : leafs)
			{
				cout << x->valu();
			}
		}
	}
	int eval() { return 90; }
	string operand;
	vector<tree*> leafs;
};

struct tree_qual : public tree {
	tree_qual(string v) {
		value = v;
	}
	void print() {};
	int eval() { return 90; }
	string valu() { return value; }
	string value;
};

tree* parse_tree(it start, it finish);
int main() {
	string input;
	cout << ">";
	getline(cin, input);
	vector<string> words = lex(input);

	for (string s : words)
		cout << s << endl;

	tree* e;
	parse_tree(words.begin(), words.end() - 1)->print();
	cout << endl;
	cin >> input;


	return 0;
}

vector<string> lex(string s) {
	const int SPACE = 0;
	const int NUM = 1;
	const int OP = 2;
	const int QUAL = 3;

	int state = SPACE;
	vector<string> words;
	string word;

	for (char c : s) {
		if (state == SPACE)
		{
			if (c == ' ')
			{
				continue;
			}
			else if (c >= '0' && c <= '9')
				state = NUM;
			else if (c >= 'a' && c <= 'z')
				state = QUAL;
			else if (c == ',' || c == '-')
				state = OP;
			
			word.push_back(c);
		}
		else {
			int tempState = state;
			if (c == ' ') {
				tempState = SPACE;
				cout << "TempSpace = space" << endl;
			}	
			else if (c >= '0' && c <= '9'){
				tempState = NUM;
				cout << "TempSpace = NUM" << endl;
			}
				
			else if (c == ',' || c == '-') {
				tempState = OP;
				cout << "TempSpace = OP" << endl;
			}
			else if (c >= 'a' && c <= 'z') {
				tempState = QUAL;
				cout << "TempSpace = QUAL" << endl;
			}
				
			else {
				cout << " fail" << endl;
				//return;
			}
			
			cout << "State: " << state << endl;


			if (tempState == SPACE){
				words.push_back(word);
				word.clear();
				state = SPACE;
				// exit out of for loop
				continue;
			}
			//previous step == new step
			if (tempState == state ) {
				// if in qualifier extend it
				if(tempState == QUAL)
					word.push_back(c);
				// if in number extend it
				else if (tempState == NUM)
					word.push_back(c);
			}

			//good
			if (tempState != state && (state == OP || state == NUM)) {
				words.push_back(word);
				word.clear();
				word.push_back(c);
			}
			else if (tempState != state && state == QUAL) {
				cout << "create QUAL token" << endl;
				if (word.compare("to") == 0 ||
					word.compare("odd") == 0 ||
					word.compare("even") == 0) {
					words.push_back(word);
					word.clear();
					word.push_back(c);
				}
			}


			state = tempState;
		}
	}
	if (!word.empty())
		words.push_back(word);

	return words;
}

bool is_num(string s) {
	return(s.at(0) >= '0' && s.at(0) <= '9');
}

bool is_op(string s) {
	return s == ",";
}

bool is_range(string s) {
	return  s == "-" || s == "to";
}

bool is_qual(string s) {
	return s == "odd" || s == "even";
}

int precedence(string s){
	if (s == "," || s == "-" || s == "to")
	return 50;
	if (s == "odd" || s == "even")
		return 75;
}

tree* parse_tree(it start, it finish) {
	cout << "start: " << *start << endl;
	//cout << "finish -1: " << *(finish-1)  << endl;
	cout << "finish: " << *finish << endl;
	if (start > finish)
		return nullptr;
	else if (start == finish) {
		//cout << "parse_tree finished" << endl;
		if (is_num(*finish))
			return new tree_num{ *start };
		else if (is_qual(*finish)) {
			return new tree_qual{ *start };
		}
		else
			return nullptr;
	}

	else {
		for (it op = start + 1; op <= finish - 1; ++op) {
			if (is_op(*op)) {
				tree* left = parse_tree(start, op - 1);
				tree* right = parse_tree(op + 1, finish);
				if (left && right && left->eval() > precedence(*op) &&
					right->eval() > precedence(*op))
					return new tree_op(*op, left, right);
			}
			if (is_range(*op)) {
				tree* left = parse_tree(start, op - 1);
				cout << "op +1 " << *(op + 1) << "final: " << *finish << endl;
				tree* right = parse_tree(op + 1, finish);
				if (left && right && left->eval() > precedence(*op) &&
					right->eval() > precedence(*op))
					cout << "range_tree is created" << endl;

				
						if((right)->valu().compare("odd"))
							return new tree_range(*op, left, (right-1), "odd");
						else if (right->valu().compare("even"))
							return new tree_range(*op, left, (right - 1), "even");
						else {
							return new tree_range(*op, left, (right - 1));
						}
			}

		}
	}
	cout << "every test failed";
	return nullptr;
}
