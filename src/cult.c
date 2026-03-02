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
        printf("PA : %d\n",activity->PA);
    }
}

CULT * initCult(){
    return NULL;
}

GAME_CONF * initConf(){
    return NULL;
}

void playGame(CULT * cult, GAME_CONF * conf){
    printf("START - playGame\n");
}


void playFastGame(CULT * Cult, GAME_CONF * conf, int nbRounds){
    printf("START - playFastGame\n");
}