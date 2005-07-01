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

#include <qapplication.h>
#include <qpoint.h>

#include "scrpalettebase.h"
#include "scrpalettebase.moc"
#include "prefsfile.h"
#include "prefscontext.h"


extern PrefsFile* prefsFile;

ScrPaletteBase::ScrPaletteBase(  QWidget * parent, const char * name, bool modal, WFlags f) : QDialog ( parent, name, modal, f) 
{
}

void ScrPaletteBase::setPrefsContext(QString context)
{
	if (prefsContextName=="")
	{
		prefsContextName=context;
		if (prefsContextName != "")
		{
			palettePrefs = prefsFile->getContext(prefsContextName);
			if (palettePrefs)
				visibleOnStartup = palettePrefs->getBool("visible");
		}
		else
			palettePrefs = NULL;
	}
}

void ScrPaletteBase::startup()
{
	setShown(visibleOnStartup);
	emit paletteShown(visibleOnStartup);
} 

void ScrPaletteBase::setPaletteShown(bool visible)
{
	storeVisibility(visible);
	setShown(visible);
}
/*
void ScrPaletteBase::keyPressEvent(QKeyEvent *keyEvent)
{
	// Sample code to use in overriding classes 
	int keyMod;
	switch (keyEvent->state())
	{
		case ShiftButton:
			keyMod = SHIFT;
			break;
		case AltButton:
			keyMod = ALT;
			break;
		case ControlButton:
			keyMod = CTRL;
			break;
		default:
			keyMod = 0;
			break;
	}
	// Tell our action to be off
	//	if (keyMod==0 && keyEvent->key()==Key_Escape)
	//	emit paletteShown(false);

	QDialog::keyPressEvent(keyEvent);
}
	*/
void ScrPaletteBase::closeEvent(QCloseEvent *closeEvent)
{
	emit paletteShown(false);
	closeEvent->ignore();
	hide();
}

void ScrPaletteBase::hideEvent(QHideEvent*)
{
	storePosition();
	storeSize();
}

void ScrPaletteBase::show()
{
	QDialog::show();
	if (palettePrefs)
	{
		if (palettePrefs->contains("left"))
		{
			int vleft   = QMAX(-80, palettePrefs->getInt("left"));
#ifndef QT_MAC
			int vtop    = QMAX(-80, palettePrefs->getInt("top"));
#else
			int vtop    = QMAX(64, palettePrefs->getInt("top"));
#endif
			int vwidth  = QMAX(100, palettePrefs->getInt("width"));
			int vheight = QMAX(100, palettePrefs->getInt("height"));
			setGeometry(vleft, vtop, vwidth, vheight);
		}
		storeVisibility(true);
	}
}

void ScrPaletteBase::hide()
{
	if (isShown())
	{
		storePosition();
		storeSize();
		QDialog::hide();
	}
}

void ScrPaletteBase::reject()
{
	emit paletteShown(false);
	QDialog::reject();
}

void ScrPaletteBase::storePosition()
{
	if (palettePrefs)
	{
		palettePrefs->set("left", x());
		palettePrefs->set("top", y());
	}
}

void ScrPaletteBase::storePosition(int newX, int newY)
{
	if (palettePrefs)
	{
		palettePrefs->set("left", newX);
		palettePrefs->set("top", newY);
	}
}

void ScrPaletteBase::storeSize()
{
	if (palettePrefs)
	{
		palettePrefs->set("width", width());
		palettePrefs->set("height", height());
	}
}

void ScrPaletteBase::storeVisibility(bool vis)
{
	if (palettePrefs)
		palettePrefs->set("visible", vis);
}