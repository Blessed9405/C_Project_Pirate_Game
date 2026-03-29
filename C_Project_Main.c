#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define maximum_items 5  // Determines maximum number of items a player can hold

/* 
---------------- World Map Of The Game World -----------------

        (Cliff)------>(Volcano)
           |              |
(Beach)-->>(Jungle)-->(Temple)--->>(Treasure)
   |         |          |
(River)    (Cave)    (Camp)
             |
        (Shipwreck)

player (Pirate) moves randomly between these 10 rooms to find the treasure.

---------------------------------------------------------------
*/

//-------- Enumerations(Enums) and Structures(Structs) --------

typedef enum { beach, jungle, cave, river, cliff, shipwreck, camp, temple, volcano, treasure } Room;
typedef enum { map, key, sword, potion, coin } Item;

typedef struct {
    int player_health;
    int player_score;
    Item inventory[maximum_items];
    int count_of_item;
} player;