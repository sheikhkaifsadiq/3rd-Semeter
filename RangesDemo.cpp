#include <iostream>
#include <vector>
#include <ranges> // Requires C++20
#include <algorithm>

using namespace std;

int main() {
    cout << "=== C++20 Ranges & Views Demo ===" << endl;

    // A list of numbers
    vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    cout << "Original: ";
    for(int n : numbers) cout << n << " ";
    cout << endl;

    // --- The "Old" Way ---
    // To filter evens, square them, and take the first few, 
    // we would typically write a loop with a temporary vector or nested calls.

    // --- The "New" Way (Ranges Pipelines) ---
    // We want to:
    // 1. Filter only EVEN numbers
    // 2. Square them (Transform)
    // 3. Take the first 3 results
    // 4. Reverse the order
    
    // Note: 'views' create a lightweight "view" into the data. 
    // The operations are lazy; they only happen when we iterate over 'result'.
    auto result = numbers 
                | views::filter([](int n) { return n % 2 == 0; })   // Keep evens: 2, 4, 6, 8, 10...
                | views::transform([](int n) { return n * n; })     // Square: 4, 16, 36, 64...
                | views::take(3)                                    // Take first 3: 4, 16, 36
                | views::reverse;                                   // Reverse: 36, 16, 4

    cout << "\nPipeline Result (Evens -> Squared -> Take 3 -> Reverse):" << endl;
    for (int n : result) {
        cout << n << " ";
    }
    cout << endl;

    // --- Ranges with Containers ---
    // You can also use ranges algorithms directly on containers (no more .begin(), .end())
    vector<int> randomNums = {50, 10, 90, 30, 70};
    
    cout << "\nRanges Sort:" << endl;
    cout << "Before: ";
    for(int n : randomNums) cout << n << " ";
    
    // std::ranges::sort handles the iterators for you
    ranges::sort(randomNums);
    
    cout << "\nAfter:  ";
    for(int n : randomNums) cout << n << " ";
    cout << endl;

    return 0;
}
