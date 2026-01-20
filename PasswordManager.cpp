#include <iostream>
#include <vector>
#include <string>
#include <functional> // For std::hash
#include <limits>

using namespace std;

// === Utility Class for Encryption ===
class CryptoUtils {
public:
    // Simple XOR Encryption/Decryption (Symmetric)
    // In a real app, use AES or similar strong encryption
    static string encryptDecrypt(string input, string key) {
        string output = input;
        for (size_t i = 0; i < input.size(); i++) {
            output[i] = input[i] ^ key[i % key.size()];
        }
        return output;
    }
};

// === Class: Credential ===
// Stores login information. The password is stored in an encrypted state.
class Credential {
private:
    string siteName;
    string username;
    string encryptedPassword;

public:
    Credential(string site, string user, string pass, string key) 
        : siteName(site), username(user) {
        // Encrypt password immediately upon creation
        encryptedPassword = CryptoUtils::encryptDecrypt(pass, key);
    }

    string getSiteName() const { return siteName; }
    string getUsername() const { return username; }

    // Decrypts and returns the password
    string getDecryptedPassword(string key) const {
        return CryptoUtils::encryptDecrypt(encryptedPassword, key);
    }
};

// === Class: PasswordManager ===
// Manages the vault and master authentication
class PasswordManager {
private:
    vector<Credential> vault;
    size_t masterHash; // Store hash of master password, not the password itself
    string currentKey; // Temporary storage for the key while logged in
    bool isLoggedIn;

public:
    PasswordManager() : isLoggedIn(false), masterHash(0) {}

    // Sets the master password for the first time
    void setMasterPassword(string password) {
        masterHash = hash<string>{}(password);
        cout << "Master password set successfully." << endl;
    }

    // Authenticates the user
    bool login(string password) {
        size_t inputHash = hash<string>{}(password);
        if (inputHash == masterHash) {
            isLoggedIn = true;
            currentKey = password; // Store key to use for decryption
            cout << "Login successful!" << endl;
            return true;
        } else {
            cout << "Access Denied: Incorrect Master Password." << endl;
            return false;
        }
    }

    void logout() {
        isLoggedIn = false;
        currentKey = ""; // Clear key from memory
        cout << "Logged out securely." << endl;
    }

    void addCredential(string site, string user, string pass) {
        if (!isLoggedIn) {
            cout << "Error: You must be logged in to add passwords." << endl;
            return;
        }
        Credential newCred(site, user, pass, currentKey);
        vault.push_back(newCred);
        cout << "Credential saved for " << site << "." << endl;
    }

    void retrieveCredential(string site) {
        if (!isLoggedIn) {
            cout << "Error: You must be logged in to view passwords." << endl;
            return;
        }

        bool found = false;
        for (const auto& cred : vault) {
            if (cred.getSiteName() == site) {
                cout << "\n--- Credential Found ---" << endl;
                cout << "Site: " << cred.getSiteName() << endl;
                cout << "Username: " << cred.getUsername() << endl;
                cout << "Password: " << cred.getDecryptedPassword(currentKey) << endl;
                found = true;
                break; // Stop after finding match
            }
        }

        if (!found) {
            cout << "No credential found for " << site << "." << endl;
        }
    }

    void listSites() {
        if (!isLoggedIn) {
            cout << "Error: Login required." << endl;
            return;
        }
        cout << "\n--- Stored Credentials ---" << endl;
        if (vault.empty()) {
            cout << "(Vault is empty)" << endl;
        } else {
            for (const auto& cred : vault) {
                cout << "- " << cred.getSiteName() << " (" << cred.getUsername() << ")" << endl;
            }
        }
    }
};

// === Main Function ===
int main() {
    PasswordManager pm;
    string input;
    int choice;

    cout << "=== Secure Password Manager ===" << endl;
    
    // 1. Setup Phase
    cout << "Please set a Master Password to start: ";
    getline(cin, input);
    pm.setMasterPassword(input);

    // 2. Authentication Phase
    cout << "\nAuthentication Required." << endl;
    while (true) {
        cout << "Enter Master Password: ";
        getline(cin, input);
        if (pm.login(input)) {
            break;
        }
        cout << "Try again." << endl;
    }

    // 3. Operation Phase
    while (true) {
        cout << "\nMenu:\n";
        cout << "1. Add Password\n";
        cout << "2. Retrieve Password\n";
        cout << "3. List All Sites\n";
        cout << "4. Logout & Exit\n";
        cout << "Enter Choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(); // Consume newline

        if (choice == 4) {
            pm.logout();
            break;
        }

        switch (choice) {
            case 1: {
                string site, user, pass;
                cout << "Enter Website/App Name: "; getline(cin, site);
                cout << "Enter Username: "; getline(cin, user);
                cout << "Enter Password: "; getline(cin, pass);
                pm.addCredential(site, user, pass);
                break;
            }
            case 2: {
                string site;
                cout << "Enter Website Name to Search: "; getline(cin, site);
                pm.retrieveCredential(site);
                break;
            }
            case 3:
                pm.listSites();
                break;
            default:
                cout << "Invalid option." << endl;
        }
    }

    return 0;
}
