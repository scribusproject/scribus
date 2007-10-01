/*
 For general Scribus (>=1.3.2) copyright and licensing information please refer
 to the COPYING file provided with the program. Following this notice may exist
 a copyright and/or license notice that predates the release of Scribus 1.3.2
 for which a new license (GPL+exception) is in place.
 */
/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#ifndef OBSERVABLE_H
#define OBSERVABLE_H

#include "scribusapi.h"

#include <QObject>
#include <QSet>
#include <QVariant>

#include "updatemanager.h"


//#include "observable_private.h"
struct SCRIBUS_API Private_Signal : public QObject 
{
	Q_OBJECT;
	
public:
	void emitSignal(QObject* what)
{
		emit changedObject(what);
}

void emitSignal(QVariant what)
{
	emit changedData(what);
}

bool connectSignal(QObject*, QObject* o, const char* slot)
{
	return QObject::connect(this, SIGNAL(changedObject(QObject*)), o, slot);
}

bool disconnectSignal(QObject*, QObject* o, const char* slot)
{
	return QObject::disconnect(this, SIGNAL(changedObject(QObject*)), o, slot);
}

bool connectSignal(QVariant, QObject* o, const char* slot)
{
	return QObject::connect(this, SIGNAL(changedData(QVariant)), o, slot);
}

bool disconnectSignal(QVariant, QObject* o, const char* slot)
{
	return QObject::disconnect(this, SIGNAL(changedData(QVariant)), o, slot);
}

signals:
void changedObject(QObject* what);
void changedData(QVariant what);
};



template<class OBSERVED>
struct Private_Memento : public UpdateMemento
{
	Private_Memento(OBSERVED data) : m_data(data) {};
	
	OBSERVED m_data;
};



/**
  Implement this interface if you want to observe an observable but dont want to derive from QObject 
 */
template<class OBSERVED>
class SCRIBUS_API Observer {
public:
	virtual void changed(OBSERVED) = 0;
	virtual ~Observer() {}
};




/**
 An MassObservable is basically just the source of a changed() signal.
 Observers can register via the Qt signal/slot mechanism or via the above interface.
 
 The difference to Observable (below) is that a MassObservable doesnt report changes to
 itself but to a bunch of SingleObservables.
 When you call update() on the SingleObservable, it will tell the associated
 MassObservable to notify all observers with the "changed" signal,
 providing a pointer to the single Observable. 
 
 The class parameter OBSERVED is usually a pointer to a subclass of SingleObservable. 
 
 MassObservable does not inherit QObject since that makes it difficult to use it as a mixin class.
 */
template<class OBSERVED>
class MassObservable : public UpdateManaged 
{	
	friend class UpdateManager;
	
public:
	MassObservable(UpdateManager* um = NULL);
	virtual ~MassObservable();
	/**
		Used if the UpdateManager is not available when the constructor is called
	*/
	void setUpdateManager(UpdateManager* um);
	
	/**
		This method will be called by the SingleObservable.
	    If updates are enabled, it calls updateNow() directly, otherwise the undomanager
	    will take care of that.
	 */
	virtual void update(OBSERVED what);
	
	void connectObserver(Observer<OBSERVED>* o);
	void disconnectObserver(Observer<OBSERVED>* o);
	
	bool connectObserver(QObject* o, const char* slot);
	bool disconnectObserver(QObject* o, const char* slot = 0);
	
protected:
	/**
		This method will be called by the updatemanager or by update()
	 */
	virtual void updateNow(UpdateMemento* what);

	QSet<Observer<OBSERVED>*> m_observers;
	Private_Signal* changedSignal;
	UpdateManager* m_um;
};




/**
  This mixin class just provides the update() method.
 */
template<class OBSERVED>
class SCRIBUS_API SingleObservable
{
public:
	/**
	  Connects this SingleObservale to the MassObservable
	 */
	SingleObservable(MassObservable<OBSERVED*> * massObservable) : m_massObservable(massObservable) 
	{};
	
	virtual ~SingleObservable() 
	{};
	
	void setMassObservable(MassObservable<OBSERVED*> * massObservable) 
	{
		m_massObservable = massObservable;
	}
	
	virtual void update() 
	{ 
		m_massObservable->update(dynamic_cast<OBSERVED*>(this)); 
	}
	
private:
	MassObservable<OBSERVED*>* m_massObservable;
};





/**
  An Observable is basically just the source of a changed() signal.
  Observers can register via the Qt signal/slot mechanism or via the above interface.
 
  When you call update(), all observers will receive the "changed" signal with a pointer
  to the Observable (this).
 
  Observable is implemented as a MassObservable which.
 
  The class parameter OBSERVED should be the implementing class.
 */
template<class OBSERVED>
class SCRIBUS_API Observable : public MassObservable<OBSERVED*> 
{	
public:
	Observable(UpdateManager* um = NULL) : MassObservable<OBSERVED*>(um) 
	{};
	
	virtual void update() 
	{ 
		MassObservable<OBSERVED*>::update(dynamic_cast<OBSERVED*>(this)); 
	}
};



// IMPLEMENTATION



template<class OBSERVED>
inline MassObservable<OBSERVED>::MassObservable(UpdateManager* um) : m_observers(), changedSignal(new Private_Signal()), m_um(um)
{
}

template<class OBSERVED>
inline MassObservable<OBSERVED>::~MassObservable()
{
	m_observers.clear();
	delete changedSignal;
}


template<class OBSERVED>
inline void MassObservable<OBSERVED>::setUpdateManager(UpdateManager* um)
{
	m_um = um;
}


template<class OBSERVED>
inline void MassObservable<OBSERVED>::update(OBSERVED what)
{
	Private_Memento<OBSERVED>* memento = new Private_Memento<OBSERVED>(what);
	if (m_um == NULL || m_um->requestUpdate(this, memento))
	{
		updateNow(memento);
	}
}


template<class OBSERVED>
inline void MassObservable<OBSERVED>::updateNow(UpdateMemento* what)
{
	Private_Memento<OBSERVED>* memento = dynamic_cast<Private_Memento<OBSERVED>*>(what);
	foreach (Observer<OBSERVED>* obs, m_observers)
	{
		obs->changed(memento->m_data);
	}
	changedSignal->emitSignal(memento->m_data);
	delete memento;
}


template<class OBSERVED>
inline void MassObservable<OBSERVED>::connectObserver(Observer<OBSERVED>* o)
{
	m_observers.insert(o);
}

template<class OBSERVED>
inline void MassObservable<OBSERVED>::disconnectObserver(Observer<OBSERVED>* o)
{
	m_observers.remove(o);
}


template <typename T>
inline void Private_Init(T& dummy) {}

template <>
inline void Private_Init(QObject*& dummy) 
{ 
//	dummy->die_compiler_die(); 
	dummy = 0;
}


template<class OBSERVED>
inline bool MassObservable<OBSERVED>::connectObserver(QObject* o, const char* slot)
{
	OBSERVED dummy;
	Private_Init(dummy);
	return changedSignal->connectSignal(dummy, o, slot);
}

template<class OBSERVED>
inline bool MassObservable<OBSERVED>::disconnectObserver(QObject* o, const char* slot)
{
	OBSERVED dummy;
	Private_Init(dummy);
	return changedSignal->disconnectSignal(dummy, o, slot);
}


#endif
