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
#include "sctextstream.h"
#include "sccolorengine.h"
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
		p->setGradient(VGradient::linear, FPoint(0,6), FPoint(48, 6), FPoint(0, 0), 1.0, 0.0);
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
	allFormats += " *.ggr *.GGR";
	allFormats += ");;";
	QString formats = tr("Scribus Gradient Files \"*.sgr\" (*.sgr *.SGR);;");
	formats += tr("Gimp Gradient Files \"*.ggr\" (*.ggr *.GGR);;");
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
		else if (ext == "ggr")
			loadGimpFormat(fileName);
		updateGradientList();
	}
}

void gradientManagerDialog::loadGimpFormat(QString fileName)
{
	QFile f(fileName);
	if (f.open(QIODevice::ReadOnly))
	{
		ScTextStream ts(&f);
		QString tmp, dummy;
		QString gradientName = "";
		int numEntrys = 0;
		int entryCount = 0;
		int stopCount = 0;
		double left, middle, right, r0, g0, b0, a0, r1, g1, b1, a1, oldr1, oldg1, oldb1, olda1;
		tmp = ts.readLine();
		if (tmp.startsWith("GIMP Gradient"))
		{
			tmp = ts.readLine();
			ScTextStream CoE(&tmp, QIODevice::ReadOnly);
			CoE >> dummy;
			gradientName = CoE.readAll().trimmed();
		}
		if (!gradientName.isEmpty())
		{
			QString stopName = gradientName+QString("_Stop%1");
			QString stopNameInUse;
			VGradient gra = VGradient(VGradient::linear);
			gra.clearStops();
			QColor color;
			tmp = ts.readLine();
			ScTextStream CoE(&tmp, QIODevice::ReadOnly);
			CoE >> numEntrys;
			while (!ts.atEnd())
			{
				entryCount++;
				tmp = ts.readLine();
				ScTextStream Cval(&tmp, QIODevice::ReadOnly);
				Cval >> left >> middle >> right >> r0 >> g0 >> b0 >> a0 >> r1 >> g1 >> b1 >> a1;
				if ((entryCount == 1) && (entryCount < numEntrys))
				{
					stopNameInUse = stopName.arg(stopCount);
					addGimpColor(stopNameInUse, r0, g0, b0);
					color = QColor(qRound(r0 * 255), qRound(g0 * 255), qRound(b0 * 255));
					gra.addStop(color, left, 0.5, a0, stopNameInUse, 100);
					stopCount++;
				}
				else if (entryCount == numEntrys)
				{
					if ((entryCount != 1) && ((r0 != oldr1) || (g0 != oldg1) || (b0 != oldb1) || (a0 != olda1)))
					{
						stopNameInUse = stopName.arg(stopCount);
						addGimpColor(stopNameInUse, oldr1, oldg1, oldb1);
						color = QColor(qRound(oldr1 * 255), qRound(oldg1 * 255), qRound(oldb1 * 255));
						gra.addStop(color, left, 0.5, olda1, stopNameInUse, 100);
						stopCount++;
					}
					stopNameInUse = stopName.arg(stopCount);
					addGimpColor(stopNameInUse, r0, g0, b0);
					color = QColor(qRound(r0 * 255), qRound(g0 * 255), qRound(b0 * 255));
					gra.addStop(color, left, 0.5, a0, stopNameInUse, 100);
					stopCount++;
					stopNameInUse = stopName.arg(stopCount);
					addGimpColor(stopNameInUse, r1, g1, b1);
					color = QColor(qRound(r1 * 255), qRound(g1 * 255), qRound(b1 * 255));
					gra.addStop(color, right, 0.5, a1, stopNameInUse, 100);
					stopCount++;
				}
				else
				{
					if ((r0 == oldr1) && (g0 == oldg1) && (b0 == oldb1) && (a0 == olda1))
					{
						stopNameInUse = stopName.arg(stopCount);
						addGimpColor(stopNameInUse, r0, g0, b0);
						color = QColor(qRound(r0 * 255), qRound(g0 * 255), qRound(b0 * 255));
						gra.addStop(color, left, 0.5, a0, stopNameInUse, 100);
						stopCount++;
					}
					else
					{
						stopNameInUse = stopName.arg(stopCount);
						addGimpColor(stopNameInUse, oldr1, oldg1, oldb1);
						color = QColor(qRound(oldr1 * 255), qRound(oldg1 * 255), qRound(oldb1 * 255));
						gra.addStop(color, left, 0.5, olda1, stopNameInUse, 100);
						stopCount++;
						stopNameInUse = stopName.arg(stopCount);
						addGimpColor(stopNameInUse, r0, g0, b0);
						color = QColor(qRound(r0 * 255), qRound(g0 * 255), qRound(b0 * 255));
						gra.addStop(color, left, 0.5, a0, stopNameInUse, 100);
						stopCount++;
					}
				}
				oldr1 = r1;
				oldg1 = g1;
				oldb1 = b1;
				olda1 = a1;
			}
			if (!dialogGradients.contains(gradientName))
				dialogGradients.insert(gradientName, gra);
			else
			{
				QString tmp;
				gradientName += "("+tmp.setNum(dialogGradients.count())+")";
				dialogGradients.insert(gradientName, gra);
			}
		}
		f.close();
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

void gradientManagerDialog::addGimpColor(QString &colorName, double r, double g, double b)
{
	ScColor lf = ScColor();
	bool found = false;
	int Rc, Gc, Bc, hR, hG, hB;
	Rc = qRound(r * 255);
	Gc = qRound(g * 255);
	Bc = qRound(b * 255);
	lf.setColorRGB(Rc, Gc, Bc);
	for (ColorList::Iterator it = m_colorList.begin(); it != m_colorList.end(); ++it)
	{
		if (it.value().getColorModel() == colorModelRGB)
		{
			it.value().getRGB(&hR, &hG, &hB);
			if ((Rc == hR) && (Gc == hG) && (Bc == hB))
			{
				colorName = it.key();
				found = true;
				return;
			}
		}
	}
	if (!found)
	{
		m_colorList.insert(colorName, lf);
		hasImportedColors = true;
	}
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
	QString formats = tr("Scribus Gradient Files (*.sgr *.SGR);;");
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
