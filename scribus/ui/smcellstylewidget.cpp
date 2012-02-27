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

void SMCellStyleWidget::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
	else
		QWidget::changeEvent(e);
}


void SMCellStyleWidget::show(CellStyle *cellStyle, QList<CellStyle> &cellStyles, const QString &defLang, int unitIndex)
{
	Q_ASSERT(cellStyle);
	if (!cellStyle)
		return;
	parentCombo->setEnabled(!cellStyle->isDefaultStyle());
	const CellStyle *parent = dynamic_cast<const CellStyle*>(cellStyle->parentStyle());
	bool hasParent =  cellStyle->hasParent() && parent != 0 && parent->hasName() && cellStyle->parent() != "";
	if (hasParent)
	{
		fillColor->setCurrentText(cellStyle->fillColor(), cellStyle->isInhFillColor());
		fillColor->setParentText(parent->fillColor());
		fillShade_->setValue(qRound(cellStyle->fillShade()), cellStyle->isInhFillShade());
		fillShade_->setParentValue(qRound(parent->fillShade()));
	}
	else
	{
		fillColor->setCurrentText(cellStyle->fillColor());
		fillShade_->setValue(qRound(cellStyle->fillShade()));
	}
	parentCombo->clear();
	parentCombo->addItem( cellStyle->isDefaultStyle()? tr("A default style cannot be assigned a parent style") : "");
	if (!cellStyle->isDefaultStyle())
	{
		for (int i = 0; i < cellStyles.count(); ++i)
		{
			if (cellStyles[i].name() != cellStyle->name())
				parentCombo->addItem(cellStyles[i].name());
		}
	}

	if (cellStyle->isDefaultStyle() || !hasParent)
		parentCombo->setCurrentIndex(0);
	else if (hasParent)
	{
		int index = 0;
		for (int i = 0; i < parentCombo->count(); ++i)
		{
			if (parentCombo->itemText(i) == cellStyle->parentStyle()->name())
			{
				index = i;
				break;
			}
		}
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
		else
			d = cellStyles[i]->fillShade();
	}
	if (d == -30000)
		fillShade_->setText( tr("Shade"));
	else
		fillShade_->setValue(qRound(d));
	QString s;
	QString emptyString;
	for (int i = 0; i < cellStyles.count(); ++i)
	{
		if (!s.isNull() && s != cellStyles[i]->fillColor())
		{
			s = emptyString;
			break;
		}
		else
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
