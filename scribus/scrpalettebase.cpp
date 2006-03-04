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

#include <qapplication.h>
#include <qpoint.h>

#include "scrpalettebase.h"
#include "scrpalettebase.moc"
#include "scribus.h"
#include "prefsmanager.h"
#include "prefsfile.h"
#include "prefscontext.h"


ScrPaletteBase::ScrPaletteBase(  QWidget * parent, const char * name, bool modal, WFlags f) : QDialog ( parent, name, modal, f | Qt::WStyle_Customize | Qt::WStyle_Tool | Qt::WStyle_Title | Qt::WStyle_MinMax | Qt::WStyle_SysMenu | Qt::WStyle_NormalBorder),
palettePrefs(0),
prefsContextName(QString::null),
visibleOnStartup(false)
{
	setPrefsContext(name);
	ScribusMainWindow *scapp = dynamic_cast<ScribusMainWindow*>(parent);
	if (scapp)
		connect(scapp, SIGNAL(prefsChanged()), this, SLOT(setFontSize()));
}

void ScrPaletteBase::setPrefsContext(QString context)
{
	if (prefsContextName.isEmpty())
	{
		prefsContextName=context;
		if (!prefsContextName.isEmpty())
		{
			palettePrefs = PrefsManager::instance()->prefsFile->getContext(prefsContextName);
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

void ScrPaletteBase::setFontSize() {
	QFont *newfont = new QFont(font());
	newfont->setPointSize(PrefsManager::instance()->appPrefs.PaletteFontSize);
	setFont(*newfont);
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
		QDesktopWidget *d = QApplication::desktop();
		QSize gStrut = QApplication::globalStrut();
		if (palettePrefs->contains("left"))
		{
			// all palettes should have enough room for 3x3 min widgets
			int vwidth  = QMIN(QMAX(3*gStrut.width(), palettePrefs->getInt("width")),
			                   d->width());
			int vheight = QMIN(QMAX(3*gStrut.height(), palettePrefs->getInt("height")),
			                   d->height());
			// palettes should not use too much screen space
			if (vwidth > d->width()/3 && vheight > d->height()/3)
				vwidth = d->width()/3;
			// and should be partly visible
			int vleft   = QMIN(QMAX(-vwidth + gStrut.width(), palettePrefs->getInt("left")),
			                   d->width() - gStrut.width());
			int vtop = QMIN(palettePrefs->getInt("top"), d->height() - gStrut.height());
#if defined(QT_MAC) || defined(_WIN32)
			// on Mac and Windows you're dead if the titlebar is not on screen
			vtop    = QMAX(64, vtop);
#else
			vtop    = QMAX(-vheight + gStrut.height(), vtop);
#endif
			// Check values against current screen size
			QRect scr = QApplication::desktop()->screenGeometry(this);
			if ( vleft >= scr.width() )
				vleft = 0;
			if ( vtop >= scr.height() )
				vtop = 64;
			if ( vwidth >= scr.width() )
				vwidth = QMAX( gStrut.width(), scr.width() - vleft );
			if ( vheight >= scr.height() )
				vheight = QMAX( gStrut.height(), scr.height() - vtop );
//			qDebug(QString("root %1x%2 %7 palette %3x%4 @ (%5,%6)").arg(d->width()).arg(d->height())
//				.arg(vwidth).arg(vheight).arg(vleft).arg(vtop).arg(name()));
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
		QRect geo = geometry();
		palettePrefs->set("left", geo.left());
		palettePrefs->set("top", geo.top());
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
