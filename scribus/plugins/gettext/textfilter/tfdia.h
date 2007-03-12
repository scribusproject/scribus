/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef TFDIA_H
#define TFDIA_H

#include <vector>
#include <qdialog.h>
#include <qcombobox.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <q3scrollview.h>
#include <q3vbox.h>
#include <qevent.h>
//Added by qt3to4:
#include <QResizeEvent>
#include <Q3Frame>
#include <prefscontext.h>
#include "tffilter.h"

class tfDia : public QDialog
{
	Q_OBJECT
private:
	PrefsContext* prefs;
	Q3BoxLayout* layout;
	Q3BoxLayout* alayout;
	QComboBox* filtersCombo;
	QLineEdit* saveEdit;
	QPushButton* clearButton;
	QPushButton* deleteButton;
	QPushButton* okButton;
	QPushButton* cancelButton;
	QString currentFilter;
	Q3ScrollView* qsv;
	Q3Frame* vbox;
	int currentIndex;
	void clear();
	void createLayout();
	void createFilter(PrefsTable* table);
	void writeFilterRow(PrefsTable* table, int row, tfFilter* filter);
	void storeLastFilter();
private slots:
	void saveTextChanged(const QString& text);
	void createFilterRow(tfFilter* after);
	void removeRow(tfFilter* tff);
	void clearClicked();
	void okClicked();
	void cancelClicked();
	void deleteClicked();
	void loadFilter(const QString& name);
	void resizeEvent(QResizeEvent* e);
public:
	std::vector<tfFilter*> filters;
	tfDia();
	~tfDia();
};

#endif // TFDIA_H
