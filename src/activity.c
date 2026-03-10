#include "cult.h"


/* -------------------------------------------------- */
/* Initialise les activités depuis activities.cfg    */
/* -------------------------------------------------- */
ACTIVITY *initActivities(void)
{
    FILE *file = fopen(PATH_ACTIVITIES, "r");
    if (!file)
    {
        printf("Erreur ouverture activities.cfg\n");
        return NULL;
    }

    ACTIVITY *head = NULL;
    ACTIVITY *current = NULL;
    char line[256];

    while (fgets(line, sizeof(line), file))
    {
        // Supprime retour ligne
        line[strcspn(line, "\n")] = 0;

        // Ignore commentaires et lignes vides
        if (line[0] == '#' || strlen(line) == 0)
            continue;

        // Nouvelle activité
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

            newActivity->next = NULL;

            if (!head)
                head = newActivity;
            else
                current->next = newActivity;

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

                /* ---------- STRINGS ---------- */

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
        printf("count : %f\n",activity->cost);
        printf("PA : %d\n\n",activity->PA);
    }
}
