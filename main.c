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
        printf("%s %d %s\n", "Please Enter Hunter", i ,"'s Names:");
        fgets(hunterName, MAX_STR, stdin);

        hunterName[strlen(hunterName)-1] = '\0';

        HunterType* newHunter;
        initHunter(hunterName, building->rooms.head->room, i, &newHunter);
        building->allHunters[i] = newHunter;
    }


    startGame(building, building->allHunters);

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

#pragma clang diagnostic pop