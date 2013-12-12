#include <iostream>
#include "svmplayer.h"
#include "svm_assert.h"

using namespace svmp;

int main ( int argc, char **argv ) {

    SVMPlayer player;
    SVM_ASSERT_MSG ( argc > 1, "To few arguments." );
    player.play ( argv[1] );
    //player.play ( "sop://broker.sopcast.com:3912/139907" );
    return 0;
}
