#ifndef STACKEDCONTAINER_H
#define STACKEDCONTAINER_H

#include <QStackedWidget>


class StackedContainer : public QStackedWidget
{	

public:
	explicit StackedContainer(QWidget *parent = nullptr);
	~StackedContainer() {};

	QSize minimumSizeHint() const override;
	QSize sizeHint() const override;

private slots:
	void updateSize();

};

#endif // STACKEDCONTAINER_H
