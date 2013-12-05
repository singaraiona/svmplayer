#include <iostream>
#include "svmplayer.h"

int main ( int argc, char **argv ) {

    svmp::SVMPlayer player;
    player.play ( "http://localhost:8908" );
    return 0;
}
