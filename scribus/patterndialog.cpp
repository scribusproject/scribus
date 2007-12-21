/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/**************************************************************************
*   Copyright (C) 2006 by Franz Schmid                                    *
*   franz.schmid@altmuehlnet.de                                           *
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
#include "customfdialog.h"
#include "pageitem_textframe.h"
#include "prefsmanager.h"
#include "prefsfile.h"
#include "util.h"
#include "stencilreader.h"
#include "scconfig.h"
#include "loadsaveplugin.h"
#include "fileloader.h"
#include "plugins/formatidlist.h"
#include <QPixmap>
#include <QImage>
#include <QDir>
#include <QFileDialog>
#include <QApplication>
#include <QEventLoop>
#include <QCursor>
#include <QImageReader>
#include <QPainter>
#include <QDebug>

PatternDialog::PatternDialog(QWidget* parent, QMap<QString, ScPattern> *docPatterns, ScribusDoc *doc, ScribusMainWindow *scMW) : QDialog(parent)
{
	setupUi(this);
	setModal(true);
	m_doc = doc;
	mainWin = scMW;
	patternView->clear();
	patternView->setMinimumWidth(175);
	for (QMap<QString, ScPattern>::Iterator it = docPatterns->begin(); it != docPatterns->end(); ++it)
	{
		dialogPatterns.insert(it.key(), it.value());
	}
	origPatterns = docPatterns->keys();
	updatePatternList();
	buttonRemove->setEnabled(false);
	connect(buttonOK, SIGNAL(clicked()), this, SLOT(accept()));
	connect(buttonCancel, SIGNAL(clicked()), this, SLOT(reject()));
	connect(buttonLoad, SIGNAL(clicked()), this, SLOT(loadPattern()));
	connect(buttonLoadDir, SIGNAL(clicked()), this, SLOT(loadPatternDir()));
	connect(buttonRemove, SIGNAL(clicked()), this, SLOT(removePattern()));
	connect(buttonRemoveAll, SIGNAL(clicked()), this, SLOT(removeAllPatterns()));
	connect(patternView, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(patternSelected(QListWidgetItem*)));
}

void PatternDialog::updatePatternList()
{
	patternView->clear();
	patternView->setIconSize(QSize(48, 48));
	patternView->setWordWrap(true);
	for (QMap<QString, ScPattern>::Iterator it = dialogPatterns.begin(); it != dialogPatterns.end(); ++it)
	{
		QPixmap pm;
		if (it.value().getPattern()->width() >= it.value().getPattern()->height())
			pm=QPixmap::fromImage(it.value().getPattern()->scaledToWidth(48, Qt::SmoothTransformation));
		else
			pm=QPixmap::fromImage(it.value().getPattern()->scaledToHeight(48, Qt::SmoothTransformation));
		QPixmap pm2(48, 48);
		pm2.fill(palette().color(QPalette::Base));
		QPainter p;
		p.begin(&pm2);
		p.drawPixmap(24 - pm.width() / 2, 24 - pm.height() / 2, pm);
		p.end();
		QListWidgetItem *item = new QListWidgetItem(pm2, it.key(), patternView);
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	}
}

void PatternDialog::loadPatternDir()
{
	PrefsContext* dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
	QString wdir = dirs->get("patterndir", ".");
	QString fileName = QFileDialog::getExistingDirectory(this, tr("Choose a Directory"), wdir);
	if (!fileName.isEmpty())
	{
		QStringList formats;
		formats += "eps";
		formats += "epsi";
		formats += "pdf";
		QString form1 = "";
		for ( int i = 0; i < QImageReader::supportedImageFormats().count(); ++i )
		{
			form1 = QString(QImageReader::supportedImageFormats().at(i)).toLower();
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
		formats += "tif";
	 	formats += "tiff";
		formats += "psd";
		formats += "pat";
		QDir d(fileName, "*", QDir::Name, QDir::Files | QDir::Readable | QDir::NoSymLinks);
		if ((d.exists()) && (d.count() != 0))
		{
			mainWin->setStatusBarInfoText( tr("Loading Patterns"));
			mainWin->mainWindowProgressBar->reset();
			mainWin->mainWindowProgressBar->setMaximum(d.count() * 2);
			qApp->changeOverrideCursor(QCursor(Qt::WaitCursor));
			qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
			for (uint dc = 0; dc < d.count(); ++dc)
			{
				mainWin->mainWindowProgressBar->setValue(dc);
				qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
				QFileInfo fi(QDir::cleanPath(QDir::convertSeparators(fileName + "/" + d[dc])));
				QString ext = fi.suffix().toLower();
				if ((ext == "sml") || (ext == "shape") || (ext == "sce") || (!formats.contains(ext)))
					loadVectors(QDir::cleanPath(QDir::convertSeparators(fileName + "/" + d[dc])));
			}
			for (uint dc = 0; dc < d.count(); ++dc)
			{
				mainWin->mainWindowProgressBar->setValue(d.count() + dc);
				qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
				QFileInfo fi(QDir::cleanPath(QDir::convertSeparators(fileName + "/" + d[dc])));
				QString ext = fi.suffix().toLower();
				if ((ext == "sml") || (ext == "shape") || (ext == "sce") || (!formats.contains(ext)))
					continue;
				else if (formats.contains(ext))
				{
					QString patNam = fi.baseName().trimmed().simplified().replace(" ", "_");
					if (!dialogPatterns.contains(patNam))
					{
						ScPattern pat = ScPattern();
						pat.setDoc(m_doc);
						pat.setPattern(QDir::cleanPath(QDir::convertSeparators(fileName + "/" + d[dc])));
						dialogPatterns.insert(patNam, pat);
					}
				}
				else
					continue;
			}
			d.cdUp();
			dirs->set("patterndir", d.absolutePath());
			qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
			mainWin->setStatusBarInfoText("");
			mainWin->mainWindowProgressBar->reset();
		}
		updatePatternList();
	}
}

void PatternDialog::loadPattern()
{
	QString fileName;
	QString formats = "Scribus Objects (*.sce *.SCE);;";
	formats += "Dia Shapes (*.shape *.SHAPE);;";
	formats += "Kivio Stencils (*.sml *.SML);;";
	int fmtCode = FORMATID_ODGIMPORT;
	const FileFormat *fmt = LoadSavePlugin::getFormatById(fmtCode);
	while (fmt != 0)
	{
		if (fmt->load)
			formats += fmt->filter + ";;";
		fmtCode++;
		if (fmtCode == FORMATID_PSIMPORT)
			fmtCode++;
		fmt = LoadSavePlugin::getFormatById(fmtCode);
	}
	formats += "EPS (*.eps *.EPS);;";
	formats += "EPSI (*.epsi *.EPSI);;";
	formats += "PDF (*.pdf *.PDF);;";
	QString form1 = "";
	QString form2 = "";
	QStringList imgFormats;
	bool jpgFound = false;
	for (int i = 0; i < QImageReader::supportedImageFormats().count(); ++i )
	{
		form1 = QString(QImageReader::supportedImageFormats().at(i)).toLower();
		form2 = QString(QImageReader::supportedImageFormats().at(i)).toUpper();
		if ((form1 == "png") || (form1 == "xpm") || (form1 == "gif"))
		{
			formats += form2 + " (*."+form1+" *."+form2+");;";
			imgFormats.append(form1);
		}
		else if ((form1 == "jpg") || (form1 == "jpeg"))
		{
			// JPEG is a special case because both .jpg and .jpeg
			// are acceptable extensions.
			if (!jpgFound)
			{
				formats += "JPEG (*.jpg *.jpeg *.JPG *.JPEG);;";
				imgFormats.append("jpeg");
				imgFormats.append("jpg");
				jpgFound = true;
			}
		}
		else
			imgFormats.append(form1);
	}
	formats += "TIFF (*.tif *.tiff *.TIF *.TIFF);;";
	formats += "PSD (*.psd *.PSD);;";
	formats += "Gimp Patterns (*.pat *.PAT);;";
	formats += tr("All Files (*)");
	imgFormats.append("tif");
	imgFormats.append("tiff");
	imgFormats.append("pat");
	imgFormats.append("psd");
	imgFormats.append("pdf");
	imgFormats.append("eps");
	imgFormats.append("epsi");
	imgFormats.append("ps");
	PrefsContext* dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
	QString wdir = dirs->get("patterns", ".");
	CustomFDialog dia(this, wdir, tr("Open"), formats, fdExistingFiles);
	if (dia.exec() == QDialog::Accepted)
		fileName = dia.selectedFile();
	else
		return;
	if (!fileName.isEmpty())
	{
		PrefsManager::instance()->prefsFile->getContext("dirs")->set("patterns", fileName.left(fileName.lastIndexOf("/")));
		QFileInfo fi(fileName);
		if ((fi.suffix().toLower() == "sml") || (fi.suffix().toLower() == "shape") || (fi.suffix().toLower() == "sce") || (!imgFormats.contains(fi.suffix().toLower())))
		{
			loadVectors(fileName);
			updatePatternList();
		}
		else
		{
			QString patNam = fi.baseName().trimmed().simplified().replace(" ", "_");
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
	m_doc->setLoading(true);
	QFileInfo fi(data);
	QString patNam = fi.baseName().trimmed().simplified().replace(" ", "_");
	if (fi.suffix().toLower() == "sml")
	{
		QString f = "";
		loadText(data, &f);
		StencilReader *pre = new StencilReader();
		data = pre->createObjects(f);
		delete pre;
	}
	else if (fi.suffix().toLower() == "shape")
	{
		QString f = "";
		loadText(data, &f);
		StencilReader *pre = new StencilReader();
		data = pre->createShape(f);
		delete pre;
	}
	uint ac = m_doc->Items->count();
	bool savedAlignGrid = m_doc->useRaster;
	bool savedAlignGuides = m_doc->SnapGuides;
	m_doc->useRaster = false;
	m_doc->SnapGuides = false;
	if (fi.suffix().toLower() == "sce")
		mainWin->slotElemRead(data, m_doc->currentPage()->xOffset(), m_doc->currentPage()->yOffset(), true, true, m_doc, m_doc->view());
	else if ((fi.suffix().toLower() == "shape") || (fi.suffix().toLower() == "sml"))
		mainWin->slotElemRead(data, m_doc->currentPage()->xOffset(), m_doc->currentPage()->yOffset(), false, true, m_doc, m_doc->view());
	else
	{
		FileLoader *fileLoader = new FileLoader(data);
		int testResult = fileLoader->TestFile();
		delete fileLoader;
		if ((testResult != -1) && (testResult >= FORMATID_ODGIMPORT))
		{
			const FileFormat * fmt = LoadSavePlugin::getFormatById(testResult);
			if( fmt )
				fmt->loadFile(data, LoadSavePlugin::lfUseCurrentPage|LoadSavePlugin::lfInteractive|LoadSavePlugin::lfScripted);
		}
	}
	m_doc->useRaster = savedAlignGrid;
	m_doc->SnapGuides = savedAlignGuides;
	uint ae = m_doc->Items->count();
	if (ac != ae)
	{
		for (uint as = ac; as < ae; ++as)
		{
			PageItem* ite = m_doc->Items->at(ac);
			if (ite->itemType() == PageItem::PathText)
			{
				ite->Frame = true;
				ite->updatePolyClip();
			}
			else if (ite->itemType() == PageItem::TextFrame)
			{
				if ( ite->prevInChain() == 0 )
					ite->asTextFrame()->layout();
			}
		}
		ScPattern pat = ScPattern();
		pat.setDoc(m_doc);
		PageItem* currItem = m_doc->Items->at(ac);
		pat.pattern = currItem->DrawObj_toImage();
		pat.width = currItem->gWidth;
		pat.height = currItem->gHeight;
		for (uint as = ac; as < ae; ++as)
		{
			pat.items.append(m_doc->Items->takeAt(ac));
		}
		if (!dialogPatterns.contains(patNam))
		{
			dialogPatterns.insert(patNam, pat);
		}
		for (QMap<QString, ScPattern>::Iterator it = m_doc->docPatterns.begin(); it != m_doc->docPatterns.end(); ++it)
		{
			if (!origPatterns.contains(it.key()))
				dialogPatterns.insert(it.key(), it.value());
		}
	}
	m_doc->setLoading(false);
	m_doc->view()->Deselect(false);
	m_doc->view()->DrawNew();
}

void PatternDialog::patternSelected(QListWidgetItem* it)
{
	if (it)
		buttonRemove->setEnabled(true);
	else
	{
		buttonRemove->setEnabled(false);
		patternView->clearSelection();
	}
}

void PatternDialog::removeAllPatterns()
{
	dialogPatterns.clear();
	updatePatternList();
}

void PatternDialog::removePattern()
{
	QListWidgetItem *it = patternView->currentItem();
	if (it)
	{
		QStringList patterns2Del;
		QStringList mainPatterns = dialogPatterns.keys();
		for (int a = 0; a < mainPatterns.count(); a++)
		{
			if (mainPatterns[a] != it->text())
			{
				QStringList subPatterns;
				subPatterns = getUsedPatternsHelper(mainPatterns[a], subPatterns);
				if (subPatterns.contains(it->text()))
					patterns2Del.append(mainPatterns[a]);
			}
		}
//		QStringList subPatterns;
//		subPatterns = getUsedPatternsHelper(it->text(), subPatterns);
//		if (!subPatterns.isEmpty())
//			patterns2Del += subPatterns;
		patterns2Del.append(it->text());
		for (int a = 0; a < patterns2Del.count(); a++)
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
	for (int c = 0; c < pat->items.count(); ++c)
	{
		if ((!results.contains(pat->items.at(c)->pattern())) && (pat->items.at(c)->GrType == 8))
			pats.append(pat->items.at(c)->pattern());
	}
	if (!pats.isEmpty())
	{
		for (int c = 0; c < pats.count(); ++c)
		{
			getUsedPatternsHelper(pats[c], results);
		}
	}
	return pats;
}
