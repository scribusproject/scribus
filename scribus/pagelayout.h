#ifndef PAGELAYOUT_H
#define PAGELAYOUT_H

#include <qvariant.h>
#include <qgroupbox.h>

class QVBoxLayout;
class QHBoxLayout;
class QIconView;
class QIconViewItem;
class QLabel;
class QSpinBox;

class PageLayouts : public QGroupBox
{
	Q_OBJECT

public:
	PageLayouts( QWidget* parent );
	~PageLayouts() {};
	void selectItem(uint nr);
	QIconView* layoutsView;
	QLabel* layoutLabel1;
	QSpinBox* firstPage;
	QLabel* layoutLabel2;

public slots:
	void itemSelected(QIconViewItem* ic);

signals:
	void selectedLayout(int);

protected:
	QVBoxLayout* layoutGroupLayout;
	QHBoxLayout* layout1;

protected slots:
	virtual void languageChange();

};

#endif // PAGELAYOUT_H
