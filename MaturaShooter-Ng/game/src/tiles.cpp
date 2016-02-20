
#include "tiles.h"
#include "dxinit.h"
#include "units.h"
#include "objekt_basis.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;




//======== Class: TileManager =============================================

// **********************
// Static initializations
// **********************

map<string, LPSPRITE> TileManager::spritemap;
list<Tile> TileManager::tilelist;


TileManager::TileManager()
{
    LoadTilesFromFile("graphics\\tile.txt");
}


TileManager::~TileManager()
{
    // show content:
    map<string, LPSPRITE>::iterator it;

    for ( it=spritemap.begin() ; it != spritemap.end(); it++ )
    {
        delete (*it).second;
    }

}


TileManager::TileManager(string filename)
{
    LoadTilesFromFile(filename);
}


// helper function
// parse given line of tile.txt
int ParseLine(string line, string values[4])
{
    string sep = " ";
    size_t pos0, pos1;
    pos0 = 0;

    for(int i = 0; i < 4; i++)
    {
        pos1 = line.find(sep, pos0 + 1);
        if(pos1==string::npos and i != 3)
        {
            return 1;
        }
        size_t delta = pos1 - pos0;
        values[i] = line.substr(pos0, delta);
        pos0 = pos1;
    }
    return 0;
}


// Should be named: load tile sprites from file, well..
int TileManager::LoadTilesFromFile(string filename)
{
    string line;
    ifstream myfile (filename.c_str());

    if (myfile.is_open())
    {
        while (! myfile.eof() )
        {
            getline (myfile,line);
            string values[4];
            // Parse
            ParseLine(line, values);
            // Create sprite
            LPSPRITE spriteptr = new SPRITE;
            int width = atoi(values[2].c_str()),
                height = atoi(values[3].c_str()),
                anzanim = atoi(values[1].c_str());

            CreateSprite(spriteptr, values[0].c_str(), anzanim, width, height, 0);
            spritemap[values[0].c_str()] = spriteptr;
        }
        myfile.close();
    }

    else cout << "Unable to open file";

    return 0;
}


int TileManager::AddTile(string ident, float x, float y)
{
    LPSPRITE sprite = GetSprite(ident);
    Tile tile(sprite, x, y);
    tilelist.push_back(tile);

    return 0;
}


LPSPRITE TileManager::GetSprite(string ident)
{
    return spritemap[ident];
}


int TileManager::Update(void)
{
    list<Tile>::iterator it = tilelist.begin();

    // --- Draw ---
    for (;it != tilelist.end(); it++)
    {
        it->Draw();
    }

    // Check each tile for collision against objects (bounding box collision)
    list<OBJEKT>::iterator ulit;
    for(it = tilelist.begin(); it != tilelist.end(); it++)
    {
        for(ulit = UnitList.begin(); ulit != UnitList.end(); ulit++)
        {
            // bounding-box collision check
            float o1x1 = it->GetX();
            float o1y1 = it->GetY();
            float o1x2 = o1x1 + it->GetW();
            float o1y2 = o1y1 + it->GetH();
            float o2x1 = ulit->x;
            float o2y1 = ulit->y;
            float o2x2 = o2x1 + (float)ulit->bild->breite;
            float o2y2 = o2y1 + (float)ulit->bild->hoehe;

            if(o2x1 < o1x2 and o2y1 < o1y2 and o2x2 > o1x1 and o2y2 > o1y1)
            {
                ulit->energie -= 1000;
            }
        }
    }

    return 0;
}


//======== Class: Tile =======================================================


Tile::Tile(LPSPRITE _sprite, float x, float y, bool collision)
{
    sprite = _sprite;
    posx = x;
    posy = y;
    width = (float)sprite->breite;
    height = (float)sprite->hoehe;
    m_collision = collision;

}


Tile::Tile(string filename, int breite, int hoehe, float x, float y, bool collision)
{
    TileManager bla("blub");
    sprite = bla.GetSprite(filename);

    posx = x;
    posy = y;
    width = breite;
    height = hoehe;
    m_collision = collision;

    return;
}


Tile::Tile(void)
{
    LPSPRITE newspr = new SPRITE;
    CreateSprite(newspr, "graphics\\Back64A2.bmp", 1, 64, 64, 0);
    sprite = newspr;
    posx = 300;
    posy = 300;
    width = (float)sprite->breite;
    height = (float)sprite->hoehe;
    return;
}

int Tile::SetSprite(LPSPRITE _sprite)
{
    sprite = _sprite;

    return 0;
}


void Tile::Draw(void)
{
    // render sprite onto screen
    int scrx = (int)((this->posx) + (-ScrollX));
    int scry = (int)((this->posy) + (-ScrollY));
    MaleSprite(GetSDLScreen(), sprite, scrx, scry, 0, 0, true);
}

float Tile::GetX()
{
    return posx;
}

float Tile::GetY()
{
    return posy;
}

float Tile::GetW()
{
    return width;
}

float Tile::GetH()
{
    return height;
}
