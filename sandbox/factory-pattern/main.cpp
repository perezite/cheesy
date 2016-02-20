#include "Base.h"
#include "BaseFactory.h"
#include "DerivedA.h"
#include "DerivedB.h"
#include <map>
#include <string>
#include <iostream>

int main()
{		
	/* Test for factory class
	 */
	BaseFactory& factory = BaseFactory::getInstance(); 
	factory.registerClass<DerivedA>("DerivedA");
	factory.registerClass<DerivedB>("DerivedB");	
	Base *instance1 = factory.create("DerivedA");
	Base *instance2 = factory.create("DerivedB");
	Base *instance3 = factory.create("DerivedB");
	instance1->printType();
	instance2->printType();
	instance3->printType();	
	
	return 0;
}
