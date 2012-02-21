/*
Copyright (C) 2011 Elvis Stansvik <elvstone@gmail.com>

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "smcellstylewidget.h"
#include "util_icon.h"

SMCellStyleWidget::SMCellStyleWidget(QWidget *parent) : QWidget()
{
	setupUi(this);

	fillColorIcon->setPixmap(loadIcon("16/color-fill.png"));
	fillColor->addItem(CommonStrings::tr_NoneColor);
	fillShadeLabel->setPixmap( loadIcon("shade.png") );
	fillShade_->setToolTip( tr("Fill Shade"));
}

SMCellStyleWidget::~SMCellStyleWidget()
{
}


void SMCellStyleWidget::show(CellStyle *cellStyle, QList<CellStyle> &cellStyles, const QString &defLang, int unitIndex)
{
	Q_ASSERT(cellStyle);
	if (!cellStyle)
		return;

	// TODO: Handle parent styles (and language?)

	fillColor->setCurrentText(cellStyle->fillColor());
	fillShade_->setValue(qRound(cellStyle->fillShade()));
}

void SMCellStyleWidget::show(QList<CellStyle*> &cellStyles, QList<CellStyle> &cellStylesAll, const QString &defaultLanguage, int unitIndex)
{
	if (cellStyles.count() == 1) {
		show(cellStyles[0], cellStylesAll, defaultLanguage, unitIndex);
	}

	// TODO: Implement actual support multiple styles.
}

void SMCellStyleWidget::languageChange()
{
	fillColor->setToolTip(tr("Fill Color"));
}

void SMCellStyleWidget::fillFillColorCombo(ColorList &colors)
{
	fillColor->clear();

	fillColor->addItem(CommonStrings::tr_NoneColor);
	ColorList::Iterator itEnd = colors.end();
	ScribusDoc* doc = colors.document();
	for (ColorList::Iterator it = colors.begin(); it != itEnd; ++it)
	{
		fillColor->insertFancyItem(it.value(), doc, it.key());
	}
	fillColor->view()->setMinimumWidth(fillColor->view()->maximumViewportSize().width()+24);
}
