#include <iostream>
#include <concepts> // Requires C++20
#include <string>

using namespace std;

// === 1. Defining a Concept ===
// We define a custom concept named 'Number'.
// A type T satisfies 'Number' if it is either an integral type (int, long, etc.) 
// OR a floating-point type (float, double).
template <typename T>
concept Number = std::integral<T> || std::floating_point<T>;

// === 2. Using Concepts in Functions ===

// Simple Function Template restricted by the 'Number' concept.
// The syntax 'Number auto' is the "Abbreviated Function Template" syntax.
// It effectively says: "Accept any type, AS LONG AS it satisfies the Number concept."
void doubleAndPrint(Number auto n) {
    cout << "Double of " << n << " is " << (n * 2) << endl;
}

// === 3. Concept Overloading ===
// You can provide different implementations based on the concept satisfied.

// This version runs only for Integers
void process(std::integral auto n) {
    cout << "Processing Integral ID: " << n << endl;
}

// This version runs only for Floating Point numbers
void process(std::floating_point auto f) {
    cout << "Processing Precision Value: " << f << endl;
}

// === 4. Constrained Classes ===
// We can use concepts to restrict class template parameters too.
template <Number T>
class SafeContainer {
private:
    T value;
public:
    SafeContainer(T v) : value(v) {}
    
    void display() const {
        cout << "Container holds numeric value: " << value << endl;
    }
};

int main() {
    cout << "=== C++20 Concepts Demo ===" << endl;

    // --- Test 1: Constrained Functions ---
    cout << "\n1. Testing 'Number' constraint:" << endl;
    
    doubleAndPrint(10);      // Works: int is a Number
    doubleAndPrint(5.5);     // Works: double is a Number
    doubleAndPrint(100L);    // Works: long is a Number

    // The following lines would cause a clean Compile-Time Error if uncommented:
    // doubleAndPrint("Hello"); // Error: std::string does not satisfy 'Number'
    // doubleAndPrint(std::vector<int>{1,2}); // Error: vector does not satisfy 'Number'

    // --- Test 2: Overloading with Concepts ---
    cout << "\n2. Testing Overloading:" << endl;
    
    int myInt = 42;
    float myFloat = 3.14f;
    
    process(myInt);   // Calls the std::integral version
    process(myFloat); // Calls the std::floating_point version

    // --- Test 3: Constrained Classes ---
    cout << "\n3. Testing Constrained Class:" << endl;
    
    SafeContainer<int> intContainer(123);
    intContainer.display();

    SafeContainer<double> floatContainer(99.99);
    floatContainer.display();

    // SafeContainer<string> strContainer("Test"); // Compile Error: string is not a Number!

    return 0;
}
