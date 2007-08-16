/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QToolTip>

#include "smlinestylewidget.h"
#include "colorcombo.h"
#include "linecombo.h"
#include "sccolorengine.h"
#include "scrspinbox.h"
#include "util_color.h"
#include "util_icon.h"


SMLineStyleWidget::SMLineStyleWidget() : QWidget()
{
	setupUi(this);
	addButton->setPixmap(loadIcon("penciladd.png"));
	removeButton->setPixmap(loadIcon("pencilsub.png"));

	dashCombo = new LineCombo(this);
	gridLayout->addWidget(dashCombo, 0, 0);

	lineWidth = new ScrSpinBox( 0, 300, this, 0 );
	gridLayout1->addWidget(lineWidth, 0, 1);
	
	colorCombo = new ColorCombo(this);
	gridLayout1->addWidget(colorCombo, 1, 0);

	endCombo->insertItem(loadIcon("ButtCap.png"), tr( "Flat Cap" ) );
	endCombo->insertItem(loadIcon("SquareCap.png"), tr( "Square Cap" ) );
	endCombo->insertItem(loadIcon("RoundCap.png"), tr( "Round Cap" ) );

	joinCombo->insertItem(loadIcon("MiterJoin.png"), tr( "Miter Join" ) );
	joinCombo->insertItem(loadIcon("BevelJoin.png"), tr( "Bevel Join" ) );
	joinCombo->insertItem(loadIcon("RoundJoin.png"), tr( "Round Join" ) );

	languageChange();
}

void SMLineStyleWidget::languageChange()
{
/***********************************/
/*      Begin Tooltips             */
/***********************************/
// These are for the line style page

	QToolTip::add(addButton,    tr("Add a new line"));
	QToolTip::add(removeButton, tr("Remove a line"));
	QToolTip::add(dashCombo,    tr("Line style"));
	QToolTip::add(lineWidth,    tr("Line width"));
	QToolTip::add(endCombo,     tr("End style"));
	QToolTip::add(joinCombo,    tr("Join style"));
	QToolTip::add(colorCombo,   tr("Line color"));
	QToolTip::add(shadeBox,     tr("Line shade"));

/***********************************/
/*      End Tooltips               */
/***********************************/


}

void SMLineStyleWidget::showStyle(const multiLine &lineStyle, ColorList &colorList, int subLine)
{
// 	disconnect(lineStyles, SIGNAL(highlighted(int)), this, SLOT(slotEditNewLine(int)));
	disconnect(lineStyles, SIGNAL(currentRowChanged(int)), this, SLOT(slotEditNewLine(int)));
	currentStyle = lineStyle;
	colorCombo->clear();
	ColorList::Iterator it;
	ScribusDoc* doc = colorList.document();
	for (it = colorList.begin(); it != colorList.end(); ++it)
		colorCombo->insertWideItem(colorList[it.key()], doc, it.key());
	colors = colorList;
	updateLineList();
	slotEditNewLine(subLine);
	connect(lineStyles, SIGNAL(currentRowChanged(int)), this, SLOT(slotEditNewLine(int)));
}

void SMLineStyleWidget::slotEditNewLine(int i)
{
	lineStyles->setCurrentRow(i);
// 	lineStyles->setSelected(i, true);
	//currentLine_ = i;

	// JG #5876 protect against broken line styles 
	// JG #6099 no current item selected
	if (currentStyle.count() <= i || (i < 0))
		return;

	lineWidth->setValue(currentStyle[i].Width);
	colorCombo->setCurrentText(currentStyle[i].Color);
	shadeBox->setValue(currentStyle[i].Shade);
	
	switch (static_cast<Qt::PenStyle>(currentStyle[i].Dash))
	{
	case Qt::SolidLine:
		dashCombo->setCurrentItem(0);
		break;
	case Qt::DashLine:
		dashCombo->setCurrentItem(1);
		break;
	case Qt::DotLine:
		dashCombo->setCurrentItem(2);
		break;
	case Qt::DashDotLine:
		dashCombo->setCurrentItem(3);
		break;
	case Qt::DashDotDotLine:
		dashCombo->setCurrentItem(4);
		break;
	default:
		dashCombo->setCurrentItem(0);
		break;
	}

	switch (static_cast<Qt::PenCapStyle>(currentStyle[i].LineEnd))
	{
	case Qt::FlatCap:
		endCombo->setCurrentItem(0);
		break;
	case Qt::SquareCap:
		endCombo->setCurrentItem(1);
		break;
	case Qt::RoundCap:
		endCombo->setCurrentItem(2);
		break;
	default:
		endCombo->setCurrentItem(0);
		break;
	}

	switch (static_cast<Qt::PenJoinStyle>(currentStyle[i].LineJoin))
	{
	case Qt::MiterJoin:
		joinCombo->setCurrentItem(0);
		break;
	case Qt::BevelJoin:
		joinCombo->setCurrentItem(1);
		break;
	case Qt::RoundJoin:
		joinCombo->setCurrentItem(2);
		break;
	default:
		joinCombo->setCurrentItem(0);
		break;
	}
}

void SMLineStyleWidget::updateLineList()
{
	QString tmp, tmp2;
	lineStyles->clear();
	QPixmap * pm2;
	for (multiLine::iterator it = currentStyle.begin(); it != currentStyle.end(); ++it)
	{
		pm2 = getWidePixmap(getColor((*it).Color, (*it).Shade));
		tmp2 = " "+tmp.setNum((*it).Width)+ tr(" pt")+" ";
		tmp2 += CommonStrings::translatePenStyleName(static_cast<Qt::PenStyle>((*it).Dash));
		tmp2 += " ";
		// lineStyles->insertItem( ...)
		lineStyles->addItem(new QListWidgetItem(*pm2, tmp2, lineStyles));
	}
}

QColor SMLineStyleWidget::getColor(const QString &name, int shade)
{
	const ScColor& color = colors[name];
	QColor tmpf = ScColorEngine::getDisplayColor(color, colors.document(), shade);
	return tmpf;
}

SMLineStyleWidget::~SMLineStyleWidget()
{
	
}
