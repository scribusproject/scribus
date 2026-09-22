/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/**************************************************************************
*   Copyright (C) 2008 by Franz Schmid                                    *
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

#include "replacecolors.h"
#include "replaceonecolor.h"
#include "commonstrings.h"
#include "sccolorengine.h"
#include "util_color.h"
#include "iconmanager.h"
#include "scribusdoc.h"
#include <QDialogButtonBox>
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QTreeWidget>
#include <QVBoxLayout>

replaceColorsDialog::replaceColorsDialog(QWidget* parent, ColorList &colorList, ColorList &colorListUsed) : QDialog(parent)
{
	setupUi(this);
	setModal(true);
	setWindowIcon(IconManager::instance().loadPixmap("app-icon"));
	EditColors = colorList;
	UsedColors = colorListUsed;
	replaceMap.clear();
	alertIcon = IconManager::instance().loadPixmap("alert-warning");
	cmykIcon = IconManager::instance().loadPixmap("color-cmyk");
	rgbIcon = IconManager::instance().loadPixmap("color-rgb");
	labIcon = IconManager::instance().loadPixmap("color-lab");
	spotIcon = IconManager::instance().loadPixmap("color-spot");
	regIcon = IconManager::instance().loadPixmap("color-registration");
	replacementTable->horizontalHeader()->setSectionsClickable(false );
	replacementTable->horizontalHeader()->setSectionsMovable( false );
	replacementTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	replacementTable->setHorizontalHeaderItem(0, new QTableWidgetItem( tr("Original")));
	replacementTable->setHorizontalHeaderItem(1, new QTableWidgetItem( tr("Replacement")));
	replacementTable->verticalHeader()->setSectionsMovable( false );
	replacementTable->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	replacementTable->verticalHeader()->hide();
	replacementTable->setIconSize(QSize(60, 15));
	updateReplacementTable();
	removeButton->setEnabled(false);
	connect(addButton, SIGNAL(clicked()), this, SLOT(addColor()));
	connect(replacementTable, SIGNAL(cellClicked(int,int)), this, SLOT(selReplacement(int)));
	connect(removeButton, SIGNAL(clicked()), this, SLOT(delReplacement()));
	connect(editButton, SIGNAL(clicked()), this, SLOT(editReplacement()));
}

RGBToCMYKDialog::RGBToCMYKDialog(QWidget* parent, const ScribusDoc* doc, const QMap<QString, ScColor>& preview) : QDialog(parent)
{
	setModal(true);
	setWindowTitle(tr("Convert RGB Colors to CMYK"));
	setWindowIcon(IconManager::instance().loadIcon("app-icon"));
	auto* layout = new QVBoxLayout(this);
	const QString sourceProfile = doc->cmsSettings().DefaultSolidColorRGBProfile;
	const QString targetProfile = doc->cmsSettings().DefaultSolidColorCMYKProfile;
	auto* profileLabel = new QLabel(tr("Source: %1\nDestination: %2").arg(sourceProfile, targetProfile), this);
	profileLabel->setWordWrap(true);
	layout->addWidget(profileLabel);
	auto* explanation = new QLabel(tr("Select named RGB process colors to convert. Spot and registration colors are preserved. Review the proposed CMYK values before applying. To change profiles, use Document Setup > Color Management. Screen color patches are approximate."), this);
	explanation->setWordWrap(true);
	layout->addWidget(explanation);

	m_colors = new QTreeWidget(this);
	m_colors->setHeaderLabels({tr("Convert"), tr("Color"), tr("RGB"), tr("CMYK %")});
	m_colors->setRootIsDecorated(false);
	m_colors->setAlternatingRowColors(true);
	for (auto it = preview.cbegin(); it != preview.cend(); ++it)
	{
		const ScColor& source = doc->PageColors.value(it.key());
		int r, g, b;
		source.getRGB(&r, &g, &b);
		double c, m, y, k;
		it.value().getCMYK(&c, &m, &y, &k);
		auto* row = new QTreeWidgetItem(m_colors);
		row->setFlags(row->flags() | Qt::ItemIsUserCheckable);
		row->setCheckState(0, Qt::Checked);
		row->setText(1, it.key());
		row->setText(2, QStringLiteral("%1, %2, %3").arg(r).arg(g).arg(b));
		row->setText(3, QStringLiteral("%1, %2, %3, %4")
			.arg(qRound(c * 100)).arg(qRound(m * 100)).arg(qRound(y * 100)).arg(qRound(k * 100)));
		QPixmap sourceIcon(16, 16);
		sourceIcon.fill(ScColorEngine::getDisplayColor(source, doc));
		row->setIcon(2, QIcon(sourceIcon));
		QPixmap targetIcon(16, 16);
		targetIcon.fill(ScColorEngine::getDisplayColor(it.value(), doc));
		row->setIcon(3, QIcon(targetIcon));
	}
	m_colors->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
	m_colors->header()->setSectionResizeMode(1, QHeaderView::Stretch);
	layout->addWidget(m_colors);

	m_buttons = new QDialogButtonBox(QDialogButtonBox::Cancel, this);
	QPushButton* convertButton = m_buttons->addButton(tr("Convert Selected"), QDialogButtonBox::AcceptRole);
	layout->addWidget(m_buttons);
	connect(m_buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(m_buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
	connect(m_colors, &QTreeWidget::itemChanged, this, [this, convertButton]() {
		convertButton->setEnabled(!selectedColors().isEmpty());
	});
	convertButton->setEnabled(!preview.isEmpty());
	resize(720, 420);
}

QStringList RGBToCMYKDialog::selectedColors() const
{
	QStringList names;
	for (int i = 0; i < m_colors->topLevelItemCount(); ++i)
	{
		const QTreeWidgetItem* row = m_colors->topLevelItem(i);
		if (row->checkState(0) == Qt::Checked)
			names.append(row->text(1));
	}
	return names;
}

void replaceColorsDialog::addColor()
{
	std::unique_ptr<replaceColorDialog> dia(new replaceColorDialog(this, EditColors, UsedColors));
	if (!dia->exec())
		return;

	QString orig = dia->getOriginalColor();
	if (orig == CommonStrings::tr_NoneColor)
		orig = CommonStrings::None;
	QString repl = dia->getReplacementColor();
	if (repl == CommonStrings::tr_NoneColor)
		repl = CommonStrings::None;
	replaceMap.insert(orig, repl);
	updateReplacementTable();
}

void replaceColorsDialog::selReplacement(int sel)
{
	selectedRow = sel;
	removeButton->setEnabled(true);
	editButton->setEnabled(true);
}

void replaceColorsDialog::delReplacement()
{
	if (selectedRow > -1)
	{
		replaceMap.remove(replacementTable->item(selectedRow, 0)->text());
		replacementTable->removeRow(selectedRow);
		selectedRow = -1;
		removeButton->setEnabled(false);
		editButton->setEnabled(false);
	}
}

void replaceColorsDialog::editReplacement()
{
	if (selectedRow < 0)
		return;

	std::unique_ptr<replaceColorDialog> dia(new replaceColorDialog(this, EditColors, UsedColors));
	dia->setReplacementColor(replacementTable->item(selectedRow, 1)->text());
	dia->setOriginalColor(replacementTable->item(selectedRow, 0)->text());
	if (!dia->exec())
		return;

	replaceMap.remove(replacementTable->item(selectedRow, 0)->text());
	QString orig = dia->getOriginalColor();
	if (orig == CommonStrings::tr_NoneColor)
		orig = CommonStrings::None;
	QString repl = dia->getReplacementColor();
	if (repl == CommonStrings::tr_NoneColor)
		repl = CommonStrings::None;
	replaceMap.insert(orig, repl);
	updateReplacementTable();
}

void replaceColorsDialog::updateReplacementTable()
{
	replacementTable->clearContents();
	replacementTable->setRowCount(0);
	selectedRow = -1;
	removeButton->setEnabled(false);
	editButton->setEnabled(false);
	if (replaceMap.count() > 0)
	{
		replacementTable->setRowCount(replaceMap.count());
		int row = 0;
		QMap<QString,QString>::Iterator it;
		for (it = replaceMap.begin(); it != replaceMap.end(); ++it)
		{
			QTableWidgetItem *tW;
			if (it.key() == CommonStrings::None)
				tW = new QTableWidgetItem(CommonStrings::tr_NoneColor);
			else
				tW = new QTableWidgetItem(getColorIcon(it.key()), it.key());
			tW->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
			replacementTable->setItem(row, 0, tW);
			QTableWidgetItem *tW2;
			if (it.value() == CommonStrings::None)
				tW2 = new QTableWidgetItem(CommonStrings::tr_NoneColor);
			else
				tW2 = new QTableWidgetItem(getColorIcon(it.value()), it.value());
			tW2->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
			replacementTable->setItem(row, 1, tW2);
			row++;
		}
	}
}

QPixmap replaceColorsDialog::getColorIcon(const QString& color) const
{
	QPixmap smallPix(15, 15);
	QPixmap pPixmap(60,15);
	pPixmap.fill(Qt::transparent);
	ScColor m_color = EditColors[color];
	QColor rgb = ScColorEngine::getDisplayColor(m_color, EditColors.document());
	smallPix.fill(rgb);
	QPainter painter(&smallPix);
	painter.setBrush(Qt::NoBrush);
	QPen b(Qt::black, 1);
	painter.setPen(b);
	painter.drawRect(0, 0, 15, 15);
	painter.end();
	paintAlert(smallPix, pPixmap, 0, 0);
	bool isOutOfGamut = ScColorEngine::isOutOfGamut(m_color, EditColors.document());
	if (isOutOfGamut)
		paintAlert(alertIcon, pPixmap, 15, 0);
	if (m_color.getColorModel() == colorModelCMYK)
		paintAlert(cmykIcon, pPixmap, 30, 0);
	else if (m_color.getColorModel() == colorModelRGB)
		paintAlert(rgbIcon, pPixmap, 30, 0);
	else if (m_color.getColorModel() == colorModelLab)
		paintAlert(labIcon, pPixmap, 30, 0);
	if (m_color.isSpotColor())
		paintAlert(spotIcon, pPixmap, 45, 0);
	if (m_color.isRegistrationColor())
		paintAlert(regIcon, pPixmap, 46, 0);
	return pPixmap;
}
