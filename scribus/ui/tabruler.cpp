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
#ifdef Q_OS_MACOS
#include <QFontDatabase>
#endif

#include "commonstrings.h"
#include "iconmanager.h"
#include "scribusapp.h"
#include "scribusstructs.h"
#include "scrspinbox.h"
#include "units.h"
#include "util.h"
#include "util_gui.h"

constexpr int bottomline = 24;
constexpr int midline = bottomline / 2;
constexpr int tabline = bottomline - 10;
constexpr int scaleS = bottomline - 4;
constexpr int scaleM = bottomline - 8;
constexpr int scaleL = bottomline - 12;
constexpr int textline = scaleL;

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

	QFont ff = font();
#ifdef Q_OS_MACOS
	ff.setPointSize(QFontDatabase::systemFont(QFontDatabase::SmallestReadableFont).pointSize());
#else
	ff.setPointSize(QFont().pointSize() * .75);
#endif
	setFont(ff);

	const QPalette& palette = this->palette();
	const QColor& textColor = palette.color(QPalette::Text);
	const QColor& selectedColor = blendColor(isDarkColor(palette.color(QPalette::Base)), QColor(255, 117, 102), QColor(255, 71, 51));
	const QColor& backgroundColor = palette.color(QPalette::Base);

	QPainter p;
	p.begin(this);
	p.fillRect(rect(), backgroundColor);
	p.drawLine(0, bottomline, width(), bottomline );
	p.translate(-offset, 0);
	p.setBrush(textColor);
	p.setFont(font());
	p.setPen(QPen(textColor, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));

	for (xl = 0; xl < width() + offset; xl += m_iter)
	{
		if (xl < offset)
			continue;
		p.drawLine(qRound(xl), scaleS, qRound(xl), bottomline);
	}

	for (xl = 0; xl < width() + (m_iter2 / 2) + offset; xl += (m_iter2 / 2))
	{
		if (xl < offset)
			continue;
		p.drawLine(qRound(xl), scaleM, qRound(xl), bottomline);
	}

	for (xl = 0; xl < width() + (m_iter2 / 2) + offset; xl += m_iter2)
	{
		if (xl < offset - 20) // -20px buffer to draw number that start outside the clipping rect
			continue;
		p.drawLine(qRound(xl), scaleL, qRound(xl), bottomline);
		switch (unitIndex)
		{
			case SC_IN:
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
				p.drawText(qRound(xl + 2), textline, tx);
				break;
			}
			case SC_P:
				p.drawText(qRound(xl + 2), textline, QString::number(xl / m_iter));
				break;
			case SC_CM:
				p.drawText(qRound(xl + 2), textline, QString::number(xl / m_iter / 10));
				break;
			default:
				p.drawText(qRound(xl + 2), textline, QString::number(xl / m_iter * 10));
				break;
		}
	}

	for (int i = 0; i < tabValues.count(); i++)
	{
		if (i == actTab)
			p.setPen(QPen(selectedColor, 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
		else
			p.setPen(QPen(textColor, 2, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
		switch (tabValues[i].tabType)
		{
			case ParagraphStyle::LeftTab:
				p.drawLine(qRound(tabValues[i].tabPosition), tabline, qRound(tabValues[i].tabPosition), bottomline - 1);
				p.drawLine(qRound(tabValues[i].tabPosition), bottomline - 1, qRound(tabValues[i].tabPosition + 8), bottomline - 1);
				break;
			case ParagraphStyle::RightTab:
				p.drawLine(qRound(tabValues[i].tabPosition), tabline, qRound(tabValues[i].tabPosition), bottomline - 1);
				p.drawLine(qRound(tabValues[i].tabPosition), bottomline - 1, qRound(tabValues[i].tabPosition - 8), bottomline - 1);
				break;
			case ParagraphStyle::CommaTab:
			case ParagraphStyle::DotTab:
				p.drawLine(qRound(tabValues[i].tabPosition), tabline, qRound(tabValues[i].tabPosition), bottomline - 1);
				p.drawLine(qRound(tabValues[i].tabPosition - 4), bottomline - 1, qRound(tabValues[i].tabPosition + 4), bottomline - 1);
				p.drawLine(qRound(tabValues[i].tabPosition + 3), bottomline - 3, qRound(tabValues[i].tabPosition + 2), bottomline - 3);
				break;
			case ParagraphStyle::CenterTab:
				p.drawLine(qRound(tabValues[i].tabPosition), tabline, qRound(tabValues[i].tabPosition), bottomline - 1);
				p.drawLine(qRound(tabValues[i].tabPosition - 4), bottomline - 1, qRound(tabValues[i].tabPosition + 4), bottomline - 1);
				break;
			default:
				break;
		}
	}

	if (haveInd)
	{
		// First Indent
		p.setPen(QPen(textColor, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
		p.drawLine(qRound(firstLine + leftIndent), tabline - 2, qRound(firstLine + leftIndent), bottomline);
		p.setPen(Qt::NoPen);
		p.setRenderHints(QPainter::Antialiasing, true);
		p.drawRect(QRect(qRound(firstLine + leftIndent), tabline - 8, 8, 6));

		// Left Margin
		QPolygon cr2;
		cr2.setPoints(3, qRound(leftIndent), tabline, qRound(leftIndent + 8), tabline, qRound(leftIndent), bottomline);
		p.drawPolygon(cr2);
	}
	p.end();
}

void RulerT::mousePressEvent(QMouseEvent *m)
{
	QRect fpo;
	mousePressed = true;
	int mPosX = m->position().x();
	QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
	rulerCode = 0;
	if (haveInd)
	{
		fpo = QRect(static_cast<int>(firstLine + leftIndent - offset) - 4, 0, 8, 12);
		if (fpo.contains(m->pos()))
		{
			rulerCode = 1;
			mouseX = mPosX;
			return;
		}
		fpo = QRect(static_cast<int>(leftIndent - offset) - 4, 12, 8, 12);
		if (fpo.contains(m->pos()))
		{
			rulerCode = 2;
			mouseX = mPosX;
			return;
		}
	}
	for (qsizetype i = 0; i < tabValues.count(); ++i)
	{
		fpo = QRect(static_cast<int>(tabValues[i].tabPosition - offset) - 3, 15, 8, 8);
		if (fpo.contains(m->pos()))
		{
			rulerCode = 3;
			actTab = static_cast<int>(i);
			mouseX = mPosX;
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
		tb.tabPosition = static_cast<double>(mPosX + offset);
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
	mouseX = mPosX;
}

void RulerT::mouseReleaseEvent(QMouseEvent *m)
{
	mousePressed = false;
	QApplication::restoreOverrideCursor();
	int mPosY = m->position().y();
	if ((mPosY < height()) && (mPosY > 0))
	{
		if (rulerCode == 3  && (m->button() == Qt::RightButton))
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
	int mPosX = m->position().x();
	int mPosY = m->position().y();
	if (mousePressed && (mPosY < height()) && (mPosY > 0) && (mPosX > 0) && (mPosX < width()))
	{
		QApplication::changeOverrideCursor(QCursor(Qt::SizeHorCursor));
		switch (rulerCode)
		{
			case 1:
				firstLine -= mouseX - mPosX;
				if (firstLine + leftIndent + offset < offset)
					firstLine += mouseX - mPosX;
				if (firstLine + leftIndent > m_rulerWidth)
					firstLine  = m_rulerWidth - leftIndent;
				emit firstLineMoved(firstLine);
				update();
				break;
			case 2:
				oldInd = leftIndent+firstLine;
				leftIndent -= mouseX - mPosX;
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
				tabValues[actTab].tabPosition -= mouseX - mPosX;
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
		mouseX = mPosX;
		return;
	}
	if ((!mousePressed) && (mPosY < height()) && (mPosY > 0) && (mPosX > 0) && (mPosX < width()))
	{
		setCursor(IconManager::instance().loadCursor("cursor-tabulator", 3));
		if (haveInd)
		{
			fpo = QRect(static_cast<int>(firstLine + leftIndent - offset) - 4, 0, 8, midline);
			if (fpo.contains(m->pos()))
			{
				setCursor(QCursor(Qt::SizeHorCursor));
				return;
			}
			fpo = QRect(static_cast<int>(leftIndent - offset) - 4, midline, 8, midline);
			if (fpo.contains(m->pos()))
			{
				setCursor(QCursor(Qt::SizeHorCursor));
				return;
			}
		}
		for (qsizetype i = 0; i < tabValues.count(); ++i)
		{
			fpo = QRect(static_cast<int>(tabValues[i].tabPosition-offset) - 3, tabline, 8, 8);
			if (fpo.contains(m->pos()))
			{
				setCursor(QCursor(Qt::SizeHorCursor));
				return;
			}
		}
	}
	if (mousePressed && ((mPosY > height()) || (mPosY < 0) || (mPosX < 0) || (mPosX > width())))
		QApplication::changeOverrideCursor(IconManager::instance().loadCursor("cursor-remove-point", 1, 1));
}

void RulerT::wheelEvent(QWheelEvent* m)
{
	QPoint numPixels = m->pixelDelta();

	if (numPixels.y() > 0 || numPixels.x() > 0)
		decreaseOffset();

	if (numPixels.y() < 0 || numPixels.x() < 0)
		increaseOffset();

	m->accept();
}

void RulerT::leaveEvent(QEvent*)
{
	if (mousePressed)
	{
		if (rulerCode == 3)
			QApplication::changeOverrideCursor(IconManager::instance().loadCursor("cursor-remove-point", 1, 1));
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
	qsizetype gg = tabValues.count() - 1;
	qsizetype g = gg;
	tabValues.removeAt(actTab);
	for (qsizetype i = tabValues.count() - 1; i > -1; --i)
	{
		if (tb.tabPosition < tabValues[i].tabPosition)
			g = i;
	}
	actTab = g;
	if (gg == g)
	{
		tabValues.append(tb);
		actTab = static_cast<int>(tabValues.count() - 1);
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
	m_haveFirst = haveFirst;
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
	layout1->setSpacing(16);
	layout1->setAlignment( Qt::AlignTop );
	typeCombo = new QComboBox(this);
	typeCombo->setEditable(false);
	layout1->addWidget( typeCombo );

	tabData = new ScrSpinBox( 0, ww / m_docUnitRatio, this, unit );
	tabData->setValue(0);
	positionLabel = new FormWidget();
	positionLabel->setDirection(FormWidget::Left);
	positionLabel->setUseSmallFont(false);
	positionLabel->addWidget(tabData);
	layout1->addWidget( positionLabel );

	tabFillCombo = new QComboBox(this);
	tabFillCombo->setEditable(false);
	tabFillComboT = new FormWidget();
	tabFillComboT->setDirection(FormWidget::Left);
	tabFillComboT->setUseSmallFont(false);
	tabFillComboT->addWidget(tabFillCombo);
	layout1->addWidget( tabFillComboT );

	clearOneButton = new QToolButton( this );

	clearButton = new QToolButton( this );

	FormWidget *clearLayout = new FormWidget();
	clearLayout->setSpace(4);
	clearLayout->setLabelVisibility(false);
	clearLayout->addWidget(clearOneButton);
	clearLayout->addWidget(clearButton);
	layout1->addWidget(clearLayout);

	indentLayout = new QHBoxLayout;
	indentLayout->setContentsMargins(0, 0, 0, 0);
	indentLayout->setSpacing(16);
	if (m_haveFirst)
	{
		firstLineData = new ScrSpinBox( -3000, ww / m_docUnitRatio, this, unit);
		firstLineData->setValue(0);
		firstLineLabel = new FormWidget(this);
		firstLineLabel->setText("");
		firstLineLabel->addWidget(firstLineData);
		indentLayout->addWidget( firstLineLabel );

		leftIndentData = new ScrSpinBox( 0, ww / m_docUnitRatio, this, unit);
		leftIndentData->setValue(0);
		leftIndentLabel = new FormWidget(this);
		leftIndentLabel->setText("");
		leftIndentLabel->addWidget(leftIndentData);
		indentLayout->addWidget(leftIndentLabel);

		rightIndentData = new ScrSpinBox(0, ww / m_docUnitRatio, this, unit);
		rightIndentData->setValue(0);
		rightIndentLabel = new FormWidget(this);
		rightIndentLabel->setText("");
		rightIndentLabel->addWidget(rightIndentData);
		indentLayout->addWidget(rightIndentLabel);

	}
	if (!m_haveFirst)
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

	iconSetChange();
	languageChange();

	connect(rulerScrollL, SIGNAL(clicked()), ruler, SLOT(decreaseOffset()));
	connect(rulerScrollR, SIGNAL(clicked()), ruler, SLOT(increaseOffset()));
	connect(rulerScrollL, SIGNAL(released()), this, SLOT(resetOFfL()));
	connect(rulerScrollR, SIGNAL(released()), this, SLOT(resetOFfR()));
	connect(ruler, SIGNAL(typeChanged(int)) , this, SLOT(setTabType(int)));
	connect(ruler, SIGNAL(fillCharChanged(QChar)) , this, SLOT(setTabFillChar(QChar)));
	connect(typeCombo, SIGNAL(activated(int)), this, SLOT(setType()));
	connect(tabFillCombo, SIGNAL(activated(int)), this, SLOT(setFillChar()));
	connect(tabFillCombo, SIGNAL(editTextChanged(QString)), this, SLOT(setCustomFillChar(QString)));
	connect(ruler, SIGNAL(tabMoved(double)) , this, SLOT(setTabData(double)));
	connect(ruler, SIGNAL(tabSelected()), this, SLOT(tabAdded()));
	connect(ruler, SIGNAL(newTab()), this, SLOT(tabAdded()));
	connect(ruler, SIGNAL(noTabs()), this, SLOT(lastTabRemoved()));
	connect(tabData, SIGNAL(valueChanged(double)), this, SLOT(setTab()));
	connect(clearButton, SIGNAL(clicked()), this, SLOT(clearAll()));
	connect(clearOneButton, SIGNAL(clicked()), this, SLOT(clearOne()));

	if (m_haveFirst)
	{
		connect(ruler, SIGNAL(firstLineMoved(double)) , this, SLOT(setFirstLineData(double)));
		connect(ruler, SIGNAL(leftIndentMoved(double)) , this, SLOT(setLeftIndentData(double)));
		connect(ruler, SIGNAL(mouseReleased()), this, SIGNAL(tabrulerChanged()));
		connect(ruler, SIGNAL(mouseReleased()), this, SLOT(slotMouseReleased()));
		connect(firstLineData, SIGNAL(valueChanged(double)), this, SLOT(setFirstLine()));
		connect(leftIndentData, SIGNAL(valueChanged(double)), this, SLOT(setLeftIndent()));
		connect(rightIndentData, SIGNAL(valueChanged(double)), this, SLOT(setRightIndent()));
	}

	if (unit == SC_INCHES)
	{
		if (m_haveFirst)
		{
			firstLineData->setDecimals(4);
			leftIndentData->setDecimals(4);
			rightIndentData->setDecimals(4);
		}
		tabData->setDecimals(4);
	}

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
		firstLineLabel->setPixmap(iconManager.loadPixmap("paragraph-indent-firstline"));

	if (leftIndentLabel)
		leftIndentLabel->setPixmap(iconManager.loadPixmap("paragraph-indent-left"));

	if (rightIndentLabel)
		rightIndentLabel->setPixmap(iconManager.loadPixmap("paragraph-indent-right"));

	clearOneButton->setIcon(iconManager.loadIcon("delete-selected"));
	clearButton->setIcon(iconManager.loadIcon("edit-delete"));
}

void Tabruler::languageChange()
{
	IconManager &im = IconManager::instance();
	int  oldTypeComboIndex = typeCombo->currentIndex();
	bool typeComboBlocked = typeCombo->blockSignals(true);
	typeCombo->clear();
	typeCombo->addItem(im.loadIcon("tabulator-left"), tr( "Left" ), ParagraphStyle::LeftTab );
	typeCombo->addItem(im.loadIcon("tabulator-center"), tr( "Center" ), ParagraphStyle::CenterTab );
	typeCombo->addItem(im.loadIcon("tabulator-comma"), tr( "Comma" ), ParagraphStyle::CommaTab );
	typeCombo->addItem(im.loadIcon("tabulator-dot"), tr( "Period" ), ParagraphStyle::DotTab );
	typeCombo->addItem(im.loadIcon("tabulator-right"), tr( "Right" ), ParagraphStyle::RightTab );
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

	clearButton->setToolTip( tr( "Delete all Tabulators" ) );
	clearOneButton->setToolTip( tr("Delete selected Tabulator"));

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
	typeCombo->setCurrentIndex(typeCombo->findData(t));
	emit tabrulerChanged();
	emit tabsChanged();
}

void Tabruler::setType()
{
	bool sigBlocked = typeCombo->blockSignals(true);
	ruler->changeTab(typeCombo->currentData().toInt());
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
