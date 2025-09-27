/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "tabruler.h"

#include <QApplication>
#include <QColor>
#include <QComboBox>
#include <QCursor>
#include <QEvent>
#include <QLabel>
#include <QLayout>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QPalette>
#include <QPixmap>
#include <QPolygon>
#include <QPushButton>
#include <QToolButton>
#include <QToolTip>
#include <QVariant>

#include "commonstrings.h"
#include "iconmanager.h"
#include "scribusapp.h"
#include "scribusstructs.h"
#include "scrspinbox.h"
#include "units.h"
#include "util.h"

RulerT::RulerT(QWidget *pa, int unit, const QList<ParagraphStyle::TabRecord>& tabs, bool ind, double wid) : QWidget(pa),
	tabValues(tabs),
	haveInd(ind),
	unitIndex(unit)
{
	QPalette palette;
	palette.setColor(backgroundRole(), QColor(255,255,255));
	setPalette(palette);
	m_iter = unitRulerGetIter1FromIndex(unitIndex);
	m_iter2 = unitRulerGetIter2FromIndex(unitIndex);

	setMouseTracking(true);
	if (wid < 0)
	{
		setMinimumSize(QSize(400,25));
		setMaximumSize(QSize(4000,25));
		m_rulerWidth = 4000;
		resize(400, 25);
	}
	else
	{
		m_rulerWidth = wid;
		setMinimumSize(QSize(qMin(static_cast<int>(m_rulerWidth), 400), 25));
		setMaximumSize(QSize(4000, 25));
		resize(qMin(static_cast<int>(wid), 400), 25);
	}
}

void RulerT::setTabs(const QList<ParagraphStyle::TabRecord>& tabs, int unit)
{
	unitIndex = unit;
	m_iter  = unitRulerGetIter1FromIndex(unitIndex);
	m_iter2 = unitRulerGetIter2FromIndex(unitIndex);
	tabValues = tabs;
	actTab    = -1;
	update();
}

void RulerT::paintEvent(QPaintEvent *)
{
	double xl;

	const QPalette& palette = this->palette();
	const QColor& textColor = palette.color(QPalette::Text);

	QPainter p;
	p.begin(this);
	p.drawLine(0, 24, width(), 24);
	p.translate(-offset, 0);
	p.setBrush(textColor);
	p.setFont(font());
	p.setPen(QPen(textColor, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
	for (xl = 0; xl < width() + offset; xl += m_iter)
	{
		if (xl < offset)
			continue;
		p.drawLine(qRound(xl), 18, qRound(xl), 24);
	}

	for (xl = 0; xl < width() + (m_iter2 / 2) + offset; xl += m_iter2)
	{
		if (xl < offset)
			continue;
		p.drawLine(qRound(xl), 11, qRound(xl), 24);
		switch (unitIndex)
		{
			case 2:
			{
				QString tx;
				int num1 = static_cast<int>(xl / m_iter2);
				if (num1 != 0)
					tx = QString::number(num1);
				double frac = (xl / m_iter2) - num1;
				if ((frac > 0.24) && (frac < 0.26))
					tx += QChar(0xBC);
				if ((frac > 0.49) && (frac < 0.51))
					tx += QChar(0xBD);
				if ((frac > 0.74) && (frac < 0.76))
					tx += QChar(0xBE);
				p.drawText(qRound(xl + 2), 17, tx);
				break;
			}
			case 3:
				p.drawText(qRound(xl + 2), 17, QString::number(xl / m_iter));
				break;
			default:
				p.drawText(qRound(xl + 2), 17, QString::number(xl / m_iter * 10));
				break;
		}
	}

	for (int i = 0; i < tabValues.count(); i++)
	{
		if (i == actTab)
			p.setPen(QPen(Qt::red, 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
		else
			p.setPen(QPen(textColor, 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
		switch (tabValues[i].tabType)
		{
			case 0:
				p.drawLine(qRound(tabValues[i].tabPosition), 15, qRound(tabValues[i].tabPosition), 23);
				p.drawLine(qRound(tabValues[i].tabPosition), 23, qRound(tabValues[i].tabPosition + 8), 23);
				break;
			case 1:
				p.drawLine(qRound(tabValues[i].tabPosition), 15, qRound(tabValues[i].tabPosition), 23);
				p.drawLine(qRound(tabValues[i].tabPosition), 23, qRound(tabValues[i].tabPosition - 8), 23);
				break;
			case 2:
			case 3:
				p.drawLine(qRound(tabValues[i].tabPosition), 15, qRound(tabValues[i].tabPosition), 23);
				p.drawLine(qRound(tabValues[i].tabPosition - 4), 23, qRound(tabValues[i].tabPosition + 4), 23);
				p.drawLine(qRound(tabValues[i].tabPosition + 3), 20, qRound(tabValues[i].tabPosition + 2), 20);
				break;
			case 4:
				p.drawLine(qRound(tabValues[i].tabPosition), 15, qRound(tabValues[i].tabPosition), 23);
				p.drawLine(qRound(tabValues[i].tabPosition - 4), 23, qRound(tabValues[i].tabPosition + 4), 23);
				break;
			default:
				break;
		}
	}

	if (haveInd)
	{
		p.setPen(QPen(Qt::blue, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
		p.setBrush(Qt::blue);
		QPolygon cr;
		cr.setPoints(3, qRound(firstLine + leftIndent), 12, qRound(firstLine + leftIndent - 4), 0, qRound(firstLine + leftIndent + 4), 0);
		p.drawPolygon(cr);
		QPolygon cr2;
		cr2.setPoints(3, qRound(leftIndent), 12, qRound(leftIndent + 4), 24, qRound(leftIndent - 4), 24);
		p.drawPolygon(cr2);
	}
	p.end();
}

void RulerT::mousePressEvent(QMouseEvent *m)
{
	QRect fpo;
	mousePressed = true;
	QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
	rulerCode = 0;
	if (haveInd)
	{
		fpo = QRect(static_cast<int>(firstLine + leftIndent - offset) - 4, 0, 8, 12);
		if (fpo.contains(m->pos()))
		{
			rulerCode = 1;
			mouseX = m->x();
			return;
		}
		fpo = QRect(static_cast<int>(leftIndent - offset) - 4, 12, 8, 12);
		if (fpo.contains(m->pos()))
		{
			rulerCode = 2;
			mouseX = m->x();
			return;
		}
	}
	for (int i = 0; i < tabValues.count(); ++i)
	{
		fpo = QRect(static_cast<int>(tabValues[i].tabPosition - offset) - 3, 15, 8, 8);
		if (fpo.contains(m->pos()))
		{
			rulerCode = 3;
			actTab = i;
			mouseX = m->x();
			emit tabSelected();
			emit typeChanged(tabValues[actTab].tabType);
			emit tabMoved(tabValues[actTab].tabPosition);
			emit fillCharChanged(tabValues[actTab].tabFillChar);
			update();
			return;
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
		update();
		emit newTab();
		emit typeChanged(tabValues[actTab].tabType);
		emit tabMoved(tabValues[actTab].tabPosition);
		emit fillCharChanged(tabValues[actTab].tabFillChar);
		QApplication::changeOverrideCursor(QCursor(Qt::SizeHorCursor));
	}
	mouseX = m->x();
}

void RulerT::mouseReleaseEvent(QMouseEvent *m)
{
	mousePressed = false;
	QApplication::restoreOverrideCursor();
	if ((m->y() < height()) && (m->y() > 0))
	{
		if ((rulerCode == 3) && (m->button() == Qt::RightButton))
		{
			tabValues[actTab].tabType += 1;
			if (tabValues[actTab].tabType > 4)
				tabValues[actTab].tabType = 0;
			emit typeChanged(tabValues[actTab].tabType);
			update();
		}
	}
	else if (rulerCode == 3)
	{
		tabValues.removeAt(actTab);
		actTab = 0;
		if (tabValues.count() != 0)
		{
			emit typeChanged(tabValues[actTab].tabType);
			emit tabMoved(tabValues[actTab].tabPosition);
			emit fillCharChanged(tabValues[actTab].tabFillChar);
		}
		else
			emit noTabs();
		update();
	}
	rulerCode = 0;
	emit mouseReleased();
}

void RulerT::mouseMoveEvent(QMouseEvent *m)
{
	double oldInd;
	QRect fpo;
	if (mousePressed && (m->y() < height()) && (m->y() > 0) && (m->x() > 0) && (m->x() < width()))
	{
		QApplication::changeOverrideCursor(QCursor(Qt::SizeHorCursor));
		switch (rulerCode)
		{
			case 1:
				firstLine -= mouseX - m->x();
				if (firstLine + leftIndent + offset < offset)
					firstLine += mouseX - m->x();
				if (firstLine + leftIndent > m_rulerWidth)
					firstLine  = m_rulerWidth - leftIndent;
				emit firstLineMoved(firstLine);
				update();
				break;
			case 2:
				oldInd = leftIndent+firstLine;
				leftIndent -= mouseX - m->x();
				if (leftIndent < 0)
					leftIndent = 0;
				if (leftIndent > m_rulerWidth - 1)
					leftIndent  = m_rulerWidth - 1;
				firstLine = oldInd - leftIndent;
				emit leftIndentMoved(leftIndent);
				emit firstLineMoved(firstLine);
				update();
				break;
			case 3:
				tabValues[actTab].tabPosition -= mouseX - m->x();
				if (tabValues[actTab].tabPosition < 0)
					tabValues[actTab].tabPosition = 0;
				if (tabValues[actTab].tabPosition > m_rulerWidth - 1)
					tabValues[actTab].tabPosition = m_rulerWidth - 1;
				updateTabList();
				emit tabMoved(tabValues[actTab].tabPosition);
				update();
				break;
			default:
				break;
		}
		mouseX = m->x();
		return;
	}
	if ((!mousePressed) && (m->y() < height()) && (m->y() > 0) && (m->x() > 0) && (m->x() < width()))
	{
		setCursor(IconManager::instance().loadCursor("tab.png", 3));
		if (haveInd)
		{
			fpo = QRect(static_cast<int>(firstLine + leftIndent - offset) - 4, 0, 8, 12);
			if (fpo.contains(m->pos()))
			{
				setCursor(QCursor(Qt::SizeHorCursor));
				return;
			}
			fpo = QRect(static_cast<int>(leftIndent - offset) - 4, 12, 8, 12);
			if (fpo.contains(m->pos()))
			{
				setCursor(QCursor(Qt::SizeHorCursor));
				return;
			}
		}
		for (int i = 0; i < tabValues.count(); ++i)
		{
			fpo = QRect(static_cast<int>(tabValues[i].tabPosition - offset) - 3, 15, 8, 8);
			if (fpo.contains(m->pos()))
			{
				setCursor(QCursor(Qt::SizeHorCursor));
				return;
			}
		}
	}
	if (mousePressed && ((m->y() > height()) || (m->y() < 0) || (m->x() < 0) || (m->x() > width())))
	{
		QApplication::changeOverrideCursor(IconManager::instance().loadCursor("DelPoint.png", 1, 1));
	}
}

void RulerT::leaveEvent(QEvent*)
{
	if (mousePressed)
	{
		if (rulerCode == 3)
			QApplication::changeOverrideCursor(IconManager::instance().loadCursor("DelPoint.png", 1, 1));
		else
			QApplication::changeOverrideCursor(QCursor(Qt::ArrowCursor));
	}
}

void RulerT::updateTabList()
{
	ParagraphStyle::TabRecord tb;
	tb.tabPosition = tabValues[actTab].tabPosition;
	tb.tabType = tabValues[actTab].tabType;
	tb.tabFillChar =  tabValues[actTab].tabFillChar;
	int gg = tabValues.count() - 1;
	int g = gg;
	tabValues.removeAt(actTab);
	for (int i = tabValues.count() - 1; i > -1; --i)
	{
		if (tb.tabPosition < tabValues[i].tabPosition)
			g = i;
	}
	actTab = g;
	if (gg == g)
	{
		tabValues.append(tb);
		actTab = tabValues.count() - 1;
	}
	else
	{
		tabValues.insert(actTab, tb);
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
	if (offset + width() > static_cast<int>(m_rulerWidth))
		offset -= 5;
	update();
}

void RulerT::decreaseOffset()
{
	offset -= offsetIncrement;
	offsetIncrement++;
	if (offsetIncrement > 30)
		offsetIncrement = 30;
	if (offset < 0)
		offset = 0;
	update();
}

void RulerT::changeTab(int t)
{
	if (actTab < 0 || actTab >= tabValues.count())
		return;
	tabValues[actTab].tabType = t;
	update();
}

void RulerT::changeTabChar(QChar t)
{
	if (actTab < 0 || actTab >= tabValues.count())
		return;
	tabValues[actTab].tabFillChar = t;
	update();
}

void RulerT::moveTab(double t)
{
	if (actTab < 0 || actTab >= tabValues.count())
		return;
	tabValues[actTab].tabPosition = t;
	updateTabList();
	update();
}

void RulerT::removeActTab()
{
	if (actTab > -1)
	{
		tabValues.removeAt(actTab);
		actTab = 0;
		if (tabValues.count() != 0)
		{
			emit typeChanged(tabValues[actTab].tabType);
			emit tabMoved(tabValues[actTab].tabPosition);
			emit fillCharChanged(tabValues[actTab].tabFillChar);
		}
		else
			emit noTabs();
	}
	update();
}

void RulerT::moveFirstLine(double t)
{
	firstLine = t;
	if (firstLine+leftIndent+offset < offset)
	{
		firstLine = 0-leftIndent;
		emit firstLineMoved(firstLine);
	}
	if (firstLine+leftIndent > m_rulerWidth)
	{
		firstLine = m_rulerWidth - leftIndent;
		emit firstLineMoved(firstLine);
	}
	update();
}

void RulerT::moveLeftIndent(double t)
{
	double oldInd = leftIndent + firstLine;
	leftIndent = t;
	if (leftIndent > m_rulerWidth - 1)
	{
		leftIndent  = m_rulerWidth - 1;
		emit leftIndentMoved(leftIndent);
	}
	firstLine = oldInd - leftIndent;
	emit firstLineMoved(firstLine);
	update();
}

Tabruler::Tabruler( QWidget* parent, bool haveFirst, int unit, const QList<ParagraphStyle::TabRecord>& tabs, double wid ): QWidget( parent )
{
	m_docUnitRatio = unitGetRatioFromIndex(unit);
	double ww = (wid < 0) ? 4000 : wid;
	tabrulerLayout = new QVBoxLayout( this );
	tabrulerLayout->setContentsMargins(0, 0, 0, 0);
	tabrulerLayout->setSpacing(6);
	layout2 = new QHBoxLayout;
	layout2->setContentsMargins(0, 0, 0, 0);
	layout2->setSpacing(6);

	rulerScrollL = new QToolButton(this);
	rulerScrollL->setArrowType(Qt::LeftArrow);
	rulerScrollL->setAutoRepeat( true );
	layout2->addWidget( rulerScrollL );
	ruler = new RulerT( this, unit, tabs, haveFirst, wid );
	ruler->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	layout2->addWidget( ruler );
	rulerScrollR = new QToolButton(this);
	rulerScrollR->setArrowType(Qt::RightArrow);
	rulerScrollR->setAutoRepeat( true );
	layout2->addWidget( rulerScrollR );

	layout1 = new QHBoxLayout;
	layout1->setContentsMargins(0, 0, 0, 0);
	layout1->setSpacing(6);
	layout1->setAlignment( Qt::AlignTop );
	typeCombo = new QComboBox(this);
	typeCombo->setEditable(false);
	typeCombo->clear();
	typeCombo->addItem( tr( "Left" ) );
	typeCombo->addItem( tr( "Right" ) );
	typeCombo->addItem( tr( "Period" ) );
	typeCombo->addItem( tr( "Comma" ) );
	typeCombo->addItem( tr( "Center" ) );
	layout1->addWidget( typeCombo );
	tabData = new ScrSpinBox( 0, ww / m_docUnitRatio, this, unit );
	tabData->setValue(0);
	positionLabel = new QLabel( tr("&Position:"), this );
	positionLabel->setBuddy(tabData);
	layout1->addWidget( positionLabel );
	layout1->addWidget( tabData );
	tabFillCombo = new QComboBox(this);
	tabFillCombo->setEditable(false);
	tabFillCombo->addItem( tr("None", "tab fill"));
	tabFillCombo->addItem( tr("Dot"));
	tabFillCombo->addItem( tr("Hyphen"));
	tabFillCombo->addItem( tr("Underscore"));
	tabFillCombo->addItem( tr("Custom"));
	tabFillComboT = new QLabel( tr( "Fill Char:" ), this );
	tabFillComboT->setBuddy(tabFillCombo);
	layout1->addWidget( tabFillComboT );
	layout1->addWidget( tabFillCombo );
	clearOneButton = new QToolButton( this );
	clearOneButton->setIcon(IconManager::instance().loadIcon("16/edit-delete.png"));
	layout1->addSpacing(12);
	layout1->addWidget(clearOneButton);
	clearButton = new QToolButton( this );
	clearButton->setIcon(IconManager::instance().loadIcon("16/edit-delete-all.png"));
	layout1->addWidget(clearButton);

	indentLayout = new QHBoxLayout;
	indentLayout->setContentsMargins(0, 0, 0, 0);
	indentLayout->setSpacing(6);
	if (haveFirst)
	{
		firstLineData = new ScrSpinBox( -3000, ww / m_docUnitRatio, this, unit);
		firstLineData->setValue(0);
		firstLineLabel = new QLabel(this);
		firstLineLabel->setText("");
		firstLineLabel->setPixmap(IconManager::instance().loadPixmap("firstline.png"));
		indentLayout->addWidget( firstLineLabel );
		indentLayout->addWidget( firstLineData );
		leftIndentData = new ScrSpinBox( 0, ww / m_docUnitRatio, this, unit);
		leftIndentData->setValue(0);
		leftIndentLabel = new QLabel(this);
		leftIndentLabel->setText("");
		leftIndentLabel->setPixmap(IconManager::instance().loadPixmap("leftindent.png"));
		indentLayout->addWidget(leftIndentLabel);
		indentLayout->addWidget(leftIndentData);
		rightIndentLabel = new QLabel(this);
		rightIndentLabel->setText("");
		rightIndentLabel->setPixmap(IconManager::instance().loadPixmap("rightindent.png"));
		rightIndentData = new ScrSpinBox(0, ww / m_docUnitRatio, this, unit);
		rightIndentData->setValue(0);
		indentLayout->addWidget(rightIndentLabel);
		indentLayout->addWidget(rightIndentData);
	}
	if (!haveFirst)
	{
		auto* spacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
		layout1->addItem(spacer);
	}
	
	layout1->addStretch( 10 );
	tabrulerLayout->addLayout( layout1 );
	tabrulerLayout->addLayout( layout2 );
	indentLayout->addStretch( 10 );
	tabrulerLayout->addLayout( indentLayout );

	tabData->setEnabled(false);
	tabFillCombo->setEnabled(false);
	typeCombo->setEnabled(false);
	if (tabs.isEmpty())
		clearButton->setEnabled(false);
	clearOneButton->setEnabled(false);
	resize( minimumSizeHint() );

	connect(rulerScrollL, SIGNAL(clicked()), ruler, SLOT(decreaseOffset()));
	connect(rulerScrollR, SIGNAL(clicked()), ruler, SLOT(increaseOffset()));
	connect(rulerScrollL, SIGNAL(released()), this, SLOT(resetOFfL()));
	connect(rulerScrollR, SIGNAL(released()), this, SLOT(resetOFfR()));
	connect(ruler, SIGNAL(typeChanged(int)) , this, SLOT(setTabType(int)));
	connect(ruler, SIGNAL(fillCharChanged(QChar)) , this, SLOT(setTabFillChar(QChar)));
	connect(typeCombo, SIGNAL(activated(int)), this, SLOT(setType()));
	connect(tabFillCombo, SIGNAL(activated(int)), this, SLOT(setFillChar()));
	connect(tabFillCombo, SIGNAL(editTextChanged(const QString &)), this, SLOT(setCustomFillChar(const QString &)));
	connect(ruler, SIGNAL(tabMoved(double)) , this, SLOT(setTabData(double)));
	connect(ruler, SIGNAL(tabSelected()), this, SLOT(tabAdded()));
	connect(ruler, SIGNAL(newTab()), this, SLOT(tabAdded()));
	connect(ruler, SIGNAL(noTabs()), this, SLOT(lastTabRemoved()));
	connect(tabData, SIGNAL(valueChanged(double)), this, SLOT(setTab()));
	connect(clearButton, SIGNAL(clicked()), this, SLOT(clearAll()));
	connect(clearOneButton, SIGNAL(clicked()), this, SLOT(clearOne()));

	tabFillCombo->setToolTip( tr( "Fill Character of Tab" ) );
	typeCombo->setToolTip( tr( "Type/Orientation of Tab" ) );
	tabData->setToolTip( tr( "Position of Tab" ) );

	if (haveFirst)
	{
		connect(ruler, SIGNAL(firstLineMoved(double)) , this, SLOT(setFirstLineData(double)));
		connect(ruler, SIGNAL(leftIndentMoved(double)) , this, SLOT(setLeftIndentData(double)));
		connect(ruler, SIGNAL(mouseReleased()), this, SIGNAL(tabrulerChanged()));
		connect(ruler, SIGNAL(mouseReleased()), this, SLOT(slotMouseReleased()));
		connect(firstLineData, SIGNAL(valueChanged(double)), this, SLOT(setFirstLine()));
		connect(leftIndentData, SIGNAL(valueChanged(double)), this, SLOT(setLeftIndent()));
		connect(rightIndentData, SIGNAL(valueChanged(double)), this, SLOT(setRightIndent()));
		firstLineData->setToolTip( tr( "Indentation for first line of the paragraph" ) );
		leftIndentData->setToolTip( tr( "Indentation from the left for the whole paragraph" ) );
		rightIndentData->setToolTip( tr( "Indentation from the right for the whole paragraph" ) );
		firstLineLabel->setToolTip(firstLineData->toolTip());
		leftIndentLabel->setToolTip(leftIndentData->toolTip());
		rightIndentLabel->setToolTip(rightIndentData->toolTip());
	}
	clearButton->setToolTip( tr( "Delete all Tabulators" ) );
	clearOneButton->setToolTip( tr("Delete selected Tabulator"));

	if (unit == SC_INCHES)
	{
		if (haveFirst)
		{
			firstLineData->setDecimals(4);
			leftIndentData->setDecimals(4);
			rightIndentData->setDecimals(4);
		}
		tabData->setDecimals(4);
	}
	m_haveFirst = haveFirst;

	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));
}

void Tabruler::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
		return;
	}
	QWidget::changeEvent(e);
}

void Tabruler::iconSetChange()
{
	IconManager& iconManager = IconManager::instance();

	if (firstLineLabel)
		firstLineLabel->setPixmap(iconManager.loadPixmap("firstline.png"));

	if (leftIndentLabel)
		leftIndentLabel->setPixmap(iconManager.loadPixmap("leftindent.png"));

	if (rightIndentLabel)
		rightIndentLabel->setPixmap(iconManager.loadPixmap("rightindent.png"));

	clearOneButton->setIcon(iconManager.loadIcon("16/edit-delete.png"));
	clearButton->setIcon(iconManager.loadIcon("16/edit-delete-all.png"));
}

void Tabruler::languageChange()
{
	int  oldTypeComboIndex = typeCombo->currentIndex();
	bool typeComboBlocked = typeCombo->blockSignals(true);
	typeCombo->clear();
	typeCombo->addItem( tr( "Left" ) );
	typeCombo->addItem( tr( "Right" ) );
	typeCombo->addItem( tr( "Period" ) );
	typeCombo->addItem( tr( "Comma" ) );
	typeCombo->addItem( tr( "Center" ) );
	typeCombo->setCurrentIndex(oldTypeComboIndex);
	typeCombo->blockSignals(typeComboBlocked);

	positionLabel->setText( tr("&Position:"));

	int  oldTabFillComboIndex = tabFillCombo->currentIndex();
	bool tabFillComboBlocked = tabFillCombo->blockSignals(true);
	tabFillCombo->clear();
	tabFillCombo->addItem( tr("None", "tab fill"));
	tabFillCombo->addItem( tr("Dot"));
	tabFillCombo->addItem( tr("Hyphen"));
	tabFillCombo->addItem( tr("Underscore"));
	tabFillCombo->addItem( tr("Custom"));
	tabFillCombo->setCurrentIndex(oldTabFillComboIndex);
	tabFillCombo->blockSignals(tabFillComboBlocked);

	tabFillComboT->setText( tr( "Fill Char:" ));

	clearButton->setText( tr( "Delete All" ) );
	clearOneButton->setText( tr( "Delete Selected" ) );

	tabFillCombo->setToolTip( tr( "Fill Character of Tab" ) );
	typeCombo->setToolTip( tr( "Type/Orientation of Tab" ) );
	tabData->setToolTip( tr( "Position of Tab" ) );

	if (m_haveFirst)
	{
		firstLineData->setToolTip( tr( "Indentation for first line of the paragraph" ) );
		leftIndentData->setToolTip( tr( "Indentation from the left for the whole paragraph" ) );
		rightIndentData->setToolTip( tr( "Indentation from the right for the whole paragraph" ) );
		firstLineLabel->setToolTip(firstLineData->toolTip());
		leftIndentLabel->setToolTip(leftIndentData->toolTip());
		rightIndentLabel->setToolTip(rightIndentData->toolTip());
	}
	clearButton->setToolTip( tr( "Delete all Tabulators" ) );
	clearOneButton->setToolTip( tr("Delete selected Tabulator"));

	QString unitSuffix = unitGetSuffixFromIndex(tabData->unitIndex());
	if (m_haveFirst)
	{
		firstLineData->setSuffix(unitSuffix);
		leftIndentData->setSuffix(unitSuffix);
		rightIndentData->setSuffix(unitSuffix);
	}
	tabData->setSuffix(unitSuffix);
}

void Tabruler::setTabs(const QList<ParagraphStyle::TabRecord>& tabs, int unit)
{
	m_docUnitRatio = unitGetRatioFromIndex(unit);
	tabData->setNewUnit(unit);
	if (m_haveFirst)
	{
		firstLineData->setNewUnit(unit);
		leftIndentData->setNewUnit(unit);
		rightIndentData->setNewUnit(unit);
	}
	ruler->setTabs(tabs, unit);
	if (tabs.isEmpty())
		clearButton->setEnabled(false);
	clearOneButton->setEnabled(false);
	tabData->setEnabled(false);
	tabFillCombo->setEnabled(false);
	typeCombo->setEnabled(false);
}

void Tabruler::resetOFfL()
{
	if (!rulerScrollL->isDown())
		ruler->resetOffsetInc();
	tabData->setEnabled(false);
	tabFillCombo->setEnabled(false);
	typeCombo->setEnabled(false);
}

void Tabruler::resetOFfR()
{
	if (!rulerScrollR->isDown())
		ruler->resetOffsetInc();
}

void Tabruler::clearAll()
{
	ruler->tabValues.clear();
	ruler->update();
	lastTabRemoved();
	emit tabrulerChanged();
	emit tabsChanged();
}

void Tabruler::clearOne()
{
	ruler->removeActTab();
	emit tabrulerChanged();
	emit tabsChanged();
}

void Tabruler::tabAdded()
{
	typeCombo->setEnabled(true);
	tabData->setEnabled(true);
	clearButton->setEnabled(true);
	clearOneButton->setEnabled(true);
	tabFillCombo->setEnabled(true);
	emit tabrulerChanged();
	emit tabsChanged();
}

void Tabruler::lastTabRemoved()
{
	typeCombo->setEnabled(false);
	tabData->setEnabled(false);
	clearButton->setEnabled(false);
	clearOneButton->setEnabled(false);
	tabFillCombo->setEnabled(false);
	emit tabrulerChanged();
	emit tabsChanged();
}

void Tabruler::setFillChar()
{
	bool sigBlocked = tabFillCombo->blockSignals(true);
	QChar ret;
	switch (tabFillCombo->currentIndex())
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
			setCurrentComboItem(tabFillCombo, CommonStrings::trCustomTabFill);
			break;
	}
	if (tabFillCombo->currentIndex() != 4)
		ruler->changeTabChar(ret);
	tabFillCombo->blockSignals(sigBlocked);
	emit tabrulerChanged();
	emit tabsChanged();
}

void Tabruler::setCustomFillChar(const QString &txt)
{
	if (txt == CommonStrings::trCustomTabFill)
		return;
	bool sigBlocked = tabFillCombo->blockSignals(true);
	QChar ret = (txt.length() > 0) ? txt.back() : QChar::Null;
	ruler->changeTabChar(ret);
	tabFillCombo->blockSignals(sigBlocked);
	emit tabrulerChanged();
	emit tabsChanged();
}

void Tabruler::setTabFillChar(QChar t)
{
	if (t.isNull())
	{
		tabFillCombo->setEditable(false);
		tabFillCombo->setCurrentIndex(0);
	}
	else if (t == '.')
	{
		tabFillCombo->setEditable(false);
		tabFillCombo->setCurrentIndex(1);
	}
	else if (t == '-')
	{
		tabFillCombo->setEditable(false);
		tabFillCombo->setCurrentIndex(2);
	}
	else if (t == '_')
	{
		tabFillCombo->setEditable(false);
		tabFillCombo->setCurrentIndex(3);
	}
	else
	{
		tabFillCombo->setCurrentIndex(4);
		tabFillCombo->setEditable(true);
		if (!t.isNull())
		{
			bool sigBlocked = tabFillCombo->blockSignals(true);
			tabFillCombo->setEditText(QString(t));
			tabFillCombo->blockSignals(sigBlocked);
		}
	}
	emit tabrulerChanged();
	emit tabsChanged();
}

void Tabruler::setTabType(int t)
{
	typeCombo->setCurrentIndex(t);
	emit tabrulerChanged();
	emit tabsChanged();
}

void Tabruler::setType()
{
	bool sigBlocked = typeCombo->blockSignals(true);
	ruler->changeTab(typeCombo->currentIndex());
	typeCombo->blockSignals(sigBlocked);
	emit tabrulerChanged();
	emit tabsChanged();
}

void Tabruler::setTabData(double t)
{
	bool sigBlocked = tabData->blockSignals(true);
	tabData->setValue(t * m_docUnitRatio);
	tabData->blockSignals(sigBlocked);
	if (!ruler->mousePressed)
	{
		emit tabrulerChanged();
		emit tabsChanged();
	}
}

void Tabruler::setTab()
{
	ruler->moveTab(tabData->value() / m_docUnitRatio);
	emit tabrulerChanged();
	emit tabsChanged();
}

void Tabruler::setFirstLineData(double t)
{
//	bool sigBlocked = firstLineData->blockSignals(true);
	firstLineData->setValue(t * m_docUnitRatio);
//	firstLineData->blockSignals(sigBlocked);
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
	ruler->moveFirstLine(firstLineData->value() / m_docUnitRatio);
	emit tabrulerChanged();
	double a, b, value;
	int c;
	firstLineData->getValues(&a, &b, &c, &value);
	emit firstLineChanged(value);
}

void Tabruler::setLeftIndentData(double t)
{
//	bool sigBlocked = leftIndentData->blockSignals(true);
	leftIndentData->setValue(t * m_docUnitRatio);
//	leftIndentData->blockSignals(sigBlocked);
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
	ruler->moveLeftIndent(leftIndentData->value() / m_docUnitRatio);
	emit tabrulerChanged();
	double a, b, value;
	int c;
	leftIndentData->getValues(&a, &b, &c, &value);
	emit leftIndentChanged(value);
}

QList<ParagraphStyle::TabRecord> Tabruler::getTabVals() const
{
	return ruler->tabValues;
}

double Tabruler::getFirstLine() const
{
	return firstLineData->value() / m_docUnitRatio;
}

double Tabruler::getLeftIndent() const
{
	return leftIndentData->value() / m_docUnitRatio;
}

void Tabruler::setRightIndentData(double t)
{
	bool sigBlocked = rightIndentData->blockSignals(true);
	rightIndentData->setValue(t * m_docUnitRatio);
	rightIndentData->blockSignals(sigBlocked);
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

double Tabruler::getRightIndent() const
{
	return rightIndentData->value() / m_docUnitRatio;
}

void Tabruler::slotMouseReleased()
{
	emit mouseReleased();
}
