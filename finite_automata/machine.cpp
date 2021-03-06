#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <vector>
#include "Machine.h"

std::vector<std::string> split(const std::string& input, const std::string& delimiter) {
    std::vector<std::string> split_string;
    
    size_t begin = 0;
    while (begin < input.size()) {
        size_t end = input.find(delimiter, begin);

        if (end == std::string::npos) {
            end = input.size();
        }

        std::string substr = input.substr(begin, end - begin);
        split_string.push_back(substr);
        begin = end + 1;
    }

    return split_string;
}

std::set<State> createStates(const std::vector<std::string>& list) {
    std::set<State> states;
    for (int i = 1; i < list.size(); i++) {
        states.insert(State(list[i]));
    }

    return states;
}

machine::NFA readDFAFile(const std::string& path) {
    std::string line;
    std::ifstream file(path);

    std::set<State> Q;
    std::set<State> F;
    State q0;
    TransitionFunction delta;

    if (file.is_open()) {
        while (getline(file, line)) {
            if (line.size() > 0 && line[0] != '#' ) {
                std::vector<std::string> split_string = split(line, " ");
                std::string key = split_string[0];

                if (key == "Q") {
                    Q = createStates(split_string);
                }
                else if (key == "delta") {
                    const std::string& state = split_string[1];
                    const std::string& symbol = split_string[2];
                    for (int i = 3; i < split_string.size(); i++) {
                        delta.addTransition(State(state), symbol, State(split_string[i]));
                    }
                }
                else if (key == "q0") {
                    q0 = State(split_string[1]);
                }
                else if (key == "F") {
                    F = createStates(split_string);
                }
            }
        }
        file.close();
    }

    return machine::NFA(Q, delta, q0, F);
}

std::vector<std::string> readLanguageFile(const std::string& path) {
    std::string line;
    std::ifstream file(path);

    std::vector<std::string> words;

    if (file.is_open()) {
        while (getline(file, line)) {
            if (line[0] != '#' ) {
                words.push_back(line);
            }
        }
        file.close();
    }

    return words;
}

int main(int argc, char* argv[]) {
    if (argc != 3 || (argc == 2 && std::string(argv[1]) == "help")) {
        std::cout << "Verifies if a Language is accepted by a Finite Automata.\n";
        std::cout << "Usage:\n\tmachine <PATH_TO_MACHINE_FILE> <PATH_TO_LANGUAGE_FILE>\n";
        return 1;
    }

    machine::NFA nfa = readDFAFile(std::string(argv[1]));
    std::vector<std::string> words = readLanguageFile(std::string(argv[2]));

    std::cout << "***** Validating language *****\n\n";
    bool languageAccepted = true;
    for (auto& word : words) {
        bool accepts = nfa.accept(word);

        if (!accepts) {
            languageAccepted = false;
        }
    }

    std::cout << "\n***** Language is " << (languageAccepted? "accepted" : "rejected") << " by the machine. *****\n";

    return 0;
}

