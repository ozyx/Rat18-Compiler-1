#include <iostream>
#include <fstream>
#include <iomanip>
#include "SyntaxAnalyzer.h"

int main()
{
	const int COL_SIZE = 20;

	std::ifstream fin;					  // Input file stream
	std::string inFile;					  // Input file name
	std::vector<Lexer::Token> lineTokens; // List of lineTokens
	std::vector<Lexer::Token> tokens;
	std::stringstream *buffer;
	std::string line;

	std::vector<std::string> files = {"test1.txt", "test2.txt", "test3.txt", "test4.txt"};
	std::ofstream out;
	out.open("output.txt");

	for (std::string file : files)
	{

		// Open the file
		fin.open(file.c_str());

		if (!fin)
		{
			std::cout << "file not found" << std::endl;
			continue;
		}

		out << std::endl
			<< "RUNNING TEST CASE FILE \"" << file << "\"" << std::endl
			<< std::endl;

		// File has opened, instantiate the lexer.
		Lexer *lexer = new Lexer();

		int lineNumber = 1;

		while (getline(fin, line))
		{
			buffer = new std::stringstream(line);
			lineTokens = lexer->lex(*buffer, lineNumber);

			tokens.insert(tokens.end(), lineTokens.begin(), lineTokens.end());

			lineNumber++;
		}
		fin.close();

		SyntaxAnalyzer *syntaxAnalyzer = new SyntaxAnalyzer(tokens, out, false);

		try
		{
			// Run syntactical analysis
			syntaxAnalyzer->Analyze();
		}
		catch (const SyntaxError &e)
		{
			out << std::endl
				<< "ERROR: " << e.getMessage();
		}

		tokens.clear();
		syntaxAnalyzer->PrintAll();

		delete syntaxAnalyzer;
	}

	std::cout << std::endl
			  << "EXECUTION HAS COMPLETED." << std::endl;
	std::cout << "Press enter to continue. . ." << std::endl;
	std::cin.get();

	return 0;
}