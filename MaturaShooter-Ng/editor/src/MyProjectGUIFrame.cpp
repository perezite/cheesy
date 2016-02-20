#include "MyProjectGUIFrame.h"
#include "MyProjectDialogNew.h"
#include "MyProjectDialogClone.h"
#include "filereader.h"
#include "levelreader.h"
#include <math.h>
//#include <wx/dcbuffer.h>
//#include <wx/textdlg.h>
#include <map>
#include <list>
#include <vector>
//#include <iostream>
//#include <fstream>

using namespace std;


// Selection Frame
int SelectFramex1 = 0, SelectFramey1 = 0;
int SelectFramex2 = 0, SelectFramey2 = 0;

// Grid Data
int GridDeltaX = 50;
int GridDeltaY = 50;
bool GridActivated = false;


bool ObjectsActivated = true, TilesActivated = true;

// Editor Objekt Liste
list<EDOBJEKT> ObjList;
// Paste-Buffer Liste (ausgeschnittene/kopierte Objekte)
list<EDOBJEKT> PasteBufferList;
// Momentan einzufuegendes Objekt
EDOBJEKT CurrObj;
// Path des momentan einzufuegenden Objektes
string strCurrObj;
// Momentan einzufügendes Tile
EditorTile CurrTile;
// Path des momentan einzfügenden Tiles
string strCurrTile;
// Iterator auf letztes selektiertes Objekt
list<EDOBJEKT>::iterator itLastSelected = ObjList.begin();

// Map mit Object Templates (index ist der path)
map<string, EDOBJEKT> ObjTmplMap;
// Map mit Tile Templates (index ist der path)
map<string, EditorTile> TileTmplMap;
// Momentaner Modus des Editors (Edit/Selection/Deletion)
int EditorMode = MODE_SELECTION;
// last save location
string LastSaveLocation("");
// Temp
wxBitmap *TestBmp;

void SaveLevel(void);
void SetLevelSizes(int x, int y);

// Size of Level
int LevelSizeX = 3000, LevelSizeY = 3000;
int NewLevelSizeX = 0, NewLevelSizeY = 0;   // Sizes coming from "New Level" Dialog (must first be validated)



// Global Helper function
void SetLevelSizes(int x, int y)
{
    NewLevelSizeX = x;
    NewLevelSizeY = y;
}

// Helper function
// Refreshes the Screen
void MyProjectGUIFrame::RefreshScreen(void)
{
    m_scrolledWindow2->Refresh();
    m_scrolledWindow21->Refresh();
}

// Helper Function
int DumpObjectCoords(string filename)
{
    ofstream myfile (filename.c_str());
    if (myfile.is_open())
    {
        list<EDOBJEKT>::iterator obj;
        for (obj = ObjList.begin(); obj != ObjList.end(); obj++)
        {
            myfile << obj->path << ": " << obj->x << " " << obj->y << "\n";
        }
        myfile.close();
        return 0;
    }
    else
        return 1;

}

// Helper Function
void SelectObjectsByFrame(int fx1, int fy1, int fx2, int fy2)
{
    list<EDOBJEKT>::iterator obj;

    // Unselect all previously selected objects
    for (obj = ObjList.begin(); obj != ObjList.end(); obj++)
    {
        (*obj).selected = false;
    }

    // Find and select all objects within the frame
    for (obj = ObjList.begin(); obj != ObjList.end(); obj++)
    {
        wxBitmap *bmp = (*obj).bmp;
        int ox1 = (int)(*obj).x;
        int oy1 = (int)(*obj).y;
        int ox2 = ox1 + bmp->GetWidth();
        int oy2 = oy1 + bmp->GetHeight();
        if( // Brute force collision check
            (ox2 > fx1 and ox2 < fx2 and oy2 > fy1 and oy2 < fy2 ) or
            (ox2 > fx1 and ox2 < fx2 and oy1 > fy1 and oy1 < fy2 ) or
            (ox1 > fx1 and ox1 < fx2 and oy2 > fy1 and oy2 < fy2 ) or
            (ox1 > fx1 and ox1 < fx2 and oy1 > fy1 and oy1 < fy2 )
            )
        {
            obj->selected = true;
        }
    }

}

// Helper function
int ReadObjectsIntoTreeCtrl(wxTreeCtrl *m_treeCtrl3)
{
    // Read Objects into Array
    int nelems = ReadObjektTemplates();
    LPEDOBJEKT ObjList = GetObjektTemplateList();
    // Read Tiles into Array
/*    map<string, EditorTile> *temp = ReadTileTemplates("tile.txt");
    TileTmplMap = *temp;*/

    // create map from array, indexed by filename
    for (int i = 0; i < nelems; i++)
    {
        EDOBJEKT insertObj = ObjList[i];
        ObjTmplMap[insertObj.path] = insertObj;
    }

    // Create editor bitmap for each template object
//    char *lastpath;
    map<string, EDOBJEKT>::iterator it;
    for (it = ObjTmplMap.begin(); it != ObjTmplMap.end(); it++)
    {
        pair<string, EDOBJEKT> stuff = (*it);
        EDOBJEKT obj = stuff.second;
        char *path = obj.path;
        wxBitmap *bmp = new wxBitmap(wxT(path), wxBITMAP_TYPE_BMP);
        if (bmp->IsOk())
        {
            wxColour color(255, 0, 255);
            wxMask *mask = new wxMask(*bmp, color);
            bmp->SetMask(mask);
//            MessageBox(NULL, path , "OK", NULL);
            ObjTmplMap[path].bmp = bmp;
            TestBmp = bmp;
            strCurrObj = path;
        }
        else
        {
            delete bmp;
            wxBitmap *bmp = new wxBitmap();
            ObjTmplMap[path].bmp = bmp;
        }
    }

    // create a map for info about the classes
    map<string, pair<wxTreeItemId, int> > classinfo;    // pair.first = Id des Parentnodes, pair.second = Anz Elems in dem Node

    // Root Item erstellen
    wxTreeItemId rootId = m_treeCtrl3->AddRoot(wxT("Objects"), 0, 0);

    // Objekte in wxTreeCtrl eintragen (classinfo wird gebraucht, um sicherzustellen, dass die Namen am richtigen Ort eingetragen
    // werden, nicht um die Templates zu speichern)
    for (int i = 0; i < nelems; i++)
    {
        string classname = ObjList[i]._class;
        wxTreeItemId &parentid = classinfo[classname].first;
        int &nelemsinclass = classinfo[classname].second;
        if (nelemsinclass == 0)
        {
            wxTreeItemId newid = m_treeCtrl3->AppendItem(rootId, wxT(classname), 1, 1); // Neuen Subnode generieren
            classinfo[classname].first = newid;
            char buf[128];
            sprintf(buf, "%s %s", ObjList[i]._class, ObjList[i].path);
//          MessageBox(NULL, buf, "foo", NULL);
        }
        nelemsinclass += 1;
        m_treeCtrl3->AppendItem(parentid, wxT(ObjList[i].path), 1, 1);
    }

    return 1;
}


// Helper function
// Fills the property window to the left bottom with props. of the given object
int MyProjectGUIFrame::FillPropertyWindow(EDOBJEKT object)
{
    char buffer[1024];
//    itoa(object.energie, buffer, 10);
    sprintf(buffer, "%d", object.energie);
    m_textCtrlEnergy->ChangeValue(wxT(buffer));
    sprintf(buffer, "%d", object.schaden);
    m_textCtrlAttack->ChangeValue(wxT(buffer));
    sprintf(buffer, "%d", (int)object.LifeTime);
    m_textCtrlLifetime->ChangeValue(wxT(buffer));
    sprintf(buffer, "%f", object.s);
    m_textCtrlVelocity->ChangeValue(wxT(buffer));
    m_checkBoxCollDetection->SetValue(object.kollision);
//  m_choiceInitialState
    sprintf(buffer, "%d", object.akt_bild);
    m_textCtrlInitialAnimFrame->ChangeValue(wxT(buffer));
    sprintf(buffer, "%d", (int)object.AnimLifetime);
    m_textCtrlAnimLifetime->ChangeValue(wxT(buffer));
    m_checkBoxAnimated->SetValue(object.animiert);
    m_textCtrlGfxFile->ChangeValue(object.path);
    m_bitmapBitmap->SetBitmap(*(object.bmp));

    // Spin Ctrl's are a little special
    m_spinCtrlXCoord->SetRange(0, LevelSizeX);
    m_spinCtrlYCoord->SetRange(0, LevelSizeY);

    if(object.x < 0)
    {
        m_spinCtrlXCoord->SetRange(int(object.x), LevelSizeX);
    }
    if(object.x > LevelSizeX)
    {
        m_spinCtrlXCoord->SetRange(0, int(object.x));
    }
    if(object.y < 0)
    {
        m_spinCtrlYCoord->SetRange(int(object.y), LevelSizeY);
    }
    if(object.y > LevelSizeY)
    {
        m_spinCtrlYCoord->SetRange(0, int(object.y));
    }

    m_spinCtrlXCoord->SetValue(int(object.x));
    m_spinCtrlYCoord->SetValue(int(object.y));

    RefreshScreen();

    return 0;
}


// Helper function
// Displace all Selected Objects in ObjList
void DisplaceSelectedObjects(int dx, int dy)
{
    list<EDOBJEKT>::iterator obj;
    for (obj = ObjList.begin(); obj != ObjList.end(); obj++)
    {
        if ((*obj).selected)
        {
            (*obj).x += dx;
            (*obj).y += dy;
        }
    }
}

// Helper function
// Copies currently selected objects into PasteBufferList with relative coordinates
bool CopyToPasteBuffer(void)
{
    list<EDOBJEKT>::iterator obj;
    list<EDOBJEKT>::iterator firstobj = ObjList.end();
    int mostleftx = 0;          // x-coord of obj. most to the left
    int mosttopy = 0 ;          // y-coord of obj. most to the top

    // Determine first selected object and take its coords
    bool found = false;

    for (obj = ObjList.begin(); obj != ObjList.end(); obj++)
    {
        if ((*obj).selected)
        {
            found = true;
            firstobj = obj;
            break;
        }
    }

    // Check whether there is any object selected
    if (found == false)
    {
        return false;
    }

    // Take the coords from first selected objects, so no magic number
    // initialization of mostleftx and mosttopy necessary
    mostleftx = (int)firstobj->x;
    mosttopy = (int)firstobj->y;


    // Determine x-coord of selected obj. that is the most to the left
    for (obj = ObjList.begin(); obj != ObjList.end(); obj++)
    {
        if (obj->selected)
        {
            if ( (int)obj->x < mostleftx)
            {
                mostleftx = (int)obj->x;
            }
        }
    }

    // Determine y-coord of selected obj. that is the most to the left
    for (obj = ObjList.begin(); obj != ObjList.end(); obj++)
    {
        if (obj->selected)
        {
            if ( (int)obj->y < mosttopy)
            {
                mosttopy = (int)obj->y;
            }
        }
    }

    // Copy selected objects into PasteBufferList with relative coords
    // to the topleft obj
    PasteBufferList.clear();

    for (obj = ObjList.begin(); obj != ObjList.end(); obj++)
    {
        if (obj->selected)
        {
            EDOBJEKT TempObj = (*obj);
            TempObj.x = TempObj.x - (float)mostleftx;  // rel. coord.
            TempObj.y = TempObj.y - (float)mosttopy;   // rel. coord.
            // In some cases, coords may be out of bounds.. correct that
            if (TempObj.x < 0)
                TempObj.x = 0;
            if (TempObj.y < 0)
                TempObj.y = 0;
            PasteBufferList.push_back(TempObj);
        }
    }

    return true;
}


// Helper function
// Pushes objects from PasteBuffer to ObjList
void MyProjectGUIFrame::PushFromPasteBuffer(void)
{
    list<EDOBJEKT>::iterator itpbl;         // PasteBufferList Iterator

    int offx = 0, offy = 0;                 // Offset of coordinates, due to scrolling
    m_scrolledWindow2->CalcUnscrolledPosition(0, 0, &offx, &offy);

    for (itpbl = PasteBufferList.begin(); itpbl != PasteBufferList.end(); itpbl++)
    {
        // Displace objects according to scrolling of window
        (*itpbl).x += offx;
        (*itpbl).y += offy;
        ObjList.push_back((*itpbl));
        // Undo displacement in the PasteBuffer list
        (*itpbl).x -= offx;
        (*itpbl).y -= offy;
    }
}


// Helper function
// Deletes all selected objects from ObjList
void DeleteAllSelectedObjects(void)
{
    list<EDOBJEKT>::iterator itol;

    for (itol = ObjList.begin(); itol != ObjList.end(); itol++)
    {
        if ((*itol).selected == true)
        {
            list<EDOBJEKT>::iterator temp;
            temp = itol;
            temp--;
            ObjList.erase(itol);
            itol = temp;
        }
    }

}


// Helper function
// Creates a wxBitmap with mask from given bmp-file
wxBitmap *CreateBitmapFromFile(string path)
{
    wxBitmap *bmp = new wxBitmap(wxT(path), wxBITMAP_TYPE_BMP);
    if (bmp->IsOk())
    {
        wxColour color(255, 0, 255);
        wxMask *mask = new wxMask(*bmp, color);
        bmp->SetMask(mask);
    }
    else
    {
        delete bmp;
        bmp = new wxBitmap();
    }

    return bmp;
}


// Helper function
// Performs magnetization of the list that is passed
void DoMagnetize(list<EDOBJEKT> lst, bool smart)
{
    // if smart: Don't exec func. too often (if this function
    // is called in a continuous way)
    if(smart)
    {
        if(rand()%100)
            return;
    }

    list<EDOBJEKT>::iterator it;
    list<EDOBJEKT>::iterator it2;
    int tolerance = 30;
    bool match = false;

//    MessageBox(NULL, "bla", "bla", NULL);

    // Run through all selected elements and displace
    // the first selected object within tolerance to
    // the nearest side of the adjacent unselected object
    for (it = ObjList.begin(); it != ObjList.end(); it++)
    {
        if (match == true)
        {
 //           MessageBox(NULL, "matched", "", NULL);
            break;
        }

//        MessageBox(NULL, "checkpoint1", " ", NULL);

        EDOBJEKT& obj = (*it);
        if (obj.selected == false)
        {
            continue;
//            MessageBox(NULL, "checkpoint3", " ", NULL);
        }

        int oleft = (int)obj.x;
        int otop = (int)obj.y;
        int owidth =  (obj.bmp)->GetWidth();
        int oheight = (obj.bmp)->GetHeight();
        int oright = oleft + owidth;
        int obottom = otop + oheight;

        // Check with all unselected elements and find first
        // adjacent side
        for (it2 = ObjList.begin(); it2 != ObjList.end(); it2++)
        {

//            MessageBox(NULL, "checkpoint2", " ", NULL);
            EDOBJEKT& obj2 = (*it2);

            if (obj2.selected == true)
            {
//                MessageBox(NULL, "skip", " ", NULL);
                continue;
            }

            int oleft2 = (int)obj2.x;
            int otop2 = (int)obj2.y;
            int owidth2 = (obj2.bmp)->GetWidth();
            int oheight2 = (obj2.bmp)->GetHeight();
            int oright2 = oleft2 + owidth2;
            int obottom2 = otop2 + oheight2;

            char buf[1024];
            sprintf(buf, "obj1: %d %d, %d %d  obj2: %d %d %d %d", oleft, oright, otop, obottom, oleft2, oright2, otop2, obottom2);
//            MessageBox(NULL, buf, "check", NULL);

            // Skip if one side is already adjacent
/*            if(oleft == oleft2 or oleft == oright2 or oright == oright2 or oright == oleft2)
                continue;
            if(otop == otop2 or otop == obottom2 or obottom == obottom2 or obottom == otop2)
                continue;*/

            // Check all sides of obj and obj2 whether they are adjacent
            // Note: !(A or B) <=> !A and !B
            int deltax = 0, deltay = 0;

            if (abs(oleft2 - oright) < tolerance and abs(oleft2 - oright) != 0 and !(otop2 > obottom or obottom2 < otop))
            {
                deltax = oleft2 - oright;
                match = true;
                //obj.x += deltax;
                //obj.y += deltay;
                DisplaceSelectedObjects(deltax, deltay);
                char buf2[128];
                sprintf(buf2, "1deltax: %d, deltay: %d", deltax, deltay);
//                MessageBox(NULL, buf2, "1", NULL);
                return;
            }
            else if (abs(oleft - oright2) < tolerance and abs(oleft - oright2) != 0 and !(otop2 > obottom or obottom2 < otop))
            {
                deltax = -(oleft - oright2);
                match = true;
                char buf2[128];
                sprintf(buf2, "2deltax: %d, deltay: %d", deltax, deltay);
//                MessageBox(NULL, buf2, "2", NULL);
                //obj.x += deltax;
                //obj.y += deltay;
                DisplaceSelectedObjects(deltax, deltay);
                return;
            }
            else if (abs(otop - obottom2) < tolerance and abs(otop - obottom2) != 0 and !(oright2 < oleft or oleft2 > oright))
            {
                deltay = -(otop - obottom2);
                match = true;
                char buf2[128];
                sprintf(buf2, "3deltax: %d, deltay: %d", deltax, deltay);
//                MessageBox(NULL, buf2, "3", NULL);
                //obj.x += deltax;
                //obj.y += deltay;
                DisplaceSelectedObjects(deltax, deltay);
                return;
            }
            else if (abs(otop2 - obottom) < tolerance and abs(otop2 - obottom) != 0 and !(oright2 < oleft or oleft2 > oright))
            {
                deltay = otop2 - obottom;
                match = true;
                char buf2[128];
                sprintf(buf2, "4deltax: %d, deltay: %d", deltax, deltay);
//                MessageBox(NULL, buf2, "4", NULL);
                //obj.x += deltax;
                //obj.y += deltay;
                DisplaceSelectedObjects(deltax, deltay);
                return;
            }
            else if (abs(obottom2 - obottom) < tolerance and abs(obottom2 - obottom) != 0 and !(oright2 < oleft or oleft2 > oright))
            {
//                MessageBox(NULL, "", "5", NULL);
                match = true;
                //obj.y = otop2 + oheight2 - oheight;
                deltay = int(otop2 + oheight2 - oheight - obj.y);
                DisplaceSelectedObjects(deltax, deltay);
                return;
            }
            else if (abs(otop2 - otop) < tolerance and abs(otop2 - otop) != 0 and !(oright2 < oleft or oleft2 > oright))
            {
//                MessageBox(NULL, "", "6", NULL);
                match = true;
                //obj.y = otop2;
                deltay = int(otop2 - obj.y);
                DisplaceSelectedObjects(deltax, deltay);
                return;
            }
            else if (abs(oleft2 - oleft) < tolerance and abs(oleft2 - oleft) != 0 and !(otop2 > obottom or obottom2 < otop))
            {
//                MessageBox(NULL, "", "7", NULL);
                match = true;
                //obj.x = oleft2;
                deltax = int(oleft2 - obj.x);
                DisplaceSelectedObjects(deltax, deltay);
                return;
            }
            else if (abs(oright2 - oright) < tolerance and abs(oright2 - oright) != 0 and !(otop2 > obottom or obottom2 < otop))
            {
                match = true;
                //obj.x = oleft2 + owidth2 - owidth;
                obj.x = oleft2 + owidth2 - owidth - obj.x;
                DisplaceSelectedObjects(deltax, deltay);
                return;
            }
        }
    }
}


MyProjectGUIFrame::MyProjectGUIFrame( wxWindow* parent )
        :
        GUIFrame( parent )
{
    // Scroll-Bereich fuer wxScrolledWindows festlegen
    m_scrolledWindow2->SetVirtualSize(LevelSizeX, LevelSizeY);
    m_scrolledWindow21->SetVirtualSize(800, 600);

    // ObjectTemplates in TreeCtrl laden
    ReadObjectsIntoTreeCtrl(m_treeCtrl3);
}


void MyProjectGUIFrame::OnClose( wxCloseEvent& event )
{
    Destroy();
}


void MyProjectGUIFrame::OnScrWindowMotion( wxMouseEvent& event )
{
    static wxPoint LastPos(0,0);

    // Select objects
    if (event.Dragging() and event.RightIsDown())
    {
        wxPoint ThisPos(event.GetX(), event.GetY());
        wxPoint DeltaPos = ThisPos - LastPos;
        DisplaceSelectedObjects(DeltaPos.x, DeltaPos.y);
        m_scrolledWindow2->Refresh();
    }

    // Displace selection frame
    if (event.Dragging() and event.LeftIsDown())
    {
        SelectFramex2 = event.GetX();
        SelectFramey2 = event.GetY();
        SelectObjectsByFrame(SelectFramex1, SelectFramey1, SelectFramex2, SelectFramey2);
        m_scrolledWindow2->Refresh();
    }

    LastPos = event.GetPosition();
}


void MyProjectGUIFrame::OnScrWindowPaint( wxPaintEvent& event )
{
    wxPaintDC dc(m_scrolledWindow2);
    m_scrolledWindow2->DoPrepareDC(dc);


    // Set pen for drawing grid
    wxPen pen3(wxColour(255, 255, 255));
    wxBrush brush3(wxColour(255, 255, 255), wxTRANSPARENT);
    dc.SetPen(pen3);
    dc.SetBrush(brush3);

    // * Draw grid (if activated) *
    if(GridActivated)
    {
        // Draw vertical lines
        int num = int(ceil((float)LevelSizeX / (float)GridDeltaX) - 1);
        for(int i = 1; i <= num; i++)
        {
            dc.DrawLine(int(i * GridDeltaX), 0, int(i * GridDeltaX), LevelSizeY);
        }
        // Draw horizontal lines
        num = int(ceil(LevelSizeY / GridDeltaY) - 1);
        for(int i = 1; i <= num; i++)
        {
            dc.DrawLine(0, int(i * GridDeltaY), LevelSizeX, int(i * GridDeltaY));
        }
    }

    // Set pen for drawing default marks
    wxPen pen(wxColour(255, 128, 0));
    wxBrush brush(wxColour(255, 0, 0), wxTRANSPARENT);
    dc.SetPen(pen);
    dc.SetBrush(brush);

    // Draw all object in object list with marks (if activated)
    list<EDOBJEKT>::iterator obj;
    for (obj = ObjList.begin(); obj != ObjList.end(); obj++)
    {
        if((*obj).activated == false)
        {
            continue;
        }
        wxBitmap *bmp = (*obj).bmp;
        int x = (int)(*obj).x;
        int y = (int)(*obj).y;
        int w = bmp->GetWidth();
        int h = bmp->GetHeight();
        dc.DrawBitmap(*bmp, x, y, true);
        dc.DrawRectangle(x, y, w, h);
    }

    // Set pen for drawing selected marks
    wxPen pen2(wxColour(0, 255, 0));
    wxBrush brush2(wxColour(255, 0, 0), wxTRANSPARENT);
    dc.SetPen(pen2);
    dc.SetBrush(brush2);

    // Draw marks for all objects in selected objects list (if activated)
    list<EDOBJEKT>::iterator obj2;
    for (obj2 = ObjList.begin(); obj2 != ObjList.end(); obj2++)
    {
        if((*obj2).activated == false)
        {
            continue;
        }
        if ((*obj2).selected)
        {
            wxBitmap *bmp = (*obj2).bmp;
            int x = (int)(*obj2).x;
            int y = (int)(*obj2).y;
            int w = bmp->GetWidth();
            int h = bmp->GetHeight();
            dc.DrawRectangle(x, y, w, h);
        }
    }

    // Set pen for drawing selection frame
    wxPen pen4(wxColour(255, 255, 0));
    wxBrush brush4(wxColour(255, 0, 0), wxTRANSPARENT);
    dc.SetPen(pen4);
    dc.SetBrush(brush4);

    // Draw selection frame if in frame selection mode
    if(EditorMode == MODE_FRAME)
    {
        int x = SelectFramex1;
        int y = SelectFramey1;
        int w = SelectFramex2 - SelectFramex1;
        int h = SelectFramey2 - SelectFramey1;
        dc.DrawRectangle(x, y, w, h);
    }

}


void MyProjectGUIFrame::OnScrWindowClick( wxMouseEvent& event )
{
    int x, y;
    m_scrolledWindow2->CalcUnscrolledPosition(event.m_x, event.m_y, &x,&y);
    EDOBJEKT InserterObject = ObjTmplMap[strCurrObj];
    InserterObject.x = x;
    InserterObject.y = y;


    if(EditorMode == MODE_FRAME)
    {   // --- Frame selection mode ---
        SelectFramex1 = event.GetX();
        SelectFramey1 = event.GetY();
        SelectFramex2 = SelectFramex1;
        SelectFramey2 = SelectFramey1;
    }
    else
    {   // --- Normal insertion ---
        // Check if object must be activated or not
        if(strcmp(InserterObject._class, "Tile") == 0)
        {
            InserterObject.activated = TilesActivated;
        }
        else
        {
            InserterObject.activated = ObjectsActivated;
        }

        // Insert stuff
        ObjList.push_back(InserterObject);
    }

    RefreshScreen();
}


void MyProjectGUIFrame::OnTreeCtrlItemActivated( wxTreeEvent& event )
{
    wxTreeItemId id = event.GetItem();
    wxString name = m_treeCtrl3->GetItemText(id);
    wxTreeItemId parentid = m_treeCtrl3->GetItemParent(id);
    wxString parentname = m_treeCtrl3->GetItemText(parentid);

    // TODO: Implement this crap
 /*   if(parentname == "Tiles") // Tile
    {
        // Change current insert object to the chosen one, if bitmap of chosen object is valid
        if (TileTemplates[name.c_str()].bmp->IsOk())
        {
            MessageBox(NULL, parentname.c_str(), "bla", NULL);
            strCurrObj = name.c_str();
//            m_bitmapBitmap->SetBitmap(*TileTemplates[name.c_str()].bmp));
            Refresh();
        }
        else
        {
            wxMessageDialog(this, "bmp file is not valid");
        }
    }
    else // non-Tile
    {*/
        // Change current insert object to the chosen one, if bitmap of chosen object is valid
        if (ObjTmplMap[name.c_str()].bmp->IsOk())
        {
            strCurrObj = name.c_str();
            m_bitmapBitmap->SetBitmap(*(ObjTmplMap[name.c_str()].bmp));
            Refresh();

        }
        else
        {
            wxMessageDialog(this, "bmp file is not valid");
        }
//    }
}


void MyProjectGUIFrame::OnScrWindowRClick( wxMouseEvent& event )
{
    int mx, my;
    m_scrolledWindow2->CalcUnscrolledPosition(event.m_x, event.m_y, &mx,&my);

    // Find and select first object for which clicked point lies within
    list<EDOBJEKT>::iterator obj;

    bool match = false;
    for (obj = ObjList.begin(); obj != ObjList.end(); obj++)
    {
        wxBitmap *bmp = (*obj).bmp;
        int ox = (int)(*obj).x;
        int oy = (int)(*obj).y;
        int ow = bmp->GetWidth();
        int oh = bmp->GetHeight();
        if (mx > ox && my > oy && mx < (ox+ow) && my < (oy+oh))
        {
            match = true;
            break;
        }
    }

    // If no objects lies within the clicked point, stop
    if (match == false)
    {
        return;
    }

    if (EditorMode == MODE_SELECTION)
    {
        (*obj).selected = !((*obj).selected);

        RefreshScreen();
    }

    if (EditorMode == MODE_EDIT)
    {
        // Set last selected object to the chosen one
        itLastSelected = obj;
        string path = (*obj).path;
        FillPropertyWindow((*itLastSelected));

        RefreshScreen();
    }

    if (EditorMode == MODE_DELETE)
    {
        ObjList.erase(obj);

        RefreshScreen();
    }

}


void MyProjectGUIFrame::OnMenuItemNew( wxCommandEvent& event )
{
    MyProjectDialogNew *dlg = new MyProjectDialogNew(this);
    if ( dlg->ShowModal() == wxID_OK )
    {
        CreateNewLevel();
    }

    dlg->Destroy();
}


// Helper Function
void MyProjectGUIFrame::CreateNewLevel(void)
{
    // check if dimensions are OK
    if(NewLevelSizeX < 640 or NewLevelSizeY < 480)
    {
//        MessageBox(NULL, "Dimensions are too small", "Error", NULL);
        return;
    }

    // Save old Level
//    MessageBox(NULL, "save old file now..", "Message", NULL);
    SaveLevel();

    // Create new Level
    LevelSizeX = NewLevelSizeX;
    LevelSizeY = NewLevelSizeY;
    wxCommandEvent dummy;
    OnMenuItemSelectAll(dummy);
    OnMenuItemDeleteSelected(dummy);
    m_scrolledWindow2->SetVirtualSize(LevelSizeX, LevelSizeY);
    LastSaveLocation = "";

}


void MyProjectGUIFrame::OnSpinCtrlXCoord ( wxCommandEvent& event)
{
    int x = m_spinCtrlXCoord->GetValue();
    (*itLastSelected).x = x;

    RefreshScreen();
}

void MyProjectGUIFrame::OnSpinCtrlYCoord ( wxCommandEvent& event)
{
    int y = m_spinCtrlYCoord->GetValue();
    (*itLastSelected).y = y;

    RefreshScreen();
}

void MyProjectGUIFrame::OnTextEnergy( wxCommandEvent& event )
{
    wxString input = m_textCtrlEnergy->GetValue();
    int value = atoi(input.c_str());
    (*itLastSelected).energie = value;
}


void MyProjectGUIFrame::OnTextAttack( wxCommandEvent& event )
{
    wxString input = m_textCtrlAttack->GetValue();
    int value = atoi(input.c_str());
    (*itLastSelected).schaden = value;
}


void MyProjectGUIFrame::OnTextLifetime( wxCommandEvent& event )
{
    wxString input = m_textCtrlLifetime->GetValue();
    int value = atoi(input.c_str());
    (*itLastSelected).LifeTime = value;
}


void MyProjectGUIFrame::OnTextInitVelocity( wxCommandEvent& event )
{
    wxString input = m_textCtrlAttack->GetValue();
    float value = atof(input.c_str());
    (*itLastSelected).s = value;
}


void MyProjectGUIFrame::OnCheckBoxCollDetection( wxCommandEvent& event )
{
    bool value = m_checkBoxCollDetection->GetValue();
    (*itLastSelected).kollision = value;
}


void MyProjectGUIFrame::OnChoiceInitialState( wxCommandEvent& event )
{
    // TODO: Implement OnChoiceInitialState
}


void MyProjectGUIFrame::OnTextInitialAnimFrame( wxCommandEvent& event )
{
    wxString input = m_textCtrlInitialAnimFrame->GetValue();
    int value = atoi(input.c_str());
    (*itLastSelected).schaden = value;
}


void MyProjectGUIFrame::OnTextAnimLifetime( wxCommandEvent& event )
{
    wxString input = m_textCtrlAnimLifetime->GetValue();
    int value = atoi(input.c_str());
    (*itLastSelected).schaden = value;
}


void MyProjectGUIFrame::OnCheckBoxAnimated( wxCommandEvent& event )
{
    bool value = m_checkBoxAnimated->GetValue();
    (*itLastSelected).kollision = value;
}


void MyProjectGUIFrame::OnTextGfxFile( wxCommandEvent& event )
{

}

void MyProjectGUIFrame::OnMenuItemEditMode( wxCommandEvent& event )
{
    EditorMode = MODE_EDIT;
    m_scrolledWindow21->Enable();
    m_statusBarMode->PushStatusText("Edit Mode");
    RefreshScreen();
}

void MyProjectGUIFrame::OnMenuItemSelectionMode( wxCommandEvent& event )
{
    EditorMode = MODE_SELECTION;
    m_scrolledWindow21->Disable();
    m_statusBarMode->PushStatusText("Selection Mode");
    RefreshScreen();
}

void MyProjectGUIFrame::OnMenuItemFrameMode( wxCommandEvent& event)
{
    m_scrolledWindow21->Disable();
    EditorMode = MODE_FRAME;
    m_statusBarMode->PushStatusText("Frame Selection Mode");
    RefreshScreen();
}

void MyProjectGUIFrame::OnMenuItemCancelSelection( wxCommandEvent& event )
{
    list<EDOBJEKT>::iterator obj;
    for (obj = ObjList.begin(); obj != ObjList.end(); obj++)
    {
        (*obj).selected = false;
    }
    RefreshScreen();
}

void MyProjectGUIFrame::OnMenuItemCopy( wxCommandEvent& event )
{
    CopyToPasteBuffer();
}


void MyProjectGUIFrame::OnMenuItemPaste( wxCommandEvent& event )
{
    list<EDOBJEKT>::iterator obj;
    for (obj = ObjList.begin(); obj != ObjList.end(); obj++)
    {
        (*obj).selected = false;
        RefreshScreen();
    }

    PushFromPasteBuffer();
    RefreshScreen();
}


void MyProjectGUIFrame::OnMenuItemCut( wxCommandEvent& event )
{
    CopyToPasteBuffer();
    DeleteAllSelectedObjects();
    RefreshScreen();
}


void MyProjectGUIFrame::OnMenuItemSelectAll( wxCommandEvent& event )
{
    list<EDOBJEKT>::iterator obj;

    // TODO: What the heck is this?!?
//    PrepareFileSaving("test.lvl", ObjList.size());

    for (obj = ObjList.begin(); obj != ObjList.end(); obj++)
    {
        (*obj).selected = true;
    }

    RefreshScreen();
}


void MyProjectGUIFrame::OnMenuItemDeletionMode( wxCommandEvent& event )
{
    m_scrolledWindow21->Disable();
    EditorMode = MODE_DELETE;
    m_statusBarMode->PushStatusText("Deletion Mode");
}

void MyProjectGUIFrame::OnMenuItemOpen( wxCommandEvent& event )
{
    // show file dialog to get file path
    wxFileDialog fd(NULL);
    fd.SetWildcard("LVL files (*.lvl)|*.lvl");
    fd.ShowModal();
    wxString path = fd.GetPath();

    // security check
    if (path == wxString(""))
        return;

    // clear
    ObjList.clear();

    // read Header
    int SizeX, SizeY;
    int num = PrepareFileLoading(path.c_str(), SizeX, SizeY);
    m_scrolledWindow2->SetVirtualSize(SizeX, SizeY);
    LevelSizeX = SizeX;
    LevelSizeY = SizeY;

    char buf[128];
    sprintf(buf, "%d", num);
//    MessageBox(NULL, buf, "load", NULL);

    // load objects
    for (int i = 0; i < num; i++)
    {
        LPEDOBJEKT obj = LoadObjekt();
        EDOBJEKT InserterObject = ObjTmplMap[obj->path];
        obj->bmp = InserterObject.bmp;

        // Check if object must be activated or not
        if(strcmp(obj->_class, "Tile") == 0)
        {
            obj->activated = TilesActivated;
        }
        else
        {
            obj->activated = ObjectsActivated;
        }

        char buf[128];
        sprintf(buf, "%s", obj->path);
//        MessageBox(NULL, buf, "insert", NULL);

        // insert the stuff
        ObjList.push_back(*obj);
    }

    // unselect all objects
    list<EDOBJEKT>::iterator obj;
    for (obj = ObjList.begin(); obj != ObjList.end(); obj++)
    {
        (*obj).selected = false;
    }

    RefreshScreen();
}


// Helper function
void SaveLevel(void)
{
    // Do some usseful dumping stuff
    DumpObjectCoords("leveldump.txt");

    list<EDOBJEKT>::iterator obj;
    wxString PathToSaveTo;

    // get path to save into
    if (LastSaveLocation == string("")) // file hasn't been saved before
    {
        // show file dialog to get file path
        wxFileDialog fd(NULL);
        fd.SetWildcard("LVL files (*.lvl)|*.lvl");
        fd.ShowModal();
        PathToSaveTo = fd.GetPath();
    }
    else
    {
        PathToSaveTo = LastSaveLocation;
    }

    // security check
    if (PathToSaveTo == wxString(""))
        return;

    // save the stuff
    PrepareFileSaving(PathToSaveTo, ObjList.size(), LevelSizeX, LevelSizeY);

    int numsaved = 0;
    for (obj = ObjList.begin(); obj != ObjList.end(); obj++)
    {
        char buf[128];
        sprintf(buf, "%f %f", (*obj).x, (*obj).y);
//        MessageBox(NULL, buf, "insert", NULL);
        SaveObjekt(&(*obj));
        numsaved++;
    }

    char buf[128];
    sprintf(buf, "%d", numsaved);
//    MessageBox(NULL, buf, "bla", NULL);

    // store location where file has been saved to
    LastSaveLocation = PathToSaveTo;
}


void MyProjectGUIFrame::OnMenuItemSave( wxCommandEvent& event )
{
    SaveLevel();
    RefreshScreen();
}

void MyProjectGUIFrame::OnMenuItemSaveAs( wxCommandEvent& event )
{
    list<EDOBJEKT>::iterator obj;
    wxString PathToSaveTo;

    // show file dialog to get file path
    wxFileDialog fd(NULL);
    fd.SetWildcard("LVL files (*.lvl)|*.lvl");
    fd.ShowModal();
    PathToSaveTo = fd.GetPath();

    // security check
    if (PathToSaveTo == wxString(""))
        return;

    // save the stuff
    PrepareFileSaving(PathToSaveTo, ObjList.size(), LevelSizeX, LevelSizeY);

    for (obj = ObjList.begin(); obj != ObjList.end(); obj++)
    {
        SaveObjekt(&(*obj));
    }

    // store location where file has been saved to
    LastSaveLocation = PathToSaveTo;

}

void MyProjectGUIFrame::OnMenuItemAbout( wxCommandEvent& event )
{
    DialogAbout *dlg = new DialogAbout(this, -1, wxT("About"));
    if ( dlg->ShowModal() == wxID_OK )
    {

    }

    dlg->Destroy();

}


void MyProjectGUIFrame::OnMenuItemDeleteSelected( wxCommandEvent& event )
{
    DeleteAllSelectedObjects();
    RefreshScreen();
}


void MyProjectGUIFrame::OnMenuItemMagnetize( wxCommandEvent& event )
{
    DoMagnetize(ObjList, false);
    RefreshScreen();
}


void MyProjectGUIFrame::OnMenuItemShowObjects( wxCommandEvent& event )
{

    // swap the activated member of each object (non-tile) in ObjList
    list<EDOBJEKT>::iterator obj;
    for (obj = ObjList.begin(); obj != ObjList.end(); obj++)
    {
        if(strcmp((*obj)._class, "Tile") == 0)
        {
            continue;
        }
        (*obj).activated = !(*obj).activated;
    }

    // Same for PasteBufferList
    for (obj = PasteBufferList.begin(); obj != PasteBufferList.end(); obj++)
    {
        if(strcmp((*obj)._class, "Tile") == 0)
        {
            continue;
        }
        (*obj).activated = !(*obj).activated;
    }


    RefreshScreen();

    ObjectsActivated = !ObjectsActivated;
}


void MyProjectGUIFrame::OnMenuItemShowTiles( wxCommandEvent& event )
{
    // swap the activated member of each tile (non-object) in ObjList
    list<EDOBJEKT>::iterator obj;
    for (obj = ObjList.begin(); obj != ObjList.end(); obj++)
    {
        if(strcmp((*obj)._class, "Tile") != 0)
        {
            continue;
        }
        (*obj).activated = !(*obj).activated;
    }

    // same for PasteBufferList
    for (obj = PasteBufferList.begin(); obj != PasteBufferList.end(); obj++)
    {
        if(strcmp((*obj)._class, "Tile") != 0)
        {
            continue;
        }
        (*obj).activated = !(*obj).activated;
    }

    RefreshScreen();

    TilesActivated = !TilesActivated;
}


// Helper function:
// Clone selected objects using a displacement angle of "angle"
// and making a gap of "gap" between each clone
void CloneSelectedObjects(int gap, int angle, int times)
{
    static const float Pi = 3.1415;
//    gap = 5;
    float rstep = 100;              // This is added to r in each repetition of the cloning
    float r = 0;
    float anglerad = (2.0*Pi*float(angle))/(float(360));

    for(int i = 1; i <= times; i++)
    {
        r += rstep;
        list<EDOBJEKT>::iterator obj;
        for (obj = ObjList.begin(); obj != ObjList.end(); obj++)
        {
            if ((*obj).selected)
            {
                EDOBJEKT objcopy = (*obj);
                objcopy.selected = false;       // Important: Otherwise infinite loop !!
                objcopy.x += cos(anglerad)*r;
                objcopy.y -= sin(anglerad)*r;
                ObjList.push_back(objcopy);
            }
        }
    }
}


void MyProjectGUIFrame::OnMenuItemClone ( wxCommandEvent& event )
{
    MyProjectDialogClone *dlg = new MyProjectDialogClone(this);
    if ( dlg->ShowModal() == wxID_OK )
    {
        int gaps[9];
        int angle;
        int times;
        dlg->GetValues(gaps, &angle, &times);
        char buf[1024];
        sprintf(buf, "%d %d %d, %d %d %d, %d %d %d, %d", gaps[0], gaps[1],
            gaps[2], gaps[3], gaps[4], gaps[5], gaps[6], gaps[7], gaps[8], angle);
//        MessageBox(NULL, buf, "debug", NULL);

        // Check all predefined angles
        for(int i = 0; i < 8; i++)
        {
            if(gaps[i] > 0)
            {
                int current_angle = i*45;   // Beginning top left (135°), adding 45° each step
                CloneSelectedObjects(gaps[i], current_angle, times);
            }
        }
        // Check custom angle
        if(gaps[8] > 0)
        {
            CloneSelectedObjects(gaps[8], angle, times);
        }

        RefreshScreen();
    }

    dlg->Destroy();
}


void MyProjectGUIFrame::OnMenuItemShowGrid( wxCommandEvent& event)
{
    GridActivated = !GridActivated;
    RefreshScreen();
}


void MyProjectGUIFrame::OnMenuItemDockOnGrid( wxCommandEvent& event)
{
    list<EDOBJEKT>::iterator obj;
    for (obj = ObjList.begin(); obj != ObjList.end(); obj++)
    {
        if ((*obj).selected)
        {
            int biasx = int(obj->x)%GridDeltaX;
            int biasy = int(obj->y)%GridDeltaY;
            (*obj).x -= biasx;
            (*obj).y -= biasy;
        }
    }

    RefreshScreen();
}


void MyProjectGUIFrame::OnMenuItemConfigGrid( wxCommandEvent& event)
{

    wxTextEntryDialog *dlg = new wxTextEntryDialog(this, wxT("GridDeltaX GridDeltaY"), wxT("Configure Grid"), wxT("32 32"));
    if ( dlg->ShowModal() == wxID_OK )
    {
        const char *orig_str = (dlg->GetValue()).c_str();
        char str[128];
        strcpy(str, orig_str);                  // This one is somewhat ugly, but I didn't find any other technique
        char * pch;

        pch = strtok(str, " ,.-");
        if(pch == NULL)
        {
//            MessageBox(NULL, "wrong input, aborting", "", NULL);
            return;
        }
        int valx = atoi(pch);

        pch = strtok(NULL, " ,.-");
        if(pch == NULL)
        {
//            MessageBox(NULL, "wrong input, aborting", "", NULL);
            return;
        }
        int valy = atoi(pch);

        GridDeltaX = valx;
        GridDeltaY = valy;
    }

    dlg->Destroy();

    RefreshScreen();
}



