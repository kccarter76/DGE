#pragma once

#include "stdafx.h"
#include <mutex>

namespace DGE
{
	//TODO:: we need to lock the mutex however we are having some trouble with that.
	//forward declaration
	template <class T> class DGE_API Singleton;

	template <class T> class DGE_API SingletonAccess 
		//: public std::lock
	{
		friend class Singleton<T>;
		std::mutex mutex;
		T *obj;

		SingletonAccess(T* t) : obj(t) {}
		SingletonAccess operator=(const SingletonAccess&);
	public:
		SingletonAccess(const SingletonAccess& sa) : obj(sa.obj) {}

		T* operator->() { return obj; }
	};

	template <class T> class DGE_API Singleton 
	{
	public:
		static SingletonAccess<T> Get()
		{
			static T obj;

			return SingletonAccess<T>(&obj);
		}
	};

	class Test
		: Singleton<Test>
	{
	public:
		Test(void);
		~Test(void);

		READONLY_PROPERTY(bool, Is);
		GET(Is) { return true; };
	};
};

