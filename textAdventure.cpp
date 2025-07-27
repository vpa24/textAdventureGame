// textAdventure.cpp
// Phuong Anh Vu, CISP 360
// 08/01/2025

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <limits> // required for numeric_limits
using namespace std;

// Room
class Room {
public:
    int id;
    string name;
    string description;
    bool occupied;
    int exits[4]; // N, S, E, W, or 0 means no exit
    // Specification B1 - Light or Dark Room
    bool LightorDark; // Light == 0 or Dark == 1
    int monsterIndex; // Index of the monster in the room
};

class Player {
    public:
    string name;
    int health = 300; // Default health
    // Specification B2 - Add Combat
    int attackPower;
};

// Specification B3 - Put monsters in a Monster() class
class Monster {
    public:
    string name;
    int health;
    int damage;
};


// Function Prototypes
void ProgramGreeting();
void displayInstructions();
int firstRoomChoice();
void displayRoom(Room* room);
void spawnMonster(Room* room, Monster* monsters);
char getValidatedInput();
void encounterMonster(Player* player, int monsterIndex, Monster* monsters);
void encounterRoom(Player* player, Room* room);
int convertExittoDirection(char exitChar);
void LKey(Room* room);
void playGame(Player* player);
void playAgain(Player* player);

int main()
{
    srand(time(0));
    ProgramGreeting();
    displayInstructions();
    
    // Specification C2 - Player Name
    Player* player = new Player;
    cout << "Please enter your name: ";
    getline(cin, player->name);

    playGame(player);
    return 0;
}
void playGame(Player* player)
{
    // Specification C1 - Five Rooms
    Room rooms[6] = {
        // randomly get Light or Dark based on 0 or 1; default value for occupied is false
        // exits are represented as N, S, E, W (as room ID)
        {1, "Red Light", "A dark cave with dripping water.", 0, {0, 0, 2, 0}, rand() % 2 ? true : false, -1},
        {2, "Dead Desert", "A narrow hall lit by torches.", 0, {0, 0, 5, 3}, rand() % 2 ? true : false, -1},
        {3, "The Arena", "Dusty old weapons lie scattered.", 0, {0, 0, 0, 4}, rand() % 2 ? true : false, -1},
        {4, "Console Corner", "You smell smoke and hear growls.", 0, {2, 4, 0, 0}, rand() % 2 ? true : false, -1},
        {5, "Electric Loft", "Random electric sounds buzz around you!", 0, {3, 0, 0, 0}, rand() % 2 ? true : false, -1},
        // Specification A2 - Add more rooms
        {6, "The Dungeon", "A devil room with eerie sounds.", 0, {0, 0, 0, 1}, rand() % 2 ? true : false, -1}};

    // Specification B3 - Put monsters in a Monster() class
    Monster monsters[6] = {
        {"Goblin", 90, 10},
        {"Zombie", 80, 15},
        {"Vampire", 60, 20},
        {"Dragon", 70, 30},
        // Specification A3 - Add more Monsters
        {"Orc", 40, 25}};


    int roomNumber = firstRoomChoice();
    Room* roomChoicePtr = &rooms[roomNumber]; // Get the room object based on the user's choice
    // Specification C4 - Abbreviated Room Description
    displayRoom(roomChoicePtr);
    // Spawn a monster in the chosen room
    spawnMonster(roomChoicePtr, monsters); // Spawn a monster in the chosen room

    player->attackPower = 10;
    encounterMonster(player, roomChoicePtr->monsterIndex, monsters);
    encounterRoom(player, roomChoicePtr); // Encounter the room's monster

            

    while (player->health > 0)
    {
        
        char choice = getValidatedInput();
        if(choice == 'L')
        {
            LKey(roomChoicePtr);
        } else{
            cout << "Your Current Health: " << player->health << endl;
            int dir = convertExittoDirection(choice);
            if (roomChoicePtr->exits[dir] == 0)
            {
                cout << "You can't go that way!";
            }
            else
            {
                roomNumber = roomChoicePtr->exits[dir];
                roomChoicePtr = &rooms[roomNumber]; // Update the room pointer
                spawnMonster(roomChoicePtr, monsters); // Spawn a monster in the new room
                displayRoom(roomChoicePtr);
                encounterMonster(player, roomChoicePtr->monsterIndex, monsters);
                encounterRoom(player, roomChoicePtr);
            }
        }
    }

    if(player->health <= 0)
    {
        cout << "You have lost all your health! Game Over!" << endl;
        playAgain(player); // Ask if the player wants to play again
    } 
}


// Program Greeting
void ProgramGreeting()
{
    cout << "Welcome to the Text Adventure Game!" << endl;
    cout << "My name is Phuong Anh Vu." << endl;
    time_t now = time(0);
    struct tm *ltm = localtime(&now);

    char buffer[80];
    strftime(buffer, sizeof(buffer), "%B %d, %Y", ltm); // %B = full month, %d = day, %Y = year

    cout << "Today's day: " << buffer << endl;
    cout << "**********************************************" << endl
            << endl
            << endl;
}

void displayInstructions()
{
    cout << "===============================================" << endl;
    cout << "Please read the instructions carefully before playing game." << endl;
    cout << "First, you will ask to enter your name." << endl;
    cout << "Then, you can enter a number from 1 to 6 to choose a room." << endl;
    cout << "When you enter a room, you will see the room description." << endl;
    cout << "Each room may contain a monster that will attack you, and randomly generate loot. "
            << "Based on the room type, which is light or dark, you may gain or lose health." << endl;
    cout << "You can move to another room by entering N, S, E, or W." << endl;
    cout << "You can move around 6 rooms in total. If you kill one monster, you will WIN." << endl;
    cout << "Otherwise, if you lose all your health, you will LOSE." << endl;
    cout << "================================================" << endl;
    cout << "MAY THE FORCE BE WITH YOU! I wish you would become a Jedi Monster Slayer! :)" << endl;
    cout << "===============================================" << endl;
    string input;
    do
    {
        cout << "Press Enter to continue..." << endl;
        getline(cin, input);
    } while (!input.empty()); // keep looping until user presses only Enter
}

// First Room Choice
int firstRoomChoice()
{
    int roomChoice;
    cout << "Please choose your starting room (1-6): ";
    do
    {
        cin >> roomChoice;
        if (roomChoice < 1 || roomChoice > 6)
        {
            cout << "Invalid choice! Please enter a number between 1 and 6: ";
        }
    } while (roomChoice < 1 || roomChoice > 6);
    return roomChoice - 1; // Return room's ID
}

// Display room info
void displayRoom(Room* room)
{
    cout << "Here is the room info:\n";
    cout << "Room Name: " << room->name << endl;
    if(!room->occupied) {
        cout << "Description: " << room->description << endl;
    }
}

void spawnMonster(Room* room, Monster* monsters)
{
    Room& roomInfo = *room;
    int monsterIndex = rand() % 6;  // Randomly select a monster index
    Monster& monster = monsters[monsterIndex];
    while (!roomInfo.occupied && roomInfo.monsterIndex != monsterIndex) // If the room is not occupied and the current monster is not already in the room
    {
            roomInfo.occupied = true;
            roomInfo.monsterIndex = monsterIndex;
            cout << "A " << monster.name << " appears in the room!" << endl;
            cout << "It has " << monster.health << " health and deals " << monster.damage << " damage." << endl;
    }
}

char getValidatedInput()
{
    cout << "Move (N/S/E/W) or (L)ook the room info again: ";
    char input;
    // Specification C3 - Input Validation
    while (true)
    {
        cin >> input;
        input = toupper(input);
        if (input == 'N' || input == 'S' || input == 'E' || input == 'W' || input == 'L')
            return input;
        cout << "Invalid input. Try N, S, E, W, or L: ";
    };
}

// Specification B1 - Add Combat (Monster deals 10 damage)
void encounterMonster(Player* player, int monsterIndex, Monster* monsters)
{

    Monster& monster = monsters[monsterIndex]; // Get the monster from monsterIndex

    player->health -= monster.damage;
    monster.health -= player->attackPower;
    if (monster.health <= 0) {
        cout << "You defeated the " << monster.name << "!" << endl;
        cout << "Congratulations, " << player->name << "! You have won the game!" << endl;
        playAgain(player);
    } else {
        cout << "You attacked the " << monster.name << " and it has " << monster.health << " health left." << endl;
    }

    if (player->health <= 0)
    {
        cout << "You have been defeated by the " << monster.name << "!" << endl;
        playAgain(player);
    }
}

void encounterRoom(Player* player, Room* room)
{
    if (room->LightorDark) // If the room is dark
    {
        cout << "You are in a dark room. Your health is reduced by 20." << endl;
        player->health -= 20;
    }
    else // If the room is light
    {
        cout << "You are in a light room. Your health is increased by 30." << endl;
        player->health += 30;
    }
    
    if (player->health <= 0)
    {
        cout << "You have lost all your health! Game Over!" << endl;
        playAgain(player);
    }
}

// Specification A1 - User can play again
void playAgain(Player* player) {
    char choice;
    do {
        cout << "Do you want to play again? (Y/N): ";
        cin >> choice;
        if (choice == 'Y' || choice == 'y') {
            player->health = 300; // Reset health
            cout << "Starting a new game... Ready to have more fun!" << endl;
            playGame(player);
        }
        else if (choice == 'N' || choice == 'n') {
            cout << "Thank you for playing! Goodbye!" << endl;
            exit(0); // Exit the program
        }
    } while (choice != 'Y' && choice != 'N');
}
int convertExittoDirection(char exitChar) {
    switch (exitChar) {
    case 'N':
        return 0;
    case 'S':
        return 1;
    case 'E':
        return 2;
    case 'W':
        return 3;
    }
    return -1; // Invalid direction
}

// Specification C5 - Detailed Look
void LKey(Room* room) {
    cout << "You are in the room: " << room->name << endl;
    cout << "Description: " << room->description << endl;
}
