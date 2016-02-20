
#include "game.h"
#include "stone.h"
#include <math.h>


// **********************
// Static initializations
// **********************

const int Game::m_segment_width = 24;
const int Game::m_segment_height = 24;
int Game::numstonetypes = 7;
int Game::m_winx;
int Game::m_winy;
int Game::m_nsegments_x;
int Game::m_nsegments_y;
map<string, SPRITE*> Game::m_stonesprites;

// **********************
// Class: Game
// **********************

Game::Game(int winx, int winy)
    : m_timer(400)
{
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
    srand(GetTickCount());
    StoneSpriteInit();
    m_screen = InitFullscreenApp(winx, winy, 32);
    m_nsegments_x = int(floor(double(winx / m_segment_width)));
    m_nsegments_y = int (floor(double(winy / m_segment_height)));
    Stone *firststone = new Stone((stonetype)(rand()%numstonetypes), (int)(m_nsegments_x/2.0), 0);
    m_curr_stone = firststone;
    m_winx = winx;
    m_winy = winy;
    m_timer.Reset();
}

void Game::Reset()
{

}

void Game::Run()
{
    bool done = false;

    while (!done)
    {
        // *** Keyboard input handling ***
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                // done = true;
                break;

            case SDL_KEYDOWN:
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_LEFT:
                    MoveLeft();
                    break;
                case SDLK_RIGHT:
                    MoveRight();
                    break;
                case SDLK_UP:
                    Rotate();
                    break;
                case SDLK_DOWN:
                    MoveDown(1);
                    break;
                default:
                    break;
                }
                break;
            }

            case SDL_MOUSEBUTTONDOWN:
            {
                switch (event.button.button)
                {
                case SDL_BUTTON_LEFT:
                case SDL_BUTTON_RIGHT:
                    break;
                default:
                    break;
                }
                break;
            }

            default:
            {
                break;
            }
            } // end switch
        } // end of message processing

        // *** Render ***
        Update();
    }
}

bool Game::MoveDown(int n)
{
    int oldy = m_curr_stone->GetY();
    int newy = m_curr_stone->GetY() + n;

    // *** Check for all segments whether move is legal ***
    bool pass = true;

    for(int i = 0; i < m_curr_stone->GetNumSegments(); i++)
    {
        Segment *seg = m_curr_stone->GetSegment(i);

        // Check if segment is below ground
        int segnewy = newy + seg->GetY();
        if(segnewy >= Game::GetNSegmentsY())
        {
            pass = false;
            break;
        }
    }

    // If all checks passed, move is legal
    if(pass == true)
    {
        m_curr_stone->SetY(newy);

        if(CollisionDet() == true)
        {
            m_curr_stone->SetY(oldy);
            SpawnNewStone();
        }
    }


    return pass;
}

bool Game::MoveLeft()
{
    int oldx = m_curr_stone->GetX();
    int newx = m_curr_stone->GetX() - 1;

    // *** Check for all segments whether move is legal ***
    bool pass = true;

    for(int i = 0; i < m_curr_stone->GetNumSegments(); i++)
    {
        Segment *seg = m_curr_stone->GetSegment(i);

        // Check if segment is beyond left boundary
        int segnewx = newx + seg->GetX();
        if(segnewx < 0)
        {
            pass = false;
            break;
        }
    }

    // If all checks passed, move is legal
    if(pass == true)
    {
        m_curr_stone->SetX(newx);

        if(CollisionDet() == true)
        {
            m_curr_stone->SetX(oldx);
        }
    }

    return pass;
}

bool Game::MoveRight()
{
    int oldx = m_curr_stone->GetX();
    int newx = m_curr_stone->GetX() + 1;

    // *** Check for all segments whether move is legal ***
    bool pass = true;

    for(int i = 0; i < m_curr_stone->GetNumSegments(); i++)
    {
        Segment *seg = m_curr_stone->GetSegment(i);

        // Check if segment is beyond right boundary
        int segnewx = newx + seg->GetX();
        if(segnewx >= Game::GetNSegmentsX())
        {
            pass = false;
            break;
        }
    }

    // If all checks passed, move is legal
    if(pass == true)
    {
        m_curr_stone->SetX(newx);

        if(CollisionDet() == true)
        {
            m_curr_stone->SetX(oldx);
        }
    }

    return pass;
}

bool Game::RotateStone(bool ccw)
{
    if(m_curr_stone->GetType() == O)
    {
        return true;
    }

    for(int i = 0; i < m_curr_stone->GetNumSegments(); i++)
    {
        Segment *seg = m_curr_stone->GetSegment(i);
        int x = seg->GetX(), y = seg->GetY();

        if(ccw == true)         // Rotate counterclockwise
        {
            seg->SetX(y);
            seg->SetY(-x);
        }
        else                    // rotate clockwise
        {
            seg->SetX(-y);
            seg->SetY(x);
        }
    }

    return true;
}

bool Game::Rotate()
{
    bool ccw = true;
    bool cw = false;
    this->RotateStone(ccw);

    if(CollisionDet() == true)
    {
        this->RotateStone(cw);
    }

    return true;
}

bool Game::CollisionDet()
{
    // Check for collision against all other stones
    vector<Stone>::iterator it;
    for(it = m_stones.begin(); it != m_stones.end(); it++)
    {
        bool res = m_curr_stone->CollDet((*it));

        if(res == true)
        {
            return true;
        }
    }

    // Check if any segment is out of bounds
    for(int i = 0; i < m_curr_stone->GetNumSegments(); i++)
    {
        Segment *seg = m_curr_stone->GetSegment(i);

        int segx = m_curr_stone->GetX() + seg->GetX();
        int segy = m_curr_stone->GetY() + seg->GetY();

        if(segx >= Game::GetNSegmentsX() or segx < 0 or segy >= Game::GetNSegmentsY() or segy < 0)
        {
            return true;
        }
    }


    return false;

}

void Game::Update()
{
    bool falldown_result = true;

    // *** Timer event handling ***
    int n = m_timer.CheckTick();
    if(n > 0)
    {
        falldown_result = MoveDown(n);
    }

    // *** Render ***
    ClearScreen();
    vector<Stone>::iterator it;

    for(it = m_stones.begin(); it != m_stones.end(); it++)
    {
        int x = it->GetX()*m_segment_width;             // Stone coordinates in window space
        int y = it->GetY()*m_segment_height;            // Stone coordinates in window space
        it->Draw(m_screen, x, y);
    }

    int x = m_curr_stone->GetX()*m_segment_width;       // Stone coordinates in window space
    int y = m_curr_stone->GetY()*m_segment_height;      // Stone coordinates in window space
    m_curr_stone->Draw(m_screen, x, y);

    // *** Create new stone if necessary ***
    if (falldown_result == false)
    {
        SpawnNewStone();
    }

    // *** Flip ***
    FlipScreen();
}

void Game::StoneSpriteInit(void)
{
    SPRITE *sprite1 = new SPRITE;
    CreateSprite(sprite1, "graphics\\dblue.bmp", 1, m_segment_width, m_segment_height, 0);
    m_stonesprites.insert(pair<string, SPRITE*>("dblue", sprite1));

    SPRITE *sprite2 = new SPRITE;
    CreateSprite(sprite2, "graphics\\lblue.bmp", 1, m_segment_width, m_segment_height, 0);
    m_stonesprites.insert(pair<string, SPRITE*>("lblue", sprite2));

    SPRITE *sprite3 = new SPRITE;
    CreateSprite(sprite3, "graphics\\orange.bmp", 1, m_segment_width, m_segment_height, 0);
    m_stonesprites.insert(pair<string, SPRITE*>("orange", sprite3));

    SPRITE *sprite4 = new SPRITE;
    CreateSprite(sprite4, "graphics\\yellow.bmp", 1, m_segment_width, m_segment_height, 0);
    m_stonesprites.insert(pair<string, SPRITE*>("yellow", sprite4));

    SPRITE *sprite5 = new SPRITE;
    CreateSprite(sprite5, "graphics\\green.bmp", 1, m_segment_width, m_segment_height, 0);
    m_stonesprites.insert(pair<string, SPRITE*>("green", sprite5));

    SPRITE *sprite6 = new SPRITE;
    CreateSprite(sprite6, "graphics\\magenta.bmp", 1, m_segment_width, m_segment_height, 0);
    m_stonesprites.insert(pair<string, SPRITE*>("magenta", sprite6));

    SPRITE *sprite7 = new SPRITE;
    CreateSprite(sprite7, "graphics\\red.bmp", 1, m_segment_width, m_segment_height, 0);
    m_stonesprites.insert(pair<string, SPRITE*>("red", sprite7));

}

SPRITE* Game::GetSprite(string name)
{
    map<string, SPRITE*>::iterator it;

    it = m_stonesprites.find(name);

    if(it == m_stonesprites.end())
    {
        return NULL;
    }

    else
    {
        return it->second;
    }
}

bool Game::HasSegmentAt(int x, int y)
{
    vector<Stone>::iterator it;
    for(it = m_stones.begin(); it != m_stones.end(); it++)
    {
        if(it->HasSegmentAt(x, y) == true)
        {
            return true;
        }
    }

    return false;
}

void Game::DeleteSegment(int x, int y)
{
    vector<Stone>::iterator it;
    for(it = m_stones.begin(); it != m_stones.end(); it++)
    {
        if(it->HasSegmentAt(x, y) == true)
        {

            it->DeleteSegment(x, y);
        }
    }
}

void Game::MoveDownSegment(int x, int y)
{
    vector<Stone>::iterator it;
    for(it = m_stones.begin(); it != m_stones.end(); it++)
    {
        if(it->HasSegmentAt(x, y) == true)
        {
            it->MoveDownSegment(x, y);
        }
    }
}

int Game::GetSegmentWidth()
{
    return m_segment_width;
}

int Game::GetSegmentHeight()
{
    return m_segment_height;
}

int Game::GetWinX()
{
    return m_winx;
}

int Game::GetWinY()
{
    return m_winy;
}

int Game::GetNSegmentsX()
{
    return m_nsegments_x;
}

int Game::GetNSegmentsY()
{
    return m_nsegments_y;
}

void Game::SpawnNewStone()
{
    // Save old stone
    m_stones.push_back(*m_curr_stone);      // Copy data of m_curr_stone into m_stones..
    delete m_curr_stone;                    // .. so, m_curr_stone can be deleted

    // Handle full lines
    DestroyFullLines();

    // Create new one
    Stone *newstone = new Stone((stonetype)(rand()%numstonetypes), (int)(m_nsegments_x/2.0), 0);
    m_curr_stone = newstone;

    // Check if stone can't be inserted (which means game over)
    if(CheckIfDead() == true)
    {
        MessageBox(NULL, "Game Over", "", NULL);
    }
}

void Game::DestroyFullLines()
{
    // A board class would reduce the complexity of this function -.-

    for(int y = Game::GetNSegmentsY() - 1; y >= 0; y--)
    {
        bool full_line = true;

        for(int x = 0; x < Game::GetNSegmentsX(); x++)
        {
            if(HasSegmentAt(x, y) == true)
            {
                continue;
            }
            else
            {
                full_line = false;
                break;
            }
        }

        if(full_line == true)
        {
            // Delete full line
            for(int x = 0; x < Game::GetNSegmentsX(); x++)
            {
                DeleteSegment(x,y);
            }

            // Move down all segments which have been above the full line
            for(int k = y - 1; k >= 0; k--)                     // Must go through board this way, otherwise segments are lowered
                                                                // multiple times
            {
                for(int l = 0; l < Game::GetNSegmentsX(); l++)
                {
                    MoveDownSegment(l, k);
                }
            }

            // Maybe, the line just above the one considered here is a full line too, which is now at the y-coordinate
            // just checked, so recheck this y-coordinate
            y = y + 1;
        }

    }
}

bool Game::CheckIfDead()
{
    if(CollisionDet() == true)
    {
        return true;
    }
    else
    {
        return false;
    }
}
