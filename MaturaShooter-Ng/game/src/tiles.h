//**************************************************************************//
//File: tiles.h 														    //
//Enthält Funktionen für die Tiledarstellungen	    						//
//**************************************************************************//

#ifndef __TILES_H_
#define __TILES_H_



#include "sprite.h"
#include <map>
#include <string>
#include <list>

using namespace std;

class Tile
{
public:
        Tile(void);
        Tile(LPSPRITE _sprite, float x, float y, bool collision = false);
        Tile(string filename, int breite, int hoehe, float x, float y, bool collision = false);
        int SetSprite(LPSPRITE _sprite);
        void Draw(void);
        // GET
        float GetX();
        float GetY();
        float GetW();
        float GetH();
private:
        char name[128];
        float posx, posy;
        float height, width;
        int _class;
        bool m_collision;
        LPSPRITE sprite;
};

class TileManager
{
public:
        TileManager(string filename);
        TileManager();
        ~TileManager();
        static LPSPRITE GetSprite(string ident);
        static int AddTile(string ident, float x, float y);
        static int LoadTilesFromFile(string filename);
        static int Update(void);
private:
        static map<string, LPSPRITE> spritemap;
        static list<Tile> tilelist;
};


#endif
