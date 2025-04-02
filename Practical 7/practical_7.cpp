#include <iostream>
#include <map>
#include <set>
#include <vector>

using namespace std;

class CFG {
private:
    map<char, vector<string>> grammar;
    map<char, set<char>> firstSets;
    map<char, set<char>> followSets;
    set<char> visited;

    void computeFirst(char symbol) {
        if (firstSets[symbol].size() > 0 || visited.count(symbol)) return; 
        visited.insert(symbol); 

        for (const string &rule : grammar[symbol]) {
            bool hasEpsilon = true; 

            for (char ch : rule) {
                if (islower(ch) || ch == '(' || ch == ')') {
                    
                    firstSets[symbol].insert(ch);
                    hasEpsilon = false;
                    break;
                } else {
                    
                    computeFirst(ch);
                    firstSets[symbol].insert(firstSets[ch].begin(), firstSets[ch].end());

                    
                    if (firstSets[ch].count('e') == 0) {
                        hasEpsilon = false;
                        break;
                    }
                }
            }
            
            if (hasEpsilon) firstSets[symbol].insert('e');
        }
    }

    void computeFollow() {
        
        followSets['S'].insert('$');

        bool changed;
        do {
            changed = false;
            for (auto &entry : grammar) {
                char A = entry.first;
                for (const string &rule : entry.second) {
                    set<char> followNext = followSets[A];

                    for (int i = rule.size() - 1; i >= 0; i--) {
                        char B = rule[i];

                        if (isupper(B)) { 
                            int prevSize = followSets[B].size();

                            
                            followSets[B].insert(followNext.begin(), followNext.end());

                            
                            if (firstSets[B].count('e')) {
                                followNext.insert(firstSets[B].begin(), firstSets[B].end());
                                followNext.erase('e'); 
                            } else {
                                followNext = firstSets[B]; 
                            }

                            if (followSets[B].size() > prevSize) {
                                changed = true;
                            }
                        } else {
                            followNext = {B}; 
                        }
                    }
                }
            }
        } while (changed); 
    }

public:
    CFG() {
        
        grammar = {
            {'S', {"ABC", "D"}},
            {'A', {"a", "e"}},
            {'B', {"b", "e"}},
            {'C', {"(S)", "c"}},
            {'D', {"AC"}}
        };
    }

    void computeAllFirst() {
        for (auto &entry : grammar) {
            visited.clear(); 
            computeFirst(entry.first);
        }
    }

    void computeAllFollow() {
        computeFollow();
    }

    void printSets(const map<char, set<char>> &sets, const string &setName) {
        cout << setName << " Sets:\n";
        for (auto &entry : sets) {
            cout << setName << "(" << entry.first << ") = { ";
            for (char ch : entry.second) {
                if (ch == 'e') cout << "e"; 
                else cout << ch << " ";
            }
            cout << "}\n";
        }
    }

    void printFirstSets() {
        printSets(firstSets, "First");
    }

    void printFollowSets() {
        printSets(followSets, "Follow");
    }
};

int main() {
    CFG cfg;
    cfg.computeAllFirst();
    cfg.printFirstSets();
    cfg.computeAllFollow();
    cfg.printFollowSets();
    cout<<"22CE051"<<endl;
    return 0;
}
