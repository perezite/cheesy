
#include "object.h"
#include "GUIFrame.h"

EDOBJEKT::EDOBJEKT()
{
}

EDOBJEKT::~EDOBJEKT()
{

}


EditorTile::EditorTile()
{
    this->selected = false;
    return;
}

EditorTile::EditorTile(const char *_filename, float _x, float _y, float _w, float _h)
{
//    MessageBox(NULL, filename, "", NULL);
    filename = _filename;
    this->SetBitmap(_filename);
    this->SetX(_x);
    this->SetY(_y);
    this->SetW((int)_w);
    this->SetH((int)_h);
    this->selected = false;

    return;
}

EditorTile::~EditorTile()
{

//    MessageBox(NULL, filename.c_str(), "calling d'tor", NULL);
    delete bmp;
    return;
}

int EditorTile::SetBitmap(const char *_filename)
{
    this->bmp = new wxBitmap(wxT(_filename), wxBITMAP_TYPE_BMP);
    if (bmp->IsOk())
    {
        filename = _filename;
        wxColour color(255, 0, 255);
        wxMask *mask = new wxMask(*bmp, color);
        bmp->SetMask(mask);
//        MessageBox(NULL, filename , "foobar", NULL);
    }
    else
    {
        delete bmp;
        bmp = new wxBitmap();
    }

    return 0;
}

int EditorTile::Draw(wxDC *dc)
{
    //wxPaintDC dc(m_scrolledWindow2);

    // Set pen for drawing default mark
    wxPen pen(wxColour(255, 128, 0));
    wxBrush brush(wxColour(255, 0, 0), wxTRANSPARENT);
    dc->SetPen(pen);
    dc->SetBrush(brush);

    if(bmp->IsOk())
    {
        dc->DrawBitmap(*bmp, (int)x, (int)y, true);
        dc->DrawRectangle((int)x, (int)y, w, h);
    }

    // Set pen for drawing selected mark
    wxPen pen2(wxColour(0, 255, 0));
    wxBrush brush2(wxColour(255, 0, 0), wxTRANSPARENT);
    dc->SetPen(pen2);
    dc->SetBrush(brush2);

    // Draw mark if selected
    if(this->selected)
    {
        dc->DrawRectangle((int)x, (int)y, w, h);
    }

    return 0;
}

void EditorTile::SetX(float x)
{
    this->x = x;
}

void EditorTile::SetY(float y)
{
    this->y = y;
}

void EditorTile::SetW(int w)
{
    this->w = w;
}

void EditorTile::SetH(int h)
{
    this->h = h;
}

string EditorTile::GetFilename(void)
{
    return this->filename;
}
