/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "tabruler.h"
//#include "tabruler.moc"
#include <qvariant.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qtoolbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <q3whatsthis.h>
#include <qpainter.h>
#include <qcursor.h>
#include <qcolor.h>
//Added by qt3to4:
#include <QApplication>
#include <Q3HBoxLayout>
#include <Q3ValueList>
#include <Q3PointArray>
#include <QPixmap>
#include <QMouseEvent>
#include <QEvent>
#include <Q3VBoxLayout>
#include <QPaintEvent>
#include "commonstrings.h"
#include "units.h"
#include "scribusstructs.h"
#include "scrspinbox.h"

extern QPixmap loadIcon(QString nam);

RulerT::RulerT(QWidget *pa, int ein, Q3ValueList<ParagraphStyle::TabRecord> Tabs, bool ind, double wid) : QWidget(pa)
{
	setEraseColor(QColor(255,255,255));
	unitIndex = ein;
	iter=unitRulerGetIter1FromIndex(unitIndex);
	iter2=unitRulerGetIter2FromIndex(unitIndex);
	tabValues = Tabs;
	haveInd = ind;
	offsetIncrement = 5;
	offset = 0;
	firstLine = 0;
	leftIndent = 0;
	mousePressed = false;
	rulerCode = 0;
	actTab = -1;
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
		setMinimumSize(QSize(qMin(static_cast<int>(Width), 400),25));
		setMaximumSize(QSize(4000,25));
		resize(qMin(static_cast<int>(wid), 400), 25);
	}
}

void RulerT::setTabs(Q3ValueList<ParagraphStyle::TabRecord> Tabs, int dEin)
{
	unitIndex = dEin;
	iter=unitRulerGetIter1FromIndex(unitIndex);
	iter2=unitRulerGetIter2FromIndex(unitIndex);
	tabValues = Tabs;
	repaint();
}

void RulerT::paintEvent(QPaintEvent *)
{
	double xl;
	QPainter p;
	p.begin(this);
	p.drawLine(0, 24, width(), 24);
	p.translate(-offset, 0);
	p.setBrush(Qt::black);
	p.setFont(font());
	p.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
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
		for (int yg = 0; yg < static_cast<int>(tabValues.count()); yg++)
		{
			if (yg == actTab)
				p.setPen(QPen(Qt::red, 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
			else
				p.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
			switch (static_cast<int>(tabValues[yg].tabType))
			{
				case 0:
					p.drawLine(qRound(tabValues[yg].tabPosition), 15, qRound(tabValues[yg].tabPosition), 23);
					p.drawLine(qRound(tabValues[yg].tabPosition), 23, qRound(tabValues[yg].tabPosition+8), 23);
					break;
				case 1:
					p.drawLine(qRound(tabValues[yg].tabPosition), 15, qRound(tabValues[yg].tabPosition), 23);
					p.drawLine(qRound(tabValues[yg].tabPosition), 23, qRound(tabValues[yg].tabPosition-8), 23);
					break;
				case 2:
				case 3:
					p.drawLine(qRound(tabValues[yg].tabPosition), 15, qRound(tabValues[yg].tabPosition), 23);
					p.drawLine(qRound(tabValues[yg].tabPosition-4), 23, qRound(tabValues[yg].tabPosition+4), 23);
					p.drawLine(qRound(tabValues[yg].tabPosition+3), 20, qRound(tabValues[yg].tabPosition+2), 20);
					break;
				case 4:
					p.drawLine(qRound(tabValues[yg].tabPosition), 15, qRound(tabValues[yg].tabPosition), 23);
					p.drawLine(qRound(tabValues[yg].tabPosition-4), 23, qRound(tabValues[yg].tabPosition+4), 23);
					break;
				default:
					break;
			}
		}
	}
	if (haveInd)
	{
		p.setPen(QPen(Qt::blue, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
		p.setBrush(Qt::blue);
		Q3PointArray cr;
		cr.setPoints(3, qRound(firstLine+leftIndent), 12, qRound(firstLine+leftIndent-4), 0, qRound(firstLine+leftIndent+4), 0);
		p.drawPolygon(cr);
		Q3PointArray cr2;
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
		for (int yg = 0; yg < static_cast<int>(tabValues.count()); yg++)
		{
			fpo = QRect(static_cast<int>(tabValues[yg].tabPosition-offset)-3, 15, 8, 8);
			if (fpo.contains(m->pos()))
			{
				rulerCode = 3;
				actTab = yg;
				mouseX = m->x();
				emit tabSelected();
				emit typeChanged(tabValues[actTab].tabType);
				emit tabMoved(tabValues[actTab].tabPosition);
				emit fillCharChanged(tabValues[actTab].tabFillChar);
				repaint();
				return;
			}
		}
	}
	if ((rulerCode == 0) && (m->button() == Qt::LeftButton))
	{
		ParagraphStyle::TabRecord tb;
		tb.tabPosition = static_cast<double>(m->x() + offset);
		tb.tabType = 0;
		tb.tabFillChar = QChar();
		tabValues.prepend(tb);
		actTab = 0;
		rulerCode = 3;
		updateTabList();
		repaint();
		emit newTab();
		emit typeChanged(tabValues[actTab].tabType);
		emit tabMoved(tabValues[actTab].tabPosition);
		emit fillCharChanged(tabValues[actTab].tabFillChar);
		qApp->setOverrideCursor(QCursor(Qt::SizeHorCursor), true);
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
			if (m->button() == Qt::RightButton)
			{
				tabValues[actTab].tabType += 1;
				if (tabValues[actTab].tabType > 4)
					tabValues[actTab].tabType = 0;
				emit typeChanged(tabValues[actTab].tabType);
				repaint();
			}
		}
	}
	else
	{
		if (rulerCode == 3)
		{
			Q3ValueList<ParagraphStyle::TabRecord>::Iterator it;
			it = tabValues.at(actTab);
			tabValues.remove(it);
			actTab = 0;
			if (tabValues.count() != 0)
			{
				emit typeChanged(tabValues[actTab].tabType);
				emit tabMoved(tabValues[actTab].tabPosition);
				emit fillCharChanged(tabValues[actTab].tabFillChar);
			}
			else
				emit noTabs();
			repaint();
			qApp->setOverrideCursor(QCursor(Qt::ArrowCursor), true);
		}
	}
	rulerCode = 0;
	emit mouseReleased();
}

void RulerT::mouseMoveEvent(QMouseEvent *m)
{
	double oldInd;
	QRect fpo;
	if ((mousePressed) && (m->y() < height()) && (m->y() > 0) && (m->x() > 0) && (m->x() < width()))
	{
		qApp->setOverrideCursor(QCursor(Qt::SizeHorCursor), true);
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
				tabValues[actTab].tabPosition -= mouseX - m->x();
				if (tabValues[actTab].tabPosition < 0)
					tabValues[actTab].tabPosition = 0;
				if (tabValues[actTab].tabPosition > Width-1)
					tabValues[actTab].tabPosition = Width-1;
				updateTabList();
				emit tabMoved(tabValues[actTab].tabPosition);
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
				qApp->setOverrideCursor(QCursor(Qt::SizeHorCursor), true);
				return;
			}
			fpo = QRect(static_cast<int>(leftIndent-offset)-4, 12, 8, 12);
			if (fpo.contains(m->pos()))
			{
				qApp->setOverrideCursor(QCursor(Qt::SizeHorCursor), true);
				return;
			}
		}
		if (tabValues.count() != 0)
		{
			for (int yg = 0; yg < static_cast<int>(tabValues.count()); yg++)
			{
				fpo = QRect(static_cast<int>(tabValues[yg].tabPosition-offset)-3, 15, 8, 8);
				if (fpo.contains(m->pos()))
				{
					qApp->setOverrideCursor(QCursor(Qt::SizeHorCursor), true);
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
		qApp->setOverrideCursor(QCursor(loadIcon("DelPoint.png"), 1, 1), true);
		return;
	}
	else
		qApp->setOverrideCursor(QCursor(Qt::ArrowCursor), true);
}

void RulerT::updateTabList()
{
	ParagraphStyle::TabRecord tb;
	tb.tabPosition = tabValues[actTab].tabPosition;
	tb.tabType = tabValues[actTab].tabType;
	tb.tabFillChar =  tabValues[actTab].tabFillChar;
	Q3ValueList<ParagraphStyle::TabRecord>::Iterator it;
	int gg = static_cast<int>(tabValues.count()-1);
	int g = gg;
	it = tabValues.at(actTab);
	tabValues.remove(it);
	for (int yg = static_cast<int>(tabValues.count()); yg > -1; yg--)
	{
		if (tb.tabPosition < tabValues[yg].tabPosition)
			g = yg;
	}
	actTab = g;
	if (gg == g)
	{
		tabValues.append(tb);
		actTab = static_cast<int>(tabValues.count()-1);
	}
	else
	{
		it = tabValues.at(actTab);
		tabValues.insert(it, tb);
	}
}

void RulerT::resetOffsetInc()
{
	offsetIncrement = 5;
}

void RulerT::increaseOffset()
{
	offset += offsetIncrement;
	offsetIncrement++;
	if (offsetIncrement > 30)
		offsetIncrement = 30;
	if (offset + width() > static_cast<int>(Width))
		offset -= 5;
	repaint();
}

void RulerT::decreaseOffset()
{
	offset -= offsetIncrement;
	offsetIncrement++;
	if (offsetIncrement > 30)
		offsetIncrement = 30;
	if (offset < 0)
		offset = 0;
	repaint();
}

void RulerT::changeTab(int t)
{
	tabValues[actTab].tabType = t;
	repaint();
}

void RulerT::changeTabChar(QChar t)
{
	tabValues[actTab].tabFillChar = t;
	repaint();
}

void RulerT::moveTab(double t)
{
	tabValues[actTab].tabPosition = t;
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

Tabruler::Tabruler( QWidget* parent, bool haveFirst, int dEin, Q3ValueList<ParagraphStyle::TabRecord> Tabs, double wid ) : QWidget( parent )
{
	docUnitRatio=unitGetRatioFromIndex(dEin);
	double ww;
	ww = (wid < 0) ? 4000 : wid;
	setName( "tabruler" );
	tabrulerLayout = new Q3VBoxLayout( this, 0, 6, "tabrulerLayout");
	layout2 = new Q3HBoxLayout( 0, 0, 6, "layout2");

	rulerScrollL = new QToolButton( Qt::LeftArrow, this, "rulerScrollL" );
	rulerScrollL->setAutoRepeat( true );
	layout2->addWidget( rulerScrollL );
	ruler = new RulerT( this, dEin, Tabs, haveFirst, wid );
	ruler->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)5, 0, 0, ruler->sizePolicy().hasHeightForWidth() ) );
	layout2->addWidget( ruler );
	rulerScrollR = new QToolButton( Qt::RightArrow, this, "RulserScrollR" );
	rulerScrollR->setAutoRepeat( true );
	layout2->addWidget( rulerScrollR );

	layout1 = new Q3HBoxLayout( 0, 0, 6, "layout1" );
	layout1->setAlignment( Qt::AlignTop );
	TypeCombo = new QComboBox( false, this, "TypeCombo" );
	TypeCombo->clear();
	TypeCombo->insertItem( tr( "Left" ) );
	TypeCombo->insertItem( tr( "Right" ) );
	TypeCombo->insertItem( tr( "Full Stop" ) );
	TypeCombo->insertItem( tr( "Comma" ) );
	TypeCombo->insertItem( tr( "Center" ) );
	layout1->addWidget( TypeCombo );
	tabData = new ScrSpinBox( 0, ww / docUnitRatio, this, 1 );
	tabData->setValue(0);
	positionLabel = new QLabel( tabData, tr("&Position:"), this, "positionLabel" );
	layout1->addWidget( positionLabel );
	layout1->addWidget( tabData );
	tabFillCombo = new QComboBox( true, this, "tabFillCombo" );
	tabFillCombo->setEditable(false);
	tabFillCombo->insertItem( tr("None", "tab fill"));
	tabFillCombo->insertItem( tr("Dot"));
	tabFillCombo->insertItem( tr("Hyphen"));
	tabFillCombo->insertItem( tr("Underscore"));
	tabFillCombo->insertItem( tr("Custom"));
	tabFillComboT = new QLabel(tabFillCombo, tr( "Fill Char:" ), this, "tabFillComboT" );
	layout1->addWidget( tabFillComboT );
	layout1->addWidget( tabFillCombo );

	layout4 = new Q3HBoxLayout(0, 0, 6, "layout3");

	indentLayout = new Q3HBoxLayout(0, 0, 6, "indentLayout");
	if (haveFirst)
	{
		firstLineData = new ScrSpinBox( -3000, ww / docUnitRatio, this, 1);
		firstLineData->setValue(0);
		firstLineLabel = new QLabel( "", this, "firstLineLabel" );
		firstLineLabel->setText("");
		firstLineLabel->setPixmap(loadIcon("firstline.png"));
		indentLayout->addWidget( firstLineLabel );
		indentLayout->addWidget( firstLineData );
		leftIndentData = new ScrSpinBox( 0, ww / docUnitRatio, this, 1 );
		leftIndentData->setValue(0);
		leftIndentLabel = new QLabel( "", this, "leftIndentLabel" );
		leftIndentLabel->setText("");
		leftIndentLabel->setPixmap(loadIcon("leftindent.png"));
		layout4->addWidget( leftIndentLabel );
		layout4->addWidget( leftIndentData );
		layout4->addStretch(10);
		rightIndentLabel = new QLabel("", this, "rightIndentLabel");
		rightIndentLabel->setText("");
		rightIndentLabel->setPixmap(loadIcon("rightindent.png"));
		rightIndentData = new ScrSpinBox(0, ww / docUnitRatio, this, 1);
		rightIndentData->setValue(0);
		indentLayout->addWidget(rightIndentLabel);
		indentLayout->addWidget(rightIndentData);
	}
	clearButton = new QPushButton( this, "clearButton" );
	clearButton->setText( tr( "Delete All" ) );
	indentLayout->addSpacing(20);
	indentLayout->addWidget( clearButton);
	indentLayout->addStretch(10);
	if (!haveFirst)
	{
		QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
		layout1->addItem( spacer );
	}
	
	layout1->addStretch( 10 );
	tabrulerLayout->addLayout( layout1 );
	tabrulerLayout->addLayout( layout2 );
	indentLayout->addStretch( 10 );
	tabrulerLayout->addLayout( indentLayout );
	tabrulerLayout->addLayout(layout4);

	TypeCombo->setEnabled(false);
	tabData->setEnabled(false);
	tabFillCombo->setEnabled(false);
	if (Tabs.count() == 0)
		clearButton->setEnabled(false);
	resize( minimumSizeHint() );
	connect(rulerScrollL, SIGNAL(clicked()), ruler, SLOT(decreaseOffset()));
	connect(rulerScrollR, SIGNAL(clicked()), ruler, SLOT(increaseOffset()));
	connect(rulerScrollL, SIGNAL(released()), this, SLOT(resetOFfL()));
	connect(rulerScrollR, SIGNAL(released()), this, SLOT(resetOFfR()));
	connect(ruler, SIGNAL(typeChanged(int)) , this, SLOT(setTabType(int)));
	connect(ruler, SIGNAL(fillCharChanged(QChar)) , this, SLOT(setTabFillChar(QChar)));
	connect(TypeCombo, SIGNAL(activated(int)), this, SLOT(setType()));
	connect(tabFillCombo, SIGNAL(activated(int)), this, SLOT(setFillChar()));
	connect(tabFillCombo, SIGNAL(textChanged(const QString &)), this, SLOT(setCustomFillChar(const QString &)));
	connect(ruler, SIGNAL(tabMoved(double)) , this, SLOT(setTabData(double)));
	connect(ruler, SIGNAL(tabSelected()), this, SLOT(tabAdded()));
	connect(ruler, SIGNAL(newTab()), this, SLOT(tabAdded()));
	connect(ruler, SIGNAL(noTabs()), this, SLOT(lastTabRemoved()));
	connect(tabData, SIGNAL(valueChanged(double)), this, SLOT(setTab()));
	connect(clearButton, SIGNAL(clicked()), this, SLOT(clearAll()));

	QToolTip::add( tabFillCombo, tr( "Fill Character of Tab" ) );
	QToolTip::add( TypeCombo, tr( "Type/Orientation of Tab" ) );
	QToolTip::add( tabData, tr( "Position of Tab" ) );

	if (haveFirst)
	{
		connect(ruler, SIGNAL(firstLineMoved(double)) , this, SLOT(setFirstLineData(double)));
		connect(ruler, SIGNAL(leftIndentMoved(double)) , this, SLOT(setLeftIndentData(double)));
		connect(ruler, SIGNAL(mouseReleased()), this, SIGNAL(tabrulerChanged()));
		connect(ruler, SIGNAL(mouseReleased()), this, SLOT(slotMouseReleased()));
		connect(firstLineData, SIGNAL(valueChanged(double)), this, SLOT(setFirstLine()));
		connect(leftIndentData, SIGNAL(valueChanged(double)), this, SLOT(setLeftIndent()));
		connect(rightIndentData, SIGNAL(valueChanged(double)), this, SLOT(setRightIndent()));
		QToolTip::add( firstLineData, tr( "Indentation for first line of the paragraph" ) );
		QToolTip::add( leftIndentData, tr( "Indentation from the left for the whole paragraph" ) );
		QToolTip::add( rightIndentData, tr( "Indentation from the right for the whole paragraph" ) );
	}
	QToolTip::add( clearButton, tr( "Delete all Tabulators" ) );
	QString ein = unitGetSuffixFromIndex(dEin);
	if (dEin == 2)
	{
		if (haveFirst)
		{
			firstLineData->setDecimals(4);
			leftIndentData->setDecimals(4);
			rightIndentData->setDecimals(4);
		}
		tabData->setDecimals(4);
	}
	if (haveFirst)
	{
		firstLineData->setSuffix(ein);
		leftIndentData->setSuffix(ein);
		rightIndentData->setSuffix(ein);
	}
	tabData->setSuffix(ein);
	haveF = haveFirst;
}

void Tabruler::setTabs(Q3ValueList<ParagraphStyle::TabRecord> Tabs, int dEin)
{
	docUnitRatio=unitGetRatioFromIndex(dEin);
	QString ein = unitGetSuffixFromIndex(dEin);
	tabData->setSuffix(ein);
	if (haveF)
	{
		firstLineData->setSuffix(ein);
		leftIndentData->setSuffix(ein);
		rightIndentData->setSuffix(ein);
	}
	ruler->setTabs(Tabs, dEin);
	if (Tabs.count() == 0)
		clearButton->setEnabled(false);
}

void Tabruler::resetOFfL()
{
	if (!rulerScrollL->isDown())
		ruler->resetOffsetInc();
}

void Tabruler::resetOFfR()
{
	if (!rulerScrollR->isDown())
		ruler->resetOffsetInc();
}

void Tabruler::clearAll()
{
	ruler->tabValues.clear();
	ruler->repaint();
	lastTabRemoved();
	emit tabrulerChanged();
	emit tabsChanged();
}

void Tabruler::tabAdded()
{
	TypeCombo->setEnabled(true);
	tabData->setEnabled(true);
	clearButton->setEnabled(true);
	tabFillCombo->setEnabled(true);
	emit tabrulerChanged();
	emit tabsChanged();
}

void Tabruler::lastTabRemoved()
{
	TypeCombo->setEnabled(false);
	tabData->setEnabled(false);
	clearButton->setEnabled(false);
	tabFillCombo->setEnabled(false);
	emit tabrulerChanged();
	emit tabsChanged();
}

void Tabruler::setFillChar()
{
	disconnect(tabFillCombo, SIGNAL(textChanged(const QString &)), this, SLOT(setCustomFillChar(const QString &)));
	disconnect(tabFillCombo, SIGNAL(activated(int)), this, SLOT(setFillChar()));
	QChar ret;
	switch (tabFillCombo->currentItem())
	{
		case 0:
			tabFillCombo->setEditable(false);
			ret = QChar();
			break;
		case 1:
			tabFillCombo->setEditable(false);
			ret = '.';
			break;
		case 2:
			tabFillCombo->setEditable(false);
			ret = '-';
			break;
		case 3:
			tabFillCombo->setEditable(false);
			ret = '_';
			break;
		case 4:
			tabFillCombo->setEditable(true);
			tabFillCombo->setEditText(CommonStrings::trCustomTabFill);
			break;
	}
	if (tabFillCombo->currentItem() != 4)
		ruler->changeTabChar(ret);
	connect(tabFillCombo, SIGNAL(activated(int)), this, SLOT(setFillChar()));
	connect(tabFillCombo, SIGNAL(textChanged(const QString &)), this, SLOT(setCustomFillChar(const QString &)));
}

void Tabruler::setCustomFillChar(const QString &txt)
{
	if (txt == CommonStrings::trCustomTabFill)
		return;
	disconnect(tabFillCombo, SIGNAL(textChanged(const QString &)), this, SLOT(setCustomFillChar(const QString &)));
	disconnect(tabFillCombo, SIGNAL(activated(int)), this, SLOT(setFillChar()));
	QChar ret = txt[txt.length()-1];
	ruler->changeTabChar(ret);
	connect(tabFillCombo, SIGNAL(activated(int)), this, SLOT(setFillChar()));
	connect(tabFillCombo, SIGNAL(textChanged(const QString &)), this, SLOT(setCustomFillChar(const QString &)));
}

void Tabruler::setTabFillChar(QChar t)
{
	if (t.isNull())
	{
		tabFillCombo->setEditable(false);
		tabFillCombo->setCurrentItem(0);
	}
	else if (t == '.')
	{
		tabFillCombo->setEditable(false);
		tabFillCombo->setCurrentItem(1);
	}
	else if (t == '-')
	{
		tabFillCombo->setEditable(false);
		tabFillCombo->setCurrentItem(2);
	}
	else if (t == '_')
	{
		tabFillCombo->setEditable(false);
		tabFillCombo->setCurrentItem(3);
	}
	else
	{
		tabFillCombo->setCurrentItem(4);
		tabFillCombo->setEditable(true);
		tabFillCombo->setEditText(CommonStrings::trCustomTabFill+QString(t));
	}
	emit tabrulerChanged();
	emit tabsChanged();
}

void Tabruler::setTabType(int t)
{
	TypeCombo->setCurrentItem(t);
	emit tabrulerChanged();
	emit tabsChanged();
}

void Tabruler::setType()
{
	disconnect(TypeCombo, SIGNAL(activated(int)), this, SLOT(setType()));
	ruler->changeTab(TypeCombo->currentItem());
	connect(TypeCombo, SIGNAL(activated(int)), this, SLOT(setType()));
	emit tabrulerChanged();
	emit tabsChanged();
}

void Tabruler::setTabData(double t)
{
	disconnect(tabData, SIGNAL(valueChanged(double)), this, SLOT(setTab()));
	tabData->setValue(t * docUnitRatio);
	connect(tabData, SIGNAL(valueChanged(double)), this, SLOT(setTab()));
	if (!ruler->mousePressed)
	{
		emit tabrulerChanged();
		emit tabsChanged();
	}
}

void Tabruler::setTab()
{
	ruler->moveTab(tabData->value() / docUnitRatio);
	emit tabrulerChanged();
	emit tabsChanged();
}

void Tabruler::setFirstLineData(double t)
{
	disconnect(firstLineData, SIGNAL(valueChanged(double)), this, SLOT(setFirstLine()));
	firstLineData->setValue(t * docUnitRatio);
	connect(firstLineData, SIGNAL(valueChanged(double)), this, SLOT(setFirstLine()));
	if (!ruler->mousePressed)
	{
		emit tabrulerChanged();
		double a, b, value;
		int c;
		firstLineData->getValues(&a, &b, &c, &value);
		emit firstLineChanged(value);
	}
}

void Tabruler::setFirstLine()
{
	ruler->moveFirstLine(firstLineData->value() / docUnitRatio);
	emit tabrulerChanged();
	double a, b, value;
	int c;
	firstLineData->getValues(&a, &b, &c, &value);
	emit firstLineChanged(value);
}

void Tabruler::setLeftIndentData(double t)
{
	disconnect(leftIndentData, SIGNAL(valueChanged(double)), this, SLOT(setLeftIndent()));
	leftIndentData->setValue(t * docUnitRatio);
	connect(leftIndentData, SIGNAL(valueChanged(double)), this, SLOT(setLeftIndent()));
	if (!ruler->mousePressed)
	{
		emit tabrulerChanged();
		double a, b, value;
		int c;
		leftIndentData->getValues(&a, &b, &c, &value);
		emit leftIndentChanged(value);
	}
}

void Tabruler::setLeftIndent()
{
	ruler->moveLeftIndent(leftIndentData->value() / docUnitRatio);
	emit tabrulerChanged();
	double a, b, value;
	int c;
	leftIndentData->getValues(&a, &b, &c, &value);
	emit leftIndentChanged(value);
}

Q3ValueList<ParagraphStyle::TabRecord> Tabruler::getTabVals()
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

void Tabruler::setRightIndentData(double t)
{
	disconnect(rightIndentData, SIGNAL(valueChanged(double)), this, SLOT(setRightIndent()));
	rightIndentData->setValue(t * docUnitRatio);
	connect(rightIndentData, SIGNAL(valueChanged(double)), this, SLOT(setRightIndent()));
	if (!ruler->mousePressed)
	{
		emit tabrulerChanged();
		double a, b, value;
		int c;
		rightIndentData->getValues(&a, &b, &c, &value);
		emit rightIndentChanged(value);
	}
}

void Tabruler::setRightIndent()
{
	emit tabrulerChanged();
	double a, b, value;
	int c;
	rightIndentData->getValues(&a, &b, &c, &value);
	emit rightIndentChanged(value);
}

double Tabruler::getRightIndent()
{
	return rightIndentData->value() / docUnitRatio;
}

void Tabruler::slotMouseReleased()
{
	emit mouseReleased();
}
