#pragma once

#include "../DeviceInterface.h"

class Platform1Device : public Device
{
public:
	void print(void);
	
	void platformDependentPrint(void);
};
