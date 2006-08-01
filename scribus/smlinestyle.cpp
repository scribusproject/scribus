/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "smlinestyle.h"
#include "smlinestyle.moc"
#include "scribusdoc.h"
#include "linecombo.h"
#include "mspinbox.h"
#include "colorm.h"
#include "util.h"
#include "selection.h"
#include <qheader.h>
#include <qlineedit.h>
#include <qlistview.h>
#include <qpushbutton.h>
#include <qpixmap.h>
#include <qvaluelist.h>
#include <qtabwidget.h>
#include <qlayout.h>
#include <qlistbox.h>

extern QPixmap loadIcon(QString nam);

/*** LineStyleWidget *******************************************************/

LineStyleWidget::LineStyleWidget() : LineStyleWBase()
{
	addButton->setPixmap(loadIcon("penciladd.png"));
	removeButton->setPixmap(loadIcon("pencilsub.png"));

	dashCombo = new LineCombo(this);
	lineLayout->addWidget(dashCombo, 0, 0);

	lineWidth = new MSpinBox( 0, 300, this, 1 );
	lineWidth->setSuffix( tr( " pt" ) );
	widthLayout->addWidget(lineWidth, 0, 1);

	endCombo->insertItem(loadIcon("ButtCap.png"), tr( "Flat Cap" ) );
	endCombo->insertItem(loadIcon("SquareCap.png"), tr( "Square Cap" ) );
	endCombo->insertItem(loadIcon("RoundCap.png"), tr( "Round Cap" ) );

	joinCombo->insertItem(loadIcon("MiterJoin.png"), tr( "Miter Join" ) );
	joinCombo->insertItem(loadIcon("BevelJoin.png"), tr( "Bevel Join" ) );
	joinCombo->insertItem(loadIcon("RoundJoin.png"), tr( "Round Join" ) );
}

void LineStyleWidget::showStyle(const multiLine &lineStyle, ColorList &colorList)
{
	currentStyle = lineStyle;
	colorCombo->clear();
	ColorList::Iterator it;
	for (it = colorList.begin(); it != colorList.end(); ++it)
	{
		colorCombo->listBox()->insertItem(new ColorWidePixmapItem(colorList[it.key()], it.key()));
	}
	colors = colorList;
	updateLineList();
	slotEditNewLine(0);
}

void LineStyleWidget::slotEditNewLine(int i)
{
	lineStyles->setSelected(i, true);
	//CurLin = i;
	lineWidth->setValue(currentStyle[i].Width);
	colorCombo->setCurrentText(currentStyle[i].Color);
	shadeBox->setValue(currentStyle[i].Shade);
	
	switch (static_cast<PenStyle>(currentStyle[i].Dash))
	{
	case SolidLine:
		dashCombo->setCurrentItem(0);
		break;
	case DashLine:
		dashCombo->setCurrentItem(1);
		break;
	case DotLine:
		dashCombo->setCurrentItem(2);
		break;
	case DashDotLine:
		dashCombo->setCurrentItem(3);
		break;
	case DashDotDotLine:
		dashCombo->setCurrentItem(4);
		break;
	default:
		dashCombo->setCurrentItem(0);
		break;
	}

	switch (static_cast<PenCapStyle>(currentStyle[i].LineEnd))
	{
	case FlatCap:
		endCombo->setCurrentItem(0);
		break;
	case SquareCap:
		endCombo->setCurrentItem(1);
		break;
	case RoundCap:
		endCombo->setCurrentItem(2);
		break;
	default:
		endCombo->setCurrentItem(0);
		break;
	}
	switch (static_cast<PenJoinStyle>(currentStyle[i].LineJoin))
	{
	case MiterJoin:
		joinCombo->setCurrentItem(0);
		break;
	case BevelJoin:
		joinCombo->setCurrentItem(1);
		break;
	case RoundJoin:
		joinCombo->setCurrentItem(2);
		break;
	default:
		joinCombo->setCurrentItem(0);
		break;
	}
}

void LineStyleWidget::updateLineList()
{
	QString tmp, tmp2;
	lineStyles->clear();
	QPixmap * pm2;
	for (multiLine::iterator it = currentStyle.begin(); it != currentStyle.end(); ++it)
	{
		pm2 = getWidePixmap(getColor((*it).Color, (*it).Shade));
		tmp2 = " "+tmp.setNum((*it).Width)+ tr(" pt")+" ";
		switch (static_cast<PenStyle>((*it).Dash))
		{
		case SolidLine:
			tmp2 += tr("Solid Line");
			break;
		case DashLine:
			tmp2 += tr("Dashed Line");
			break;
		case DotLine:
			tmp2 += tr("Dotted Line");
			break;
		case DashDotLine:
			tmp2 += tr("Dash Dot Line");
			break;
		case DashDotDotLine:
			tmp2 += tr("Dash Dot Dot Line");
			break;
		default:
			tmp2 += tr("Solid Line");
			break;
		}
		tmp2 += " ";
		lineStyles->insertItem(*pm2, tmp2);
	}
}

QColor LineStyleWidget::getColor(const QString &name, int shade)
{
	QColor tmpf;
	int h, s, v, sneu;
	colors[name].getRGBColor().rgb(&h, &s, &v);
	if ((h == s) && (s == v))
	{
		colors[name].getRGBColor().hsv(&h, &s, &v);
		sneu = 255 - ((255 - v) * shade / 100);
		tmpf.setHsv(h, s, sneu);
	}
	else
	{
		colors[name].getRGBColor().hsv(&h, &s, &v);
		sneu = s * shade / 100;
		tmpf.setHsv(h, sneu, v);
	}
	return tmpf;
}

LineStyleWidget::~LineStyleWidget()
{
	
}

/*** SMLineStyle *************************************************************/

SMLineStyle::SMLineStyle() : StyleItem(), doc_(0), twidget_(0)
{
	
}

QTabWidget* SMLineStyle::widget()
{
	if (!twidget_)
	{
		twidget_ = new QTabWidget();
		widget_ = new LineStyleWidget();
		twidget_->addTab(widget_, tr("Properties"));
	}
	return twidget_;
}

QString SMLineStyle::typeName()
{
	return tr("Line Styles");
}

void SMLineStyle::currentDoc(ScribusDoc *doc)
{
	doc_ = doc;
	if (doc_)
		tmpLines = doc_->MLineStyles;
}

QValueList<StyleName> SMLineStyle::styles()
{
	QValueList<StyleName> tmp;
	if (doc_) {
		QMap<QString,multiLine>::Iterator it;
		for (it = tmpLines.begin(); it != tmpLines.end(); ++it)
			tmp << StyleName(it.key(), QString::null);
	}
	return tmp;
}

void SMLineStyle::selected(const QStringList &styleNames)
{
	reset();

	if (styleNames.count() > 1) 
		setMultiSelection(styleNames);
	else if (styleNames.count() == 1)
		setSelection(styleNames[0]);
}

void SMLineStyle::reset()
{

}

void SMLineStyle::setSelection(const QString& styleName)
{
	if (!tmpLines.contains(styleName))
		return; // something's wrong here

	widget_->showStyle(tmpLines[styleName], doc_->PageColors);
}

void SMLineStyle::setMultiSelection(const QStringList& styles)
{
	
}

QString SMLineStyle::fromSelection() const
{
	Q_ASSERT(doc_ && doc_->m_Selection);
	QString lsName = QString::null;

	for (uint i = 0; i < doc_->m_Selection->count(); ++i)
	{
		PageItem *item = doc_->m_Selection->itemAt(i);
		QString tmpName = item->customLineStyle();
		if (lsName == QString::null && !tmpName.isEmpty() && tmpName != "")
		{
			lsName = item->customLineStyle();
		}
		else if (lsName != QString::null && !tmpName.isEmpty() && tmpName != "" && lsName != tmpName)
		{
			lsName = QString::null;
			break;
		}
	}
	return lsName;
}

void SMLineStyle::apply()
{
	
}

void SMLineStyle::deleteStyles(const QValueList<RemoveItem> &removeList)
{
	
}

void SMLineStyle::nameChanged(const QString &newName)
{

}

void SMLineStyle::languageChange()
{
	
}

SMLineStyle::~SMLineStyle()
{
	delete widget_;
	delete twidget_;
	widget_ = 0;
	twidget_ = 0;
}
