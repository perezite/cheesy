
#include <iostream>
#include <math.h>
#include <conio.h>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "filereader.h"

using namespace std;

using namespace std;

// Objekt-"Template" Liste
LPEDOBJEKT ObjektTemplates = NULL;
// Tile-"Template" Liste
//EditorTile *TileTemplates = NULL;
map<string, EditorTile> _TileTmplMap;


int get_value(char *line, char *value)
{
    memset(value, 0, strlen(value));

    char *temp = strchr(line, '=');
    char *crap = strchr(temp, '\n');
    if (crap!=NULL)
        *crap = ' ';
    if (temp==NULL)
        return 1;
    temp++;
    strcpy(value, temp);
    return 0;
}

int read_properties(FILE *file, EDOBJEKT *Objekt)
{
    char buffer[1024];
    char number [64];
    memset(buffer, 0, strlen(buffer));
    memset(number, 0, strlen(number));

    // Objekt nullen
    memset(Objekt, 0, sizeof(EDOBJEKT));

    // 8 Integers auslesen
    fgets(buffer, 1024, file);
    get_value(buffer, number);
    printf("%d", atoi(number));
    Objekt->typ = atoi(number);
    fgets(buffer, 1024, file);
    get_value(buffer, number);
    printf("%d", atoi(number));
    Objekt->energie = atoi(number);
    fgets(buffer, 1024, file);
    get_value(buffer, number);
    printf(" %d", atoi(number));
    Objekt->schaden = atoi(number);
    fgets(buffer, 1024, file);
    get_value(buffer, number);
    printf(" %d", atoi(number));
    Objekt->zustand = atoi(number);
    fgets(buffer, 1024, file);
    get_value(buffer, number);
    printf(" %d", atoi(number));
    Objekt->akt_bild = atoi(number);
    fgets(buffer, 1024, file);
    get_value(buffer, number);
    printf(" %d", atoi(number));
    Objekt->zeile = atoi(number);
    fgets(buffer, 1024, file);
    get_value(buffer, number);
    printf(" %d", atoi(number));
    Objekt->LifeTime = atoi(number);
    fgets(buffer, 1024, file);
    get_value(buffer, number);
    printf(" %d", atoi(number));
    Objekt->AnimLifetime = (DWORD)atoi(number);

    // 3 Floats auslesen
    fgets(buffer, 1024, file);
    get_value(buffer, number);
    printf(" %f", atof(number));
    Objekt->parallax_ratio = atof(number);
    fgets(buffer, 1024, file);
    get_value(buffer, number);
    printf(" %f", atof(number));
    Objekt->s = atof(number);

    // 2 Booleans auslesen
    bool value;
    fgets(buffer, 1024, file);
    get_value(buffer, number);
    printf(" %d", atoi(number));
    atoi(number) ? value = true : value = false;
    Objekt->kollision = value;
    fgets(buffer, 1024, file);
    get_value(buffer, number);
    printf(" %d", atoi(number));
    atoi(number) ? value = true : value = false;
    Objekt->animiert = value;

    // 2 Strings auslesen
    fgets(buffer, 1024, file);
    get_value(buffer, number);
    printf("bla %s", number);
    printf(" strlen; %d", strlen(number));
    strcpy(Objekt->path, STDPATH);          // Standardpfad vorne dranhaengen
    strcat(Objekt->path, number);

    memset(buffer, 0, sizeof(buffer));
    fgets(buffer, 1024, file);
    get_value(buffer, number);
    printf("bla %s", number);
    printf(" strlen; %d", strlen(number));
    strcpy(Objekt->_class, number);
//    MessageBox(NULL, Objekt->_class, "foo", NULL);
    printf("\n");

    // Editor stuff
    Objekt->selected = false;
    Objekt->activated = true;

    return 0;
}

int show_properties(LPEDOBJEKT lpObjekt)
{
    printf("Zeugs\n");
    printf("path: %s\n", lpObjekt->path);
    printf("x: %f\n", lpObjekt->x);
    printf("y: %f\n", lpObjekt->y);
    printf("zx: %f\n", lpObjekt->zx);
    printf("zy: %f\n", lpObjekt->zy);
    printf("vx: %f\n", lpObjekt->v[0]);
    printf("zy: %f\n", lpObjekt->v[1]);
    printf("akt_bild: %d\n", lpObjekt->akt_bild);
    printf("animiert: %d\n", lpObjekt->animiert);
    printf("energie: %d\n", lpObjekt->energie);
    printf("kollision: %d\n", lpObjekt->kollision);
    printf("LifeTime: %d\n", (int)lpObjekt->LifeTime);
    printf("AnimLifeTime: %d\n", (int)lpObjekt->AnimLifetime);
    printf("parallax_ratio: %f\n", (float)lpObjekt->parallax_ratio);
    printf("s: %f\n", lpObjekt->s);
    printf("schaden: %d\n", lpObjekt->schaden);
    printf("timer: %d\n", lpObjekt->timer);
    printf("class: %d\n", (int)lpObjekt->_class);
    printf("typ: %d\n", lpObjekt->typ);
    printf("zeile: %d\n", lpObjekt->zeile);
    printf("zustand: %d\n", lpObjekt->zustand);

    return 0;
}


// helper function
// parse given line of tile.txt
int ParseLine(string line, string values[5])
{
    string sep = " ";
    size_t pos0, pos1;
    pos0 = 0;

    for(int i = 0; i < 5; i++)
    {
        pos1 = line.find(sep, pos0 + 1);
        if(pos1==string::npos and i != 4)
        {
            return 1;
        }
        size_t delta = pos1 - pos0;
        values[i] = line.substr(pos0, delta);
        pos0 = pos1;
    }
    return 0;
}


int ReadObjektTemplates(void)
{
    FILE *file = fopen("objects.ini", "r");
    char buffer[128];

    // Durchgehen und Anzahl Objekte in File zaehlen
    int found_objects = 0;
    while (fgets(buffer, 1024, file)!=NULL)
    {
        if (buffer[0] == '[')
        {
            for (int i = 1; (int)i < (int)strlen(buffer); i++)
            {
                if (buffer[i] == ']')
                {
                    found_objects++;
                    break;
                }
            }
        }
    }

    // Durchgehen und Anzahl Tiles in File zählen
    string line;
    ifstream myfile ("Tile.txt");
    int found_tiles = 0;
    if (myfile.is_open())
    {
        while (! myfile.eof() )
        {
            getline (myfile,line);
            found_tiles++;
        }
        myfile.close();
    }

    // Objektanzahl ausgeben
    printf("%d Objekte\n", found_objects);

    // Platz fuer die Objekt Templates und Tile Templates reservieren
    ObjektTemplates = new EDOBJEKT[found_objects + found_tiles];

    // Objekte einlesen
    fseek(file, 0, SEEK_SET);
    int objects_read = 0;
    while (fgets(buffer, 1024, file)!=NULL)
    {
        if (buffer[0] == '[')                       // Pruefen, ob akt. Zeile Titelzeile
        {
            for (int i = 1; (int)i < (int)strlen(buffer); i++) // ''
            {
                if (buffer[i] == ']')               // ''
                {
                    read_properties(file, &(ObjektTemplates[objects_read]));
                    objects_read++;
                    break;
                }
            }
        }
    }

    // Tiles einlesen
    ifstream myfile2 ("tile.txt");
    if (myfile2.is_open())
    {
        for(int i = 0; i < found_tiles; i++)
        {
            getline (myfile2,line);
            string values[5];
            EDOBJEKT *NewObjekt = &(ObjektTemplates[objects_read + i]);
            memset(NewObjekt, 0, sizeof(EDOBJEKT));

            ParseLine(line, values);
            string name = values[0];
            sprintf(NewObjekt->path, values[0].c_str());
            sprintf(NewObjekt->_class, "Tile");
            NewObjekt->breite = atoi(values[2].c_str());
            NewObjekt->hoehe = atoi(values[3].c_str());
            NewObjekt->kollision = atoi(values[4].c_str());
            NewObjekt->selected = false;
            NewObjekt->activated = true;

        }
        myfile2.close();
    }

    // Objekte ausgeben
    for (int i = 0; i < found_objects; i++)
    {
        show_properties(&(ObjektTemplates[i]));
    }

    printf("%d\n", ObjektTemplates[0].schaden);

    printf("sizeof: %d\n", sizeof(ObjektTemplates[0]));

//    ReadTileTemplates("tile.txt");

    return found_objects + found_tiles;
}

// Helper function
// Reads Tiles from tile.txt into TileTemplate List
void ReadTileTemplates(string filename)
{
    string line;
    ifstream myfile (filename.c_str());

    // Check how much elements are in the file
    int found_objects = 0;
    if (myfile.is_open())
    {
        while (! myfile.eof() )
        {
            getline (myfile,line);
            found_objects++;
        }
        myfile.close();
    }



    return;
}



LPEDOBJEKT GetObjektTemplateList(void)
{
    return ObjektTemplates;
}


/*
int main()
{
    int anz_objekte = ReadObjektTemplates();
    printf("-------------------------------------------- \n");
   printf("%d, %d\n", ObjektTemplates[12].schaden, anz_objekte);
//    OBJEKT ObjektGetObjektTemplates()
    return 0;
}*/

