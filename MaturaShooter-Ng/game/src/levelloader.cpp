#include "levelloader.h"
#include "objekt_basis.h"
#include "sprite.h"
#include "units.h"
#include "tiles.h"
#include "linalg.h"


FILE *file = NULL;
int num, numread;
int LevelSizeX, LevelSizeY;

// Helper function
// Transforms the y-coord. from editor- to game-space
float TransformYCoordinate(float y)
{
    float res = y - (float)LevelSizeY;
    char buf[128];
    sprintf(buf, "y-coord: %f to %f", y, res);
//    MessageBox(NULL, buf, "", NULL);
    return res;
}

LPSPRITE get_Sprite(int typ)
{
    switch(typ)
    {
        case UD_PLAYERSHIP:{
            return &SpriteArray[SD_PLAYERSHIP]; break;}
        case UD_STONE64:{
            return &SpriteArray[SD_STONE64];break;}
        case UD_MONSTER1A:{
            return &SpriteArray[SD_MONSTER1A];break;}
        case UD_MONSTER1B:{
            return &SpriteArray[SD_MONSTER1B];break;}
        case UD_MONSTER1C:{
            return &SpriteArray[SD_MONSTER1C];break;}
        case UD_SCHUSS:{
            return &SpriteArray[SD_SCHUSS];break;}
        case UD_EXTRA:{
            return &SpriteArray[SD_EXTRA];break;}
        case UD_SPOREA:{
            return &SpriteArray[SD_SPOREA];break;}
        case UD_POD:{
            return &SpriteArray[SD_POD]; break;}
        case UD_RUSHER:{
            return &SpriteArray[SD_RUSHER];break;}
        case UD_SASTER96:{
            return &SpriteArray[SD_SASTER96]; break;}
        case UD_SASTER64:{
            return &SpriteArray[SD_SASTER64]; break;}
        case UD_SASTER32:{
            return &SpriteArray[SD_SASTER32]; break;}
        case UD_GASTER96:{
            return &SpriteArray[SD_GASTER96];break;}
        case UD_GASTER64:{
            return &SpriteArray[SD_GASTER64];break;}
        case UD_GASTER32:{
            return &SpriteArray[SD_GASTER32];break;}
        case UD_MASTER32:{
            return &SpriteArray[SD_MASTER32];break;}
        case UD_MASTER64:{
            return &SpriteArray[SD_MASTER64];break;}
        case UD_MASTER96:{
            return &SpriteArray[SD_MASTER96];break;}
        case UD_GSHOOTL:{
            return &SpriteArray[SD_GSHOOTL];break;}
        case UD_GSHOOTR:{
            return &SpriteArray[SD_GSHOOTR];break;}
        case UD_WALLHL:{
            return &SpriteArray[SD_WALLHUGGERL];break;}
        case UD_WALLHR:{
            return &SpriteArray[SD_WALLHUGGERR];break;}
        case UD_DUST:{
            return &SpriteArray[SD_DUST];break;}
        case UD_PUDIVE:{
            return &SpriteArray[SD_PUDIVE];break;}
        case UD_PUINVULN:{
            return &SpriteArray[SD_PUINVULN];break;}
        case UD_PULASER:{
            return &SpriteArray[SD_PULASER];break;}
        case UD_PUDUALROCKET:{
            return &SpriteArray[SD_PUWEAPON];break;}
        case UD_PUARMOR:{
            return &SpriteArray[SD_PUARMOR];break;}
        case UD_PUSPEED:{
            return &SpriteArray[SD_PUSPEED];break;}
        case UD_PULIFE:{
            return &SpriteArray[SD_PULIFE];break;}
        case UD_PUHROCKET:{
            return &SpriteArray[SD_PUHROCKET];break;}
        case UD_BACK64A:{
            return &SpriteArray[SD_BACK64A];break;}
        case UD_EXP16:{
            return &SpriteArray[SD_EXP16];break;}
        case UD_EXP32:{
            return &SpriteArray[SD_EXP32];break;}
        case UD_EXP64:{
            return &SpriteArray[SD_EXP64];break;}
        case UD_STDROCKET:{
            return &SpriteArray[SD_STDROCKET];break;}

        case UD_BLOCK1:{
            return &SpriteArray[SD_BLOCK1];break;}
    }

    return NULL;
}


// Note: This function is not redundant - Objekts from a file should be initialised
// by this one, in-game created objects by InitUnit
// But maybe, this function here is misplaced in this file
void InitEditorObjekt(LPOBJEKT objekt)
{
    // Sprite auf Objekt legen
    objekt->bild = get_Sprite(objekt->typ);

    // Animationstimer initialisieren
    objekt->timer = GetTickCount();

    // Bewegungsvektor initialisieren
    switch(objekt->typ)
    {
        case UD_PLAYERSHIP:
        {
            objekt->groups[0] = GD_SHIP;
            objekt->zx = 0;
            objekt->zy = 0;
            objekt->v[0] = 0;
            objekt->v[1] = 0;
            break;
        }
        case UD_MONSTER1A:
        {
            objekt->groups[0] = GD_ENEMY;
            objekt->zx = PlayerShipPosX;
            objekt->zy = PlayerShipPosY;
            InitVectorDiagonal(objekt);
            break;
        }
        case UD_POD:
        {
            objekt->groups[0] = GD_ENEMY;
            InitVectorDiagonal(objekt);
            break;
        }
        case UD_RUSHER:
        {
            objekt->groups[0] = GD_ENEMY;
            objekt->v[0] = 0;
            objekt->v[1] = objekt->s;
            objekt->zx = 0;
            objekt->zy = 0;
            break;
        }
        case UD_SASTER96:
        case UD_SASTER64:
        case UD_SASTER32:
        case UD_GASTER96:
        case UD_MASTER32:
        case UD_MASTER64:
        case UD_MASTER96:
        {
            objekt->groups[0] = GD_ENEMY;
            objekt->v[0] = 0.0;
            objekt->v[1] = objekt->s;
            objekt->zx = 0;
            objekt->zy = 0;
            break;
        }
        case UD_GSHOOTR:
        case UD_GSHOOTL:
        case UD_WALLHL:
        case UD_WALLHR:
        {
            // Feldmissbrauch: zx = obere Ziel-y-Koordinate; zy = untere Ziel-y-Koordinate
            static float biasup = 50.0, biasdown = 50.0;
            objekt->groups[0] = GD_ENEMY;
            objekt->zx = objekt->y - biasup;
            objekt->zy = objekt->y + biasdown;
            objekt->v[0] = 0;
            objekt->v[1] = objekt->s;
            break;
        }
        case UD_DUST:
        {
            objekt->groups[0] = GD_MISC;
            // Zuefällige Geschwindigkeit zw. 0 und 5
            float r = (float)(rand()%5000);
            objekt->s = 0.001*r;
            // Zufälligen Geschwindigkeitsvektor für das Partikel generieren (In Bogenmass)
            int	  alpha_grad = rand()%360;					// Zufälliger Winkel zw. 0°-360°
            float alpha_rad = (float)(Pi() * alpha_grad)/180;	// Umrechnung von Grad in Radian
            objekt->v[0] = objekt->s * cos(alpha_rad);
            objekt->v[1] = objekt->s * sin(alpha_rad);
            break;
        }
        case UD_PUDIVE:
        case UD_PUINVULN:
        case UD_PULASER:
        case UD_PUDUALROCKET:
        case UD_PUARMOR:
        case UD_PUSPEED:
        case UD_PULIFE:
        case UD_PUHROCKET:
        {
            objekt->groups[0] = GD_ENEMY;
            break;
        }

        case UD_BLOCK1:
        {
            objekt->groups[0] = GD_BLOCK;
            objekt->energie = 1;
            break;
        }
        default:
        {
            objekt->zx = PlayerShipPosX;
            objekt->zy = PlayerShipPosY;
            InitVectorDiagonal(objekt);
            break;
        }
    }

    return;
}

LPOBJEKT FileFormatToGameFormat(LPFILE_OBJEKT objekt)
{
    LPOBJEKT _objekt = new OBJEKT;

    _objekt->x = objekt->x;
    // Transform y-coordinate
    _objekt->y = TransformYCoordinate(objekt->y);;
    _objekt->zx = objekt->zx;
    _objekt->zy = objekt->zy;
    _objekt->v[0] = objekt->v[0];
    _objekt->v[1] = objekt->v[1];
    _objekt->energie = objekt->energie;
    _objekt->schaden = objekt->schaden;
    _objekt->typ = objekt->typ;
    _objekt->zustand = objekt->zustand;
    _objekt->LifeTime = objekt->LifeTime;
    _objekt->s = objekt->s;
    _objekt->kollision = objekt->kollision;
    _objekt->animiert = objekt->animiert;
    _objekt->parallax_ratio = objekt->parallax_ratio;
    _objekt->zeile = objekt->zeile;
    _objekt->akt_bild = objekt->akt_bild;
    _objekt->timer = objekt->timer;
    _objekt->AnimLifetime = objekt->AnimLifetime;

    return _objekt;
}


int PrepareFileLoading(char *filename)
{
    file = fopen(filename, "rb");
    assert(file);

    // ***Read Header***
    // Number of objects to load
    int number_to_read;
    fread(&number_to_read, sizeof(int), 1, file);
    numread = number_to_read;
    // Level Size
    fread(&LevelSizeX, sizeof(int), 1, file);
    fread(&LevelSizeY, sizeof(int), 1, file);
    char buf[128];
    sprintf(buf, "%d", LevelSizeY);
//    MessageBox(NULL, buf, "", NULL);

    return number_to_read;
}


// Note:    This func. seperates between objecte and tiles:
//          If object is a tile, it is _automatically_ inserted
//          and the func. returns NULL
LPOBJEKT LoadObjekt(void)
{
    assert(file);
    static int read = 0;

    LPOBJEKT _objekt = NULL;

    LPFILE_OBJEKT objekt = new FILE_OBJEKT;
    fread(objekt, sizeof(FILE_OBJEKT), 1, file);
    if(strcmp(objekt->_class, "Tile") == 0)
    {
        float y = TransformYCoordinate(objekt->y);
        TileManager::AddTile(objekt->path, objekt->x, y);
        delete objekt;
        _objekt = NULL;
    }
    else
    {
        _objekt = FileFormatToGameFormat(objekt);
    }
    read++;


    if(read >= numread)
    {
        fclose(file);
        read = 0;
    }

    return _objekt;
}
