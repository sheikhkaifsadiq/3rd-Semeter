#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

// Class Definition
class Student {
private:
    string name;
    int rollNumber;
    float grade;

public:
    // Constructor
    Student(string n, int r, float g) {
        name = n;
        rollNumber = r;
        grade = g;
    }

    // Getter functions (Encapsulation)
    int getRollNumber() const { return rollNumber; }
    
    void displayRecord() const {
        cout << left << setw(20) << name 
             << setw(15) << rollNumber 
             << setw(10) << grade << endl;
    }
};

// Main System Logic
int main() {
    vector<Student> database;
    int choice;

    do {
        cout << "\n--- Student Database System ---\n";
        cout << "1. Add Student\n";
        cout << "2. View All Students\n";
        cout << "3. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            string name;
            int roll;
            float grade;
            
            cout << "Enter Name: ";
            cin.ignore(); // Clear buffer
            getline(cin, name);
            cout << "Enter Roll Number: ";
            cin >> roll;
            cout << "Enter Grade: ";
            cin >> grade;

            // Create object and add to vector
            Student newStudent(name, roll, grade);
            database.push_back(newStudent);
            cout << "Student added successfully!\n";

        } else if (choice == 2) {
            cout << "\n" << left << setw(20) << "Name" 
                 << setw(15) << "Roll No" 
                 << setw(10) << "Grade" << endl;
            cout << "---------------------------------------------\n";
            for (const auto& student : database) {
                student.displayRecord();
            }
        }

    } while (choice != 3);

    return 0;
}
