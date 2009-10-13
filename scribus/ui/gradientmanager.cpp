/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/**************************************************************************
*   Copyright (C) 2009 by Franz Schmid                                    *
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

#include "gradientaddedit.h"
#include "gradientmanager.h"
#include "scpainter.h"
#include "fpoint.h"
#include "util_icon.h"
#include <QPixmap>
#include <QImage>
#include <QPainter>

gradientManagerDialog::gradientManagerDialog(QWidget* parent, QMap<QString, VGradient> *docGradients, ColorList doco, ScribusDoc *doc, ScribusMainWindow *scMW) : QDialog(parent)
{
	setupUi(this);
	setModal(true);
	m_doc = doc;
	m_colorList = doco;
	mainWin = scMW;
	gradientsList->clear();
	for (QMap<QString, VGradient>::Iterator it = docGradients->begin(); it != docGradients->end(); ++it)
	{
		dialogGradients.insert(it.key(), it.value());
		origNames.insert(it.key(), it.key());
	}
	origGradients = docGradients->keys();
	updateGradientList();
	replaceMap.clear();
	buttonRemove->setEnabled(false);
	buttonEdit->setEnabled(false);
	connect(buttonOK, SIGNAL(clicked()), this, SLOT(accept()));
	connect(buttonCancel, SIGNAL(clicked()), this, SLOT(reject()));
	connect(buttonAdd, SIGNAL(clicked()), this, SLOT(addGradient()));
	connect(buttonRemove, SIGNAL(clicked()), this, SLOT(removeGradient()));
	connect(buttonEdit, SIGNAL(clicked()), this, SLOT(editGradient()));
	connect(gradientsList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(gradientSelected(QListWidgetItem*)));
}

void gradientManagerDialog::updateGradientList()
{
	gradientsList->clear();
	for (QMap<QString, VGradient>::Iterator it = dialogGradients.begin(); it != dialogGradients.end(); ++it)
	{
		QImage pixm(48, 12, QImage::Format_ARGB32);
		QPainter pb;
		QBrush b(QColor(205,205,205), loadIcon("testfill.png"));
		pb.begin(&pixm);
		pb.fillRect(0, 0, 48, 12, b);
		pb.end();
		ScPainter *p = new ScPainter(&pixm, 48, 12);
		p->setPen(Qt::black);
		p->setLineWidth(1);
		p->setFillMode(2);
		p->fill_gradient = it.value();
		p->setGradient(VGradient::linear, FPoint(0,6), FPoint(48, 6));
		p->drawRect(0, 0, 48, 12);
		p->end();
		delete p;
		QPixmap pm;
		pm = QPixmap::fromImage(pixm);
		QListWidgetItem *item = new QListWidgetItem(pm, it.key(), gradientsList);
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	}
}

void gradientManagerDialog::gradientSelected(QListWidgetItem* it)
{
	if (it)
	{
		buttonRemove->setEnabled(true);
		buttonEdit->setEnabled(true);
	}
	else
	{
		buttonRemove->setEnabled(false);
		buttonEdit->setEnabled(false);
		gradientsList->clearSelection();
	}
}

void gradientManagerDialog::addGradient()
{
	VGradient fill_gradient = VGradient(VGradient::linear);
	fill_gradient.clearStops();
	fill_gradient.addStop(QColor(Qt::black), 0.0, 0.5, 1.0, "Black", 100);
	fill_gradient.addStop(QColor(Qt::white), 1.0, 0.5, 1.0, "White", 100);
	gradientEditDialog *dia = new gradientEditDialog(this, tr("New Gradient"), fill_gradient, m_colorList, m_doc, &dialogGradients, true);
	if (dia->exec())
	{
		dialogGradients.insert(dia->name(), dia->gradient());
		updateGradientList();
	}
	delete dia;
}

void gradientManagerDialog::editGradient()
{
	QListWidgetItem *it = gradientsList->currentItem();
	if (it)
	{
		QString gradN = it->text();
		QString patternName = origNames[it->text()];
		QString newName = "";
		gradientEditDialog *dia = new gradientEditDialog(this, gradN, dialogGradients[gradN], m_colorList, m_doc, &dialogGradients, false);
		if (dia->exec())
		{
			newName = dia->name();
			if (newName != gradN)
			{
				origNames.remove(patternName);
				origNames.insert(newName, patternName);
				replaceMap.insert(patternName, newName);
				dialogGradients.remove(gradN);
				dialogGradients.insert(newName, dia->gradient());
			}
			else
				dialogGradients[gradN] = dia->gradient();
			updateGradientList();
		}
		delete dia;
	}
}

void gradientManagerDialog::removeGradient()
{
	QListWidgetItem *it = gradientsList->currentItem();
	if (it)
	{
		dialogGradients.remove(it->text());
		replaceMap.insert(it->text(), "");
		updateGradientList();
	}
}
/*
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
						origNames.insert(patNam, patNam);
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
	QString formats = "";
	QString allFormats = tr("All Supported Formats")+" (";
	int fmtCode = FORMATID_ODGIMPORT;
	const FileFormat *fmt = LoadSavePlugin::getFormatById(fmtCode);
	while (fmt != 0)
	{
		if (fmt->load)
		{
			formats += fmt->filter + ";;";
			int an = fmt->filter.indexOf("(");
			int en = fmt->filter.indexOf(")");
			while (an != -1)
			{
				allFormats += fmt->filter.mid(an+1, en-an-1)+" ";
				an = fmt->filter.indexOf("(", en);
				en = fmt->filter.indexOf(")", an);
			}
		}
		fmtCode++;
		fmt = LoadSavePlugin::getFormatById(fmtCode);
	}
	allFormats += "*.sce *.SCE ";
	allFormats += "*.shape *.SHAPE ";
	allFormats += "*.sml *.SML ";
	formats += "Scribus Objects (*.sce *.SCE);;";
	formats += "Dia Shapes (*.shape *.SHAPE);;";
	formats += "Kivio Stencils (*.sml *.SML);;";
	QString form1 = "";
	QString form2 = "";
	QStringList imgFormats;
	bool jpgFound = false;
	bool tiffFound = false;
	for (int i = 0; i < QImageReader::supportedImageFormats().count(); ++i )
	{
		form1 = QString(QImageReader::supportedImageFormats().at(i)).toLower();
		form2 = QString(QImageReader::supportedImageFormats().at(i)).toUpper();
		if ((form1 == "png") || (form1 == "xpm") || (form1 == "gif"))
		{
			formats += form2 + " (*."+form1+" *."+form2+");;";
			allFormats += "*."+form1+" *."+form2+" ";
			imgFormats.append(form1);
		}
		else if ((form1 == "jpg") || (form1 == "jpeg"))
		{
			// JPEG is a special case because both .jpg and .jpeg
			// are acceptable extensions.
			if (!jpgFound)
			{
				formats += "JPEG (*.jpg *.jpeg *.JPG *.JPEG);;";
				allFormats += "*.jpg *.jpeg *.JPG *.JPEG ";
				imgFormats.append("jpeg");
				imgFormats.append("jpg");
				jpgFound = true;
			}
		}
		else if ((form1 == "tif") || (form1 == "tiff"))
		{
			if (!tiffFound)
			{
				formats += "TIFF (*.tif *.tiff *.TIF *.TIFF);;";
				allFormats += "*.tif *.tiff *.TIF *.TIFF ";
				imgFormats.append("tif");
				imgFormats.append("tiff");
				tiffFound = true;
			}
		}
		else if (form1 != "svg")
		{
			imgFormats.append(form1);
			allFormats += "*."+form1+" *."+form2+" ";
		}
	}
	if (!tiffFound)
	{
		formats += "TIFF (*.tif *.tiff *.TIF *.TIFF);;";
		allFormats += "*.tif *.tiff *.TIF *.TIFF ";
	}
	if (!jpgFound)
	{
		formats += "JPEG (*.jpg *.jpeg *.JPG *.JPEG);;";
		allFormats += "*.jpg *.jpeg *.JPG *.JPEG ";
	}
	formats += "PSD (*.psd *.PSD);;";
	formats += "Gimp Patterns (*.pat *.PAT);;";
	formats += tr("All Files (*)");
	allFormats += "*.psd *.PSD ";
	allFormats += "*.pat *.PAT);;";
	imgFormats.append("tif");
	imgFormats.append("tiff");
	imgFormats.append("pat");
	imgFormats.append("psd");
	imgFormats.append("pdf");
	imgFormats.append("eps");
	imgFormats.append("epsi");
	imgFormats.append("ps");
	allFormats += formats;
	PrefsContext* dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
	QString wdir = dirs->get("patterns", ".");
	CustomFDialog dia(this, wdir, tr("Open"), allFormats, fdHidePreviewCheckBox | fdExistingFiles);
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
				origNames.insert(patNam, patNam);
				updatePatternList();
			}
		}
	}
}

void PatternDialog::loadVectors(QString data)
{
	bool wasUndo = UndoManager::undoEnabled();
	UndoManager::instance()->setUndoEnabled(false);
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
			else
			{
				ite->layout();
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
			origNames.insert(patNam, patNam);
		}
		for (QMap<QString, ScPattern>::Iterator it = m_doc->docPatterns.begin(); it != m_doc->docPatterns.end(); ++it)
		{
			if (!origPatterns.contains(it.key()))
			{
				dialogPatterns.insert(it.key(), it.value());
				origNames.insert(it.key(), it.key());
			}
		}
	}
	m_doc->setLoading(false);
	m_doc->view()->Deselect(false);
	UndoManager::instance()->setUndoEnabled(wasUndo);
	m_doc->view()->DrawNew();
}

void PatternDialog::patternSelected(QListWidgetItem* it)
{
	if (it)
	{
		buttonRemove->setEnabled(true);
		buttonRename->setEnabled(true);
	}
	else
	{
		buttonRemove->setEnabled(false);
		buttonRename->setEnabled(false);
		patternView->clearSelection();
	}
}

void PatternDialog::removeAllPatterns()
{
	dialogPatterns.clear();
	updatePatternList();
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
*/