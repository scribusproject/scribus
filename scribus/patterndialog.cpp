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
#include "stencilreader.h"
#include "scconfig.h"
#include <qpixmap.h>
#include <qimage.h>

PatternDialog::PatternDialog(QWidget* parent, QMap<QString, ScPattern> *docPatterns, ScribusDoc *doc, ScribusMainWindow *scMW) : patternDialogBase(parent)
{
	m_doc = doc;
	mainWin = scMW;
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
	QString formats = "Scribus Objects (*.sce *.SCE);;Dia Shapes (*.shape *.SHAPE);;Kivio Stencils (*.sml *.SML);;EPS (*.eps *.EPS);;PDF (*.pdf *.PDF);;";
	QString form1 = "";
	QString form2 = "";
	for ( uint i = 0; i < QImageIO::inputFormats().count(); ++i )
	{
		form1 = QString(QImageIO::inputFormats().at(i)).lower();
		form2 = QString(QImageIO::inputFormats().at(i)).upper();
		if (form1 == "jpeg")
		{
			form1 = "jpg";
			form2 = "JPG";
		}
		if ((form1 == "png") || (form1 == "xpm") || (form1 == "gif"))
		{
			formats += form2 + " (*."+form1+" *."+form2+");;";
		}
		else if (form1 == "jpg")
		{
			// JPEG is a special case because both .jpg and .jpeg
			// are acceptable extensions.
			formats += "JPEG (*.jpg *.jpeg *.JPG *.JPEG);;";
		}
	}
#ifdef HAVE_TIFF
	formats += "TIFF (*.tif *.tiff *.TIF *.TIFF);;";
#endif
	formats += "PSD (*.psd *.PSD);;";
	formats += tr("All Files (*)");
	PrefsContext* dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
	QString wdir = dirs->get("patterns", ".");
	CustomFDialog dia(this, wdir, tr("Open"), formats, fdExistingFiles);
	if (dia.exec() == QDialog::Accepted)
		fileName = dia.selectedFile();
	else
		return;
	if (!fileName.isEmpty())
	{
		PrefsManager::instance()->prefsFile->getContext("dirs")->set("patterns", fileName.left(fileName.findRev("/")));
		QFileInfo fi(fileName);
		if ((fi.extension(true).lower() == "sml") || (fi.extension(true).lower() == "shape") || (fi.extension(true).lower() == "sce"))
			loadVectors(fileName);
		else
		{
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
}

void PatternDialog::loadVectors(QString data)
{
	QFileInfo fi(data);
	QString patNam = fi.baseName().stripWhiteSpace().simplifyWhiteSpace().replace(" ", "_");
	if (fi.extension(true).lower() == "sml")
	{
		QString f = "";
		loadText(data, &f);
		StencilReader *pre = new StencilReader();
		data = pre->createObjects(f);
		delete pre;
	}
	else if (fi.extension(true).lower() == "shape")
	{
		QString f = "";
		loadText(data, &f);
		StencilReader *pre = new StencilReader();
		data = pre->createShape(f);
		delete pre;
	}
	else if (fi.extension(true).lower() == "sce")
	{
		QString f = "";
		loadText(data, &f);
		data = f;
	}
	uint ac = m_doc->Items->count();
	bool savedAlignGrid = m_doc->useRaster;
	bool savedAlignGuides = m_doc->SnapGuides;
	m_doc->useRaster = false;
	m_doc->SnapGuides = false;
	mainWin->slotElemRead(data, m_doc->currentPage()->xOffset(), m_doc->currentPage()->yOffset(), false, true, m_doc, m_doc->view());
	m_doc->useRaster = savedAlignGrid;
	m_doc->SnapGuides = savedAlignGuides;
	uint ae = m_doc->Items->count();
	ScPattern pat = ScPattern();
	pat.setDoc(m_doc);
	PageItem* currItem = m_doc->Items->at(ac);
	pat.pattern = currItem->DrawObj_toImage();
	for (uint as = ac; as < ae; ++as)
	{
		pat.items.append(m_doc->Items->take(ac));
	}
	if (!dialogPatterns.contains(patNam))
	{
		dialogPatterns.insert(patNam, pat);
		updatePatternList();
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
