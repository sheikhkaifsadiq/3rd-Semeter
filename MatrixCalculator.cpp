#include <iostream>
#include <vector>
#include <iomanip>
#include <limits>

using namespace std;

// Class to represent a mathematical Matrix
class Matrix {
private:
    int rows;
    int cols;
    vector<vector<int>> data;

public:
    // Constructor
    Matrix(int r, int c) : rows(r), cols(c) {
        data.resize(rows, vector<int>(cols, 0));
    }

    // Function to fill matrix with user input
    void input() {
        cout << "Enter elements for a " << rows << "x" << cols << " matrix:" << endl;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                cout << "Element [" << i << "][" << j << "]: ";
                while (!(cin >> data[i][j])) {
                    cout << "Invalid input. Enter an integer: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            }
        }
    }

    // Function to display the matrix
    void display() const {
        for (int i = 0; i < rows; i++) {
            cout << "| ";
            for (int j = 0; j < cols; j++) {
                cout << setw(4) << data[i][j] << " ";
            }
            cout << "|" << endl;
        }
    }

    // Overloading the + operator for Matrix Addition
    Matrix operator+(const Matrix& other) {
        if (rows != other.rows || cols != other.cols) {
            cout << "Error: Matrix dimensions must match for addition." << endl;
            return Matrix(0, 0); // Return empty matrix on error
        }

        Matrix result(rows, cols);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                result.data[i][j] = this->data[i][j] + other.data[i][j];
            }
        }
        return result;
    }

    // Overloading the - operator for Matrix Subtraction
    Matrix operator-(const Matrix& other) {
        if (rows != other.rows || cols != other.cols) {
            cout << "Error: Matrix dimensions must match for subtraction." << endl;
            return Matrix(0, 0);
        }

        Matrix result(rows, cols);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                result.data[i][j] = this->data[i][j] - other.data[i][j];
            }
        }
        return result;
    }

    // Overloading the * operator for Matrix Multiplication
    Matrix operator*(const Matrix& other) {
        if (cols != other.rows) {
            cout << "Error: Cols of first matrix must equal rows of second for multiplication." << endl;
            return Matrix(0, 0);
        }

        Matrix result(rows, other.cols);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < other.cols; j++) {
                for (int k = 0; k < cols; k++) {
                    result.data[i][j] += this->data[i][k] * other.data[k][j];
                }
            }
        }
        return result;
    }

    // Check if matrix is valid (not empty)
    bool isValid() const {
        return rows > 0 && cols > 0;
    }
};

int main() {
    int r1, c1, r2, c2;
    int choice;

    cout << "=== Matrix Calculator (Operator Overloading) ===" << endl;

    while (true) {
        cout << "\n1. Add Matrices\n2. Subtract Matrices\n3. Multiply Matrices\n4. Exit\n";
        cout << "Enter choice: ";
        if (!(cin >> choice)) {
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (choice == 4) break;

        // Input dimensions for first matrix
        cout << "\nMatrix A dimensions (rows cols): ";
        cin >> r1 >> c1;
        Matrix matA(r1, c1);
        matA.input();

        // Input dimensions for second matrix
        cout << "\nMatrix B dimensions (rows cols): ";
        cin >> r2 >> c2;
        Matrix matB(r2, c2);
        matB.input();

        cout << "\n--- Matrix A ---" << endl;
        matA.display();
        cout << "\n--- Matrix B ---" << endl;
        matB.display();

        Matrix result(0, 0);

        switch (choice) {
            case 1:
                cout << "\nResult (A + B):" << endl;
                result = matA + matB; // Using overloaded +
                break;
            case 2:
                cout << "\nResult (A - B):" << endl;
                result = matA - matB; // Using overloaded -
                break;
            case 3:
                cout << "\nResult (A * B):" << endl;
                result = matA * matB; // Using overloaded *
                break;
            default:
                cout << "Invalid choice." << endl;
        }

        if (result.isValid()) {
            result.display();
        }
    }

    return 0;
}
