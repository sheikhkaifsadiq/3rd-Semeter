#include <iostream>
#include <vector>
#include <cstring> // For memcpy
#include <utility> // For std::move

using namespace std;

// Class representing a buffer of memory
// Demonstrates the "Rule of Five": Destructor, Copy Ctor, Copy Assign, Move Ctor, Move Assign
class HugeBuffer {
private:
    int* data;
    size_t size;

public:
    // 1. Constructor
    HugeBuffer(size_t s) : size(s) {
        data = new int[size];
        for (size_t i = 0; i < size; ++i) data[i] = i; // Fill with dummy data
        cout << "  [Const] Allocated buffer of size " << size << endl;
    }

    // 2. Destructor
    ~HugeBuffer() {
        if (data) {
            delete[] data;
            cout << "  [Destr] Freed buffer." << endl;
        } else {
            cout << "  [Destr] Nothing to free (object was moved from)." << endl;
        }
    }

    // 3. Copy Constructor (Deep Copy - Slow)
    // Called when creating a new object as a copy of an existing one.
    HugeBuffer(const HugeBuffer& other) : size(other.size) {
        data = new int[size];
        memcpy(data, other.data, size * sizeof(int));
        cout << "  [Copy]  Deep copy performed (Slow)!" << endl;
    }

    // 4. Move Constructor (Shallow Copy - Fast)
    // Called when creating a new object from a temporary (r-value) or explicitly moved object.
    // 'noexcept' is crucial for STL containers to use this optimization.
    HugeBuffer(HugeBuffer&& other) noexcept : data(other.data), size(other.size) {
        // "Steal" the resource
        // Nullify the source pointer so the destructor doesn't free the memory we just stole
        other.data = nullptr;
        other.size = 0;
        cout << "  [Move]  Resources stolen/moved (Fast)!" << endl;
    }

    // 5. Copy Assignment Operator (b1 = b2)
    HugeBuffer& operator=(const HugeBuffer& other) {
        if (this == &other) return *this; // Self-assignment check

        delete[] data; // Free existing resource

        size = other.size;
        data = new int[size];
        memcpy(data, other.data, size * sizeof(int));
        cout << "  [Copy=] Assignment deep copy." << endl;
        return *this;
    }

    // 6. Move Assignment Operator (b1 = std::move(b2))
    HugeBuffer& operator=(HugeBuffer&& other) noexcept {
        if (this == &other) return *this;

        delete[] data; // Free existing resource

        // Steal resource
        data = other.data;
        size = other.size;

        // Nullify source
        other.data = nullptr;
        other.size = 0;

        cout << "  [Move=] Assignment move." << endl;
        return *this;
    }
};

// Function that takes object by Value
// This forces a Copy if passed an l-value, or a Move if passed an r-value
void processBuffer(HugeBuffer b) {
    cout << "  ...Processing buffer..." << endl;
}

int main() {
    cout << "=== C++ Move Semantics Demo ===" << endl;

    cout << "\n1. Normal Construction:" << endl;
    HugeBuffer b1(100);

    cout << "\n2. Copy Semantics (Slow):" << endl;
    // b1 is an l-value (it has a name and persists), so it is Copied.
    HugeBuffer b2 = b1; 

    cout << "\n3. Move Semantics (Fast):" << endl;
    // std::move(b1) casts b1 to an r-value (temporary), triggering the Move Constructor.
    // WARNING: b1 is now empty/invalid after this!
    HugeBuffer b3 = std::move(b1); 

    cout << "\n4. Passing to function:" << endl;
    
    cout << "-> Passing l-value (b2):" << endl;
    processBuffer(b2); // Calls Copy Constructor

    cout << "-> Passing temporary (HugeBuffer(50)):" << endl;
    processBuffer(HugeBuffer(50)); // Calls Move Constructor (or elided entirely by compiler optimization)

    cout << "\n5. Vector Push Optimization:" << endl;
    vector<HugeBuffer> vec;
    // Uses Move Constructor because we are pushing a temporary object
    vec.push_back(HugeBuffer(200)); 

    cout << "\n=== End of Scope (Destructors will run) ===" << endl;
    return 0;
}
