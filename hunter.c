#include "gameDefs.h"


void* hunterSprite(void* hunterType){
    HunterType* hunter = hunterType;

    char className[MAX_STR];
    getEvidenceClassName(className, hunter->readableEvidence);

    while(1){
        //printHunterType(hunter);
        //usleep(USLEEP_TIME);

        // If Hunter Has Collected 3 or More Pieces Of Ghostly Evidence
        if(hunter->ghostlyEvidenceCollected >= 3){
            break;
        }

        // If Ghost Is In Hunters Current Room, Increment By FEAR_RATE
        if(hunter->currRoom->ghost != NULL){
            hunter->fear += FEAR_RATE;
            // If Hunter Reaches MAX_FEAR, Exit Building
            if(hunter->fear >= MAX_FEAR){
                break;
            }
            // Hunter Is No Longer Bored, Reset Boredom Level To BOREDOM_MAX
            hunter->boredomTimer = BOREDOM_MAX;
        }else{
            // If No Ghost Is Found In Room, Decrement Hunter boredomTimer By 1
            hunter->boredomTimer--;
            // If Hunter boredomTimer Falls Below 0, Exit Building
            if(hunter->boredomTimer <= 0){
                break;
            }
        }

        RoomNodeType* newRoomNode;
        switch(randInt(0,3)){
            case 0:
                // Move Rooms
                getRandomRoom(hunter->currRoom->adjacentRooms, &newRoomNode);
                if(sem_trywait(&(newRoomNode->room->semaphore)) == 0){           // <---------------- Lock New Room
                    // Lock Current Room And Move To New Room
                    sem_wait(&(hunter->currRoom->semaphore));                   // <---------------- Lock Curr Room
                    RoomType* prevRoom = hunter->currRoom;
                    moveHuntersRoom(hunter, newRoomNode);

                    // Unlock Both Rooms
                    sem_post(&(prevRoom->semaphore));                         // <---------------- Unlock Prev Room
                    sem_post(&(hunter->currRoom->semaphore));                 // <---------------- Unlock Curr Room
                    //usleep(USLEEP_TIME);
                }

                break;
            case 1:
                // Collect Evidence
                sem_wait(&(hunter->currRoom->semaphore));                       // <---------------- Lock Curr Room
                pickUpEvidence(hunter);
                sem_post(&(hunter->currRoom->semaphore));                     // <---------------- Unlock Curr Room
                //usleep(USLEEP_TIME);
                break;
            case 2:
                // Communicate Evidence
                sem_wait(&(hunter->currRoom->semaphore));                       // <---------------- Lock Curr Room


                HunterType *hunterChosen = NULL;

                if(hunter->currRoom->amountHuntersInRoom > 1){
                    if (hunter->currRoom->amountHuntersInRoom == 2){
                        for(int i = 0 ;  i < 2 ; ++i){
                            if (hunter->currRoom->huntersInRoom[i] != hunter){
                                hunterChosen = hunter->currRoom->huntersInRoom[i];
                                break;
                            }
                        }
                    }

                    if (hunter->currRoom->amountHuntersInRoom > 2){
                        int count = 0;
                        while(count < 5) {
                            int randomNumber = randInt(0, hunter->currRoom->amountHuntersInRoom);
                            if (hunter->currRoom->huntersInRoom[randomNumber] != hunter) {
                                hunterChosen = hunter->currRoom->huntersInRoom[randomNumber];
                                break;
                            }else{
                                count++;
                            }
                        }

                    }

                    if(hunterChosen != NULL && hunterChosen->evidenceCollected->size > 0) {

                        EvidenceType* newEvidence = NULL;
                        EvidenceNodeType* newEvidenceNode = NULL;

                        for(int i = 0; i < 5; ++i) {
                            //Get Random Evidence
                            getRandomEvidence(hunterChosen->evidenceCollected, &newEvidence);
                            if(isGhostly(newEvidence, hunterChosen->readableEvidence) == 0){
                                break;
                            }else{
                                newEvidence = NULL;
                            }
                        }

                        if(newEvidence != NULL){
                            //Copy Evidence Into New EvidenceType
                            EvidenceType* evidenceToAdd = NULL;
                            initEvidence(&evidenceToAdd, newEvidence->data, newEvidence->evidenceClass);
                            initEvidenceNode(&newEvidenceNode, evidenceToAdd);

                            //Add Copied Evidence To Hunter
                            appendEvidence(hunter->evidenceCollected, newEvidenceNode);
                            hunter->ghostlyEvidenceCollected++;
                            printf("Communicating");
                        }
                    }

                }

                sem_post(&(hunter->currRoom->semaphore));                     // <---------------- Unlock Curr Room
                //usleep(USLEEP_TIME);

                break;
            default:
                break;
        }
    }
    sem_wait(&(hunter->currRoom->semaphore));                                   // <---------------- Lock Curr Room
    removeHunterFromRoom(hunter, hunter->currRoom);
    sem_post(&(hunter->currRoom->semaphore));                                 // <---------------- Unlock Curr Room

    printf("%s %s %s\n", "Hunter", hunter->name,"Has Exited The Building");
    printf("\n");
    pthread_exit(NULL);
    return FUNC_SUCCESS;
}




/*
  Function: initHunter
  Purpose:  function that initializes fields of the given type to value in parameters or default values
       in:  location of the old hunter type to be initialized
       out: location of the new hunter type that is initialized
       in:  name of the hunter
       in:  the room the hunter is currently in.
       in:  the evidence class type hunter can collect
  return:   FUNC_SUCCESS if success, MEMORY_ALLOCATION_ERROR for memory allocating failing
*/
int initHunter(char* name, RoomType* room, EvidenceClassType class, HunterType** hunter){
    HunterType* newHunter = malloc(sizeof(HunterType));
    memAllocCheck(newHunter);

    strcpy(newHunter->name, name);
    newHunter->currRoom = room;
    newHunter->fear = 0;
    newHunter->boredomTimer = BOREDOM_MAX;
    newHunter->ghostlyEvidenceCollected = 0;

    EvidenceLinkedList* evidenceFound = malloc(sizeof(EvidenceLinkedList));
    memAllocCheck(evidenceFound);

    initEvidenceList(evidenceFound);
    newHunter->evidenceCollected = evidenceFound;


    initEvidenceList(newHunter->evidenceCollected);
    newHunter->readableEvidence = class;

    room->huntersInRoom[room->amountHuntersInRoom] = newHunter;

    room->amountHuntersInRoom++;

    *hunter = newHunter;

    return FUNC_SUCCESS;

}


/*
  Function: moveHuntersRoom
  Purpose:  function that changes the hunters current room into another room (stimulates moving)
       in:  the hunter that is going to be moved
       out: the hunter with the currRoom field changed
       in:  the NEW room the hunter wants to move into
  return:   void
*/
void moveHuntersRoom(HunterType* hunter, RoomNodeType* newRoom){
    //Remove From Old Room

    removeHunterFromRoom(hunter, hunter->currRoom);

    //Add Hunter To New Room
    newRoom->room->huntersInRoom[newRoom->room->amountHuntersInRoom] = hunter;
    newRoom->room->amountHuntersInRoom++;


    //Set Room As Hunter's Current Room
    hunter->currRoom = newRoom->room;
}

void removeHunterFromRoom(HunterType* hunter, RoomType* room){
    int index = 0;
    for(int i = 0; i < room->amountHuntersInRoom; ++i){

        if(room->huntersInRoom[i] == hunter){
            index = i;
            break;
        }
    }

    //shifting
    for (int i = index ; i < hunter->currRoom->amountHuntersInRoom-1; ++i){
        room->huntersInRoom[i] = room->huntersInRoom[i+1];
    }
    room->amountHuntersInRoom--;
}

/*
  Function: printHunterType
  Purpose:  prints the Hunter type
       in:  Hunter to print
  return:   void
*/
void printHunterType(HunterType* hunter){
    printf("%s %s\n", "Hunter Name:", hunter->name);

    char name[MAX_STR];
    getEvidenceClassName(name, hunter->readableEvidence);
    printf("%s %s\n", "Hunter's Readable Evidence:", name);
    printf("%s %s\n", "Hunter Room:", hunter->currRoom->name);
    printf("%s %d\n", "Hunter Boredom:", hunter->boredomTimer);
    printf("%s %d\n", "Hunter Fear:", hunter->fear);
    printf("%s %d\n", "Hunter's Ghost Evidence Collected:", hunter->ghostlyEvidenceCollected);
    printf("\n");
}