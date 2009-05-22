/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QEvent>
#include <QToolTip>

#include "smlinestylewidget.h"
#include "colorcombo.h"
#include "linecombo.h"
#include "sccolorengine.h"
#include "scrspinbox.h"
#include "util_color.h"
#include "util_icon.h"
#include "util.h"


SMLineStyleWidget::SMLineStyleWidget() : QWidget()
{
	setupUi(this);
	addButton->setIcon(QIcon(loadIcon("penciladd.png")));
	removeButton->setIcon(QIcon(loadIcon("pencilsub.png")));

	dashCombo = new LineCombo(this);
	gridLayout->addWidget(dashCombo, 0, 0);

	lineWidth = new ScrSpinBox( 0, 300, this, 0 );
	gridLayout1->addWidget(lineWidth, 0, 1);
	
	colorCombo = new ColorCombo(this);
	gridLayout1->addWidget(colorCombo, 1, 0);

	endCombo->addItem(loadIcon("ButtCap.png"), tr( "Flat Cap" ) );
	endCombo->addItem(loadIcon("SquareCap.png"), tr( "Square Cap" ) );
	endCombo->addItem(loadIcon("RoundCap.png"), tr( "Round Cap" ) );

	joinCombo->addItem(loadIcon("MiterJoin.png"), tr( "Miter Join" ) );
	joinCombo->addItem(loadIcon("BevelJoin.png"), tr( "Bevel Join" ) );
	joinCombo->addItem(loadIcon("RoundJoin.png"), tr( "Round Join" ) );

	languageChange();
}

void SMLineStyleWidget::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
}

void SMLineStyleWidget::languageChange()
{
/***********************************/
/*      Begin Tooltips             */
/***********************************/
// These are for the line style page

	addButton->setToolTip(    tr("Add a new line"));
	removeButton->setToolTip( tr("Remove a line"));
	dashCombo->setToolTip(    tr("Line style"));
	lineWidth->setToolTip(    tr("Line width"));
	endCombo->setToolTip(     tr("End style"));
	joinCombo->setToolTip(    tr("Join style"));
	colorCombo->setToolTip(   tr("Line color"));
	shadeBox->setToolTip(     tr("Line shade"));

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
	colorCombo->updateBox(colorList, ColorCombo::fancyPixmaps, false);
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
	setCurrentComboItem(colorCombo, currentStyle[i].Color);
	shadeBox->setValue(currentStyle[i].Shade);
	
	dashCombo->setCurrentIndex(currentStyle[i].Dash - 1);
	switch (static_cast<Qt::PenCapStyle>(currentStyle[i].LineEnd))
	{
	case Qt::FlatCap:
		endCombo->setCurrentIndex(0);
		break;
	case Qt::SquareCap:
		endCombo->setCurrentIndex(1);
		break;
	case Qt::RoundCap:
		endCombo->setCurrentIndex(2);
		break;
	default:
		endCombo->setCurrentIndex(0);
		break;
	}

	switch (static_cast<Qt::PenJoinStyle>(currentStyle[i].LineJoin))
	{
	case Qt::MiterJoin:
		joinCombo->setCurrentIndex(0);
		break;
	case Qt::BevelJoin:
		joinCombo->setCurrentIndex(1);
		break;
	case Qt::RoundJoin:
		joinCombo->setCurrentIndex(2);
		break;
	default:
		joinCombo->setCurrentIndex(0);
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
		if ((*it).Dash < 6)
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
