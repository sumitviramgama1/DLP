#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cctype>
using namespace std;

// Check if the given string is a number
bool isNumber(const string &s) {
    for (char c : s) {
        if (!isdigit(c) && c != '.') return false;
    }
    return true;
}

// Perform arithmetic operations
double applyOperation(double a, double b, const string &op) {
    if (op == "+") return a + b;
    if (op == "-") return a - b;
    if (op == "*") return a * b;
    if (op == "/") return a / b;
    return 0;
}

// Evaluate the constant folding expression with precedence
string evaluateExpression(const string &expr) {
    istringstream iss(expr);
    vector<string> tokens;
    string token;

    while (iss >> token) tokens.push_back(token);

    // First pass: Handle * and /
    for (int i = 0; i < tokens.size(); i++) {
        if (i + 2 < tokens.size() && (tokens[i + 1] == "*" || tokens[i + 1] == "/") && isNumber(tokens[i]) && isNumber(tokens[i + 2])) {
            double left = stod(tokens[i]);
            double right = stod(tokens[i + 2]);
            string op = tokens[i + 1];
            double result = applyOperation(left, right, op);

            tokens[i] = to_string(result);
            tokens.erase(tokens.begin() + i + 1, tokens.begin() + i + 3);
            i--;
        }
    }

    // Second pass: Handle + and -
    for (int i = 0; i < tokens.size(); i++) {
        if (i + 2 < tokens.size() && (tokens[i + 1] == "+" || tokens[i + 1] == "-") && isNumber(tokens[i]) && isNumber(tokens[i + 2])) {
            double left = stod(tokens[i]);
            double right = stod(tokens[i + 2]);
            string op = tokens[i + 1];
            double result = applyOperation(left, right, op);

            tokens[i] = to_string(result);
            tokens.erase(tokens.begin() + i + 1, tokens.begin() + i + 3);
            i--;
        }
    }

    string result;
    for (const string &t : tokens) result += t + " ";
    return result;
}

int main() {
    string expr;
    while (true) {
        cout << "\nEnter expression : ";
        getline(cin, expr);
        if (expr == "exit") break;
        cout << "Optimized expression: " << evaluateExpression(expr) << endl;
    }
    return 0;
}
