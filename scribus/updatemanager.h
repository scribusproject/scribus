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

#ifndef UPDATEMANAGER_H
#define UPDATEMANAGER_H

#include "scribusapi.h"

#include <QSet>



/**
 Clients of UpdateManager use this class to encapsulate their argument
 to updateNow(). 
 */
struct SCRIBUS_API UpdateMemento
{
	virtual ~UpdateMemento();
};



/**
  Implement this interface in Observable.
*/


class SCRIBUS_API UpdateManaged
{
public:
	virtual void updateNow(UpdateMemento* what) = 0;
	virtual ~UpdateManaged() {}
};


/**
  An UpdateManager controls if and when updates will be propagated to the Observers.
  Usually each document has one UpdateManager.
  They are used this way:
 
  When many changes are about to happen, the application calls "setUpdatesDisabled()"
  before it begins the changes and calls "setUpdatesEnabled()" when it's done.
 
  An Observable calls "requestUpdate(this)". If the result is positive, the Observable
  calls "updateNow()" directly. Otherwise the UpdateManager stores a reference to the
  Observable and will call its "updateNow()" method once updates are enabled again.

  If "requestUpdate()" is called multiple times before updates are enabled again, each
  observable will only get one notification with "updateNow()" 
  when the updates are enabled again.
 */

class SCRIBUS_API UpdateManager
{
	int m_updatesDisabled;
	QSet<QPair<UpdateManaged*, UpdateMemento*> > m_pending;
	
public:
	UpdateManager() : m_updatesDisabled(0), m_pending() {}
	virtual ~UpdateManager();
	
	void setUpdatesEnabled(bool val=true);
	void setUpdatesDisabled() { setUpdatesEnabled(false); };
	bool updatesEnabled() { return m_updatesDisabled == 0; };
	/**
		Returns true if updates are enabled, otherwise stores 'observable' for notification when updates get enabled again.
	 */
	bool requestUpdate(UpdateManaged* observable, UpdateMemento* what);
};



#endif
