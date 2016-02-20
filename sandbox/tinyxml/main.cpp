// Source: http://www.dinomage.com/2012/01/tutorial-using-tinyxml-part-1/

#include <iostream>

#include "tinyxml.h"

int main()
{
	TiXmlDocument doc;
	if(doc.LoadFile("test.xml") == false) {
		std::cout << "error loading XML" << std::endl;
	}

	TiXmlElement *root = doc.FirstChildElement();

	if (root) {
			for (TiXmlElement *elem = root->FirstChildElement(); 
				elem != NULL; elem = elem->NextSiblingElement()) {
				std::string elemName = elem->Value();
				
				if (elemName == "Element1") {
					const char* attr = elem->Attribute("attribute1"); 
					if (attr) {
						std::cout << std::string(attr) << std::endl;
					}
				}
				
				else if( elemName == "Element2") {
					const char* attr = elem->Attribute("attribute2");
					if (attr) {
						std::cout << std::string(attr) << std::endl;
					} 
					attr = elem->Attribute("attribute3");
					if (attr) {
						std::cout << std::string(attr) << std::endl;
					}
					
					// TODO: Get Element3
				}
			}
	}
	
	return 0;
}
