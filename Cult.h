#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

#define PATH_SAVE "save.txt"


typedef struct {
    float minLegitimity;
    float minVisibility;
    float minIllegality;
    int minMember;
    int maxMember;
} CONDITION;

typedef struct {
    char name[NAME_SIZE];
    char description[DESC_EVENT_SIZE];
    CONDITION * condition;
    IMPACT * impact;
} EVENT;

typedef struct {
    float legitimity;
    float visibility;
    float illegality;
    float fund;
} IMPACT;
// Type de membre
// faire une liste des profils charger depuis un fichiers de config
// on utilise une instanciation de type pour tout les membres du meme type



typedef struct member {
    char name[NAME_SIZE];
    char description[DESC_MEMBER_SIZE];
    char type[TYPE_SIZE];
    IMPACT impact;
    float control; // a quel point il est sous l'emprise 
    //struct member next;
    //struct member previous;
} MEMBER;

typedef struct group {
    MEMBER * member;
    struct group *next;
    struct group *previous;
} GROUP;

typedef struct activity {
    char name[NAME_SIZE];
    char description[DESC_ACTIVITY_SIZE];
    IMPACT impactSuccess;
    IMPACT impactFailed;
    CONDITION * condition;
    int time;
    int PA;// point d'action 
    float cost;
    struct activity *next;
    //struct activity *previous;
} ACTIVITY;

typedef struct assign {
    ACTIVITY activity;
    GROUP* group;
    int time;
    int isSuccess; // TRUE OR FALSE
    struct assign *next;
    struct assign *previous;
} ASSIGN;

typedef struct {
    char nameCultLeader[NAME_SIZE];
    char ideology[IDEOLOGY_SIZE];
    int elapsedTime; 
    float funds;
    GROUP * members;
    ASSIGN * assigns;
    ACTIVITY * activities;
} CULT;


void menu();
void menuGame();
void menuLoad();
void menuAdmin(); // pas prioritaire 
void menuActivitity(ACTIVITY * activity); // choisir une activité prend en param la liste d'activité

float getLegitimity(CULT *culte);
float getLegitimityOfMembers(GROUP * members);
float getLegitimityOfAssigns(ASSIGN * activities);
float getVisibility(CULT *culte);
float getVisibilityOfMembers(GROUP * members);
float getVisibilityOfAssigns(ASSIGN * activities);
float getControl(CULT *culte);

CULT * initCult();
void playGame(CULT * cult);
void playFastGame(CULT * Cult, int nbRounds); //mettre genre 10 tours pour une partie rapide 

void showGroup();
GROUP * addMemberToGroup(GROUP * group, MEMBER * member); // le faire d'abord en fonction du type puis en fonction du nom
GROUP * fireMemberFromGroup(GROUP * group, MEMBER * member);
MEMBER * searchMemberFromGroup(GROUP * group, MEMBER * member);

ASSIGN * initAssign();
ASSIGN * assignActivity(ACTIVITY *activity, GROUP *group);
void showAssign();

ACTIVITY * addActivity(ACTIVITY * activity);
ACTIVITY * removeActivity(ACTIVITY * activity);
void showActivities();

void menu(){
    int exit = FALSE;
    char choice;
    while (!exit)
    {
        printf("\n=== MENU PRINCIPAL ===\n");
        printf("0 - Quitter\n");
        printf("1 - Nouvelle partie\n");
        printf("2 - Reprendre une partie\n");
        printf("3 - Mode admin\n");
        printf("> ");

        scanf(" %c", &choice);
        switch(choice){
            case '0':
                exit = TRUE;
                break;
            case '1':
                menuGame();
                break;
            case '2':
                menuLoad();
                break;
            case '3':
                menuAdmin();
                break;
            default :
                printf("Choix non pris en compte\n");
        }
    }
    
}

void menuGame(){
    int exit = FALSE;
    char choice;
    while (!exit)
    {
        printf("\n=== MENU JEU ===\n");
        printf("0 - Retour au menu principal\n");
        printf("1 - Partie rapide\n");
        printf("2 - Partie classique\n");
        printf("> ");

        scanf(" %c", &choice);
        switch(choice){
            case '0':
                menu();
                break;
            case '1':
                CULT * cult = initCult();
                playFastGame(cult, NB_ROUNDS_FAST_GAME);
                break;
            case '2':
                CULT * cult = initCult();
                playGame(cult);
                break;
            default :
                printf("Choix non pris en compte\n");
        }
    }
    
}

void menuLoad(){

}