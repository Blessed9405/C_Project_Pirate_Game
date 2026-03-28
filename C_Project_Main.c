#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ITEMS 5  // Max items a player can hold

/* 
===================== GAME WORLD MAP ======================

        [CLIFF] ----- [VOLCANO]
           |              |
[BEACH]--[JUNGLE]--[TEMPLE]--[TREASURE]
   |         |          |
[RIVER]   [CAVE]     [CAMP]
                |
           [SHIPWRECK]

Player moves randomly between these 10 rooms.

=========================================================
*/

/* ===================== ENUMS AND STRUCTS ===================== */

typedef enum { BEACH, JUNGLE, CAVE, RIVER, CLIFF, SHIPWRECK, CAMP, TEMPLE, VOLCANO, TREASURE } Room;
typedef enum { MAP, KEY, SWORD, POTION, COIN } Item;

typedef struct {
    int health;
    int score;
    Item inventory[MAX_ITEMS];
    int itemCount;
} Player;

/* ===================== FUNCTION DECLARATIONS ===================== */

void showStory();                  // Displays the game story at start
void showRoom(Room r);             // Shows the current room name
void move(Room *r);                // Handles player movement with input validation
void fight(Player *p);             // Handles combat with enemies, potion use, and score update
void addItem(Player *p, Item item); // Adds item to inventory if space is available
void showInventory(Player *p);     // Displays player health, score, and inventory
int hasItem(Player *p, Item item); // Checks if player has a specific item
void usePotion(Player *p);         // Uses a potion to restore health and removes it from inventory
void ending(Player *p);            // Shows final results when player reaches treasure
const char* getItemName(Item item);         // Returns the name of an item
const char* getItemDescription(Item item);  // Returns the description of an item

/* ===================== MAIN FUNCTION ===================== */

int main() {
    Player player = {100, 0, {COIN}, 1}; // Start with a COIN
    Room currentRoom = BEACH;
    
    srand(time(NULL));
    showStory();

    while (player.health > 0) {
        printf("\n====================================\n");
        showRoom(currentRoom);         // Display current room
        showInventory(&player);        // Display player status
        printf("====================================\n");

        if (rand() % 3 == 0) fight(&player); // Random enemy encounter

        move(&currentRoom);             // Move to next room

        // Room-specific items
        if (currentRoom == SHIPWRECK) addItem(&player, MAP);
        else if (currentRoom == CAVE) addItem(&player, KEY);
        else if (currentRoom == CAMP) addItem(&player, SWORD);
        else if (currentRoom == TEMPLE) addItem(&player, POTION);

        if (currentRoom == TREASURE) { ending(&player); break; }
    }

    if (player.health <= 0) printf("\nYou lost all your health. Game over.\n");
    return 0;
}

/* ===================== STORY AND ROOM DISPLAY ===================== */

// Displays the introduction story of the game
void showStory() {
    printf("\n------------------------------------\n");
    printf("You are a pirate searching for treasure on a hidden island.\n");
    printf("Explore different areas, collect items, and survive enemies.\n");
    printf("Find the treasure before it's too late.\n");
    printf("------------------------------------\n");
}

// Displays the name of the current room
void showRoom(Room r) {
    const char* names[] = { "Beach","Jungle","Cave","River","Cliff","Shipwreck","Camp","Temple","Volcano","Treasure Room" };
    printf("Current Location: %s\n", names[r]);
}

/* ===================== MOVEMENT AND COMBAT ===================== */

// Handles player movement with input validation and random next room
void move(Room *r) {
    int choice;
    while (1) {
        printf("\nChoose direction:\n1. Forward\n2. Left\n3. Right\nEnter choice: ");
        if (scanf("%d", &choice) != 1 || choice < 1 || choice > 3) {
            while(getchar() != '\n');
            printf("Invalid choice. Please enter 1, 2, or 3.\n");
            continue;
        }
        break;
    }
    *r = rand() % 10;
}

// Handles combat, potion use, and score updates
void fight(Player *p) {
    int enemyHealth = 30, choice;
    printf("\n--- Enemy Encounter ---\n");

    while (enemyHealth > 0 && p->health > 0) {
        // Potion usage prompt if player has potion
        if (hasItem(p, POTION)) {
            char c;
            while (1) {
                printf("\nYou have a POTION. Use it? (y for YES/n for NO): ");
                scanf(" %c", &c);
                if (c=='y'||c=='Y') { usePotion(p); break; }
                else if (c=='n'||c=='N') break;
                else printf("Invalid input. Enter 'y' for yes or 'n' for no.\n");
            }
        }

        // Attack or run input validation
        while (1) {
            printf("\n1. Attack\n2. Run\nChoice: ");
            if (scanf("%d", &choice) != 1 || (choice != 1 && choice != 2)) {
                while(getchar() != '\n');
                printf("Invalid input. Enter 1 or 2.\n");
                continue;
            }
            break;
        }

        if (choice == 2) { 
            printf("\nYou ran away safely!\n-----------------------------------\n"); 
            return; 
        }

        // Player attacks enemy
        int damage = rand() % 10 + 5;
        enemyHealth -= damage;
        if (enemyHealth < 0) enemyHealth = 0;
        printf("\nYou attacked the enemy for %d damage!\n", damage);

        // Enemy attacks back if still alive
        if (enemyHealth > 0) {
            p->health -= 10;
            printf("Enemy hit you for 10 damage! Health: %d\n", p->health);
        } else printf("\nEnemy defeated!\n");

        printf("Enemy Health Remaining: %d\n-----------------------------------\n", enemyHealth);
    }

    // Reward for defeating enemy
    if (enemyHealth <= 0) {
        p->score += 10;
        printf("You gained 10 points!\n-----------------------------------\n");
    }
}

/* ===================== INVENTORY AND ITEMS ===================== */

// Adds an item to the player's inventory if there is space
void addItem(Player *p, Item item) {
    if (p->itemCount < MAX_ITEMS) {
        p->inventory[p->itemCount++] = item;
        printf("\nYou found: %s\nInfo: %s\n-----------------------------------\n",
               getItemName(item), getItemDescription(item));
    }
}

// Displays the player's health, score, and inventory
void showInventory(Player *p) {
    printf("\n------ Player Status ------\nHealth: %d\nScore : %d\nInventory:\n", p->health, p->score);
    if (p->itemCount==0) printf("No items yet.\n");
    else for (int i=0;i<p->itemCount;i++) printf("- %s\n", getItemName(p->inventory[i]));
    printf("---------------------------\n");
}

// Checks if player has a specific item in inventory
int hasItem(Player *p, Item item) {
    for (int i=0;i<p->itemCount;i++) if (p->inventory[i]==item) return 1;
    return 0;
}

// Uses a potion to restore health and removes it from inventory
void usePotion(Player *p) {
    for (int i=0;i<p->itemCount;i++) {
        if (p->inventory[i]==POTION) {
            p->health += 30; if(p->health>100) p->health=100;
            printf("\nYou used a POTION! Health restored to %d.\n", p->health);
            p->inventory[i]=COIN; // Replace potion with coin
            return;
        }
    }
}

/* ===================== ITEMS AND ENDING ===================== */

// Returns the name of an item
const char* getItemName(Item item) {
    const char* names[] = { "Map","Key","Sword","Potion","Coin" };
    return names[item];
}

// Returns the description of an item
const char* getItemDescription(Item item) {
    const char* desc[] = {
        "Shows where the treasure is hidden.",
        "Used to unlock the treasure chest.",
        "Helps you fight enemies.",
        "Restores some health.",
        "A shiny coin you found on your adventure."
    };
    return desc[item];
}

// Displays the final results when player reaches treasure
void ending(Player *p) {
    printf("\n====== FINAL RESULT ======\n");
    if (hasItem(p, KEY) && hasItem(p, MAP)) printf("You used the key and map to find the treasure. You win!\n");
    else if (p->score > 20) printf("You found some treasure but missed the main one.\n");
    else printf("You reached the treasure but could not open it.\n");
    printf("==========================\n");
}