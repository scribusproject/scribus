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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef SCRPALETTE_BASE_H
#define SCRPALETTE_BASE_H

#include <QCloseEvent>
#include <QDialog>
#include <QHideEvent>
#include <QKeySequence>

#include "scribusapi.h"
class PrefsContext;

class SCRIBUS_API ScrPaletteBase : public QDialog
{
	Q_OBJECT

	public:
		ScrPaletteBase( QWidget * parent = NULL, const QString& prefsContext = QString::null, bool modal = false, Qt::WFlags f = 0 );
		~ScrPaletteBase() {};
		/** @brief Sample way to grab keystrokes, simply calls superclass at this point */
		//virtual void keyPressEvent(QKeyEvent *keyEvent);
		/** @brief Captures the close event and changes it to hide */
		virtual void closeEvent(QCloseEvent *closeEvent);
		/** @brief Stores the geometry of the window when hiding. */
		void hideEvent(QHideEvent*);
		virtual void show();
		virtual void hide();
		void startup();
		
	public slots:
		virtual void setPaletteShown(bool);
		virtual void setFontSize();
		int exec(QWidget* newParent);
		
	signals:
		/** @brief Let the action for this palette know when something changes and it hasnt caused it */
		void paletteShown(bool);
		
	protected:
		/** @brief Set the Preferences context to be used for storage of startup visibility and position and size */
		virtual void setPrefsContext(QString context);
		void storePosition();
		void storePosition(int newX, int newY);
		void storeSize();
		void storeVisibility(bool);
		
		PrefsContext* palettePrefs;
		QString prefsContextName;
		bool visibleOnStartup;
		QWidget* originalParent;
		QWidget* tempParent;
		
	protected slots:
		virtual void reject();
};
#endif
