
#ifndef __SCRIPT_H_
#define __SCRIPT_H_

#include <windows.h>
#include <list>
using namespace std;

typedef DWORD TIMEVAR;

class SCRIPTITEM
{
    public:
        TIMEVAR timer;
        bool timed;
    private:
        virtual void bla(int x)=0;
};

class SCRIPTITEM2 : public SCRIPTITEM
{
    private:
        void bla(int x);
};

typedef struct SCRIPT
{
    list<SCRIPTITEM2>::iterator current;
    list<SCRIPTITEM2> Items;
}SCRIPT, *LPSCRIPT;

#endif
