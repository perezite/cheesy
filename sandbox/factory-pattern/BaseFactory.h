#include <string>
#include "Base.h"
#include <map>

class BaseFactory
{
private:
	typedef std::map<std::string, Base*(BaseFactory::*)()> map_type;	
	map_type map;

protected:
	BaseFactory() 
	{
	}
	
	~BaseFactory()
	{
	}
	
	template<class T> 
	Base * createInstance() 
	{ 
		return new T; 
	}

public:
	static BaseFactory &getInstance()
	{
		static BaseFactory instance;
		return instance;
	}
	
	Base *create(const char *type)
	{
		std::string typeStr(type);
		Base*(BaseFactory::*ptr2Member)() = map[typeStr]; 
		Base *instance = (this->*ptr2Member)();
		return instance;
	}
	
	template<class T>
	void registerClass(const char* id)
	{
		std::string idStr(id);
		map[idStr] = &BaseFactory::createInstance<T>;
	}
	
};
