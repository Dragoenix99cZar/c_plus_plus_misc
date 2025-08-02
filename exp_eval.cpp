// g++ exp_eval.cpp -o expEval
// https://www.geeksforgeeks.org/expression-evaluation/

// CPP program to evaluate a given
// expression where tokens are 
// separated by space.
#include <bits/stdc++.h>
#include <cassert>
#include <math.h>
#include <stdio.h>
// using namespace std;

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;
    return 0; // For parentheses
}

// Function to perform an operation
double applyOp(double a, double b, char op) {
    switch (op) {
        case '^': return pow(a, b);
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/':
            if (b == 0) throw std::runtime_error("Division by zero!");
            return a / b;
        default: return 0;
    }
}

// Function to evaluate the infix expression
double evaluate(const std::string& expression) {
    std::stack<double> values;
    std::stack<char> ops;

    for (size_t i = 0; i < expression.length(); ++i) {
        if (expression[i] == ' ') continue; // Skip spaces

        if (expression[i] == '(') {
            ops.push(expression[i]);
        } else if (isdigit(expression[i]) || expression[i] == '.') {
            std::string num_str;
            while (i < expression.length() && (isdigit(expression[i]) || expression[i] == '.')) {
                num_str += expression[i];
                ++i;
            }
            --i; // Adjust index after reading the full number
            values.push(stod(num_str)); // Convert string to double
        } else if (expression[i] == ')') {
            while (!ops.empty() && ops.top() != '(') {
                double val2 = values.top();
                values.pop();
                double val1 = values.top();
                values.pop();
                char op = ops.top();
                ops.pop();
                values.push(applyOp(val1, val2, op));
            }
            if (!ops.empty()) ops.pop(); // Pop the '('
        } else {
            // Operator
            while (!ops.empty() && precedence(ops.top()) >= precedence(expression[i])) {
                double val2 = values.top();
                values.pop();
                double val1 = values.top();
                values.pop();
                char op = ops.top();
                ops.pop();
                values.push(applyOp(val1, val2, op));
            }
            ops.push(expression[i]);
        }
    }

    // Evaluate any remaining operators
    while (!ops.empty()) {
        double val2 = values.top();
        values.pop();
        double val1 = values.top();
        values.pop();
        char op = ops.top();
        ops.pop();
        values.push(applyOp(val1, val2, op));
    }

    return values.top();
}

void testEvaluate() {
    // Test cases
    std::string ex = "2 + 3";
    std::cout << ex << " : " << evaluate(ex) << "\n";
    assert(abs(evaluate("2 + 3") - 5.0) < 1e-6);
    assert(abs(evaluate("2 - 3") - (-1.0)) < 1e-6);
    assert(abs(evaluate("2 * 3") - 6.0) < 1e-6);
    assert(abs(evaluate("6 / 3") - 2.0) < 1e-6);
    assert(abs(evaluate("2 + 3 * 4") - 14.0) < 1e-6);
    assert(abs(evaluate("(2 + 3) * 4") - 20.0) < 1e-6);
    assert(abs(evaluate("10 / (2 + 3)") - 2.0) < 1e-6);
    assert(abs(evaluate("2.5 + 3.5") - 6.0) < 1e-6);
    assert(abs(evaluate("2.5 * 4") - 10.0) < 1e-6);
    assert(abs(evaluate("1.0 + 2.0 * 3.0 - 4.0 / 2.0") - 5.0) < 1e-6);
    assert(abs(evaluate("(5 + 3) * 2.5 - 10 / 2") - 15.0) < 1e-6);
    
    std::cout << "All tests passed!" << std::endl;
}

void pprint(std::string exp){
    // std::cout << exp << ": " << evaluate(exp) << "\n";
    fprintf(stdout, "%s: %f\n", exp.c_str(), evaluate(exp));
}

int main(int argc, char* argv[]) {
    // testEvaluate();
    // string ex = "10.2 + 2 * 6";
	// cout << ex << ": " << setprecision(2) << evaluate(ex) << "\n";
    // pprint("10.2 + 2 * 6");
    // pprint("100 * 2 + 12");
	// pprint("100 * ( 2 + 12 )");
	// pprint("100 * ( 2 + 12 ) / 14");
	// pprint("(5 + 3) * 2.5 - 10 / 2");
	// pprint("3^2");

    int i = 1;
    while (i < argc) {
        // cout << "Argument " << i << ": " << argv[i] << "\n";
        pprint(argv[i]);
        i++;
    }


	return 0;
}

// This code is contributed by Nikhil jindal.
