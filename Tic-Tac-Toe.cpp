#include <iostream>
#include <vector>
#include <limits>

using namespace std;

// Class to manage the game board and game state
class Board {
private:
    char board[3][3];

public:
    // Constructor initializes the board with numbers 1-9
    Board() {
        reset();
    }

    // Resets the board to initial state
    void reset() {
        char count = '1';
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                board[i][j] = count++;
            }
        }
    }

    // Displays the current state of the board
    void display() const {
        cout << "\n";
        cout << " " << board[0][0] << " | " << board[0][1] << " | " << board[0][2] << endl;
        cout << "---|---|---" << endl;
        cout << " " << board[1][0] << " | " << board[1][1] << " | " << board[1][2] << endl;
        cout << "---|---|---" << endl;
        cout << " " << board[2][0] << " | " << board[2][1] << " | " << board[2][2] << endl;
        cout << "\n";
    }

    // Updates the board with the player's move
    // Returns true if move was successful, false if spot is taken or invalid
    bool updateBoard(int slot, char playerSymbol) {
        int row = (slot - 1) / 3;
        int col = (slot - 1) % 3;

        if (board[row][col] != 'X' && board[row][col] != 'O') {
            board[row][col] = playerSymbol;
            return true;
        }
        return false;
    }

    // Checks if the current player has won
    bool checkWin(char playerSymbol) const {
        // Check Rows and Columns
        for (int i = 0; i < 3; i++) {
            if ((board[i][0] == playerSymbol && board[i][1] == playerSymbol && board[i][2] == playerSymbol) ||
                (board[0][i] == playerSymbol && board[1][i] == playerSymbol && board[2][i] == playerSymbol)) {
                return true;
            }
        }
        // Check Diagonals
        if ((board[0][0] == playerSymbol && board[1][1] == playerSymbol && board[2][2] == playerSymbol) ||
            (board[0][2] == playerSymbol && board[1][1] == playerSymbol && board[2][0] == playerSymbol)) {
            return true;
        }
        return false;
    }

    // Checks if the game is a draw
    bool checkDraw() const {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] != 'X' && board[i][j] != 'O') {
                    return false; // Found an empty spot (number)
                }
            }
        }
        return true;
    }
};

// Main Game Logic
int main() {
    Board gameBoard;
    char currentPlayer = 'X';
    int slot;
    bool gameRunning = true;
    
    cout << "=== Tic-Tac-Toe Game ===" << endl;

    while (gameRunning) {
        gameBoard.display();
        cout << "Player " << currentPlayer << ", enter slot (1-9): ";
        
        while (!(cin >> slot) || slot < 1 || slot > 9) {
            cout << "Invalid input. Please enter a number between 1 and 9: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        if (gameBoard.updateBoard(slot, currentPlayer)) {
            if (gameBoard.checkWin(currentPlayer)) {
                gameBoard.display();
                cout << "Congratulations! Player " << currentPlayer << " wins!" << endl;
                gameRunning = false;
            } else if (gameBoard.checkDraw()) {
                gameBoard.display();
                cout << "It's a draw!" << endl;
                gameRunning = false;
            } else {
                // Switch player
                currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
            }
        } else {
            cout << "Slot already taken. Try again." << endl;
        }
    }

    return 0;
}
