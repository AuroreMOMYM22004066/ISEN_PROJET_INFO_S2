#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cult.h"

/* -------------------------------------------------- */
/* Supprime espaces début + fin                      */
/* -------------------------------------------------- */
void trim(char *str)
{
    char *end;

    // Trim début
    while (*str == ' ' || *str == '\t')
        str++;

    // Si chaîne vide
    if (*str == 0)
        return;

    // Trim fin
    end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\t'))
        end--;

    *(end + 1) = '\0';
}

CULT * initCult(void){
    CULT * nouveauCult=NULL;
    nouveauCult=malloc(sizeof(CULT));
    if (nouveauCult==NULL){
        printf("Vide\n");
        return NULL;
    }
    printf("\nQuel est votre nom Leader Supreme?\n> ");
    scanf("%s",nouveauCult->nameCultLeader);
    printf("\nComment s appelle votre Culte O Grand %s ?\n> ",nouveauCult->nameCultLeader);
    scanf("%s",nouveauCult->nameCult);
    printf("\nQu elle est l ideologie de la Secte %s?\n> ", nouveauCult->nameCult);
    scanf("%s",nouveauCult->ideology);

    nouveauCult->elapsedTime = 1;
    nouveauCult->pa = NB_PA;
    nouveauCult->funds = 5.0f;
    nouveauCult->members = NULL;
    nouveauCult->assigns = NULL;
    return nouveauCult;
}



CULT *initCultUI(){

    CULT *cult = malloc(sizeof(CULT));
    if(!cult) return NULL;

    inputPopup("Nom du Leader Supreme ?", cult->nameCultLeader, NAME_SIZE);

    char buffer[100];
    snprintf(buffer,sizeof(buffer),
        "Nom du Culte de %s ?",cult->nameCultLeader);

    inputPopup(buffer, cult->nameCult, NAME_SIZE);

    snprintf(buffer,sizeof(buffer),
        "Ideologie du culte %s ?",cult->nameCult);

    inputPopup(buffer, cult->ideology, NAME_SIZE);

    cult->elapsedTime = 1;
    cult->pa = NB_PA;
    cult->funds = 5.0f;
    cult->members = NULL;
    cult->assigns = NULL;

    return cult;
}


GAME_CONF * initConf(){
    GAME_CONF * conf = NULL;
    conf = malloc(sizeof(GAME_CONF));

    if(conf == NULL) return NULL;

    ACTIVITY_NODE * activities = initActivities();
    //showActivities(activities);
    EVENT * events = initEvents();
    //showEvents(events);
    GROUP * profils = initPeoplesProfils();
    //showGroup(profils);

    conf->allActivities = activities;
    conf->allEvents = events;
    conf->allPeoples = profils;

    return conf;
}
