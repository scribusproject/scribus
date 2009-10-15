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
#include "prefsfile.h"
#include "scpainter.h"
#include "fpoint.h"
#include "ui/customfdialog.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "scclocale.h"
#include "util.h"
#include "util_icon.h"
#include "commonstrings.h"
#include <QPixmap>
#include <QImage>
#include <QPainter>
#include <QTextStream>
#include <QMessageBox>
#include <QDomDocument>

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
	hasImportedColors = false;
	connect(buttonOK, SIGNAL(clicked()), this, SLOT(accept()));
	connect(buttonCancel, SIGNAL(clicked()), this, SLOT(reject()));
	connect(buttonAdd, SIGNAL(clicked()), this, SLOT(addGradient()));
	connect(buttonRemove, SIGNAL(clicked()), this, SLOT(removeGradient()));
	connect(buttonRemoveAll, SIGNAL(clicked()), this, SLOT(removeAllGradients()));
	connect(buttonEdit, SIGNAL(clicked()), this, SLOT(editGradient()));
	connect(gradientsList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(gradientSelected(QListWidgetItem*)));
	connect(buttonLoad, SIGNAL(clicked()), this, SLOT(loadGradients()));
	connect(buttonSave, SIGNAL(clicked()), this, SLOT(saveGradients()));
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
	if (dialogGradients.count() == 0)
	{
		buttonRemoveAll->setEnabled(false);
		buttonSave->setEnabled(false);
	}
	else
	{
		buttonRemoveAll->setEnabled(true);
		buttonSave->setEnabled(true);
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

void gradientManagerDialog::removeAllGradients()
{
	for (QMap<QString, VGradient>::Iterator it = dialogGradients.begin(); it != dialogGradients.end(); ++it)
	{
		replaceMap.insert(it.key(), "");
	}
	dialogGradients.clear();
	updateGradientList();
}

void gradientManagerDialog::loadGradients()
{
	QString fileName;
	QString allFormats = tr("All Supported Formats")+" (";
	allFormats += "*.sgr *.SGR";
	allFormats += ");;";
	QString formats = tr("Scribus Gradient Files (*.sgr *SGR);;");
	formats += tr("All Files (*)");
	allFormats += formats;
	PrefsContext* dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
	QString wdir = dirs->get("gradients", ".");
	CustomFDialog dia(this, wdir, tr("Open"), allFormats, fdHidePreviewCheckBox | fdExistingFiles);
	if (dia.exec() == QDialog::Accepted)
		fileName = dia.selectedFile();
	else
		return;
	if (!fileName.isEmpty())
	{
		PrefsManager::instance()->prefsFile->getContext("dirs")->set("gradients", fileName.left(fileName.lastIndexOf("/")));
		QFileInfo fi(fileName);
		QString ext = fi.suffix().toLower();
		if (ext == "sgr")
			loadScribusFormat(fileName);
		updateGradientList();
	}
	/* File format is:
   *
   *   GIMP Gradient
   *   Name: name
   *   number_of_segments
   *   left middle right r0 g0 b0 a0 r1 g1 b1 a1 type coloring left_color_type
   *   left middle right r0 g0 b0 a0 r1 g1 b1 a1 type coloring right_color_type
   *   ...
   */

}

void gradientManagerDialog::loadScribusFormat(QString fileName)
{
	QFile f(fileName);
	if(!f.open(QIODevice::ReadOnly))
		return;
	QDomDocument docu("scridoc");
	QTextStream ts(&f);
	ts.setCodec("UTF-8");
	QString errorMsg;
	int errorLine = 0, errorColumn = 0;
	if( !docu.setContent(ts.readAll(), &errorMsg, &errorLine, &errorColumn) )
	{
		f.close();
		return;
	}
	f.close();
	QDomElement elem = docu.documentElement();
	if (elem.tagName() != "SCRIBUSGRADIENT")
		return;
	QDomNode DOC = elem.firstChild();
	while(!DOC.isNull())
	{
		QDomElement dc = DOC.toElement();
		if (dc.tagName()=="COLOR")
		{
			ScColor lf = ScColor();
			if (dc.hasAttribute("CMYK"))
				lf.setNamedColor(dc.attribute("CMYK"));
			else
				lf.fromQColor(QColor(dc.attribute("RGB")));
			if (dc.hasAttribute("Spot"))
				lf.setSpotColor(static_cast<bool>(dc.attribute("Spot").toInt()));
			else
				lf.setSpotColor(false);
			if (dc.hasAttribute("Register"))
				lf.setRegistrationColor(static_cast<bool>(dc.attribute("Register").toInt()));
			else
				lf.setRegistrationColor(false);
			if (!m_colorList.contains(dc.attribute("NAME")))
			{
				m_colorList.insert(dc.attribute("NAME"), lf);
				hasImportedColors = true;
			}
		}
		if (dc.tagName() == "Gradient")
		{
			VGradient gra = VGradient(VGradient::linear);
			gra.clearStops();
			QDomNode grad = dc.firstChild();
			while(!grad.isNull())
			{
				QDomElement stop = grad.toElement();
				QString name = stop.attribute("NAME");
				double ramp  = ScCLocale::toDoubleC(stop.attribute("RAMP"), 0.0);
				int shade    = stop.attribute("SHADE", "100").toInt();
				double opa   = ScCLocale::toDoubleC(stop.attribute("TRANS"), 1.0);
				QColor color;
				if (name == CommonStrings::None)
					color = QColor(255, 255, 255, 0);
				else
				{
					const ScColor& col = m_colorList[name];
					color = ScColorEngine::getShadeColorProof(col, NULL, shade);
				}
				gra.addStop(color, ramp, 0.5, opa, name, shade);
				grad = grad.nextSibling();
			}
			if (!dialogGradients.contains(dc.attribute("Name")))
				dialogGradients.insert(dc.attribute("Name"), gra);
			else
			{
				QString tmp;
				QString name = dc.attribute("Name");
				name += "("+tmp.setNum(dialogGradients.count())+")";
				dialogGradients.insert(name, gra);
			}
		}
		DOC=DOC.nextSibling();
	}
}

void gradientManagerDialog::saveGradients()
{
	QString fileName;
	QString allFormats = tr("All Supported Formats")+" (";
	allFormats += "*.sgr *.SGR";
	allFormats += ");;";
	QString formats = tr("Scribus Gradient Files (*.sgr *SGR);;");
	formats += tr("All Files (*)");
	allFormats += formats;
	QString wdir = PrefsManager::instance()->prefsFile->getContext("dirs")->get("gradients", ".");
	CustomFDialog dia(this, wdir, tr("Save as"), allFormats, fdHidePreviewCheckBox | fdNone);
	if (dia.exec() == QDialog::Accepted)
		fileName = dia.selectedFile();
	else
		return;
	if (!fileName.isEmpty())
	{
		if (!fileName.endsWith(".sgr"))
			fileName += ".sgr";
		PrefsManager::instance()->prefsFile->getContext("dirs")->set("gradients", fileName.left(fileName.lastIndexOf("/")));
		if (overwrite(this, fileName))
		{
			ColorList gradientColors;
			for (QMap<QString, VGradient>::Iterator it = dialogGradients.begin(); it != dialogGradients.end(); ++it)
			{
				QList<VColorStop*> cs = it.value().colorStops();
				for (uint i = 0; i < it.value().Stops(); ++i)
				{
					if (cs.at(i)->name != CommonStrings::None)
						gradientColors.insert(cs.at(i)->name, m_colorList[cs.at(i)->name]);
				}
			}
			QDomDocument docu("scribusrc");
			QString st="<SCRIBUSGRADIENT></SCRIBUSGRADIENT>";
			docu.setContent(st);
			QDomElement elem=docu.documentElement();
			ColorList::Iterator itc;
			for (itc = gradientColors.begin(); itc != gradientColors.end(); ++itc)
			{
				QDomElement co=docu.createElement("COLOR");
				co.setAttribute("NAME",itc.key());
				if (itc.value().getColorModel() == colorModelRGB)
					co.setAttribute("RGB", itc.value().nameRGB());
				else
					co.setAttribute("CMYK", itc.value().nameCMYK());
				co.setAttribute("Spot", static_cast<int>(itc.value().isSpotColor()));
				co.setAttribute("Register", static_cast<int>(itc.value().isRegistrationColor()));
				elem.appendChild(co);
			}
			QMap<QString, VGradient>::Iterator itGrad;
			for (itGrad = dialogGradients.begin(); itGrad != dialogGradients.end(); ++itGrad)
			{
				QDomElement grad = docu.createElement("Gradient");
				grad.setAttribute("Name",itGrad.key());
				VGradient gra = itGrad.value();
				QList<VColorStop*> cstops = gra.colorStops();
				for (uint cst = 0; cst < gra.Stops(); ++cst)
				{
					QDomElement stop = docu.createElement("CSTOP");
					stop.setAttribute("NAME", cstops.at(cst)->name);
					stop.setAttribute("RAMP", ScCLocale::toQStringC(cstops.at(cst)->rampPoint));
					stop.setAttribute("TRANS", ScCLocale::toQStringC(cstops.at(cst)->opacity));
					stop.setAttribute("SHADE", cstops.at(cst)->shade);
					grad.appendChild(stop);
				}
				elem.appendChild(grad);
			}
		// write file
			QFile f(fileName);
			if(f.open(QIODevice::WriteOnly))
			{
				QTextStream s(&f);
				s.setCodec("UTF-8");
				s << docu.toString();
				f.close();
			}
			else
				QMessageBox::warning(this, CommonStrings::trWarning, tr("Cannot write the file: \n%1").arg(fileName), CommonStrings::tr_OK);
		}
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