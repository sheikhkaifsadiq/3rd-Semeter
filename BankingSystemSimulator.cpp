#include <iostream>
#include <vector>
#include <string>
#include <iomanip> // For setprecision
#include <limits>  // For numeric_limits

using namespace std;

// === Abstract Base Class: Account ===
class Account {
protected:
    int accountNumber;
    string holderName;
    double balance;

public:
    Account(int accNum, string name, double bal) 
        : accountNumber(accNum), holderName(name), balance(bal) {}

    virtual ~Account() {}

    int getAccountNumber() const { return accountNumber; }
    string getHolderName() const { return holderName; }
    double getBalance() const { return balance; }

    // Concrete method for deposit (same for all accounts)
    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            cout << "Deposited: $" << fixed << setprecision(2) << amount << endl;
            cout << "New Balance: $" << balance << endl;
        } else {
            cout << "Invalid deposit amount." << endl;
        }
    }

    // Pure virtual method for withdrawal (implementation varies)
    virtual bool withdraw(double amount) = 0;

    // Virtual display method
    virtual void display() const {
        cout << "Acc #: " << accountNumber << " | Holder: " << holderName 
             << " | Balance: $" << fixed << setprecision(2) << balance;
    }
};

// === Derived Class: SavingsAccount ===
class SavingsAccount : public Account {
private:
    double interestRate; // Annual interest rate (e.g., 0.05 for 5%)

public:
    SavingsAccount(int accNum, string name, double bal, double rate) 
        : Account(accNum, name, bal), interestRate(rate) {}

    bool withdraw(double amount) override {
        if (amount > 0 && balance >= amount) {
            balance -= amount;
            cout << "Withdrawn: $" << fixed << setprecision(2) << amount << endl;
            cout << "Remaining Balance: $" << balance << endl;
            return true;
        } else {
            cout << "Transaction failed: Insufficient funds." << endl;
            return false;
        }
    }

    void applyInterest() {
        double interest = balance * interestRate;
        balance += interest;
        cout << "Interest Applied: $" << fixed << setprecision(2) << interest << endl;
        cout << "New Balance: $" << balance << endl;
    }

    void display() const override {
        Account::display();
        cout << " | Type: Savings | Rate: " << (interestRate * 100) << "%" << endl;
    }
};

// === Derived Class: CheckingAccount ===
class CheckingAccount : public Account {
private:
    double overdraftLimit; // Amount allowed to overdraw

public:
    CheckingAccount(int accNum, string name, double bal, double limit) 
        : Account(accNum, name, bal), overdraftLimit(limit) {}

    bool withdraw(double amount) override {
        if (amount > 0 && (balance + overdraftLimit) >= amount) {
            balance -= amount;
            cout << "Withdrawn: $" << fixed << setprecision(2) << amount << endl;
            if (balance < 0) {
                cout << "Notice: You are using overdraft." << endl;
            }
            cout << "New Balance: $" << balance << endl;
            return true;
        } else {
            cout << "Transaction failed: Exceeds overdraft limit." << endl;
            return false;
        }
    }

    void display() const override {
        Account::display();
        cout << " | Type: Checking | Overdraft Limit: $" << overdraftLimit << endl;
    }
};

// === Bank Management Class ===
class Bank {
private:
    vector<Account*> accounts;

public:
    ~Bank() {
        // Clean up memory
        for (Account* acc : accounts) {
            delete acc;
        }
    }

    void openAccount(Account* acc) {
        accounts.push_back(acc);
        cout << "Account created successfully!" << endl;
    }

    Account* findAccount(int accNum) {
        for (Account* acc : accounts) {
            if (acc->getAccountNumber() == accNum) {
                return acc;
            }
        }
        return nullptr;
    }

    void deposit(int accNum, double amount) {
        Account* acc = findAccount(accNum);
        if (acc) {
            acc->deposit(amount);
        } else {
            cout << "Account not found." << endl;
        }
    }

    void withdraw(int accNum, double amount) {
        Account* acc = findAccount(accNum);
        if (acc) {
            acc->withdraw(amount);
        } else {
            cout << "Account not found." << endl;
        }
    }

    void displayAll() {
        if (accounts.empty()) {
            cout << "No accounts to display." << endl;
            return;
        }
        cout << "\n--- All Bank Accounts ---" << endl;
        for (const auto& acc : accounts) {
            acc->display();
        }
    }
};

// === Main Function ===
int main() {
    Bank myBank;
    int choice;
    int nextAccNum = 1001; // Auto-increment account numbers

    cout << "=== Banking System Simulator ===" << endl;

    while (true) {
        cout << "\nMenu:\n";
        cout << "1. Open Savings Account\n";
        cout << "2. Open Checking Account\n";
        cout << "3. Deposit\n";
        cout << "4. Withdraw\n";
        cout << "5. Display All Accounts\n";
        cout << "6. Exit\n";
        cout << "Enter Choice: ";
        
        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (choice == 6) break;

        switch (choice) {
            case 1: {
                string name;
                double initialDep, rate;
                cout << "Enter Holder Name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Initial Deposit: "; cin >> initialDep;
                cout << "Interest Rate (e.g., 0.03 for 3%): "; cin >> rate;
                myBank.openAccount(new SavingsAccount(nextAccNum++, name, initialDep, rate));
                break;
            }
            case 2: {
                string name;
                double initialDep, limit;
                cout << "Enter Holder Name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Initial Deposit: "; cin >> initialDep;
                cout << "Overdraft Limit: "; cin >> limit;
                myBank.openAccount(new CheckingAccount(nextAccNum++, name, initialDep, limit));
                break;
            }
            case 3: {
                int accNum;
                double amount;
                cout << "Enter Account Number: "; cin >> accNum;
                cout << "Enter Deposit Amount: "; cin >> amount;
                myBank.deposit(accNum, amount);
                break;
            }
            case 4: {
                int accNum;
                double amount;
                cout << "Enter Account Number: "; cin >> accNum;
                cout << "Enter Withdrawal Amount: "; cin >> amount;
                myBank.withdraw(accNum, amount);
                break;
            }
            case 5:
                myBank.displayAll();
                break;
            default:
                cout << "Invalid choice." << endl;
        }
    }

    cout << "Exiting Banking System..." << endl;
    return 0;
}
