#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include "BST.h"

using namespace std;

//Input a list of numbers, and create a database of those numbers which will be printed for the user in out.txt
int main(int argc, char* argv[]) {

	ifstream in(argv[1]);
	if (!in) return 1;
	ofstream out(argv[2]);
	if (!out) return 2;
	
	BST<int> intTree;
	BST<string> stringTree;
	bool intMode = false, stringMode = false;
	out << std::boolalpha;

	string input, expression;
	while (getline(in, input)) {
		try {
			if (input.substr(0, 3) == "INT") {
				intMode = true; 
				stringMode = false;
				out << input << " " << intMode << endl;
				continue;
			}
			if (input.substr(0, 4) == "find") {
				if (stringMode) {
					out << input << " " << stringTree.find(input.substr(input.find(" "))) << endl;
				}
				else if (intMode) {
					out << input << " " << intTree.find(stoi(input.substr(input.find(" ")))) << endl;
				}
				continue;
			}
			if (input.substr(0, 4) == "tree") {
				out << input << endl;
				continue;
			}
			if (input.substr(0, 4) == "size") {
				if (stringMode) {
					out << input << " " << stringTree.size() << endl;
				}
				else if (intMode) {
					out << input << " " << intTree.size() << endl;
				}
				continue;
			}
			if (input.substr(0, 5) == "print") {
				out << input << ":";
				if (stringMode) {
					out << stringTree.toString();
				}
				else if (intMode) {
					out << intTree.toString();
				}
				continue;
			}
			if (input.substr(0, 5) == "clear") {
				if (stringMode) {
					out << input << " " << stringTree.clearTree() << endl;
				}
				else if (intMode) {
					out << input << " " << intTree.clearTree() << endl;
				}
				continue;
			}
			if (input.substr(0, 6) == "STRING") {
				intMode = false;
				stringMode = true;
				out << input << " " << stringMode << endl;
				continue;
			}
			if (input.substr(0, input.find(" ")) == "add") {
				if (stringMode) {
					out << input << " " << stringTree.addNode(input.substr(input.find(" "))) << endl;
				}
				else if (intMode) {
					out << input << " " << intTree.addNode(stoi(input.substr(input.find(" ")))) << endl;
				}
				continue;
			}
			if (input.substr(0, input.find(" ")) == "remove") {
				if (stringMode) {
					out << input << " " << stringTree.removeNode(input.substr(input.find(" "))) << endl;
				}
				else if (intMode) {
					out << input << " " << intTree.removeNode(stoi(input.substr(input.find(" ")))) << endl;
				}
				continue;
			}
		}
		catch (std::exception& e) {
			out << e.what() << endl;
		}
	}
	intTree.clearTree();
	stringTree.clearTree();
	in.close();
	out.close();
	return 0;
}