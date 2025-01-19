#include "floatingwindow.h"
#include "iconmanager.h"
#include "scribusapp.h"

#include <QPainter>
#include <QPaintEvent>
#include <QVBoxLayout>
#include <QApplication>
#include <QToolButton>


FloatingWindow::FloatingWindow(QWidget *child, QWidget *parent)
	: QWidget(parent, Qt::Tool | Qt::FramelessWindowHint),
	  m_child(child)
{
	if (m_child == nullptr)
		m_child = new QWidget(this);

	m_handle = new QWidget(this);
	m_handle->installEventFilter(this);

	buttonClose = new QToolButton();
	buttonClose->setToolButtonStyle(Qt::ToolButtonIconOnly);
	buttonClose->setFixedSize(16, 16);
	buttonClose->setAutoRaise(true);

	QHBoxLayout *layoutHeader = new QHBoxLayout();
	layoutHeader->addWidget(m_handle);
	layoutHeader->addWidget(buttonClose);
	layoutHeader->setContentsMargins(8, 8, 8, 8);

	m_layout = new QVBoxLayout();
	m_layout->addLayout(layoutHeader);
	m_layout->addWidget(m_child);
	m_layout->setSpacing(0);
	m_layout->setContentsMargins(1, 1, 1, 1);

	setLayout(m_layout);
	iconSetChange();

	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));
	connect(buttonClose, &QToolButton::pressed, this, &FloatingWindow::hide);
}

QWidget *FloatingWindow::child()
{
	return m_child;
}

QWidget *FloatingWindow::reference()
{
	return m_reference;
}

void FloatingWindow::iconSetChange()
{
	IconManager &iconManager = IconManager::instance();
	buttonClose->setIcon(iconManager.loadIcon("close"));
}

bool FloatingWindow::eventFilter(QObject *obj, QEvent *event)
{
	QWidget *wdg = dynamic_cast<QWidget*>(obj);
	if (!wdg)
		return QWidget::eventFilter(obj, event);

	if (m_handle && (m_handle == wdg))
	{
		switch (event->type())
		{
		case QEvent::MouseButtonPress:
		{
			QMouseEvent *mEvent = (QMouseEvent*) event;
			m_mousePos = mEvent->pos();
		}
			break;
		case QEvent::MouseMove:
		{
			QMouseEvent *mEvent = (QMouseEvent*) event;
			if (mEvent->buttons() & Qt::LeftButton)
				this->move(mapToParent(mEvent->pos() - m_mousePos));
		}
			break;
		default:
			break;
		}
	}

	return QWidget::eventFilter(obj, event);
}

void FloatingWindow::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Escape)
		hide();
}

void FloatingWindow::hideEvent(QHideEvent *event)
{
	emit closed();

	QWidget::hideEvent(event);
}

QSize FloatingWindow::screenSize() const
{
	QList<QScreen*> screens = QGuiApplication::screens();
	QRect bounds;

	for (auto* screen : screens)
		bounds = bounds.united(screen->geometry());

	return bounds.size();
}

void FloatingWindow::calculatePosition()
{
	updateSize();

	if (m_reference == nullptr)
		return;

	QPoint parentGlobalPos = m_reference->mapToGlobal(QPoint());
	QPoint position = parentGlobalPos;

	QSize screen = screenSize();

	// Check if widget can be displayed on a side of reference widget
	bool canBeLeft = (parentGlobalPos.x() >= this->width()) ? true : false;
	bool canBeTop = (parentGlobalPos.y() >= this->height()) ? true : false;
	bool canBeRight = (parentGlobalPos.x() + m_reference->width() + this->width() <= screen.width()) ? true : false;
	bool canBeBottom = (parentGlobalPos.y() + m_reference->height() + this->height() <= screen.height()) ? true : false;

	// TOP
	// Show at top side of reference
	if (canBeTop == true && canBeBottom == false)
		position.setY( parentGlobalPos.y() - this->height());

	// Show at left side of reference
	if (canBeTop == true && canBeLeft == true && canBeRight == false)
		position.setX( parentGlobalPos.x() - this->width());

	// Show at right side of reference
	if (canBeTop == true && canBeRight == true)
		position.setX( parentGlobalPos.x() + m_reference->width() );


	// BOTTOM
	// Show at bottom side of reference
	if (canBeBottom == true)
		position.setY( parentGlobalPos.y() + m_reference->height());

	// Offset from screen right
	if (canBeBottom == true && canBeLeft == true && canBeRight == false)
		position.setX( screen.width() - this->width());

	// Offset from screen left
	if (canBeBottom == true && canBeLeft == false && canBeRight == true)
		position.setX( 0 );

	// Align with left side of reference
	if (canBeBottom == true && canBeLeft == true && canBeRight == true)
		position.setX( parentGlobalPos.x() );

/*
 * This is just an alternative behaviour
 *
	// Show left
	if (canBeLeft == true && canBeRight == false)
		position.setX( parentGlobalPos.x() - this->width());

	// Show Right
	if (canBeRight == true)
		position.setX( parentGlobalPos.x() + m_reference->width());

	// Show Top
	if (canBeTop == true && canBeBottom == false && canBeLeft == true && canBeRight == true)
		position.setY( parentGlobalPos.y() - this->height());

	// Show Bottom
	if (canBeBottom == true && canBeLeft == true && canBeRight == true)
		position.setY( parentGlobalPos.y() + m_reference->height());
*/
	setGeometry(position.x(),position.y(), this->width(), this->height() );
}

void FloatingWindow::show(QWidget *reference)
{
	m_reference = reference;
	calculatePosition();	
	QWidget::show();
	QWidget::activateWindow();
}

void FloatingWindow::updateSize()
{
	adjustSize();
}
