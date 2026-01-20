#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>

using namespace std;

const string FILENAME = "contacts.txt";

// Class to represent a Contact
class Contact {
private:
    string name;
    string phoneNumber;
    string email;

public:
    Contact() {} // Default constructor
    Contact(string n, string p, string e) : name(n), phoneNumber(p), email(e) {}

    // Getters
    string getName() const { return name; }
    string getPhone() const { return phoneNumber; }
    string getEmail() const { return email; }

    // Display contact info
    void display() const {
        cout << left << setw(20) << name 
             << setw(15) << phoneNumber 
             << setw(25) << email << endl;
    }

    // Format for file storage (CSV style)
    string toString() const {
        return name + "," + phoneNumber + "," + email;
    }

    // Create from file string (Simple parsing)
    static Contact fromString(string line) {
        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);

        if (pos1 != string::npos && pos2 != string::npos) {
            string n = line.substr(0, pos1);
            string p = line.substr(pos1 + 1, pos2 - pos1 - 1);
            string e = line.substr(pos2 + 1);
            return Contact(n, p, e);
        }
        return Contact("Unknown", "000", "none");
    }
};

// Class to manage the collection of contacts and file I/O
class ContactManager {
private:
    vector<Contact> contacts;

public:
    ContactManager() {
        loadContacts();
    }

    void addContact(string name, string phone, string email) {
        contacts.push_back(Contact(name, phone, email));
        saveContacts(); // Auto-save after adding
        cout << "Contact added and saved successfully." << endl;
    }

    void displayAll() const {
        if (contacts.empty()) {
            cout << "No contacts found." << endl;
            return;
        }
        cout << "\n------------------------------------------------------------" << endl;
        cout << left << setw(20) << "Name" 
             << setw(15) << "Phone" 
             << setw(25) << "Email" << endl;
        cout << "------------------------------------------------------------" << endl;
        for (const auto& contact : contacts) {
            contact.display();
        }
        cout << "------------------------------------------------------------" << endl;
    }

    void searchContact(string name) const {
        bool found = false;
        cout << "\n--- Search Results ---" << endl;
        for (const auto& contact : contacts) {
            // Simple substring search
            if (contact.getName().find(name) != string::npos) {
                contact.display();
                found = true;
            }
        }
        if (!found) cout << "No contact found matching '" << name << "'." << endl;
    }

    void deleteContact(string name) {
        for (auto it = contacts.begin(); it != contacts.end(); ++it) {
            if (it->getName() == name) {
                contacts.erase(it);
                saveContacts(); // Auto-save after delete
                cout << "Contact deleted successfully." << endl;
                return;
            }
        }
        cout << "Contact not found." << endl;
    }

private:
    // Save all contacts to file
    void saveContacts() {
        ofstream outFile(FILENAME);
        if (outFile.is_open()) {
            for (const auto& contact : contacts) {
                outFile << contact.toString() << endl;
            }
            outFile.close();
        } else {
            cerr << "Error: Unable to open file for saving." << endl;
        }
    }

    // Load contacts from file on startup
    void loadContacts() {
        ifstream inFile(FILENAME);
        if (inFile.is_open()) {
            string line;
            while (getline(inFile, line)) {
                if (!line.empty()) {
                    contacts.push_back(Contact::fromString(line));
                }
            }
            inFile.close();
            cout << "Data loaded from " << FILENAME << endl;
        } else {
            cout << "No existing data file found. Creating new list." << endl;
        }
    }
};

int main() {
    ContactManager manager;
    int choice;

    cout << "=== File-Based Contact Manager ===" << endl;

    while (true) {
        cout << "\n1. Add Contact\n2. View All\n3. Search\n4. Delete\n5. Exit\n";
        cout << "Enter Choice: ";
        
        if (!(cin >> choice)) {
            cout << "Invalid input." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(); // Consume newline

        if (choice == 5) break;

        switch (choice) {
            case 1: {
                string name, phone, email;
                cout << "Name: "; getline(cin, name);
                cout << "Phone: "; getline(cin, phone);
                cout << "Email: "; getline(cin, email);
                manager.addContact(name, phone, email);
                break;
            }
            case 2:
                manager.displayAll();
                break;
            case 3: {
                string name;
                cout << "Enter Name to Search: "; getline(cin, name);
                manager.searchContact(name);
                break;
            }
            case 4: {
                string name;
                cout << "Enter Exact Name to Delete: "; getline(cin, name);
                manager.deleteContact(name);
                break;
            }
            default:
                cout << "Invalid option." << endl;
        }
    }

    return 0;
}
