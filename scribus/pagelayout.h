/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PAGELAYOUT_H
#define PAGELAYOUT_H

#include <qvariant.h>
#include <q3groupbox.h>
//Added by qt3to4:
#include <Q3VBoxLayout>
#include <Q3HBoxLayout>
#include <QLabel>
#include <QList>
#include "scribusapi.h"
#include "scribusstructs.h"

class Q3VBoxLayout;
class Q3HBoxLayout;
class Q3IconView;
class Q3IconViewItem;
class QLabel;
class ScComboBox;

class SCRIBUS_API PageLayouts : public Q3GroupBox
{
	Q_OBJECT

public:
	PageLayouts( QWidget* parent, QList<PageSet> pSets, bool mode = true );
	~PageLayouts() {};
	void updateLayoutSelector(QList<PageSet> pSets);
	void selectFirstP(int nr);
	void selectItem(uint nr);
	Q3IconView* layoutsView;
	ScComboBox* layoutsCombo;
	QLabel* layoutLabel1;
	ScComboBox* firstPage;
	QList<PageSet> pageSets;

public slots:
	void itemSelected(Q3IconViewItem* ic);
	void itemSelected(int ic);

signals:
	void selectedLayout(int);
	void selectedFirstPage(int);

private:
	void itemSelectedPost(int choosen);
	bool modus;

protected:
	Q3VBoxLayout* layoutGroupLayout;

protected slots:
	virtual void languageChange();

};

#endif // PAGELAYOUT_H
