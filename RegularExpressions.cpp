#include <iostream>
#include <regex> // Required for std::regex
#include <string>
#include <vector>

using namespace std;

int main() {
    cout << "=== C++ Regular Expressions (Regex) Demo ===" << endl;

    // --- 1. Input Validation (regex_match) ---
    // We use a "Raw String Literal" R"(...)" to avoid escaping backslashes repeatedly.
    // Pattern: alphanumeric chars + @ + alphanumeric chars + . + alphanumeric chars
    regex emailPattern(R"(\w+@\w+\.\w+)"); 

    vector<string> emails = {"user@example.com", "invalid-email", "john.doe@test.org"};

    cout << "\n1. Validating Emails:" << endl;
    for (const auto& email : emails) {
        // regex_match returns true only if the ENTIRE string matches the pattern
        bool isValid = regex_match(email, emailPattern);
        cout << "   Testing '" << email << "': " << (isValid ? "Valid" : "Invalid") << endl;
    }

    // --- 2. Searching within text (regex_search) ---
    cout << "\n2. Searching for patterns:" << endl;
    string logData = "Error code 404 at line 12. Warning code 200 at line 45.";
    
    // Pattern: one or more digits (\d+)
    regex digitPattern(R"(\d+)");
    smatch match; // Used to store the results of the match

    // regex_search finds the FIRST match. To find all, we loop.
    string searchSubject = logData;
    cout << "   Text: \"" << logData << "\"" << endl;
    cout << "   Numbers found: ";
    
    while (regex_search(searchSubject, match, digitPattern)) {
        cout << "[" << match.str() << "] ";
        // Update subject to be the suffix (everything AFTER the match)
        searchSubject = match.suffix().str();
    }
    cout << endl;

    // --- 3. Text Replacement (regex_replace) ---
    cout << "\n3. Redacting Sensitive Data:" << endl;
    string secretText = "My credit card is 1234-5678-9101-1121.";
    
    // Pattern: 4 digits, hyphen, 4 digits, etc.
    regex cardPattern(R"(\d{4}-\d{4}-\d{4}-\d{4})");

    // Replace the matched pattern with a safe string
    string redacted = regex_replace(secretText, cardPattern, "XXXX-XXXX-XXXX-XXXX");
    
    cout << "   Original: " << secretText << endl;
    cout << "   Redacted: " << redacted << endl;

    // --- 4. Capture Groups ---
    cout << "\n4. Formatting Dates (Capture Groups):" << endl;
    string date = "2023-10-25"; // YYYY-MM-DD
    // Pattern: (\d{4}) is Group 1, (\d{2}) is Group 2, (\d{2}) is Group 3
    regex datePattern(R"((\d{4})-(\d{2})-(\d{2}))");

    // Reformat to DD/MM/YYYY using group references ($3, $2, $1)
    string formattedDate = regex_replace(date, datePattern, "$3/$2/$1");

    cout << "   ISO Format: " << date << endl;
    cout << "   US Format:  " << formattedDate << endl;

    return 0;
}
