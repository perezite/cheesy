

#include "stone.h"
#include "sprite.h"
#include "game.h"


// **********************
// Class: Segment
// **********************

Segment::Segment(color type, int x, int y)          // (x, y) are the natural-number coordinates in 'segment-space'
{
    m_color = type;
    m_x = x;
    m_y = y;

    switch(m_color)
    {
        case red:
            m_sprite = Game::GetSprite("red");
            break;
        case lblue:
            m_sprite = Game::GetSprite("lblue");
            break;
        case dblue:
            m_sprite = Game::GetSprite("dblue");
            break;
        case orange:
            m_sprite = Game::GetSprite("orange");
            break;
        case yellow:
            m_sprite = Game::GetSprite("yellow");
            break;
        case green:
            m_sprite = Game::GetSprite("green");
            break;
        case magenta:
            m_sprite = Game::GetSprite("magenta");
            break;
        default:
            m_sprite = NULL;
            break;
    }

}

void Segment::Draw(SDL_Surface *screen, int x, int y)
{
    MaleSprite(screen, m_sprite, x + m_x * Game::GetSegmentWidth(), y + m_y * Game::GetSegmentHeight(), 0, 0, false);
}

// GET

int Segment::GetX()
{
    return m_x;
}

int Segment::GetY()
{
    return m_y;
}

// SET

void Segment::SetX(int x)
{
    m_x = x;
}

void Segment::SetY(int y)
{
    m_y = y;
}

// **********************
// Class: Stone
// **********************

Stone::Stone(stonetype type, int x, int y)
{
    m_x = x, m_y = y;

    switch(type)
    {
        case O:
        {
            Segment seg1(yellow, 0, 0);
            Segment seg2(yellow, 1, 0);
            Segment seg3(yellow, 0, 1);
            Segment seg4(yellow, 1, 1);
            m_segments.push_back(seg1);
            m_segments.push_back(seg2);
            m_segments.push_back(seg3);
            m_segments.push_back(seg4);

            m_type = type;

            break;
        }
        case I:
        {
            Segment seg1(lblue, 0, 0);
            Segment seg2(lblue, -1, 0);
            Segment seg3(lblue, 1, 0);
            Segment seg4(lblue, 2, 0);
            m_segments.push_back(seg1);
            m_segments.push_back(seg2);
            m_segments.push_back(seg3);
            m_segments.push_back(seg4);

            m_type = type;

            break;
        }
        case J:
        {
            Segment seg1(dblue, 0, 0);
            Segment seg2(dblue, -1, 0);
            Segment seg3(dblue, 1, 0);
            Segment seg4(dblue, 1, 1);
            m_segments.push_back(seg1);
            m_segments.push_back(seg2);
            m_segments.push_back(seg3);
            m_segments.push_back(seg4);

            m_type = type;

            break;
        }
        case L:
        {
            Segment seg1(orange, 0, 0);
            Segment seg2(orange, -1, 0);
            Segment seg3(orange, 1, 0);
            Segment seg4(orange, -1, 1);
            m_segments.push_back(seg1);
            m_segments.push_back(seg2);
            m_segments.push_back(seg3);
            m_segments.push_back(seg4);

            m_type = type;

            break;
        }
        case S:
        {
            Segment seg1(green, 0, 0);
            Segment seg2(green, 1, 0);
            Segment seg3(green, -1, 1);
            Segment seg4(green, 0, 1);
            m_segments.push_back(seg1);
            m_segments.push_back(seg2);
            m_segments.push_back(seg3);
            m_segments.push_back(seg4);

            m_type = type;

            break;
        }
        case T:
        {
            Segment seg1(magenta, 0, 0);
            Segment seg2(magenta, -1, 0);
            Segment seg3(magenta, 1, 0);
            Segment seg4(magenta, 0, 1);
            m_segments.push_back(seg1);
            m_segments.push_back(seg2);
            m_segments.push_back(seg3);
            m_segments.push_back(seg4);

            m_type = type;

            break;
        }
        case Z:
        {
            Segment seg1(red, -1, 0);
            Segment seg2(red, 0, 0);
            Segment seg3(red, 0, 1);
            Segment seg4(red, 1, 1);
            m_segments.push_back(seg1);
            m_segments.push_back(seg2);
            m_segments.push_back(seg3);
            m_segments.push_back(seg4);

            m_type = type;

            break;
        }
        default:
        {
            break;
        }
    }
}

void Stone::Draw(SDL_Surface *screen, int x, int y)
{
    vector<Segment>::iterator it;

    for(it = m_segments.begin(); it != m_segments.end(); it++)
    {
        it->Draw(screen, x, y);
    }
}

bool Stone::CollDet(Stone &stone)
{
    vector<Segment>::iterator it;

    for(it = this->m_segments.begin(); it != this->m_segments.end(); it++)
    {
        for(int i = 0; i < stone.GetNumSegments(); i++)
        {
            Segment *seg2 = stone.GetSegment(i);
            int x1 = this->GetX() + it->GetX();
            int y1 = this->GetY() + it->GetY();
            int x2 = stone.GetX() + seg2->GetX();
            int y2 = stone.GetY() + seg2->GetY();

            if(x1 == x2 and y1 == y2)
            {
                return true;
            }
        }
    }

    return false;
}

// GET

bool Stone::HasSegmentAt(int x, int y)
{
    vector<Segment>::iterator it;

    for(it = this->m_segments.begin(); it != this->m_segments.end(); it++)
    {
        int segx = this->GetX() + it->GetX();
        int segy = this->GetY() + it->GetY();

        if(segx == x and segy == y)
        {
            return true;
        }
    }

    return false;
}

void Stone::DeleteSegment(int x, int y)
{
    vector<Segment>::iterator it;

    for(it = this->m_segments.begin(); it != this->m_segments.end(); it++)
    {
        int segx = this->GetX() + it->GetX();
        int segy = this->GetY() + it->GetY();

        if(segx == x and segy == y)
        {
            m_segments.erase(it);
            return;
        }
    }
}

void Stone::MoveDownSegment(int x, int y)
{
    vector<Segment>::iterator it;

    for(it = this->m_segments.begin(); it != this->m_segments.end(); it++)
    {
        int segx = this->GetX() + it->GetX();
        int segy = this->GetY() + it->GetY();

        if(segx == x and segy == y)
        {
            it->SetY(it->GetY() + 1);
            return;
        }
    }
}

int Stone::GetX()
{
    return m_x;
}

int Stone::GetY()
{
    return m_y;
}

int Stone::GetNumSegments()
{
    return m_segments.size();
}

Segment* Stone::GetSegment(int n)
{
    Segment *seg = &(m_segments.at(n));         // Throws exception automatically, if n out of bounds

    return seg;
}

/*
Segment Stone::SetSegmentX(int n, int x)
{

}

Segment Stone::SetSegmentY(int n, int y)
{

}
*/

stonetype Stone::GetType()
{
    return m_type;
}

// SET

void Stone::SetX(int x)
{
    m_x = x;
}

void Stone::SetY(int y)
{
    m_y = y;
}
