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
#include <qdir.h>
#include <qfiledialog.h>

PatternDialog::PatternDialog(QWidget* parent, QMap<QString, ScPattern> *docPatterns, ScribusDoc *doc, ScribusMainWindow *scMW) : patternDialogBase(parent)
{
	m_doc = doc;
	mainWin = scMW;
	patternView->clear();
	for (QMap<QString, ScPattern>::Iterator it = docPatterns->begin(); it != docPatterns->end(); ++it)
	{
		dialogPatterns.insert(it.key(), it.data());
	}
	origPatterns = docPatterns->keys();
	updatePatternList();
	buttonRemove->setEnabled(false);
	connect(buttonOK, SIGNAL(clicked()), this, SLOT(accept()));
	connect(buttonCancel, SIGNAL(clicked()), this, SLOT(reject()));
	connect(buttonLoad, SIGNAL(clicked()), this, SLOT(loadPattern()));
	connect(buttonLoadDir, SIGNAL(clicked()), this, SLOT(loadPatternDir()));
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

void PatternDialog::loadPatternDir()
{
	PrefsContext* dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
	QString wdir = dirs->get("patterns", ".");
	QString fileName = QFileDialog::getExistingDirectory(wdir, this, "d", tr("Choose a Directory"), true);
	if (!fileName.isEmpty())
	{
		QStringList formats;
		formats += "eps";
		formats += "pdf";
		QString form1 = "";
		for ( uint i = 0; i < QImageIO::inputFormats().count(); ++i )
		{
			form1 = QString(QImageIO::inputFormats().at(i)).lower();
			if (form1 == "jpeg")
				form1 = "jpg";
			if ((form1 == "png") || (form1 == "xpm") || (form1 == "gif"))
			formats += form1;
			else if (form1 == "jpg")
			{
				formats += "jpg";
				formats += "jpeg";
			}
		}
#ifdef HAVE_TIFF
		formats += "tif";
	 	formats += "tiff";
#endif
		formats += "psd";
		formats += "pat";
		QDir d(fileName, "*", QDir::Name, QDir::Files | QDir::Readable | QDir::NoSymLinks);
		if ((d.exists()) && (d.count() != 0))
		{
			for (uint dc = 0; dc < d.count(); ++dc)
			{
				QFileInfo fi(QDir::cleanDirPath(QDir::convertSeparators(fileName + "/" + d[dc])));
				QString ext = fi.extension(true).lower();
				if ((ext == "sml") || (ext == "shape") || (ext == "sce"))
					loadVectors(QDir::cleanDirPath(QDir::convertSeparators(fileName + "/" + d[dc])));
				else if (formats.contains(ext))
				{
					QString patNam = fi.baseName().stripWhiteSpace().simplifyWhiteSpace().replace(" ", "_");
					ScPattern pat = ScPattern();
					pat.setDoc(m_doc);
					pat.setPattern(QDir::cleanDirPath(QDir::convertSeparators(fileName + "/" + d[dc])));
					if (!dialogPatterns.contains(patNam))
						dialogPatterns.insert(patNam, pat);
				}
				else
					continue;
			}
		}
		d.cdUp();
		dirs->set("patterns", d.absPath());
		updatePatternList();
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
	formats += "Gimp Patterns (*.pat *.PAT);;";
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
		{
			loadVectors(fileName);
			updatePatternList();
		}
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
	pat.width = currItem->gWidth;
	pat.height = currItem->gHeight;
	for (uint as = ac; as < ae; ++as)
	{
		pat.items.append(m_doc->Items->take(ac));
	}
	if (!dialogPatterns.contains(patNam))
	{
		dialogPatterns.insert(patNam, pat);
	}
	for (QMap<QString, ScPattern>::Iterator it = m_doc->docPatterns.begin(); it != m_doc->docPatterns.end(); ++it)
	{
		if (!origPatterns.contains(it.key()))
			dialogPatterns.insert(it.key(), it.data());
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
		QStringList patterns2Del;
		QStringList mainPatterns = dialogPatterns.keys();
		for (uint a = 0; a < mainPatterns.count(); a++)
		{
			if (mainPatterns[a] != it->text())
			{
				QStringList subPatterns;
				subPatterns = getUsedPatternsHelper(mainPatterns[a], subPatterns);
				if (subPatterns.contains(it->text()))
					patterns2Del.append(mainPatterns[a]);
			}
		}
		QStringList subPatterns;
		subPatterns = getUsedPatternsHelper(it->text(), subPatterns);
		if (!subPatterns.isEmpty())
			patterns2Del += subPatterns;
		patterns2Del.append(it->text());
		for (uint a = 0; a < patterns2Del.count(); a++)
		{
			dialogPatterns.remove(patterns2Del[a]);
		}
		updatePatternList();
	}
}

QStringList PatternDialog::getUsedPatternsHelper(QString pattern, QStringList &results)
{
	ScPattern *pat = &dialogPatterns[pattern];
	QStringList pats;
	pats.clear();
	for (uint c = 0; c < pat->items.count(); ++c)
	{
		if ((!results.contains(pat->items.at(c)->pattern())) && (pat->items.at(c)->GrType == 8))
			pats.append(pat->items.at(c)->pattern());
	}
	if (!pats.isEmpty())
	{
		for (uint c = 0; c < pats.count(); ++c)
		{
			getUsedPatternsHelper(pats[c], results);
		}
	}
	return pats;
}
