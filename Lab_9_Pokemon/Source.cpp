#include "HashMap.h"
#include "Set.h"
using namespace std;

int main(int argc, char* argv[])
{
	ifstream in(argv[1]);
	if (!in) return 1;
	ofstream out(argv[2]);
	if (!out) return 2;
	out << std::boolalpha;

	Set<string> mySet;
	hash_map<string, string> pokemon;
	hash_map<string, string> moves;
	hash_map<string, Set<string>> effectives;
	hash_map<string, Set<string>> ineffectives;


	string input, tmp;
	while (getline(in, input)) {
		try {
			if (input.substr(0, 15) == "Ineffectivities") {
				out << endl << input << ": " << ineffectives.size() << "/" << ineffectives.max_size();
				out << ineffectives.toString() << endl;
				continue;
			}
			else if (input.substr(0, 13) == "Effectivities") {
				out << endl << input << ": " << effectives.size() << "/" << effectives.max_size();
				out << effectives.toString() << endl;
				continue;
			}
			else if (input.substr(0, input.find(" ")) == "Ineffective:") {
				out << input << endl;
				input = input.substr(input.find(" ") + 1);
				tmp = input.substr(input.find(" ") + 1);
				while (input.size() > 2) {
					ineffectives[input.substr(0, input.find(" "))].insert(tmp.substr(0, tmp.find(" ")));
					if (tmp.find(" ") == -1) {
						break;
					}
					tmp = tmp.substr(tmp.find(" ") + 1);
				}
				continue;
			}
			else  if (input.substr(0, input.find(" ")) == "Effective:") {
				out << input << endl;
				input = input.substr(input.find(" ") + 1);
				tmp = input.substr(input.find(" ") + 1);
				while (input.size() > 2) {
					effectives[input.substr(0, input.find(" "))].insert(tmp.substr(0, tmp.find(" ")));
					if (tmp.find(" ") == -1) {
						break;
					}
					tmp = tmp.substr(tmp.find(" ") + 1);
				}
				continue;
			}
			else if (input.substr(0, input.find(" ")) == "Move:") {
				out << input << endl;
				input = input.substr(input.find(" ") + 1);
				moves[input.substr(0, input.find(" "))] = input.substr(input.find(" ") + 1);
				continue;
			}
			else if (input.substr(0, input.find(" ")) == "Set:") {
				out << input << endl;
				for (size_t i = 5; i < input.size(); i+=4) {
					mySet.insert(input.substr(i, 3));
				}

				out << mySet.toString();
				mySet.clear();
				continue;
			}
			else if (input.substr(0, input.find(" ")) == "Pokemon:") {
				out << input << endl;
				input = input.substr(input.find(" ") + 1);
				pokemon[input.substr(0, input.find(" "))] = input.substr(input.find(" ") + 1);
				continue;
			}
			else if (input.substr(0, input.find(" ")) == "Battle:") {
				out << input << endl;
				string pokemon1, pokemon2, move1, move2;
				input = input.substr(input.find(" ") + 1);
				pokemon1 = input.substr(0, input.find(" "));
				input = input.substr(input.find(" ") + 1);
				move1 = input.substr(0, input.find(" "));
				input = input.substr(input.find(" ") + 1);
				pokemon2 = input.substr(0, input.find(" "));
				input = input.substr(input.find(" ") + 1);
				move2 = input;

				out << "\t" << pokemon1 << " (" << move1 << ") vs " << pokemon2 << " (" << move2 << ")" << endl;
				int move1Power = 0, move2Power = 0;
				if (effectives[moves[move1]].count(pokemon[pokemon2]) == 1) {
					move1Power = effectives[moves[move1]].count(pokemon[pokemon2]);
				}
				else {
					move1Power = -1 * ineffectives[moves[move1]].count(pokemon[pokemon2]);
				}
				if (effectives[moves[move2]].count(pokemon[pokemon1]) == 1) {
					move2Power = effectives[moves[move2]].count(pokemon[pokemon1]);
				}
				else {
					move2Power = -1 * ineffectives[moves[move2]].count(pokemon[pokemon1]);
				}

				out << "\t" << pokemon1 << "'s " << move1 << " is ";
				if (move1Power == 1) {
					out << "super effective against ";
				}
				if (move1Power == 0) {
					out << "effective against ";
				}
				if (move1Power == -1) {
					out << "ineffective against ";
				}
				out << pokemon2 << endl;

				out << "\t" << pokemon2 << "'s " << move2 << " is ";
				if (move2Power == 1) {
					out << "super effective against ";
				}
				if (move2Power == 0) {
					out << "effective against ";
				}
				if (move2Power == -1) {
					out << "ineffective against ";
				}
				out << pokemon1 << endl;

				if (move1Power == move2Power) {
					out << "\tThe battle between " << pokemon1 << " and " << pokemon2 << " is a tie." << endl;
				}
				if (move1Power > move2Power) {
					out << "\tIn the battle between " << pokemon1 << " and " << pokemon2 << ", " << pokemon1 << " wins!" << endl;
				}
				if (move1Power < move2Power) {
					out << "\tIn the battle between " << pokemon1 << " and " << pokemon2 << ", " << pokemon2 << " wins!" << endl;
				}

				continue;
			}
			else if (input.substr(0, 7) == "Pokemon") {
				out << endl << input << ": " << pokemon.size() << "/" << pokemon.max_size();
				out << pokemon.toString() << endl;
				continue;
			}
			else if (input.substr(0, 5) == "Moves") {
				out << endl << input << ": " << moves.size() << "/" << moves.max_size();
				out << moves.toString() << endl;
				continue;
			}
		}
		catch (std::exception& e) {
			out << e.what() << endl;
		}
	}
	pokemon.clear();
	moves.clear();

	return 0;
}
