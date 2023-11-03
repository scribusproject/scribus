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
*   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.             *
***************************************************************************/

#include <memory>

#include <QCheckBox>
#include <QCursor>
#include <QDir>
#include <QDomDocument>
#include <QEventLoop>
#include <QFileDialog>
#include <QFontMetrics>
#include <QImageReader>
#include <QMenu>
#include <QMessageBox>
#include <QScopedPointer>
#include <QScreen>

#include "cmykfw.h"
#include "colorlistbox.h"
#include "colorsandfills.h"
#include "commonstrings.h"
#include "dcolor.h"
#include "fileloader.h"
#include "gradientaddedit.h"
#include "iconmanager.h"
#include "loadsaveplugin.h"
#include "plugins/formatidlist.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "query.h"
#include "scclocale.h"
#include "sccolor.h"
#include "sccolorengine.h"
#include "scconfig.h"
#include "sclimits.h"
#include "sclistboxpixmap.h"
#include "scpainter.h"
#include "scpaths.h"
#include "scribus.h"
#include "scribusview.h"
#include "scribusXml.h"
#include "sctextstream.h"
#include "ui/customfdialog.h"
#include "undomanager.h"
#include "util.h"
#include "util_color.h"
#include "util_formats.h"

ColorsAndFillsDialog::ColorsAndFillsDialog(QWidget* parent, QHash<QString, VGradient> *gradients, const ColorList& colorlist, const QString& docColSet, QHash<QString, ScPattern> *patterns, ScribusDoc *doc, ScribusMainWindow *scMW)
	: ScDialog(parent, "ColorsAndFillsDialog"),
	m_doc(doc),
	mainWin(scMW),
	m_colorList(colorlist)
{
	setupUi(this);
	setModal(true);

	setWindowIcon(IconManager::instance().loadIcon("AppIcon.png"));
	dataTree->setContextMenuPolicy(Qt::CustomContextMenu);
	dataTree->setIconSize(QSize(60, 48));
	colorItems = new QTreeWidgetItem(dataTree);
	colorItems->setText(0, tr("Solid Colors"));
	gradientItems = new QTreeWidgetItem(dataTree);
	gradientItems->setText(0, tr("Gradients"));
	for (auto it = gradients->cbegin(); it != gradients->cend(); ++it)
	{
		dialogGradients.insert(it.key(), it.value());
		origNames.insert(it.key(), it.key());
	}
	origGradients = gradients->keys();

	patternItems = new QTreeWidgetItem(dataTree);
	patternItems->setText(0, tr("Patterns"));
	for (auto it = patterns->cbegin(); it != patterns->cend(); ++it)
	{
		dialogPatterns.insert(it.key(), it.value());
		origNamesPatterns.insert(it.key(), it.key());
	}
	origPatterns = patterns->keys();

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
			QString pfad;
			if (customColSet.contains(docColSet))
				pfad = csm.userPaletteFileFromName(docColSet);
			else
				pfad = csm.paletteFileFromName(docColSet);
			QFileInfo fi(pfad);
			if (fi.absolutePath().contains(ScPaths::applicationDataDir()+"swatches/locked"))
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
	dataTree->setCurrentItem(dataTree->topLevelItem(0));
	itemSelected(dataTree->currentItem());

	QFontMetrics fontMetrics(dataTree->fontMetrics());

	int maxAdvance = 0;
	for (QString colorName : m_colorList.keys())
	{
		QRect itemRect = dataTree->style()->itemTextRect(fontMetrics, QRect(), Qt::AlignLeft, true, colorName);
		maxAdvance = std::max(maxAdvance, itemRect.width());
	}
	maxAdvance = std::max(maxAdvance, 20 * fontMetrics.averageCharWidth());

	QSize treeMinSize = dataTree->minimumSize();
	int maxTreeWidth = dataTree->screen()->size().width() * 0.5;
	int minTreeWidth = maxAdvance + 75;
	minTreeWidth += fontMetrics.horizontalAdvance(colorItems->child(0)->text(0)) / 2.0;
	minTreeWidth += dataTree->verticalScrollBar()->height();
	minTreeWidth = std::max(treeMinSize.width(), std::min(minTreeWidth, maxTreeWidth));
	treeMinSize.setWidth(minTreeWidth);
	dataTree->setMinimumSize(treeMinSize);

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
	connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	connect(SaveColSet, SIGNAL(clicked()), this, SLOT(saveDefaults()));
	connect(LoadColSet, SIGNAL(activated(QTreeWidgetItem*)), this, SLOT(loadDefaults(QTreeWidgetItem*)));
}

QTreeWidgetItem* ColorsAndFillsDialog::updatePatternList(const QString& addedName)
{
	QList<QTreeWidgetItem*> lg = patternItems->takeChildren();
	for (int i = 0; i < lg.count(); ++i)
		delete lg[i];

	QTreeWidgetItem* ret = nullptr;
	QStringList patK = dialogPatterns.keys();
	patK.sort();
	for (int i = 0; i < patK.count(); ++i)
	{
		ScPattern sp = dialogPatterns.value(patK[i]);
		QPixmap pm;
		if (sp.getPattern()->width() >= sp.getPattern()->height())
			pm = QPixmap::fromImage(sp.getPattern()->scaledToWidth(48, Qt::SmoothTransformation));
		else
			pm = QPixmap::fromImage(sp.getPattern()->scaledToHeight(48, Qt::SmoothTransformation));
		QPixmap pm2(48, 48);
		pm2.fill(palette().color(QPalette::Base));
		QPainter p;
		p.begin(&pm2);
		p.drawPixmap(24 - pm.width() / 2, 24 - pm.height() / 2, pm);
		p.end();
		QTreeWidgetItem *item = new QTreeWidgetItem(patternItems);
		item->setText(0, patK[i]);
		if (patK[i] == addedName)
			ret = item;
		item->setIcon(0, pm2);
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	}
	return ret;
}

QTreeWidgetItem* ColorsAndFillsDialog::updateGradientList(const QString& addedName)
{
	QList<QTreeWidgetItem*> lg = gradientItems->takeChildren();
	for (int i = 0; i < lg.count(); ++i)
		delete lg[i];

	QTreeWidgetItem* ret = nullptr;
	QStringList patK = dialogGradients.keys();
	patK.sort();
	for (int i = 0; i < patK.count(); ++i)
	{
		VGradient gr = dialogGradients.value(patK[i]);
		QImage pixm(48, 12, QImage::Format_ARGB32);
		QPainter pb;
		QBrush b(QColor(205,205,205), IconManager::instance().loadPixmap("testfill.png"));
		pb.begin(&pixm);
		pb.fillRect(0, 0, 48, 12, b);
		pb.end();

		ScPainter *p = new ScPainter(&pixm, 48, 12);
		p->setPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		p->setFillMode(ScPainter::Gradient);
		p->fill_gradient = gr;
		p->setGradient(VGradient::linear, FPoint(0,6), FPoint(48, 6), FPoint(0, 0), 1.0, 0.0);
		p->drawRect(0, 0, 48, 12);
		p->end();
		delete p;

		QPixmap pm = QPixmap::fromImage(pixm);
		QTreeWidgetItem *item = new QTreeWidgetItem(gradientItems);
		item->setText(0, patK[i]);
		if (patK[i] == addedName)
			ret = item;
		item->setIcon(0, pm);
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	}
	return ret;
}

QTreeWidgetItem* ColorsAndFillsDialog::updateColorList(const QString& addedName)
{
	QList<QTreeWidgetItem*> lg = colorItems->takeChildren();
	for (int i = 0; i < lg.count(); ++i)
		delete lg[i];

	QTreeWidgetItem* ret = nullptr;
	if (sortRule > 0)
	{
		QMap<QString, QString> sortMap;
		for (auto it = m_colorList.cbegin(); it != m_colorList.cend(); ++it)
		{
			if (sortRule == 1)
			{
				QColor c = it.value().getRawRGBColor();
				QString sortString = QString("%1-%2-%3-%4").arg(c.hue(), 3, 10, QChar('0')).arg(c.saturation(), 3, 10, QChar('0')).arg(c.value(), 3, 10, QChar('0')).arg(it.key());
				sortMap.insert(sortString, it.key());
			}
			else if (sortRule == 2)
			{
				QString sortString("%1-%2");
				if (it.value().isRegistrationColor())
					sortMap.insert(sortString.arg("A", it.key()), it.key());
				else if (it.value().isSpotColor())
					sortMap.insert(sortString.arg("B", it.key()), it.key());
				else if (it.value().getColorModel() == colorModelCMYK)
					sortMap.insert(sortString.arg("C", it.key()), it.key());
				else
					sortMap.insert(sortString.arg("D", it.key()), it.key());
			}
		}
		for (auto itc = sortMap.cbegin(); itc != sortMap.cend(); ++itc)
		{
			const ScColor& color = m_colorList[itc.value()];
			QTreeWidgetItem *item = new QTreeWidgetItem(colorItems);
			item->setText(0, itc.value());
			if (itc.value() == addedName)
				ret = item;
			QPixmap* pPixmap = getFancyPixmap(color, m_doc);
			item->setIcon(0, *pPixmap);
			item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
			item->setData(0, Qt::ToolTipRole, getColorTooltip(color));
		}
	}
	else
	{
		for (auto it = m_colorList.cbegin(); it != m_colorList.cend(); ++it)
		{
			const ScColor& color = it.value();
			QTreeWidgetItem *item = new QTreeWidgetItem(colorItems);
			item->setText(0, it.key());
			if (it.key() == addedName)
				ret = item;
			QPixmap* pPixmap = getFancyPixmap(color, m_doc);
			item->setIcon(0, *pPixmap);
			item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
			item->setData(0, Qt::ToolTipRole, getColorTooltip(color));
		}
	}
	return ret;
}

void ColorsAndFillsDialog::slotRightClick(QPoint p)
{
	const QTreeWidgetItem* it = dataTree->itemAt(p);
	if (!it)
		return;

	if ((it->parent() == colorItems) || (it == colorItems))
	{
		QMenu *pmen = new QMenu();
//			qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
		pmen->addAction( tr("Sort by Name"));
		pmen->addAction( tr("Sort by Color"));
		pmen->addAction( tr("Sort by Type"));
		sortRule = pmen->actions().indexOf(pmen->exec(QCursor::pos()));
		pmen->deleteLater();
		updateColorList();
	}
}

void ColorsAndFillsDialog::selEditColor(QTreeWidgetItem *it)
{
	if (!it || paletteLocked)
		return;

	if ((it->parent() == colorItems) || (it->parent() == gradientItems))
	{
		QString curCol = it->text(0);
		bool enableDel  = (!isMandatoryColor(curCol)) && (m_colorList.count() > 1);
		bool enableEdit = (!isMandatoryColor(curCol));
		duplicateButton->setEnabled(curCol != "Registration");
		deleteButton->setEnabled(enableDel);
		editButton->setEnabled(enableEdit);
		if (enableEdit)
			editColorItem();
	}
}

void ColorsAndFillsDialog::itemSelectionChanged()
{
	QList<QTreeWidgetItem *> selItems = dataTree->selectedItems();
	if (selItems.count() > 1)
		deleteButton->setEnabled(true);
}

void ColorsAndFillsDialog::itemSelected(QTreeWidgetItem* it)
{
	QList<QTreeWidgetItem *> selItems = dataTree->selectedItems();
	importButton->setText( tr("&Import"));
	newButton->setText( tr("Add"));
	editButton->setText( tr("&Edit"));
	if (it && !paletteLocked)
	{
		if ((it->parent() == colorItems) || (it->parent() == gradientItems))
		{
			importButton->setEnabled(false);
			newButton->setEnabled(true);
			deleteUnusedButton->setEnabled(it->parent() == colorItems);
			if (it->parent() == colorItems)
			{
				QString curCol = it->text(0);
				bool enableDel  = (!isMandatoryColor(curCol)) && (m_colorList.count() > 1);
				bool enableEdit = (!isMandatoryColor(curCol));
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

void ColorsAndFillsDialog::createNew()
{
	if (paletteLocked)
		return;
	QTreeWidgetItem* it = dataTree->currentItem();
	if (!it)
		return;

	if ((it->parent() == gradientItems) || (it == gradientItems))
	{
		VGradient fill_gradient(VGradient::linear);
		fill_gradient.clearStops();
		fill_gradient.addStop(QColor(Qt::black), 0.0, 0.5, 1.0, "Black", 100);
		fill_gradient.addStop(QColor(Qt::white), 1.0, 0.5, 1.0, "White", 100);
		QScopedPointer<gradientEditDialog> dia(new gradientEditDialog(this, tr("New Gradient"), fill_gradient, m_colorList, m_doc, &dialogGradients, true));
		if (!dia->exec())
			return;

		dialogGradients.insert(dia->name(), dia->gradient());
		QTreeWidgetItem *lg = updateGradientList(dia->name());
		if (lg != nullptr)
		{
			dataTree->expandItem(lg->parent());
			dataTree->setCurrentItem(lg, 0, QItemSelectionModel::ClearAndSelect);
		}
		itemSelected(dataTree->currentItem());
		modified = true;
	}
	else if ((it->parent() == colorItems) || (it == colorItems))
	{
		ScColor tmpColor(0, 0, 0, 0);
		QScopedPointer<CMYKChoose> dia(new CMYKChoose(this, m_doc, tmpColor, tr("New Color"), &m_colorList, true));
		if (!dia->exec())
			return;

		dia->Farbe.setSpotColor(dia->isSpotColor());
		m_colorList.insert(dia->colorName(), dia->Farbe);
		QTreeWidgetItem *lg = updateColorList(dia->colorName());
		if (lg != nullptr)
		{
			dataTree->expandItem(lg->parent());
			dataTree->setCurrentItem(lg, 0, QItemSelectionModel::ClearAndSelect);
		}
		itemSelected(dataTree->currentItem());
		modified = true;
	}
	else if ((it->parent() == patternItems) || (it == patternItems))
	{
		loadPatternDir();
		updateColorList();
		updateGradientList();
		updatePatternList();
		itemSelected(nullptr);
	}
}

void ColorsAndFillsDialog::editColorItem()
{
	if (paletteLocked)
		return;
	const QTreeWidgetItem* it = dataTree->currentItem();
	if (!it)
		return;

	if (it->parent() == gradientItems)
	{
		QString gradN = it->text(0);
		QString patternName = origNames[it->text(0)];
		QScopedPointer<gradientEditDialog> dia(new gradientEditDialog(this, gradN, dialogGradients[gradN], m_colorList, m_doc, &dialogGradients, false));
		if (!dia->exec())
			return;

		QString newName = dia->name();
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
		for (int i = 0; i < dialogPatterns.count(); ++i)
		{
			ScPattern pa = dialogPatterns[patterns[i]];
			for (int j = 0; j < pa.items.count(); ++j)
			{
				PageItem *ite = pa.items.at(j);
				if (ite->gradient() == gradN)
					ite->setGradient(newName);
				if (ite->strokeGradient() == gradN)
					ite->setStrokeGradient(newName);
				if (ite->gradientMask() == gradN)
					ite->setGradientMask(newName);
			}
			PageItem *ite = pa.items.at(0);
			dialogPatterns[patterns[i]].pattern = ite->DrawObj_toImage(pa.items, 1.0);
		}
		QTreeWidgetItem *lg = updateGradientList(dia->name());
		if (lg != nullptr)
		{
			dataTree->expandItem(lg->parent());
			dataTree->setCurrentItem(lg, 0, QItemSelectionModel::ClearAndSelect);
		}
		itemSelected(dataTree->currentItem());
		modified = true;
	}
	else if (it->parent() == colorItems)
	{
		ScColor tmpColor = m_colorList[it->text(0)];
		QScopedPointer<CMYKChoose> dia(new CMYKChoose(this, m_doc, tmpColor, it->text(0), &m_colorList, false));
		if (!dia->exec())
			return;

		dia->Farbe.setSpotColor(dia->isSpotColor());
		dia->Farbe.setRegistrationColor(tmpColor.isRegistrationColor());
		m_colorList[dia->colorName()] = dia->Farbe;
		if (it->text(0) != dia->colorName())
		{
			replaceColorMap.insert(it->text(0), dia->colorName());
			m_colorList.remove(it->text(0));
		}
		updateGradientColors(dia->colorName(), it->text(0));
		updateGradientList();
		updatePatternList();
		QTreeWidgetItem *lg = updateColorList(dia->colorName());
		if (lg != nullptr)
		{
			dataTree->expandItem(lg->parent());
			dataTree->setCurrentItem(lg, 0, QItemSelectionModel::ClearAndSelect);
		}
		itemSelected(dataTree->currentItem());
		modified = true;
	}
	else if (it->parent() == patternItems)
	{
		QString patternName = origNames[it->text(0)];
		QString newName;
		Query dia(this, "tt", true, tr("&Name:"), tr("Rename Entry"));
		dia.setEditText(it->text(0), true);
		dia.setTestList(dialogPatterns.keys());
		if (!dia.exec())
			return;

		newName = dia.getEditText();
		ScPattern pat = dialogPatterns.take(it->text(0));
		dialogPatterns.insert(newName, pat);
		replaceMapPatterns.insert(patternName, newName);
		origNamesPatterns.remove(it->text(0));
		origNamesPatterns.insert(newName, patternName);
		QStringList patterns = dialogPatterns.keys();
		for (int i = 0; i < dialogPatterns.count(); ++i)
		{
			ScPattern pa = dialogPatterns[patterns[i]];
			for (int j = 0; j < pa.items.count(); ++j)
			{
				PageItem *ite = pa.items.at(j);
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
		if (lg != nullptr)
		{
			dataTree->expandItem(lg->parent());
			dataTree->setCurrentItem(lg, 0, QItemSelectionModel::ClearAndSelect);
		}
		itemSelected(dataTree->currentItem());
	}
}

void ColorsAndFillsDialog::duplicateColorItem()
{
	if (paletteLocked)
		return;
	const QTreeWidgetItem* it = dataTree->currentItem();
	if (!it)
		return;

	if (it->parent() == gradientItems)
	{
		QString gradN = tr("Copy of %1").arg(it->text(0));
		dialogGradients.insert(gradN, dialogGradients[it->text(0)]);
		origNames.insert(gradN, gradN);
		QString patternName = origNames[gradN];
		QScopedPointer<gradientEditDialog> dia(new gradientEditDialog(this, gradN, dialogGradients[gradN], m_colorList, m_doc, &dialogGradients, false));
		if (!dia->exec())
		{
			dialogGradients.remove(gradN);
			origNames.remove(gradN);
			return;
		}

		QString newName = dia->name();
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
		if (lg != nullptr)
		{
			dataTree->expandItem(lg->parent());
			dataTree->setCurrentItem(lg, 0, QItemSelectionModel::ClearAndSelect);
		}
		itemSelected(dataTree->currentItem());
		modified = true;
	}
	else if (it->parent() == colorItems)
	{
		QString nam = tr("Copy of %1").arg(it->text(0));
		m_colorList.insert(nam, m_colorList[it->text(0)]);
		ScColor tmpColor = m_colorList[nam];

		QScopedPointer<CMYKChoose> dia(new CMYKChoose(this, m_doc, tmpColor, nam, &m_colorList, false));
		if (!dia->exec())
		{
			m_colorList.remove(nam);
			return;
		}

		dia->Farbe.setSpotColor(dia->isSpotColor());
		dia->Farbe.setRegistrationColor(tmpColor.isRegistrationColor());
		m_colorList[dia->colorName()] = dia->Farbe;
		if (nam != dia->colorName())
		{
			replaceColorMap.insert(nam, dia->colorName());
			m_colorList.remove(nam);
		}
		updateGradientColors(dia->colorName(), nam);
		updateGradientList();
		updatePatternList();
		QTreeWidgetItem *lg = updateColorList(dia->colorName());
		if (lg != nullptr)
		{
			dataTree->expandItem(lg->parent());
			dataTree->setCurrentItem(lg, 0, QItemSelectionModel::ClearAndSelect);
		}
		itemSelected(dataTree->currentItem());
		modified = true;
	}
}

void ColorsAndFillsDialog::removeColorItem()
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
		for (int i = 0; i < selItems.count(); ++i)
		{
			const QTreeWidgetItem* it = selItems.at(i);
			if (it == gradientItems)
				continue;
			if (it == colorItems)
				continue;
			if (it == patternItems)
				continue;
			if (isMandatoryColor(it->text(0)))
				continue;
			if (it->parent() == gradientItems)
				gradients.append(it->text(0));
			if (it->parent() == colorItems)
				colors.append(it->text(0));
			if (it->parent() == patternItems)
				patterns.append(it->text(0));
		}
		for (int i = 0; i < gradients.count(); ++i)
		{
			dialogGradients.remove(gradients[i]);
			replaceMap.insert(gradients[i], QString());
		}
		bool hasUsed = false;
		ColorList UsedCG = getGradientColors();
		for (int i = 0; i < colors.count(); ++i)
		{
			if (UsedCG.contains(colors[i]) || inDocUsedColors.contains(colors[i]))
			{
				hasUsed = true;
				usedColors.append(colors[i]);
			}
		}
		if (hasUsed)
		{
			ColorList dCols = m_colorList;
			QString dColor = tr("Selected Colors");
			for (int i = 0; i < usedColors.count(); ++i)
			{
				dCols.remove(usedColors[i]);
			}
			dCols.insert(dColor , ScColor());
			DelColor *dia = new DelColor(this, dCols, dColor, true);
			if (dia->exec())
			{
				QString replacementColor(dia->getReplacementColor());
				for (int i = 0; i < colors.count(); ++i)
				{
					dColor = colors[i];
					if (replacementColor == CommonStrings::tr_NoneColor)
						replacementColor = CommonStrings::None;
					if (replaceColorMap.values().contains(dColor))
					{
						for (auto itt = replaceColorMap.begin(); itt != replaceColorMap.end(); ++itt)
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
			for (int i = 0; i < colors.count(); ++i)
			{
				replaceColorMap.insert(colors[i], "Black");
				m_colorList.remove(colors[i]);
			}
			modified = true;
		}
		updatePatternList();
		updateGradientList();
		updateColorList();
		itemSelected(nullptr);
	}
	else
	{
		QTreeWidgetItem* it = dataTree->currentItem();
		if (!it)
			return;
		if ((it->parent() == gradientItems) || (it == gradientItems))
		{
			if (it == gradientItems)
			{
				int t = ScMessageBox::warning(this, CommonStrings::trWarning, tr("Do you really want to clear all your gradients?"),
						QMessageBox::Yes | QMessageBox::No,
						QMessageBox::No,	// GUI default
						QMessageBox::Yes);	// batch default
				if (t == QMessageBox::No)
					return;
				replaceMap.clear();
				for (auto git = dialogGradients.cbegin(); git != dialogGradients.cend(); ++git)
				{
					replaceMap.insert(git.key(), "");
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
				int t = ScMessageBox::warning(this, CommonStrings::trWarning, tr("Do you really want to clear all your colors and gradients?"),
						QMessageBox::Yes | QMessageBox::No,
						QMessageBox::No,	// GUI default
						QMessageBox::Yes);	// batch default
				if (t == QMessageBox::No)
					return;
				replaceMap.clear();
				for (auto git = dialogGradients.cbegin(); git != dialogGradients.cend(); ++git)
				{
					replaceMap.insert(git.key(), "");
				}
				replaceColorMap.clear();
				for (auto cit = m_colorList.cbegin(); cit != m_colorList.cend(); ++cit)
				{
					replaceColorMap.insert(cit.key(), "Black");
				}
				dialogPatterns.clear();
				dialogGradients.clear();
				m_colorList.clear();
				m_colorList.ensureDefaultColors();
			}
			else
			{
				QString dColor = it->text(0);
				if (isMandatoryColor(dColor))
					return;
				ColorList UsedCG = getGradientColors();
				if (inDocUsedColors.contains(dColor) || UsedCG.contains(dColor))
				{
					QScopedPointer<DelColor> dia(new DelColor(this, m_colorList, dColor, true));
					if (dia->exec())
					{
						QString replacementColor(dia->getReplacementColor());
						if (replacementColor == CommonStrings::tr_NoneColor)
							replacementColor = CommonStrings::None;
						if (replaceColorMap.values().contains(dColor))
						{
							for (auto itt = replaceColorMap.begin(); itt != replaceColorMap.end(); ++itt)
							{
								if (itt.value() == dColor)
									itt.value() = replacementColor;
							}
						}
						replaceColorMap.insert(dColor, replacementColor);
						m_colorList.remove(dColor);
						updateGradientColors(replacementColor, dColor);
					}
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
				int t = ScMessageBox::warning(this, CommonStrings::trWarning, tr("Do you really want to clear all your patterns?"),
							QMessageBox::Yes | QMessageBox::No,
							QMessageBox::No,	// GUI default
							QMessageBox::Yes);	// batch default
				if (t == QMessageBox::No)
					return;
				replaceMapPatterns.clear();
				for (auto pit = dialogPatterns.cbegin(); pit != dialogPatterns.cend(); ++pit)
				{
					replaceMapPatterns.insert(pit.key(), QString());
				}
				dialogPatterns.clear();
			}
			else if (it->parent() == patternItems)
			{
				QStringList patterns2Del;
				QStringList mainPatterns = dialogPatterns.keys();
				for (int i = 0; i < mainPatterns.count(); ++i)
				{
					if (mainPatterns[i] != it->text(0))
					{
						QStringList subPatterns;
						subPatterns = getUsedPatternsHelper(mainPatterns[i], subPatterns);
						if (subPatterns.contains(it->text(0)))
							patterns2Del.append(mainPatterns[i]);
					}
				}
				patterns2Del.append(it->text(0));
				for (int i = 0; i < patterns2Del.count(); ++i)
				{
					dialogPatterns.remove(patterns2Del[i]);
				}
			}
		}
		updatePatternList();
		updateGradientList();
		updateColorList();
		itemSelected(nullptr);
		modified = true;
	}
}

QStringList ColorsAndFillsDialog::getUsedPatternsHelper(const QString& pattern, QStringList &results) const
{
	auto patternIt = dialogPatterns.find(pattern);
	if (patternIt == dialogPatterns.end())
		return results;

	const ScPattern* pat = &patternIt.value();
	QStringList pats;

	for (int i = 0; i < pat->items.count(); ++i)
	{
		if ((pat->items.at(i)->GrType == Gradient_Pattern) || (pat->items.at(i)->itemType() == PageItem::Symbol))
		{
			const QString& patName = pat->items.at(i)->pattern();
			if (!patName.isEmpty() && !results.contains(patName))
				pats.append(patName);
		}
		const QString& pat2 = pat->items.at(i)->strokePattern();
		if (!pat2.isEmpty() && !results.contains(pat2))
			pats.append(pat->items.at(i)->strokePattern());
		const QString& pat3 = pat->items.at(i)->patternMask();
		if (!pat3.isEmpty() && !results.contains(pat3))
			pats.append(pat->items.at(i)->patternMask());
	}
	if (!pats.isEmpty())
	{
		results = pats;
		for (int i = 0; i < pats.count(); ++i)
		{
			getUsedPatternsHelper(pats[i], results);
		}
	}
	return results;
}

void ColorsAndFillsDialog::removeUnusedColorItem()
{
	if (paletteLocked)
		return;
	QTreeWidgetItem* it = dataTree->currentItem();
	if (!it)
		return;

	if ((it->parent() == colorItems) || (it == colorItems))
	{
		ColorList usedGColors = getGradientColors();
		if (mainWin->HaveDoc)
		{
			m_colorList = inDocUsedColors;
			for (auto cit = usedGColors.cbegin(); cit != usedGColors.cend(); ++cit)
			{
				if (!m_colorList.contains(cit.key()))
					m_colorList.insert(cit.key(), cit.value());
			}
		}
		else
			m_colorList = usedGColors;
		m_colorList.ensureDefaultColors();
		updatePatternList();
		updateGradientList();
		updateColorList();
		itemSelected(nullptr);
		modified = true;
	}
}

void ColorsAndFillsDialog::importColorItems()
{
	const QTreeWidgetItem* it = dataTree->currentItem();
	if (!it)
		return;

	if ((it->parent() == gradientItems) || (it == gradientItems))
	{
		QString allFormats = tr("All Supported Formats")+" (";
		allFormats += "*.sgr *.SGR";
		allFormats += " *.ggr *.GGR";
		allFormats += ");;";
		QString formats = tr("Scribus Gradient Files \"*.sgr\" (*.sgr *.SGR);;");
		formats += tr("Gimp Gradient Files \"*.ggr\" (*.ggr *.GGR);;");
		formats += tr("All Files (*)");
		allFormats += formats;

		PrefsContext* dirs = PrefsManager::instance().prefsFile->getContext("dirs");
		QString wdir = dirs->get("gradients", ".");

		CustomFDialog dia(this, wdir, tr("Open"), allFormats, fdHidePreviewCheckBox | fdExistingFiles | fdDisableOk);
		if (dia.exec() != QDialog::Accepted)
			return;
		
		QString fileName = dia.selectedFile();
		if (!fileName.isEmpty())
		{
			PrefsManager::instance().prefsFile->getContext("dirs")->set("gradients", fileName.left(fileName.lastIndexOf("/")));
			QFileInfo fi(fileName);
			QString ext = fi.suffix().toLower();
			if (ext == "sgr")
				loadScribusFormat(fileName);
			else if (ext == "ggr")
				loadGimpFormat(fileName);
			updateGradientList();
			updateColorList();
			itemSelected(nullptr);
			modified = true;
		}
	}
	else if ((it->parent() == colorItems) || (it == colorItems))
	{
		QStringList allFormatsV = LoadSavePlugin::getExtensionsForColors();
		allFormatsV.removeAll("sla");
		allFormatsV.removeAll("scd");
		allFormatsV.removeAll("sla.gz");
		allFormatsV.removeAll("scd.gz");
		allFormatsV.removeAll("ai");
		QString extra = allFormatsV.join(" *.");
		extra.prepend(" *.");

		PrefsContext* dirs = PrefsManager::instance().prefsFile->getContext("dirs");
		QString wdir = dirs->get("colors", ".");
		QString docexts("*.sla *.sla.gz *.scd *.scd.gz");
		QString aiepsext(FormatsManager::instance()->extensionListForFormat(FormatsManager::EPS|FormatsManager::PS|FormatsManager::AI, 0));
		QString ooexts(" *.acb *.aco *.ase *.cxf *.gpl *.sbz *.skp *.soc *.xml");
		ooexts += extra;
		QString filter = tr("All Supported Formats (%1);;Documents (%2);;Other Files (%3);;All Files (*)").arg(docexts+" "+aiepsext+ooexts, docexts, aiepsext+ooexts);
		
		CustomFDialog dia(this, wdir, tr("Import Colors"), filter, fdHidePreviewCheckBox | fdDisableOk);
		if (dia.exec() != QDialog::Accepted)
			return;

		QString fileName = dia.selectedFile();
		if (!fileName.isEmpty())
			dirs->set("colors", fileName.left(fileName.lastIndexOf("/")));
		if (!importColorsFromFile(fileName, m_colorList))
			ScMessageBox::information(this, tr("Information"), "<qt>" + tr("The file %1 does not contain colors which can be imported.\nIf the file was a PostScript-based, try to import it with File -&gt; Import. \nNot all files have DSC conformant comments where the color descriptions are located.\n This prevents importing colors from some files.\nSee the Edit Colors section of the documentation for more details.").arg(fileName) + "</qt>");
		else
		{
			m_colorList.ensureDefaultColors();
			updateGradientList();
			updateColorList();
			modified = true;
		}
		itemSelected(nullptr);
	}
	else if ((it->parent() == patternItems) || (it == patternItems))
	{
		QString fileName;
		QStringList formats;
		QString allFormats = tr("All Supported Formats")+" (";
		int fmtCode = FORMATID_FIRSTUSER;
		const FileFormat *fmt = LoadSavePlugin::getFormatById(fmtCode);
		while (fmt != nullptr)
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
		QString form1;
		QString form2;
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
	//	imgFormats.append("pdf");
		imgFormats.append("eps");
		imgFormats.append("epsi");
		imgFormats.append("ps");
		formats.sort();
		allFormats += formats.join(";;");

		PrefsContext* dirs = PrefsManager::instance().prefsFile->getContext("dirs");
		QString wdir = dirs->get("patterns", ".");
		CustomFDialog dia(this, wdir, tr("Open"), allFormats, fdHidePreviewCheckBox | fdExistingFiles | fdDisableOk);
		if (dia.exec() != QDialog::Accepted)
			return;

		fileName = dia.selectedFile();
		if (fileName.isEmpty())
			return;

		QGuiApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
		PrefsManager::instance().prefsFile->getContext("dirs")->set("patterns", fileName.left(fileName.lastIndexOf("/")));
		QFileInfo fi(fileName);
		if ((fi.suffix().toLower() == "sce") || (!imgFormats.contains(fi.suffix().toLower())))
		{
			loadVectors(fileName);
		}
		else
		{
			QString patNam = fi.baseName().trimmed().simplified().replace(" ", "_");
			ScPattern pat(m_doc);
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
		itemSelected(nullptr);
		QGuiApplication::restoreOverrideCursor();
	}
}

void ColorsAndFillsDialog::loadPatternDir()
{
	PrefsContext* dirs = PrefsManager::instance().prefsFile->getContext("dirs");
	QString wdir = dirs->get("patterndir", ".");
	QString fileName = QFileDialog::getExistingDirectory(this, tr("Choose a Directory"), wdir);
	if (fileName.isEmpty())
		return;

	QStringList formats;
	formats += "eps";
	formats += "epsi";
	formats += "pdf";
	QString form1;
	for (int i = 0; i < QImageReader::supportedImageFormats().count(); ++i)
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
	if ((!d.exists()) || (d.count() == 0))
		return;

	mainWin->setStatusBarInfoText( tr("Loading Patterns"));
	mainWin->mainWindowProgressBar->reset();
	mainWin->mainWindowProgressBar->setMaximum(d.count() * 2);

	QGuiApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	QGuiApplication::processEvents(QEventLoop::ExcludeUserInputEvents);

	for (uint dc = 0; dc < d.count(); ++dc)
	{
		mainWin->mainWindowProgressBar->setValue(dc);
		QGuiApplication::processEvents(QEventLoop::ExcludeUserInputEvents);

		QFileInfo fi(QDir::cleanPath(QDir::toNativeSeparators(fileName + "/" + d[dc])));
		QString ext = fi.suffix().toLower();
		if ((ext == "sce") || (!formats.contains(ext)))
			loadVectors(QDir::cleanPath(QDir::toNativeSeparators(fileName + "/" + d[dc])));
	}
	for (uint dc = 0; dc < d.count(); ++dc)
	{
		mainWin->mainWindowProgressBar->setValue(d.count() + dc);
		QGuiApplication::processEvents(QEventLoop::ExcludeUserInputEvents);

		QFileInfo fi(QDir::cleanPath(QDir::toNativeSeparators(fileName + "/" + d[dc])));
		QString ext = fi.suffix().toLower();
		if ((ext == "sce") || (!formats.contains(ext)))
			continue;
		if (!formats.contains(ext))
			continue;

		QString patNam = fi.baseName().trimmed().simplified().replace(" ", "_");
		if (!dialogPatterns.contains(patNam))
		{
			ScPattern pat(m_doc);
			pat.setPattern(QDir::cleanPath(QDir::toNativeSeparators(fileName + "/" + d[dc])));
			dialogPatterns.insert(patNam, pat);
			origNamesPatterns.insert(patNam, patNam);
		}
	}
	d.cdUp();
	dirs->set("patterndir", d.absolutePath());

	QGuiApplication::restoreOverrideCursor();

	mainWin->setStatusBarInfoText("");
	mainWin->mainWindowProgressBar->reset();
}

void ColorsAndFillsDialog::loadVectors(const QString& data)
{
	int storedPageNum = m_doc->currentPageNumber();
	int storedContentsX = m_doc->view()->contentsX();
	int storedContentsY = m_doc->view()->contentsY();
	double storedViewScale = m_doc->view()->scale();
	FPoint stored_minCanvasCoordinate = m_doc->minCanvasCoordinate;
	FPoint stored_maxCanvasCoordinate = m_doc->maxCanvasCoordinate;

	m_doc->PageColors = m_colorList;
	m_doc->docGradients = dialogGradients;
	UndoManager::instance()->setUndoEnabled(false);
	m_doc->setLoading(true);
	QFileInfo fi(data);
	QString patNam = fi.baseName().trimmed().simplified().replace(" ", "_");
	uint ac = m_doc->Items->count();
	uint ap = m_doc->docPatterns.count();
	bool savedAlignGrid = m_doc->SnapGrid;
	bool savedAlignGuides = m_doc->SnapGuides;
	bool savedAlignElement = m_doc->SnapElement;
	m_doc->SnapGrid = false;
	m_doc->SnapGuides = false;
	m_doc->SnapElement = false;
	if (fi.suffix().toLower() == "sce")
	{
		ScriXmlDoc ss;
		ss.readElem(data, m_doc, m_doc->currentPage()->xOffset(), m_doc->currentPage()->yOffset(), true, true);
	}
	else
	{
		FileLoader *fileLoader = new FileLoader(data);
		int testResult = fileLoader->testFile();
		delete fileLoader;
		if ((testResult != -1) && (testResult >= FORMATID_FIRSTUSER))
		{
			const FileFormat * fmt = LoadSavePlugin::getFormatById(testResult);
			if (fmt)
			{
				fmt->setupTargets(m_doc, nullptr, mainWin, nullptr, &(PrefsManager::instance().appPrefs.fontPrefs.AvailFonts));
				fmt->loadFile(data, LoadSavePlugin::lfUseCurrentPage|LoadSavePlugin::lfInteractive|LoadSavePlugin::lfScripted|LoadSavePlugin::lfKeepPatterns|LoadSavePlugin::lfLoadAsPattern);
			}
		}
	}
	m_doc->SnapGrid = savedAlignGrid;
	m_doc->SnapGuides = savedAlignGuides;
	m_doc->SnapElement = savedAlignElement;
	uint ae = m_doc->Items->count();
	if (ac != ae)
	{
		for (uint as = ac; as < ae; ++as)
		{
			PageItem* ite = m_doc->Items->at(ac);
			if (ite->itemType() == PageItem::PathText)
				ite->updatePolyClip();
			else
				ite->layout();
		}

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

		ScPattern pat(m_doc);
		pat.pattern = currItem->DrawObj_toImage(qMin(qMax(maxx - minx, maxy - miny), 500.0));
		pat.width = maxx - minx;
		pat.height = maxy - miny;

		currItem->setXYPos(0, 0, true);
		currItem->setWidthHeight(maxx - minx, maxy - miny, true);
		currItem->groupWidth = maxx - minx;
		currItem->groupHeight = maxy - miny;
		currItem->gWidth = maxx - minx;
		currItem->gHeight = maxy - miny;
		for (uint as = ac; as < ae; ++as)
		{
			pat.items.append(m_doc->Items->takeAt(ac));
		}
		if (!dialogPatterns.contains(patNam))
		{
			dialogPatterns.insert(patNam, pat);
			origNamesPatterns.insert(patNam, patNam);
		}
		for (auto it = m_doc->docPatterns.cbegin(); it != m_doc->docPatterns.cend(); ++it)
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
			for (auto it = m_doc->docPatterns.cbegin(); it != m_doc->docPatterns.cend(); ++it)
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
	UndoManager::instance()->setUndoEnabled(true);

	m_doc->minCanvasCoordinate = stored_minCanvasCoordinate;
	m_doc->maxCanvasCoordinate = stored_maxCanvasCoordinate;
	m_doc->view()->setScale(storedViewScale);
	m_doc->setCurrentPage(m_doc->DocPages.at(storedPageNum));
	m_doc->view()->setContentsPos(static_cast<int>(storedContentsX * storedViewScale), static_cast<int>(storedContentsY * storedViewScale));
}

bool ColorsAndFillsDialog::isMandatoryColor(const QString& colorName) const
{
	if (colorName == "Black" || colorName == "White")
		return true;
	ScColor color = m_colorList.value(colorName, ScColor());
	return color.isRegistrationColor();
}

QString ColorsAndFillsDialog::getColorTooltip(const ScColor& color) const
{
	QString tooltip;
	if (color.getColorModel() == colorModelRGB)
	{
		int r, g, b;
		color.getRawRGBColor(&r, &g, &b);
		tooltip = tr("R: %1 G: %2 B: %3").arg(r).arg(g).arg(b);
	}
	else if (color.getColorModel() == colorModelCMYK)
	{
		double c, m, y, k;
		color.getCMYK(&c, &m, &y, &k);
		return tr("C: %1% M: %2% Y: %3% K: %4%").arg(c * 100, 0, 'f', 2).arg(m * 100, 0, 'f', 2).arg(y * 100, 0, 'f', 2).arg(k * 100, 0, 'f', 2);
	}
	else if (color.getColorModel() == colorModelLab)
	{
		double L, a, b;
		color.getLab(&L, &a, &b);
		tooltip = tr("L: %1 a: %2 b: %3").arg(L, 0, 'f', 2).arg(a, 0, 'f', 2).arg(b, 0, 'f', 2);
	}
	return tooltip;
}

ColorList ColorsAndFillsDialog::getGradientColors() const
{
	ColorList colorList;
	for (auto itg = dialogGradients.begin(); itg != dialogGradients.end(); ++itg)
	{
		QList<VColorStop*> cstops = itg.value().colorStops();
		for (int cst = 0; cst < itg.value().stops(); ++cst)
		{
			if ((!colorList.contains(cstops.at(cst)->name)) && (cstops.at(cst)->name != CommonStrings::None))
				colorList.insert(cstops.at(cst)->name, m_colorList[cstops.at(cst)->name]);
		}
	}
	QStringList patterns = dialogPatterns.keys();
	for (int i = 0; i < dialogPatterns.count(); ++i)
	{
		ScPattern pa = dialogPatterns[patterns[i]];
		for (int j = 0; j < pa.items.count(); ++j)
		{
			PageItem *ite = pa.items.at(j);
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
					MeshPoint mp = ite->meshGradientArray[grow][gcol];
					if ((!colorList.contains(mp.colorName)) && (mp.colorName != CommonStrings::None))
						colorList.insert(mp.colorName, m_colorList[mp.colorName]);
				}
			}
			QList<VColorStop*> cstops = ite->fill_gradient.colorStops();
			for (int cst = 0; cst < ite->fill_gradient.stops(); ++cst)
			{
				if ((!colorList.contains(cstops.at(cst)->name)) && (cstops.at(cst)->name != CommonStrings::None))
					colorList.insert(cstops.at(cst)->name, m_colorList[cstops.at(cst)->name]);
			}
			cstops = ite->stroke_gradient.colorStops();
			for (int cst = 0; cst < ite->stroke_gradient.stops(); ++cst)
			{
				if ((!colorList.contains(cstops.at(cst)->name)) && (cstops.at(cst)->name != CommonStrings::None))
					colorList.insert(cstops.at(cst)->name, m_colorList[cstops.at(cst)->name]);
			}
			cstops = ite->mask_gradient.colorStops();
			for (int cst = 0; cst < ite->mask_gradient.stops(); ++cst)
			{
				if ((!colorList.contains(cstops.at(cst)->name)) && (cstops.at(cst)->name != CommonStrings::None))
					colorList.insert(cstops.at(cst)->name, m_colorList[cstops.at(cst)->name]);
			}
		}
	}
	colorList.remove(CommonStrings::None);
	return colorList;
}

void ColorsAndFillsDialog::updateGradientColors(const QString& newName, const QString& oldName)
{
	for (auto itg = dialogGradients.cbegin(); itg != dialogGradients.cend(); ++itg)
	{
		QList<VColorStop*> cstops = itg.value().colorStops();
		for (int cst = 0; cst < itg.value().stops(); ++cst)
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
	for (int i = 0; i < dialogPatterns.count(); ++i)
	{
		ScPattern pa = dialogPatterns[patterns[i]];
		for (int j = 0; j < pa.items.count(); ++j)
		{
			PageItem *ite = pa.items.at(j);
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
					MeshPoint mp = ite->meshGradientArray[grow][gcol];
					if (mp.colorName == oldName)
					{
						mp.colorName = newName;
						ite->setMeshPointColor(grow, gcol, mp.colorName, mp.shade, mp.transparency);
					}
				}
			}
			QList<VColorStop*> cstops = ite->fill_gradient.colorStops();
			for (int cst = 0; cst < ite->fill_gradient.stops(); ++cst)
			{
				if (oldName == cstops.at(cst)->name)
				{
					cstops.at(cst)->name = newName;
					ite->SetQColor(&cstops.at(cst)->color, cstops.at(cst)->name, cstops.at(cst)->shade);
				}
			}
			cstops = ite->stroke_gradient.colorStops();
			for (int cst = 0; cst < ite->stroke_gradient.stops(); ++cst)
			{
				if (oldName == cstops.at(cst)->name)
				{
					cstops.at(cst)->name = newName;
					ite->SetQColor(&cstops.at(cst)->color, cstops.at(cst)->name, cstops.at(cst)->shade);
				}
			}
			cstops = ite->mask_gradient.colorStops();
			for (int cst = 0; cst < ite->mask_gradient.stops(); ++cst)
			{
				if (oldName == cstops.at(cst)->name)
				{
					cstops.at(cst)->name = newName;
					ite->SetQColor(&cstops.at(cst)->color, cstops.at(cst)->name, cstops.at(cst)->shade);
				}
			}
		}
		PageItem *ite = pa.items.at(0);
		dialogPatterns[patterns[i]].pattern = ite->DrawObj_toImage(pa.items, 1.0);
	}
	m_doc->PageColors = colorListBack;
}

void ColorsAndFillsDialog::loadGimpFormat(const QString& fileName)
{
	QFile f(fileName);
	if (f.open(QIODevice::ReadOnly))
	{
		ScTextStream ts(&f);
		QString tmp, dummy;
		QString gradientName;
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
			VGradient gra(VGradient::linear);
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
				gradientName += "(" + QString::number(dialogGradients.count()) + ")";
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

void ColorsAndFillsDialog::addGimpColor(QString &colorName, double r, double g, double b)
{
	ScColor lf;
	int hR, hG, hB;
	int Rc = qRound(r * 255);
	int Gc = qRound(g * 255);
	int Bc = qRound(b * 255);
	lf.setRgbColor(Rc, Gc, Bc);
	for (auto it = m_colorList.cbegin(); it != m_colorList.cend(); ++it)
	{
		if (it.value().getColorModel() == colorModelRGB)
		{
			it.value().getRGB(&hR, &hG, &hB);
			if ((Rc == hR) && (Gc == hG) && (Bc == hB))
			{
				colorName = it.key();
				return;
			}
		}
	}
	m_colorList.insert(colorName, lf);
	hasImportedColors = true;
}

void ColorsAndFillsDialog::loadScribusFormat(const QString& fileName)
{
	QFile f(fileName);
	if (!f.open(QIODevice::ReadOnly))
		return;

	QDomDocument docu("scridoc");
	QTextStream ts(&f);
	ts.setCodec("UTF-8");
	QString errorMsg;
	int errorLine = 0;
	int errorColumn = 0;
	if (!docu.setContent(ts.readAll(), &errorMsg, &errorLine, &errorColumn) )
	{
		f.close();
		return;
	}
	f.close();

	QDomElement elem = docu.documentElement();
	if (elem.tagName() != "SCRIBUSGRADIENT")
		return;

	QDomNode domDoc = elem.firstChild();
	while (!domDoc.isNull())
	{
		QDomElement dc = domDoc.toElement();
		if (dc.tagName() == "COLOR")
		{
			ScColor lf;
			if (dc.hasAttribute("SPACE"))
			{
				QString space = dc.attribute("SPACE");
				if (space == "CMYK")
				{
					double c = dc.attribute("C", "0").toDouble() / 100.0;
					double m = dc.attribute("M", "0").toDouble() / 100.0;
					double y = dc.attribute("Y", "0").toDouble() / 100.0;
					double k = dc.attribute("K", "0").toDouble() / 100.0;
					lf.setCmykColorF(c, m, y, k);
				}
				else if (space == "RGB")
				{
					double r = dc.attribute("R", "0").toDouble() / 255.0;
					double g = dc.attribute("G", "0").toDouble() / 255.0;
					double b = dc.attribute("B", "0").toDouble() / 255.0;
					lf.setRgbColorF(r, g, b);
				}
				else if (space == "Lab")
				{
					double L = dc.attribute("L", "0").toDouble();
					double a = dc.attribute("A", "0").toDouble();
					double b = dc.attribute("B", "0").toDouble();
					lf.setLabColor(L, a, b);
				}
			}
			else if (dc.hasAttribute("CMYK"))
				lf.setNamedColor(dc.attribute("CMYK"));
			else if (dc.hasAttribute("RGB"))
				lf.fromQColor(QColor(dc.attribute("RGB")));
			else
			{
				double L = dc.attribute("L", "0").toDouble();
				double a = dc.attribute("A", "0").toDouble();
				double b = dc.attribute("B", "0").toDouble();
				lf.setLabColor(L, a, b);
			}
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
			VGradient gra(VGradient::linear);
			gra.clearStops();
			QDomNode grad = dc.firstChild();
			while (!grad.isNull())
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
					color = ScColorEngine::getShadeColorProof(col, nullptr, shade);
				}
				gra.addStop(color, ramp, 0.5, opa, name, shade);
				grad = grad.nextSibling();
			}
			if (!dialogGradients.contains(dc.attribute("Name")))
				dialogGradients.insert(dc.attribute("Name"), gra);
			else
			{
				QString name = dc.attribute("Name");
				name += "(" + QString::number(dialogGradients.count()) + ")";
				dialogGradients.insert(name, gra);
			}
		}
		domDoc = domDoc.nextSibling();
	}
}

void ColorsAndFillsDialog::loadDefaults(QTreeWidgetItem* item)
{
	QString txt = item->data(0, Qt::UserRole).toString() + "/" + item->text(0);
	if (!mainWin->HaveDoc)
	{
		m_colorList.clear();
		dialogGradients.clear();
		dialogPatterns.clear();
	}
	QString pfadC2;
	if (item->text(0) == "Scribus Small")
	{
		m_colorList.insert("White", ScColor(0, 0, 0, 0));
		m_colorList.insert("Black", ScColor(0, 0, 0, 255));
		ScColor cc(255, 255, 255, 255);
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
			if (fi.absolutePath().contains(ScPaths::applicationDataDir()+"swatches/locked"))
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
			ScColor cc(255, 255, 255, 255);
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
	itemSelected(nullptr);
}

void ColorsAndFillsDialog::saveDefaults()
{
	const QTreeWidgetItem* item = LoadColSet->currentItem();
	QString NameK = item->data(0, Qt::UserRole).toString() + "/" + item->text(0);
	QString Name = LoadColSet->text();

	Query* dia = new Query(this, "Name", true, tr("&Name:"), tr("Choose a Name"));
	if ((customColSet.contains(NameK)) && (!paletteLocked))
		dia->setEditText(Name, false);
	else
		dia->setEditText("", false);
	if (dia->exec())
		doSaveDefaults(dia->getEditText(), (dia->getEditText() != Name));
	dia->deleteLater();
}

void ColorsAndFillsDialog::doSaveDefaults(const QString& name, bool changed)
{
	QString filename = name;
	filename.replace(" ", "_");
	filename  = QDir::toNativeSeparators(ScPaths::userPaletteFilesDir(true)) + filename;
	filename += ".xml";
	const FileFormat *fmt = LoadSavePlugin::getFormatById(FORMATID_SLA150EXPORT);
	if (fmt)
	{
		std::unique_ptr<ScribusDoc> s_doc(new ScribusDoc());
		s_doc->setup(0, 1, 1, 1, 1, "Custom", "Custom");
		s_doc->setPage(100, 100, 0, 0, 0, 0, 0, 0, false, false);
		s_doc->addPage(0);
		s_doc->setGUI(false, mainWin, nullptr);
		s_doc->PageColors = m_colorList;
		s_doc->setGradients(dialogGradients);
		s_doc->setPatterns(dialogPatterns);
		fmt->setupTargets(s_doc.get(), nullptr, mainWin, nullptr, &(PrefsManager::instance().appPrefs.fontPrefs.AvailFonts));
		fmt->savePalette(filename);
	}
	if (changed)
	{
		QString nameC = name;
		nameC.replace(" ", "_");
		nameC += ".xml";
		QFileInfo fi(filename);
		customColSet.append(fi.absolutePath() + "/" + nameC);
		QTreeWidgetItem *item = LoadColSet->addSubItem(name, userSwatches);
		item->setData(0, Qt::UserRole, fi.absolutePath());
		disconnect(LoadColSet, SIGNAL(activated(QTreeWidgetItem*)), this, SLOT(loadDefaults(QTreeWidgetItem*)));
		LoadColSet->setCurrentComboItem(fi.absolutePath() + "/" + name);
		connect(LoadColSet, SIGNAL(activated(QTreeWidgetItem*)), this, SLOT(loadDefaults(QTreeWidgetItem*)));
	}
}

QString ColorsAndFillsDialog::getColorSetName()
{
	QString NameK;
	const QTreeWidgetItem* item = LoadColSet->currentItem();
	if (item->text(0) == "Scribus Small")
		NameK = "Scribus Small";
	else
		NameK = item->data(0, Qt::UserRole).toString() + "/" + item->text(0);
	return NameK;
}

ScColor ColorsAndFillsDialog::selectedColor() const
{
	const QTreeWidgetItem* it = dataTree->currentItem();
	if (it && (it->parent() == colorItems))
		return m_colorList[it->text(0)];
	return ScColor();
}

QString ColorsAndFillsDialog::selectedColorName() const
{
	const QTreeWidgetItem* it = dataTree->currentItem();
	if (it && (it->parent() == colorItems))
		return it->text(0);
	return CommonStrings::None;
}

void ColorsAndFillsDialog::keyPressEvent(QKeyEvent* k)
{
	if (k->modifiers() != Qt::NoModifier || (k->key() != Qt::Key_Delete && k->key() != Qt::Key_Backspace))
		return;

	QString belowText;
	QList<QTreeWidgetItem *> selItems = dataTree->selectedItems();
	if (!selItems.isEmpty())
	{
		const QTreeWidgetItem* belowItem = dataTree->itemBelow(selItems.last());
		if (belowItem)
			belowText = belowItem->text(0);
	}

	removeColorItem();

	QList<QTreeWidgetItem *> belowItems = dataTree->findItems(belowText, Qt::MatchFixedString|Qt::MatchCaseSensitive|Qt::MatchRecursive);
	if (!belowItems.isEmpty())
	{
		// In case non deletable colors have been selected, some items
		//may still be selected, so clear selection first
		dataTree->selectionModel()->clearSelection();
		dataTree->setCurrentItem(belowItems[0]);
	}
}
