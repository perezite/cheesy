#pragma once
#include <list>
#include "IEntityObserver.h"

class EntityObservable 
{
	private:
		std::vector<IEntityObserver*> m_observers;

	public: 	
		void attach(IEntityObserver *observer);
		void detach(IEntityObserver *observer);
		void notify(EntityObservable *observable);
}