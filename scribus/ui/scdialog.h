/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
 *   Copyright (C) 2005 by Craig Bradney                                   *
 *   cbradney@scribus.info                                                 *
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
#ifndef SCDIALOG_H
#define SCDIALOG_H

#include <QCloseEvent>
#include <QDialog>
#include <QHideEvent>
#include <QKeySequence>
#include <QShowEvent>

#include "scribusapi.h"
class PrefsContext;

class SCRIBUS_API ScDialog : public QDialog
{
	Q_OBJECT

public:
	ScDialog(QWidget * parent = nullptr, const QString& prefsContext = QString());

protected:
	/** @brief Set the Preferences context to be used for storage of startup visibility and position and size */
	virtual void setPrefsContext(const QString& context);
	void storeSize();

	/** @brief Restore the geometry of the window when showing it. */
	void showEvent(QShowEvent *showEvent) override;
	/** @brief Captures the close event and changes it to hide */
	void closeEvent(QCloseEvent *closeEvent) override;
	/** @brief Stores the geometry of the window when hiding. */
	void hideEvent(QHideEvent* hideEvent) override;

	PrefsContext* m_dialogPrefs {nullptr};
	QString m_prefsContextName;
};

#endif
