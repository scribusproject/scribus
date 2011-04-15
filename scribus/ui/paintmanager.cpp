/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/**************************************************************************
*   Copyright (C) 2010 by Franz Schmid                                    *
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
#include "paintmanager.h"

#include <QCheckBox>
#include <QMessageBox>
#include <QDomDocument>
#include <QMenu>
#include <QCursor>
#include <QEventLoop>
#include <QImageReader>
#include <QDir>
#include <QFileDialog>
#include "cmykfw.h"
#include "colorlistbox.h"
#include "commonstrings.h"
#include "ui/customfdialog.h"
#include "dcolor.h"
#include "fileloader.h"
#include "gradientaddedit.h"
#include "loadsaveplugin.h"
#include "plugins/formatidlist.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "query.h"
#include "sccolor.h"
#include "sccolorengine.h"
#include "scconfig.h"
#include "sclimits.h"
#include "sclistboxpixmap.h"
#include "scclocale.h"
#include "scpainter.h"
#include "scpaths.h"
#include "sctextstream.h"
#include "scribusXml.h"
#include "undomanager.h"
#include "util.h"
#include "util_color.h"
#include "util_formats.h"
#include "util_icon.h"

PaintManagerDialog::PaintManagerDialog(QWidget* parent, QMap<QString, VGradient> *docGradients, ColorList doco, QString docColSet, QMap<QString, ScPattern> *docPatterns, ScribusDoc *doc, ScribusMainWindow *scMW) : QDialog(parent)
{
	setupUi(this);
	setModal(true);
	paletteLocked = false;
	modified = false;
	sortRule = 0;
	m_doc = doc;
	m_colorList = doco;
	mainWin = scMW;
	setWindowIcon(QIcon(loadIcon ( "AppIcon.png" )));
	dataTree->setContextMenuPolicy(Qt::CustomContextMenu);
	dataTree->setIconSize(QSize(60, 48));
	colorItems = new QTreeWidgetItem(dataTree);
	colorItems->setText(0, tr("Solid Colors"));
	gradientItems = new QTreeWidgetItem(dataTree);
	gradientItems->setText(0, tr("Gradients"));
	for (QMap<QString, VGradient>::Iterator it = docGradients->begin(); it != docGradients->end(); ++it)
	{
		dialogGradients.insert(it.key(), it.value());
		origNames.insert(it.key(), it.key());
	}
	origGradients = docGradients->keys();

	patternItems = new QTreeWidgetItem(dataTree);
	patternItems->setText(0, tr("Patterns"));
	for (QMap<QString, ScPattern>::Iterator it = docPatterns->begin(); it != docPatterns->end(); ++it)
	{
		dialogPatterns.insert(it.key(), it.value());
		origNamesPatterns.insert(it.key(), it.key());
	}
	origPatterns = docPatterns->keys();

	csm.findPaletteLocations();
	systemSwatches = LoadColSet->addTopLevelItem( tr("Scribus Swatches"));
	csm.findPalettes(systemSwatches);
	LoadColSet->addSubItem("Scribus Small", systemSwatches);
	systemSwatches->setExpanded(true);
	userSwatches = LoadColSet->addTopLevelItem( tr("User Swatches"));
	csm.findUserPalettes(userSwatches);
	customColSet = csm.userPaletteNames();
	userSwatches->setExpanded(true);
//	QFileInfo fi(docColSet);
	LoadColSet->setCurrentComboItem(docColSet);
	if (mainWin->HaveDoc)
	{
		label->setText( tr("Merge Color Set"));
		m_doc->getUsedColors(inDocUsedColors);
		paletteLocked = false;
	}
	else
	{
		if (docColSet != "Scribus Small")
		{
			QString pfad = "";
			if (customColSet.contains(docColSet))
				pfad = csm.userPaletteFileFromName(docColSet);
			else
				pfad = csm.paletteFileFromName(docColSet);
			QFileInfo fi(pfad);
			if (fi.absolutePath().contains(ScPaths::getApplicationDataDir()+"swatches/locked"))
				paletteLocked = true;
			else
				paletteLocked = !fi.isWritable();
		}
	}
	importButton->setEnabled(false);
	newButton->setEnabled(false);
	editButton->setEnabled(false);
	duplicateButton->setEnabled(false);
	deleteButton->setEnabled(false);
	deleteUnusedButton->setEnabled(false);
	updateColorList();
	updateGradientList();
	updatePatternList();
	dataTree->expandItem(colorItems);
	dataTree->expandItem(gradientItems);
	dataTree->expandItem(patternItems);
	dataTree->setSelectionMode(QAbstractItemView::ExtendedSelection);
	connect(dataTree, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(itemSelected(QTreeWidgetItem*)));
	connect(dataTree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(selEditColor(QTreeWidgetItem*)));
	connect(dataTree, SIGNAL(itemSelectionChanged()), this, SLOT(itemSelectionChanged()));
	connect(dataTree, SIGNAL(customContextMenuRequested (const QPoint &)), this, SLOT(slotRightClick(QPoint)));
	connect(newButton, SIGNAL(clicked()), this, SLOT(createNew()));
	connect(editButton, SIGNAL(clicked()), this, SLOT(editColorItem()));
	connect(duplicateButton, SIGNAL(clicked()), this, SLOT(duplicateColorItem()));
	connect(deleteButton, SIGNAL(clicked()), this, SLOT(removeColorItem()));
	connect(deleteUnusedButton, SIGNAL(clicked()), this, SLOT(removeUnusedColorItem()));
	connect(importButton, SIGNAL(clicked()), this, SLOT(importColorItems()));
	connect(okButton, SIGNAL(clicked()), this, SLOT(leaveDialog()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	connect(SaveColSet, SIGNAL(clicked()), this, SLOT(saveDefaults()));
	connect(LoadColSet, SIGNAL(activated(QTreeWidgetItem*)), this, SLOT(loadDefaults(QTreeWidgetItem*)));
}

void PaintManagerDialog::leaveDialog()
{
/*	if (!mainWin->HaveDoc)
	{
		if (modified)
		{
			if (paletteLocked)
				saveDefaults();
			else
				doSaveDefaults(LoadColSet->text(), true);
		}
	} */
	accept();
}

QTreeWidgetItem* PaintManagerDialog::updatePatternList(QString addedName)
{
	QList<QTreeWidgetItem*> lg = patternItems->takeChildren();
	for (int a = 0; a < lg.count(); a++)
	{
		delete lg[a];
	}
	QTreeWidgetItem* ret = 0;
	for (QMap<QString, ScPattern>::Iterator it = dialogPatterns.begin(); it != dialogPatterns.end(); ++it)
	{
		QPixmap pm;
		if (it.value().getPattern()->width() >= it.value().getPattern()->height())
			pm = QPixmap::fromImage(it.value().getPattern()->scaledToWidth(48, Qt::SmoothTransformation));
		else
			pm = QPixmap::fromImage(it.value().getPattern()->scaledToHeight(48, Qt::SmoothTransformation));
		QPixmap pm2(48, 48);
		pm2.fill(palette().color(QPalette::Base));
		QPainter p;
		p.begin(&pm2);
		p.drawPixmap(24 - pm.width() / 2, 24 - pm.height() / 2, pm);
		p.end();
		QTreeWidgetItem *item = new QTreeWidgetItem(patternItems);
		item->setText(0, it.key());
		if (it.key() == addedName)
			ret = item;
		item->setIcon(0, pm2);
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	}
	return ret;
}

QTreeWidgetItem* PaintManagerDialog::updateGradientList(QString addedName)
{
	QList<QTreeWidgetItem*> lg = gradientItems->takeChildren();
	for (int a = 0; a < lg.count(); a++)
	{
		delete lg[a];
	}
	QTreeWidgetItem* ret = 0;
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
		QTreeWidgetItem *item = new QTreeWidgetItem(gradientItems);
		item->setText(0, it.key());
		if (it.key() == addedName)
			ret = item;
		item->setIcon(0, pm);
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	}
	return ret;
}

QTreeWidgetItem* PaintManagerDialog::updateColorList(QString addedName)
{
	QList<QTreeWidgetItem*> lg = colorItems->takeChildren();
	for (int a = 0; a < lg.count(); a++)
	{
		delete lg[a];
	}
	QTreeWidgetItem* ret = 0;
	if (sortRule > 0)
	{
		QMap<QString, QString> sortMap;
		ColorList::Iterator it;
		for (it = m_colorList.begin(); it != m_colorList.end(); ++it)
		{
			if (sortRule == 1)
			{
				QColor c = it.value().getRawRGBColor();
				QString sortString = QString("%1-%2-%3-%4").arg(c.hue(), 3, 10, QChar('0')).arg(c.saturation(), 3, 10, QChar('0')).arg(c.value(), 3, 10, QChar('0')).arg(it.key());
				sortMap.insert(sortString, it.key());
			}
			else if (sortRule == 2)
			{
				QString sortString = QString("%1-%2");
				if (it.value().isRegistrationColor())
					sortMap.insert(sortString.arg("A").arg(it.key()), it.key());
				else if (it.value().isSpotColor())
					sortMap.insert(sortString.arg("B").arg(it.key()), it.key());
				else if (it.value().getColorModel() == colorModelCMYK)
					sortMap.insert(sortString.arg("C").arg(it.key()), it.key());
				else
					sortMap.insert(sortString.arg("D").arg(it.key()), it.key());
			}
		}
		QMap<QString, QString>::Iterator itc;
		for (itc = sortMap.begin(); itc != sortMap.end(); ++itc)
		{
			QTreeWidgetItem *item = new QTreeWidgetItem(colorItems);
			item->setText(0, itc.value());
			if (itc.value() == addedName)
				ret = item;
			QPixmap* pPixmap = getFancyPixmap(m_colorList[itc.value()], m_doc);
			item->setIcon(0, *pPixmap);
			item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		}
	}
	else
	{
		ColorList::Iterator it;
		for (it = m_colorList.begin(); it != m_colorList.end(); ++it)
		{
			QTreeWidgetItem *item = new QTreeWidgetItem(colorItems);
			item->setText(0, it.key());
			if (it.key() == addedName)
				ret = item;
			QPixmap* pPixmap = getFancyPixmap(it.value(), m_doc);
			item->setIcon(0, *pPixmap);
			item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		}
	}
	return ret;
}

void PaintManagerDialog::slotRightClick(QPoint p)
{
	QTreeWidgetItem* it = dataTree->itemAt(p);
	if (it)
	{
		if ((it->parent() == colorItems) || (it == colorItems))
		{
			QMenu *pmen = new QMenu();
			qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
			pmen->addAction( tr("Sort by Name"));
			pmen->addAction( tr("Sort by Color"));
			pmen->addAction( tr("Sort by Type"));
			sortRule = pmen->actions().indexOf(pmen->exec(QCursor::pos()));
			delete pmen;
			updateColorList();
		}
	}
}

void PaintManagerDialog::selEditColor(QTreeWidgetItem *it)
{
	if ((it) && (!paletteLocked))
	{
		if ((it->parent() == colorItems) || (it->parent() == gradientItems))
		{
			QString curCol = it->text(0);
			ScColor tmpColor = m_colorList[curCol];
			bool enableDel  = (curCol != "Black" && curCol != "White" && !tmpColor.isRegistrationColor()) && (m_colorList.count() > 1);
			bool enableEdit = (curCol != "Black" && curCol != "White" && !tmpColor.isRegistrationColor());
			duplicateButton->setEnabled(curCol != "Registration");
			deleteButton->setEnabled(enableDel);
			editButton->setEnabled(enableEdit);
			if(enableEdit)
				editColorItem();
		}
	}
}

void PaintManagerDialog::itemSelectionChanged()
{
	QList<QTreeWidgetItem *> selItems = dataTree->selectedItems();
	if (selItems.count() > 1)
		deleteButton->setEnabled(true);
}

void PaintManagerDialog::itemSelected(QTreeWidgetItem* it)
{
	QList<QTreeWidgetItem *> selItems = dataTree->selectedItems();
	importButton->setText( tr("&Import"));
	newButton->setText( tr("Add"));
	editButton->setText( tr("&Edit"));
	if ((it) && (!paletteLocked))
	{
		if ((it->parent() == colorItems) || (it->parent() == gradientItems))
		{
			importButton->setEnabled(false);
			newButton->setEnabled(true);
			deleteUnusedButton->setEnabled(it->parent() == colorItems);
			if (it->parent() == colorItems)
			{
				QString curCol = it->text(0);
				ScColor tmpColor = m_colorList[curCol];
				bool enableDel  = (curCol != "Black" && curCol != "White" && !tmpColor.isRegistrationColor()) && (m_colorList.count() > 1);
				bool enableEdit = (curCol != "Black" && curCol != "White" && !tmpColor.isRegistrationColor());
				duplicateButton->setEnabled(curCol != "Registration");
				deleteButton->setEnabled(enableDel);
				editButton->setEnabled(enableEdit);
			}
			else
			{
				editButton->setEnabled(true);
				duplicateButton->setEnabled(true);
				deleteButton->setEnabled(true);
			}
		}
		else if (it->parent() == patternItems)
		{
			importButton->setText( tr("Load File"));
			newButton->setText( tr("Load Set"));
			editButton->setText( tr("Rename"));
			importButton->setEnabled(true);
			newButton->setEnabled(true);
			editButton->setEnabled(true);
			duplicateButton->setEnabled(false);
			deleteButton->setEnabled(true);
		}
		else if (it == patternItems)
		{
			importButton->setText( tr("Load File"));
			newButton->setText( tr("Load Set"));
			importButton->setEnabled(true);
			newButton->setEnabled(true);
			editButton->setEnabled(false);
			duplicateButton->setEnabled(false);
			if (selItems.count() < 2)
				deleteButton->setEnabled(it->childCount() > 0);
			else
				deleteButton->setEnabled(true);
		}
		else
		{
			importButton->setEnabled(true);
			newButton->setEnabled(true);
			editButton->setEnabled(false);
			duplicateButton->setEnabled(false);
			if (selItems.count() < 2)
				deleteButton->setEnabled(it->childCount() > 0);
			else
				deleteButton->setEnabled(true);
			deleteUnusedButton->setEnabled((it == colorItems) && (it->childCount() > 0));
		}
	}
	else
	{
		importButton->setEnabled(false);
		newButton->setEnabled(false);
		editButton->setEnabled(false);
		duplicateButton->setEnabled(false);
		deleteButton->setEnabled(false);
		deleteUnusedButton->setEnabled(false);
		dataTree->clearSelection();
	}
}

void PaintManagerDialog::createNew()
{
	if (paletteLocked)
		return;
	QTreeWidgetItem* it = dataTree->currentItem();
	if (it)
	{
		if ((it->parent() == gradientItems) || (it == gradientItems))
		{
			VGradient fill_gradient = VGradient(VGradient::linear);
			fill_gradient.clearStops();
			fill_gradient.addStop(QColor(Qt::black), 0.0, 0.5, 1.0, "Black", 100);
			fill_gradient.addStop(QColor(Qt::white), 1.0, 0.5, 1.0, "White", 100);
			gradientEditDialog *dia = new gradientEditDialog(this, tr("New Gradient"), fill_gradient, m_colorList, m_doc, &dialogGradients, true);
			if (dia->exec())
			{
				dialogGradients.insert(dia->name(), dia->gradient());
				QTreeWidgetItem *lg = updateGradientList(dia->name());
				if (lg != 0)
				{
					dataTree->expandItem(lg->parent());
					dataTree->setCurrentItem(lg, 0, QItemSelectionModel::ClearAndSelect);
				}
				itemSelected(dataTree->currentItem());
				modified = true;
			}
			delete dia;
		}
		else if ((it->parent() == colorItems) || (it == colorItems))
		{
			ScColor tmpColor = ScColor(0, 0, 0, 0);
			CMYKChoose* dia = new CMYKChoose(this, m_doc, tmpColor, tr("New Color"), &m_colorList, true);
			if (dia->exec())
			{
				dia->Farbe.setSpotColor(dia->Separations->isChecked());
				m_colorList.insert(dia->Farbname->text(), dia->Farbe);
				QTreeWidgetItem *lg = updateColorList(dia->Farbname->text());
				if (lg != 0)
				{
					dataTree->expandItem(lg->parent());
					dataTree->setCurrentItem(lg, 0, QItemSelectionModel::ClearAndSelect);
				}
				itemSelected(dataTree->currentItem());
				modified = true;
			}
			delete dia;
		}
		else if ((it->parent() == patternItems) || (it == patternItems))
		{
			loadPatternDir();
			updateColorList();
			updateGradientList();
			updatePatternList();
			itemSelected(0);
		}
	}
}

void PaintManagerDialog::editColorItem()
{
	if (paletteLocked)
		return;
	QTreeWidgetItem* it = dataTree->currentItem();
	if (it)
	{
		if (it->parent() == gradientItems)
		{
			QString gradN = it->text(0);
			QString patternName = origNames[it->text(0)];
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
				QStringList patterns = dialogPatterns.keys();
				for (int c = 0; c < dialogPatterns.count(); ++c)
				{
					ScPattern pa = dialogPatterns[patterns[c]];
					for (int o = 0; o < pa.items.count(); o++)
					{
						PageItem *ite = pa.items.at(o);
						if (ite->gradient() == gradN)
							ite->setGradient(newName);
						if (ite->strokeGradient() == gradN)
							ite->setStrokeGradient(newName);
						if (ite->gradientMask() == gradN)
							ite->setGradientMask(newName);
					}
					PageItem *ite = pa.items.at(0);
					dialogPatterns[patterns[c]].pattern = ite->DrawObj_toImage(pa.items, 1.0);
				}
				QTreeWidgetItem *lg = updateGradientList(dia->name());
				if (lg != 0)
				{
					dataTree->expandItem(lg->parent());
					dataTree->setCurrentItem(lg, 0, QItemSelectionModel::ClearAndSelect);
				}
				itemSelected(dataTree->currentItem());
				modified = true;
			}
			delete dia;
		}
		else if (it->parent() == colorItems)
		{
			ScColor tmpColor = m_colorList[it->text(0)];
			CMYKChoose* dia = new CMYKChoose(this, m_doc, tmpColor, it->text(0), &m_colorList, false);
			if (dia->exec())
			{
				dia->Farbe.setSpotColor(dia->Separations->isChecked());
				dia->Farbe.setRegistrationColor(tmpColor.isRegistrationColor());
				m_colorList[dia->Farbname->text()] = dia->Farbe;
				if (it->text(0) != dia->Farbname->text())
				{
					replaceColorMap.insert(it->text(0), dia->Farbname->text());
					m_colorList.remove(it->text(0));
				}
				updateGradientColors(dia->Farbname->text(), it->text(0));
				updateGradientList();
				updatePatternList();
				QTreeWidgetItem *lg = updateColorList(dia->Farbname->text());
				if (lg != 0)
				{
					dataTree->expandItem(lg->parent());
					dataTree->setCurrentItem(lg, 0, QItemSelectionModel::ClearAndSelect);
				}
				itemSelected(dataTree->currentItem());
				modified = true;
			}
			delete dia;
		}
		else if (it->parent() == patternItems)
		{
			QString patternName = origNames[it->text(0)];
			QString newName = "";
			Query dia(this, "tt", 1, 0, tr("&Name:"), tr("Rename Entry"));
			dia.setEditText(it->text(0), true);
			dia.setTestList(dialogPatterns.keys());
			if (dia.exec())
			{
				newName = dia.getEditText();
				ScPattern pat = dialogPatterns.take(it->text(0));
				dialogPatterns.insert(newName, pat);
				replaceMapPatterns.insert(patternName, newName);
				origNamesPatterns.remove(it->text(0));
				origNamesPatterns.insert(newName, patternName);
				QStringList patterns = dialogPatterns.keys();
				for (int c = 0; c < dialogPatterns.count(); ++c)
				{
					ScPattern pa = dialogPatterns[patterns[c]];
					for (int o = 0; o < pa.items.count(); o++)
					{
						PageItem *ite = pa.items.at(o);
						if ((ite->pattern() == patternName) && ((ite->GrType == 8) || (ite->itemType() == PageItem::Symbol)))
							ite->setPattern(newName);
						if (!ite->strokePattern().isEmpty())
						{
							if (ite->strokePattern() == patternName)
								ite->setStrokePattern(newName);
						}
						if (!ite->patternMask().isEmpty())
						{
							if (ite->patternMask() == patternName)
								ite->setPatternMask(newName);
						}
					}
				}
				QTreeWidgetItem *lg = updatePatternList(newName);
				if (lg != 0)
				{
					dataTree->expandItem(lg->parent());
					dataTree->setCurrentItem(lg, 0, QItemSelectionModel::ClearAndSelect);
				}
				itemSelected(dataTree->currentItem());
			}
			else
				return;
		}
	}
}

void PaintManagerDialog::duplicateColorItem()
{
	if (paletteLocked)
		return;
	QTreeWidgetItem* it = dataTree->currentItem();
	if (it)
	{
		if (it->parent() == gradientItems)
		{
			QString gradN = tr("Copy of %1").arg(it->text(0));
			dialogGradients.insert(gradN, dialogGradients[it->text(0)]);
			origNames.insert(gradN, gradN);
			QString patternName = origNames[gradN];
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
				updatePatternList();
				QTreeWidgetItem *lg = updateGradientList(dia->name());
				if (lg != 0)
				{
					dataTree->expandItem(lg->parent());
					dataTree->setCurrentItem(lg, 0, QItemSelectionModel::ClearAndSelect);
				}
				itemSelected(dataTree->currentItem());
				modified = true;
			}
			delete dia;
		}
		else if (it->parent() == colorItems)
		{
			QString nam = tr("Copy of %1").arg(it->text(0));
			m_colorList.insert(nam, m_colorList[it->text(0)]);
			ScColor tmpColor = m_colorList[nam];
			CMYKChoose* dia = new CMYKChoose(this, m_doc, tmpColor, nam, &m_colorList, false);
			if (dia->exec())
			{
				dia->Farbe.setSpotColor(dia->Separations->isChecked());
				dia->Farbe.setRegistrationColor(tmpColor.isRegistrationColor());
				m_colorList[dia->Farbname->text()] = dia->Farbe;
				if (nam != dia->Farbname->text())
				{
					replaceColorMap.insert(nam, dia->Farbname->text());
					m_colorList.remove(nam);
				}
				updateGradientColors(dia->Farbname->text(), nam);
				updateGradientList();
				updatePatternList();
				QTreeWidgetItem *lg = updateColorList(dia->Farbname->text());
				if (lg != 0)
				{
					dataTree->expandItem(lg->parent());
					dataTree->setCurrentItem(lg, 0, QItemSelectionModel::ClearAndSelect);
				}
				itemSelected(dataTree->currentItem());
				modified = true;
			}
			delete dia;
		}
	}
}

void PaintManagerDialog::removeColorItem()
{
	if (paletteLocked)
		return;
	QList<QTreeWidgetItem *> selItems = dataTree->selectedItems();
	if (selItems.count() > 1)
	{
		QStringList usedColors;
		QStringList colors;
		QStringList gradients;
		QStringList patterns;
		for (int a = 0; a < selItems.count(); a++)
		{
			QTreeWidgetItem* it = selItems[a];
			if (it == gradientItems)
				continue;
			if (it == colorItems)
				continue;
			if (it->text(0) == "Black")
				continue;
			if (it->text(0) == "White")
				continue;
			if (m_colorList[it->text(0)].isRegistrationColor())
				continue;
			if (it->parent() == gradientItems)
				gradients.append(it->text(0));
			if (it->parent() == colorItems)
				colors.append(it->text(0));
			if (it->parent() == patternItems)
				patterns.append(it->text(0));
		}
		for (int a = 0; a < gradients.count(); a++)
		{
			dialogGradients.remove(gradients[a]);
			replaceMap.insert(gradients[a], "");
		}
		bool hasUsed = false;
		ColorList UsedCG = getGradientColors();
		for (int a = 0; a < colors.count(); a++)
		{
			if (UsedCG.contains(colors[a]) || inDocUsedColors.contains(colors[a]))
			{
				hasUsed = true;
				usedColors.append(colors[a]);
			}
		}
		if (hasUsed)
		{
			ColorList dCols = m_colorList;
			QString dColor = tr("Selected Colors");
			for (int a = 0; a < usedColors.count(); a++)
			{
				dCols.remove(usedColors[a]);
			}
			dCols.insert(dColor , ScColor());
			DelColor *dia = new DelColor(this, dCols, dColor, true);
			if (dia->exec())
			{
				QString replacementColor(dia->getReplacementColor());
				for (int a = 0; a < colors.count(); a++)
				{
					dColor = colors[a];
					if (replacementColor == CommonStrings::tr_NoneColor)
						replacementColor = CommonStrings::None;
					if (replaceColorMap.values().contains(dColor))
					{
						QMap<QString,QString>::Iterator itt;
						for (itt = replaceColorMap.begin(); itt != replaceColorMap.end(); ++itt)
						{
							if (itt.value() == dColor)
								itt.value() = replacementColor;
						}
					}
					replaceColorMap.insert(dColor, replacementColor);
					m_colorList.remove(dColor);
					updateGradientColors(replacementColor, dColor);
				}
				modified = true;
			}
			delete dia;
		}
		else
		{
			for (int a = 0; a < colors.count(); a++)
			{
				replaceColorMap.insert(colors[a], "Black");
				m_colorList.remove(colors[a]);
			}
			modified = true;
		}
		updatePatternList();
		updateGradientList();
		updateColorList();
		itemSelected(0);
	}
	else
	{
		QTreeWidgetItem* it = dataTree->currentItem();
		if (it)
		{
			if ((it->parent() == gradientItems) || (it == gradientItems))
			{
				if (it == gradientItems)
				{
					int t = QMessageBox::warning(this, CommonStrings::trWarning, tr("Do you really want to clear all your gradients?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
					if (t == QMessageBox::No)
						return;
					replaceMap.clear();
					for (QMap<QString, VGradient>::Iterator it = dialogGradients.begin(); it != dialogGradients.end(); ++it)
					{
						replaceMap.insert(it.key(), "");
					}
					dialogGradients.clear();
				}
				else
				{
					dialogGradients.remove(it->text(0));
					replaceMap.insert(it->text(0), "");
				}
			}
			else if ((it->parent() == colorItems) || (it == colorItems))
			{
				if (it == colorItems)
				{
					int t = QMessageBox::warning(this, CommonStrings::trWarning, tr("Do you really want to clear all your colors and gradients?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
					if (t == QMessageBox::No)
						return;
					replaceMap.clear();
					for (QMap<QString, VGradient>::Iterator it = dialogGradients.begin(); it != dialogGradients.end(); ++it)
					{
						replaceMap.insert(it.key(), "");
					}
					replaceColorMap.clear();
					for (ColorList::Iterator it = m_colorList.begin(); it != m_colorList.end(); ++it)
					{
						replaceColorMap.insert(it.key(), "Black");
					}
					dialogPatterns.clear();
					dialogGradients.clear();
					m_colorList.clear();
					m_colorList.ensureDefaultColors();
				}
				else
				{
					QString dColor = it->text(0);
					ColorList UsedCG = getGradientColors();
					if (inDocUsedColors.contains(dColor) || UsedCG.contains(dColor))
					{
						DelColor *dia = new DelColor(this, m_colorList, dColor, true);
						if (dia->exec())
						{
							QString replacementColor(dia->getReplacementColor());
							if (replacementColor == CommonStrings::tr_NoneColor)
								replacementColor = CommonStrings::None;
							if (replaceColorMap.values().contains(dColor))
							{
								QMap<QString,QString>::Iterator itt;
								for (itt = replaceColorMap.begin(); itt != replaceColorMap.end(); ++itt)
								{
									if (itt.value() == dColor)
										itt.value() = replacementColor;
								}
							}
							replaceColorMap.insert(dColor, replacementColor);
							m_colorList.remove(dColor);
							updateGradientColors(replacementColor, dColor);
						}
						delete dia;
					}
					else
					{
						replaceColorMap.insert(dColor, "Black");
						updateGradientColors("Black", dColor);
						m_colorList.remove(dColor);
					}
				}
			}
			else if ((it->parent() == patternItems) || (it == patternItems))
			{
				if (it == patternItems)
				{
					int t = QMessageBox::warning(this, CommonStrings::trWarning, tr("Do you really want to clear all your patterns?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
					if (t == QMessageBox::No)
						return;
					replaceMapPatterns.clear();
					for (QMap<QString, ScPattern>::Iterator it = dialogPatterns.begin(); it != dialogPatterns.end(); ++it)
					{
						replaceMapPatterns.insert(it.key(), "");
					}
					dialogPatterns.clear();
				}
				else if (it->parent() == patternItems)
				{
					QStringList patterns2Del;
					QStringList mainPatterns = dialogPatterns.keys();
					for (int a = 0; a < mainPatterns.count(); a++)
					{
						if (mainPatterns[a] != it->text(0))
						{
							QStringList subPatterns;
							subPatterns = getUsedPatternsHelper(mainPatterns[a], subPatterns);
							if (subPatterns.contains(it->text(0)))
								patterns2Del.append(mainPatterns[a]);
						}
					}
					patterns2Del.append(it->text(0));
					for (int a = 0; a < patterns2Del.count(); a++)
					{
						dialogPatterns.remove(patterns2Del[a]);
					}
				}
			}
			updatePatternList();
			updateGradientList();
			updateColorList();
			itemSelected(0);
			modified = true;
		}
	}
}

QStringList PaintManagerDialog::getUsedPatternsHelper(QString pattern, QStringList &results)
{
	ScPattern *pat = &dialogPatterns[pattern];
	QStringList pats;
	pats.clear();
	for (int c = 0; c < pat->items.count(); ++c)
	{
		if ((pat->items.at(c)->GrType == 8) || (pat->items.at(c)->itemType() == PageItem::Symbol))
		{
			const QString& patName = pat->items.at(c)->pattern();
			if (!patName.isEmpty() && !results.contains(patName))
				pats.append(patName);
		}
		const QString& pat2 = pat->items.at(c)->strokePattern();
		if (!pat2.isEmpty() && !results.contains(pat2))
			pats.append(pat->items.at(c)->strokePattern());
		const QString& pat3 = pat->items.at(c)->patternMask();
		if (!pat3.isEmpty() && !results.contains(pat3))
			pats.append(pat->items.at(c)->patternMask());
	}
	if (!pats.isEmpty())
	{
		results = pats;
		for (int c = 0; c < pats.count(); ++c)
		{
			getUsedPatternsHelper(pats[c], results);
		}
	}
/*	QStringList pats;
	pats.clear();
	for (int c = 0; c < pat->items.count(); ++c)
	{
		if ((!results.contains(pat->items.at(c)->pattern())) && ((pat->items.at(c)->GrType == 8) || (pat->items.at(c)->itemType() == PageItem::Symbol)))
			pats.append(pat->items.at(c)->pattern());
	}
	if (!pats.isEmpty())
	{
		results = pats;
		for (int c = 0; c < pats.count(); ++c)
		{
			getUsedPatternsHelper(pats[c], results);
		}
	} */
	return results;
}

void PaintManagerDialog::removeUnusedColorItem()
{
	if (paletteLocked)
		return;
	QTreeWidgetItem* it = dataTree->currentItem();
	if (it)
	{
		if ((it->parent() == colorItems) || (it == colorItems))
		{
			ColorList UsedC;
			ColorList::Iterator it;
			ColorList UsedCG = getGradientColors();
			if (mainWin->HaveDoc)
			{
				m_colorList = inDocUsedColors;
				for (it = UsedCG.begin(); it != UsedCG.end(); ++it)
				{
					if (!m_colorList.contains(it.key()))
						m_colorList.insert(it.key(), it.value());
				}
			}
			else
				m_colorList = UsedCG;
			m_colorList.ensureDefaultColors();
			updatePatternList();
			updateGradientList();
			updateColorList();
			itemSelected(0);
			modified = true;
		}
	}
}

void PaintManagerDialog::importColorItems()
{
	QTreeWidgetItem* it = dataTree->currentItem();
	if (it)
	{
		if ((it->parent() == gradientItems) || (it == gradientItems))
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
				updateColorList();
				itemSelected(0);
				modified = true;
			}
		}
		else if ((it->parent() == colorItems) || (it == colorItems))
		{
			QString fileName;
			PrefsContext* dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
			QString wdir = dirs->get("colors", ".");
			QString docexts("*.sla *.sla.gz *.scd *.scd.gz");
			QString aiepsext(FormatsManager::instance()->extensionListForFormat(FormatsManager::EPS|FormatsManager::PS|FormatsManager::AI, 0));
			QString ooexts(" *.acb *.aco *.skp *.soc *.gpl *.xar *.xml");
			QString filter = tr("All Supported Formats (%1);;Documents (%2);;Other Files (%3);;All Files (*)").arg(docexts+" "+aiepsext+ooexts).arg(docexts).arg(aiepsext+ooexts);
			CustomFDialog dia(this, wdir, tr("Import Colors"), filter, fdHidePreviewCheckBox);
			if (dia.exec() == QDialog::Accepted)
				fileName = dia.selectedFile();
			else
				return;
			if (!fileName.isEmpty())
				dirs->set("colors", fileName.left(fileName.lastIndexOf("/")));
			if (!importColorsFromFile(fileName, m_colorList))
				QMessageBox::information(this, tr("Information"), "<qt>" + tr("The file %1 does not contain colors which can be imported.\nIf the file was a PostScript-based, try to import it with File -&gt; Import. \nNot all files have DSC conformant comments where the color descriptions are located.\n This prevents importing colors from some files.\nSee the Edit Colors section of the documentation for more details.").arg(fileName) + "</qt>", 1, 0, 0);
			else
			{
				updateGradientList();
				updateColorList();
				modified = true;
			}
			itemSelected(0);
		}
		else if ((it->parent() == patternItems) || (it == patternItems))
		{
			QString fileName;
			QStringList formats;
			QString allFormats = tr("All Supported Formats")+" (";
			int fmtCode = FORMATID_ODGIMPORT;
			const FileFormat *fmt = LoadSavePlugin::getFormatById(fmtCode);
			while (fmt != 0)
			{
				if (fmt->load)
				{
					formats.append(fmt->filter);
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
			formats.append("Scribus Objects (*.sce *.SCE)");
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
					formats.append(form2 + " (*."+form1+" *."+form2+")");
					allFormats += "*."+form1+" *."+form2+" ";
					imgFormats.append(form1);
				}
				else if ((form1 == "jpg") || (form1 == "jpeg"))
				{
					// JPEG is a special case because both .jpg and .jpeg
					// are acceptable extensions.
					if (!jpgFound)
					{
						formats.append("JPEG (*.jpg *.jpeg *.JPG *.JPEG)");
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
						formats.append("TIFF (*.tif *.tiff *.TIF *.TIFF)");
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
				formats.append("TIFF (*.tif *.tiff *.TIF *.TIFF)");
				allFormats += "*.tif *.tiff *.TIF *.TIFF ";
			}
			if (!jpgFound)
			{
				formats.append("JPEG (*.jpg *.jpeg *.JPG *.JPEG)");
				allFormats += "*.jpg *.jpeg *.JPG *.JPEG ";
			}
			formats.append("PSD (*.psd *.PSD)");
			formats.append("Gimp Patterns (*.pat *.PAT)");
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
			qSort(formats);
			allFormats += formats.join(";;");
			PrefsContext* dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
			QString wdir = dirs->get("patterns", ".");
			CustomFDialog dia(this, wdir, tr("Open"), allFormats, fdHidePreviewCheckBox | fdExistingFiles);
			if (dia.exec() == QDialog::Accepted)
				fileName = dia.selectedFile();
			else
				return;
			if (!fileName.isEmpty())
			{
				qApp->changeOverrideCursor(QCursor(Qt::WaitCursor));
				PrefsManager::instance()->prefsFile->getContext("dirs")->set("patterns", fileName.left(fileName.lastIndexOf("/")));
				QFileInfo fi(fileName);
				if ((fi.suffix().toLower() == "sce") || (!imgFormats.contains(fi.suffix().toLower())))
				{
					loadVectors(fileName);
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
						origNamesPatterns.insert(patNam, patNam);
					}
				}
				updateColorList();
				updateGradientList();
				updatePatternList();
				itemSelected(0);
				qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
			}
		}
	}
}

void PaintManagerDialog::loadPatternDir()
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
				QFileInfo fi(QDir::cleanPath(QDir::toNativeSeparators(fileName + "/" + d[dc])));
				QString ext = fi.suffix().toLower();
				if ((ext == "sce") || (!formats.contains(ext)))
					loadVectors(QDir::cleanPath(QDir::toNativeSeparators(fileName + "/" + d[dc])));
			}
			for (uint dc = 0; dc < d.count(); ++dc)
			{
				mainWin->mainWindowProgressBar->setValue(d.count() + dc);
				qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
				QFileInfo fi(QDir::cleanPath(QDir::toNativeSeparators(fileName + "/" + d[dc])));
				QString ext = fi.suffix().toLower();
				if ((ext == "sce") || (!formats.contains(ext)))
					continue;
				else if (formats.contains(ext))
				{
					QString patNam = fi.baseName().trimmed().simplified().replace(" ", "_");
					if (!dialogPatterns.contains(patNam))
					{
						ScPattern pat = ScPattern();
						pat.setDoc(m_doc);
						pat.setPattern(QDir::cleanPath(QDir::toNativeSeparators(fileName + "/" + d[dc])));
						dialogPatterns.insert(patNam, pat);
						origNamesPatterns.insert(patNam, patNam);
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
	}
}

void PaintManagerDialog::loadVectors(QString data)
{
	m_doc->PageColors = m_colorList;
	m_doc->docGradients = dialogGradients;
	bool wasUndo = UndoManager::undoEnabled();
	UndoManager::instance()->setUndoEnabled(false);
	m_doc->setLoading(true);
	QFileInfo fi(data);
	QString patNam = fi.baseName().trimmed().simplified().replace(" ", "_");
	uint ac = m_doc->Items->count();
	uint ap = m_doc->docPatterns.count();
	bool savedAlignGrid = m_doc->useRaster;
	bool savedAlignGuides = m_doc->SnapGuides;
	m_doc->useRaster = false;
	m_doc->SnapGuides = false;
	if (fi.suffix().toLower() == "sce")
	{
		ScriXmlDoc ss;
		ss.ReadElem(data, PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts, m_doc, m_doc->currentPage()->xOffset(), m_doc->currentPage()->yOffset(), true, true, PrefsManager::instance()->appPrefs.fontPrefs.GFontSub);
	}
	else
	{
		FileLoader *fileLoader = new FileLoader(data);
		int testResult = fileLoader->TestFile();
		delete fileLoader;
		if ((testResult != -1) && (testResult >= FORMATID_ODGIMPORT))
		{
			const FileFormat * fmt = LoadSavePlugin::getFormatById(testResult);
			if( fmt )
			{
				fmt->setupTargets(m_doc, 0, mainWin, 0, &(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts));
				fmt->loadFile(data, LoadSavePlugin::lfUseCurrentPage|LoadSavePlugin::lfInteractive|LoadSavePlugin::lfScripted|LoadSavePlugin::lfKeepPatterns|LoadSavePlugin::lfLoadAsPattern);
			}
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
		double minx =  std::numeric_limits<double>::max();
		double miny =  std::numeric_limits<double>::max();
		double maxx = -std::numeric_limits<double>::max();
		double maxy = -std::numeric_limits<double>::max();
		double x1, x2, y1, y2;
		currItem->getVisualBoundingRect(&x1, &y1, &x2, &y2);
		minx = qMin(minx, x1);
		miny = qMin(miny, y1);
		maxx = qMax(maxx, x2);
		maxy = qMax(maxy, y2);
		pat.pattern = currItem->DrawObj_toImage(qMax(maxx - minx, maxy - miny));
		pat.width = currItem->gWidth;
		pat.height = currItem->gHeight;
		for (uint as = ac; as < ae; ++as)
		{
			pat.items.append(m_doc->Items->takeAt(ac));
		}
		if (!dialogPatterns.contains(patNam))
		{
			dialogPatterns.insert(patNam, pat);
			origNamesPatterns.insert(patNam, patNam);
		}
		for (QMap<QString, ScPattern>::Iterator it = m_doc->docPatterns.begin(); it != m_doc->docPatterns.end(); ++it)
		{
			if (!origPatterns.contains(it.key()))
			{
				dialogPatterns.insert(it.key(), it.value());
				origNamesPatterns.insert(it.key(), it.key());
			}
		}
	}
	else
	{
		uint ape = m_doc->docPatterns.count();
		if (ap != ape)
		{
			for (QMap<QString, ScPattern>::Iterator it = m_doc->docPatterns.begin(); it != m_doc->docPatterns.end(); ++it)
			{
				if (!origPatterns.contains(it.key()))
				{
					dialogPatterns.insert(it.key(), it.value());
					origNamesPatterns.insert(it.key(), it.key());
				}
			}
		}
	}
	m_doc->setLoading(false);
	m_colorList = m_doc->PageColors;
	dialogGradients = m_doc->docGradients;
	UndoManager::instance()->setUndoEnabled(wasUndo);
}

ColorList PaintManagerDialog::getGradientColors()
{
	ColorList colorList;
	QMap<QString,VGradient>::Iterator itg;
	for (itg = dialogGradients.begin(); itg != dialogGradients.end(); ++itg)
	{
		QList<VColorStop*> cstops = itg.value().colorStops();
		for (uint cst = 0; cst < itg.value().Stops(); ++cst)
		{
			if ((!colorList.contains(cstops.at(cst)->name)) && (cstops.at(cst)->name != CommonStrings::None))
				colorList.insert(cstops.at(cst)->name, m_colorList[cstops.at(cst)->name]);
		}
	}
	QStringList patterns = dialogPatterns.keys();
	for (int c = 0; c < dialogPatterns.count(); ++c)
	{
		ScPattern pa = dialogPatterns[patterns[c]];
		for (int o = 0; o < pa.items.count(); o++)
		{
			PageItem *ite = pa.items.at(o);
			if ((!colorList.contains(ite->lineColor())) && (ite->lineColor() != CommonStrings::None))
				colorList.insert(ite->lineColor(), m_colorList[ite->lineColor()]);
			if ((!colorList.contains(ite->fillColor())) && (ite->fillColor() != CommonStrings::None))
				colorList.insert(ite->fillColor(), m_colorList[ite->fillColor()]);
			QString gCol1, gCol2, gCol3, gCol4;
			ite->get4ColorColors(gCol1, gCol2, gCol3, gCol4);
			if ((!colorList.contains(gCol1)) && (gCol1 != CommonStrings::None))
				colorList.insert(gCol1, m_colorList[gCol1]);
			if ((!colorList.contains(gCol2)) && (gCol2 != CommonStrings::None))
				colorList.insert(gCol2, m_colorList[gCol2]);
			if ((!colorList.contains(gCol3)) && (gCol3 != CommonStrings::None))
				colorList.insert(gCol3, m_colorList[gCol3]);
			if ((!colorList.contains(gCol4)) && (gCol4 != CommonStrings::None))
				colorList.insert(gCol4, m_colorList[gCol4]);
			for (int grow = 0; grow < ite->meshGradientArray.count(); grow++)
			{
				for (int gcol = 0; gcol < ite->meshGradientArray[grow].count(); gcol++)
				{
					meshPoint mp = ite->meshGradientArray[grow][gcol];
					if ((!colorList.contains(mp.colorName)) && (mp.colorName != CommonStrings::None))
						colorList.insert(mp.colorName, m_colorList[mp.colorName]);
				}
			}
			QList<VColorStop*> cstops = ite->fill_gradient.colorStops();
			for (uint cst = 0; cst < ite->fill_gradient.Stops(); ++cst)
			{
				if ((!colorList.contains(cstops.at(cst)->name)) && (cstops.at(cst)->name != CommonStrings::None))
					colorList.insert(cstops.at(cst)->name, m_colorList[cstops.at(cst)->name]);
			}
			cstops = ite->stroke_gradient.colorStops();
			for (uint cst = 0; cst < ite->stroke_gradient.Stops(); ++cst)
			{
				if ((!colorList.contains(cstops.at(cst)->name)) && (cstops.at(cst)->name != CommonStrings::None))
					colorList.insert(cstops.at(cst)->name, m_colorList[cstops.at(cst)->name]);
			}
			cstops = ite->mask_gradient.colorStops();
			for (uint cst = 0; cst < ite->mask_gradient.Stops(); ++cst)
			{
				if ((!colorList.contains(cstops.at(cst)->name)) && (cstops.at(cst)->name != CommonStrings::None))
					colorList.insert(cstops.at(cst)->name, m_colorList[cstops.at(cst)->name]);
			}
		}
	}
	colorList.remove(CommonStrings::None);
	return colorList;
}

void PaintManagerDialog::updateGradientColors(QString newName, QString oldName)
{
	QMap<QString,VGradient>::Iterator itg;
	for (itg = dialogGradients.begin(); itg != dialogGradients.end(); ++itg)
	{
		QList<VColorStop*> cstops = itg.value().colorStops();
		for (uint cst = 0; cst < itg.value().Stops(); ++cst)
		{
			if (oldName == cstops.at(cst)->name)
			{
				cstops.at(cst)->name = newName;
				const ScColor& col = m_colorList[cstops.at(cst)->name];
				cstops.at(cst)->color = ScColorEngine::getShadeColorProof(col, m_doc, cstops.at(cst)->shade);
			}
		}
	}
	ColorList colorListBack = m_doc->PageColors;
	m_doc->PageColors = m_colorList;
	QStringList patterns = dialogPatterns.keys();
	for (int c = 0; c < dialogPatterns.count(); ++c)
	{
		ScPattern pa = dialogPatterns[patterns[c]];
		for (int o = 0; o < pa.items.count(); o++)
		{
			PageItem *ite = pa.items.at(o);
			if (oldName == ite->lineColor())
				ite->setLineColor(newName);
			if (oldName == ite->fillColor())
				ite->setFillColor(newName);
			QString gCol1, gCol2, gCol3, gCol4;
			ite->get4ColorColors(gCol1, gCol2, gCol3, gCol4);
			if (oldName == gCol1)
				gCol1 = newName;
			if (oldName == gCol2)
				gCol2 = newName;
			if (oldName == gCol3)
				gCol3 = newName;
			if (oldName == gCol4)
				gCol4 = newName;
			ite->set4ColorColors(gCol1, gCol2, gCol3, gCol4);
			for (int grow = 0; grow < ite->meshGradientArray.count(); grow++)
			{
				for (int gcol = 0; gcol < ite->meshGradientArray[grow].count(); gcol++)
				{
					meshPoint mp = ite->meshGradientArray[grow][gcol];
					if (mp.colorName == oldName)
					{
						mp.colorName = newName;
						ite->setMeshPointColor(grow, gcol, mp.colorName, mp.shade, mp.transparency);
					}
				}
			}
			QList<VColorStop*> cstops = ite->fill_gradient.colorStops();
			for (uint cst = 0; cst < ite->fill_gradient.Stops(); ++cst)
			{
				if (oldName == cstops.at(cst)->name)
				{
					cstops.at(cst)->name = newName;
					ite->SetQColor(&cstops.at(cst)->color, cstops.at(cst)->name, cstops.at(cst)->shade);
				}
			}
			cstops = ite->stroke_gradient.colorStops();
			for (uint cst = 0; cst < ite->stroke_gradient.Stops(); ++cst)
			{
				if (oldName == cstops.at(cst)->name)
				{
					cstops.at(cst)->name = newName;
					ite->SetQColor(&cstops.at(cst)->color, cstops.at(cst)->name, cstops.at(cst)->shade);
				}
			}
			cstops = ite->mask_gradient.colorStops();
			for (uint cst = 0; cst < ite->mask_gradient.Stops(); ++cst)
			{
				if (oldName == cstops.at(cst)->name)
				{
					cstops.at(cst)->name = newName;
					ite->SetQColor(&cstops.at(cst)->color, cstops.at(cst)->name, cstops.at(cst)->shade);
				}
			}
		}
		PageItem *ite = pa.items.at(0);
		dialogPatterns[patterns[c]].pattern = ite->DrawObj_toImage(pa.items, 1.0);
	}
	m_doc->PageColors = colorListBack;
}

void PaintManagerDialog::loadGimpFormat(QString fileName)
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
		double left, middle, right, r0, g0, b0, a0, r1, g1, b1, a1;
		double oldr1 = 0.0;
		double oldg1 = 0.0;
		double oldb1 = 0.0;
		double olda1 = 0.0;
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

void PaintManagerDialog::addGimpColor(QString &colorName, double r, double g, double b)
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

void PaintManagerDialog::loadScribusFormat(QString fileName)
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

void PaintManagerDialog::loadDefaults(QTreeWidgetItem* item)
{
	QString txt = item->data(0, Qt::UserRole).toString() + "/" + item->text(0);
	if (!mainWin->HaveDoc)
	{
		m_colorList.clear();
		dialogGradients.clear();
		dialogPatterns.clear();
	}
	QString pfadC2 = "";
	if (item->text(0) == "Scribus Small")
	{
		m_colorList.insert("White", ScColor(0, 0, 0, 0));
		m_colorList.insert("Black", ScColor(0, 0, 0, 255));
		ScColor cc = ScColor(255, 255, 255, 255);
		cc.setRegistrationColor(true);
		m_colorList.insert("Registration", cc);
		m_colorList.insert("Blue", ScColor(255, 255, 0, 0));
		m_colorList.insert("Cyan", ScColor(255, 0, 0, 0));
		m_colorList.insert("Green", ScColor(255, 0, 255, 0));
		m_colorList.insert("Red", ScColor(0, 255, 255, 0));
		m_colorList.insert("Yellow", ScColor(0, 0, 255, 0));
		m_colorList.insert("Magenta", ScColor(0, 255, 0, 0));
		paletteLocked = false;
	}
	else
	{
		if (!customColSet.contains(txt))
			pfadC2 = csm.paletteFileFromName(txt);
		else
			pfadC2 = csm.userPaletteFileFromName(txt);
		if (!mainWin->HaveDoc)
		{
			QFileInfo fi(pfadC2);
			if (fi.absolutePath().contains(ScPaths::getApplicationDataDir()+"swatches/locked"))
				paletteLocked = true;
			else
				paletteLocked = !fi.isWritable();
		}
		else
			paletteLocked = false;
		if (!csm.loadPalette(pfadC2, m_doc, m_colorList, dialogGradients, dialogPatterns, mainWin->HaveDoc))
		{
			m_colorList.insert("White", ScColor(0, 0, 0, 0));
			m_colorList.insert("Black", ScColor(0, 0, 0, 255));
			ScColor cc = ScColor(255, 255, 255, 255);
			cc.setRegistrationColor(true);
			m_colorList.insert("Registration", cc);
			m_colorList.insert("Blue", ScColor(255, 255, 0, 0));
			m_colorList.insert("Cyan", ScColor(255, 0, 0, 0));
			m_colorList.insert("Green", ScColor(255, 0, 255, 0));
			m_colorList.insert("Red", ScColor(0, 255, 255, 0));
			m_colorList.insert("Yellow", ScColor(0, 0, 255, 0));
			m_colorList.insert("Magenta", ScColor(0, 255, 0, 0));
		}
	}
	updateColorList();
	updateGradientList();
	updatePatternList();
	itemSelected(0);
}

void PaintManagerDialog::saveDefaults()
{
	QTreeWidgetItem* item = LoadColSet->currentItem();
	QString NameK = item->data(0, Qt::UserRole).toString() + "/" + item->text(0);
	QString Name = LoadColSet->text();
	Query* dia = new Query(this, "Name", 1, 0, tr("&Name:"), tr("Choose a Name"));
	if ((customColSet.contains(NameK)) && (!paletteLocked))
		dia->setEditText(Name, false);
	else
		dia->setEditText("", false);
	if (dia->exec())
		doSaveDefaults(dia->getEditText(), (dia->getEditText() != Name));
	delete dia;
}

void PaintManagerDialog::doSaveDefaults(QString name, bool changed)
{
	QString Cpfad = QDir::toNativeSeparators(ScPaths::getApplicationDataDir());
	QString Fname = name;
	Fname.replace(" ", "_");
	Fname  = Cpfad + "swatches/"+ Fname;
	Fname += ".xml";	
	const FileFormat *fmt = LoadSavePlugin::getFormatById(FORMATID_SLA150EXPORT);
	if (fmt)
	{
		ScribusDoc *s_doc = new ScribusDoc();
		s_doc->setup(0, 1, 1, 1, 1, "Custom", "Custom");
		s_doc->setPage(100, 100, 0, 0, 0, 0, 0, 0, false, false);
		s_doc->addPage(0);
		s_doc->setGUI(false, mainWin, 0);
		s_doc->PageColors = m_colorList;
		s_doc->setGradients(dialogGradients);
		s_doc->setPatterns(dialogPatterns);
		fmt->setupTargets(s_doc, 0, mainWin, 0, &(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts));
		fmt->savePalette(Fname);
		delete s_doc;
	}
	if (changed)
	{
		QString nameC = name;
		nameC.replace(" ", "_");
		nameC += ".xml";
		QFileInfo fi(Fname);
		customColSet.append(fi.absolutePath() + "/" + nameC);
		QTreeWidgetItem *item = LoadColSet->addSubItem(name, userSwatches);
		item->setData(0, Qt::UserRole, fi.absolutePath());
		disconnect(LoadColSet, SIGNAL(activated(QTreeWidgetItem*)), this, SLOT(loadDefaults(QTreeWidgetItem*)));
		LoadColSet->setCurrentComboItem(fi.absolutePath() + "/" + name);
		connect(LoadColSet, SIGNAL(activated(QTreeWidgetItem*)), this, SLOT(loadDefaults(QTreeWidgetItem*)));
	}
}

QString PaintManagerDialog::getColorSetName()
{
	QString NameK;
	QTreeWidgetItem* item = LoadColSet->currentItem();
	if (item->text(0) == "Scribus Small")
		NameK = "Scribus Small";
	else
		NameK = item->data(0, Qt::UserRole).toString() + "/" + item->text(0);
	return NameK;
}

ScColor PaintManagerDialog::selectedColor()
{
	QTreeWidgetItem* it = dataTree->currentItem();
	if (it)
	{
		if (it->parent() == colorItems)
			return m_colorList[it->text(0)];
	}
	return ScColor();
}

QString PaintManagerDialog::selectedColorName()
{
	QTreeWidgetItem* it = dataTree->currentItem();
	if (it)
	{
		if (it->parent() == colorItems)
			return it->text(0);
	}
	return CommonStrings::None;
}
