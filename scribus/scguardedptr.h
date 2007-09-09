/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCGUARDEDPTR_H
#define SCGUARDEDPTR_H

#include "scribusapi.h"
#include "assert.h"
/*
A replacement for QPointer
Does not rely on QObject, and provides faster destructor
*/ 

template<typename T>
class ScGuardedPtrData
{
public:

	int refs;
	T* pointer;

	ScGuardedPtrData(void) { pointer = NULL; refs = 0; }
	ScGuardedPtrData(T* ptr) { pointer = ptr; refs = 0; }
};

template<typename T>
class ScGuardedPtr
{
protected:
	ScGuardedPtrData<T> *data;
public:
	ScGuardedPtr(void);
	ScGuardedPtr(T* ptr);
	ScGuardedPtr(const ScGuardedPtr& gPtr);
	~ScGuardedPtr();

	ScGuardedPtr& operator=(const ScGuardedPtr& gPtr);
	bool operator==( const ScGuardedPtr<T> &p ) const { return (T*)(*this) == (T*) p;}
	bool operator!= ( const ScGuardedPtr<T>& p ) const { return !( *this == p ); }

	T* operator->() const { return (T*)(data ? data->pointer : 0); }
	T& operator*() const { return *((T*)(data ? data->pointer : 0)); }
	operator T*() const { return (T*)(data ? data->pointer : 0); }

	void deref(void);
};

template<typename T>
class ScGuardedObject : public ScGuardedPtr<T>
{
public:
	ScGuardedObject(T* ptr);
	ScGuardedObject(const ScGuardedObject& gPtr);
	~ScGuardedObject();

	ScGuardedObject& operator=(const ScGuardedObject& gPtr);
	bool operator==( const ScGuardedObject<T> &p ) const { return (T*)(*this) == (T*) p;}
	bool operator!= ( const ScGuardedObject<T>& p ) const { return !( *this == p ); }

	void nullify(void);
};

template<typename T>
ScGuardedPtr<T>::ScGuardedPtr(void)
{
	data = new ScGuardedPtrData<T>();
	++(data->refs);
};

template<typename T>
ScGuardedPtr<T>::ScGuardedPtr(T* ptr)
{
	data = new ScGuardedPtrData<T>(ptr);
	++(data->refs);
};

template<typename T>
ScGuardedPtr<T>::ScGuardedPtr(const ScGuardedPtr& other)
{
	data = other.data;
	++(data->refs);
};

template<typename T>
ScGuardedPtr<T>::~ScGuardedPtr()
{
	deref();
};

template<typename T>
ScGuardedPtr<T>& ScGuardedPtr<T>::operator=(const ScGuardedPtr& other)
{
	// We do not copy the owner member
	if (data != other.data)
	{
		++(other.data->refs);
		deref();
		data = other.data;
	}
	return *this;
};

template<typename T>
void ScGuardedPtr<T>::deref(void)
{
	if (data && --(data->refs) == 0)
	{
		delete data;
		data = NULL;
	}
};

template<typename T>
ScGuardedObject<T>::ScGuardedObject(T* ptr) : ScGuardedPtr<T>(ptr)
{
};

template<typename T>
ScGuardedObject<T>::ScGuardedObject(const ScGuardedObject& other)
{
	this->data=NULL;
	// Must never be used
	assert(false);
};

template<typename T>
ScGuardedObject<T>& ScGuardedObject<T>::operator=(const ScGuardedObject& other)
{
	// We do nothing, think about an object being assigned to another
	// The object being assigned must keep track of its own data
	return *this;
};

template<typename T>
void ScGuardedObject<T>::nullify(void)
{
	if (this->data)
		this->data->pointer = NULL;
};

template<typename T>
ScGuardedObject<T>::~ScGuardedObject()
{
	nullify();
	// deref() is called by ScGuardedPtr<T> dtor
};

#endif
