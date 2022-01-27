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
#ifndef SCDOCKPALETTE_H
#define SCDOCKPALETTE_H

#include <QCloseEvent>
#include <QDockWidget>
#include <QHideEvent>
#include <QKeySequence>
#include <QShowEvent>

#include "scribusapi.h"
class PrefsContext;

class SCRIBUS_API ScDockPalette : public QDockWidget
{
Q_OBJECT

public:
	ScDockPalette(QWidget * parent = nullptr, const QString& prefsContext = QString(), Qt::WindowFlags f = Qt::WindowFlags());
	~ScDockPalette() {}

	/** @brief Sample way to grab keystrokes, simply calls superclass at this point */
	//virtual void keyPressEvent(QKeyEvent *keyEvent);
	virtual void hide();
	void startup();

public slots:
	virtual void setPaletteShown(bool);
	virtual void setFontSize();

signals:
	/** @brief Let the action for this palette know when something changes and it hasn't caused it */
	void paletteShown(bool);

protected:
	/** @brief Set the Preferences context to be used for storage of startup visibility and position and size */
	virtual void setPrefsContext(const QString& context);
	void storePosition();
	void storePosition(int newX, int newY);
	void storeSize();
	void storeVisibility(bool);
	void storeDockState();

	/** @brief Restore the geometry of the window when showing it. */
	void showEvent(QShowEvent *showEvent) override;
	/** @brief Captures the close event and changes it to hide */
	void closeEvent(QCloseEvent *closeEvent) override;
	/** @brief Stores the geometry of the window when hiding. */
	void hideEvent(QHideEvent* hideEvent) override;

	PrefsContext* m_palettePrefs {nullptr};
	QString m_prefsContextName;
	bool m_visibleOnStartup {false};
	QWidget* m_originalParent {nullptr};
	QWidget* m_tempParent {nullptr};
};
#endif
