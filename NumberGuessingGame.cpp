#include <iostream>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()
#include <limits>  // For numeric_limits

using namespace std;

// Class to manage the Number Guessing Game logic
class Game {
private:
    int secretNumber;
    int attempts;

public:
    // Constructor initializes the random number generator and resets game state
    Game() {
        // Seed the random number generator using current time
        srand(static_cast<unsigned int>(time(0)));
        resetGame();
    }

    // Resets the game with a new random number and zero attempts
    void resetGame() {
        secretNumber = rand() % 100 + 1; // Generate number between 1 and 100
        attempts = 0;
    }

    // Main game loop
    void play() {
        int guess;
        bool hasWon = false;

        cout << "--- Number Guessing Game ---" << endl;
        cout << "I have selected a number between 1 and 100." << endl;
        cout << "Can you guess it?" << endl;

        while (!hasWon) {
            cout << "\nEnter your guess: ";
            
            // Input validation
            while (!(cin >> guess)) {
                cout << "Invalid input. Please enter a number: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }

            attempts++;

            if (guess > secretNumber) {
                cout << "Too high! Try again." << endl;
            } else if (guess < secretNumber) {
                cout << "Too low! Try again." << endl;
            } else {
                cout << "\nCongratulations! You guessed the number " << secretNumber << "!" << endl;
                cout << "It took you " << attempts << " attempts." << endl;
                hasWon = true;
            }
        }
    }
};

int main() {
    Game numberGame;
    char playAgain;

    do {
        numberGame.play();

        cout << "\nDo you want to play again? (y/n): ";
        cin >> playAgain;

        if (playAgain == 'y' || playAgain == 'Y') {
            numberGame.resetGame();
        } else {
            cout << "Thanks for playing! Goodbye." << endl;
        }

    } while (playAgain == 'y' || playAgain == 'Y');

    return 0;
}
