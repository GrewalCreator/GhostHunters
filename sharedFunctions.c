#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-msc50-cpp"
#include "gameDefs.h"

/*
  Function: memAllocCheck
  Purpose:  prints out the memory allocation error statement
       in:  pointer
  return:   FUNC_SUCCESS for success
*/
int memAllocCheck(void* ptr){
    if(ptr == NULL){
        printf("Memory Allocation Error\n");
        exit(-1);
    }
    return FUNC_SUCCESS;
}

/*
  Function:  randInt
  Purpose:   returns a pseudo randomly generated number,
             in the range min to (max - 1), inclusively
       in:   upper end of the range of the generated number
   return:   randomly generated integer in the range [min, max-1)
*/
int randInt(int min, int max){

    if(min == max) return 0;

    int n = max - min;
    int randomNum;

    do {
        randomNum = rand();
    } while (randomNum >= (RAND_MAX - RAND_MAX % n));

    randomNum %= n;

    return randomNum;
}

/*
  Function:  randDouble
  Purpose:   returns a pseudo randomly generated number,
             in the range min to max, inclusively
       in:   upper end of the range of the generated number
   return:   randomly generated integer in the range [0, max-1)
*/
double randDouble(double min, double max) {
    return (double) (min +  (double) (rand()) /( (double) (RAND_MAX/(max-min))));
}

/*
  Function: makeChoice
  Purpose:  makes the random choice from a list of ints
       in:  options --> list of ints
       in:  size (size of options list)
  return:   the "choice"
*/
int makeChoice(const int* options, int size){
    int chosen = -1;
    while(chosen == -1)
        chosen = options[randInt(0,size)];

    return chosen;
}

/*
  Function: sem_destroy_all
  Purpose:  destroy all room semaphores
       in:  linked list of rooms to destroy semaphores
       out: linked list with no semaphores
    return: void
*/
void sem_destroy_all(RoomLinkedList* list){
    RoomNodeType* currNode = list->head;
    while(currNode != NULL){

        int ret = sem_destroy(&currNode->room->semaphore);
        if (ret != 0) {
            printf("%s\n", "Semaphore Destruction Error");
            exit(-1);
        }

        currNode = currNode->next;
    }
}
#pragma clang diagnostic pop