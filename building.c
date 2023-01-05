#include "gameDefs.h"

/*
  Function: initBuilding
  Purpose:  function that initializes fields of the given type to value in parameters or default values.
       in:  location of the old building to be initialized
       out: location of the new  building that is initialized
  return:  void
*/
void initBuilding(BuildingType** build){
    BuildingType* newBuilding = malloc(sizeof(BuildingType));
    memAllocCheck(newBuilding);
    initRoomList(&(newBuilding->rooms));

    populateRooms(newBuilding);

    GhostType* ghost;

    //Ghost
    RoomNodeType* ghostStartRoom;
    getRandomRoom(&newBuilding->rooms, &ghostStartRoom);
    initGhost("Casper", randInt(0,4), ghostStartRoom->room, &ghost);
    newBuilding->ghost = ghost;

    *build = newBuilding;
}

/*
  Function: cleanupBuilding
  Purpose:  function that cleans up all the dynamically allocated memory in the given building b.
       in:  location of the building type to clean
  return:   void
*/
void cleanBuilding(BuildingType* building){
    for(int i = 0; i < MAX_HUNTERS; ++i) {
        cleanEvidenceListData(building->allHunters[i]->evidenceCollected);
        cleanEvidenceListNodes(building->allHunters[i]->evidenceCollected);
    }

    for(int i = 0; i < MAX_HUNTERS; ++i){
        free(building->allHunters[i]->evidenceCollected);
        free(building->allHunters[i]);
    }

    cleanRoomListData(&building->rooms);
    cleanRoomListNodes(&building->rooms);
    free(building->ghost);
    free(building);
}

/*
 Function:    populateRooms
  Purpose:    Generate Initial Building Map
       in:    Building Type
       out:   Initialised Building Rooms & Connections
    return:   void

 */
void populateRooms(BuildingType* building) {

    char roomNames[MAX_ROOMS][MAX_STR] = {
            "Van", "Hallway", "Master Bedroom", "Boy's Bedroom",
            "Bathroom", "Basement", "Basement Hallway",
            "Right Storage Room", "Left Storage Room", "Kitchen",
            "Living Room", "Garage","Utility Room"
    };

    RoomType* allRooms[MAX_ROOMS];

    for(int i = 0; i < MAX_ROOMS; ++i){
        RoomType* newRoom;
        initRoom(&newRoom, roomNames[i]);

        allRooms[i] = newRoom;

        appendRoom(&building->rooms, newRoom);
    }

    connectRooms(allRooms[0], allRooms[1]);                       // Van -> Hallway
    connectRooms(allRooms[0], allRooms[11]);                      // Van -> Garage
    connectRooms(allRooms[1], allRooms[2]);                       // Hallway -> Master Bedroom
    connectRooms(allRooms[1], allRooms[3]);                       // Hallway -> Boy's Bedroom
    connectRooms(allRooms[1], allRooms[4]);                       // Hallway -> Bathroom
    connectRooms(allRooms[1], allRooms[9]);                       // Hallway -> Kitchen
    connectRooms(allRooms[1], allRooms[5]);                       // Hallway -> Basement
    connectRooms(allRooms[5], allRooms[6]);                       // Basement -> Basement Hallway
    connectRooms(allRooms[6], allRooms[7]);                       // Basement Hallway -> Right Storage Room
    connectRooms(allRooms[6], allRooms[8]);                       // Basement Hallway -> Left Storage Room
    connectRooms(allRooms[9], allRooms[10]);                      // Kitchen -> Living Room
    connectRooms(allRooms[9], allRooms[11]);                      // Kitchen -> Garage
    connectRooms(allRooms[11], allRooms[12]);                     // Garage -> Utility Room
}