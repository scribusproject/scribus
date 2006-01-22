/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
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
class ScComboBox;

class SCRIBUS_API PageLayouts : public QGroupBox
{
	Q_OBJECT

public:
	PageLayouts( QWidget* parent, QValueList<PageSet> pSets, bool mode = true );
	~PageLayouts() {};
	void updateLayoutSelector(QValueList<PageSet> pSets);
	void selectFirstP(int nr);
	void selectItem(uint nr);
	QIconView* layoutsView;
	ScComboBox* layoutsCombo;
	QLabel* layoutLabel1;
	ScComboBox* firstPage;
	QValueList<PageSet> pageSets;

public slots:
	void itemSelected(QIconViewItem* ic);
	void itemSelected(int ic);

signals:
	void selectedLayout(int);
	void selectedFirstPage(int);

private:
	void itemSelectedPost(int choosen);
	bool modus;

protected:
	QVBoxLayout* layoutGroupLayout;

protected slots:
	virtual void languageChange();

};

#endif // PAGELAYOUT_H
