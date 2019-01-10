
#include <algorithm>
#include "font.h"

using namespace std;

// ************
// Class: Screen Text
// ************

// STATIC INITS
const int ScreenText::m_numtokens = 96;

// CTOR

ScreenText::ScreenText()
{
    int w = 16;
    int h = 16;
    string fontpath = "graphics\\font16x16.bmp";

    CreateSprite(&m_fontsprite, fontpath.c_str(), m_numtokens, w, h, 0);
    SDL_PixelFormat *Format = m_fontsprite.Bild->format;
    SDL_SetColorKey(m_fontsprite.Bild, SDL_SRCCOLORKEY, SDL_MapRGB(Format, 255, 0, 255));

    m_w = w;
    m_h = h;
    m_x = 0;
    m_y = 0;
    m_stringpath = fontpath;
    m_text = "";
}

ScreenText::ScreenText(string text, string fontpath, int x, int y, int w, int h)
{
    CreateSprite(&m_fontsprite, fontpath.c_str(), m_numtokens, w, h, 0);
    SDL_PixelFormat *Format = m_fontsprite.Bild->format;
    SDL_SetColorKey(m_fontsprite.Bild, SDL_SRCCOLORKEY, SDL_MapRGB(Format, 255, 0, 255));

    m_w = w;
    m_h = h;
    m_x = x;
    m_y = y;
    m_stringpath = fontpath;
    m_text = text;
    m_text = "";
}

// DTOR

ScreenText::~ScreenText()
{

}

// SET

void ScreenText::SetX(int x)
{
    m_x = x;
}

void ScreenText::SetY(int y)
{
    m_y = y;
}


void ScreenText::SetW(int w)
{
    m_w = w;
}

void ScreenText::SetH(int h)
{
    m_h = h;
}

void ScreenText::SetText(string text)
{
    m_text = text;
}

// GET

int ScreenText::GetW()
{
    return m_w;
}

int ScreenText::GetH()
{
    return m_h;
}

int ScreenText::GetX()
{
    return m_x;
}

int ScreenText::GetY()
{
    return m_y;
}

string ScreenText::GetText()
{
    return m_text;
}


// ************
// Class: StaticScreenText
// Derived From: ScreenText
// ************

// STATIC INITS

DWORD StaticScreenText::m_interval = 25;

void StaticScreenText::Draw(SDL_Surface *screen)
{
    char starttoken = ' ';
    string text = GetText();
    string promt = "(press enter)";
    int w = GetW();
    int h = GetH();
    int x = GetX();
    int y = GetY();
    int xpos = 0;
    int ypos = 0;

    // Update position according to timer
    m_stoppos += m_timer.CheckTick();
    if (m_stoppos > (text.size()))
    {
        m_stoppos = text.size();
    }

    // Draw text
    unsigned int i = m_startpos;                 // Declare this var here, because it is used after the for-loop
    for (; i < m_stoppos; i++)
    {
        // Handle line break
        if (text[i] == '\n')
        {
            ypos++;
            xpos = 0;
            m_lastpos = i;
            continue;
        }

        // Automatic word wrapping at new words
        // Note: don't care for text[i] == '\n' here.. this case is handled above and this code
        // here won't be reached then
        if (text[i] == ' ')
        {
            // find first critical token
            size_t nextspace = text.find(' ', i+1); // Is calling with i+1 secure ?!?
            size_t nextbreak = text.find('\n', i+1);
            if (nextspace == string::npos)
            {
                nextspace = text.size();            // Note: Indexing beginning by zero
            }
            if (nextbreak == string::npos)
            {
                nextbreak = text.size();            // Note: Indexing beginning by zero
            }
            int nextcrittoken = min(nextspace, nextbreak);
            // compute width and height of next word and of whole line
            int wordlen = nextcrittoken - i - 1;
            int wordwidth = wordlen * GetW();           // width of only the next word
            int width = (xpos + wordlen + 1) * GetW();  // width of the the tokens drawn up to now plus the next word
            int height = (ypos + 1 + 1) * GetH();
            // Handle case that the new word alone is broader than the whole clipping width (overlength)
            if (wordwidth >= m_w)
            {
                // Just go ahead and let the word be split up by the out of range handler below
            }
            // otherwise perform appropriate word wrapping
            else if (width >= m_w and m_w > 0)
            {
                if (height >= m_h and m_h > 0)
                {
                    // Rest of the text can't be drawn anymore in this case
                    m_lastpos = i;
                    m_state = INCOMPLETE;
                    break;
                }
                else
                {
                    ypos++;             // line break
                    xpos = 0;
                    continue;
                }
            }
        }

        // Handle tokens that are out of clipping area
        // Note: don't care for text[i] == '\n' here.. this case is handled above and this code
        // here won't be reached
        int width = (xpos+1)*GetW();
        int height = (ypos+1+1)*GetH();
        if (width >= m_w)
        {
            if (height >= m_h)
            {
                // Rest of the text can't be drawn anymore in this case
                m_lastpos = i;
                m_state = INCOMPLETE;
                break;
            }
            else
            {
                ypos++;                 // line break
                xpos = 0;
            }
        }

        // Handle out of range tokens
        char tokenpos = text[i]-starttoken;
        if (tokenpos > 95)              // position out of range in fontsprite
        {
            // TODO: Plug a log warning here
            tokenpos = 0;
        }

        // Draw and update
        MaleSprite(screen, &m_fontsprite, x + xpos*w, y + ypos*h, tokenpos, 0, true);
        xpos++;
        m_lastpos = i;
    }

    // Check whether whole text has been drawn in for loop above
    // In this case: check switch and draw promt message
//    if(i == (text.size()))
    if(i == text.size())
    {
        // update switch
        m_state = COMPLETE;
    }
}

void StaticScreenText::Scroll()
{
    m_startpos = m_lastpos + 1;
    m_stoppos = m_lastpos + 1;
    m_state = RUNNING;
    m_timer.Reset();
}

// GET

text_state StaticScreenText::GetState()
{
    return m_state;
}

// DTOR

StaticScreenText::~StaticScreenText()
{

}

// CTOR

StaticScreenText::StaticScreenText()
        : ScreenText(), m_timer(m_interval)
{
    m_state = RUNNING;
    m_startpos = 0;
    m_stoppos = 0;
    m_lastpos = 0;
    m_w = 100;
    m_h = 100;
    if(m_h < 0)
    {
        m_h = 0;
    }
}

StaticScreenText::StaticScreenText(int x, int y, int w, int h, int tokenw, int tokenh, string path)
        : ScreenText("", path, x, y, tokenw, tokenh), m_timer(m_interval)
{
    m_state = RUNNING;
    m_startpos = 0;
    m_stoppos = 0;
    m_lastpos = 0;
    m_w = w;
    m_h = h;
    if(m_h < 0)
    {
        m_h = 0;
    }
}

void StaticScreenText::SetSize(int w, int h)
{
    // Check range
    if (w < GetW() or h < GetH())           // GetW() = Token width, GetH() = Token height
    {
        // TODO: log warning
        return;
    }

    m_w = w;
    m_h = h;
}

