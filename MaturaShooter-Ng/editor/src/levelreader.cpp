
#include <iostream>
#include <windows.h>
#include "levelreader.h"

using namespace std;

//====================================================================================================

FILE *file = NULL;
int num, numread;
int SaveObjekt(LPEDOBJEKT objekt);
int PrepareFileSaving(const char *filename,int num_to_save);
LPEDOBJEKT LoadObjekt(void);
int PrepareFileLoading(const char *filename);
LPFILE_OBJEKT EditorFormatToFileFormat(LPEDOBJEKT objekt);
LPEDOBJEKT FileFormatToEditorFormat(LPFILE_OBJEKT objekt);

/*int main()
{
    const int anz = 3;
    EDOBJEKT objekte[anz];
    for(int i = 0; i < 3; i++)
    {
        objekte[i].x = 1.5*(float)i;
    }

    PrepareFileSaving("blub.bla", 3);
    for(int i = 0; i < 3; i++)
    {
        SaveObjekt(&objekte[i]);
    }

    LPEDOBJEKT objekte2[3];
    int bla = PrepareFileLoading("blub.bla");
    cout << "blub " << bla << endl;
    for(int i = 0; i < bla; i++)
    {
        objekte2[i] = LoadObjekt();
    }

    for(int i = 0; i < 3; i++)
    {
        cout << "blubber " << objekte2[i]->x << endl;
    }

}*/


int PrepareFileSaving(const char *filename, int num_to_save, int LevelSizeX, int LevelSizeY)
{
    file = fopen(filename, "wb");
    assert(file);
    num = num_to_save;

    char buf[128];
    sprintf(buf, "%d", num_to_save);
//    MessageBox(NULL, buf, "PrepareFileSaving", NULL);

    // *** Write Header ***
    // Number of objects to save
    fwrite(&num_to_save, sizeof(int), 1, file);
    // Size of Level
    fwrite(&LevelSizeX, sizeof(int), 1, file);
    fwrite(&LevelSizeY, sizeof(int), 1, file);

    return 0;
}

int SaveObjekt(LPEDOBJEKT objekt)
{
    assert(file);
    static int read = 0;

    LPFILE_OBJEKT FileObjekt = EditorFormatToFileFormat(objekt);
    fwrite(FileObjekt, sizeof(FILE_OBJEKT), 1, file);
    read++;

    if (read >= num)
    {
        fclose(file);
        read = 0;
    }

    return 0;
}

int PrepareFileLoading(const char *filename, int &LevelSizeX, int &LevelSizeY)
{
    file = fopen(filename, "rb");
    assert(file);

    int number_to_read;
    // *** Read Header ***
    // Number of objects to load
    fread(&number_to_read, sizeof(int), 1, file);
    numread = number_to_read;
    // Level Size
    fread(&LevelSizeX, sizeof(int), 1, file);
    fread(&LevelSizeY, sizeof(int), 1, file);

    char buf[128];
    sprintf(buf, "%d", numread);
//    MessageBox(NULL, buf, "PrepareFileLoading", NULL);


    return number_to_read;
}

LPEDOBJEKT LoadObjekt(void)
{
    assert(file);
    static int read = 0;

    LPFILE_OBJEKT objekt = new FILE_OBJEKT;
    fread(objekt, sizeof(FILE_OBJEKT), 1, file);
    LPEDOBJEKT _objekt = FileFormatToEditorFormat(objekt);
    read++;

    if (read >= numread)
    {
        fclose(file);
        read = 0;
    }

    return _objekt;
}

LPFILE_OBJEKT EditorFormatToFileFormat(LPEDOBJEKT objekt)
{
    LPFILE_OBJEKT _objekt = new FILE_OBJEKT;

    _objekt->x = objekt->x;
    _objekt->y = objekt->y;
    _objekt->zx = objekt->zx;
    _objekt->zy = objekt->zy;
    _objekt->v[0] = objekt->v[0];
    _objekt->v[1] = objekt->v[1];
    _objekt->energie = objekt->energie;
    _objekt->schaden = objekt->schaden;
    strcpy(_objekt->_class, objekt->_class);
    MessageBox(NULL, _objekt->_class, "saving", NULL);
    _objekt->typ = objekt->typ;
    _objekt->zustand = objekt->zustand;
    _objekt->LifeTime = objekt->LifeTime;
    _objekt->s = objekt->s;
    _objekt->kollision = objekt->kollision;
    _objekt->animiert = objekt->animiert;
    _objekt->parallax_ratio = objekt->parallax_ratio;
    strcpy(_objekt->path, objekt->path);
    _objekt->breite = objekt->breite;
    _objekt->hoehe = objekt->hoehe;
    _objekt->zeile = objekt->zeile;
    _objekt->akt_bild = objekt->akt_bild;
    _objekt->timer = objekt->timer;
    _objekt->AnimLifetime = objekt->AnimLifetime;

    return _objekt;
}


LPEDOBJEKT FileFormatToEditorFormat(LPFILE_OBJEKT objekt)
{
    LPEDOBJEKT _objekt = new EDOBJEKT;

    _objekt->x = objekt->x;
    _objekt->y = objekt->y;
    _objekt->zx = objekt->zx;
    _objekt->zy = objekt->zy;
    _objekt->v[0] = objekt->v[0];
    _objekt->v[1] = objekt->v[1];
    _objekt->energie = objekt->energie;
    _objekt->schaden = objekt->schaden;
    strcpy(_objekt->_class, objekt->_class);
    _objekt->typ = objekt->typ;
    _objekt->zustand = objekt->zustand;
    _objekt->LifeTime = objekt->LifeTime;
    _objekt->s = objekt->s;
    _objekt->kollision = objekt->kollision;
    _objekt->animiert = objekt->animiert;
    _objekt->parallax_ratio = objekt->parallax_ratio;
    strcpy(_objekt->path, objekt->path);
    _objekt->breite = objekt->breite;
    _objekt->hoehe = objekt->hoehe;
    _objekt->zeile = objekt->zeile;
    _objekt->akt_bild = objekt->akt_bild;
    _objekt->timer = objekt->timer;
    _objekt->AnimLifetime = objekt->AnimLifetime;

    return _objekt;
}
