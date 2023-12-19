/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PAGELAYOUT_H
#define PAGELAYOUT_H

#include <QGroupBox>
#include <QListWidget>
#include <QList>

class QComboBox;
class QLabel;
class QListWidgetItem;
class QVBoxLayout;

#include "scribusapi.h"
#include "scribusstructs.h"
#include "scribusdoc.h"

/*! \brief A widget containing pages layout schema */
class SCRIBUS_API PageListWidget : public QListWidget
{
	Q_OBJECT

public:
	PageListWidget(QWidget* parent);
	~PageListWidget() {};
	void arrangeIcons();
};


/*! \brief A widget for changing pages layout.
User can change layouts on-the-fly changing widget's
comboboxes. */
class SCRIBUS_API PageLayouts : public QGroupBox
{
	Q_OBJECT

public:
	PageLayouts(QWidget* parent);
	PageLayouts(QWidget* parent, const QList<PageSet>& pSets, bool mode = true);
	~PageLayouts() {};

	void updateLayoutSelector(const ScribusDoc* doc);
	void selectFirstP(int nr);
	void selectItem(uint nr);
	PageListWidget* layoutsView;
	QComboBox* layoutsCombo;
	QLabel* layoutLabel1;
	QComboBox* firstPage;
	QList<PageSet> pageSets;
	int docPagePositioning {0};

public slots:
	void itemSelected(QListWidgetItem* ic);
	void itemSelected(int ic);

signals:
	void selectedLayout(int);
	void selectedFirstPage(int);

private:
	void itemSelectedPost(int chosen);
	bool modus;

protected:
	QVBoxLayout* layoutGroupLayout;

protected slots:
	void languageChange();

};

#endif // PAGELAYOUT_H
