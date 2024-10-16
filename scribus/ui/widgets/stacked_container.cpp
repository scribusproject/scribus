#include "stacked_container.h"
#include <QStackedLayout>


StackedContainer::StackedContainer(QWidget *parent)
	: QStackedWidget( parent)
{
	connect(this, &QStackedWidget::currentChanged, this, &StackedContainer::updateSize);
}

QSize StackedContainer::minimumSizeHint() const
{
	return widget(currentIndex())->minimumSizeHint();
}

QSize StackedContainer::sizeHint() const
{
	return widget(currentIndex())->sizeHint();
}

void StackedContainer::updateSize()
{
	adjustSize();
}

