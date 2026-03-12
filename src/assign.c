#include "cult.h"

int isAssignFinished(ASSIGN * assign){
    if (assign == NULL || assign->activity == NULL) {
        return FALSE;
    }
    return assign->time >= assign->activity->time;
}

ASSIGN * assignActivityMembers(ACTIVITY *activity, GROUP *group){
    if(activity == NULL || group == NULL){
        return NULL;
    }

    ASSIGN *assign = malloc(sizeof(ASSIGN));
    assign->activity = activity;
    assign->group = group;
    assign->isSuccess = 1;
    assign->time = 0;
    assign->next = NULL;

    return assign;
}

ASSIGN * addAssign(CULT *cult, ASSIGN *assign){
    if(cult == NULL || assign == NULL){
        return NULL;
    }
    
    if(cult->assigns == NULL){
        cult->assigns = assign;
    } else {
        ASSIGN *current = cult->assigns; 
        while(current->next != NULL){
            current = current->next;
        }
        current->next = assign;
    }

    return cult->assigns;
}