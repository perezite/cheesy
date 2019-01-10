#ifndef __FONT_H_
#define __FONT_H_

#include "sprite.h"
#include "timer.h"
#include <string>
#include <vector>

using namespace std;

// ************
// Class: ScreenText
// ************

class ScreenText
{
public:
    virtual void Draw(SDL_Surface *screen) = 0;
    // CTOR
    ScreenText();
    ScreenText(string text, string fontpath, int w, int h, int x, int y);   // w,h: size of one token
    // DTOR
    virtual ~ScreenText();
    // SET
    void SetW(int w);
    void SetH(int h);
    void SetX(int x);
    void SetY(int y);
    void SetText(string text);
    // GET
    int GetW();
    int GetH();
    int GetX();
    int GetY();
    string GetText();
private:
    static const int m_numtokens;
    int m_w, m_h;
    int m_x, m_y;
    string m_stringpath;
    string m_text;
protected:
    SPRITE m_fontsprite;
};


// ************
// Class: StaticScreenText
// Derived From: ScreenText
// ************

enum text_state {RUNNING, COMPLETE, INCOMPLETE};

class StaticScreenText : public ScreenText
{
public:
    virtual void Draw(SDL_Surface *screen);
    void Scroll();                                      // Writes next chunk of text into clip rect
    // CTOR
    StaticScreenText();
    StaticScreenText(int x, int y, int w, int h, int tokenw = 8, int tokenh = 8, string path = "graphics\\font8x8.bmp");
    // DTOR
    virtual ~StaticScreenText();
    // SET
    void SetSize(int w, int h);                        // (w,h) in Pixel; size of the TextField
    // GET
    text_state GetState();
private:
    static DWORD m_interval;
    int m_w, m_h;
    unsigned int m_startpos;                            // Position from which text is drawn
    unsigned int m_stoppos;                             // Position up to which text _should_ be drawn
    unsigned int m_lastpos;                             // Last position that has actually been reached
    Timer m_timer;
    text_state m_state;
};


// ************
// Class: TextCollection
// Desc: Shows text in a series of textfields
// ************
/*
class TextCollection
{
public:
    // CTOR
    TextCollection(string text, int x, int y, int tokenw, int tokenh, int fieldw, fieldh);
    // DTOR
    virtual ~TextCollection();
private:
    vector<StaticScreenText> m_texts;
    int m_currtext;

};
*/

#endif
