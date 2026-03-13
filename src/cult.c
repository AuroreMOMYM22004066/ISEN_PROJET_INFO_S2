#include "cult.h" 


void menu(){
    int exit = FALSE;
    char choice;
    while (!exit)
    {
        printf("\n=== MENU PRINCIPAL ===\n");
        printf("0 - Quitter\n");
        printf("1 - Nouvelle partie\n");
        printf("2 - Reprendre une partie\n");
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
                exit = TRUE;
                menu();
                break;
            case '1': {
                CULT * cult = initCult();
                GAME_CONF * conf = initConf();
                playFastGame(cult, conf, NB_ROUNDS_FAST_GAME);
                break;
            }
            case '2':{
                CULT * cult = initCult();
                GAME_CONF * conf = initConf();
                playGame(cult, conf);
                break;
            }
            default :
                printf("Choix non pris en compte\n");
        }
    }
    
}

void menuLoad(){

}


void playGame(CULT * cult, GAME_CONF * conf){
    printf("START - playGame\n");
}

void playFastGame(CULT * Cult, GAME_CONF * conf, int nbRounds){
    printf("START - playFastGame\n");
}


ACTIVITY_NODE* menuActivitity(ACTIVITY_NODE* activity)
{
    if (activity == NULL) {
        printf("Aucune activite disponible.\n");
        return NULL;
    }
    printf("\n=== CHOIX DE L'ACTIVITE ===\n");
    ACTIVITY_NODE* actuel = activity;
    int count = 0;

    while (actuel != NULL) {
        if(actuel->activity != NULL){
            printf("%d - %s\n", count + 1, actuel->activity->name);
            actuel = actuel->next   ;
            count++;
        }
    }
    
    if (count == 0) {
        printf("Aucune activite disponible.\n");
        return NULL;
    }
    int choice;
    printf("Voici les propositions suivantes :\n");
    scanf("%d", &choice);

    if (choice == 0 || choice > count) {
        printf("Vous avez choisi de ne pas faire d'activite.\n");
        return NULL;
    }
    actuel = activity;
    for (int i = 1; i < choice; i++) {
        actuel = actuel->next;
    }
    return actuel;

}

int isSpawnConditionChecked(CULT *cult, CONDITION *cond){
    if (cult == NULL ||cond == NULL) {
        return FALSE;
    }

    int memberCount = 0;
    GROUP *g = cult->members;

    while (g != NULL) {
        memberCount++;
        g = g->next;
    }

    //if (memberCount < cond->minMember) return 0;
    //if (memberCount > cond->maxMember) return 0;
    //TODO ajouter le reste des stats 

    if(getLegitimity(cult) < cond->minLegitimity) return 0;
    if(getVisibility(cult) < cond->minVisibility) return 0;
    if(getIllegalityOfGroup(cult->members) < cond->minIllegality) return 0;

    return 1;
}


float getLegitimity(CULT *culte){
    if (culte == NULL){
        return 0.0;
    }    

    float membersLegitimity = getLegitimityOfMembers(culte->members) * 0.40;
    float assignsLegitimity = getLegitimityOfAssigns(culte->assigns) * 0.60;

    float baseLegitimity = membersLegitimity + assignsLegitimity;
    float visibility = getVisibility(culte);
    float control = (MAX_LEGITIMITY * 0.2) * getControl(culte); // bonus ne depassant pas 20% du score total

    return baseLegitimity + visibility + control;
}

float getLegitimityOfMembers(GROUP *members){
    if (members == NULL) {
        return 0.0;
    }

    float legitimity = 0;
    GROUP *current = members;

    while (current != NULL)
    {
        legitimity += current->member->impact.legitimity;
        current = current->next;
    }

    return legitimity;
}

float getLegitimityOfAssigns(ASSIGN *activities){
    if(activities == NULL){
        return 0.0;
    }

    float legitimity = 0;
    int count = 0;
    ASSIGN *current = activities;

    while (current != NULL)
    {
        if (isAssignFinished(current)){
            if (current->isSuccess){
                legitimity += current->activity->impactSuccess.legitimity;
            } else {
                legitimity += current->activity->impactFailed.legitimity;
            } 
            count++;
        }
        current = current->next;

    }

    return legitimity / count;
}

float getVisibility(CULT *culte){
    if (culte == NULL){
        return 0.0;
    }

    float membersVisibility = getVisibilityOfMembers(culte->members) * 0.40;
    float assignsVisibility = getVisibilityOfAssigns(culte->assigns) * 0.60;

    return membersVisibility + assignsVisibility;
}

float   getVisibilityOfMembers(GROUP *members){
    if (members == NULL) {
        return 0.0;
    }

    float visibility = 0;
    GROUP *current = members;

    while (current != NULL)
    {
        visibility += current->member->impact.visibility;
        current = current->next;
    }

    return visibility;
}

float getVisibilityOfAssigns(ASSIGN *activities){
    if(activities == NULL){
        return 0.0;
    }

    float visibility = 0;
    int count = 0;
    ASSIGN *current = activities;

    while (current != NULL)
    {
        if (isAssignFinished(current)){
            if (current->isSuccess){
                visibility += current->activity->impactSuccess.visibility;
            } else {
                visibility += current->activity->impactFailed.visibility;
            }
            count++; // on ne compte que les activité fini pas celle en cours
        }
        current = current->next;
    }

    return visibility / count;
}

float getControl(CULT *culte){
    if (culte == NULL || culte->members == NULL){
        return 0.0;
    }

    GROUP *current = culte->members;

    float total = 0;
    int count = 0;

    while (current != NULL)
    {
        total += current->member->control;
        count++;
        current = current->next;
    }

    return total / count;
}

float getIllegalityOfGroup(GROUP *group){
    if(group == NULL){
        return 0.0;
    }

    GROUP *current = group;
    float result = 0;
    while(current != NULL){
        if(group->member != NULL){
            result += group->member->impact.illegality;
        }
            current = current->next;
    }
    return result;
}


MEMBER * hireMember(CULT * cult, GAME_CONF * conf){
    if(cult == NULL || conf == NULL) return NULL;

    GROUP *spawnList = generateSpawnMember(conf, cult->members);

    if(spawnList == NULL){
        showEventPopup("Recrutement", "Personne ne souhaite rejoindre votre culte.");
        return NULL;
    }

    // popup selection
    MEMBER *selected = selectMemberPopup(spawnList);

    if(selected == NULL){
        return NULL;
    }

    int confirm = confirmPopup("Recruter ce membre ?");

    if(!confirm){
        return NULL;
    }

    MEMBER *newMember = cloneMember(selected);

    return newMember;
}

GROUP * generateSpawnMember(GAME_CONF * conf, GROUP * group){
    if(conf == NULL || conf->allPeoples == NULL)
        return NULL;

    GROUP * spawnList = NULL;
    GROUP * current = conf->allPeoples;

    while(current != NULL)
    {
        float r = (float)rand() / RAND_MAX;

        if(r < current->member->spawnChance && !isMemberIsInGroup(current->member, group)) 
        {
            // on ajoute le prototype dans la liste de spawn
            GROUP *node = createGroupNode(current->member);
            spawnList = addEndGroup(spawnList, node);
        }

        current = current->next;
    }

    return spawnList;
}