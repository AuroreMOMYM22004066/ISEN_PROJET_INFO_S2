// display.c
#include "cult.h"
#include <ncurses.h>
#include <string.h>


// actions principales
char *actions[] = {
    "Recruter un membre",
    "Assigner activite"
};
int actionCount = 2;


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

    int h_status = 5;
    int h_actions = 10;
    int h_members = 10;
    int h_log = 5;

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
    mvwprintw(ui->logWin, 0, 2, " LOG / EVENTS ");

    wrefresh(ui->statusWin);
    wrefresh(ui->actionWin);
    wrefresh(ui->memberWin);
    wrefresh(ui->logWin);
}


// ---------- DRAW STATUS ----------
void drawStatus(UI *ui, CULT *cult) {

    werase(ui->statusWin);
    box(ui->statusWin,0,0);

    mvwprintw(ui->statusWin,1,2,"Legitimite : %.2f", getLegitimity(cult));
    mvwprintw(ui->statusWin,2,2,"Visibilite : %.2f", getVisibility(cult));
    mvwprintw(ui->statusWin,3,2,"Controle : %.2f", getControl(cult));

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

    int y = 1;
    int idx = 0;

    GROUP *current = members;

    while(current != NULL && y < 9) {

        if(idx >= ui->memberOffset) {

            mvwprintw(ui->memberWin,y,2,"%s Ctrl: %.2f Etat: %s",
                current->member->name,
                current->member->control,
                isMemberAvailable(current->member, cult->assigns)?"Libre":"Occupe");

            y++;
        }

        current = current->next;
        idx++;
    }

    if(current != NULL)
        mvwprintw(ui->memberWin,y,2,"▼");

    wrefresh(ui->memberWin);
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

        // Parcourir la liste pour afficher
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
            
            // Parcourir la liste pour retourner le membre sélectionné
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


// ---------- HANDLE ACTION ----------
void handleAction(int action, CULT *cult, GAME_CONF *conf){

    switch(action){
        case 0: {

            GROUP *spawnList = generateSpawnMember(conf);

            if(spawnList == NULL){
                showEventPopup("Recrutement", "Personne ne souhaite rejoindre votre culte.");
                return;
            }

            int recruiting = 1;

            while(recruiting){

                MEMBER *selected = selectMemberPopup(spawnList);

                if(selected == NULL)  // joueur a choisi "ne plus recruter"
                    break;

                int confirm = confirmPopup("Recruter ce membre ?");

                if(confirm){

                    MEMBER *newMember = cloneMember(selected);

                    cult->members = addMemberToGroup(cult->members, newMember);

                    showEventPopup("Succes", "Le membre rejoint votre culte !");

                    recruiting = 0; // on sort après recrutement
                }
            }

            break;
        }
        
        case 1:
            showEventPopup("Activite", "Systeme d'activite pas encore implemente.");
            break;
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
}


// ---------- GAME LOOP ----------
void runGameUI(CULT *cult, GAME_CONF *conf){

    UI ui;

    ui.selectedAction = 0;
    ui.memberOffset = 0;

    initUI();
    createWindows(&ui);

    while(1){

        updateScreen(&ui, cult);

        int ch = getch();

        if(ch == 27)
            break;

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