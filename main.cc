#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <cmath>

bool imp(bool lhs, bool rhs) { return lhs | not(lhs ^ rhs); }

struct parseNode {
	std::string value;
	int position, hierarchy;
	parseNode(std::string v, int p, int h) { this->value = v; this->position = p; this->hierarchy = h; }
	parseNode(void);
};

class parser {
	#define strVector vector <std::string>
	#define strArray(sz) array <std::string, sz>
	int parseTreeCurrentHierarchy = 1;
	std::strVector names;
	std::vector <parseNode> parseTree;
	std::string f;
	int nameCount = 0;

public:
	parser(std::string s) { this->f = '(' + s + ')'; this->names = nameLookup(this->f); }
	parser(void);

	inline int getShift(std::string n, std::strVector& Namespace) {
		int i = Namespace.size()-1;
		for (std::string d : Namespace) {
			if (n == d)
				return i;
			--i;
		}
		return -1;
	}

	inline char isOperator(char c) {
		std::string operators = "&|^~yo";
		for (char o : operators) {
			if (c == o) return o;
		}
		return ' ';
	}

	inline std::string computeExpression(parseNode pNode) {
		std::string evaluated;
		std::strVector Namespace = nameLookup(pNode.value);
		const int nameCount = std::pow(Namespace.size(), 2);
		std::strVector evalExprs;
		evalExprs.resize(nameCount);
		for (int i = 0; i < evalExprs.size(); i++) { evalExprs[i] = pNode.value; }
		int ps = 0;
		unsigned long long n = 0;
		for (std::string s : evalExprs) {
			for (std::string nm : Namespace) {
				ps = s.find(nm);
				s.replace(ps, nm.size(), std::to_string((n & (1 << getShift(nm, Namespace))) != 0 ? 1 : 0));
			}
			int opp = 0;
			for (char c : s) {
				int lhs = 0, rhs = 0;
				char op = isOperator(c);
				if (op != ' ') {
					lhs = std::stoi(s.substr(1, opp-1));
					rhs = std::stoi(s.substr(opp+1, s.length()-1));
					switch(op) {
						case '&': evaluated += std::to_string(lhs & rhs); break;
						case '|': evaluated += std::to_string(lhs | rhs); break;
						case '^': evaluated += std::to_string(lhs ^ rhs); break;
						case 'y': evaluated += std::to_string(lhs & rhs); break;
						case 'o': evaluated += std::to_string(lhs | rhs); break;
					}
				}
				++opp;
			}
			++n;
		}
		return evaluated;
	}

	inline parseNode getInnermostCode() {
		parseNode pNode = parseNode("", 0, 0);
		int i = 1;
		for (char c : this->f) {
			if (c == '(') {
				pNode.position = i;
				pNode.value = "";
			} else {
				if (c == ')') {
					pNode.value += c;
					pNode.hierarchy = parseTreeCurrentHierarchy;
					++parseTreeCurrentHierarchy;
					this->f.replace(pNode.position-1, pNode.value.length(), "__Expr" + std::to_string(pNode.hierarchy) + "__");
					return pNode;
				}
			}
		pNode.value += c;
		++i;
		}
	}

	inline void generateParseTree() {
		parseTree.push_back(getInnermostCode());
		parseTree.push_back(getInnermostCode());
		this->f.replace(5, 7, "__Expr0__");
	}

	inline std::string processParseTree() {

	}

	inline bool nameExists(std::string p, std::strVector& Namespace) {
		for (std::string n : Namespace) {
			if (p == n) { return true; }
		}
		return false;
	}

	inline std::strVector nameLookup(std::string s) {
		bool lastIsCharacter = false;
		std::strVector nVec;
		std::string name;
		int i = 0;
		for (char c : s) {
			if ((std::isalpha(c) != 0) || (c == '_')) {
				name += c;
				lastIsCharacter = true;
			} else {
				if (lastIsCharacter) {
					if (not(nameExists(name, nVec))) {
						nVec.push_back(name);
						++this->nameCount;
					}
					name = "";
					lastIsCharacter = false;
				} else {
					lastIsCharacter = false;
				}
			}
		}
		if ((name.empty() != true) && not(nameExists(name, nVec))) {
			++this->nameCount;
			nVec.push_back(name);
		}
		return nVec;
	}

	inline int getNameCount() { return this->nameCount; }
	inline std::string getInternalString() { return this->f; }
	inline std::strVector& getNames() { return this->names; }
};

int main(int argc, const char** argv) {
	std::string s = argv[1];
	parser Parser = parser(s);
	parseNode pNode = Parser.getInnermostCode();
	std::cout << pNode.value << " = ";
	std::cout << Parser.computeExpression(pNode) << '\n';
}
