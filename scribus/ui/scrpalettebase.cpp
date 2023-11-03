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

#include <QApplication>
#include <QCloseEvent>
#include <QDesktopWidget>
#include <QHideEvent>
#include <QPoint>

#include "iconmanager.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "scrpalettebase.h"
#include "util.h"

ScrPaletteBase::ScrPaletteBase(  QWidget * parent, const QString& prefsContext, bool modal, Qt::WindowFlags f)
	: QDialog ( parent, f | Qt::Tool | Qt::CustomizeWindowHint
			| Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint
			| Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint )
{
	if (PrefsManager::instance().appPrefs.uiPrefs.useSmallWidgets)
	{
		setStyleSheet("	QToolButton { margin: 1px; padding: 0px; font-size: 10px; } \
						QToolButton:pressed { padding-top: 2px; padding-left: 2px } \
						QPushButton { margin: 1px; padding: 0px; font-size: 10px; } \
						QPushButton:pressed { padding-top: 2px; padding-left: 2px } \
						QRadioButton, QComboBox, QLineEdit \
							QListView, QLabel { margin:1px; padding: 0px; font-size: 10px; } \
						QCheckBox, QSpinBox, QDoubleSpinBox \
							{ margin:1px; padding: 0px; font-size: 10px; } \
						QTabWidget, QTabBar, QTableView, QGroupBox, QTreeView \
							{ font-size: 10px ; } \
						QToolBox::tab { font-size: 10px; padding: 0px; margin: 0px; } \
			  		");
	}
	m_originalParent = parent;
	m_tempParent = nullptr;
	setWindowIcon(IconManager::instance().loadIcon("AppIcon.png"));
	setPrefsContext(prefsContext);
	setModal(modal);
	connect(&PrefsManager::instance(), SIGNAL(prefsChanged()), this, SLOT(setFontSize()));
}

void ScrPaletteBase::setPrefsContext(const QString& context)
{
	if (m_prefsContextName.isEmpty())
	{
		m_prefsContextName = context;
		if (!m_prefsContextName.isEmpty())
		{
			m_palettePrefs = PrefsManager::instance().prefsFile->getContext(m_prefsContextName);
			if (m_palettePrefs)
				m_visibleOnStartup = m_palettePrefs->getBool("visible");
		}
		else
			m_palettePrefs = nullptr;
	}
}

void ScrPaletteBase::startup()
{
	setFontSize();
	if (m_visibleOnStartup)
		show();
	else
		hide();
	emit paletteShown(m_visibleOnStartup);
}

void ScrPaletteBase::setPaletteShown(bool visible)
{
	storeVisibility(visible);
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
	newfont.setPointSize(PrefsManager::instance().appPrefs.uiPrefs.paletteFontSize);
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
	//	if (keyMod == 0 && keyEvent->key() == Key_Escape)
	//	emit paletteShown(false);

	QDialog::keyPressEvent(keyEvent);
}
	*/
void ScrPaletteBase::closeEvent(QCloseEvent *closeEvent)
{
	closeEvent->ignore();
	hide();
	emit paletteShown(false);
}

void ScrPaletteBase::hideEvent(QHideEvent*)
{
	storePosition();
	storeSize();
}

void ScrPaletteBase::showEvent(QShowEvent *showEvent)
{
	// According to Qt doc, non-spontaneous show events are sent to widgets
	// immediately before they are shown. We want to restore geometry for those
	// events as spontaneous events are delivered after dialog has been shown
	if (m_palettePrefs && !showEvent->spontaneous())
	{
		QDesktopWidget *d = QApplication::desktop();
		if (m_palettePrefs->contains("left"))
		{
			QRect scr = QApplication::desktop()->availableGeometry(this);
			// all palettes should have enough room for 3x3 min widgets
			int vwidth  = qMin(qMax(0, m_palettePrefs->getInt("width")),
			                   d->width());
			int vheight = qMin(qMax(0, m_palettePrefs->getInt("height")),
			                   d->height());
			// palettes should not use too much screen space
			if (vwidth > d->width()/3 && vheight > d->height()/3)
				vwidth = d->width()/3;
			// and should be partly visible
			int vleft   = qMin(qMax(scr.left() - vwidth, m_palettePrefs->getInt("left")),
							   scr.right());
			int vtop = qMin(m_palettePrefs->getInt("top"), d->height());
#if defined(Q_OS_MACOS) || defined(_WIN32)
			// on Mac and Windows you're dead if the titlebar is not on screen
			vtop    = qMax(64, vtop);
#else
			vtop    = qMax(-vheight, vtop);
#endif
			// Check values against current screen size
			if ( vleft <= scr.left() )
				vleft = scr.left();
			if ( vleft >= scr.right() )
				vleft = scr.left();
			if ( vtop >= scr.bottom() )
				vtop = 64;
			if ( vtop <= scr.top() )
				vtop = scr.top();
			if ( vwidth >= scr.width() )
				vwidth = qMax( 0, scr.width() - vleft );
			if ( vheight >= scr.height() )
				vheight = qMax( 0, scr.height() - vtop );
//			qDebug() << QString("root %1x%2 %7 palette %3x%4 @ (%5,%6)").arg(d->width()).arg(d->height())
//				.arg(vwidth).arg(vheight).arg(vleft).arg(vtop).arg(name());
//			setGeometry(vleft, vtop, vwidth, vheight);
			resize(vwidth, vheight);
			move(vleft, vtop);
		}
		storeVisibility(true);
	}
	QDialog::showEvent(showEvent);
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
	QDialog::reject();
	emit paletteShown(false);
}

void ScrPaletteBase::storePosition()
{
	if (m_palettePrefs)
	{
		QPoint geo = pos();
		m_palettePrefs->set("left", geo.x());
		m_palettePrefs->set("top", geo.y());
	}
}

void ScrPaletteBase::storePosition(int newX, int newY)
{
	if (m_palettePrefs)
	{
		m_palettePrefs->set("left", newX);
		m_palettePrefs->set("top", newY);
	}
}

void ScrPaletteBase::storeSize()
{
	if (m_palettePrefs)
	{
		m_palettePrefs->set("width", width());
		m_palettePrefs->set("height", height());
	}
}

void ScrPaletteBase::storeVisibility(bool vis)
{
	if (m_palettePrefs)
		m_palettePrefs->set("visible", vis);
}

int ScrPaletteBase::exec(QWidget* newParent)
{
	Q_ASSERT(m_tempParent == nullptr && newParent != nullptr);
	m_tempParent = newParent;
	Qt::WindowFlags wflags = windowFlags();
	setParent(newParent, wflags);
	int i = QDialog::exec();
	setParent(m_originalParent, wflags);
	m_tempParent = nullptr;
	return i;
}
