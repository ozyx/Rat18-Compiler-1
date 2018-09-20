
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
