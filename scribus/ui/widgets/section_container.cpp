#include "section_container.h"

#include <QPainter>
#include <QPen>
#include <QMouseEvent>
#include <QToolButton>
#include "iconmanager.h"
#include "scribusapp.h"



/* ********************************************************************************* *
 *
 * Constructor + Setup
 *
 * ********************************************************************************* */

SectionContainer::SectionContainer(QWidget *parent) : SectionContainer("Title", true, parent){}

SectionContainer::SectionContainer(QString title, bool isCollapsible, QWidget *parent)
    : QWidget{parent}
{
	widgetHeader = new SectionContainerHeader(title);
	iconCollapsed = QIcon();
	iconExpanded = QIcon();
	boolIsCollapsible = isCollapsible;
	boolIsCollapsed = false;
	boolHasStyle = true;

    // Main Layout
	mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0,0,0,0);
	mainLayout->addWidget(widgetHeader);

    // Size Policy
    QSizePolicy sizePol = QSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::Maximum );
	setSizePolicy(sizePol);

	iconSetChange();

    // initial Setup
    setIsCollapsible(isCollapsible);
    connectSlots();

}

void SectionContainer::connectSlots()
{
	connect( widgetHeader->buttonCollapse, &QPushButton::clicked,this, &SectionContainer::toggleCollpasedState );
	connect( widgetHeader, &SectionContainerHeader::toggleCollpasedState,this, &SectionContainer::toggleCollpasedState );
	connect( ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()) );
}

/* ********************************************************************************* *
 *
 * Properties
 *
 * ********************************************************************************* */

void SectionContainer::setText(QString text)
{
	widgetHeader->labelTitle->setText(text);
}

QString SectionContainer::text() const
{
	return widgetHeader->labelTitle->text();
}

void SectionContainer::setFont(QFont font)
{
	widgetHeader->labelTitle->setFont(font);
    QWidget::setFont(font);
}

QFont SectionContainer::font()
{
	return widgetHeader->font();
}

void SectionContainer::setHasStyle(bool isSimple)
{
    boolHasStyle = isSimple;
	widgetHeader->boolHasStyle = isSimple;
    update();
}

bool SectionContainer::hasStyle()
{
    return boolHasStyle;
}

void SectionContainer::setIsCollapsible(bool isCollapsible)
{
    boolIsCollapsible = isCollapsible;
	widgetHeader->buttonCollapse->setVisible(isCollapsible);
}

bool SectionContainer::isCollapsible()
{
    return boolIsCollapsible;
}

bool SectionContainer::isCollapsed()
{
    return boolIsCollapsed;
}

void SectionContainer::setIsCollapsed(bool state)
{
	if(isCollapsible() == false || mainLayout->count() != 2)
		return;

	QWidget *widget = mainLayout->itemAt(1)->widget();
    boolIsCollapsed = state;

	if( boolIsCollapsed )
	{
		widget->hide();
		widgetHeader->buttonCollapse->setIcon(iconCollapsed);
	}
	else
	{
		widget->show();
		widgetHeader->buttonCollapse->setIcon(iconExpanded);
    }

    emit collapsedState(boolIsCollapsed);
}

void SectionContainer::setCollapseIcons(QIcon collapsed, QIcon expanded)
{
	iconCollapsed = collapsed;
	iconExpanded = expanded;

	if( boolIsCollapsed )
		widgetHeader->buttonCollapse->setIcon(iconCollapsed);
	else
		widgetHeader->buttonCollapse->setIcon(iconExpanded);
}

void SectionContainer::setWidget(QWidget *widget)
{
	// Has no body widget
	if(mainLayout->count() == 1)
	{
		mainLayout->insertWidget(1, widget);
	}
	// Has body widget
	else if(mainLayout->count() == 2)
	{
		QWidget *oldWidget = mainLayout->itemAt(1)->widget();
		mainLayout->replaceWidget(oldWidget, widget);
	}
	else
		return;

	// Override widget policy to fit into the body container
	widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
//	widget->installEventFilter(this);
	widget->adjustSize();

}

QWidget* SectionContainer::widget()
{
	 return mainLayout->count() == 2 ? mainLayout->itemAt(1)->widget() : nullptr;
}

/* ********************************************************************************* *
 *
 * Slots
 *
 * ********************************************************************************* */

void SectionContainer::toggleCollpasedState()
{
    if(isCollapsible() == false) return;
	setIsCollapsed(!boolIsCollapsed);
}

void SectionContainer::iconSetChange()
{
	IconManager &iconManager = IconManager::instance();
	setCollapseIcons(iconManager.loadPixmap("collapse"), iconManager.loadPixmap("expand"));
}

/* ********************************************************************************* *
 *
 * Header
 *
 * ********************************************************************************* */

void SectionContainer::addHeaderSuffixWidget(QWidget *widget)
{
	insertHeaderSuffixWidget(-1, widget);
}

void SectionContainer::insertHeaderSuffixWidget(int index, QWidget *widget)
{
	QToolButton *toolButton = static_cast<QToolButton*>(widget);

	if(toolButton)
	{
		toolButton->setAutoRaise(true);
		widgetHeader->layoutHeaderSuffix->insertWidget(index, toolButton);
	}
	else
	{
		widgetHeader->layoutHeaderSuffix->insertWidget(index, widget);
	}
}

void SectionContainer::removeHeaderSuffixWidget(QWidget *widget)
{
	widgetHeader->layoutHeaderSuffix->removeWidget( widget );
	widget->deleteLater();
}

void SectionContainer::addHeaderSuffixSpacing(int size)
{
	insertHeaderSuffixSpacing(-1, size);
}

void SectionContainer::insertHeaderSuffixSpacing(int index, int size)
{
	widgetHeader->layoutHeaderSuffix->insertSpacing(index, size);
}

void SectionContainer::addHeaderPrefixWidget(QWidget *widget)
{
	insertHeaderPrefixWidget(-1, widget);
}

void SectionContainer::insertHeaderPrefixWidget(int index, QWidget *widget)
{
	QToolButton *toolButton = static_cast<QToolButton*>(widget);

	if(toolButton)
	{
		toolButton->setAutoRaise(true);
		widgetHeader->layoutHeaderPrefix->insertWidget(index, toolButton);
	}
	else
	{
		widgetHeader->layoutHeaderPrefix->insertWidget(index, widget);
	}
}

void SectionContainer::removeHeaderPrefixWidget(QWidget *widget)
{
	widgetHeader->layoutHeaderPrefix->removeWidget( widget );
	widget->deleteLater();
}

void SectionContainer::addHeaderPrefixSpacing(int size)
{
	insertHeaderPrefixSpacing(-1, size);
}

void SectionContainer::insertHeaderPrefixSpacing(int index, int size)
{
	widgetHeader->layoutHeaderPrefix->insertSpacing(index, size);
}


/* ********************************************************************************* *
 *
 * Events
 *
 * ********************************************************************************* */

void SectionContainer::paintEvent(QPaintEvent *event)
{
	QWidget::paintEvent(event);

	if(!boolHasStyle)
	{
		int lineWidth = 1;
		QColor bottomLineColor( palette().color(QPalette::Dark) );

		QPainter painter( this );

		// Bottom Line
		painter.setPen( QPen(bottomLineColor, lineWidth) );
		painter.drawLine( 8, this->height() - lineWidth, this->width() - 8, this->height() - lineWidth );
	}

}

//bool SectionContainer::eventFilter(QObject *object, QEvent *event)
//{
//	QWidget *widget = qobject_cast<QWidget*>(object);

//	switch(event->type())
//	{
//	case QEvent::Resize:
//	{
//		if(widget)
//			qDebug() << "height" << widget->height() << widget->objectName();

//		qDebug() << "resize body widget";
//		break;
//	}
//	default:
//		break;
//	}

//	return false;
//}

/* ********************************************************************************* *
 *
 *
 * SECTION CONTAINER HEADER
 *
 *
 * ********************************************************************************* */

/* ********************************************************************************* *
 *
 * Constructor + Setup
 *
 * ********************************************************************************* */

SectionContainerHeader::SectionContainerHeader(QWidget *parent) : SectionContainerHeader("Title", parent){}

SectionContainerHeader::SectionContainerHeader(QString title, QWidget *parent)
{

	int padding = 6;
	boolHasStyle = true;

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

	// Buttons
	buttonCollapse->setFlat(true);
	buttonCollapse->setFixedSize(20, 20);
//	buttonCollapse->adjustSize(); // needed to calculate the header height later
//	buttonCollapse->setMaximumWidth(buttonCollapse->height());

	// Header Prefix layout
	layoutHeaderPrefix->setContentsMargins( 0,0,0,0 );
	layoutHeaderPrefix->setSpacing(padding);

	// Header Suffix Layout
	layoutHeaderSuffix->setContentsMargins( 0,0,0,0 );
	layoutHeaderSuffix->setSpacing(padding);

	// Header Layout
	layoutHeader->addWidget( buttonCollapse );
	layoutHeader->addLayout( layoutHeaderPrefix );
	layoutHeader->addWidget( labelTitle );
	layoutHeader->addStretch( 0 );
	layoutHeader->addLayout( layoutHeaderSuffix );
	layoutHeader->setContentsMargins( 4, padding / 2, 4, padding / 2 );

	// Header Widget
	this->setLayout(layoutHeader);
	this->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Maximum );
	this->setMaximumHeight(buttonCollapse->height() + padding);
}

/* ********************************************************************************* *
 *
 * Events
 *
 * ********************************************************************************* */

void SectionContainerHeader::paintEvent(QPaintEvent *event)
{

	if(boolHasStyle)
	{
		int lineWidth = 1;
//		QColor topLineColor( palette().color(QPalette::Mid) );
		QColor bottomLineColor( palette().color(QPalette::Dark) );

		QColor bgColor( palette().color(QPalette::WindowText) );
		bgColor.setAlphaF(0.07f);


		int headerheight = this->height();
		QRect headerRect(0, 0, this->width(), headerheight - lineWidth);

		QPainter painter( this );

		painter.setPen( Qt::NoPen );
		painter.setBrush(bgColor);
		painter.drawRect(headerRect);

		painter.setBrush(Qt::NoBrush);

		// Top Line
//		painter.setPen( QPen(topLineColor, lineWidth) );
//		painter.drawLine( 0, 0,this->width(), 0 );

		// Bottom Line
		painter.setPen( QPen(bottomLineColor, lineWidth) );
		painter.drawLine( 0, headerheight - lineWidth, this->width(), headerheight - lineWidth );

	}
}

void SectionContainerHeader::mouseDoubleClickEvent(QMouseEvent *mouseEvent)
{
	QWidget::mouseDoubleClickEvent(mouseEvent);

	if (mouseEvent->button() == Qt::LeftButton)
	{
		emit toggleCollpasedState();
	}
}
