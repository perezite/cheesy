
#ifndef __SINGLETON_H_
#define __SINGLETON_H_

#define GetSingleton(x) x::Get()
#define GetST(x) x::Get()

#include <iostream>

using namespace std;

template<class T>
class Singleton
{
protected:
    static T *m_pSingleton;

public:
    // C'TOR
    Singleton()
    {
    }

    // D'TOR
    virtual ~Singleton()
    {
    }

    // GET
    inline static T* Get()
    {
        if (!m_pSingleton)
        {
            cout << "Constructing new Singleton" << endl;
            m_pSingleton = new T;
            cout << "wapoint 3" << endl;
        }

        return m_pSingleton;
    }

    static void Del()
    {
        if (m_pSingleton)
        {
            delete m_pSingleton;
            m_pSingleton = 0;
        }
    }

};

template<class T>
T* Singleton<T>::m_pSingleton = 0;

#endif
