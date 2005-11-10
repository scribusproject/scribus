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
#include <Q3Frame>
#include <QResizeEvent>
#include <QBoxLayout>
#include <prefscontext.h>
#include "tffilter.h"

class tfDia : public QDialog
{
	Q_OBJECT
private:
	PrefsContext* prefs;
	QBoxLayout* layout;
	QBoxLayout* alayout;
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
