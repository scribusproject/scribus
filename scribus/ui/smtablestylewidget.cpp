/*
Copyright (C) 2011 Elvis Stansvik <elvstone@gmail.com>

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "smtablestylewidget.h"
#include "util_icon.h"

SMTableStyleWidget::SMTableStyleWidget(QWidget *parent) : QWidget()
{
	setupUi(this);

	fillColorIcon->setPixmap(loadIcon("16/color-fill.png"));
	fillColor->addItem(CommonStrings::tr_NoneColor);
	fillShadeLabel->setPixmap( loadIcon("shade.png") );
}

SMTableStyleWidget::~SMTableStyleWidget()
{
}

void SMTableStyleWidget::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
	else
		QWidget::changeEvent(e);
}

void SMTableStyleWidget::show(TableStyle *tableStyle, QList<TableStyle> &tableStyles, const QString &defLang, int unitIndex)
{
	Q_ASSERT(tableStyle);
	if (!tableStyle)
		return;
	parentCombo->setEnabled(!tableStyle->isDefaultStyle());
	const TableStyle *parent = dynamic_cast<const TableStyle*>(tableStyle->parentStyle());
	bool hasParent =  tableStyle->hasParent() && parent != 0 && parent->hasName() && tableStyle->parent() != "";
	if (hasParent)
	{
		fillColor->setCurrentText(tableStyle->fillColor(), tableStyle->isInhFillColor());
		fillColor->setParentText(parent->fillColor());
		fillShade_->setValue(qRound(tableStyle->fillShade()), tableStyle->isInhFillShade());
		fillShade_->setParentValue(qRound(parent->fillShade()));
	}
	else
	{
		fillColor->setCurrentText(tableStyle->fillColor());
		fillShade_->setValue(qRound(tableStyle->fillShade()));
	}
	parentCombo->clear();
	parentCombo->addItem( tableStyle->isDefaultStyle()? tr("A default style cannot be assigned a parent style") : "");
	if (!tableStyle->isDefaultStyle())
	{
		for (int i = 0; i < tableStyles.count(); ++i)
		{
			if (tableStyles[i].name() != tableStyle->name())
				parentCombo->addItem(tableStyles[i].name());
		}
	}

	if (tableStyle->isDefaultStyle() || !hasParent)
		parentCombo->setCurrentIndex(0);
	else if (hasParent)
	{
		int index = 0;
		for (int i = 0; i < parentCombo->count(); ++i)
		{
			if (parentCombo->itemText(i) == tableStyle->parentStyle()->name())
			{
				index = i;
				break;
			}
		}
		parentCombo->setCurrentIndex(index);
	}
}

void SMTableStyleWidget::show(QList<TableStyle*> &tableStyles, QList<TableStyle> &tableStylesAll, const QString &defaultLanguage, int unitIndex)
{
	if (tableStyles.count() == 1)
		show(tableStyles[0], tableStylesAll, defaultLanguage, unitIndex);
	else if (tableStyles.count() > 1)
	{
		showColors(tableStyles);
		parentCombo->setEnabled(false);
	}
}

void SMTableStyleWidget::showColors(const QList<TableStyle*> &tableStyles)
{
	double d = -30000;
	for (int i = 0; i < tableStyles.count(); ++i)
	{
		if (d != -30000 && tableStyles[i]->fillShade() != d)
		{
			d = -30000;
			break;
		}
		else
			d = tableStyles[i]->fillShade();
	}
	if (d == -30000)
		fillShade_->setText( tr("Shade"));
	else
		fillShade_->setValue(qRound(d));
	QString s;
	QString emptyString;
	for (int i = 0; i < tableStyles.count(); ++i)
	{
		if (!s.isNull() && s != tableStyles[i]->fillColor())
		{
			s = emptyString;
			break;
		}
		else
			s = tableStyles[i]->fillColor();
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

void SMTableStyleWidget::languageChange()
{
	fillColor->setToolTip( tr("Fill Color"));
	fillShade_->setToolTip( tr("Fill Shade"));
}

void SMTableStyleWidget::fillFillColorCombo(ColorList &colors)
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


