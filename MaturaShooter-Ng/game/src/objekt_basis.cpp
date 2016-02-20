//**************************************************************************//
//File: objekt_basis.cpp  													//
//Enthält Funktionen für die Objekte										//
//**************************************************************************//

#include <stdio.h>										// Für File-Funktionen
#include "objekt_basis.h"
#include "dxinit.h"
#include "units.h"
#include "levelloader.h"
#include <iostream>
#include <fstream>
#include <map>
using std::cout;
using std::cin;

// Für verkettete Listen
using namespace std;


// Objekt-Liste
list<OBJEKT> UnitList;
list<OBJEKT> InactiveUnitList;

// Fügt ein Objekt direkt in die Liste ein
int CreateObjekt(float x, float y, float zx, float zy, int typ);
// Aktualisiert das Sprite eines Objektes
int UpdateObjectSprite(LPOBJEKT objekt);
// Führt eine Kollisionsabfrage für jedes Element der Liste mit jedem anderen aus
void GlobalCollisionCheck(void);
// Löscht die gesamte Objektliste
void ClearUnitList(void);
// Prüft alle inaktiven Objekte, ob sie aktiviert werden sollen
void GlobalTriggerCheck(void);
// Pusht inaktives Objekt in InactiveUnitList
int CreateInactiveObjekt(float x, float y, int typ);
// Pusht bereits erstelltes inaktives Objekt in InactiveUnitList
int InsertInactiveObjekt(OBJEKT objekt);

//-----------Aktualisiert die verketteten Listen-------------------------------------------------
int ListUpdate()
{
    list<OBJEKT>::iterator iter1;				// Iterator für UnitList

    // Prüfen, ob zu löschende oder zerstörte Objekte in der UnitList
    for (iter1 = UnitList.begin(); iter1 != UnitList.end(); iter1++)
    {
        // Falls Objekte Tot
        if ((*iter1).zustand == OZ_DEAD)		// Muss am Anfang stehen, da Objekt sonst immer wieder auf OZ_DESTROYED gestellt wird
        {
            list<OBJEKT>::iterator iterTemp1;	// Temporär-Iterator zum Löschen
            // Objektstatus auf zerstört -> Objekt löschen
            iterTemp1 = iter1;
            ++iterTemp1;						// Temp-Iterator auf nächstes Element
            UnitList.erase(iter1);				// Zerstörtes / abgelaufenes Element entfernen
            iter1 = iterTemp1;					// Iterator auf nächstes Element(ist in iterTemp gespeichert)
            continue;
        }
        // Falls Objekt zerstört (Noch nicht tot)
        if ((*iter1).energie <= 0)
        {
            // Objekt zerstört->Status auf zerstört setzen
            (*iter1).zustand = OZ_DESTROYED;
        }
        // Falls Lebenszeit des Objekts abgelaufen
        if ((*iter1).LifeTime <= 0 /*&& (*iter1).LifeTime != NULL*/) // TODO: What's this in /* */ ???
        {
            (*iter1).zustand = OZ_DEAD;			// Lebenszeit abgelaufen -> Im nächsten Frame löschen lassen
        }
    }

    // Verhalten der Objekte (KI) aufrufen
    for (iter1 = UnitList.begin(); iter1 != UnitList.end(); iter1++)
    {
        UnitKI(&(*iter1));						// Verhalten (KI) der Objekte aufrufen
    }

    // Sprites malen
    for (iter1 = UnitList.begin(); iter1 != UnitList.end(); iter1++)
    {
        UpdateObjectSprite(&(*iter1));			// Sprite des Objektes updaten und malen
    }

    // Gesamte Liste auf Kollisionen prüfen
    GlobalCollisionCheck();

    // Check whole inactive list for trigger events
    GlobalTriggerCheck();

    return 0;
}


//-------------------------------------------------------------------------------------------
int LoadLevelFromFile(char path[128])
{


    int num = PrepareFileLoading(path);
//    UnitList.clear();

    for (int i = 0; i < num; i++)
    {
        LPOBJEKT objekt = LoadObjekt();
        if(objekt == NULL)                      // Loaded object was a Tile
        {
            continue;
        }
        InitEditorObjekt(objekt);               // Redundant (is called again in GlobalTriggerCheck)
                                                // but inevitable because GlobalTriggerCheck needs
                                                // initialised sprite data to know height and width of
                                                // object
//        UnitList.push_back(*objekt);
        InsertInactiveObjekt(*objekt);
    }

    return 0;
}


//---------Fügt ein neues Objekt in die Liste ein und setzt dessen Eigenschaften-----------------
int CreateObjekt(float x, float y,   // Position
                 float zx, float zy, // Zielkoordinaten des Objekts
                 int typ)
{
    OBJEKT objekt;
    objekt.x            =  x;					// Startposition x
    objekt.y            =  y;					// Startposition y
    objekt.zx           = zx;					// Zielposition x
    objekt.zy           = zy;					// Zielposition y
    objekt.s			=  1;					// Geschwindigkeit
    objekt.LifeTime		=  0;
    objekt.zeile		= 0;
    objekt.AnimLifetime = 50;
    objekt.timer		= GetTickCount();
    objekt.akt_bild     = 0;
    objekt.kollision	= false;				// Kollisionsabfrage standardmässig aus
    objekt.typ			= typ;
    objekt.bild			= NULL;
    objekt.parallax_ratio = 1;                  // Parallax-Ratio des Objekts
    // Objekt je nach Einheitentyp noch genauer einstellen
    InitUnit(&objekt);
    // Objekt in UnitList einfügen
    UnitList.push_front(objekt);
    return 0;
} //CreateObjekt


//--------Pusht inaktives Objekt in InactiveUnitList------------------------------------------------
int CreateInactiveObjekt(float x, float y, int typ)
{
    OBJEKT objekt;
    objekt.x            =  x;					// Startposition x
    objekt.y            =  y;					// Startposition y
    objekt.zx           =  0;
    objekt.zy           =  0;
    objekt.s			=  1;					// Geschwindigkeit
    objekt.LifeTime		=  0;
    objekt.zeile		= 0;
    objekt.AnimLifetime = 50;
    objekt.timer		= GetTickCount();
    objekt.akt_bild     = 0;
    objekt.kollision	= false;				// Kollisionsabfrage standardmässig aus
    objekt.typ			= typ;
    objekt.bild			= NULL;
    objekt.parallax_ratio = 1;                  // Parallax-Ratio des Objekts
    InitUnit(&objekt);                          // Noetige, um Spritedaten (Breite/Hoehe) zu bekommen
    // Objekt in InacitveUnitList einfügen
    InactiveUnitList.push_front(objekt);
    return 0;
}


//-----------Pusht bereits erstelltes Objekt in InactiveUnitList------------------------------------
int InsertInactiveObjekt(OBJEKT objekt)
{
    InactiveUnitList.push_front(objekt);
    return 0;
}


//--------------------------------------------------------------------------------------------------
void GlobalTriggerCheck(void)
{
    list<OBJEKT>::iterator it;
    for (it = InactiveUnitList.begin(); it != InactiveUnitList.end(); it++)
    {
        // * Temporary: Check, whether objects enters screen *
        float x = it->x,  y = it->y, w = it->bild->breite, h = it->bild->hoehe;

        if  (
            (x > ScrollX and x < ScrollX + RESOLUTION_X and y > ScrollY and y < ScrollY + RESOLUTION_Y)
            or
            (x + w > ScrollX and x + w < ScrollX + RESOLUTION_X and y > ScrollY and y < ScrollY + RESOLUTION_Y)
            or
            (x > ScrollX and x < ScrollX + RESOLUTION_X and y + h > ScrollY and y + h < ScrollY + RESOLUTION_Y)
            or
            (x + w > ScrollX and x + w < ScrollX + RESOLUTION_X and y + h > ScrollY and y + h < ScrollY + RESOLUTION_Y)
            )
        {

            // Push object into UnitList after deleting it from InactiveUnitList
            OBJEKT obj = (*it);
            InitEditorObjekt(&obj);                 // init velocity etc.
            UnitList.push_front((*it));
            list<OBJEKT>::iterator tempit;	        // temp iterator for deletion
            tempit = it;
            ++tempit;
            InactiveUnitList.erase(it);
            it = tempit;

        }

    }
}


//----------Fügt ein Objekt manuell in die Liste ein-------------------------------------------
int InsertObjekt(OBJEKT objekt)
{
    UnitList.push_front(objekt);
    return 0;
}


//----------Aktualisiert das Sprite eines Objekts----------------------------------------------
int UpdateObjectSprite(LPOBJEKT objekt)
{
    // check if there's already passed enougth time to change the frame
    if (objekt->animiert == true)
    {
        if ((GetTickCount() - objekt->timer) > objekt->AnimLifetime)
        {
            if (objekt->akt_bild < objekt->bild->anz_anim)
            {
                objekt->akt_bild++;
            }
            else
            {
                objekt->akt_bild = 0;
            }
            // store current time in case the frame has been changed
            objekt->timer = GetTickCount();
        }
    }

    // render sprite onto screen
    float ScrollYParallax = (float)ScrollY/objekt->parallax_ratio;
    int BildschirmX = (int)((objekt->x) + (-ScrollX));
    int BildschirmY = (int)((objekt->y) + (-ScrollYParallax));
    MaleSprite(GetSDLScreen(), objekt->bild, BildschirmX, BildschirmY, objekt->akt_bild, objekt->zeile, true);

    return 0;
} //UpdateObjekt


bool pixel_collision(LPOBJEKT objekt1, LPOBJEKT objekt2)
{
    int left1, left2, over_left;
    int right1, right2, over_right;
    int top1, top2, over_top;
    int bottom1, bottom2, over_bottom;
    int over_width, over_height;
    int breite1, breite2, hoehe1, hoehe2, akt_bild1, akt_bild2;
    int i, j;
    int pixel1, pixel2;
    bool *hardmask1 = objekt1->bild->hardmask;
    bool *hardmask2 = objekt2->bild->hardmask;

    // store stuff into variables
    left1 = (int)objekt1->x;
    left2 = (int)objekt2->x;
    right1 = int(objekt1->x) + objekt1->bild->breite;
    right2 = int(objekt2->x) + objekt2->bild->breite;
    top1 = (int)objekt1->y;
    top2 = (int)objekt2->y;
    bottom1 = int(objekt1->y) + objekt1->bild->hoehe;
    bottom2 = int(objekt2->y) + objekt2->bild->hoehe;
    breite1 = objekt1->bild->breite;
    breite2 = objekt2->bild->breite;
    hoehe1 = objekt1->bild->hoehe;
    hoehe2 = objekt2->bild->hoehe;
    akt_bild1 = objekt1->akt_bild;
    akt_bild2 = objekt2->akt_bild;

    // compute the rectangle of overlap
    if (bottom1 > bottom2) over_bottom = bottom2;
    else over_bottom = bottom1;
    if (top1 < top2) over_top = top2;
    else over_top = top1;
    if (right1 > right2) over_right = right2;
    else over_right = right1;
    if (left1 < left2) over_left = left2;
    else over_left = left1;
    over_width = over_right - over_left;
    over_height = over_bottom - over_top;

    // compute starting offsets
    i = (over_top - top1) * breite1 + (over_left-left1); // x-offset
    pixel1 = breite1*hoehe1*akt_bild1+i;                 // position of current pixel (in the current animation frame)
    // in hardmask
    j = (over_top - top2) * breite2 + (over_left-left2);
    pixel2 = breite2*hoehe2*akt_bild2+j;

    // pixel-wise scan
    bool collision = false;
    for (int k = 0; k < over_height; k++)
    {
        for (int l = 0; l < over_width; l++)
        {
            if (hardmask1[pixel1] && hardmask2[pixel2])
            {
//              drawpixel(screen, over_left + l, over_top + k, 255, 255, 0);
                collision = true;
            }
//          drawpixel(screen, left2+(pixel2%breite2), top2+((pixel2-(pixel2%breite2))/breite2), 0, 255, 255);
//          drawpixel(screen, left1+(pixel1%breite1), top1+((pixel1-(pixel1%breite1))/breite1), 255, 0, 0);
            pixel1++;
            pixel2++;
        }
        pixel1 += breite1 - over_width;
        pixel2 += breite2 - over_width;
    }

    return collision;
}


// Helper function: Checks, whether two objects share at least one group
bool HaveGroupsShared(OBJEKT &obj1, OBJEKT &obj2)
{
    for(int i = 0; i < (signed int)obj1.groups.size(); i++)
    {
        for(int j = 0; j < (signed int)obj2.groups.size(); j++)
        {
            if(obj1.groups[i] == obj2.groups[j])
                return true;
        }
    }
    return false;
}


//----------Führt eine Kollisionsabfrage für jedes Element der Liste mit seinen Nachfolgern aus--
void GlobalCollisionCheck(void)
{
    list<OBJEKT>::iterator objekt1;
    // Check each element with its successors
    for (objekt1 = UnitList.begin(); objekt1 != UnitList.end(); objekt1++)
    {
        list<OBJEKT>::iterator objekt2;
        objekt2 = objekt1;
        objekt2++;
        for (;objekt2 != UnitList.end(); objekt2++)
        {
            if ((*objekt1).kollision == true && (*objekt2).kollision == true)
            {
                // bounding-box collision check
                int obj1_x2 = int((*objekt1).x) + (*objekt1).bild->breite;
                int obj1_y2 = int((*objekt1).y) + (*objekt1).bild->hoehe;
                int obj2_x2 = int((*objekt2).x) + (*objekt2).bild->breite;
                int obj2_y2 = int((*objekt2).y) + (*objekt2).bild->hoehe;
                if (((*objekt1).x < obj2_x2) && ((*objekt1).y < obj2_y2) && (obj1_x2 > (*objekt2).x) && (obj1_y2 > (*objekt2).y))
                {
                    // drop this one if objects share a group
                    if(HaveGroupsShared(*objekt1, *objekt2) == true)
                    {
                        continue;
                    }
                    // pixel-based collision-check if the bounding boxes overlap
                    if (pixel_collision(&(*objekt1), &(*objekt2)))
                    {
                        (*objekt1).energie -= ((*objekt2).schaden)/2;
                        (*objekt2).energie -= ((*objekt1).schaden)/2;
                    }
                }
            }
        }
    }
}


//----------Löscht die gesamte Objektliste--------------------------------------------------------
void ClearUnitList(void)
{
    UnitList.clear();
}


int SetPropertiesFromFile(char filename[128])
{/*
    FILE *file = fopen("
    outfile1
    return 0;*/
    return 0;
}

