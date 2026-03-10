#include <stdio.h>
#include "cult.h"

int main(){

    srand(time(NULL));

    CULT *cult = initCult();
    GAME_CONF *conf = initConf();
    //showGroup(conf->allPeoples);

    runGameUI(cult, conf);

    return 0;
}