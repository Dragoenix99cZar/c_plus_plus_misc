#include <iostream>
#include <stack>
#include <cmath>
#include <cassert>
#include <cstdio>

// Function to determine the precedence of an operator
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;
    return 0; // For parentheses
}

// Function to apply an operation
double applyOp(double a, double b, char op) {
    switch (op) {
        case '^': return pow(a, b);
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/':
            if (b == 0) {
                std::cerr << "Error: Division by zero!" << std::endl;
                exit(1); // Exit with error
            }
            return a / b;
        default: return 0;
    }
}

// Function to evaluate an infix expression
double evaluate(const char* expression) {
    std::stack<double> values;
    std::stack<char> ops;

    for (int i = 0; expression[i] != '\0'; ++i) {
        if (expression[i] == ' ') continue; // Skip spaces

        if (expression[i] == '(') {
            ops.push(expression[i]);
        } else if (isdigit(expression[i]) || expression[i] == '.') {
            double num = 0.0;
            int decimalFound = 0;
            int decimalPlaces = 0;
            while (expression[i] != '\0' && (isdigit(expression[i]) || expression[i] == '.')) {
                if (expression[i] == '.') {
                    decimalFound = 1;
                } else if (decimalFound) {
                    num += (expression[i] - '0') / pow(10.0, ++decimalPlaces);
                } else {
                    num = num * 10 + (expression[i] - '0');
                }
                ++i;
            }
            --i; // Adjust index after reading the full number
            values.push(num); // Push the number onto the stack
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
    const char* ex = "2 + 3";
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
    assert(abs(evaluate("3^2") - 9.0) < 1e-6);
    
    std::cout << "All tests passed!" << std::endl;
}

void pprint(const char* exp){
    fprintf(stdout, "%s: %f\n", exp, evaluate(exp));
}

int main(int argc, char* argv[]) {
    // testEvaluate();

    int i = 1;
    while (i < argc) {
        pprint(argv[i]);
        i++;
    }

    return 0;
}
