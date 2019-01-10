
#include "physicalmodel.hpp"
#include <SDL_draw.h>
#include <windows.h>
#include <algorithm>

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
    if(m_vertices.size() < 2)
    {
        return;
    }

    // Draw sides
    curr = m_vertices.begin();
    next = m_vertices.begin();
    next++;
    while(next != m_vertices.end())
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
void SquashPolygon(PhysicalModel *poly, Vector2D squashaxis, real *max, real *min, SDL_Surface *screen)
{
    bool trigger = true;               // guarantees, that valid value is stored in ret

    // normalize axis which has been passed
    Vector2D normaxis = squashaxis;
    normaxis.Normalize();

    for(int i = 0; i < poly->GetNumVertices(); i++)
    {
        // calculate world coordinates
        Vector2D vertexlocal = poly->GetVertex(i);
        real wx = poly->GetX() + vertexlocal.GetX();
        real wy = poly->GetY() + vertexlocal.GetY();
        Vector2D vertexworld(wx, wy);

        // squash vertex onto subspace
        real squash = normaxis*vertexworld;

        // store value if necessary
        if(trigger)         // store first vertex automatically
        {
            *max = squash;
            *min = squash;
            trigger = false;
        }
        else
        {
            if(squash > *max)
                *max = squash;
            if(squash < *min)
                *min = squash;
        }

        // temp: draw squashed vertex
/*        Vector2D drawsquash = normaxis;
        drawsquash.Stretch(squash);
        Draw_Circle(screen, (int)drawsquash.GetX(), (int)drawsquash.GetY(), 5, 100000);*/
    }

}

bool PhysicalModel::CollDet(PhysicalModel *rhs, SDL_Surface *screen)
{
    // Stop if not enough vertices have been pushed yet
    if(this->GetNumVertices() < 2 or rhs->GetNumVertices() < 2)
    {
        return false;
    }

    // Find subspace of *this's edges
    Vector2D last, curr, diff, perp;    // last vertex, current vertex, difference, perpendicular vector
    Vector2D drawdiff, drawperp;
    last = this->GetVertex(0);

/*
    for(int i = 1; i < this->GetNumVertices(); i++)
    {
        curr = this->GetVertex(i);
        diff = curr - last;
        perp.SetX(-diff.GetY());
        perp.SetY(diff.GetX());
        drawdiff = diff;
        drawperp = perp;
        drawdiff.Stretch(50.0);
        drawperp.Stretch(50.0);

        // Draw perp and diff
        Draw_Line(screen, (int)GetX(), (int)GetY(), (int)GetX() + (int)drawdiff.GetX(),
            (int) GetY() + (int)drawdiff.GetY(), 200000);
        Draw_Line(screen, (int)GetX(), (int)GetY(), (int)GetX() + (int)drawperp.GetX(),
            (int) GetY() + (int)drawperp.GetY(), 100000);

        last = curr;
    }


    // Handle closing side
    curr = this->GetVertex(0);
    diff = curr - last;
    perp.SetX(-diff.GetY());
    perp.SetY(diff.GetX());
    perp.Normalize();
    diff.Normalize();
    drawdiff = diff;
    drawdiff.Stretch(50.0);
    drawperp = perp;
    drawperp.Stretch(50.0);

    Draw_Line(screen, (int)GetX(), (int)GetY(), (int)GetX() + (int)drawdiff.GetX(),
        (int) GetY() + (int)drawdiff.GetY(), 200000);
    Draw_Line(screen, (int)GetX(), (int)GetY(), (int)GetX() + (int)drawperp.GetX(),
        (int) GetY() + (int)drawperp.GetY(), 100000);
*/

    bool match = false;
    for(int i = 1; i < this->GetNumVertices(); i++)
    {
        curr = this->GetVertex(i);
        diff = curr - last;
        perp.SetX(-diff.GetY());
        perp.SetY(diff.GetX());
        perp.Normalize();
        diff.Normalize();
        drawdiff = diff;
        drawdiff.Stretch(50.0);
        drawperp = perp;
        drawperp.Stretch(50.0);

    /*    Draw_Line(screen, (int)GetX(), (int)GetY(), (int)GetX() + (int)drawdiff.GetX(),
            (int) GetY() + (int)drawdiff.GetY(), 200000);
        Draw_Line(screen, (int)GetX(), (int)GetY(), (int)GetX() + (int)drawperp.GetX(),
            (int) GetY() + (int)drawperp.GetY(), 100000);*/

        real thismax = 0, thismin = 0;
        real rhsmax = 0, rhsmin = 0;
        SquashPolygon(this, perp, &thismax, &thismin, screen);
        SquashPolygon(rhs, perp, &rhsmax, &rhsmin, screen);


        // intervals overlap, meaning that there's no sep. axis yet
        if((thismax > rhsmin and thismax < rhsmax) or (thismin > rhsmin and thismin < rhsmax))
        {
            //Draw_Line(screen, 0, 10, 100, 10, 200000);

        }
        // found a separating axis
        else
        {
            Draw_Line(screen, 0, 10, 100, 10, 200000);
            match = true;
            break;
        }

        last = curr;

    }

    curr = this->GetVertex(0);
    perp.SetX(-diff.GetY());
    perp.SetY(diff.GetX());
    perp.Normalize();
    diff.Normalize();
    drawdiff = diff;
    drawdiff.Stretch(50.0);
    drawperp = perp;
    drawperp.Stretch(50.0);

    real thismax = 0, thismin = 0;
    real rhsmax = 0, rhsmin = 0;
    SquashPolygon(this, perp, &thismax, &thismin, screen);
    SquashPolygon(rhs, perp, &rhsmax, &rhsmin, screen);

    // intervals overlap, meaning that there's no sep. axis yet
    if((thismax > rhsmin and thismax < rhsmax) or (thismin > rhsmin and thismin < rhsmax))
    {
        //Draw_Line(screen, 0, 10, 100, 10, 200000);

    }
    // found a separating axis
    else
    {
        Draw_Line(screen, 0, 10, 100, 10, 200000);
        match = true;
    }

    return false;

}

