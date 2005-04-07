#include "tabruler.h"
#include "tabruler.moc"
#include <qvariant.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include "mspinbox.h"
#include <qtoolbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qpainter.h>
#include <qcursor.h>
#include <qcolor.h>
#include "units.h"
extern QPixmap loadIcon(QString nam);

RulerT::RulerT(QWidget *pa, int ein, QValueList<double> Tabs, bool ind, double wid) : QWidget(pa)
{
	setEraseColor(QColor(255,255,255));
	unitIndex = ein;
	tabValues = Tabs;
	haveInd = ind;
	offset = 0;
	firstLine = 0;
	leftIndent = 0;
	mousePressed = false;
	rulerCode = 0;
	setMouseTracking(true);
	if (wid < 0)
	{
		setMinimumSize(QSize(400,25));
		setMaximumSize(QSize(4000,25));
		Width = 4000;
		resize(400, 25);
	}
	else
	{
		Width = wid;
		setMinimumSize(QSize(QMIN(static_cast<int>(Width), 400),25));
		setMaximumSize(QSize(QMIN(static_cast<int>(Width), 400),25));
		resize(QMIN(static_cast<int>(wid), 400), 25);
	}
}

void RulerT::paintEvent(QPaintEvent *)
{
	double xl, iter, iter2;
	switch (unitIndex)
	{
		case 0:
			iter = 10.0;
	  		iter2 = 100.0;
			break;
		case 1:
			iter = (10.0 / 25.4) * 72.0;
  			iter2 = iter * 10.0;
			break;
		case 2:
			iter = 18.0;
			iter2 = 72.0;
			break;
		case 3:
			iter = 12.0;
			iter2 = 120.0;
			break;
	}
	QPainter p;
	p.begin(this);
	p.drawLine(0, 24, width(), 24);
	p.translate(-offset, 0);
	p.setBrush(black);
	p.setFont(font());
	p.setPen(QPen(black, 1, SolidLine, FlatCap, MiterJoin));
	for (xl = 0; xl < width()+offset; xl += iter)
	{
		if (xl < offset)
			continue;
		p.drawLine(qRound(xl), 18, qRound(xl), 24);
	}
	for (xl = 0; xl < width()+(iter2/2)+offset; xl += iter2)
	{
		if (xl < offset)
			continue;
		p.drawLine(qRound(xl), 11, qRound(xl), 24);
		switch (unitIndex)
		{
			case 2:
			{
				QString tx = "";
				int num1 = static_cast<int>(xl / iter2);
				if (num1 != 0)
					tx = QString::number(num1);
				double frac = (xl / iter2) - num1;
				if ((frac > 0.24) && (frac < 0.26))
					tx += QChar(0xBC);
				if ((frac > 0.49) && (frac < 0.51))
					tx += QChar(0xBD);
				if ((frac > 0.74) && (frac < 0.76))
					tx += QChar(0xBE);
				p.drawText(qRound(xl+2), 17, tx);
				break;
			}
			case 3:
				p.drawText(qRound(xl+2), 17, QString::number(xl / iter));
				break;
			default:
				p.drawText(qRound(xl+2), 17, QString::number(xl / iter * 10));
				break;
		}
	}
	if (tabValues.count() != 0)
	{
		for (int yg = 0; yg < static_cast<int>(tabValues.count()-1); yg += 2)
		{
			if (yg == actTab)
				p.setPen(QPen(red, 2, SolidLine, FlatCap, MiterJoin));
			else
				p.setPen(QPen(black, 2, SolidLine, FlatCap, MiterJoin));
			switch (static_cast<int>(tabValues[yg]))
			{
				case 0:
					p.drawLine(qRound(tabValues[yg+1]), 15, qRound(tabValues[yg+1]), 23);
					p.drawLine(qRound(tabValues[yg+1]), 23, qRound(tabValues[yg+1]+8), 23);
					break;
				case 1:
					p.drawLine(qRound(tabValues[yg+1]), 15, qRound(tabValues[yg+1]), 23);
					p.drawLine(qRound(tabValues[yg+1]), 23, qRound(tabValues[yg+1]-8), 23);
					break;
				case 2:
				case 3:
					p.drawLine(qRound(tabValues[yg+1]), 15, qRound(tabValues[yg+1]), 23);
					p.drawLine(qRound(tabValues[yg+1]-4), 23, qRound(tabValues[yg+1]+4), 23);
					p.drawLine(qRound(tabValues[yg+1]+3), 20, qRound(tabValues[yg+1]+2), 20);
					break;
				case 4:
					p.drawLine(qRound(tabValues[yg+1]), 15, qRound(tabValues[yg+1]), 23);
					p.drawLine(qRound(tabValues[yg+1]-4), 23, qRound(tabValues[yg+1]+4), 23);
					break;
				default:
					break;
			}
		}
	}
	if (haveInd)
	{
		p.setPen(QPen(blue, 1, SolidLine, FlatCap, MiterJoin));
		p.setBrush(blue);
		QPointArray cr;
		cr.setPoints(3, qRound(firstLine+leftIndent), 12, qRound(firstLine+leftIndent-4), 0, qRound(firstLine+leftIndent+4), 0);
		p.drawPolygon(cr);
		QPointArray cr2;
		cr2.setPoints(3, qRound(leftIndent), 12, qRound(leftIndent+4), 24, qRound(leftIndent-4), 24);
		p.drawPolygon(cr2);
	}
	p.end();
}

void RulerT::mousePressEvent(QMouseEvent *m)
{
	QRect fpo;
	mousePressed = true;
	rulerCode = 0;
	if (haveInd)
	{
		fpo = QRect(static_cast<int>(firstLine+leftIndent-offset)-4, 0, 8, 12);
		if (fpo.contains(m->pos()))
		{
			rulerCode = 1;
			mouseX = m->x();
			return;
		}
		fpo = QRect(static_cast<int>(leftIndent-offset)-4, 12, 8, 12);
		if (fpo.contains(m->pos()))
		{
			rulerCode = 2;
			mouseX = m->x();
			return;
		}
	}
	if (tabValues.count() != 0)
	{
		for (int yg = 0; yg < static_cast<int>(tabValues.count()-1); yg += 2)
		{
			fpo = QRect(static_cast<int>(tabValues[yg+1]-offset)-3, 15, 8, 8);
			if (fpo.contains(m->pos()))
			{
				rulerCode = 3;
				actTab = yg;
				mouseX = m->x();
				emit typeChanged(static_cast<int>(tabValues[actTab]));
				emit tabMoved(tabValues[actTab+1]);
				repaint();
				return;
			}
		}
	}
	if ((rulerCode == 0) && (m->button() == LeftButton))
	{
		tabValues.prepend(static_cast<double>(m->x() + offset));
		tabValues.prepend(0);
		actTab = 0;
		rulerCode = 3;
		updateTabList();
		repaint();
		emit newTab();
		emit typeChanged(static_cast<int>(tabValues[actTab]));
		emit tabMoved(tabValues[actTab+1]);
		qApp->setOverrideCursor(QCursor(SizeHorCursor), true);
	}
	mouseX = m->x();
}

void RulerT::mouseReleaseEvent(QMouseEvent *m)
{
	mousePressed = false;
	if ((m->y() < height()) && (m->y() > 0))
	{
		if (rulerCode == 3)
		{
			if (m->button() == RightButton)
			{
				tabValues[actTab] += 1.0;
				if (tabValues[actTab] > 4.0)
					tabValues[actTab] = 0.0;
				emit typeChanged(static_cast<int>(tabValues[actTab]));
				repaint();
			}
		}
	}
	else
	{
		if (rulerCode == 3)
		{
			QValueList<double>::Iterator it;
			it = tabValues.at(actTab);
			it = tabValues.remove(it);
			tabValues.remove(it);
			actTab = 0;
			if (tabValues.count() != 0)
			{
				emit typeChanged(static_cast<int>(tabValues[actTab]));
				emit tabMoved(tabValues[actTab+1]);
			}
			else
				emit noTabs();
			repaint();
			qApp->setOverrideCursor(QCursor(ArrowCursor), true);
		}
	}
	rulerCode = 0;
}

void RulerT::mouseMoveEvent(QMouseEvent *m)
{
	double oldInd;
	QRect fpo;
	if ((mousePressed) && (m->y() < height()) && (m->y() > 0) && (m->x() > 0) && (m->x() < width()))
	{
		qApp->setOverrideCursor(QCursor(SizeHorCursor), true);
		switch (rulerCode)
		{
			case 1:
				firstLine -= mouseX - m->x();
				if (firstLine+leftIndent+offset < offset)
					firstLine += mouseX - m->x();
				if (firstLine+leftIndent > Width)
					firstLine  = Width-leftIndent;
				emit firstLineMoved(firstLine);
				repaint();
				break;
			case 2:
				oldInd = leftIndent+firstLine;
				leftIndent -= mouseX - m->x();
				if (leftIndent < 0)
					leftIndent = 0;
				if (leftIndent > Width-1)
					leftIndent  = Width-1;
				firstLine = oldInd - leftIndent;
				emit leftIndentMoved(leftIndent);
				emit firstLineMoved(firstLine);
				repaint();
				break;
			case 3:
				tabValues[actTab+1] -= mouseX - m->x();
				if (tabValues[actTab+1] < 0)
					tabValues[actTab+1] = 0;
				if (tabValues[actTab+1] > Width-1)
					tabValues[actTab+1] = Width-1;
				updateTabList();
				emit tabMoved(tabValues[actTab+1]);
				repaint();
				break;
			default:
				break;
		}
		mouseX = m->x();
		return;
	}
	if ((!mousePressed) && (m->y() < height()) && (m->y() > 0) && (m->x() > 0) && (m->x() < width()))
	{
		qApp->setOverrideCursor(QCursor(loadIcon("tab.png"), 3), true);
		if (haveInd)
		{
			fpo = QRect(static_cast<int>(firstLine+leftIndent-offset)-4, 0, 8, 12);
			if (fpo.contains(m->pos()))
			{
				qApp->setOverrideCursor(QCursor(SizeHorCursor), true);
				return;
			}
			fpo = QRect(static_cast<int>(leftIndent-offset)-4, 12, 8, 12);
			if (fpo.contains(m->pos()))
			{
				qApp->setOverrideCursor(QCursor(SizeHorCursor), true);
				return;
			}
		}
		if (tabValues.count() != 0)
		{
			for (int yg = 0; yg < static_cast<int>(tabValues.count()-1); yg += 2)
			{
				fpo = QRect(static_cast<int>(tabValues[yg+1]-offset)-3, 15, 8, 8);
				if (fpo.contains(m->pos()))
				{
					qApp->setOverrideCursor(QCursor(SizeHorCursor), true);
					return;
				}
			}
		}
	}
}

void RulerT::leaveEvent(QEvent*)
{
	if ((mousePressed) && (rulerCode == 3))
	{
		qApp->setOverrideCursor(QCursor(loadIcon("DelPoint.png"), 4, 3), true);
		return;
	}
	else
		qApp->setOverrideCursor(QCursor(ArrowCursor), true);
}

void RulerT::updateTabList()
{
	QValueList<double>::Iterator it;
	double CurX = tabValues[actTab+1];
	int gg = static_cast<int>(tabValues.count()-1);
	int g = gg;
	double type = tabValues[actTab];
	it = tabValues.at(actTab);
	it = tabValues.remove(it);
	tabValues.remove(it);
	for (int yg = static_cast<int>(tabValues.count()-1); yg > 0; yg -= 2)
	{
		if (CurX < tabValues[yg])
			g = yg;
	}
	actTab = g-1;
	if (gg == g)
	{
		tabValues.append(type);
		tabValues.append(CurX);
		actTab = static_cast<int>(tabValues.count()-2);
	}
	else
	{
		it = tabValues.at(actTab);
		it = tabValues.insert(it, CurX);
		tabValues.insert(it, type);
	}
}

void RulerT::increaseOffset()
{
	offset += 5;
	if (offset + width() > static_cast<int>(Width))
		offset -= 5;
	repaint();
}

void RulerT::decreaseOffset()
{
	offset -= 5;
	if (offset < 0)
		offset = 0;
	repaint();
}

void RulerT::changeTab(int t)
{
	tabValues[actTab] = static_cast<double>(t);
	repaint();
}

void RulerT::moveTab(double t)
{
	tabValues[actTab+1] = t;
	updateTabList();
	repaint();
}

void RulerT::moveFirstLine(double t)
{
	firstLine = t;
	if (firstLine+leftIndent+offset < offset)
	{
		firstLine = 0-leftIndent;
		emit firstLineMoved(firstLine);
	}
	if (firstLine+leftIndent > Width)
	{
		firstLine = Width-leftIndent;
		emit firstLineMoved(firstLine);
	}
	repaint();
}

void RulerT::moveLeftIndent(double t)
{
	double oldInd = leftIndent+firstLine;
	leftIndent = t;
	if (leftIndent > Width-1)
	{
		leftIndent  = Width-1;
		emit leftIndentMoved(leftIndent);
	}
	firstLine = oldInd - leftIndent;
	emit firstLineMoved(firstLine);
	repaint();
}

Tabruler::Tabruler( QWidget* parent, bool haveFirst, int dEin, QValueList<double> Tabs, double wid ) : QWidget( parent )
{
	docUnitRatio=unitGetRatioFromIndex(dEin);
	double ww;
	ww = (wid < 0) ? 4000 : wid;
	setName( "tabruler" );
	tabrulerLayout = new QVBoxLayout( this, 0, 6, "tabrulerLayout");
	layout2 = new QHBoxLayout( 0, 0, 6, "layout2");
	TypeCombo = new QComboBox( false, this, "TypeCombo" );
	TypeCombo->clear();
	TypeCombo->insertItem( tr( "Left" ) );
	TypeCombo->insertItem( tr( "Right" ) );
	TypeCombo->insertItem( tr( "Full Stop" ) );
	TypeCombo->insertItem( tr( "Comma" ) );
	TypeCombo->insertItem( tr( "Center" ) );
	layout2->addWidget( TypeCombo );
	rulerScrollL = new QToolButton( LeftArrow, this, "rulerScrollL" );
	rulerScrollL->setAutoRepeat( true );
	layout2->addWidget( rulerScrollL );
	ruler = new RulerT( this, dEin, Tabs, haveFirst, wid );
	ruler->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)5, 0, 0, ruler->sizePolicy().hasHeightForWidth() ) );
	layout2->addWidget( ruler );
	rulerScrollR = new QToolButton( RightArrow, this, "RulserScrollR" );
	rulerScrollR->setAutoRepeat( true );
	layout2->addWidget( rulerScrollR );
	tabrulerLayout->addLayout( layout2 );
	layout1 = new QHBoxLayout( 0, 0, 6, "layout1");
	tabData = new MSpinBox( 0, ww, this, 1 );
	tabData->setValue(0);
	positionLabel = new QLabel( tabData, tr("&Position:"), this, "positionLabel" );
	layout1->addWidget( positionLabel );
	layout1->addWidget( tabData );
	if (haveFirst)
	{
		firstLineData = new MSpinBox( -3000, ww, this, 1);
		firstLineData->setValue(0);
		firstLineLabel = new QLabel( firstLineData, tr( "First &Line:"), this, "firstLineLabel" );
		layout1->addWidget( firstLineLabel );
		layout1->addWidget( firstLineData );
		leftIndentData = new MSpinBox( 0, ww, this, 1 );
		leftIndentData->setValue(0);
		leftIndentLabel = new QLabel( leftIndentData, tr( "Left Ind&ent:" ), this, "leftIndentLabel" );
		layout1->addWidget( leftIndentLabel );
		layout1->addWidget( leftIndentData );
	}
	clearButton = new QPushButton( this, "clearButton" );
	clearButton->setText( tr( "Delete All" ) );
	layout1->addWidget( clearButton );
	if (!haveFirst)
	{
		QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
		layout1->addItem( spacer );
	}
	tabrulerLayout->addLayout( layout1 );
	if (Tabs.count() == 0)
	{
		TypeCombo->setEnabled(false);
		tabData->setEnabled(false);
		clearButton->setEnabled(false);
	}
	resize( minimumSizeHint() );
	connect(rulerScrollL, SIGNAL(clicked()), ruler, SLOT(decreaseOffset()));
	connect(rulerScrollR, SIGNAL(clicked()), ruler, SLOT(increaseOffset()));
	connect(ruler, SIGNAL(typeChanged(int)) , this, SLOT(setTabType(int)));
	connect(TypeCombo, SIGNAL(activated(int)), this, SLOT(setType()));
	connect(ruler, SIGNAL(tabMoved(double)) , this, SLOT(setTabData(double)));
	connect(ruler, SIGNAL(newTab()), this, SLOT(tabAdded()));
	connect(ruler, SIGNAL(noTabs()), this, SLOT(lastTabRemoved()));
	connect(tabData, SIGNAL(valueChanged(int)), this, SLOT(setTab()));
	connect(clearButton, SIGNAL(clicked()), this, SLOT(clearAll()));
	if (haveFirst)
	{
		connect(ruler, SIGNAL(firstLineMoved(double)) , this, SLOT(setFirstLineData(double)));
		connect(ruler, SIGNAL(leftIndentMoved(double)) , this, SLOT(setLeftIndentData(double)));
		connect(firstLineData, SIGNAL(valueChanged(int)), this, SLOT(setFirstLine()));
		connect(leftIndentData, SIGNAL(valueChanged(int)), this, SLOT(setLeftIndent()));
		QToolTip::add( firstLineData, tr( "Indentation for first line of the paragraph" ) );
		QToolTip::add( leftIndentData, tr( "Indentation from the left for the whole paragraph" ) );
	}
	QToolTip::add( clearButton, tr( "Delete all Tabulators" ) );
	QString ein = unitGetSuffixFromIndex(dEin);
	if (dEin == 2)
	{
		if (haveFirst)
		{
			firstLineData->setDecimals(10000);
			leftIndentData->setDecimals(10000);
		}
		tabData->setDecimals(10000);
	}
	if (haveFirst)
	{
		firstLineData->setSuffix(ein);
		leftIndentData->setSuffix(ein);
	}
	tabData->setSuffix(ein);
	haveF = haveFirst;
}

void Tabruler::clearAll()
{
	ruler->tabValues.clear();
	ruler->repaint();
	lastTabRemoved();
}

void Tabruler::tabAdded()
{
	TypeCombo->setEnabled(true);
	tabData->setEnabled(true);
	clearButton->setEnabled(true);
}

void Tabruler::lastTabRemoved()
{
	TypeCombo->setEnabled(false);
	tabData->setEnabled(false);
	clearButton->setEnabled(false);
}

void Tabruler::setTabType(int t)
{
	TypeCombo->setCurrentItem(t);
}

void Tabruler::setType()
{
	disconnect(TypeCombo, SIGNAL(activated(int)), this, SLOT(setType()));
	ruler->changeTab(TypeCombo->currentItem());
	connect(TypeCombo, SIGNAL(activated(int)), this, SLOT(setType()));
}

void Tabruler::setTabData(double t)
{
	disconnect(tabData, SIGNAL(valueChanged(int)), this, SLOT(setTab()));
	tabData->setValue(t * docUnitRatio);
	connect(tabData, SIGNAL(valueChanged(int)), this, SLOT(setTab()));
}

void Tabruler::setTab()
{
	ruler->moveTab(tabData->value() / docUnitRatio);
}

void Tabruler::setFirstLineData(double t)
{
	disconnect(firstLineData, SIGNAL(valueChanged(int)), this, SLOT(setFirstLine()));
	firstLineData->setValue(t * docUnitRatio);
	connect(firstLineData, SIGNAL(valueChanged(int)), this, SLOT(setFirstLine()));
}

void Tabruler::setFirstLine()
{
	ruler->moveFirstLine(firstLineData->value() / docUnitRatio);
}

void Tabruler::setLeftIndentData(double t)
{
	disconnect(leftIndentData, SIGNAL(valueChanged(int)), this, SLOT(setLeftIndent()));
	leftIndentData->setValue(t * docUnitRatio);
	connect(leftIndentData, SIGNAL(valueChanged(int)), this, SLOT(setLeftIndent()));
}

void Tabruler::setLeftIndent()
{
	ruler->moveLeftIndent(leftIndentData->value() / docUnitRatio);
}

QValueList<double> Tabruler::getTabVals()
{
	return ruler->tabValues;
}

double Tabruler::getFirstLine()
{
	return firstLineData->value() / docUnitRatio;
}

double Tabruler::getLeftIndent()
{
	return leftIndentData->value() / docUnitRatio;
}
