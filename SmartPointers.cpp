#include <iostream>
#include <memory> // Required for smart pointers
#include <string>

using namespace std;

// A simple class to represent a resource (e.g., a file, connection, or large object)
class Resource {
private:
    string name;

public:
    Resource(string n) : name(n) {
        cout << "  [Resource '" << name << "' allocated]" << endl;
    }

    ~Resource() {
        cout << "  [Resource '" << name << "' deallocated]" << endl;
    }

    void sayHello() const {
        cout << "    Hello from " << name << endl;
    }
};

// Demo for std::unique_ptr
// Represents EXCLUSIVE ownership. Cannot be copied, only moved.
void uniquePtrDemo() {
    cout << "\n--- 1. Unique Pointer Demo ---" << endl;
    cout << "Creating a unique_ptr..." << endl;
    
    // make_unique is the preferred way to create unique_ptrs (C++14)
    unique_ptr<Resource> res1 = make_unique<Resource>("UniqueRes");
    res1->sayHello();

    // unique_ptr<Resource> res2 = res1; // This would cause a COMPILER ERROR (cannot copy)

    cout << "Exiting uniquePtrDemo function scope..." << endl;
    // res1 goes out of scope here. 
    // The Resource destructor is called automatically. No 'delete' needed.
}

// Demo for std::shared_ptr
// Represents SHARED ownership. Reference counting ensures safety.
void sharedPtrDemo() {
    cout << "\n--- 2. Shared Pointer Demo ---" << endl;
    
    // make_shared is the preferred way to create shared_ptrs
    shared_ptr<Resource> ptr1 = make_shared<Resource>("SharedRes");
    
    {
        cout << "  Inside inner scope..." << endl;
        shared_ptr<Resource> ptr2 = ptr1; // Share ownership. Both point to the same object.
        
        cout << "  Reference count: " << ptr1.use_count() << endl; // Should be 2
        ptr2->sayHello();
        
        cout << "  Exiting inner scope..." << endl;
    } 
    // ptr2 dies here. Reference count decreases, but resource is NOT destroyed yet.
    
    cout << "Back in outer scope. Reference count: " << ptr1.use_count() << endl; // Should be 1
    ptr1->sayHello();
    
    cout << "Exiting sharedPtrDemo function scope..." << endl;
    // ptr1 dies here. Reference count hits 0. Resource is deallocated.
}

int main() {
    cout << "=== Modern C++ Smart Pointers Demo ===" << endl;
    
    uniquePtrDemo();
    
    sharedPtrDemo();
    
    cout << "\n=== End of Program ===" << endl;
    return 0;
}
