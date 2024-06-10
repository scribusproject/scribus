#include "section_container.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPen>
#include <QToolButton>

#ifndef EXCLUDE_FOR_DESIGNER_PLUGIN
#include "iconmanager.h"
#include "scribusapp.h"
#include "util_gui.h"
#include "prefsmanager.h"
#include "prefsfile.h"
#endif

/* *********************************************************************************
 * *
 *
 * Constructor + Setup
 *
 * *********************************************************************************
 */

SectionContainer::SectionContainer(QWidget *parent)
	: SectionContainer("Title", "", true, parent) {}

SectionContainer::SectionContainer(QString title, QString objectName, bool isCollapsible, bool isExpanded,
								   QWidget *parent)
	: QWidget{parent}
{

	if(!objectName.isEmpty())
		setObjectName(objectName);

	header = new SectionContainerHeader(title);
	m_isCollapsed = !isExpanded;

	// Main Layout
	mainLayout = new QVBoxLayout(this);
	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->addWidget(header);
	mainLayout->addWidget(body);

	// Size Policy
	QSizePolicy sizePol(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
	setSizePolicy(sizePol);

	iconSetChange();

	// initial Setup
	setIsCollapsible(isCollapsible);
	setIsCollapsedInternal(m_isCollapsed);
	connectSlots();
}

void SectionContainer::connectSlots()
{
	connect(header->buttonCollapse, &QPushButton::clicked, this,
			&SectionContainer::toggleCollpasedState);
	connect(header, &SectionContainerHeader::toggleCollpasedState, this,
			&SectionContainer::toggleCollpasedState);

#ifndef EXCLUDE_FOR_DESIGNER_PLUGIN
	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));
#endif
}

/* *********************************************************************************
 * *
 *
 * Properties
 *
 * *********************************************************************************
 */

void SectionContainer::setText(QString text)
{
	header->labelTitle->setText(text);
	header->labelTitle->adjustSize();
}

QString SectionContainer::text() const
{
	return header->labelTitle->text();
}

void SectionContainer::setFont(QFont font)
{
	header->labelTitle->setFont(font);
	QWidget::setFont(font);
}

QFont SectionContainer::font() const
{
	return header->font();
}

void SectionContainer::setHasStyle(bool isSimple)
{
	m_hasStyle = isSimple;
	header->m_hasStyle = isSimple;
	update();
}

bool SectionContainer::hasStyle() const
{
	return m_hasStyle;
}

void SectionContainer::setCanSaveState(bool saveState)
{
	m_canSaveState = saveState;
}

bool SectionContainer::canSaveState()
{
	return m_canSaveState;
}

void SectionContainer::setIsCollapsible(bool isCollapsible)
{
	m_isCollapsible = isCollapsible;
	header->buttonCollapse->setVisible(isCollapsible);
}

bool SectionContainer::isCollapsible() const
{
	return m_isCollapsible;
}

bool SectionContainer::isCollapsed() const
{
	return m_isCollapsed;
}

void SectionContainer::setIsCollapsed(bool state)
{
	if (isCollapsible() == false)
		return;

	setIsCollapsedInternal(state);

#ifndef EXCLUDE_FOR_DESIGNER_PLUGIN
	savePreferences();
#endif

	emit collapsedStateChanged(m_isCollapsed);
	emit sizeChanged();

}

void SectionContainer::setIsCollapsedInternal(bool state)
{
	if (isCollapsible() == false)
		return;

	m_isCollapsed = state;

	header->buttonCollapse->setIcon(m_isCollapsed ? iconCollapsed : iconExpanded);
	body->setVisible(!m_isCollapsed);

	resize(calculateSize());
}

void SectionContainer::collapse()
{
	setIsCollapsed(true);
}

void SectionContainer::expand()
{
	setIsCollapsed(false);
}

void SectionContainer::setCollapseIcons(QIcon collapsed, QIcon expanded)
{
	iconCollapsed = collapsed;
	iconExpanded = expanded;

	header->buttonCollapse->setIcon(m_isCollapsed ? iconCollapsed : iconExpanded);
}

void SectionContainer::setBodyEnabled(bool enabled)
{
	body->setEnabled(enabled);
}

void SectionContainer::removeWidget()
{
	if (body)
		body->removeEventFilter(this);
	body = new QWidget;
	mainLayout->addWidget(body);
	m_hasBody = false;
}

void SectionContainer::setWidget(QWidget *widget)
{
	body = widget;
	body->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
	body->adjustSize();
	body->installEventFilter(this);

	mainLayout->addWidget(body);
	m_hasBody = true;

	resize(calculateSize());
	emit sizeChanged();
}

QWidget *SectionContainer::widget()
{
	return body;
}

bool SectionContainer::hasWidget()
{
	return m_hasBody;
}

void SectionContainer::setLayout(QLayout *layout)
{
	body->setLayout(layout);
}

QLayout *SectionContainer::layout() const
{
	return body->layout();
}

QSize SectionContainer::minimumSizeHint() const
{
	QSize s(QWidget::minimumSizeHint());

	if (isCollapsed())
		s.setHeight(header->height());

	return s;
}

QSize SectionContainer::calculateSize()
{
	QSize s = minimumSizeHint();
	s.setWidth(this->width());

	return s;
}

/* *********************************************************************************
 * *
 *
 * Slots
 *
 * *********************************************************************************
 */

void SectionContainer::toggleCollpasedState()
{
	if (isCollapsible() == false)
		return;
	setIsCollapsed(!m_isCollapsed);
}

void SectionContainer::iconSetChange()
{
#ifndef EXCLUDE_FOR_DESIGNER_PLUGIN
	IconManager &iconManager = IconManager::instance();
	setCollapseIcons(iconManager.loadPixmap("collapse"),
					 iconManager.loadPixmap("expand"));
#endif
}

/* *********************************************************************************
 * *
 *
 * Header
 *
 * *********************************************************************************
 */

void SectionContainer::addHeaderSuffixWidget(QWidget *widget)
{
	insertHeaderSuffixWidget(-1, widget);
}

void SectionContainer::insertHeaderSuffixWidget(int index, QWidget *widget)
{
	QToolButton *toolButton = dynamic_cast<QToolButton *>(widget);
	if (toolButton)
	{
		toolButton->setAutoRaise(true);
		header->layoutHeaderSuffix->insertWidget(index, toolButton);
	}
	else
		header->layoutHeaderSuffix->insertWidget(index, widget);
}

void SectionContainer::removeHeaderSuffixWidget(QWidget *widget)
{
	header->layoutHeaderSuffix->removeWidget(widget);
	widget->deleteLater();
}

void SectionContainer::addHeaderSuffixSpacing(int size)
{
	insertHeaderSuffixSpacing(-1, size);
}

void SectionContainer::insertHeaderSuffixSpacing(int index, int size)
{
	header->layoutHeaderSuffix->insertSpacing(index, size);
}

void SectionContainer::addHeaderPrefixWidget(QWidget *widget)
{
	insertHeaderPrefixWidget(-1, widget);
}

void SectionContainer::insertHeaderPrefixWidget(int index, QWidget *widget)
{
	QToolButton *toolButton = dynamic_cast<QToolButton *>(widget);
	if (toolButton)
	{
		toolButton->setAutoRaise(true);
		header->layoutHeaderPrefix->insertWidget(index, toolButton);
	}
	else
		header->layoutHeaderPrefix->insertWidget(index, widget);
}

void SectionContainer::removeHeaderPrefixWidget(QWidget *widget)
{
	header->layoutHeaderPrefix->removeWidget(widget);
	widget->deleteLater();
}

void SectionContainer::addHeaderPrefixSpacing(int size)
{
	insertHeaderPrefixSpacing(-1, size);
}

void SectionContainer::insertHeaderPrefixSpacing(int index, int size)
{
	header->layoutHeaderPrefix->insertSpacing(index, size);
}

bool SectionContainer::eventFilter(QObject *object, QEvent *event)
{
	if (event->type() == QEvent::EnabledChange)
	{
		QWidget *wdg = qobject_cast<QWidget*>(object);
		header->labelTitle->setEnabled(wdg->isEnabled());

		return true;
	}

	return false;
}

/* *********************************************************************************
 * *
 *
 * Events
 *
 * *********************************************************************************
 */

void SectionContainer::paintEvent(QPaintEvent *event)
{
	QWidget::paintEvent(event);

	if (m_hasStyle)
		return;

	int lineWidth = 1;
	QColor bottomLineColor(palette().color(QPalette::Dark));

	QPainter painter(this);

	// Bottom Line
	painter.setPen(QPen(bottomLineColor, lineWidth));
	painter.drawLine(8, this->height() - lineWidth, this->width() - 8,
					 this->height() - lineWidth);
}

#ifndef EXCLUDE_FOR_DESIGNER_PLUGIN
void SectionContainer::savePreferences()
{
	if(!canSaveState())
		return;

	if(objectName().isEmpty())
		return;

	PrefsContext *prefCollapse = PrefsManager::instance().prefsFile->getContext(objectName());
	prefCollapse->set("collapsed", m_isCollapsed);

	PrefsManager::instance().prefsFile->write();
}

void SectionContainer::restorePreferences()
{
	if(!canSaveState())
		return;

	if(objectName().isEmpty())
		return;

	PrefsContext *prefCollapse = PrefsManager::instance().prefsFile->getContext(objectName());
	bool isCollapsed = prefCollapse->getBool("collapsed", m_isCollapsed);

	setIsCollapsedInternal(isCollapsed);
}
#endif

/* *********************************************************************************
 * *
 *
 *
 * SECTION CONTAINER HEADER
 *
 *
 * *********************************************************************************
 */

/* *********************************************************************************
 * *
 *
 * Constructor + Setup
 *
 * *********************************************************************************
 */

SectionContainerHeader::SectionContainerHeader(QWidget *parent)
	: SectionContainerHeader("Title", parent) {}

SectionContainerHeader::SectionContainerHeader(QString title, QWidget *parent)
{
	int padding = 6;
	m_hasStyle = true;

	layoutHeader = new QHBoxLayout;
	layoutHeaderPrefix = new QHBoxLayout;
	layoutHeaderSuffix = new QHBoxLayout;
	labelTitle = new QLabel(title);
	buttonCollapse = new QPushButton;

	// Label
	//	QFont m_font(this->font());
	//	m_font.setBold( true );
	//	labelTitle->setFont(m_font);
	labelTitle->setText(title);
	labelTitle->setBuddy(this);
	labelTitle->adjustSize();

	// Buttons
	buttonCollapse->setFlat(true);
	buttonCollapse->setFixedSize(20, 20);

	// Header Prefix layout
	layoutHeaderPrefix->setContentsMargins(0, 0, 0, 0);
	layoutHeaderPrefix->setSpacing(padding);

	// Header Suffix Layout
	layoutHeaderSuffix->setContentsMargins(0, 0, 0, 0);
	layoutHeaderSuffix->setSpacing(padding);

	// Header Layout
	layoutHeader->addWidget(buttonCollapse);
	layoutHeader->addLayout(layoutHeaderPrefix);
	layoutHeader->addWidget(labelTitle);
	layoutHeader->addStretch(0);
	layoutHeader->addLayout(layoutHeaderSuffix);
	layoutHeader->setContentsMargins(4, padding / 2, 4, padding / 2);

	// Header Widget
	this->setLayout(layoutHeader);
	this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
	this->setFixedHeight(buttonCollapse->height() + padding);
}

/* *********************************************************************************
 * *
 *
 * Events
 *
 * *********************************************************************************
 */

void SectionContainerHeader::paintEvent(QPaintEvent *event)
{
	if (!m_hasStyle)
		return;

	int lineWidth = 1;
	//	QColor topLineColor( palette().color(QPalette::Mid) );
	QColor bottomLineColor(palette().color(QPalette::Dark));

#ifndef EXCLUDE_FOR_DESIGNER_PLUGIN
	QColor bgColor = colorByRole(QPalette::WindowText, 0.07, isEnabled());
#else
	QColor bgColor(palette().color(QPalette::WindowText));
	bgColor.setAlphaF(0.07f);
#endif

	int headerHeight = this->height();
	QRect headerRect(0, 0, this->width(), headerHeight - lineWidth);

	QPainter painter(this);

	painter.setPen(Qt::NoPen);
	painter.setBrush(bgColor);
	painter.drawRect(headerRect);

	painter.setBrush(Qt::NoBrush);

	// Top Line
	//	painter.setPen( QPen(topLineColor, lineWidth) );
	//	painter.drawLine( 0, 0,this->width(), 0 );

	// Bottom Line
	painter.setPen(QPen(bottomLineColor, lineWidth));
	painter.drawLine(0, headerHeight - lineWidth, this->width(),
					 headerHeight - lineWidth);
}

void SectionContainerHeader::mousePressEvent(QMouseEvent *mouseEvent)
{
	QWidget::mousePressEvent(mouseEvent);

	if (mouseEvent->button() == Qt::LeftButton)
		emit toggleCollpasedState();
}
