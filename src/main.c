#include <stdio.h>
#include "cult.h"

int main(){
    //printf("Hello");
    //menu();
    ACTIVITY * allActivities = initActivities();
    showActivities(allActivities);
    EVENT * allevents = initEvents();
    showEvents(allevents);
    GROUP * group = initPeoplesProfils();
    showGroup(group);
    ACTIVITY * act  = menuActivitity(allActivities);

    showActivity(act);

    initCult();
    return 1;
}