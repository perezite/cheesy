#define PLATFORM_2

#include "Device.h"
#include "platform1/Platform1Device.h"
#include <iostream>

using namespace std;

int main() 
{
	Device *myDevice = new PlatformDevice;
	Platform1Device *myPlatform1Device = new Platform1Device;
	
	myPlatform1Device->print();
	myPlatform1Device->platformDependentPrint();
	
	myDevice->print();
	//myDevice->platformDependentPrint();			// compiler error, as wanted
	
	return 0;
}
