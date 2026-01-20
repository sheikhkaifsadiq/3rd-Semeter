#include <iostream>
#include <variant>   // Requires C++17
#include <optional>  // Requires C++17
#include <string>
#include <map>
#include <vector>

using namespace std;

// --- 1. Define the supported types for our settings ---
// A 'SettingValue' can hold an int, double, bool, or string.
using SettingValue = variant<int, double, bool, string>;

// --- 2. Configuration Manager Class ---
class ConfigManager {
private:
    map<string, SettingValue> settings;

public:
    // Add or Update a setting
    // The 'val' argument automatically wraps the input into the variant
    void set(const string& key, SettingValue val) {
        settings[key] = val;
    }

    // Retrieve a setting safely using std::optional
    // We return optional<SettingValue> because the key might not exist.
    optional<SettingValue> get(const string& key) {
        if (settings.find(key) != settings.end()) {
            return settings[key];
        }
        return nullopt; // Represents "No Value"
    }

    // Retrieve a setting ONLY if it matches a specific type (e.g., only want 'int')
    // Template T allows us to ask for int, double, etc.
    template <typename T>
    optional<T> getTyped(const string& key) {
        if (settings.find(key) != settings.end()) {
            // std::get_if returns a pointer to the value if the type matches, else nullptr
            if (auto valPtr = get_if<T>(&settings[key])) {
                return *valPtr; // Return the value inside the optional
            }
        }
        return nullopt; // Key doesn't exist OR type didn't match
    }

    // Helper to print a single variant value using std::visit
    // The Visitor struct defines how to handle each possible type
    struct SettingVisitor {
        void operator()(int i) const { cout << "[Integer] " << i; }
        void operator()(double d) const { cout << "[Float]   " << d; }
        void operator()(bool b) const { cout << "[Boolean] " << (b ? "true" : "false"); }
        void operator()(const string& s) const { cout << "[String]  \"" << s << "\""; }
    };

    void displayAll() {
        cout << "\n--- Current Configuration ---" << endl;
        for (const auto& [key, value] : settings) {
            cout << key << ": ";
            // std::visit takes a visitor (functor) and the variant, 
            // and calls the correct operator() based on the active type.
            visit(SettingVisitor{}, value); 
            cout << endl;
        }
    }
};

int main() {
    cout << "=== C++17 Variant & Optional Demo (Config System) ===" << endl;

    ConfigManager config;

    // --- 1. Storing mixed types in the same container ---
    cout << "\n1. Setting values of different types..." << endl;
    config.set("WindowWidth", 1920);           // int
    config.set("WindowHeight", 1080);          // int
    config.set("Opacity", 0.85);               // double
    config.set("FullScreen", true);            // bool
    config.set("Title", string("My App"));     // string

    config.displayAll();

    // --- 2. Using std::optional to handle missing keys ---
    cout << "\n2. Searching for keys:" << endl;
    
    string searchKey = "Opacity";
    auto result = config.get(searchKey); // Returns optional<SettingValue>

    if (result.has_value()) {
        cout << "   Found '" << searchKey << "'. (Value exists)" << endl;
    } else {
        cout << "   '" << searchKey << "' not found." << endl;
    }

    searchKey = "MissingKey";
    if (config.get(searchKey) == nullopt) {
        cout << "   '" << searchKey << "' correctly identified as missing." << endl;
    }

    // --- 3. Safe Type Retrieval ---
    cout << "\n3. Type-Safe Retrieval:" << endl;

    // Correct Type
    auto width = config.getTyped<int>("WindowWidth");
    if (width) {
        cout << "   WindowWidth is: " << *width << endl;
    } else {
        cout << "   WindowWidth is missing or not an int." << endl;
    }

    // Incorrect Type (Asking for string when it is int)
    auto titleAsInt = config.getTyped<int>("Title"); 
    if (!titleAsInt) {
        cout << "   Safety Check Passed: 'Title' is not an integer." << endl;
    }

    // --- 4. Updating Values (Changing Type) ---
    cout << "\n4. Modifying types dynamically:" << endl;
    // Variant allows changing the type stored in the same variable
    config.set("WindowWidth", string("Auto-Detected")); 
    
    // Now verify it's a string
    visit(ConfigManager::SettingVisitor{}, *config.get("WindowWidth"));
    cout << endl;

    return 0;
}
