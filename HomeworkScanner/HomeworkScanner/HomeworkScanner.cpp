#include<regex>
#include<iostream>
#include<queue>
#include<fstream>

std::queue<std::string>g_Tokens;

std::string parseQueue(const std::queue<std::string>q) {
	std::queue<std::string> queue = q;
	std::string str = "";
	while (queue.size() != 0) {
		str += queue.front();
		queue.pop();
	}
	return str;
}

void endVarCatch(std::queue<std::string>*var) {
	std::string varStr = parseQueue(*var);
	delete var;
	var = nullptr;
	std::cout << "var(" << varStr << ")";
	g_Tokens.push("VAR");
}

void endIntCatch(std::queue<std::string>*var) {
	std::string varStr = parseQueue(*var);
	delete var;
	var = nullptr;
	std::cout << "Int(" << varStr << ")";
	g_Tokens.push("INT");
}

std::string readFile(std::string filepath) {
	std::ifstream file(filepath);
	std::string str = "";
	if (file.is_open()) {
		std::string help = "";
		while (std::getline(file, help)) {
			str += help;
		}
	}
	else {
		return "";
	}
	return str;
}

std::string nextToken() {
	if (!g_Tokens.empty()) {
		g_Tokens.pop();
		return g_Tokens.front();
	}
	return "";
}

bool start(std::string &token);
bool e(std::string &token);
bool Rhs(std::string &token);

bool Rhs(std::string &token) {
	if (token == "VAR") {
		return true;
	}
	else if (token == "LPAR") {
		token = nextToken();
		if (token == "") {
			return false;
		}
		if (Rhs(token)) {
			token = nextToken();
			if (token == "") {
				return false;
			}
			if (token == "RPAR") {
				return true;
			}
		}
	}
	else if (Rhs(token)) {
		token = nextToken();
		if (token == "") {
			return false;
		}
		if (token == "PLUS") {
			token = nextToken();
			if (token == "") {
				return false;
			}
			if (Rhs(token)) {
				return true;
			}
			else {
				std::cout << "Failed to validate rhs after plus" << std::endl;
				return false;
			}
		}
		else if(token=="MUL"){
			token = nextToken();
			if (token == "") {
				return false;
			}
			if (Rhs(token)) {
				return true;
			}
			else {
				std::cout << "Failed to validate rhs after mul" << std::endl;
				return false;
			}
		}
		else if (token == "DIV") {
			token = nextToken();
			if (token == "") {
				return false;
			}
			if (Rhs(token)) {
				return true;
			}
			else {
				std::cout << "Failed to validate rhs after div" << std::endl;
				return false;
			}
		}
		else if (token == "SUB") {
			token = nextToken();
			if (token == "") {
				return false;
			}
			if (Rhs(token)) {
				return true;
			}
			else {
				std::cout << "Failed to validate rhs after sub" << std::endl;
				return false;
			}
		}
	}
	return false;
}

bool e(std::string &token) {
	if (token == "INT") {
		token = nextToken();
		if (token == "") {
			return false;
		}
		if (token == "SEMICOLON") {
			token = nextToken();
			if (token == "") {
				return false;
			}
			if (start(token)) {
				return true;
			}
		}
	}else if(Rhs(token)) {
		token = nextToken();
		if (token == "") {
			return false;
		}
		if (token == "SEMICOLON") {
			return true;
		}
	}
	std::cout << "e not validated" << std::endl;
	return false;
}

bool start(std::string &token) {
	if (token == "VAR") {
		token = nextToken();
		if (token == "") {
			return false;
		}
		if (token == "EQ") {
			token = nextToken();
			if (token == "") {
				return false;
			}
			if (e(token)) {
				return true;
			}
		}
	}
	std::cout << "S not validated" << std::endl;
	return false;
}

int main(int argc, char** argv) {

	if (argc < 3) {
		std::cout << "2 Arguments are required those are input type( 0 - file | 1 - string ) \nand input ( filepath | string )" << std::endl;
		std::cin.get();
		return -1;
	}

	std::string type = argv[1];
	std::string input = "";

	for (unsigned int i = 2; i < argc; ++i) {
		input += argv[i];
	}

	std::regex intCatch("([0-9])");
	std::regex varStartRegex("[a-zA-Z]");
	std::regex varContCatch("([a-zA-Z]|[0-9])");
	std::string str = "";

	if (type == "0") {
		str = readFile(input);
		if (str == "") {
			std::cout << "The file was either not found of empty" << std::endl;
			std::cin.get();
			return -1;
		}
	}
	else if (type == "1") {
		str = input;
	}
	else {
		std::cout << "Invalid Input Type" << std::endl;
		std::cin.get();
		return -1;
	}

	std::queue<std::string>*var		= nullptr;
	std::queue<std::string>*intq	= nullptr;

	bool numberCatch				= false;
	bool varCatch					= false;
	for (auto& c : str) {
		switch (c)
		{
		case(';'): {
			if (numberCatch) {
				numberCatch = false;
				endIntCatch(intq);
			}
			if (varCatch) {
				varCatch = false;
				endVarCatch(var);
			}
			std::cout << " SEMICOLON ";
			g_Tokens.push("SEMICOLON");
			break;
		}
		case('('): {
			if (numberCatch) {
				numberCatch = false;
				endIntCatch(intq);
			}
			if (varCatch) {
				varCatch = false;
				endVarCatch(var);
			}
			std::cout << " LPAR ";
			g_Tokens.push("LPAR");
			break;
		}
		case(')'): {
			if (numberCatch) {
				numberCatch = false;
				endIntCatch(intq);
			}
			if (varCatch) {
				varCatch = false;
				endVarCatch(var);
			}
			std::cout << " RPAR ";
			g_Tokens.push("RPAR");
			break;
		}
		case('*'): {
			if (numberCatch) {
				numberCatch = false;
				endIntCatch(intq);
			}
			if (varCatch) {
				varCatch = false;
				endVarCatch(var);
			}
			std::cout << " MUL ";
			g_Tokens.push("MUL");
			break;
		}
		case('+'): {
			if (numberCatch) {
				numberCatch = false;
				endIntCatch(intq);
			}
			if (varCatch) {
				varCatch = false;
				endVarCatch(var);
			}
			std::cout << " PLUS ";
			g_Tokens.push("PLUS");
			break;
		}
		case('-'): {
			if (numberCatch) {
				numberCatch = false;
				endIntCatch(intq);
			}
			if (varCatch) {
				varCatch = false;
				endVarCatch(var);
			}
			std::cout << " SUB ";
			g_Tokens.push("SUB");
			break;
		}
		case('\\'): {
			if (numberCatch) {
				numberCatch = false;
				endIntCatch(intq);
			}
			if (varCatch) {
				varCatch = false;
				endVarCatch(var);
			}
			std::cout << " DIV ";
			g_Tokens.push("DIV");
			break;
		}
		case('='): {
			if (numberCatch) {
				numberCatch = false;
				endIntCatch(intq);
			}
			if (varCatch) {
				varCatch = false;
				endVarCatch(var);
			}
			std::cout << " EQ ";
			g_Tokens.push("EQ");
			break;
		}
		case(0): {
			if (numberCatch) {
				numberCatch = false;
				endIntCatch(intq);
			}
			if (varCatch) {
				varCatch = false;
				endVarCatch(var);
			}
			break;
		}
		case('\t'): {
			if (numberCatch) {
				numberCatch = false;
				endIntCatch(intq);
			}
			if (varCatch) {
				varCatch = false;
				endVarCatch(var);
			}
			break;
		}
		case('\n'): {
			if (numberCatch) {
				numberCatch = false;
				endIntCatch(intq);
			}
			if (varCatch) {
				varCatch = false;
				endVarCatch(var);
			}
			break;
		}
		default:
			std::string toMatch = "";
			toMatch += c;
			if (!varCatch) {
				if (std::regex_match(toMatch, varStartRegex)) {
					if (numberCatch) {
						std::cout << "Ivalid variable name" << std::endl;
						return -1;
						/*
						numberCatch = false;
						endIntCatch(intq);
						*/
					}
					varCatch = true;
					var = new std::queue<std::string>();
					var->push(toMatch);
				}

				if (std::regex_match(toMatch, intCatch)) {
					if (!numberCatch) {
						numberCatch = true;
						intq = new std::queue<std::string>();
					}
					intq->push(toMatch);
				}
			}
			else {
				if (std::regex_match(toMatch, varContCatch)) {
					var->push(toMatch);
				}
				else {
					varCatch = false;
					endVarCatch(var);
				}
			}

			break;
		}
	}

	std::cout << "\n Finished Parsing String \n";

	std::string token = g_Tokens.front();
	bool s = start(token);

	if (s) {
		std::cout << "Valid" << std::endl;
	}
	else {
		std::cout << "Invalid" << std::endl;
	}

	std::cin.get();
	return 0;
}