#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// === Class Definitions ===

// Class representing a Book
class Book {
public:
    int id;
    string title;
    string author;
    bool isIssued;
    int issuedToMemberId; // ID of member who borrowed it, -1 if none

    Book(int id, string t, string a) 
        : id(id), title(t), author(a), isIssued(false), issuedToMemberId(-1) {}

    void display() const {
        cout << "ID: " << id << " | Title: " << title << " | Author: " << author;
        if (isIssued) cout << " [Issued]";
        else cout << " [Available]";
        cout << endl;
    }
};

// Base Class representing a Library Member
class Member {
protected:
    int id;
    string name;
    int booksBorrowed;

public:
    Member(int id, string n) : id(id), name(n), booksBorrowed(0) {}
    virtual ~Member() {} // Virtual destructor

    int getId() const { return id; }
    string getName() const { return name; }
    int getBooksBorrowed() const { return booksBorrowed; }

    // Pure virtual function to be implemented by derived classes
    virtual int getBorrowLimit() const = 0; 
    virtual string getType() const = 0;

    bool borrowBook() {
        if (booksBorrowed < getBorrowLimit()) {
            booksBorrowed++;
            return true;
        }
        return false;
    }

    void returnBook() {
        if (booksBorrowed > 0) booksBorrowed--;
    }

    void display() const {
        cout << getType() << " - ID: " << id << " | Name: " << name 
             << " | Borrowed: " << booksBorrowed << "/" << getBorrowLimit() << endl;
    }
};

// Derived Class: Student
class Student : public Member {
public:
    Student(int id, string n) : Member(id, n) {}

    int getBorrowLimit() const override {
        return 3; // Students can borrow 3 books
    }

    string getType() const override {
        return "Student";
    }
};

// Derived Class: Faculty
class Faculty : public Member {
public:
    Faculty(int id, string n) : Member(id, n) {}

    int getBorrowLimit() const override {
        return 5; // Faculty can borrow 5 books
    }

    string getType() const override {
        return "Faculty";
    }
};

// Class to manage the Library
class Library {
private:
    vector<Book> books;
    vector<Member*> members; // Vector of pointers to handle polymorphism

public:
    ~Library() {
        // Clean up allocated memory for members
        for (Member* m : members) {
            delete m;
        }
    }

    void addBook(int id, string title, string author) {
        books.push_back(Book(id, title, author));
        cout << "Book added successfully.\n";
    }

    void addMember(int id, string name, int type) {
        // Type 1 = Student, Type 2 = Faculty
        if (type == 1) {
            members.push_back(new Student(id, name));
            cout << "Student member added.\n";
        } else if (type == 2) {
            members.push_back(new Faculty(id, name));
            cout << "Faculty member added.\n";
        } else {
            cout << "Invalid member type.\n";
        }
    }

    void issueBook(int memberId, int bookId) {
        Member* member = findMember(memberId);
        Book* book = findBook(bookId);

        if (member && book) {
            if (book->isIssued) {
                cout << "Error: Book is already issued.\n";
            } else if (member->getBooksBorrowed() >= member->getBorrowLimit()) {
                cout << "Error: Member has reached borrow limit.\n";
            } else {
                book->isIssued = true;
                book->issuedToMemberId = memberId;
                member->borrowBook();
                cout << "Book issued successfully to " << member->getName() << ".\n";
            }
        } else {
            cout << "Error: Book or Member not found.\n";
        }
    }

    void returnBook(int bookId) {
        Book* book = findBook(bookId);
        if (book && book->isIssued) {
            Member* member = findMember(book->issuedToMemberId);
            if (member) {
                member->returnBook();
                book->isIssued = false;
                book->issuedToMemberId = -1;
                cout << "Book returned successfully.\n";
            }
        } else {
            cout << "Error: Book is not currently issued or does not exist.\n";
        }
    }

    void displayAllBooks() {
        cout << "\n--- Library Books ---\n";
        for (const auto& book : books) {
            book.display();
        }
    }

    void displayAllMembers() {
        cout << "\n--- Library Members ---\n";
        for (const auto& member : members) {
            member->display();
        }
    }

private:
    // Helper functions
    Book* findBook(int id) {
        for (auto& book : books) {
            if (book.id == id) return &book;
        }
        return nullptr;
    }

    Member* findMember(int id) {
        for (auto& member : members) {
            if (member->getId() == id) return member;
        }
        return nullptr;
    }
};

// === Main Function ===
int main() {
    Library lib;
    int choice;

    cout << "=== Library Management System ===" << endl;

    while (true) {
        cout << "\n1. Add Book\n2. Add Member\n3. Issue Book\n4. Return Book\n5. Display All\n6. Exit\n";
        cout << "Choice: ";
        cin >> choice;

        if (choice == 6) break;

        switch (choice) {
            case 1: {
                int id;
                string title, author;
                cout << "Enter Book ID: "; cin >> id;
                cin.ignore();
                cout << "Enter Title: "; getline(cin, title);
                cout << "Enter Author: "; getline(cin, author);
                lib.addBook(id, title, author);
                break;
            }
            case 2: {
                int id, type;
                string name;
                cout << "Enter Member ID: "; cin >> id;
                cin.ignore();
                cout << "Enter Name: "; getline(cin, name);
                cout << "Type (1=Student, 2=Faculty): "; cin >> type;
                lib.addMember(id, name, type);
                break;
            }
            case 3: {
                int mId, bId;
                cout << "Enter Member ID: "; cin >> mId;
                cout << "Enter Book ID: "; cin >> bId;
                lib.issueBook(mId, bId);
                break;
            }
            case 4: {
                int bId;
                cout << "Enter Book ID to Return: "; cin >> bId;
                lib.returnBook(bId);
                break;
            }
            case 5:
                lib.displayAllBooks();
                lib.displayAllMembers();
                break;
            default:
                cout << "Invalid choice.\n";
        }
    }

    return 0;
}
