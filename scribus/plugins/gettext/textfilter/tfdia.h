/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef TFDIA_H
#define TFDIA_H

#include <vector>
#include <QDialog>
#include <QResizeEvent>
class QFrame;
class QComboBox;
class QLineEdit;
class QPushButton;
class QScrollArea;
class QBoxLayout;
class QHBoxLayout;
class QVBoxLayout;
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
	QScrollArea* qsv;
	QFrame* vbox;
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
