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

int checkActivitySuccess(ASSIGN *assign, CULT *cult){

    if(assign == NULL || assign->activity == NULL || cult == NULL)
        return 0;

    CONDITION *cond = assign->activity->successCondition;

    if(cond == NULL)
        return 1;

    if(getLegitimityOfMembers(assign->group) < cond->minLegitimity)
        return 0;

    if(getVisibilityOfMembers(assign->group) < cond->minVisibility)
        return 0;

    if(getIllegalityOfGroup(assign->group) < cond->minIllegality)
        return 0;

    int members = 0;
    GROUP *g = assign->group;

    while(g){
        members++;
        g = g->next;
    }

    if(members < cond->minMember)
        return 0;

    return 1;
}


void resolveAssign(ASSIGN *assign, CULT *cult){
    if(assign == NULL || cult == NULL)
        return;

    assign->isSuccess = checkActivitySuccess(assign, cult);
}