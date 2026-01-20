#include <iostream>
#include <vector>
#include <string>
#include <limits>

using namespace std;

// Class to represent a Student
class Student {
private:
    string name;
    int rollNumber;
    char grade;

public:
    // Constructor to initialize student details
    Student(string n, int r, char g) : name(n), rollNumber(r), grade(g) {}

    // Function to display student details
    void display() const {
        cout << "---------------------------------" << endl;
        cout << "Name: " << name << endl;
        cout << "Roll Number: " << rollNumber << endl;
        cout << "Grade: " << grade << endl;
        cout << "---------------------------------" << endl;
    }

    // Getter for roll number (useful for searching, though not implemented in basic menu)
    int getRollNumber() const {
        return rollNumber;
    }
};

int main() {
    vector<Student> students; // Vector to store student objects
    int choice;

    cout << "=== Student Database System ===" << endl;

    while (true) {
        cout << "\nMenu:\n";
        cout << "1. Add New Student\n";
        cout << "2. Display All Students\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        
        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (choice == 3) {
            cout << "Exiting system..." << endl;
            break;
        }

        switch (choice) {
            case 1: {
                string name;
                int roll;
                char grade;

                cout << "Enter Name: ";
                cin.ignore(); // Clear newline from buffer
                getline(cin, name);

                cout << "Enter Roll Number: ";
                while (!(cin >> roll)) {
                    cout << "Invalid roll number. Enter a number: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }

                cout << "Enter Grade (A, B, C, etc.): ";
                cin >> grade;

                // Create object and add to vector
                Student newStudent(name, roll, grade);
                students.push_back(newStudent);
                cout << "Student added successfully!" << endl;
                break;
            }
            case 2: {
                if (students.empty()) {
                    cout << "No students found in the database." << endl;
                } else {
                    cout << "\n--- Student Records ---" << endl;
                    for (const auto& student : students) {
                        student.display();
                    }
                }
                break;
            }
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }

    return 0;
}
