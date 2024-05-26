#include "stacked_container.h"
#include <QStackedLayout>


StackedContainer::StackedContainer(QWidget *parent)
	: QStackedWidget( parent)
{
	connect(this, &QStackedWidget::currentChanged, this, &StackedContainer::updateSize);
}

QSize StackedContainer::minimumSizeHint() const
{
	QSize s(widget(currentIndex())->sizeHint());
	s.setWidth(this->width());
	return s;
}

QSize StackedContainer::sizeHint() const
{
	return minimumSizeHint();
}

void StackedContainer::updateSize()
{
	//resize(this->width(), sizeHint().height());
	adjustSize();
}

