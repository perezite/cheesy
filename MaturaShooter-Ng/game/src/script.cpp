#include "script.h"
#include "objekt_basis.h"
#include "units.h"
#include <list>
using namespace std;

LPOBJEKT current_script_objekt = NULL;


void SCRIPTITEM2::bla(int x)
{
    return;
}

LPOBJEKT get_script_objekt(void)
{
    return current_script_objekt;
}

void set_script_objekt(LPOBJEKT objekt)
{
    current_script_objekt = objekt;
}

void AddScriptItem(LPOBJEKT objekt, void(*_fun)(void), TIMEVAR _timer, bool _timed)
{
    list<SCRIPTITEM2> itemlist = objekt->script.Items;
    SCRIPTITEM2 newitem;
//    newitem.fun = _fun;
    newitem.timer = _timer;
    newitem.timed = _timed;
    itemlist.push_back(newitem);
}

void ExecScript(LPSCRIPT script)
{
    // Check for empty list
    if((script->Items).empty())
        return;
    // TODO: Check for timer overflow
          /*/*/
    // Exec current scriptitem
    SCRIPTITEM2 item = *(script->current);
//    item.fun();
}

void delete_this_script_item(void)
{
    LPOBJEKT objekt = get_script_objekt();
    SCRIPT script = objekt->script;
    list<SCRIPTITEM2>::iterator this_script_item = script.current;
    script.current++;
    script.Items.erase(this_script_item);
}

void next_script_item(void)
{
    LPOBJEKT objekt = get_script_objekt();
    SCRIPT script = objekt->script;
    script.current++;
}

void set_attack_playership(void)
{
    LPOBJEKT objekt = get_script_objekt();
    objekt->zx = PlayerShipPosX;
    objekt->zy = PlayerShipPosY;
    next_script_item();
}

void move_attacking(void)
{
    LPOBJEKT objekt = get_script_objekt();
    MoveDiagonal(objekt);
}

