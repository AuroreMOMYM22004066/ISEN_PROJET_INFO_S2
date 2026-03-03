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


void showActivities(ACTIVITY * activity){
    while (activity != NULL){
        showActivity(activity);
        activity = activity->next;
    }
}

void showActivity(ACTIVITY * activity){
    if (activity != NULL){
        printf("Nom : %s\n",activity->name);
        printf("Description : %s\n",activity->description);
        printf("Time : %d\n",activity->time);
        printf("count : %d\n",activity->cost);
        printf("PA : %d\n\n",activity->PA);
    }
}


void showEvents(EVENT *event)
{
    if (!event)
    {
        printf("Aucun evenement à afficher.\n");
        return;
    }

    EVENT *current = event;
    int count = 1;

    while (current)
    {
        printf("---------- Evenement %d ----------\n", count);
        printf("Nom         : %s\n", current->name);
        printf("Description : %s\n", current->description);

        // Conditions de spawn
        if (current->sponCondition)
        {
            printf("Conditions de spawn:\n");
            printf("  Legitimite min : %.2f\n", current->sponCondition->minLegitimity);
            printf("  Visibilite min : %.2f\n", current->sponCondition->minVisibility);
            printf("  Illegalite min : %.2f\n", current->sponCondition->minIllegality);
            printf("  Membres min    : %d\n", current->sponCondition->minMember);
            printf("  Membres max    : %d\n", current->sponCondition->maxMember);
        }

        // Conditions de succès
        if (current->successCondition)
        {
            printf("Conditions de succes:\n");
            printf("  Legitimite min : %.2f\n", current->successCondition->minLegitimity);
            printf("  Visibilite min : %.2f\n", current->successCondition->minVisibility);
            printf("  Illegalite min : %.2f\n", current->successCondition->minIllegality);
            printf("  Membres min    : %d\n", current->successCondition->minMember);
            printf("  Membres max    : %d\n", current->successCondition->maxMember);
        }

        // Impact
        if (current->impact)
        {
            printf("Impact de l'evenement:\n");
            printf("  Legitimite : %.2f\n", current->impact->legitimity);
            printf("  Visibilite : %.2f\n", current->impact->visibility);
            printf("  Illegalite : %.2f\n", current->impact->illegality);
            printf("  Fonds      : %.2f\n", current->impact->fund);
            printf("  Controle   : %.2f\n", current->impact->control);
        }

        printf("\n");

        current = current->next;
        count++;
    }
}

void playGame(CULT * cult, GAME_CONF * conf){
    printf("START - playGame\n");
}


void playFastGame(CULT * Cult, GAME_CONF * conf, int nbRounds){
    printf("START - playFastGame\n");
}

ACTIVITY* menuActivitity(ACTIVITY* activity)
{
    if (activity == NULL) {
        printf("Aucune activite disponible.\n");
        return NULL;
    }
    printf("\n=== CHOIX DE L'ACTIVITE ===\n");
    ACTIVITY* actuel = activity;
    int count = 0;

    while (actuel != NULL) {
        printf("%d - %s\n", count + 1, actuel->name);
        actuel = actuel->next;
        count++;
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