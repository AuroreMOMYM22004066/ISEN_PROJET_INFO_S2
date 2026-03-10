#include "cult.h"

int isAssignFinished(ASSIGN * assign){
    if (assign == NULL) {
        return FALSE;
    }
    return assign->time >= assign->activity.time;
}

ASSIGN * assignActivityMembers(ACTIVITY *activity, GROUP *group){

}