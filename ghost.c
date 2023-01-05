#include "gameDefs.h"


/*
 Function: ghostSprite
  Purpose: Instructions for the ghost as the game continues
   in/out: GhostType To Instruct
 */

void* ghostSprite(void* ghostType){
    GhostType* ghost = ghostType;

    while(1){
        //printGhostType(ghost);
        //usleep(USLEEP_TIME);
        int roomOptions[3] = {-1, -1, -1};

        if(ghost->currRoom->amountHuntersInRoom <= 0){
            // If No Hunters Are In The Room With Ghost
            ghost->boredomTimer--;
            if(ghost->boredomTimer <= 0){
                // Ghost Got Too Bored And Left
                break;
            }
            roomOptions[0] = 0;
            roomOptions[1] = 1;
            roomOptions[2] = 2;
        }else{
            //If 1 or More Hunters Are In The Room With Ghost
            ghost->boredomTimer = BOREDOM_MAX;
            roomOptions[1] = 1;
            roomOptions[2] = 2;
        }

        RoomNodeType* newRoomNode;
        switch(makeChoice(roomOptions, 3)){
            case 1:
                // Move Rooms
                getRandomRoom(ghost->currRoom->adjacentRooms, &newRoomNode);

                if(sem_trywait(&(newRoomNode->room->semaphore)) == 0){
                    //Switch Rooms
                    sem_wait(&(ghost->currRoom->semaphore));
                    //Remove From Old Room
                    ghost->currRoom->ghost = NULL;
                    ghost->currRoom = NULL;

                    //Add Ghost To New Room
                    newRoomNode->room->ghost = ghost;

                    //Set Room As Ghost's Current Room
                    ghost->currRoom = newRoomNode->room;

                    sem_post(&(ghost->currRoom->semaphore));
                    sem_post(&(newRoomNode->room->semaphore));
                    //usleep(USLEEP_TIME);
                }
                break;


            case 2:
                // Leave Evidence

                if(sem_trywait(&(ghost->currRoom->semaphore)) == 0){
                    // Generate New Evidence
                    dropEvidence(ghost);
                    sem_post(&(ghost->currRoom->semaphore));
                    //usleep(USLEEP_TIME);
                }
                break;

            default:
                // Do Nothing
                break;
        }
    }
    sem_wait(&(ghost->currRoom->semaphore));
    ghost->currRoom->ghost = NULL;
    sem_post(&(ghost->currRoom->semaphore));
    ghost->currRoom = NULL;
    printf("%s\n", "Ghost Has Exited The Building");
    printf("\n");

    pthread_exit(NULL);
    return FUNC_SUCCESS;
}



/*
  Function: initGhost
  Purpose:  function that dynamically allocates memory for a GhostType structure,
            initializes fields, and “returns” this new structure using the ghost parameter
       in:  location of the old ghost type to be initialized
       out: location of the new ghost type that is initialized
       in:  name of the ghost
       in:  ghost class type
       in:  the room the ghost is currently in
  return:   FUNC_SUCCESS if success, MEMORY_ALLOCATION_ERROR for memory allocating failing
*/

int initGhost(char* name, GhostClassType ghostClass, RoomType* currRoom, GhostType** ghost){
    GhostType* newGhost = (GhostType*) malloc(sizeof(GhostType));
    memAllocCheck(newGhost);

    newGhost->ghostClass = ghostClass;
    newGhost->currRoom = currRoom;
    newGhost->boredomTimer = BOREDOM_MAX;

    strcpy(newGhost->name, name);

    (*ghost) = newGhost;

    return FUNC_SUCCESS;
}


/*
  Function: printGhostType
  Purpose:  prints the ghost type
       in:  location of ghost to print
  return:   void
*/
void printGhostType(GhostType* ghost){
    printf("%s %s\n", "Ghost Name:", ghost->name);

    char name[MAX_STR];
    getGhostClassName(name, ghost->ghostClass);
    printf("%s %s\n", "Ghost Class:", name);
    printf("%s %s\n", "Ghost Room:", ghost->currRoom->name);
    printf("%s %d\n", "Ghost Boredom:", ghost->boredomTimer);
    printf("\n");
}

/*
  Function: getGhostClassName
  Purpose:  finds the ghost class type based on the ghost type
      out:  the ghost type as a string
       in:  the ghost class
  return:   void
*/
void getGhostClassName(char* ghostType, GhostClassType class){
    switch (class) {
        case POLTERGEIST:
            strcpy(ghostType, "POLTERGEIST");
            break;
        case BANSHEE:
            strcpy(ghostType, "BANSHEE");
            break;
        case BULLIES:
            strcpy(ghostType, "BULLIES");
            break;
        case PHANTOM:
            strcpy(ghostType, "PHANTOM");
            break;
        default:
            break;
    }

}


/*
  Function: isGhostly
  Purpose:  function that checks if the ghost is "ghostly" based on certain criteria
       in:  location of the ghost node type
       in:  the class type of the ghost node
  return:   1 if it is ghostly, 0 if it's not ghostly
*/
int isGhostly(EvidenceType* evidence, EvidenceClassType class){
    double data = evidence->data;
    int isGhostly = 1;
    switch (class) {
        case EMF:
            if(4.70 <= data && data <= 5.00){
                isGhostly = 0;
            }
            break;
        case TEMPERATURE:
            if(-10 <= data && data <= 1.00){
                isGhostly = 0;
            }
            break;
        case FINGERPRINTS:
            if(data == 1){
                isGhostly = 0;
            }
            break;
        case SOUND:
            if(65 <= data && data <= 75.00){
                isGhostly = 0;
            }
            break;
        default:
            break;
    }

    return isGhostly;
}