
#ifndef __PHYSICALMODEL_H_
#define __PHYSICALMODEL_H_

#include "physmath.hpp"
#include <vector>
#include <SDL.h>

using namespace std;

//***********************
// Class: PhysicalModel

class PhysicalModel
{
public:
    PhysicalModel() {};
    PhysicalModel(real x, real y);

    void PushVertex(real x, real y);    // x, y in local coords.
    void Draw(SDL_Surface *screen);
    bool CollDet(PhysicalModel *rhs, SDL_Surface *screen);

    real GetX() {return m_x;}
    real GetY() {return m_y;}
    Vector2D GetPos()
    {
        Vector2D ret(m_x, m_y);
        return ret;
    }
    Vector2D GetVertex(int n) {return m_vertices.at(n);}
    int GetNumVertices() {return m_vertices.size();}

    void SetX(real x) {m_x = x;}
    void SetY(real y) {m_y = y;}
private:
    vector<Vector2D> m_vertices;        // in local coordinates
    real m_x, m_y;                      // center position
};

#endif
