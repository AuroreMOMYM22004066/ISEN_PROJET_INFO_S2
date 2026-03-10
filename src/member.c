#include "cult.h"


/* -------------------------------------------------- */
/* Initialise les profils depuis members.cfg et types.cfg */
/* -------------------------------------------------- */

GROUP *initPeoplesProfils(void)
{
    FILE *file = fopen(PATH_TYPE, "r");
    if (!file)
    {
        printf("Erreur ouverture %s\n", PATH_TYPE);
        return NULL;
    }

    GROUP *allGroups = NULL;
    char line[256];

    // Variables locales pour stocker les infos du type
    char typeName[NAME_SIZE];
    float spawnChance;
    IMPACT typeImpact;
    float control;
    char typeFile[256];

    while (fgets(line, sizeof(line), file))
    {
        line[strcspn(line, "\n")] = 0;
        if (line[0] == '#' || strlen(line) == 0)
            continue;

        if (strcmp(line, "[type]") == 0)
        {
            // Reset variables locales
            memset(typeName, 0, sizeof(typeName));
            spawnChance = 0;
            memset(&typeImpact, 0, sizeof(IMPACT));
            control = 1.0f;
            typeFile[0] = '\0';
        }
        else
        {
            char key[100], value[200];
            if (sscanf(line, " %99[^=]= %199[^\n]", key, value) == 2)
            {
                trim(key);
                trim(value);

                if (strcmp(key, "name") == 0)
                    strncpy(typeName, value, NAME_SIZE - 1);
                else if (strcmp(key, "apparition") == 0)
                    spawnChance = atof(value);
                else if (strcmp(key, "legitimity") == 0)
                    typeImpact.legitimity = atof(value);
                else if (strcmp(key, "visibility") == 0)
                    typeImpact.visibility = atof(value);
                else if (strcmp(key, "illegality") == 0)
                    typeImpact.illegality = atof(value);
                else if (strcmp(key, "fund") == 0)
                    typeImpact.fund = atof(value);
                else if (strcmp(key, "control") == 0)
                    control = atof(value);
                else if (strcmp(key, "file") == 0)
                {
                    trim(value);
                    value[strcspn(value, "\r\n")] = 0;  // <- supprime \r et \n
                    snprintf(typeFile, sizeof(typeFile), "assets/config/%s", value);
                    printf("Ouverture du fichier membre : '%s'\n", typeFile);
                    FILE *memberFile = fopen(typeFile, "r");
                    if (memberFile == NULL){
                        printf("Impossible d'ouvrir %s\n", typeFile);
                        continue;
                    }

                    char memberLine[256];
                    MEMBER *newMember = NULL;

                    while (fgets(memberLine, sizeof(memberLine), memberFile))
                    {
                        memberLine[strcspn(memberLine, "\n")] = 0;
                        if (memberLine[0] == '#' || strlen(memberLine) == 0)
                            continue;

                        if (strcmp(memberLine, "[member]") == 0)
                        {
                            // Ajouter le membre précédent si nécessaire
                            if (newMember)
                            {
                                allGroups = addMemberToGroup(allGroups, newMember);
                                newMember = NULL;
                            }

                            newMember = calloc(1, sizeof(MEMBER));
                            if (!newMember)
                                continue;

                            // Copier les infos du type
                            newMember->spawnChance = spawnChance;
                            newMember->impact = typeImpact;
                            newMember->control = control;
                            //printf("Membre : %s\n", typeName);
                            //printf("spawnChance : %f\n", spawnChance);
                            //printf("typeImpact : %f\n", typeImpact);
                            //printf("control : %f\n", control);
                            
                        }
                        else if (newMember)
                        {
                            char mKey[100], mValue[200];
                            if (sscanf(memberLine, " %99[^=]= %199[^\n]", mKey, mValue) == 2)
                            {
                                trim(mKey);
                                trim(mValue);
                                //printf("\n\n\n%s : %s \n\n\n", mKey, mValue);

                                if (strcmp(mKey, "name") == 0)
                                    strncpy(newMember->name, mValue, NAME_SIZE - 1);
                                else if (strcmp(mKey, "description") == 0)
                                {
                                    // Concaténer typeName + description du profil
                                    snprintf(newMember->description, DESC_MEMBER_SIZE, "%s - %s", typeName, mValue);
                                }
                            }
                        }
                    }

                    // Ajouter le dernier membre du fichier
                    if (newMember)
                    {
                        allGroups = addMemberToGroup(allGroups, newMember);
                        newMember = NULL;
                    }

                    fclose(memberFile);
                }
            }
        }
    }

    fclose(file);
    return allGroups;
}

/* -------------------------------------------------- */
/* Ajoute un groupe en tête                           */
/* -------------------------------------------------- */
GROUP *addHeadGroup(GROUP *head, GROUP *newMemberGroup){
    if(newMemberGroup == NULL){
        return head;
    }
    if(isEmptyGroup(head)){
        newMemberGroup->next = NULL;
        newMemberGroup->previous = NULL;
        return newMemberGroup;
    }
    newMemberGroup->next = head;
    newMemberGroup->previous = NULL;
    head->previous = newMemberGroup;
    return newMemberGroup;
}

/* -------------------------------------------------- */
/* Ajoute un groupe à la fin                           */
/* -------------------------------------------------- */
GROUP *addEndGroup(GROUP *head, GROUP *newGroup){
    if(newGroup == NULL){
        return head;
    } 

    newGroup->next = NULL;
    if(isEmptyGroup(head)){
        return newGroup;
    }

    GROUP *current = head;
    while(current->next != NULL ){
        current = current->next;
    }

    current->next = newGroup;
    newGroup->previous = current;
    return head;
}

/* -------------------------------------------------- */
/* Insère un groupe dans la liste triée par fund      */
/* Ordre croissant : moins de fonds -> début, plus -> fin */
/* -------------------------------------------------- */
GROUP *insertOrderGroup(GROUP *head, GROUP *newGroup){
    if (newGroup == NULL){
         return head;
    }
    if (isEmptyGroup(head) || newGroup->member->impact.fund < head->member->impact.fund){
        return addHeadGroup(head, newGroup);
    }
        
    GROUP *current = head;

    while (current->next != NULL && current->next->member->impact.fund < newGroup->member->impact.fund){
        current = current->next;
    }


    if (current->next == NULL){
        // add end liste
        newGroup->next = NULL;
    } else {
        newGroup->next = current->next;
        current->next->previous = newGroup;
    }
    /* insertion avant current */
    newGroup->previous = current;
    current->next = newGroup;

    return head;
}

/* -------------------------------------------------- */
/* Crée un nouveau node GROUP contenant un MEMBER     */
/* -------------------------------------------------- */
GROUP *createGroupNode(MEMBER *member){
    if (member == NULL){
        return NULL;
    }

    GROUP *newGroupNode = malloc(sizeof(GROUP));
    if (!newGroupNode) return NULL;

    newGroupNode->member = member;
    newGroupNode->next = NULL;
    newGroupNode->previous = NULL;

    return newGroupNode;
}

GROUP *addMemberToGroup(GROUP *group, MEMBER *member)
{
    //showMember(member);
    GROUP *newMemberGroup = createGroupNode(member);
    group = insertOrderGroup(group, newMemberGroup);
    return group;
}

/* -------------------------------------------------- */
/* Crée un nouveau MEMBER depuis la conf              */
/* -------------------------------------------------- */
MEMBER *cloneMember(MEMBER *proto)
{
    if (proto == NULL) return NULL;

    MEMBER *newMember = malloc(sizeof(MEMBER));
    if (!newMember) return NULL;

    *newMember = *proto;   // copie complète de la structure

    return newMember;
}

/* -------------------------------------------------- */
/* Vérification sur la liste                          */
/* -------------------------------------------------- */
int isEmptyGroup(GROUP *head){
    return head == NULL;
}

int isMemberAvailable(MEMBER * member, ASSIGN * assign){
    if(member == NULL || assign == NULL){
        return TRUE;
    }

    int isAvailable = TRUE;
    ASSIGN * current = assign;
    while(current != NULL){
        // si activity en cours et membre present dans le group false
        if(!isAssignFinished(current) && isMemberIsInGroup(member, current->group)){ 
            return FALSE;
        }
        current = current->next;
    }

    return isAvailable;
}

int isMemberIsInGroup(MEMBER * member, GROUP * group){
    if(member == NULL || group == NULL){
        return FALSE;
    }

    GROUP * current = group;
    while(current != NULL && current->member != NULL){
        if(member == current->member){ // on compare les adr memoire
            return TRUE;
        }  
        current = current->next;
    }
    return FALSE;
}

/* -------------------------------------------------- */
/* Affiche un membre                                  */
/* -------------------------------------------------- */
void showMember(MEMBER *member)
{
    if (!member)
        return;

    printf("---- Member ----\n");
    printf("Name       : %s\n", member->name);
    printf("Description: %s\n", member->description);
    printf("SpawnChance: %.2f\n", member->spawnChance);
    printf("Control    : %.2f\n", member->control);
    printf("Impact:\n");
    printf("  Legitimity : %.2f\n", member->impact.legitimity);
    printf("  Visibility : %.2f\n", member->impact.visibility);
    printf("  Illegality : %.2f\n", member->impact.illegality);
    printf("  Fund       : %.2f\n", member->impact.fund);
    printf("  Control    : %.2f\n", member->impact.control);
}

/* -------------------------------------------------- */
/* Affiche un groupe (liste chaînée)                 */
/* -------------------------------------------------- */
void showGroup(GROUP *group){
    if (!group)
    {
        printf("Le groupe est vide.\n");
        return;
    }

    printf("==== Group ====\n");
    GROUP *current = group;
    int count = 1;
    while (current)
    {
        printf("Member #%d:\n", count);
        showMember(current->member);
        printf("\n");
        current = current->next;
        count++;
    }
}

