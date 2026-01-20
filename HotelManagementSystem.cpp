#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>

using namespace std;

// Class to represent a Hotel Room
class Room {
public:
    int roomNumber;
    string type;      // e.g., "Single", "Double", "Suite"
    double dailyRate;
    bool isBooked;
    string guestName;

    Room(int number, string t, double rate) 
        : roomNumber(number), type(t), dailyRate(rate), isBooked(false), guestName("") {}

    void book(string name) {
        isBooked = true;
        guestName = name;
    }

    void vacate() {
        isBooked = false;
        guestName = "";
    }

    void displayStatus() const {
        cout << "Room " << roomNumber << " [" << type << "] - Rate: $" << dailyRate << "/night";
        if (isBooked) {
            cout << " | Occupied by: " << guestName;
        } else {
            cout << " | Available";
        }
        cout << endl;
    }
};

// Class to manage the Hotel logic
class Hotel {
private:
    vector<Room> rooms;

public:
    // Initialize hotel with some default rooms
    Hotel() {
        rooms.push_back(Room(101, "Single", 50.0));
        rooms.push_back(Room(102, "Single", 50.0));
        rooms.push_back(Room(201, "Double", 80.0));
        rooms.push_back(Room(202, "Double", 80.0));
        rooms.push_back(Room(301, "Suite", 150.0));
    }

    void displayStatus() {
        cout << "\n--- Hotel Room Status ---" << endl;
        for (const auto& room : rooms) {
            room.displayStatus();
        }
    }

    void checkIn(int roomNum, string guestName) {
        Room* room = findRoom(roomNum);
        if (room == nullptr) {
            cout << "Error: Room not found." << endl;
            return;
        }

        if (room->isBooked) {
            cout << "Error: Room " << roomNum << " is already occupied." << endl;
        } else {
            room->book(guestName);
            cout << "Check-in successful! Room " << roomNum << " booked for " << guestName << "." << endl;
        }
    }

    void checkOut(int roomNum, int daysStayed) {
        Room* room = findRoom(roomNum);
        if (room == nullptr) {
            cout << "Error: Room not found." << endl;
            return;
        }

        if (!room->isBooked) {
            cout << "Error: Room " << roomNum << " is not currently occupied." << endl;
        } else {
            double totalBill = room->dailyRate * daysStayed;
            cout << "\n--- Checkout Bill ---" << endl;
            cout << "Guest: " << room->guestName << endl;
            cout << "Room Number: " << room->roomNumber << " (" << room->type << ")" << endl;
            cout << "Days Stayed: " << daysStayed << endl;
            cout << "Total Amount Due: $" << fixed << setprecision(2) << totalBill << endl;
            
            room->vacate();
            cout << "Checkout complete. Room is now available." << endl;
        }
    }

    // Search for a guest by name
    void searchGuest(string name) {
        bool found = false;
        for (const auto& room : rooms) {
            if (room.isBooked && room.guestName == name) {
                cout << "Guest " << name << " is staying in Room " << room.roomNumber << "." << endl;
                found = true;
            }
        }
        if (!found) {
            cout << "Guest not found." << endl;
        }
    }

private:
    Room* findRoom(int roomNum) {
        for (auto& room : rooms) {
            if (room.roomNumber == roomNum) {
                return &room;
            }
        }
        return nullptr;
    }
};

int main() {
    Hotel myHotel;
    int choice;

    cout << "=== Hotel Management System ===" << endl;

    while (true) {
        cout << "\nMenu:\n";
        cout << "1. View Room Status\n";
        cout << "2. Check-In Guest\n";
        cout << "3. Check-Out Guest\n";
        cout << "4. Search Guest\n";
        cout << "5. Exit\n";
        cout << "Enter choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (choice == 5) {
            cout << "Exiting system..." << endl;
            break;
        }

        switch (choice) {
            case 1:
                myHotel.displayStatus();
                break;
            case 2: {
                int roomNum;
                string name;
                cout << "Enter Room Number to book: ";
                cin >> roomNum;
                cout << "Enter Guest Name: ";
                cin.ignore();
                getline(cin, name);
                myHotel.checkIn(roomNum, name);
                break;
            }
            case 3: {
                int roomNum, days;
                cout << "Enter Room Number for checkout: ";
                cin >> roomNum;
                cout << "Enter Number of Days Stayed: ";
                cin >> days;
                myHotel.checkOut(roomNum, days);
                break;
            }
            case 4: {
                string name;
                cout << "Enter Guest Name to search: ";
                cin.ignore();
                getline(cin, name);
                myHotel.searchGuest(name);
                break;
            }
            default:
                cout << "Invalid choice. Try again." << endl;
        }
    }

    return 0;
}
