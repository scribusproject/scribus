
/*
 For general Scribus (>=1.3.2) copyright and licensing information please refer
 to the COPYING file provided with the program. Following this notice may exist
 a copyright and/or license notice that predates the release of Scribus 1.3.2
 for which a new license (GPL+exception) is in place.
 */
/***************************************************************************
*   Copyright (C) 2008 by Andreas Vox                                     *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.             *
***************************************************************************/

#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "scribusapi.h"


/**
  Interface class for objects representing a going transaction.
  Will be specialized by classes like Undomanager for use as
  a result type in their beginTransaction() method.
*/
class SCRIBUS_API Transaction {

public:
	enum Status {
		STATE_OPEN,
		STATE_WILLFAIL,
		STATE_FAILED,
		STATE_COMMITTED
	};
	
	struct TransactionStateBase {
		Status m_status;
	};
	
	Transaction(TransactionStateBase* data) : m_data(data)
	{
		if (data != 0)
			m_data->m_status = STATE_OPEN;
	}
	
	Transaction(const Transaction& other) : m_data(other.m_data)
	{
		const_cast<Transaction&>(other).m_data = 0;  // mean, but necessary so that not both destructors commit/close
	}
	
	
	/**
		Automatically commits if forgotten.
		Override as appropiate. Since the superclass destructor is called last, you
	    can cancel in the subclass destructor; the commit here then will do nothing.
	    Don't forget to set m_data to NULL if you free the m_data pointer!
	 */
	virtual ~Transaction();
	
	/**
	  Commits this transaction if in STATE_OPEN.
	  If in STATE_WILLFAIL, cancel the transaction instead; otherwise do nothing.
	  Returns true iff the transaction was committed or canceled.
	 */
	virtual bool commit() = 0;
	
	/**
		Cancels this transaction if in STATE_OPEN or STATE_WILLFAIL; otherwise do nothing
	    Returns true iff the transaction was canceled.
	 */
	virtual bool cancel() = 0;
	
	/**
		Marks this transaction as failed.
	 */
	virtual void markFailed();
	
	int getState() const;
	
protected:
	// if you subclass, do *not* add any data members but use this pointer instead,
	// otherwise the copy initializer will strip your objects.
	TransactionStateBase*    m_data;
	
private:
	// blocked
	Transaction& operator= (const Transaction&) { return *this; }
};

#endif
