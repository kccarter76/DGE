#pragma once

#include "stdafx.h"
#include <mutex>

namespace HLE
{
	//TODO:: we need to lock the mutex however we are having some trouble with that.
	//forward declaration
	template <class T> class ENGINE_API Singleton;

	template <class T> class ENGINE_API SingletonAccess 
		//: public std::lock
	{
		friend class Singleton<T>;
		std::mutex mutex;
		T *obj;

		SingletonAccess(T* t) : obj(t) { 
			mutex.lock();
		}
		SingletonAccess operator=(const SingletonAccess&);
	public:
		SingletonAccess(const SingletonAccess& sa) : obj(sa.obj) { }
		~SingletonAccess(void) {
			mutex.unlock();
		}

		T* operator->() { return obj; }
	};

	template <class T> class ENGINE_API Singleton 
	{
	public:
		static SingletonAccess<T> Get()
		{
			static T obj;

			return SingletonAccess<T>(&obj);
		}
	};
};

