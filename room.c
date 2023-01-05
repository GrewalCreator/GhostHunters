#include "gameDefs.h"

/*
  Function: initRoomList
  Purpose:  initializes the fields of the given RoomListType structure
       in:  location of the old room array to be initialized
       out: location of the new room array that is initialized
  return:   void
*/
void initRoomList(RoomLinkedList* list){
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}


#pragma clang diagnostic push
#pragma ide diagnostic ignored "ConstantFunctionResult"
/*
  Function: initRoom
  Purpose:  initializes the fields of the given roomType structure
       in:  location of the old room to be initialized
       out: location of the new room that is initialized
       in:  name of the room
  return:   FUNC_SUCCESS if successful
*/
int initRoom(RoomType** room, char* name){

    // Allocate Memory For Room
    RoomType* newRoom = malloc(sizeof(RoomType));
    memAllocCheck(newRoom);

    //Set Room Name
    strcpy(newRoom->name, name);
    newRoom->amountHuntersInRoom = 0;

    //Set LinkList Of Connected Rooms
    RoomLinkedList* connectedRooms = malloc(sizeof(RoomLinkedList));
    memAllocCheck(connectedRooms);

    initRoomList(connectedRooms);
    newRoom->adjacentRooms = connectedRooms;

    //Set LinkList of Evidence

    EvidenceLinkedList* evidenceInRoom = malloc(sizeof(EvidenceLinkedList));
    memAllocCheck(evidenceInRoom);

    initEvidenceList(evidenceInRoom);
    newRoom->evidence = evidenceInRoom;

    //Clear Garbage Data From Array
    for(int i = 0; i < MAX_HUNTERS; ++i){
        newRoom->huntersInRoom[i] = NULL;
    }

    newRoom->ghost = NULL;

    sem_init(&newRoom->semaphore, 0, 1);
    *room = newRoom;



    return FUNC_SUCCESS;
}
#pragma clang diagnostic pop


/*
  Function: initRoomNode
  Purpose:  initializes the fields of the given roomNode structure
       in:  location of the old room node to be initialized
       out: location of the new room node that is initialized
       in:  the room type (so that it's data can be accessed)
  return:   FUNC_SUCCESS if success, MEMORY_ALLOCATION_ERROR for memory allocating failing
*/
int initRoomNode(RoomNodeType** node, RoomType* room){
    RoomNodeType* roomNode = malloc(sizeof(RoomNodeType));
    memAllocCheck(roomNode);

    roomNode->room = room;
    roomNode->next = NULL;
    roomNode->prev = NULL;

    *node = roomNode;

    return FUNC_SUCCESS;
}

/*
  Function: appendRoom
  Purpose:  function that adds the room node directly at the back (i.e. the end) of the given list.
       in:  location of the room list to add onto
       out: location of the room list with room node added.
       in:  room being added at end of list.
  return:   void
*/
void appendRoom(RoomLinkedList* list, RoomType* room){
    RoomNodeType* newNode;
    initRoomNode(&newNode, room);

    if(list->head == NULL){
        list->head = newNode;
        list->tail = newNode;

    }else {
        list->tail->next=newNode;
        newNode->prev = list->tail;
        list->tail = newNode;
    }
    list->size++;
}
/*
  Function: connectRooms
  Purpose: connect the two different room types given in the parameter and connect them together (adjacent rooms)
       in: location of roomA
       in: location of roomB
       out: location of roomA after adjacent rooms list was modified to store roomB
       out: location of roomB after adjacent rooms list was modified to store roomA
  return:   returns the random node
*/
void connectRooms(RoomType* roomA, RoomType* roomB){
    appendRoom(roomB->adjacentRooms, roomA);
    appendRoom(roomA->adjacentRooms, roomB);

}


/*
  Function: cleanRoomListData
  Purpose:  function that traverses the given list and deallocates its data only
       in:  location of the room list to clean its data only
  return:   void
*/
void cleanRoomListData(RoomLinkedList* list){
    RoomNodeType* currNode = list->head;
    while(currNode != NULL){
        cleanEvidenceListData(currNode->room->evidence);
        cleanEvidenceListNodes(currNode->room->evidence);
        free(currNode->room->evidence);

        cleanRoomListNodes(currNode->room->adjacentRooms);
        free(currNode->room->adjacentRooms);
        free(currNode->room);

        currNode = currNode->next;
    }

}

/*
  Function: cleanRoomList
  Purpose:   function that traverses the given list and deallocates its nodes only.
       in:  location of the room list to clean its nodes only
  return:   void
*/
void cleanRoomListNodes(RoomLinkedList* list){
    RoomNodeType* currNode = list->head;
    RoomNodeType* nextNode = NULL;
    while(currNode != NULL){
        nextNode = currNode->next;
        free(currNode);
        currNode = nextNode;
    }
}


/*
  Function: getRandomRoom
  Purpose:  traverse through the room list a given amount of times to select a random room
       in:  location of the room list to traverse
       in:  location of room node
       out: location of room node that is picked
  return:   void
*/
void getRandomRoom(RoomLinkedList* roomList, RoomNodeType** room){
    RoomNodeType* currNode = roomList->head;
    int randomVal = randInt(1, roomList->size-1);
    for(int i = 0; i < randomVal; ++i){
        if(currNode->next != NULL)
            currNode = currNode->next;
    }

    *room = currNode;
}
