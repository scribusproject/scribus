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
extern QPixmap loadIcon(QString nam);
extern double UmReFaktor;

RulerT::RulerT(QWidget *pa, int ein, QValueList<double> Tabs, bool ind, double wid) : QWidget(pa)
{
	setEraseColor(QColor(255,255,255));
	Einheit = ein;
	TabValues = Tabs;
	haveInd = ind;
	Offset = 0;
	First = 0;
	Indent = 0;
	Mpressed = false;
	RulerCode = 0;
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
	switch (Einheit)
	{
		case 0:
			iter = 10.0;
	  		iter2 = iter * 10.0;
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
	p.translate(-Offset, 0);
	p.setBrush(black);
	p.setFont(font());
	p.setPen(QPen(black, 1, SolidLine, FlatCap, MiterJoin));
	for (xl = 0; xl < width()+Offset; xl += iter)
	{
		if (xl < Offset)
			continue;
		p.drawLine(qRound(xl), 18, qRound(xl), 24);
	}
	for (xl = 0; xl < width()+(iter2/2)+Offset; xl += iter2)
	{
		if (xl < Offset)
			continue;
		p.drawLine(qRound(xl), 11, qRound(xl), 24);
		switch (Einheit)
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
	if (TabValues.count() != 0)
	{
		for (int yg = 0; yg < static_cast<int>(TabValues.count()-1); yg += 2)
		{
			if (yg == ActTab)
				p.setPen(QPen(red, 2, SolidLine, FlatCap, MiterJoin));
			else
				p.setPen(QPen(black, 2, SolidLine, FlatCap, MiterJoin));
			switch (static_cast<int>(TabValues[yg]))
			{
				case 0:
					p.drawLine(qRound(TabValues[yg+1]), 15, qRound(TabValues[yg+1]), 23);
					p.drawLine(qRound(TabValues[yg+1]), 23, qRound(TabValues[yg+1]+8), 23);
					break;
				case 1:
					p.drawLine(qRound(TabValues[yg+1]), 15, qRound(TabValues[yg+1]), 23);
					p.drawLine(qRound(TabValues[yg+1]), 23, qRound(TabValues[yg+1]-8), 23);
					break;
				case 2:
				case 3:
					p.drawLine(qRound(TabValues[yg+1]), 15, qRound(TabValues[yg+1]), 23);
					p.drawLine(qRound(TabValues[yg+1]-4), 23, qRound(TabValues[yg+1]+4), 23);
					p.drawLine(qRound(TabValues[yg+1]+3), 20, qRound(TabValues[yg+1]+2), 20);
					break;
				case 4:
					p.drawLine(qRound(TabValues[yg+1]), 15, qRound(TabValues[yg+1]), 23);
					p.drawLine(qRound(TabValues[yg+1]-4), 23, qRound(TabValues[yg+1]+4), 23);
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
		cr.setPoints(3, qRound(First+Indent), 12, qRound(First+Indent-4), 0, qRound(First+Indent+4), 0);
		p.drawPolygon(cr);
		QPointArray cr2;
		cr2.setPoints(3, qRound(Indent), 12, qRound(Indent+4), 24, qRound(Indent-4), 24);
		p.drawPolygon(cr2);
	}
	p.end();
}

void RulerT::mousePressEvent(QMouseEvent *m)
{
	QRect fpo;
	Mpressed = true;
	RulerCode = 0;
	if (haveInd)
	{
		fpo = QRect(static_cast<int>(First+Indent-Offset)-4, 0, 8, 12);
		if (fpo.contains(m->pos()))
		{
			RulerCode = 1;
			MouseX = m->x();
			return;
		}
		fpo = QRect(static_cast<int>(Indent-Offset)-4, 12, 8, 12);
		if (fpo.contains(m->pos()))
		{
			RulerCode = 2;
			MouseX = m->x();
			return;
		}
	}
	if (TabValues.count() != 0)
	{
		for (int yg = 0; yg < static_cast<int>(TabValues.count()-1); yg += 2)
		{
			fpo = QRect(static_cast<int>(TabValues[yg+1]-Offset)-3, 15, 8, 8);
			if (fpo.contains(m->pos()))
			{
				RulerCode = 3;
				ActTab = yg;
				MouseX = m->x();
				emit TypeChanged(static_cast<int>(TabValues[ActTab]));
				emit TabMoved(TabValues[ActTab+1]);
				repaint();
				return;
			}
		}
	}
	if ((RulerCode == 0) && (m->button() == LeftButton))
	{
		TabValues.prepend(static_cast<double>(m->x() + Offset));
		TabValues.prepend(0);
		ActTab = 0;
		RulerCode = 3;
		UpdateTabList();
		repaint();
		emit NewTab();
		emit TypeChanged(static_cast<int>(TabValues[ActTab]));
		emit TabMoved(TabValues[ActTab+1]);
		qApp->setOverrideCursor(QCursor(SizeHorCursor), true);
	}
	MouseX = m->x();
}

void RulerT::mouseReleaseEvent(QMouseEvent *m)
{
	Mpressed = false;
	if ((m->y() < height()) && (m->y() > 0))
	{
		if (RulerCode == 3)
		{
			if (m->button() == RightButton)
			{
				TabValues[ActTab] += 1.0;
				if (TabValues[ActTab] > 4.0)
					TabValues[ActTab] = 0.0;
				emit TypeChanged(static_cast<int>(TabValues[ActTab]));
				repaint();
			}
		}
	}
	else
	{
		if (RulerCode == 3)
		{
			QValueList<double>::Iterator it;
			it = TabValues.at(ActTab);
			it = TabValues.remove(it);
			TabValues.remove(it);
			ActTab = 0;
			if (TabValues.count() != 0)
			{
				emit TypeChanged(static_cast<int>(TabValues[ActTab]));
				emit TabMoved(TabValues[ActTab+1]);
			}
			else
				emit NoTabs();
			repaint();
			qApp->setOverrideCursor(QCursor(ArrowCursor), true);
		}
	}
	RulerCode = 0;
}

void RulerT::mouseMoveEvent(QMouseEvent *m)
{
	double oldInd;
	QRect fpo;
	if ((Mpressed) && (m->y() < height()) && (m->y() > 0) && (m->x() > 0) && (m->x() < width()))
	{
		qApp->setOverrideCursor(QCursor(SizeHorCursor), true);
		switch (RulerCode)
		{
			case 1:
				First -= MouseX - m->x();
				if (First+Indent+Offset < Offset)
					First += MouseX - m->x();
				if (First+Indent > Width)
					First  = Width-Indent;
				emit FirstMoved(First);
				repaint();
				break;
			case 2:
				oldInd = Indent+First;
				Indent -= MouseX - m->x();
				if (Indent < 0)
					Indent = 0;
				if (Indent > Width-1)
					Indent  = Width-1;
				First = oldInd - Indent;
				emit IndentMoved(Indent);
				emit FirstMoved(First);
				repaint();
				break;
			case 3:
				TabValues[ActTab+1] -= MouseX - m->x();
				if (TabValues[ActTab+1] < 0)
					TabValues[ActTab+1] = 0;
				if (TabValues[ActTab+1] > Width-1)
					TabValues[ActTab+1]  = Width-1;
				UpdateTabList();
				emit TabMoved(TabValues[ActTab+1]);
				repaint();
				break;
			default:
				break;
		}
		MouseX = m->x();
		return;
	}
	if ((!Mpressed) && (m->y() < height()) && (m->y() > 0) && (m->x() > 0) && (m->x() < width()))
	{
		qApp->setOverrideCursor(QCursor(loadIcon("tab.png"), 3), true);
		if (haveInd)
		{
			fpo = QRect(static_cast<int>(First+Indent-Offset)-4, 0, 8, 12);
			if (fpo.contains(m->pos()))
			{
				qApp->setOverrideCursor(QCursor(SizeHorCursor), true);
				return;
			}
			fpo = QRect(static_cast<int>(Indent-Offset)-4, 12, 8, 12);
			if (fpo.contains(m->pos()))
			{
				qApp->setOverrideCursor(QCursor(SizeHorCursor), true);
				return;
			}
		}
		if (TabValues.count() != 0)
		{
			for (int yg = 0; yg < static_cast<int>(TabValues.count()-1); yg += 2)
			{
				fpo = QRect(static_cast<int>(TabValues[yg+1]-Offset)-3, 15, 8, 8);
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
	if ((Mpressed) && (RulerCode == 3))
	{
		qApp->setOverrideCursor(QCursor(loadIcon("DelPoint.png"), 4, 3), true);
		return;
	}
	else
		qApp->setOverrideCursor(QCursor(ArrowCursor), true);
}

void RulerT::UpdateTabList()
{
	QValueList<double>::Iterator it;
	double CurX = TabValues[ActTab+1];
	int gg = static_cast<int>(TabValues.count()-1);
	int g = gg;
	double type = TabValues[ActTab];
	it = TabValues.at(ActTab);
	it = TabValues.remove(it);
	TabValues.remove(it);
	for (int yg = static_cast<int>(TabValues.count()-1); yg > 0; yg -= 2)
	{
		if (CurX < TabValues[yg])
			g = yg;
	}
	ActTab = g-1;
	if (gg == g)
	{
		TabValues.append(type);
		TabValues.append(CurX);
		ActTab = static_cast<int>(TabValues.count()-2);
	}
	else
	{
		it = TabValues.at(ActTab);
		it = TabValues.insert(it, CurX);
		TabValues.insert(it, type);
	}
}

void RulerT::increaseOffset()
{
	Offset += 5;
	if (Offset + width() > static_cast<int>(Width))
		Offset -= 5;
	repaint();
}

void RulerT::decreaseOffset()
{
	Offset -= 5;
	if (Offset < 0)
		Offset = 0;
	repaint();
}

void RulerT::changeTab(int t)
{
	TabValues[ActTab] = static_cast<double>(t);
	repaint();
}

void RulerT::moveTab(double t)
{
	TabValues[ActTab+1] = t;
	UpdateTabList();
	repaint();
}

void RulerT::moveFirst(double t)
{
	First = t;
	if (First+Indent+Offset < Offset)
	{
		First = 0-Indent;
		emit FirstMoved(First);
	}
	if (First+Indent > Width)
	{
		First = Width-Indent;
		emit FirstMoved(First);
	}
	repaint();
}

void RulerT::moveIndent(double t)
{
	double oldInd = Indent+First;
	Indent = t;
	if (Indent > Width-1)
	{
		Indent  = Width-1;
		emit IndentMoved(Indent);
	}
	First = oldInd - Indent;
	emit FirstMoved(First);
	repaint();
}

Tabruler::Tabruler( QWidget* parent, bool haveFirst, int dEin, QValueList<double> Tabs, double wid ) : QWidget( parent )
{
	double ww;
	if (wid < 0)
		ww = 4000;
	else
		ww = wid;
	setName( "tabruler" );
	tabrulerLayout = new QVBoxLayout( this, 0, 6, "tabrulerLayout");
	layout2 = new QHBoxLayout( 0, 0, 6, "layout2");
	TypeCombo = new QComboBox( true, this, "TypeCombo" );
	TypeCombo->setEditable(false);
	TypeCombo->clear();
	TypeCombo->insertItem( tr( "Left" ) );
	TypeCombo->insertItem( tr( "Right" ) );
	TypeCombo->insertItem( tr( "Full Stop" ) );
	TypeCombo->insertItem( tr( "Comma" ) );
	TypeCombo->insertItem( tr( "Center" ) );
	layout2->addWidget( TypeCombo );
	RulerScrollL = new QToolButton( LeftArrow, this, "RulerScrollL" );
	RulerScrollL->setAutoRepeat( true );
	layout2->addWidget( RulerScrollL );
	Ruler = new RulerT( this, dEin, Tabs, haveFirst, wid );
	Ruler->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)5, 0, 0, Ruler->sizePolicy().hasHeightForWidth() ) );
	layout2->addWidget( Ruler );
	RulerScrollR = new QToolButton( RightArrow, this, "RulserScrollR" );
	RulerScrollR->setAutoRepeat( true );
	layout2->addWidget( RulerScrollR );
	tabrulerLayout->addLayout( layout2 );
	layout1 = new QHBoxLayout( 0, 0, 6, "layout1");
	TabSpin = new MSpinBox( this, 1 );
	TabSpin->setMaxValue( ww );
	TabSpin->setMinValue( 0 );
	TabSpin->setValue(0);
	Label1 = new QLabel( TabSpin, tr("&Position:"), this, "Label1" );
	layout1->addWidget( Label1 );
	layout1->addWidget( TabSpin );
	if (haveFirst)
	{
		FirstSpin = new MSpinBox( this, 1);
		FirstSpin->setMaxValue( ww );
		FirstSpin->setMinValue( -30000 );
		FirstSpin->setValue(0);
		Label2 = new QLabel( FirstSpin, tr( "First &Line:"), this, "Label2" );
		layout1->addWidget( Label2 );
		layout1->addWidget( FirstSpin );
		IndentSpin = new MSpinBox( this, 1 );
		IndentSpin->setMaxValue( ww );
		IndentSpin->setMinValue( 0 );
		IndentSpin->setValue(0);
		Label3 = new QLabel( IndentSpin, tr( "Left Ind&ent:" ), this, "Label3" );
		layout1->addWidget( Label3 );
		layout1->addWidget( IndentSpin );
	}
	ClearButton = new QPushButton( this, "ClearButton" );
	ClearButton->setText( tr( "Delete All" ) );
	layout1->addWidget( ClearButton );
	if (!haveFirst)
	{
		QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
		layout1->addItem( spacer );
	}
	tabrulerLayout->addLayout( layout1 );
	if (Tabs.count() == 0)
	{
		TypeCombo->setEnabled(false);
		TabSpin->setEnabled(false);
		ClearButton->setEnabled(false);
	}
	resize( minimumSizeHint() );
	connect(RulerScrollL, SIGNAL(clicked()), Ruler, SLOT(decreaseOffset()));
	connect(RulerScrollR, SIGNAL(clicked()), Ruler, SLOT(increaseOffset()));
	connect(Ruler, SIGNAL(TypeChanged(int)) , this, SLOT(setTabType(int)));
	connect(TypeCombo, SIGNAL(activated(int)), this, SLOT(setType()));
	connect(Ruler, SIGNAL(TabMoved(double)) , this, SLOT(setTabSpin(double)));
	connect(Ruler, SIGNAL(NewTab()), this, SLOT(tabAdded()));
	connect(Ruler, SIGNAL(NoTabs()), this, SLOT(lastTabRemoved()));
	connect(TabSpin, SIGNAL(valueChanged(int)), this, SLOT(setTab()));
	connect(ClearButton, SIGNAL(clicked()), this, SLOT(clearAll()));
	if (haveFirst)
	{
		connect(Ruler, SIGNAL(FirstMoved(double)) , this, SLOT(setFirstSpin(double)));
		connect(Ruler, SIGNAL(IndentMoved(double)) , this, SLOT(setIndentSpin(double)));
		connect(FirstSpin, SIGNAL(valueChanged(int)), this, SLOT(setFirst()));
		connect(IndentSpin, SIGNAL(valueChanged(int)), this, SLOT(setIndent()));
		QToolTip::add( FirstSpin, tr( "Indentation for first line of the paragraph" ) );
		QToolTip::add( IndentSpin, tr( "Indentation from the left for the whole paragraph" ) );
	}
	QToolTip::add( ClearButton, tr( "Delete all Tabulators" ) );
	QString ein, measure[] = { tr(" pt"), tr(" mm"), tr(" in"), tr(" p")};
	ein = measure[dEin];
	if (dEin == 2)
	{
		if (haveFirst)
		{
			FirstSpin->setDecimals(10000);
			IndentSpin->setDecimals(10000);
		}
		TabSpin->setDecimals(10000);
	}
	if (haveFirst)
	{
		FirstSpin->setSuffix(ein);
		IndentSpin->setSuffix(ein);
	}
	TabSpin->setSuffix(ein);
	haveF = haveFirst;
}

void Tabruler::clearAll()
{
	Ruler->TabValues.clear();
	Ruler->repaint();
	lastTabRemoved();
}

void Tabruler::tabAdded()
{
	TypeCombo->setEnabled(true);
	TabSpin->setEnabled(true);
	ClearButton->setEnabled(true);
}

void Tabruler::lastTabRemoved()
{
	TypeCombo->setEnabled(false);
	TabSpin->setEnabled(false);
	ClearButton->setEnabled(false);
}

void Tabruler::setTabType(int t)
{
	TypeCombo->setCurrentItem(t);
}

void Tabruler::setType()
{
	disconnect(TypeCombo, SIGNAL(activated(int)), this, SLOT(setType()));
	Ruler->changeTab(TypeCombo->currentItem());
	connect(TypeCombo, SIGNAL(activated(int)), this, SLOT(setType()));
}

void Tabruler::setTabSpin(double t)
{
	disconnect(TabSpin, SIGNAL(valueChanged(int)), this, SLOT(setTab()));
	TabSpin->setValue(t * UmReFaktor);
	connect(TabSpin, SIGNAL(valueChanged(int)), this, SLOT(setTab()));
}

void Tabruler::setTab()
{
	Ruler->moveTab(TabSpin->value() / UmReFaktor);
}

void Tabruler::setFirstSpin(double t)
{
	disconnect(FirstSpin, SIGNAL(valueChanged(int)), this, SLOT(setFirst()));
	FirstSpin->setValue(t * UmReFaktor);
	connect(FirstSpin, SIGNAL(valueChanged(int)), this, SLOT(setFirst()));
}

void Tabruler::setFirst()
{
	Ruler->moveFirst(FirstSpin->value() / UmReFaktor);
}

void Tabruler::setIndentSpin(double t)
{
	disconnect(IndentSpin, SIGNAL(valueChanged(int)), this, SLOT(setIndent()));
	IndentSpin->setValue(t * UmReFaktor);
	connect(IndentSpin, SIGNAL(valueChanged(int)), this, SLOT(setIndent()));
}

void Tabruler::setIndent()
{
	Ruler->moveIndent(IndentSpin->value() / UmReFaktor);
}

QValueList<double> Tabruler::getTabVals()
{
	return Ruler->TabValues;
}

double Tabruler::getFirst()
{
	return FirstSpin->value() / UmReFaktor;
}

double Tabruler::getIndent()
{
	return IndentSpin->value() / UmReFaktor;
}
