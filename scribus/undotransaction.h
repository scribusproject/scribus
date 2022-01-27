/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
 *   Copyright (C) 2005 by Riku Leino                                      *
 *   riku@scribus.info                                                     *
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

#ifndef UNDOTRANSACTION_H
#define UNDOTRANSACTION_H

#include <QString>

#include "scribusapi.h"
#include "transaction.h"

class QPixmap;

class TransactionData;
class TransactionState;
class UndoManager;
class UndoObject;

/**
    Class which handles Undo transactions. No data, just methods.
 */
class SCRIBUS_API UndoTransaction : public Transaction
{
public:
	UndoTransaction();
	UndoTransaction(TransactionData* data);
	virtual ~UndoTransaction();
	virtual bool commit();
	virtual bool cancel();
	bool commit(const QString &targetName,
				QPixmap *targetPixmap,
				const QString &name = QString(),
				const QString &description = QString(),
				QPixmap *actionPixmap = nullptr);
};

/**
  This struct is use instead of std::pair<TransactionObject*, TransactionState*> now.
  Just data, no methods.
*/
class TransactionData : public Transaction::TransactionStateBase
{
public:
	TransactionData()
	{
	}

	~TransactionData() {
		/*if (transactionObject)
			delete transactionObject;
		if (transactionState)
			delete transactionState;*/
	}

	int stackLevel {0};
	UndoManager* UM {nullptr};
	UndoObject* transactionObject {nullptr};  // will be DummyUndoObject
	TransactionState* transactionState {nullptr};
};

#endif
