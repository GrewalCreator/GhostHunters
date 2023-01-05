#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-msc51-cpp"
#include "gameDefs.h"

/*
  Function:  main
  Purpose:   calls all functions and gets user input
   return:   0 for success,MEMORY_ALLOCATION_ERROR for memory allocation error
*/
int main(){
    // Set Random Seed
    srand(time(NULL));

    // Init Building
    BuildingType* building;
    initBuilding(&building);

    populateRooms(building);

    char hunterName[MAX_STR];

    for(int i = 0; i < MAX_HUNTERS; ++i){
        printf("%s %d %s\n", "Please Enter Hunter", i + 1 ,"'s Names:");
        fgets(hunterName, MAX_STR, stdin);

        hunterName[strlen(hunterName)-1] = '\0';

        HunterType* newHunter;
        initHunter(hunterName, building->rooms.head->room, i, &newHunter);
        building->allHunters[i] = newHunter;
    }


    startGame(building, building->allHunters);

    identifyGhostType(building->ghost, building->allHunters);
    sem_destroy_all(&building->rooms);


    cleanBuilding(building);
}

void startGame(BuildingType* building, HunterType** hunterList){
    pthread_t ghost;
    pthread_t hunter[MAX_HUNTERS];

    if (pthread_create(&ghost, NULL, &ghostSprite, building->ghost) != 0) {
        perror("Failed To Create Ghost Thread");
        exit(-1);
    }

    for(int i = 0; i < MAX_HUNTERS; ++i) {
        if (pthread_create(&hunter[i], NULL, &hunterSprite, hunterList[i]) != 0) {
            perror("Failed To Create Hunter Thread");
            exit(-1);
        }
    }

    if (pthread_join(ghost, NULL) != 0) {
        perror("Failed To Join Ghost Thread");
        exit(-1);
    }

    for(int i = 0; i < MAX_HUNTERS; ++i) {
        if (pthread_join(hunter[i], NULL) != 0) {
            perror("Failed To Join Hunter Thread");
            exit(-1);
        }
    }

}

void identifyGhostType(GhostType* ghost, HunterType** hunterList){
    int fingerprintFound = 0;
    int tempFound = 0;
    int soundFound = 0;
    int emfFound = 0;
    for(int i = 0; i < MAX_HUNTERS; ++i){
        EvidenceNodeType* currNode = hunterList[i]->evidenceCollected->head;

        while(currNode != NULL){
            if(isGhostly(currNode->data, currNode->data->evidenceClass) == 0){
                char name[MAX_STR];
                getEvidenceClassName(name, currNode->data->evidenceClass);


                if(strcmp(name, "FINGERPRINTS") == 0){
                    fingerprintFound = 1;
                    break;
                }else if(strcmp(name, "SOUND") == 0){
                    soundFound = 1;
                    break;
                }else if(strcmp(name, "EMF") == 0){
                    emfFound = 1;
                    break;
                }else if(strcmp(name, "TEMPERATURE") == 0){
                    tempFound = 1;
                    break;
                }

            }
            currNode = currNode->next;
        }

    }

    char speculatedType[MAX_STR];
    if(emfFound == 1 && tempFound == 1 && fingerprintFound == 1 && soundFound == 0){
        strcpy(speculatedType,"POLTERGEIST");

    }else if(emfFound == 1 && tempFound == 1 && soundFound == 1 && fingerprintFound == 0){
        strcpy(speculatedType,"BANSHEE");

    }else if(emfFound == 1 && fingerprintFound == 1 && soundFound == 1 && tempFound == 0){
        strcpy(speculatedType,"BULLIES");

    }else if(tempFound == 1 && fingerprintFound == 1 && soundFound == 1 && emfFound == 0){
        strcpy(speculatedType,"PHANTOM");

    }else{
        strcpy(speculatedType, "UNKNOWN");

    }

    printf("%s %s\n", "Speculated Ghost Type:", speculatedType);

    char ghostClassName[MAX_STR];
    getGhostClassName(ghostClassName, ghost->ghostClass);
    printf("%s %s\n", "Ghost Type:", ghostClassName);

    if(strcmp(speculatedType, ghostClassName) == 0){
        printf("%s\n", "Ghost Was Successfully Identified! The Ghost Has Been Captured :)");
    }else{
        printf("%s\n", "The Hunters Mis-Identified The Ghost! The Ghost Now Owns The Building . . .");
    }
}

#pragma clang diagnostic pop