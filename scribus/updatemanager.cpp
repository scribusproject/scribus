

#include "updatemanager.h"

UpdateMemento::~UpdateMemento() 
{}


//	int m_updatesDisabled;
//	QSet<QPair<UpdateManaged*, UpdateMemento*> > m_pending;
	
typedef QPair<UpdateManaged*, UpdateMemento*> PendingUpdate;


UpdateManager::~UpdateManager()
{
	foreach(PendingUpdate pair, m_pending) {
		delete pair.second;
	}
}
	
void UpdateManager::setUpdatesEnabled(bool val)
{
	if (val)
	{
		if (m_updatesDisabled > 0)
		{
			if (--m_updatesDisabled == 0)
			{
				foreach(PendingUpdate pair, m_pending) {
					pair.first->updateNow(pair.second);
				}
				m_pending.clear();
			}
		}
	}
	else
	{
		++m_updatesDisabled;
	}
}


bool UpdateManager::requestUpdate(UpdateManaged* observable, UpdateMemento* what)
{
	if (m_updatesDisabled == 0)
	{
		return true;	
	}
	else
	{
		m_pending.insert(PendingUpdate(observable, what));
		return false;
	}
}

