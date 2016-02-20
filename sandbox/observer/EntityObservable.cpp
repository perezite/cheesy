#include "EntityObservable.h"

void EntityObservable::attach(IEntityObserver *observer) 
{
	m_observers.push_back(observer);
}

void EntityObservable::detach(IEntityObserver *observer)
{
	m_observers.erase(std::remove(m_observers.begin(), m_observers.end(), observer), m_observers.end()); 
}

void EntityObservable::notify(EntityObservable *observable)
{
	for(vector<*IEntityObserver>::const_iterator iter = m_observers.begin(); iter != m_observers.end(); ++iter) {
		if (iter != NULL) {
			(*iter)->update(this);
		}
	}
}