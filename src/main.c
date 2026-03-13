#include <stdio.h>
#include "cult.h"

int main(){

    srand(time(NULL));

    GAME_CONF *conf = initConf();

    initUI();

    showStartScreen();

    startGame(conf);

    endUI();

    return 0;
}