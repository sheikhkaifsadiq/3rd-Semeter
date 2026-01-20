#include <iostream>
#include <vector>
#include <string>
#include <stdexcept> // For standard exceptions

using namespace std;

// === Template Class: Stack ===
// T is a placeholder for any data type (int, double, string, etc.)
template <typename T>
class Stack {
private:
    vector<T> elements;

public:
    // Push an element onto the stack
    void push(T const& value) {
        elements.push_back(value);
    }

    // Remove the top element
    void pop() {
        if (elements.empty()) {
            cout << "Stack Underflow: Cannot pop from an empty stack." << endl;
            return;
        }
        elements.pop_back();
    }

    // Return the top element
    T top() const {
        if (elements.empty()) {
            throw out_of_range("Stack is empty");
        }
        return elements.back();
    }

    // Check if stack is empty
    bool isEmpty() const {
        return elements.empty();
    }

    // Display stack contents
    void display() const {
        if (elements.empty()) {
            cout << "(Empty Stack)" << endl;
            return;
        }
        cout << "[ ";
        // Iterate backwards to show top first
        for (int i = elements.size() - 1; i >= 0; i--) {
            cout << elements[i] << " ";
        }
        cout << "]" << endl;
    }
};

int main() {
    cout << "=== Generic Stack (Templates) ===" << endl;

    // --- Integer Stack ---
    cout << "\n1. Testing Stack<int>:" << endl;
    Stack<int> intStack;
    
    intStack.push(10);
    intStack.push(20);
    intStack.push(30);
    cout << "Stack after pushes: ";
    intStack.display();

    cout << "Top element: " << intStack.top() << endl;

    intStack.pop();
    cout << "Stack after pop:    ";
    intStack.display();

    // --- String Stack ---
    cout << "\n2. Testing Stack<string>:" << endl;
    Stack<string> stringStack;

    stringStack.push("Hello");
    stringStack.push("World");
    stringStack.push("C++");
    cout << "Stack after pushes: ";
    stringStack.display();

    cout << "Top element: " << stringStack.top() << endl;

    stringStack.pop();
    cout << "Stack after pop:    ";
    stringStack.display();

    // --- Exception Handling Demo ---
    cout << "\n3. Testing Empty Stack Exception:" << endl;
    Stack<double> emptyStack;
    try {
        cout << "Attempting to get top of empty stack..." << endl;
        cout << emptyStack.top() << endl; 
    } catch (const exception& e) {
        cerr << "Exception caught: " << e.what() << endl;
    }

    return 0;
}
