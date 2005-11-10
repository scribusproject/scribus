#ifndef PAGELAYOUT_H
#define PAGELAYOUT_H

#include <qvariant.h>
#include <q3groupbox.h>
//Added by qt3to4:
#include <QLabel>
#include <QVBoxLayout>
#include <Q3ValueList>
#include <QHBoxLayout>
#include "scribusapi.h"
#include "scribusstructs.h"

class QVBoxLayout;
class QHBoxLayout;
class Q3IconView;
class Q3IconViewItem;
class QLabel;
class ScComboBox;

class SCRIBUS_API PageLayouts : public Q3GroupBox
{
	Q_OBJECT

public:
	PageLayouts( QWidget* parent, Q3ValueList<PageSet> pSets, bool mode = true );
	~PageLayouts() {};
	void updateLayoutSelector(Q3ValueList<PageSet> pSets);
	void selectFirstP(int nr);
	void selectItem(uint nr);
	Q3IconView* layoutsView;
	ScComboBox* layoutsCombo;
	QLabel* layoutLabel1;
	ScComboBox* firstPage;
	Q3ValueList<PageSet> pageSets;

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
	QVBoxLayout* layoutGroupLayout;

protected slots:
	virtual void languageChange();

};

#endif // PAGELAYOUT_H
