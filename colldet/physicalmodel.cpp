
#include "physicalmodel.hpp"
#include <SDL_draw.h>
#include <windows.h>
#include <algorithm>

// Helper function:
// Draw straight line for test purposes
// axis: vector giving direction len: length of line to be drawn
// origin: pos. of origin
void DrawLine(Vector2D axis, Vector2D origin, real len, SDL_Surface *screen)
{
    axis.Normalize();
    Vector2D beg = origin - axis*(len/2.0);
    Vector2D end = origin + axis*(len/2.0);

    Draw_Line(screen, (int)beg.GetX(), (int)beg.GetY(), (int)end.GetX(), (int)end.GetY(), 100000);
}

// Helper function
void DrawCircle(int x, int y, int rad, SDL_Surface *screen)
{
    if (x <= rad or x >= (640-rad) or y <= rad or y >= (480-rad))
        return;
    else
        Draw_Circle(screen, x, y, 5, 100000);
}

PhysicalModel::PhysicalModel(real x, real y)
{
    m_x = x;
    m_y = y;
}

void PhysicalModel::PushVertex(real x, real y)
{
    Vector2D NewVertex(x, y);
    m_vertices.push_back(NewVertex);
}

void PhysicalModel::Draw(SDL_Surface *screen)
{
    vector<Vector2D>::iterator curr;
    vector<Vector2D>::iterator next;

    // Stop if not enough vertices have been pushed yet
    if (m_vertices.size() < 2)
    {
        return;
    }

    // Draw sides
    curr = m_vertices.begin();
    next = m_vertices.begin();
    next++;
    while (next != m_vertices.end())
    {
        Draw_Line(screen,
                  (integer)m_x + curr->GetXAsInteger(), (integer)m_y + curr->GetYAsInteger(),
                  (integer)m_x + next->GetXAsInteger(),  (integer)m_y + next->GetYAsInteger(), 150000);

        curr++;
        next++;

    }

    // Draw closing side
    next = curr;                            // next points now to last elem.
    curr = m_vertices.begin();
    Draw_Line(screen,
              (integer)m_x + next->GetXAsInteger(), (integer)m_y + next->GetYAsInteger(),
              (integer)m_x + curr->GetXAsInteger(), (integer)m_y + curr->GetYAsInteger(), 150000);

}

// Helper function:
// squash all vertices of polygon onto squashaxis
// squashaxis doesn't have to be normalized
// stores range of polygons into &max and &min
void SquashPolygon(PhysicalModel *poly, Vector2D squashaxis, real *max, real *min, SDL_Surface *screen, bool draw)
{
    bool trigger = true;               // guarantees, that valid value is stored in ret

    // normalize axis which has been passed
    Vector2D normaxis = squashaxis;
    normaxis.Normalize();

    for (int i = 0; i < poly->GetNumVertices(); i++)
    {
        // calculate world coordinates
        Vector2D vertexlocal = poly->GetVertex(i);
        real wx = poly->GetX() + vertexlocal.GetX();
        real wy = poly->GetY() + vertexlocal.GetY();
        Vector2D vertexworld(wx, wy);

        // squash vertex onto subspace
        real squash = normaxis*vertexworld;

        // store value if necessary
        if (trigger)        // store first vertex automatically
        {
            *max = squash;
            *min = squash;
            trigger = false;
        }
        else
        {
            if (squash > *max)
                *max = squash;
            if (squash < *min)
                *min = squash;
        }

        // temp: draw squashed vertex
        Vector2D drawsquash = normaxis;
        drawsquash.Stretch(squash);
        if (draw)
        {
            DrawCircle((int)drawsquash.GetX(), (int)drawsquash.GetY(), 5, screen);
        }
    }

}

bool IsSeperatingAxis(Vector2D axis, PhysicalModel *poly1, PhysicalModel *poly2, SDL_Surface *screen, bool draw)
{
    Vector2D perp;
    perp.SetX(-axis.GetY());
    perp.SetY(axis.GetX());
    axis.Normalize();
    perp.Normalize();

    real max1 = 0, min1 = 0;
    real max2 = 0, min2 = 0;
    SquashPolygon(poly1, perp, &max1, &min1, screen, draw);
    SquashPolygon(poly2, perp, &max2, &min2, screen, draw);

    if (draw)
    {
        char buf[128];
        sprintf(buf, "%f %f %f %f", min1, max1, min2, max2);
//        MessageBox(NULL, buf, "", NULL);
    }

    if ( (max1 > min2 and max1 < max2) or (min1 > min2 and min1 < max2) or (min2 < max1 and min2 > min1) )
    {
        return false;
    }
    else
    {
        return true;
    }
}

// Note: this is a function in debug-fashion
// delete the collision variable and return value directly
// (such that not all axes are checked)
// delete the DrawLine calls
bool PhysicalModel::CollDet(PhysicalModel *rhs, SDL_Surface *screen)
{
    Vector2D axis;
    bool collision = true;
    int len = 30;

    // Stop if not enough vertices have been pushed yet
    if (this->GetNumVertices() < 2 or rhs->GetNumVertices() < 2)
    {
        return false;
    }

    // Check *this for separating axes
    for (int i = 1; i < this->GetNumVertices(); i++)
    {
        axis = this->GetVertex(i - 1) - this->GetVertex(i);
        if (IsSeperatingAxis(axis, this, rhs, screen, false))
        {
            DrawLine(axis, this->GetPos(), len, screen);
            collision = false;
        }
    }
    // Check last side of *this if it's a sep. axis
    axis = this->GetVertex(0) - this->GetVertex(this->GetNumVertices() - 1);
    if (IsSeperatingAxis(axis, this, rhs, screen, false))
    {
        DrawLine(axis, this->GetPos(), len, screen);
        collision = false;
    }

    // Check *rhs for separating axes
    for (int i = 1; i < rhs->GetNumVertices(); i++)
    {
        axis = rhs->GetVertex(i - 1) - rhs->GetVertex(i);
        if (IsSeperatingAxis(axis, this, rhs, screen, false))
        {
            DrawLine(axis, rhs->GetPos(), len, screen);
            collision = false;
        }
    }

    // Check last side of *rhs whether it is a sep. axis
    axis = rhs->GetVertex(0) - rhs->GetVertex(rhs->GetNumVertices() - 1);
    if (IsSeperatingAxis(axis, this, rhs, screen, false))
    {
        DrawLine(axis, rhs->GetPos(), len, screen);
        collision = false;
    }

    Vector2D orig(320, 240);
    axis = rhs->GetVertex(2) - rhs->GetVertex(1);
    if (GetAsyncKeyState(VK_RETURN))
    {
        bool ret = IsSeperatingAxis(axis, this, rhs, screen, false);
    }

    return collision;

}


