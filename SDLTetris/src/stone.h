
#ifndef STONE_H
#define STONE_H


#include "sprite.h"
#include "timer.h"
#include <SDL.h>
#include <vector>

using namespace std;


enum color{lblue, dblue, orange, yellow, green, magenta, red};
enum stonetype{I, J, L, O, S, T, Z};



// **********************
// Class: Segment
// **********************

class Segment               // Portion of a stone
{
public:
    Segment(color type, int x, int y);   // (x, y) are the natural-number coordinates in 'segment-space'
    void Draw(SDL_Surface *screen, int x, int y);
    // GET
    int GetX();
    int GetY();
    // SET
    void SetX(int x);
    void SetY(int y);
private:
    SPRITE *m_sprite;
    color m_color;
    int m_x, m_y;
};

// **********************
// Class: Stone
// **********************

class Stone
{
public:
    Stone(stonetype type, int x, int y);
    void Draw(SDL_Surface *screen, int x, int y);
    bool CollDet(Stone &stone);                 // Checks for collision between this and &stone
    // GET
    bool HasSegmentAt(int x, int y);            // Check if there is a segment at (x,y) for this stone
    int GetX();
    int GetY();
    int GetNumSegments();
    Segment* GetSegment(int n);
//    Segment SetSegmentX(int n, int x);
//    Segment SetSegmentY(int n, int y);
    stonetype GetType();
    // SET
    void SetX(int x);
    void SetY(int y);
    void DeleteSegment(int x, int y);           // Delete segment at position (x,y) (board coordinates, not local ones !!!)
                                                // should give back true, if last segment of this stone has been deleted !!!
    void MoveDownSegment(int x, int y);         // Move down segment at position (x,y) by one (board coordinates, not local ones !!!)
private:
    vector<Segment> m_segments;
    stonetype m_type;
    int m_x, m_y;
};


#endif
