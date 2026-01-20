#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
#include <ctime>

using namespace std;

// Forward Declarations
class Player;
class Enemy;

// === Item System ===
// Base class for all items
class Item {
protected:
    string name;
    string description;
public:
    Item(string n, string d) : name(n), description(d) {}
    virtual ~Item() {}
    string getName() const { return name; }
    virtual void use(Player& player) = 0; // Pure virtual function
};

// Derived Potion Class
class Potion : public Item {
    int healAmount;
public:
    Potion(string n, string d, int heal) : Item(n, d), healAmount(heal) {}
    void use(Player& player) override; // Defined after Player class
};

// Derived Weapon Class
class Weapon : public Item {
    int damageBonus;
public:
    Weapon(string n, string d, int dmg) : Item(n, d), damageBonus(dmg) {}
    void use(Player& player) override; // Defined after Player class
    int getDamageBonus() const { return damageBonus; }
};

// === Enemy Class ===
class Enemy {
    string name;
    int health;
    int damage;
public:
    Enemy(string n, int h, int d) : name(n), health(h), damage(d) {}
    
    bool isAlive() const { return health > 0; }
    string getName() const { return name; }
    
    void takeDamage(int dmg) {
        health -= dmg;
        if (health < 0) health = 0;
        cout << "  > " << name << " takes " << dmg << " damage!" << endl;
    }
    
    // Attack method requires Player definition, defined later
    void attack(Player& player); 
};

// === Player Class ===
class Player {
    string name;
    int health;
    int maxHealth;
    int baseDamage;
    vector<Item*> inventory;
    Weapon* currentWeapon;

public:
    Player(string n) : name(n), health(100), maxHealth(100), baseDamage(10), currentWeapon(nullptr) {}

    bool isAlive() const { return health > 0; }
    string getName() const { return name; }

    void takeDamage(int dmg) {
        health -= dmg;
        if (health < 0) health = 0;
        cout << "  > " << name << " takes " << dmg << " damage! (HP: " << health << "/" << maxHealth << ")" << endl;
    }

    void heal(int amount) {
        health += amount;
        if (health > maxHealth) health = maxHealth;
        cout << "  > You are healed for " << amount << " HP. (HP: " << health << "/" << maxHealth << ")" << endl;
    }

    void equipWeapon(Weapon* w) {
        currentWeapon = w;
        cout << "  > You equipped " << w->getName() << ". Damage increased!" << endl;
    }

    int getAttackDamage() const {
        int total = baseDamage;
        if (currentWeapon) {
            total += currentWeapon->getDamageBonus();
        }
        return total;
    }

    void addItem(Item* item) {
        inventory.push_back(item);
        cout << "  > You picked up: " << item->getName() << endl;
    }

    void showInventory() {
        cout << "\n--- Inventory ---" << endl;
        if (inventory.empty()) {
            cout << "  (Empty)" << endl;
            return;
        }
        for (size_t i = 0; i < inventory.size(); ++i) {
            cout << i + 1 << ". " << inventory[i]->getName() << endl;
        }
    }

    // Handles item usage logic
    void useItemFromInventory(int index) {
        if (index < 1 || index > (int)inventory.size()) {
            cout << "Invalid selection." << endl;
            return;
        }
        
        Item* item = inventory[index - 1];
        item->use(*this); // Polymorphic call

        // If it's a potion, remove it. If it's a weapon, keep it equipped but in inventory
        if (dynamic_cast<Potion*>(item)) {
            inventory.erase(inventory.begin() + index - 1);
            delete item; // Consume the potion object
        }
    }
};

// === Implementation of Deferred Methods ===

void Potion::use(Player& player) {
    cout << "  > You drink the " << name << "." << endl;
    player.heal(healAmount);
}

void Weapon::use(Player& player) {
    player.equipWeapon(this);
}

void Enemy::attack(Player& player) {
    cout << "  > " << name << " attacks you!" << endl;
    player.takeDamage(damage);
}

// === Room Class ===
class Room {
    string description;
    map<string, Room*> exits;
    Enemy* enemy;
    Item* item;

public:
    Room(string desc) : description(desc), enemy(nullptr), item(nullptr) {}
    
    // Connect rooms
    void setExit(string direction, Room* neighbor) {
        exits[direction] = neighbor;
    }

    void setEnemy(Enemy* e) { enemy = e; }
    void setItem(Item* i) { item = i; }

    Room* getExit(string direction) {
        if (exits.find(direction) != exits.end()) {
            return exits[direction];
        }
        return nullptr;
    }

    void describe() {
        cout << "\n------------------------------------------------" << endl;
        cout << description << endl;
        if (enemy) {
            cout << "DANGER: You see a " << enemy->getName() << " here!" << endl;
        }
        if (item) {
            cout << "ITEM: There is a " << item->getName() << " on the ground." << endl;
        }
        cout << "Exits: ";
        for (auto const& [dir, room] : exits) {
            cout << "[" << dir << "] ";
        }
        cout << endl;
    }

    Enemy* getEnemy() { return enemy; }
    void removeEnemy() { 
        if (enemy) {
            delete enemy;
            enemy = nullptr; 
        }
    }

    Item* getItem() { return item; }
    void removeItem() { item = nullptr; }
};

// === Game Main ===
int main() {
    srand(time(0));
    string playerName;

    cout << "=== TEXT RPG ADVENTURE ===" << endl;
    cout << "Enter your character's name: ";
    getline(cin, playerName);
    Player player(playerName);

    // --- World Setup ---
    Room* start = new Room("You waken in a damp dungeon cell. The iron door is open.");
    Room* hallway = new Room("A dark stone hallway. Torches flicker on the walls.");
    Room* armory = new Room("An abandoned armory. Racks of rusty weapons line the walls.");
    Room* throne = new Room("A grand throne room. The air is cold and ominous.");

    // Map Connections
    start->setExit("north", hallway);
    hallway->setExit("south", start);
    hallway->setExit("east", armory);
    hallway->setExit("north", throne);
    armory->setExit("west", hallway);
    throne->setExit("south", hallway);

    // Items and Enemies
    start->setItem(new Potion("Small Health Potion", "A vial of red liquid.", 30));
    armory->setItem(new Weapon("Steel Sword", "A sharp steel blade.", 20)); // +20 dmg
    hallway->setEnemy(new Enemy("Goblin", 40, 5));
    throne->setEnemy(new Enemy("Dark Lord", 100, 15));

    // --- Game Loop ---
    Room* currentRoom = start;
    bool gameRunning = true;

    while (gameRunning && player.isAlive()) {
        currentRoom->describe();

        // Check for Combat
        if (currentRoom->getEnemy()) {
            Enemy* enemy = currentRoom->getEnemy();
            cout << "\nCOMBAT STARTED with " << enemy->getName() << "!" << endl;
            
            while (enemy->isAlive() && player.isAlive()) {
                cout << "\nActions: 1. Attack  2. Inventory  3. Run" << endl;
                cout << "Choice: ";
                int combatChoice;
                cin >> combatChoice;

                if (combatChoice == 1) {
                    int dmg = player.getAttackDamage();
                    cout << "  > You hit " << enemy->getName() << " for " << dmg << " damage." << endl;
                    enemy->takeDamage(dmg);
                    
                    if (enemy->isAlive()) {
                        enemy->attack(player);
                    }
                } else if (combatChoice == 2) {
                    player.showInventory();
                    cout << "Enter item number to use (0 to cancel): ";
                    int itemIdx;
                    cin >> itemIdx;
                    if (itemIdx != 0) {
                        player.useItemFromInventory(itemIdx);
                        // Enemy attacks while you fumble with inventory
                        if (enemy->isAlive()) enemy->attack(player);
                    }
                } else if (combatChoice == 3) {
                    cout << "  > You try to run away..." << endl;
                    if (rand() % 2 == 0) {
                        cout << "  > Escaped successfully!" << endl;
                        // Move back south if possible, or just break loop (simplified)
                        goto movement_phase; 
                    } else {
                        cout << "  > Failed to escape!" << endl;
                        enemy->attack(player);
                    }
                }
            }

            if (!player.isAlive()) {
                cout << "\n*** YOU DIED ***" << endl;
                gameRunning = false;
            } else if (!enemy->isAlive()) {
                cout << "\n*** VICTORY! ***" << endl;
                cout << "You defeated the " << enemy->getName() << "!" << endl;
                currentRoom->removeEnemy();
                
                if (currentRoom == throne) {
                    cout << "\nCongratulations! You have defeated the Dark Lord and cleared the dungeon!" << endl;
                    gameRunning = false;
                }
            }
        }

        // Movement Phase
        movement_phase:
        if (gameRunning) {
            cout << "\nCommands: go [north/south/east/west], take, inventory, quit" << endl;
            cout << "> ";
            string cmd, arg;
            cin >> cmd;

            if (cmd == "quit") {
                gameRunning = false;
            } else if (cmd == "go") {
                cin >> arg;
                Room* next = currentRoom->getExit(arg);
                if (next) {
                    currentRoom = next;
                } else {
                    cout << "  > You can't go that way." << endl;
                }
            } else if (cmd == "take") {
                if (currentRoom->getItem()) {
                    player.addItem(currentRoom->getItem());
                    currentRoom->removeItem();
                } else {
                    cout << "  > Nothing to take here." << endl;
                }
            } else if (cmd == "inventory") {
                player.showInventory();
                cout << "Enter item number to use (0 to cancel): ";
                int idx;
                cin >> idx;
                if (idx != 0) player.useItemFromInventory(idx);
            } else {
                cout << "  > Unknown command." << endl;
                // Clear input buffer in case of multi-word garbage
                cin.clear();
                cin.ignore(1000, '\n'); 
            }
        }
    }

    cout << "Thanks for playing!" << endl;
    return 0;
}
