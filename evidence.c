#include "gameDefs.h"

/*
  Function: initEvidence
  Purpose:  function that initializes fields of the given type to value in parameters or default values.
       in:  location of the old evidence to be initialized
       in:  the value of the data
       in: the class type of evidence
       out: location of the new  evidence that is initialized
  return:   FUNC_SUCCESS if success, MEMORY_ALLOCATION_ERROR for memory allocating failing
*/
int initEvidence(EvidenceType** evidence, double dataValue, int classType){
    EvidenceType* newEvidence = malloc(sizeof(EvidenceType));
    memAllocCheck(newEvidence);

    newEvidence->evidenceClass = classType;
    newEvidence->data = dataValue;

    *evidence = newEvidence;

    return FUNC_SUCCESS;
}
/*
  Function: initEvidenceList
  Purpose:  function that initializes the three fields of the given list parameter to default values.
       in:  location of the old evidence list to be initialized
       out: location of the new  evidence list that is initialized
  return:  void
*/
void initEvidenceList(EvidenceLinkedList* list){
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}


/*
  Function: appendEvidence
  Purpose: adds the evidence type (evidenceNode) to the back of the evidence linked list in parameter list.
       in:  location of the old evidence list to add evidenceNode in
       out: location of the new room array that has evidenceNode added
       in:  evidenceNode is evidence type we want to add
  return:   void
*/
void appendEvidence(EvidenceLinkedList* list, EvidenceNodeType* evidenceNode){
    evidenceNode->next = NULL;
    evidenceNode->prev = NULL;
    if(list->head == NULL){
        list->head = evidenceNode;
        list->tail = evidenceNode;

    }else {
        list->tail->next=evidenceNode;
        evidenceNode->prev = list->tail;
        list->tail = evidenceNode;
    }
    list->size++;
}

/*
  Function: initEvidenceNode
  Purpose:  function that initializes fields of the given type to default values.
       in:  location of the old node to be initialized
       in:  evidenceType which holds the data of the EvidenceNode
       out: location of the new  evidence node that is initialized
  return:   FUNC_SUCCESS if success, MEMORY_ALLOCATION_ERROR for memory allocating failing
*/
int initEvidenceNode(EvidenceNodeType** node, EvidenceType* evidence){
    EvidenceNodeType* evidenceNode = malloc(sizeof(EvidenceNodeType));
    memAllocCheck(evidenceNode);

    evidenceNode->data = evidence;
    evidenceNode->next = NULL;
    evidenceNode->prev = NULL;

    *node = evidenceNode;

    return FUNC_SUCCESS;
}

/*
  Function: cleanEvidenceListData
  Purpose:  function that traverses the given list and deallocates its data only
       in:  location of the evidence list to clean its data only
  return:   void
*/
void cleanEvidenceListData(EvidenceLinkedList* list) {
//Free Hunter Evidence Data

    EvidenceNodeType *currNode = list->head;
    while (currNode != NULL) {
        free(currNode->data);
        currNode = currNode->next;
    }

}

/*
  Function: cleanEvidenceList
  Purpose:   function that traverses the given list and deallocates its nodes only.
       in:  location of the evidence list to clean its nodes only
  return:   void
*/
void cleanEvidenceListNodes(EvidenceLinkedList *list) {
    //Free Hunter Evidence Nodes

    EvidenceNodeType *currNode = list->head;
    EvidenceNodeType *nextNode = NULL;
    while (currNode != NULL) {
        nextNode = currNode->next;
        free(currNode);
        currNode = nextNode;
    }

}


void dropEvidence(GhostType* ghost){
    int evidenceOptions[3] = {-1, -1, -1};
    GhostClassType ghostClass = ghost->ghostClass;
    if (ghostClass == POLTERGEIST) {

        evidenceOptions[0] = EMF;
        evidenceOptions[1] = TEMPERATURE;
        evidenceOptions[2] = FINGERPRINTS;

    } else if (ghostClass == BANSHEE) {

        evidenceOptions[0] = EMF;
        evidenceOptions[1] = TEMPERATURE;
        evidenceOptions[2] = SOUND;

    } else if (ghostClass == BULLIES) {

        evidenceOptions[0] = EMF;
        evidenceOptions[1] = FINGERPRINTS;
        evidenceOptions[2] = SOUND;

    } else if (ghostClass == PHANTOM) {

        evidenceOptions[0] = TEMPERATURE;
        evidenceOptions[1] = FINGERPRINTS;
        evidenceOptions[2] = SOUND;

    }

    EvidenceClassType evidenceClass = makeChoice(evidenceOptions, 3);
    int isGhostly = randInt(0,2);

    double evidenceData;
    if(isGhostly == 0){
        generateGhostlyEvidence(evidenceClass, &evidenceData);
    }else{
        generateStdEvidence(evidenceClass, &evidenceData);
    }

    EvidenceType* newEvidence;
    EvidenceNodeType* newEvidenceNode;

    initEvidence(&newEvidence, evidenceData, evidenceClass);
    initEvidenceNode(&newEvidenceNode, newEvidence);
    appendEvidence(ghost->currRoom->evidence, newEvidenceNode);
}

/*
  Function: generateStdEvidence
  Purpose: generates a random ghostly evidence
       in:  the evidence class type used (since each class has a different range)
  return:   the data value
*/
void generateGhostlyEvidence(EvidenceClassType class, double* dataVal){
    switch(class){
        case EMF:
            *dataVal = randDouble(4.70, 5.00);
            break;
        case TEMPERATURE:
            *dataVal = randDouble(-10.00, 1.00);
            break;
        case FINGERPRINTS:
            *dataVal = 1.00;
            break;
        case SOUND:
            *dataVal = randDouble(65.00, 75.00);
            break;
        default:
            break;
    }
}

/*
  Function: generateStdEvidence
  Purpose: generates a random standard evidence
       in:  the evidence class type used (since each class has a different range)
  return:   the data value
*/
void generateStdEvidence(EvidenceClassType class, double* dataVal){

    switch(class){
        case EMF:
            *dataVal = randDouble(0,4.90);
            break;
        case TEMPERATURE:
            *dataVal = randDouble(0, 27);
            break;
        case FINGERPRINTS:
            *dataVal = 0.00f;
            break;
        case SOUND:
            *dataVal = randDouble(40, 70);
            break;
        default:
            break;
    }

}

/*
  Function: getEvidenceClassName
  Purpose:  finds the evidence class type based on the evidence type
      out:  the evidence type as a string
       in:  the evidence class
  return:   void
*/
void getEvidenceClassName(char* evidenceType, EvidenceClassType class){

    switch (class) {
        case EMF:
            strcpy(evidenceType, "EMF");
            break;
        case TEMPERATURE:
            strcpy(evidenceType, "TEMPERATURE");
            break;
        case FINGERPRINTS:
            strcpy(evidenceType, "FINGERPRINTS");
            break;
        case SOUND:
            strcpy(evidenceType, "SOUND");
            break;
        default:
            break;
    }

}


void pickUpEvidence(HunterType* hunter){
    EvidenceNodeType* newEvidenceNode = NULL;

    // If Room Contains No Evidence, Collect Standard Evidence
    if(hunter->currRoom->evidence->size == 0){
        double standardEvidence;

        generateStdEvidence(hunter->readableEvidence, &standardEvidence);

        EvidenceType* newEvidence;


        initEvidence(&newEvidence, standardEvidence, hunter->readableEvidence);
        initEvidenceNode(&newEvidenceNode, newEvidence);

        appendEvidence(hunter->evidenceCollected, newEvidenceNode);
    }else{
        findRoomEvidence(hunter->currRoom->evidence, hunter->readableEvidence, &newEvidenceNode);
        if(newEvidenceNode != NULL){
            removeEvidenceNode(hunter->currRoom->evidence, newEvidenceNode);
            appendEvidence(hunter->evidenceCollected, newEvidenceNode);

            if(isGhostly(newEvidenceNode->data, hunter->readableEvidence) == 0){
                hunter->boredomTimer = BOREDOM_MAX;
                hunter->ghostlyEvidenceCollected++;
            }
        }

    }


}

void findRoomEvidence(EvidenceLinkedList* list, EvidenceClassType evidenceClass, EvidenceNodeType** node){
    EvidenceNodeType* currNode = list->head;

    while(currNode != NULL){
        if(currNode->data->evidenceClass == evidenceClass){
            *node = currNode;
            break;
        }
        currNode = currNode->next;
    }
}

/*
  Function: removeEvidenceNode
  Purpose:  removes an evidence node from an evidence list
       in:  location of the list to remove the evidence node from
       out: location of the list with removed evidence node
       in:  location of the evidence node to be removed
  return:   void
*/
void removeEvidenceNode(EvidenceLinkedList* list, EvidenceNodeType* node){
    if(node != NULL){
        EvidenceNodeType* prev = node->prev;
        EvidenceNodeType* next = node->next;

        if(prev == NULL)
            list->head = next;
        else
            prev->next = next;

        if(next == NULL)
            list->tail = prev;
        else
            next->prev = prev;
    }
}


/*
  Function: getRandomEvidence
  Purpose: picks a random value from 0 to the end of the list, traverses the list that many times
            then gets the node in that position. (for choice to be random)
       in: location of the linked list to traverse
  return:   returns the random node
*/
void getRandomEvidence(EvidenceLinkedList* list, EvidenceType** evidence){
    EvidenceNodeType* currNode = list->head;
    int randomVal = randInt(1, list->size-1);
    for(int i = 0; i < randomVal; ++i){
        if(currNode->next != NULL)
            currNode = currNode->next;
    }

    *evidence = currNode->data;
}