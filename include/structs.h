#ifndef STRUCTS_H
#define STRUCTS_H

#include "constants.h"


typedef struct {
    float minLegitimity;
    float minVisibility;
    float minIllegality;
    int minMember;
    int maxMember;
} CONDITION;

typedef struct {
    float legitimity;
    float visibility;
    float illegality;
    float fund;
    float control; //impact sur le control d'un membre sert au calcul d'évolution des stats 
} IMPACT;

typedef struct {
    char name[NAME_SIZE];
    char description[DESC_EVENT_SIZE];
    CONDITION * sponCondition;
    CONDITION * successCondition;
    IMPACT * impact; 
} EVENT;


/**
 * Type de membre ?
 * faire une liste des profils charger depuis un fichiers de config
 * on utilise une instanciation de type pour tout les membres du meme type
 */
typedef struct member {
    char name[NAME_SIZE];
    char description[DESC_MEMBER_SIZE];
    IMPACT impact; 
    float control; // a quel point il est sous l'emprise 
    // ATTENTION member.control != member.impact.control
    // member.control = control exerce sur le membres de la secte
    // member.impact.control = control exerce par le membre pourra être utile pour ds fonctionnalites plus complexes 
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
    CONDITION * sponCondition;
    CONDITION * successCondition;
    int time;
    int PA;// point d'action qu'elle coute
    float cost; //couts
    struct activity *next;
    //struct activity *previous;
    //pas necessaire de la double chaine car la liste n'évoluera pas dans le temps 
    //elle dépent juste des .conf
} ACTIVITY;

typedef struct assign {
    ACTIVITY activity;
    GROUP* group;
    int time; //temps ecoule depuis le debut de l'activite != activity.time qui est le temps que prend un activité pour se réaliser
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
} CULT;

typedef struct {
    ACTIVITY *allActivities;
    EVENT *allEvents;
    GROUP *allPeoples; //ensemble des profils des personnes que l'on peut recruter
} GAME_CONF;


#endif