
#ifndef GAME_H
#define GAME_H

#include "sprite.h"
#include "stone.h"
#include "timer.h"
#include "sprite.h"
#include <SDL.h>
#include <map>
#include <string>

using namespace std;

void StoneSpriteInit(void);

// **********************
// Class: Game
// **********************

class Game
{
public:
    Game(int winx,int winy);
    void Run();
    void Reset();
    static SPRITE* GetSprite(string name);
    bool MoveDown(int delta);                           // false, if movement impossible
    bool MoveLeft();
    bool MoveRight();
    bool Rotate();                                      // See rotate stone
    bool CollisionDet();
    void DestroyFullLines();                            // Check for any full lines and delete them
    bool CheckIfDead();                                 // Gives back true if no stone can be inserted anymore
    // GET
    bool HasSegmentAt(int x, int y);                    // Check if there is a segment at (x,y)
    static int GetSegmentWidth();
    static int GetSegmentHeight();
    static int GetWinX();
    static int GetWinY();
    static int GetNSegmentsX();
    static int GetNSegmentsY();
    // SET
    void DeleteSegment(int x, int y);                   // Delete segment at position (x,y)
    void MoveDownSegment(int x, int y);                 // Move down segment at position (x,y) by one
private:
    void Update();
    bool RotateStone(bool ccw);                         // Performs actual rotation in function Rotate() (ccw == true -> ccw rotation
                                                        //                                                  ccw == false ->cw rotation)
    void StoneSpriteInit();
    void SpawnNewStone();
    vector<Stone> m_stones;
    Stone *m_curr_stone;
    static int m_nsegments_x;
    static int m_nsegments_y;
    static map<string, SPRITE*> m_stonesprites;
    Timer m_timer;
    SDL_Surface *m_screen;
    static int m_winx, m_winy;
    static const int m_segment_width, m_segment_height;
    static int numstonetypes;
};


#endif
