/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
*   Copyright (C) 2006 by Franz Schmid                                   *
*   franz.schmid@altmuehlnet.de                                                   *
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

#include "patterndialog.h"
#include "patterndialog.moc"
#include "customfdialog.h"
#include "prefsmanager.h"
#include "prefsfile.h"
#include "util.h"
#include <qpixmap.h>
#include <qimage.h>

PatternDialog::PatternDialog(QWidget* parent, QMap<QString, ScPattern> *docPatterns, ScribusDoc *doc) : patternDialogBase(parent)
{
	m_doc = doc;
	patternView->clear();
	for (QMap<QString, ScPattern>::Iterator it = docPatterns->begin(); it != docPatterns->end(); ++it)
	{
		dialogPatterns.insert(it.key(), it.data());
	}
	updatePatternList();
	buttonRemove->setEnabled(false);
	connect(buttonOK, SIGNAL(clicked()), this, SLOT(accept()));
	connect(buttonCancel, SIGNAL(clicked()), this, SLOT(reject()));
	connect(buttonLoad, SIGNAL(clicked()), this, SLOT(loadPattern()));
	connect(buttonRemove, SIGNAL(clicked()), this, SLOT(removePattern()));
	connect(patternView, SIGNAL(clicked(QIconViewItem*)), this, SLOT(patternSelected(QIconViewItem*)));
}

void PatternDialog::updatePatternList()
{
	patternView->clear();
	for (QMap<QString, ScPattern>::Iterator it = dialogPatterns.begin(); it != dialogPatterns.end(); ++it)
	{
		QPixmap pm;
		if (it.data().getPattern()->width() > it.data().getPattern()->height())
			pm.convertFromImage(it.data().getPattern()->scaleWidth(48));
		else
			pm.convertFromImage(it.data().getPattern()->scaleHeight(48));
		(void) new QIconViewItem(patternView, it.key(), pm);
	}
}

void PatternDialog::loadPattern()
{
	QString fileName;
	QString formatD(setupImageFormats());
	PrefsContext* dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
	QString wdir = dirs->get("patterns", ".");
	CustomFDialog dia(this, wdir, tr("Open"), formatD, fdShowPreview | fdExistingFiles);
	if (dia.exec() == QDialog::Accepted)
		fileName = dia.selectedFile();
	else
		return;
	if (!fileName.isEmpty())
	{
		PrefsManager::instance()->prefsFile->getContext("dirs")->set("patterns", fileName.left(fileName.findRev("/")));
		QFileInfo fi(fileName);
		QString patNam = fi.baseName().stripWhiteSpace().simplifyWhiteSpace().replace(" ", "_");
		ScPattern pat = ScPattern();
		pat.setDoc(m_doc);
		pat.setPattern(fileName);
		if (!dialogPatterns.contains(patNam))
		{
			dialogPatterns.insert(patNam, pat);
			updatePatternList();
		}
	}
}

void PatternDialog::patternSelected(QIconViewItem* it)
{
	if (it)
		buttonRemove->setEnabled(true);
	else
	{
		buttonRemove->setEnabled(false);
		patternView->clearSelection();
	}
}

void PatternDialog::removePattern()
{
	QIconViewItem *it = patternView->currentItem();
	if (it)
	{
		dialogPatterns.remove(it->text());
		updatePatternList();
	}
}
