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

#include <QApplication>
#include <QCloseEvent>
#include <QDesktopWidget>
#include <QHideEvent>
#include <QPoint>

#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "scribus.h"
#include "scrpalettebase.h"
#include "util.h"
#include "util_icon.h"


ScrPaletteBase::ScrPaletteBase(  QWidget * parent, const QString& prefsContext, bool modal, Qt::WFlags f)
: QDialog ( parent, f | Qt::Tool | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint | Qt::WindowSystemMenuHint ),
palettePrefs(0),
prefsContextName(QString::null),
visibleOnStartup(false)
{
	if (PrefsManager::instance()->appPrefs.useSmallWidgets)
	{
		setStyleSheet("	QToolButton { margin: 0px; padding: 0px; font-size: 10px; } \
						QToolButton:pressed { padding-top: 2px; padding-left: 2px } \
						QPushButton { margin: 0px; padding: 0px; font-size: 10px; } \
						QPushButton:pressed { padding-top: 2px; padding-left: 2px } \
						QRadioButton, QComboBox, QLineEdit, QListView, QLabel { margin:0px; padding: 0px; font-size: 10px; } \
						QCheckBox, QSpinBox, QDoubleSpinBox { margin:0px; padding: 0px; font-size: 10px; padding-right: 13px;} \
						QTabWidget, QTabBar, QTableView, QGroupBox { font-size: 10px ; } \
			  		");
	}
	originalParent=parent;
	tempParent=0;
	setWindowIcon(loadIcon("AppIcon.png"));
	setPrefsContext(prefsContext);
	setModal(modal);
	connect(PrefsManager::instance(), SIGNAL(prefsChanged()), this, SLOT(setFontSize()));
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
	setFontSize();
//	setShown(visibleOnStartup);
	if (visibleOnStartup)
		show();
	else
		hide();
	emit paletteShown(visibleOnStartup);
}

void ScrPaletteBase::setPaletteShown(bool visible)
{
	storeVisibility(visible);
//	setShown(visible);
	if (!visible)
		hide();
	else if (!isVisible())
	{
		show();
		activateWindow();
	}
}

void ScrPaletteBase::setFontSize()
{
	QFont newfont(font());
	newfont.setPointSize(PrefsManager::instance()->appPrefs.PaletteFontSize);
	setFont(newfont);
}

/*
void ScrPaletteBase::keyPressEvent(QKeyEvent *keyEvent)
{
	// Sample code to use in overriding classes 
	int keyMod;
	switch (keyEvent->modifiers())
	{
		case Qt::ShiftModifier:
			keyMod = Qt::SHIFT;
			break;
		case Qt::AltModifier:
			keyMod = Qt::ALT;
			break;
		case Qt::ControlModifier:
			keyMod = Qt::CTRL;
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
	if (palettePrefs)
	{
		QDesktopWidget *d = QApplication::desktop();
		QSize gStrut = QApplication::globalStrut();
		if (palettePrefs->contains("left"))
		{
			// all palettes should have enough room for 3x3 min widgets
			int vwidth  = qMin(qMax(3*gStrut.width(), palettePrefs->getInt("width")),
			                   d->width());
			int vheight = qMin(qMax(3*gStrut.height(), palettePrefs->getInt("height")),
			                   d->height());
			// palettes should not use too much screen space
			if (vwidth > d->width()/3 && vheight > d->height()/3)
				vwidth = d->width()/3;
			// and should be partly visible
			int vleft   = qMin(qMax(-vwidth + gStrut.width(), palettePrefs->getInt("left")),
			                   d->width() - gStrut.width());
			int vtop = qMin(palettePrefs->getInt("top"), d->height() - gStrut.height());
#if defined(Q_OS_MAC) || defined(_WIN32)
			// on Mac and Windows you're dead if the titlebar is not on screen
			vtop    = qMax(64, vtop);
#else
			vtop    = qMax(-vheight + gStrut.height(), vtop);
#endif
			// Check values against current screen size
			QRect scr = QApplication::desktop()->screen()->geometry();
			if ( vleft >= scr.width() )
				vleft = 0;
			if ( vtop >= scr.height() )
				vtop = 64;
			if ( vwidth >= scr.width() )
				vwidth = qMax( gStrut.width(), scr.width() - vleft );
			if ( vheight >= scr.height() )
				vheight = qMax( gStrut.height(), scr.height() - vtop );
//			qDebug(QString("root %1x%2 %7 palette %3x%4 @ (%5,%6)").arg(d->width()).arg(d->height())
//				.arg(vwidth).arg(vheight).arg(vleft).arg(vtop).arg(name()));
//			setGeometry(vleft, vtop, vwidth, vheight);
			resize(vwidth, vheight);
			move(vleft, vtop);
		}
		storeVisibility(true);
	}
	QDialog::show();
}

void ScrPaletteBase::hide()
{
	if (isVisible())
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
		QPoint geo = pos();
		palettePrefs->set("left", geo.x());
		palettePrefs->set("top", geo.y());
/*
#if QT_VERSION  >= 0x040300 && !defined(_WIN32)
		QRect geo = geometry();
#else
		QRect geo = frameGeometry();
#endif
		palettePrefs->set("left", geo.left());
		palettePrefs->set("top", geo.top());
*/
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

int ScrPaletteBase::exec(QWidget* newParent)
{
	Q_ASSERT(tempParent==0 && newParent!=0);
	tempParent=newParent;
	Qt::WindowFlags wflags = windowFlags();
	setParent(newParent, wflags);
	int i=QDialog::exec();
	setParent(originalParent, wflags);
	tempParent=0;
	return i;
}
