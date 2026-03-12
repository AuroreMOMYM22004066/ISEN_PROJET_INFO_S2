#include "cult.h"


/* -------------------------------------------------- */
/* Initialise les activités depuis activities.cfg    */
/* -------------------------------------------------- */
ACTIVITY_NODE *initActivities(void){
    FILE *file = fopen(PATH_ACTIVITIES, "r");
    if (!file)
    {
        printf("Erreur ouverture activities.cfg\n");
        return NULL;
    }

    ACTIVITY_NODE *head = NULL;
    ACTIVITY_NODE *tail = NULL;
    ACTIVITY *current = NULL;

    char line[256];

    while (fgets(line, sizeof(line), file))
    {
        line[strcspn(line, "\n")] = 0;

        if (line[0] == '#' || strlen(line) == 0)
            continue;

        if (strcmp(line, "[activity]") == 0)
        {
            ACTIVITY *newActivity = calloc(1, sizeof(ACTIVITY));
            if (!newActivity)
                break;

            newActivity->sponCondition = calloc(1, sizeof(CONDITION));
            newActivity->successCondition = calloc(1, sizeof(CONDITION));

            if (!newActivity->sponCondition || !newActivity->successCondition)
            {
                free(newActivity);
                break;
            }

            ACTIVITY_NODE *node = malloc(sizeof(ACTIVITY_NODE));
            if (!node)
            {
                free(newActivity);
                break;
            }

            node->activity = newActivity;
            node->next = NULL;

            if (!head)
                head = node;
            else
                tail->next = node;

            tail = node;
            current = newActivity;
        }
        else if (current)
        {
            char key[100];
            char value[200];

            if (sscanf(line, " %99[^=]= %199[^\n]", key, value) == 2)
            {
                trim(key);
                trim(value);

                if (strcmp(key, "name") == 0)
                {
                    strncpy(current->name, value, NAME_SIZE - 1);
                    current->name[NAME_SIZE - 1] = '\0';
                }
                else if (strcmp(key, "description") == 0)
                {
                    strncpy(current->description, value, DESC_ACTIVITY_SIZE - 1);
                    current->description[DESC_ACTIVITY_SIZE - 1] = '\0';
                }

                /* ---------- SPAWN ---------- */

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

                /* ---------- SUCCESS IMPACT ---------- */

                else if (strcmp(key, "successLegitimity") == 0)
                    current->impactSuccess.legitimity = atof(value);

                else if (strcmp(key, "successVisibility") == 0)
                    current->impactSuccess.visibility = atof(value);

                else if (strcmp(key, "successIllegality") == 0)
                    current->impactSuccess.illegality = atof(value);

                else if (strcmp(key, "successFund") == 0)
                    current->impactSuccess.fund = atof(value);

                /* ---------- FAIL IMPACT ---------- */

                else if (strcmp(key, "failLegitimity") == 0)
                    current->impactFailed.legitimity = atof(value);

                else if (strcmp(key, "failVisibility") == 0)
                    current->impactFailed.visibility = atof(value);

                else if (strcmp(key, "failIllegality") == 0)
                    current->impactFailed.illegality = atof(value);

                else if (strcmp(key, "failFund") == 0)
                    current->impactFailed.fund = atof(value);

                /* ---------- AUTRES ---------- */

                else if (strcmp(key, "time") == 0)
                    current->time = atoi(value);

                else if (strcmp(key, "PA") == 0)
                    current->PA = atoi(value);

                else if (strcmp(key, "cost") == 0)
                    current->cost = atof(value);
            }
        }
    }

    fclose(file);
    return head;
}


void showActivities(ACTIVITY_NODE * activity){
    while (activity != NULL){
        showActivity(activity->activity);
        activity = activity->next;
    }
}

void showActivity(ACTIVITY * activity){
    if (activity != NULL){
        printf("Nom : %s\n",activity->name);
        printf("Description : %s\n",activity->description);
        printf("Time : %d\n",activity->time);
        printf("count : %f\n",activity->cost);
        printf("PA : %d\n\n",activity->PA);
    }
}


ACTIVITY_NODE *getAvailableActivities(CULT *cult, GAME_CONF *conf){
    if(cult == NULL || conf == NULL){
        return NULL;
    }

    ACTIVITY_NODE *result = NULL;
    ACTIVITY_NODE *current = conf->allActivities;

    while(current != NULL){

        if(isSpawnConditionChecked(cult,current->activity->sponCondition)){

            ACTIVITY_NODE *node = malloc(sizeof(ACTIVITY_NODE));
            node->activity = current->activity;

            node->next = result;
            result = node; // attention ici la liste est construite a l'envers dernier entrer = tete de liste
        }

        current = current->next;
    }

    return result;
}


void freeActivityNodes(ACTIVITY_NODE *list){

    ACTIVITY_NODE *tmp;

    while(list){
        tmp = list;
        list = list->next;
        free(tmp);
    }
}

