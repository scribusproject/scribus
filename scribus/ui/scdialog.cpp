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
#include <QHideEvent>
#include <QMainWindow>
#include <QPoint>
#include <QScreen>

#include "iconmanager.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "scdialog.h"
#include "util.h"

ScDialog::ScDialog(QWidget * parent, const QString& prefsContext)
        : QDialog(parent)
{
	setWindowIcon(IconManager::instance().loadPixmap("AppIcon.png"));
	setPrefsContext(prefsContext);
	setObjectName(prefsContext);
}

void ScDialog::setPrefsContext(const QString& context)
{
	if (m_prefsContextName.isEmpty())
	{
		m_prefsContextName = context;
		if (!m_prefsContextName.isEmpty())
			m_dialogPrefs = PrefsManager::instance().prefsFile->getContext(m_prefsContextName);
		else
			m_dialogPrefs = nullptr;
	}
}

void ScDialog::closeEvent(QCloseEvent *closeEvent)
{
	storeSize();
	QDialog::closeEvent(closeEvent);
}

void ScDialog::hideEvent(QHideEvent* hideEvent)
{
	storeSize();
	QDialog::hideEvent(hideEvent);
}

void ScDialog::showEvent(QShowEvent *showEvent)
{
	// According to Qt doc, non-spontaneous show events are sent to widgets
	// immediately before they are shown. We want to restore geometry for those
	// events as spontaneous events are delivered after dialog has been shown
	if (m_dialogPrefs && !showEvent->spontaneous())
	{
		const QScreen *diaScreen = this->screen();
		if (m_dialogPrefs->contains("width"))
		{
			QRect scr = diaScreen->availableGeometry();
			// All dialogs should have enough room for 3x3 min widgets
			int diaWidth  = qMax(0, qMin(m_dialogPrefs->getInt("width"), scr.width()));
			int diaHeight = qMax(0, qMin(m_dialogPrefs->getInt("height"), scr.height()));
			if (diaWidth > 0 && diaHeight > 0)
			{
				int newWidth = qMax(0, qMin(diaWidth, scr.width()));
				int newHeight = qMax(0, qMin(diaHeight, scr.height()));
				if (newWidth > 0 && newHeight > 0)
					resize(newWidth, newHeight);
			}
		}
	}
	QDialog::showEvent(showEvent);
}

void ScDialog::storeSize()
{
	if (m_dialogPrefs)
	{
		m_dialogPrefs->set("width", width());
		m_dialogPrefs->set("height", height());
	}
}
