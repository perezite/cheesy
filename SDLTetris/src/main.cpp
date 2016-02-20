#include <stdlib.h>
#include <math.h>
#include "dxinit.h"
#include "sprite.h"
#include "stone.h"
#include "timer.h"
#include "game.h"
#include <iostream>
#include <fstream>
using namespace std;


int main ( int argc, char** argv )
{


    Game game(200, 400);

    game.Run();

    printf("Exited cleanly\n");
    return 0;
}
