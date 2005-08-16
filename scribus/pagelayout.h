#ifndef PAGELAYOUT_H
#define PAGELAYOUT_H

#include <qvariant.h>
#include <qgroupbox.h>
#include "scribusapi.h"
#include "scribusstructs.h"

class QVBoxLayout;
class QHBoxLayout;
class QIconView;
class QIconViewItem;
class QLabel;
class QComboBox;

class SCRIBUS_API PageLayouts : public QGroupBox
{
	Q_OBJECT

public:
	PageLayouts( QWidget* parent, QValueList<PageSet> pSets );
	~PageLayouts() {};
	void selectItem(uint nr);
	QIconView* layoutsView;
	QLabel* layoutLabel1;
	QComboBox* firstPage;
	QValueList<PageSet> pageSets;

public slots:
	void itemSelected(QIconViewItem* ic);

signals:
	void selectedLayout(int);

protected:
	QVBoxLayout* layoutGroupLayout;

protected slots:
	virtual void languageChange();

};

#endif // PAGELAYOUT_H
