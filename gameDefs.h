#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define MAX_STR                  64
#define FEAR_RATE                 1
#define MAX_FEAR                 100
#define MAX_HUNTERS               4
#define USLEEP_TIME           500000
#define BOREDOM_MAX               99

#define FUNC_SUCCESS              0
#define MAX_ROOMS                13




typedef enum { EMF, TEMPERATURE, FINGERPRINTS, SOUND } EvidenceClassType;
typedef enum { POLTERGEIST, BANSHEE, BULLIES, PHANTOM } GhostClassType;




typedef struct Ghost{
    GhostClassType ghostClass;
    struct Room* currRoom;
    int boredomTimer;
    char name[MAX_STR];
}GhostType;

typedef struct LinkedListE{
    struct EvidenceNode* head;
    struct EvidenceNode* tail;
    int size;
}EvidenceLinkedList;

typedef struct Room{
    char name[MAX_STR];
    struct RoomList* adjacentRooms;
    EvidenceLinkedList* evidence;
    struct Hunter* huntersInRoom[MAX_HUNTERS];
    int amountHuntersInRoom;
    GhostType* ghost;
    sem_t semaphore;
}RoomType;

typedef struct RoomNode {
    RoomType *room;
    struct RoomNode* next;
    struct RoomNode* prev;
} RoomNodeType;

typedef struct RoomList{
    RoomNodeType* head;
    RoomNodeType* tail;
    int size;
}RoomLinkedList;


typedef struct Building{
    RoomLinkedList rooms;
    GhostType* ghost;
    struct Hunter* allHunters[MAX_HUNTERS];
}BuildingType;

typedef struct Evidence{
    EvidenceClassType evidenceClass;
    double data;
}EvidenceType;

typedef struct EvidenceNode{
    EvidenceType* data;
    struct EvidenceNode* next;
    struct EvidenceNode* prev;
}EvidenceNodeType;

typedef struct Hunter{
    char name[MAX_STR];
    RoomType *currRoom;
    EvidenceClassType readableEvidence;
    int fear;
    EvidenceLinkedList *evidenceCollected;
    int boredomTimer;
    int ghostlyEvidenceCollected;

}HunterType;


// Main Functions
void startGame(BuildingType*, HunterType**);
void identifyGhostType(GhostType*, HunterType**);

// Ghost Functions
int initGhost(char*, GhostClassType, RoomType*, GhostType**);
void* ghostSprite(void*);
void printGhostType(GhostType*);
int isGhostly(EvidenceType*, EvidenceClassType);
void getGhostClassName(char*, GhostClassType);

// Room Functions
int initRoom(RoomType**, char*);
void initRoomList(RoomLinkedList*);
void appendRoom(RoomLinkedList*, RoomType*);
void connectRooms(RoomType*, RoomType*);
int initRoomNode(RoomNodeType**, RoomType*);
void getRandomRoom(RoomLinkedList*, RoomNodeType**);
void removeHunterFromRoom(HunterType*, RoomType*);

// Building Functions
void initBuilding(BuildingType**);
void populateRooms(BuildingType*);

// Hunter Functions
int initHunter(char*, RoomType*, EvidenceClassType, HunterType**);
void* hunterSprite(void*);
void moveHuntersRoom(HunterType*, RoomNodeType*);
void printHunterType(HunterType*);

// Evidence Functions
void initEvidenceList(EvidenceLinkedList*);
int initEvidence(EvidenceType**, double, int);
void appendEvidence(EvidenceLinkedList*, EvidenceNodeType*);
int initEvidenceNode(EvidenceNodeType**, EvidenceType*);
void removeEvidenceNode(EvidenceLinkedList*, EvidenceNodeType*);
void generateStdEvidence(EvidenceClassType, double*);
void generateGhostlyEvidence(EvidenceClassType, double*);
void getRandomEvidence(EvidenceLinkedList*, EvidenceType**);
void findRoomEvidence(EvidenceLinkedList*, EvidenceClassType, EvidenceNodeType**);
void getEvidenceClassName(char*, EvidenceClassType);
void dropEvidence(GhostType*);
void pickUpEvidence(HunterType*);


// Helper Functions
int memAllocCheck(void*);
int makeChoice(const int*, int);
void sem_destroy_all(RoomLinkedList*);
int randInt(int, int);
double randDouble(double, double);


// Clean
void cleanBuilding(BuildingType*);
void cleanEvidenceListData(EvidenceLinkedList*);
void cleanEvidenceListNodes(EvidenceLinkedList*);
void cleanRoomListData(RoomLinkedList*);
void cleanRoomListNodes(RoomLinkedList*);
