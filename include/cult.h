#ifndef CULT_H
#define CULT_H 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <structs.h>

void menu();
void menuGame();
void menuLoad();
ACTIVITY* menuActivitity(ACTIVITY * activity); // choisir une activité prend en param la liste d'activité return null si aucune choisis

float getLegitimity(CULT *culte);
float getLegitimityOfMembers(GROUP * members);
float getLegitimityOfAssigns(ASSIGN * activities);
float getVisibility(CULT *culte);
float getVisibilityOfMembers(GROUP * members);
float getVisibilityOfAssigns(ASSIGN * activities);
float getControl(CULT *culte);

CULT * initCult();
GAME_CONF * initConf(); //charge la conf 
void playGame(CULT * cult, GAME_CONF * conf);
void playFastGame(CULT * Cult, GAME_CONF * conf, int nbRounds); //mettre genre 10 tours pour une partie rapide 

void showGroup();
GROUP * addMemberToGroup(GROUP * group, MEMBER * member); // le faire d'abord en fonction du type puis en fonction du nom
GROUP * fireMemberFromGroup(GROUP * group, MEMBER * member);
MEMBER * searchMemberFromGroup(GROUP * group, MEMBER * member);

ASSIGN * initAssign();
ASSIGN * assignActivity(ACTIVITY *activity, GROUP *group);
void showOneAssign(ASSIGN * assign);
void showAssigns();

ACTIVITY * addActivity(ACTIVITY * activity);
ACTIVITY * removeActivity(ACTIVITY * activity);
void showActivity(ACTIVITY * activity);
void showActivities();

int getNbMemberFromGroup(GROUP * group);
int getnbMemberTypeFromGroup(GROUP * group, char type[TYPE_SIZE]); // changer le type pour un int

/**
 * On génère une liste de membre que l'on pourrait recruter et le joueur en choisis un 
 * attention penser au pourcentage d'echec 
 * en faire une activité 
*/ 
MEMBER HireMember(CULT * cult, GAME_CONF * conf); 



#endif

/*
pour eviter les problème de remplissage dans les fichiers de config faire un fichier par type 

et un fichier contenant les types et les path des fichiers de profils 
avec le pourcentage d'apparition de chaque type  

typelib = "membres_marginaux";
apparition = 0,80;
typeid = 0;
path = "/membres_marginaux.txt"
legitimity = 0;
visibility = 2;
illegality = 10;
fund = 0;

typelib = "membres_ordinaires";
apparition = 0,50;
typeid = 1;
path = "/membres_ordinaires.txt"
legitimity = 5;
visibility = 5;
illegality = 0;
fund = 5;

typelib = "membres_elites";
apparition = 0,10;
typeid = 2;
path = "/membres_elites.txt"
legitimity = 10;
visibility = 10;
illegality = -5;
fund = 20;


fichier membre_marginaux.txt
membrename = "Chales";
membredescription = "sdf";

membrename = "Dimitri";
membredescription = "gangster";


EVENT policeRaid = {
    .name = "Descente de police",
    .description = "Les autorités enquêtent sur la secte",

    .minLegitimity = 0.0,
    .maxLegitimity = 4.0,
    .minVisibility = 6.0,
    .maxVisibility = 10.0,
    .minIllegality = 6.0,
    .maxIllegality = 10.0,


    .impact = { -1.0, -2.0, 0.0, -3.0 },
    .fundDelta = -200,

    .duration = 3,
    .cooldown = 5,
};



loadTypes("types.cfg");

for each type:
    members = loadMembers(type.file);
    for each member:
        member.impact = type.impact;

*/