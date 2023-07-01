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

public:
	tfDia();
	~tfDia();

	std::vector<tfFilter*> filters;

protected:
	void resizeEvent(QResizeEvent* e) override;

private:
	PrefsContext* prefs { nullptr };
	QBoxLayout* layout { nullptr };
	QBoxLayout* alayout { nullptr };
	QComboBox* filtersCombo { nullptr };
	QLineEdit* saveEdit { nullptr };
	QPushButton* clearButton { nullptr };
	QPushButton* deleteButton { nullptr };
	QPushButton* okButton { nullptr };
	QPushButton* cancelButton { nullptr };
	QScrollArea* qsv { nullptr };
	QFrame* vbox { nullptr };
	
	QString currentFilter { "tf_lastUsed" };
	int currentIndex { 0 };

	void clear();
	void createLayout();
	void createFilter(PrefsTable* table);
	void writeFilterRow(PrefsTable* table, int row, tfFilter* filter);
	void storeLastFilter();

private slots:
	void adjustVBoxSize();
	void saveTextChanged(const QString& text);
	void createFilterRow(tfFilter* after);
	void removeRow(tfFilter* tff);
	void clearClicked();
	void okClicked();
	void cancelClicked();
	void deleteClicked();
	void loadFilter(const QString& name);
};

#endif // TFDIA_H
