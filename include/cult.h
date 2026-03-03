#ifndef CULT_H
#define CULT_H 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <structs.h>


/**
 * init.c
 * va appeler les methode init des autres 
 */
void trim(char *str); 
CULT * initCult();
GAME_CONF * initConf(); //charge la conf 

/**
 * cult.c
 */
void menu();
void menuGame();
void menuLoad();
ACTIVITY* menuActivitity(ACTIVITY * activity); // choisir une activité prend en param la liste d'activité return null si aucune choisis

void playGame(CULT * cult, GAME_CONF * conf);
void playFastGame(CULT * Cult, GAME_CONF * conf, int nbRounds); //mettre genre 10 tours pour une partie rapide 

void showCulte(CULT * cult);
/**
 * On génère une liste de membre que l'on pourrait recruter et le joueur en choisis un 
 * attention penser au pourcentage d'echec 
 * en faire une activité 
 * 
 * certaines activité vont etre faite directement par le gourou 
 * ce sont les activitées de base exemple ces dernière ne dependent pas des .cfg exemple:
 * recruter un membre 
 * assigner des membres a une activité (c'est a ça que sert la liste all activities)
*/ 
MEMBER HireMember(CULT * cult, GAME_CONF * conf); 


float getLegitimity(CULT *culte);
float getLegitimityOfMembers(GROUP * members);
float getLegitimityOfAssigns(ASSIGN * activities);
float getVisibility(CULT *culte);
float getVisibilityOfMembers(GROUP * members);
float getVisibilityOfAssigns(ASSIGN * activities);
float getControl(CULT *culte);

/**
 * activity.c
 */
ACTIVITY * initActivities(void);
ACTIVITY * addActivity(ACTIVITY * activity);
ACTIVITY * removeActivity(ACTIVITY * activity);
void showActivity(ACTIVITY * activity);
void showActivities(ACTIVITY * activity);

/**
 * event.c
 */
EVENT * initEvents(void);
void showOneEvent(EVENT * event);
void showEvents(EVENT * event);

EVENT * getEvent(CULT * cult, GAME_CONF * conf); // regarde tout les éléments présent dans la list event de conf et en tire un parmis tous ceux elligible 

/**
 * member.c
 */
GROUP * initPeoplesProfils(void);
GROUP * addHeadGroup(GROUP *head, GROUP *newGroup);
GROUP * addEndGroup(GROUP *head, GROUP *newGroup);
GROUP * insertOrderGroup(GROUP *head, GROUP *newGroup);
GROUP * createGroupNode(MEMBER *member);
GROUP * addMemberToGroup(GROUP * group, MEMBER * member); // le faire d'abord en fonction du type puis en fonction du nom
GROUP * fireMemberFromGroup(GROUP * group, MEMBER * member);
MEMBER * searchMemberFromGroup(GROUP * group, MEMBER * member);
int getNbMemberFromGroup(GROUP * group);
int getnbMemberTypeFromGroup(GROUP * group, char type[TYPE_SIZE]); // changer le type pour un int
int isEmptyGroup(GROUP *head);
void showGroup(GROUP * group);
void showMember(MEMBER * member);

/**
 * assign.c
 */
ASSIGN * assignActivity(ACTIVITY *activity, GROUP *group);
void showOneAssign(ASSIGN * assign);
void showAssigns();




// pas utile au jeu mais peut l'etre pour debug
void showCondition(CONDITION * cond);
void showImpact(IMPACT * impact);



void applyImpact();
//void assignIsSuccess

#endif
