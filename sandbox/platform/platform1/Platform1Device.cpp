#include "Platform1Device.h"
#include <iostream>

using namespace std;

void Platform1Device::print(void)
{
	cout << "Platform 1 Device print" << endl;
}

void Platform1Device::platformDependentPrint(void) 
{
	cout << "Platform 1 Device - Platform dependent print" << endl;
}
