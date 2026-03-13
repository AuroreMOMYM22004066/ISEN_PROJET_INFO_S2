// display.c
#include "cult.h"
#include <ncurses.h>
#include <string.h>


// actions principales
char *actions[] = {
    "Recruter un membre",
    "Assigner activite",
    "Passer au jour suivant"
};
int actionCount = 3;



// ---------- INIT & END ----------
void initUI() {
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
}

void endUI() {
    endwin();
}

// ---------- CREATE WINDOWS ----------
void createWindows(UI *ui) {
    int h_status = 7;
    int h_actions = 10;
    int h_members = 10;
    int h_log = 10;
    int w = COLS;

    ui->statusWin = newwin(h_status, w, 0, 0);
    ui->actionWin = newwin(h_actions, w/2, h_status, 0);
    ui->memberWin = newwin(h_members, w/2, h_status, w/2);
    ui->logWin = newwin(h_log, w, h_status + h_actions, 0);
}

// ---------- DRAW LAYOUT ----------
void drawLayout(UI *ui) {
    box(ui->statusWin, 0, 0);
    box(ui->actionWin, 0, 0);
    box(ui->memberWin, 0, 0);
    box(ui->logWin, 0, 0);

    mvwprintw(ui->statusWin, 0, 2, " CULT STATUS ");
    mvwprintw(ui->actionWin, 0, 2, " ACTIONS ");
    mvwprintw(ui->memberWin, 0, 2, " MEMBERS ");
    mvwprintw(ui->logWin, 0, 2, " ACTIVITES EN COURS ");

    wrefresh(ui->statusWin);
    wrefresh(ui->actionWin);
    wrefresh(ui->memberWin);
    wrefresh(ui->logWin);
}

// ---------- DRAW STATUS ----------
void drawStatus(UI *ui, CULT *cult) {

    werase(ui->statusWin);
    box(ui->statusWin,0,0);

    int mid = COLS/2;

    // ---- Bloc gauche : infos culte ----
    mvwprintw(ui->statusWin,1,2,"Secte : %s", cult->nameCult);
    mvwprintw(ui->statusWin,2,2,"Gourou : %s", cult->nameCultLeader);

    mvwprintw(ui->statusWin,3,2,"Legitimite : %.2f / %d", getLegitimity(cult), MAX_LEGITIMITY);
    //drawProgressBar(ui->statusWin,3,15,20,getLegitimity(cult));

    // ---- Bloc droite : stats ----
    mvwprintw(ui->statusWin,1,mid,"Visibilite : %.2f", getVisibility(cult));
    mvwprintw(ui->statusWin,2,mid,"Controle : %.2f", getControl(cult));
    mvwprintw(ui->statusWin,3,mid,"Jour : %d | PA : %d", cult->elapsedTime, cult->pa);
    mvwprintw(ui->statusWin,4,mid,"Fonds : %.2f", cult->funds);

    wrefresh(ui->statusWin);
}

// ---------- DRAW ACTIONS ----------
void drawActions(UI *ui) {

    werase(ui->actionWin);
    box(ui->actionWin,0,0);

    for(int i=0;i<actionCount;i++) {

        if(i == ui->selectedAction)
            wattron(ui->actionWin,A_REVERSE);

        mvwprintw(ui->actionWin,2+i,2,"%s",actions[i]);

        wattroff(ui->actionWin,A_REVERSE);
    }

    wrefresh(ui->actionWin);
}

// ---------- DRAW MEMBERS ----------
void drawMembers(UI *ui, CULT *cult) {
    GROUP *members = cult->members;

    werase(ui->memberWin);
    box(ui->memberWin,0,0);

    int h = getmaxy(ui->memberWin) - 2; // lignes disponibles pour membres
    int y = 1;
    int idx = 0;

    GROUP *current = members;

    // avancer jusqu'à memberOffset
    while(current && idx < ui->memberOffset) {
        current = current->next;
        idx++;
    }

    while(current && y <= h) {
        mvwprintw(ui->memberWin,y,2,"%s Control: %.2f Etat: %s",
            current->member->name,
            current->member->control,
            isMemberAvailable(current->member, cult->assigns)?"Libre":"Occupe");
        y++;
        current = current->next;
    }

    // si reste des membres non affichés
    if(current)
        mvwprintw(ui->memberWin,y,2,"▼");

    wrefresh(ui->memberWin);
}

// ---------- DRAW PROGRESS BAR ----------
void drawProgressBar(WINDOW *win, int y, int x, int width, float value){
    int filled = (int)(value * width);

    mvwprintw(win, y, x, "[");

    for(int i=0;i<width;i++){
        if(i < filled)
            waddch(win, '#');
        else
            waddch(win, '-');
    }

    waddch(win, ']');
}

// ---------- DRAW ASSIGN ---------- 
void drawAssigns(UI *ui, CULT *cult){
    if(ui == NULL || cult == NULL) return;

    werase(ui->logWin);
    box(ui->logWin,0,0);

    mvwprintw(ui->logWin,0,2," ACTIVITES EN COURS ");

    ASSIGN *current = cult->assigns;
    int y = 1;

    while(current != NULL && y < 4){

        if(current->activity != NULL && current->group != NULL){
            if(isAssignFinished(current)){
                current = current->next;
                continue;
            }

            int remaining = current->activity->time - current->time;

            GROUP *g = current->group;

            char names[100] = "";
            char buffer[32];

            while(g){
                snprintf(buffer,sizeof(buffer),"%s ",g->member->name);
                strcat(names,buffer);
                g = g->next;
            }

            mvwprintw(ui->logWin,y,2,"%s (%d j)",current->activity->name,remaining);
            y++;

            mvwprintw(ui->logWin,y,4,"%s",names);
            y++;
        }

        current = current->next;
    }
        
    if(current != NULL)
        mvwprintw(ui->logWin,y,2,"...");

    wrefresh(ui->logWin);
}

// ---------- EVENT POPUP ----------
void showEventPopup(char *title, char *desc) {
    int h = 10, w = 50;
    int y = (LINES - h)/2;
    int x = (COLS - w)/2;
    WINDOW *popup = newwin(h,w,y,x);

    box(popup,0,0);
    wattron(popup,COLOR_PAIR(1));
    mvwprintw(popup,1,(w - strlen(title))/2,"%s",title);
    wattroff(popup,COLOR_PAIR(1));

    mvwprintw(popup,3,2,"%s",desc);
    mvwprintw(popup,h-2,10,"Appuyez sur ENTER");
    wrefresh(popup);

    while(getch()!='\n');

    delwin(popup);
}

// ---------- START SCREEN ----------
void showStartScreen(){

    clear();

    const char *art[] = {
"##################################################################################################################",
"##################################################################################################################",
"##################################################################################################################",
"##################################################################################################################",
"##################################################################################################################",
"#######################################################+++########################################################",
"######################################################+++++#######################################################",
"#####################################################+++++++######################################################",
"####################################################+++++++++#####################################################",
"####################################################+++++++++#####################################################",
"###################################################+++++++++++####################################################",
"##################################################+++++++++++++###################################################",
"##################################################+++++++++++++###################################################",
"#################################################+++++-----+++++##################################################",
"#################################################++-----------++##################################################",
"################################################+++-----------+++#################################################",
"###############################################+++-------------+++################################################",
"##############################################+++---------------+++###############################################",
"#######################++++++++++++++++++++++++++---------------++++++++++++++++++++++++++########################",
"###################+++++++++++++++++++++++++++++-----------------+++++++++++++++++++++++++++++####################",
"####################++++++++++++++++++++++++++++-----------------++++++++++++++++++++++++++++#####################",
"######################+++++++++++++++++++++++++++--------------++++++++++++++++++++++++++++#######################",
"########################+++++++++++++++++++++++++++-+-------+-+++++++++++++++++++++++++++#########################",
"##########################+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++###########################",
"#############################+++++++++++++++++++++++++++++++++++++++++++++++++++++++##############################",
"###############################+++++++++++++++++++++++++++++++++++++++++++++++++++################################",
"##################################+++++++++++++++++++++++++++++++++++++++++++++###################################",
"####################################+++++++++++++++++++++++++++++++++++++++++#####################################",
"######################################+++++++++++++++++++++++++++++++++++++#######################################",
"#######################################+++++++++++++++++++++++++++++++++++########################################",
"#######################################+++++++++++++++++++++++++++++++++++########################################",
"######################################+++++++++++++++++++++++++++++++++++++#######################################",
"######################################+++++++++++++++++++++++++++++++++++++#######################################",
"#####################################+++++++++++++++++++++++++++++++++++++++######################################",
"####################################+++++++++++++++++++++++++++++++++++++++++#####################################",
"####################################+++++++++++++++++++++++++++++++++++++++++#####################################",
"###################################+++++++++++++++++++#####+++++++++++++++++++####################################",
"###################################++++++++++++++++###########++++++++++++++++####################################",
"##################################++++++++++++++#################++++++++++++++###################################",
"##################################++++++++++++#####################++++++++++++###################################",
"#################################++++++++++###########################++++++++++##################################",
"#################################+++++++#################################+++++++##################################",
"#################################++++#######################################++++##################################",
"##################################################################################################################",
"##################################################################################################################",
"##################################################################################################################",
"##################################################################################################################",
"##################################################################################################################"
    };

    int lines = sizeof(art)/sizeof(art[0]);

    int startY = (LINES - lines) / 2;

    for(int i=0;i<lines;i++){
        mvprintw(startY + i, (COLS - strlen(art[i]))/2 , "%s", art[i]);
    }

    mvprintw(LINES-3,(COLS-30)/2,"Appuyez sur ENTER pour commencer");

    refresh();

    while(getch()!='\n');
}

// ---------- END SCREEN ----------
void showEndScreen(float score){

    clear();

    const char *art[]={
"FELICITATIONS !",
"",
"Votre culte a termine la partie rapide."
    };

    int lines = sizeof(art)/sizeof(art[0]);

    int startY = (LINES-lines)/2;

    for(int i=0;i<lines;i++){
        mvprintw(startY+i,(COLS-strlen(art[i]))/2,"%s",art[i]);
    }

    mvprintw(startY+lines+2,(COLS-20)/2,"Score : %.2f",score);

    mvprintw(LINES-3,(COLS-30)/2,"Appuyez sur ENTER");

    refresh();

    while(getch()!='\n');
}

// ---------- VICTORY SCREEN ----------
void showVictoryScreen(float legitimacy){

    clear();

    const char *art[]={
"Votre culte domine le monde."
    };

    int lines = sizeof(art)/sizeof(art[0]);
    int startY = (LINES-lines)/2;

    for(int i=0;i<lines;i++){
        mvprintw(startY+i,(COLS-strlen(art[i]))/2,"%s",art[i]);
    }

    mvprintw(startY+lines+2,(COLS-30)/2,"Legitimite finale : %.2f",legitimacy);
    mvprintw(LINES-3,(COLS-30)/2,"Appuyez sur ENTER");

    refresh();

    while(getch()!='\n');
}

// ---------- SELECT MEMBER POPUP ----------
MEMBER *selectMemberPopup(GROUP *list) {

    if (!list) return NULL;

    GROUP *current = list;
    int selectedIndex = 0;
    int totalCount = 0;

    // Compter le nombre d'éléments
    while(current) {
        totalCount++;
        current = current->next;
    }

    int ch;
    current = list;

    WINDOW *popup = newwin(totalCount + 6, 50, (LINES - (totalCount + 6)) / 2, (COLS - 50) / 2);
    keypad(popup, TRUE);

    while(1) {
        werase(popup);
        box(popup, 0, 0);

        mvwprintw(popup, 1, 2, "Choisir un nouveau membre");

        // Affichage des membres
        GROUP *iter = list;
        int idx = 0;
        while(iter) {
            if(idx == selectedIndex)
                wattron(popup, A_REVERSE);

            mvwprintw(popup, 3 + idx, 2, "%s", iter->member->name);

            if(idx == selectedIndex)
                wattroff(popup, A_REVERSE);

            iter = iter->next;
            idx++;
        }

        // Option "Ne plus recruter"
        if(selectedIndex == totalCount)
            wattron(popup, A_REVERSE);

        mvwprintw(popup, 3 + totalCount, 2, "Ne plus recruter");

        if(selectedIndex == totalCount)
            wattroff(popup, A_REVERSE);

        wrefresh(popup);

        ch = wgetch(popup);

        if(ch == KEY_UP) {
            selectedIndex--;
            if(selectedIndex < 0) selectedIndex = 0;
        }
        else if(ch == KEY_DOWN) {
            selectedIndex++;
            if(selectedIndex > totalCount) selectedIndex = totalCount;
        }
        else if(ch == '\n') {
            delwin(popup);

            if(selectedIndex == totalCount)
                return NULL; // "Ne plus recruter"
            
            // Retourner le membre sélectionné
            iter = list;
            idx = 0;
            while(idx < selectedIndex && iter) {
                iter = iter->next;
                idx++;
            }
            return iter ? iter->member : NULL;
        }
    }
}


GROUP *selectMembersPopup(GROUP *list){

    if(!list) return NULL;

    int count=0;
    GROUP *tmp=list;

    while(tmp){
        count++;
        tmp=tmp->next;
    }

    int selected[count];
    memset(selected,0,sizeof(selected));

    int index=0;

    WINDOW *popup=newwin(count+6,50,(LINES-(count+6))/2,(COLS-50)/2);
    keypad(popup,TRUE);

    while(1){

        werase(popup);
        box(popup,0,0);

        mvwprintw(popup,1,2,"Selectionner membres (SPACE)");

        GROUP *iter=list;
        int i=0;

        while(iter){

            if(i==index) wattron(popup,A_REVERSE);

            mvwprintw(popup,3+i,2,"[%c] %s",
                selected[i]?'x':' ',
                iter->member->name);

            if(i==index) wattroff(popup,A_REVERSE);

            iter=iter->next;
            i++;
        }

        mvwprintw(popup,count+4,2,"ENTER pour valider");

        wrefresh(popup);

        int ch=wgetch(popup);

        if(ch==KEY_UP){
            index--;
            if(index<0) index=0;
        }

        if(ch==KEY_DOWN){
            index++;
            if(index>=count) index=count-1;
        }

        if(ch==' '){
            selected[index]=!selected[index];
        }

        if(ch=='\n') break;
    }

    delwin(popup);

    GROUP *result=NULL;
    GROUP *tail=NULL;

    GROUP *iter=list;
    int i=0;

    while(iter){

        if(selected[i]){

            GROUP *node=malloc(sizeof(GROUP));
            node->member=iter->member;
            node->next=NULL;

            if(!result) result=node;
            else tail->next=node;

            tail=node;
        }

        iter=iter->next;
        i++;
    }

    return result;
}

// ---------- SELECT ACTIVITY POPUP ----------
ACTIVITY *selectActivityPopup(ACTIVITY_NODE *list){
    if(list == NULL){
        return NULL;
    }

    int index = 0;
    int count = 0;
    ACTIVITY_NODE *current = list;

    while(current != NULL){
        count++;
        current = current->next;
    }

    WINDOW *popup = newwin(count+6,60,(LINES-(count+6))/2,(COLS-60)/2);
    keypad(popup,TRUE);

    while(1){

        werase(popup);
        box(popup,0,0);

        mvwprintw(popup,1,2,"Choisir une activite");

        ACTIVITY_NODE *iter = list;
        int i = 0;

        while(iter){

            if(i==index)
                wattron(popup,A_REVERSE);

            mvwprintw(popup,3+i,2,"%s (PA:%d $%.1f)",
                iter->activity->name,
                iter->activity->PA,
                iter->activity->cost);

            if(i==index)
                wattroff(popup,A_REVERSE);

            iter=iter->next;
            i++;
        }

        wrefresh(popup);

        int ch = wgetch(popup);

        if(ch==KEY_UP){
            index--;
            if(index<0) index=0;
        }

        if(ch==KEY_DOWN){
            index++;
            if(index>=count) index=count-1;
        }

        if(ch=='\n'){

            ACTIVITY_NODE *sel = list;
            int i=0;

            while(i<index && sel){
                sel=sel->next;
                i++;
            }

            delwin(popup);
            return sel->activity;
        }
    }
}

// ---------- CONFIRM POPUP ----------
int confirmPopup(char *text){

    WINDOW *popup = newwin(7,40,(LINES-7)/2,(COLS-40)/2);
    keypad(popup, TRUE);

    int choice = 0;

    while(1){

        werase(popup);
        box(popup,0,0);

        mvwprintw(popup,2,2,"%s",text);

        if(choice==0) wattron(popup,A_REVERSE);
        mvwprintw(popup,4,10,"Oui");
        wattroff(popup,A_REVERSE);

        if(choice==1) wattron(popup,A_REVERSE);
        mvwprintw(popup,4,20,"Non");
        wattroff(popup,A_REVERSE);

        wrefresh(popup);

        int ch = wgetch(popup);

        if(ch==KEY_LEFT) choice=0;
        if(ch==KEY_RIGHT) choice=1;

        if(ch=='\n'){
            delwin(popup);
            return choice==0;
        }
    }
}

// ---------- INPUT POPUP ----------
void inputPopup(char *title, char *buffer, int size){

    int h = 7, w = 50;
    int y = (LINES-h)/2;
    int x = (COLS-w)/2;

    WINDOW *popup = newwin(h,w,y,x);
    box(popup,0,0);

    mvwprintw(popup,1,2,"%s",title);
    mvwprintw(popup,3,2,"> ");

    echo();
    curs_set(1);

    wrefresh(popup);
    wgetnstr(popup,buffer,size-1);

    noecho();
    curs_set(0);

    delwin(popup);
}

// ---------- RECRUITMENT HANDLER ----------
void handleRecruitment(CULT *cult, GAME_CONF *conf) {
    if (cult == NULL || conf == NULL) return;

    if(cult->pa < 1){
        showEventPopup("PA insuffisants", "Vous n'avez pas assez de PA pour cette action.");
        return;
    }
    cult->pa -= 1; // coût de génération de la liste

    GROUP *spawnList = generateSpawnMember(conf, cult->members);
    if(spawnList == NULL){
        showEventPopup("Recrutement", "Personne ne souhaite rejoindre votre culte.");
        return;
    }

    int recruiting = 1;
    while(recruiting){
        MEMBER *selected = selectMemberPopup(spawnList);
        if(selected == NULL) break; // annuler

        int confirm = confirmPopup("Recruter ce membre ?");
        if(confirm){
            MEMBER *newMember = cloneMember(selected);
            cult->members = addMemberToGroup(cult->members, newMember);
            showEventPopup("Succes", "Le membre rejoint votre culte !");
            recruiting = 0;
        }
    }
}


// ---------- ACTIVITY HANDLER ----------
void handleActivity(CULT *cult, GAME_CONF *conf){
    ACTIVITY_NODE *available = getAvailableActivities(cult,conf);

    if(available == NULL){
        showEventPopup("Activite","Aucune activite disponible.");
        return;
    }

    ACTIVITY *act = selectActivityPopup(available);
    freeActivityNodes(available);
    if(act == NULL) return;

    if(cult->pa < act->PA){
        showEventPopup("PA insuffisants","Vous n'avez pas assez de PA pour cette action.");
        return;
    }

    if(cult->funds < act->cost){
        showEventPopup("Fonds insuffisants","Vous n'avez pas assez d'argent.");
        return;
    }

    GROUP *freeMembers = getFreeMembers(cult);
    if(!freeMembers){
        showEventPopup("Membres","Aucun membre disponible.");
        return;
    }

    GROUP *group = selectMembersPopup(freeMembers); // multi-sélection possible
    if(group == NULL){
        showEventPopup("Activite","Aucun membre selectionne.");
        return;
    }

    ASSIGN *assign = assignActivityMembers(act, group);
    if(assign == NULL) return;

    cult->assigns = addAssign(cult, assign);

    cult->pa -= act->PA;
    cult->funds -= act->cost;

    showEventPopup("Activite","Activite lancee !");
}

// ---------- PASSER AU JOUR SUIVANT ----------
void handleNextDay(CULT *cult){
    if(cult == NULL){
        return;
    }

    cult->elapsedTime += 1;
    cult->pa = NB_PA;
    ASSIGN *currentAssign = cult->assigns;
    while (currentAssign != NULL){
        if(currentAssign->activity != NULL && !isAssignFinished(currentAssign)){
            currentAssign->time += 1;

            if(isAssignFinished(currentAssign)){
                resolveAssign(currentAssign, cult);
            }
        }
        currentAssign = currentAssign->next;
    }

    GROUP *currentMember = cult->members;
    while (currentMember != NULL){
        if(currentMember->member != NULL){
            cult->funds += currentMember->member->impact.fund;
        }
        currentMember = currentMember->next;
    }
    
    showEventPopup("Nouveau jour", "Les PA ont été réinitialisés.");
}

// ---------- HANDLE ACTION ----------
void handleAction(int action, CULT *cult, GAME_CONF *conf){
    switch(action){
        case 0: 
            handleRecruitment(cult, conf); 
            break;
        case 1: 
            handleActivity(cult, conf); 
            break;
        case 2: 
            handleNextDay(cult); 
            runEvent(cult, conf);
            break;
    }
}

// ---------- SELECT GAME MODE ----------
GAME_MODE selectGameMode(){

    char *choices[]={
        "Partie normale",
        "Partie rapide",
        "Quitter"
    };

    int count=3;
    int index=0;

    while(1){

        clear();

        mvprintw(5,(COLS-20)/2,"CHOISIR MODE DE JEU");

        for(int i=0;i<count;i++){

            if(i==index) attron(A_REVERSE);

            mvprintw(8+i,(COLS-20)/2,"%s",choices[i]);

            if(i==index) attroff(A_REVERSE);
        }

        refresh();

        int ch=getch();

        if(ch==KEY_UP){
            index--;
            if(index<0) index=0;
        }

        if(ch==KEY_DOWN){
            index++;
            if(index>=count) index=count-1;
        }

        if(ch=='\n'){
            if(index==0) return GAME_NORMAL;
            if(index==1) return GAME_FAST;
            return -1;
        }
    }
}

// ---------- START GAME ----------
void startGame(GAME_CONF *conf){

    GAME_MODE mode = selectGameMode();

    if(mode == -1)
        return;

    CULT *cult = initCultUI();

    int maxRounds = 0;

    if(mode == GAME_FAST)
        maxRounds = NB_ROUNDS_FAST_GAME;

    runGameUI(cult,conf,mode,maxRounds);
}


// ---------- GAME LOOP ----------
void runGameUI(CULT *cult, GAME_CONF *conf, GAME_MODE mode, int maxRounds){
    UI ui;
    ui.selectedAction = 0;
    ui.memberOffset = 0;

    
    cult->pa = NB_PA; // PA initial
    initUI();
    //showStartScreen();
    createWindows(&ui);

    while(1){        
        updateScreen(&ui, cult);

        // ---- CONDITION VICTOIRE ----
        if(mode == GAME_NORMAL){
            if(getLegitimity(cult) >= MAX_LEGITIMITY){
                showVictoryScreen(MAX_LEGITIMITY);
                break;
            }
        }

        // ---- CONDITION FIN FAST GAME ----
        if(mode == GAME_FAST){
            if(cult->elapsedTime >= maxRounds){
                
                char buffer[128];
                snprintf(buffer,128,"Score final : %.2f",getLegitimity(cult));

                showEndScreen(getLegitimity(cult));
                break;
            }
        }
        
        int ch = getch();
        if(ch == 27) break; // ESC

        switch(ch){
            case KEY_UP:
                ui.selectedAction--;
                if(ui.selectedAction < 0) ui.selectedAction = 0;
                break;
            case KEY_DOWN:
                ui.selectedAction++;
                if(ui.selectedAction >= actionCount)
                    ui.selectedAction = actionCount-1;
                break;
            case KEY_NPAGE:
                ui.memberOffset += 8;
                break;
            case KEY_PPAGE:
                ui.memberOffset -= 8;
                if(ui.memberOffset < 0) ui.memberOffset = 0;
                break;
            case '\n':
                handleAction(ui.selectedAction, cult, conf);
                break;
        }
    }
    endUI();
}

void runEvent(CULT *cult, GAME_CONF *conf){
    EVENT *event = getEvent(cult, conf);
    if(event != NULL){
        showEventPopup(event->name, event->description);
        applyEventImpact(cult, event);
    } 
}


// ---------- UPDATE SCREEN ----------
void updateScreen(UI *ui, CULT *cult){
    clear();   // nettoie l'écran
    refresh();

    drawLayout(ui);
    drawStatus(ui, cult);
    drawActions(ui);
    drawMembers(ui, cult);
    drawAssigns(ui, cult);
}
