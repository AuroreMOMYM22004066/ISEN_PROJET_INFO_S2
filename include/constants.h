#ifndef CONSTANTS_H
#define CONSTANTS_H

#define TRUE 1
#define FALSE 0
#define NAME_SIZE 20
#define TYPE_SIZE 20
#define DESC_MEMBER_SIZE 50   
#define DESC_ACTIVITY_SIZE 200
#define DESC_EVENT_SIZE 200
#define MEMBER_TYPE 20
#define IDEOLOGY_SIZE 200

#define MAX_LEGITIMITY 10
#define NB_PA 3 // nb de point d'action par tour
#define NB_ROUNDS_FAST_GAME 10
#define MAX_CONTROL 1

#define NB_MAX_SPAWN 5

#define PATH_SAVE "save.txt"
#define PATH_ACTIVITIES "assets/config/activities.cfg"
#define PATH_EVENTS "assets/config/events.cfg"
#define PATH_TYPE "assets/config/types.cfg"

// serve a rien on utilise celui dans type.cfg
#define PATH_ELITES_PROFIL "assets/config/profil_members/elites.cfg"
#define PATH_MARGINAUX_PROFIL "assets/config/profil_members/marginaux.cfg"
#define PATH_ORDINARY_PROFIL "assets/config/profil_members/ordinary.cfg"


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>

#endif