#include "cult.h"

/* -------------------------------------------------- */
/* Initialise les événements depuis events.cfg       */
/* -------------------------------------------------- */
EVENT *initEvents(void)
{
    FILE *file = fopen(PATH_EVENTS, "r");
    if (!file)
    {
        printf("Erreur ouverture events.cfg\n");
        return NULL;
    }

    EVENT *head = NULL;
    EVENT *current = NULL;
    char line[256];

    while (fgets(line, sizeof(line), file))
    {
        line[strcspn(line, "\n")] = 0;

        // Ignore commentaires et lignes vides
        if (line[0] == '#' || strlen(line) == 0)
            continue;

        /* ---------- NOUVEL EVENT ---------- */

        if (strcmp(line, "[event]") == 0)
        {
            EVENT *newEvent = calloc(1, sizeof(EVENT));
            if (!newEvent)
                break;

            newEvent->sponCondition = calloc(1, sizeof(CONDITION));
            newEvent->successCondition = calloc(1, sizeof(CONDITION));
            newEvent->impact = calloc(1, sizeof(IMPACT));

            if (!newEvent->sponCondition || 
                !newEvent->successCondition || 
                !newEvent->impact)
            {
                free(newEvent);
                break;
            }

            newEvent->next = NULL;

            if (!head)
                head = newEvent;
            else
                current->next = newEvent;

            current = newEvent;
        }

        /* ---------- LECTURE DES PARAMETRES ---------- */

        else if (current)
        {
            char key[100];
            char value[200];

            if (sscanf(line, " %99[^=]= %199[^\n]", key, value) == 2)
            {
                trim(key);
                trim(value);

                /* ---------- STRINGS ---------- */

                if (strcmp(key, "name") == 0)
                {
                    strncpy(current->name, value, NAME_SIZE - 1);
                    current->name[NAME_SIZE - 1] = '\0';
                }
                else if (strcmp(key, "description") == 0)
                {
                    strncpy(current->description, value, DESC_EVENT_SIZE - 1);
                    current->description[DESC_EVENT_SIZE - 1] = '\0';
                }

                /* ---------- SPAWN CONDITION ---------- */

                else if (strcmp(key, "spawnMinLegitimity") == 0)
                    current->sponCondition->minLegitimity = atof(value);

                else if (strcmp(key, "spawnMinVisibility") == 0)
                    current->sponCondition->minVisibility = atof(value);

                else if (strcmp(key, "spawnMinIllegality") == 0)
                    current->sponCondition->minIllegality = atof(value);

                else if (strcmp(key, "spawnMinMember") == 0)
                    current->sponCondition->minMember = atoi(value);

                else if (strcmp(key, "spawnMaxMember") == 0)
                    current->sponCondition->maxMember = atoi(value);

                /* ---------- SUCCESS CONDITION ---------- */

                else if (strcmp(key, "successMinLegitimity") == 0)
                    current->successCondition->minLegitimity = atof(value);

                else if (strcmp(key, "successMinVisibility") == 0)
                    current->successCondition->minVisibility = atof(value);

                else if (strcmp(key, "successMinIllegality") == 0)
                    current->successCondition->minIllegality = atof(value);

                else if (strcmp(key, "successMinMember") == 0)
                    current->successCondition->minMember = atoi(value);

                else if (strcmp(key, "successMaxMember") == 0)
                    current->successCondition->maxMember = atoi(value);

                /* ---------- IMPACT ---------- */

                else if (strcmp(key, "impactLegitimity") == 0)
                    current->impact->legitimity = atof(value);

                else if (strcmp(key, "impactVisibility") == 0)
                    current->impact->visibility = atof(value);

                else if (strcmp(key, "impactIllegality") == 0)
                    current->impact->illegality = atof(value);

                else if (strcmp(key, "impactFund") == 0)
                    current->impact->fund = atof(value);

                else if (strcmp(key, "impactControl") == 0)
                    current->impact->control = atof(value);
            }
        }
    }

    fclose(file);
    return head;
}

EVENT *getEvent(CULT *cult, GAME_CONF *conf)
{
    EVENT *current = conf->allEvents;
    int count = 0;

    // compter les events possibles
    while (current != NULL) {
        if (isSpawnConditionChecked(cult, current->sponCondition)) {
            count++;
        }
        current = current->next;
    }

    if (count == 0) return NULL;

    // tirer un index aleatoire
    int indexRandomEvent = rand() % count;

    // on repars de la tete de liste 
    current = conf->allEvents;

    // retrouver l'event correspondant
    while (current != NULL) {
        if (isSpawnConditionChecked(cult, current->sponCondition)) {
            if (indexRandomEvent == 0){
                return current;
            }
            indexRandomEvent--;
        }
        current = current->next;
    }

    return NULL;
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


