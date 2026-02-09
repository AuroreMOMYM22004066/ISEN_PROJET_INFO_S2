#include "Cult.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FAUX 0
#define VRAI 1



/*typedef struct {
    char nameCultLeader[NAME_SIZE];
    char nameCult[NAME_SIZE];
    char ideology[IDEOLOGY_SIZE];
    int elapsedTime;
    float funds;
    GROUP * members;
    ASSIGN * assigns;
    ACTIVITY * activities;
} CULT; */

CULT * initCult(void){
    CULT * nouveauCult=NULL;
    nouveauCult=malloc(sizeof(CULT));
    if (nouveauCult==NULL){
        printf("Vide\n");
        return NULL;
    }
    printf("Quel est votre nom Leader Supreme?\n");
    scanf("%s",nouveauCult->nameCultLeader);
    printf("Comment s appelle votre Culte O Grand %s ?\n",nouveauCult->nameCultLeader);
    scanf("%s",nouveauCult->nameCult);
    printf("Qu elle est l ideologie de la Secte %s?\n", nouveauCult->nameCult);
    scanf("%s",nouveauCult->ideology);
    nouveauCult->elapsedTime = 0;
    nouveauCult->funds = 0.0f;
    nouveauCult->members = NULL;
    nouveauCult->assigns = NULL;
    nouveauCult->activities = NULL;

    return nouveauCult;
}
