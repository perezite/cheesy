#ifndef FILEREADER_H
#define FILEREADER_H

#define STDPATH "graphics\\"

#include "object.h"
#include <map>

using namespace std;


int ReadObjektTemplates(void);
void ReadTileTemplates(string filename);
LPEDOBJEKT GetObjektTemplateList(void);

#endif
