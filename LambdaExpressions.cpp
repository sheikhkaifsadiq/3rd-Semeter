#include <iostream>
#include <vector>
#include <algorithm> // For std::sort, std::for_each
#include <functional> // For std::function

using namespace std;

int main() {
    cout << "=== C++ Lambda Expressions Demo ===" << endl;

    // --- 1. Basic Lambda Syntax ---
    // Syntax: [capture](parameters) -> return_type { body }
    auto sayHello = []() {
        cout << "\n1. Hello from a simple Lambda!" << endl;
    };
    sayHello();

    // --- 2. Lambda with Parameters ---
    auto add = [](int a, int b) -> int {
        return a + b;
    };
    cout << "\n2. Sum of 5 + 3 = " << add(5, 3) << endl;

    // --- 3. Using Lambdas with STL Algorithms (Sorting) ---
    vector<int> numbers = {5, 2, 9, 1, 5, 6};
    
    cout << "\n3. Sorting a vector using Lambda:" << endl;
    cout << "Original: ";
    for(int n : numbers) cout << n << " ";
    cout << endl;

    // Sort in Descending order using a lambda comparator
    sort(numbers.begin(), numbers.end(), [](int a, int b) {
        return a > b; // Returns true if a should come before b
    });

    cout << "Sorted (Desc): ";
    // Use for_each with a lambda to print
    for_each(numbers.begin(), numbers.end(), [](int n) {
        cout << n << " ";
    });
    cout << endl;

    // --- 4. Capturing Variables (The [] clause) ---
    cout << "\n4. Capturing Variables:" << endl;
    int multiplier = 3;
    int counter = 0;

    // [=] captures all local variables by VALUE (read-only)
    // [&] captures all local variables by REFERENCE (read-write)
    // [multiplier] captures only 'multiplier' by value
    
    vector<int> values = {1, 2, 3, 4};
    
    // We want to count how many numbers are even and print the number * multiplier
    for_each(values.begin(), values.end(), [=, &counter](int n) {
        // 'multiplier' is captured by value (read-only)
        // 'counter' is captured by reference (we can modify it)
        
        if (n % 2 == 0) {
            counter++; // Increment external counter
            cout << n << " * " << multiplier << " = " << (n * multiplier) << endl;
        }
    });

    cout << "Total even numbers found: " << counter << endl;

    // --- 5. Mutable Lambdas ---
    // By default, capture-by-value makes variables const inside the lambda.
    // Use 'mutable' to modify a copy inside the lambda.
    int x = 10;
    auto modifyCopy = [x]() mutable {
        x += 5; // Modifies the internal COPY of x, not the original
        cout << "\n5. Inside mutable lambda, x = " << x << endl;
    };
    
    modifyCopy();
    cout << "   Outside lambda, original x = " << x << " (unchanged)" << endl;

    return 0;
}
