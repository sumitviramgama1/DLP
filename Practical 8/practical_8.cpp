#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stack>
#include <set>
using namespace std;

map<char, vector<string>> grammar;
map<char, set<char>> firstSet, followSet;
map<pair<char, char>, string> parsingTable;

void initializeSets() {
    // First sets
    firstSet['S'] = {'a', 'b', '(', 'c'};
    firstSet['A'] = {'a', 'e'};
    firstSet['B'] = {'b', 'e'};
    firstSet['C'] = {'(', 'c'};
    firstSet['D'] = {'a', '(', 'c'};

    // Follow sets
    followSet['S'] = {'$', ')'};
    followSet['A'] = {'b', '(', 'c'};
    followSet['B'] = {'(', 'c'};
    followSet['C'] = {'$', ')'};
    followSet['D'] = {'$', ')'};
}

void initializeGrammar() {
    grammar['S'] = {"ABC", "D"};
    grammar['A'] = {"a", "e"};
    grammar['B'] = {"b", "e"};
    grammar['C'] = {"(S)", "c"};
    grammar['D'] = {"AC"};
}

void constructParsingTable() {
    // S productions
    parsingTable[{'S', 'a'}] = "ABC";
    parsingTable[{'S', 'b'}] = "ABC";
    parsingTable[{'S', '('}] = "ABC";
    parsingTable[{'S', 'c'}] = "D";

    // A productions
    parsingTable[{'A', 'a'}] = "a";
    parsingTable[{'A', 'b'}] = "e";
    parsingTable[{'A', '('}] = "e";
    parsingTable[{'A', 'c'}] = "e";

    // B productions
    parsingTable[{'B', 'b'}] = "b";
    parsingTable[{'B', '('}] = "e";
    parsingTable[{'B', 'c'}] = "e";

    // C productions
    parsingTable[{'C', '('}] = "(S)";
    parsingTable[{'C', 'c'}] = "c";

    // D productions
    parsingTable[{'D', 'a'}] = "AC";
    parsingTable[{'D', '('}] = "AC";
    parsingTable[{'D', 'c'}] = "AC";
}

bool validateString(const string &input) {
    stack<char> parseStack;
    parseStack.push('$');
    parseStack.push('S');
    size_t i = 0;

    while(!parseStack.empty() && i <= input.size()) {
        char top = parseStack.top();
        char currentInput = (i < input.size()) ? input[i] : '$';

        if(top == currentInput) {
            parseStack.pop();
            i++;
        }
        else if(isupper(top)) {
            string production = parsingTable[{top, currentInput}];

            if(production.empty()) {
                // Check for epsilon production
                if(firstSet[top].count('e') &&
                   followSet[top].count(currentInput)) {
                    parseStack.pop();
                    continue;
                }
                return false;
            }

            parseStack.pop();
            for(int j = production.size()-1; j >= 0; j--) {
                if(production[j] != 'e') {
                    parseStack.push(production[j]);
                }
            }
        }
        else {
            return false;
        }
    }

    return parseStack.empty() && i == input.size()+1;
}

int main() {
    initializeSets();
    initializeGrammar();
    constructParsingTable();


    string testCases[] = {
        "abc",
        "ac",
        "(abc)",
        "c",
        "(ac)",
        "a",
        "( )",
        "(ab)",
        "abcabc",
        "b"
    };

    cout << "Test Results:\n";
    cout << "-------------\n";
    for(string input : testCases) {
        cout << "Input: " << input << " - ";
        if(validateString(input))
            cout << "Valid string" << endl;
        else
            cout << "Invalid string" << endl;
    }

    cout<<"22CE051";

    return 0;
}
