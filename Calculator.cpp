#include <iostream>
#include <limits> // For numeric_limits

using namespace std;

// Class to handle calculator operations
class Calculator {
public:
    // Function to add two numbers
    double add(double a, double b) {
        return a + b;
    }

    // Function to subtract two numbers
    double subtract(double a, double b) {
        return a - b;
    }

    // Function to multiply two numbers
    double multiply(double a, double b) {
        return a * b;
    }

    // Function to divide two numbers
    // Returns 0 and prints error if division by zero is attempted
    double divide(double a, double b) {
        if (b == 0) {
            cerr << "Error: Division by zero is not allowed." << endl;
            return 0.0; // Return 0 or handle as specific error code
        }
        return a / b;
    }
};

int main() {
    Calculator calc;
    double num1, num2;
    char op;
    bool running = true;

    cout << "--- Simple Calculator Application ---" << endl;

    while (running) {
        cout << "\nEnter first number: ";
        while (!(cin >> num1)) {
            cout << "Invalid input. Please enter a number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        cout << "Enter operator (+, -, *, /) or 'q' to quit: ";
        cin >> op;

        if (op == 'q' || op == 'Q') {
            running = false;
            cout << "Exiting calculator..." << endl;
            continue;
        }

        cout << "Enter second number: ";
        while (!(cin >> num2)) {
            cout << "Invalid input. Please enter a number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        double result = 0.0;
        bool validOperation = true;

        switch (op) {
            case '+':
                result = calc.add(num1, num2);
                break;
            case '-':
                result = calc.subtract(num1, num2);
                break;
            case '*':
                result = calc.multiply(num1, num2);
                break;
            case '/':
                // Special check to avoid printing result if division failed due to zero
                if (num2 == 0) {
                    calc.divide(num1, num2); // Call just to print the error message
                    validOperation = false;
                } else {
                    result = calc.divide(num1, num2);
                }
                break;
            default:
                cout << "Error: Invalid operator." << endl;
                validOperation = false;
                break;
        }

        if (validOperation) {
            cout << "Result: " << num1 << " " << op << " " << num2 << " = " << result << endl;
        }
    }

    return 0;
}
