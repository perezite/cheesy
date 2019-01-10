#include <stdlib.h>
#include "dxinit.h"
#include "font.h"
#include "physicalmodel.hpp"
#include <SDL_draw.h>
#include <iostream>
#include <fstream>
using std::cout;
using std::cin;

Uint8 ColorkeyR = 255;
Uint8 ColorkeyG = 0;
Uint8 ColorkeyB = 255;

SDL_Surface *screen = InitFullscreenApp(640, 480, 32);

int main ( int argc, char** argv )
{
    bool done = false;

    // Create Model
    PhysicalModel polygon1(210.0, 200.0);
    polygon1.PushVertex(0, -50);
    polygon1.PushVertex(-50, 50);
    polygon1.PushVertex(50, 50);
    PhysicalModel polygon2(350, 173);
    polygon2.PushVertex(0, 0);
    polygon2.PushVertex(0, 100);
    polygon2.PushVertex(100, 50);
    polygon2.PushVertex(50, 0);

    while (!done)
    {
        // message processing loop
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            // check for messages
            switch (event.type)
            {
                // exit if the window is closed
                case SDL_QUIT:
                    done = true;
                    break;

                    // check for keypresses
                case SDL_KEYDOWN:
                {
                    // exit if ESCAPE is pressed
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                        done = true;
                    break;
                }

            } // end switch
        } // end of message processing

        // DRAWING STARTS HERE

        // clear screen
        ClearScreen();

        // INPUT
        real sx = polygon1.GetX();
        real sy = polygon1.GetY();

        if(GetAsyncKeyState(VK_LEFT))
            sx -= 1.0;
        if(GetAsyncKeyState(VK_RIGHT))
            sx += 1.0;
        if(GetAsyncKeyState(VK_UP))
            sy -= 1.0;
        if(GetAsyncKeyState(VK_DOWN))
            sy += 1.0;
        if(GetAsyncKeyState(VK_RETURN))
        {

        }

        polygon1.SetX(sx);
        polygon1.SetY(sy);

        // Draw Polygon
        polygon1.Draw(screen);
        polygon2.Draw(screen);
        if(polygon1.CollDet(&polygon2, screen))
        {
            Draw_Line(screen, 0, 10, 100, 10, 200000);
        }

        // DRAWING ENDS HERE

        // finally, update the screen :)
        FlipScreen();
    } // end main loop

    // free loaded bitmap


    // all is well ;)
    printf("Exited cleanly\n");
    return 0;
}
