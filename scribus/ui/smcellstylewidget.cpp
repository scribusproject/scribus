/*
Copyright (C) 2011 Elvis Stansvik <elvstone@gmail.com>

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "iconmanager.h"
#include "scribus.h"
#include "scribusapp.h"
#include "smcellstylewidget.h"

SMCellStyleWidget::SMCellStyleWidget(QWidget *parent)
                 : QWidget(parent)
{
	setupUi(this);

	fillColor->setPixmapType(ColorCombo::fancyPixmaps);
	fillColor->addItem(CommonStrings::tr_NoneColor);

	iconSetChange();

	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));
}

void SMCellStyleWidget::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
		languageChange();
	else
		QWidget::changeEvent(e);
}

void SMCellStyleWidget::iconSetChange()
{
	IconManager& iconManager = IconManager::instance();
	fillColorIcon->setPixmap(iconManager.loadPixmap("16/color-fill.png"));
	fillShadeLabel->setPixmap(iconManager.loadPixmap("shade.png") );
}

void SMCellStyleWidget::languageChange()
{
	retranslateUi(this);

	if (fillColor->count() > 0)
	{
		bool fillColorBlocked = fillColor->blockSignals(true);
		fillColor->setItemText(0, CommonStrings::tr_NoneColor);
		fillColor->blockSignals(fillColorBlocked);
	}
}

void SMCellStyleWidget::handleUpdateRequest(int updateFlags)
{
	if (!m_Doc)
		return;
	if (updateFlags & reqColorsUpdate)
		fillFillColorCombo(m_Doc->PageColors);
}

void SMCellStyleWidget::setDoc(ScribusDoc* doc)
{
	if (m_Doc)
		disconnect(m_Doc->scMW(), SIGNAL(UpdateRequest(int)), this , SLOT(handleUpdateRequest(int)));

	m_Doc = doc;
	if (!m_Doc)
		return;

	fillFillColorCombo(m_Doc->PageColors);
	connect(m_Doc->scMW(), SIGNAL(UpdateRequest(int)), this , SLOT(handleUpdateRequest(int)));
}

void SMCellStyleWidget::show(CellStyle *cellStyle, QList<CellStyle> &cellStyles, const QString &defaultLanguage, int unitIndex)
{
	Q_ASSERT(cellStyle);
	if (!cellStyle)
		return;

	parentCombo->setEnabled(!cellStyle->isDefaultStyle());

	const CellStyle *parent = dynamic_cast<const CellStyle*>(cellStyle->parentStyle());
	bool hasParent =  cellStyle->hasParent() && parent != nullptr && parent->hasName() && cellStyle->parent() != "";
	if (hasParent)
	{
		fillColor->setCurrentText(cellStyle->fillColor(), cellStyle->isInhFillColor());
		fillColor->setParentText(parent->fillColor());
		fillShade->setValue(qRound(cellStyle->fillShade()), cellStyle->isInhFillShade());
		fillShade->setParentValue(qRound(parent->fillShade()));
	}
	else
	{
		fillColor->setCurrentText(cellStyle->fillColor());
		fillShade->setValue(qRound(cellStyle->fillShade()));
	}

	parentCombo->clear();
	parentCombo->addItem( cellStyle->isDefaultStyle()? tr("A default style cannot be assigned a parent style") : "");
	if (!cellStyle->isDefaultStyle())
	{
		QStringList styleNames;
		for (int i = 0; i < cellStyles.count(); ++i)
		{
			if (cellStyles[i].name() != cellStyle->name())
				styleNames.append(cellStyles[i].name());
		}
		styleNames.sort(Qt::CaseSensitive);
		parentCombo->addItems(styleNames);
	}

	if (cellStyle->isDefaultStyle() || !hasParent)
		parentCombo->setCurrentIndex(0);
	else if (hasParent)
	{
		int index = parentCombo->findText(cellStyle->parentStyle()->name());
		if (index < 0)
			index = 0;
		parentCombo->setCurrentIndex(index);
	}
}

void SMCellStyleWidget::show(QList<CellStyle*> &cellStyles, QList<CellStyle> &cellStylesAll, const QString &defaultLanguage, int unitIndex)
{
	if (cellStyles.count() == 1)
		show(cellStyles[0], cellStylesAll, defaultLanguage, unitIndex);
	else if (cellStyles.count() > 1)
	{
		showColors(cellStyles);
		parentCombo->setEnabled(false);
	}
}

void SMCellStyleWidget::showColors(const QList<CellStyle*> &cellStyles)
{
	double d = -30000;
	for (int i = 0; i < cellStyles.count(); ++i)
	{
		if (d != -30000 && cellStyles[i]->fillShade() != d)
		{
			d = -30000;
			break;
		}
		d = cellStyles[i]->fillShade();
	}
	if (d == -30000)
		fillShade->setText( tr("Shade"));
	else
		fillShade->setValue(qRound(d));
	QString s;
	for (int i = 0; i < cellStyles.count(); ++i)
	{
		if (!s.isEmpty() && s != cellStyles[i]->fillColor())
		{
			s.clear();
			break;
		}
		s = cellStyles[i]->fillColor();
	}
	if (s.isEmpty())
	{
		if (fillColor->itemText(fillColor->count() - 1) != "")
			fillColor->addItem("");
		fillColor->setCurrentIndex(fillColor->count() - 1);
	}
	else
		fillColor->setCurrentText(s);
}

void SMCellStyleWidget::fillFillColorCombo(ColorList &colors)
{
	fillColor->clear();

	fillColor->setColors(colors, true);
	fillColor->view()->setMinimumWidth(fillColor->view()->maximumViewportSize().width()+24);
}
