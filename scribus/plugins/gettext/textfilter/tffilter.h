/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef TFFILTER_H
#define TFFILTER_H

#include <QWidget>
class QCheckBox;
class QBoxLayout;
class QFrame;
class QLabel;
class QCheckBox;
class QPushButton;
class QComboBox;
#include <prefstable.h>
#include <prefscontext.h>

enum Action {
	REMOVE,
	REPLACE,
	APPLY
};

enum MatchStyle {
	ALL_PARAGRAPHS,
	STARTS_WITH,
	LESS_THAN,
	MORE_THAN
};

enum SecondaryAction {
	REMOVE_MATCH,
	DO_NOT_REMOVE_MATCH
};

class tfFilter : public QWidget
{
	Q_OBJECT
private:
	QCheckBox* enableCheck;
	QBoxLayout* alayout;
	QBoxLayout* blayout;
	QFrame* actionFrame;
	QComboBox* firstCombo;
	QLabel* firstLabel;
	QComboBox* secondCombo;
	QCheckBox* secondRegexpCheck;
	QLabel* secondLabel;
	QComboBox* thirdCombo;
// 	QCheckBox* thirdRegexpCheck;
	QLabel* thirdLabel;
	QComboBox* fourthCombo;
	QLabel* fourthLabel;
	QComboBox* fifthCombo;
	QCheckBox* fifthRegexpCheck;
	QLabel* fifthLabel;
	QComboBox* sixthCombo;
	QPushButton* removeButton;
	QPushButton* addButton;
	int currentAction;
	PrefsContext* prefs;
	PrefsTable* history;
	void createWidget();
	void getFirstCombo();
	void getSecondCombo();
	void getThirdCombo(int secondIndex);
	void getFourthCombo();
	void getFifthCombo();
	void getSixthCombo();
	void resetBRow();
	void getParagraphStyles();
public:
	tfFilter(QWidget *parent, const char *name,
             int action, QString regExp, QString replace, QString pstyleName,
             int less, int more, int style, bool match, bool enabled, bool regexp);
	tfFilter(QWidget *parent = 0, const char *name = 0);
	~tfFilter();
	void setRemovable(bool b);
	int getAction();
	QString regExp();
	QString replaceWith();
	QString getPStyleName();
	int getLessThan();
	int getMoreThan();
	int getStyle();
	bool removeMatch();
	bool isEnabled();
	bool isRegExp();
signals:
	void removeClicked(tfFilter* tff);
	void addClicked(tfFilter* tff);
private slots:
	void enableToggled(bool on);
	void addClick();
	void removeClick();
	void firstChanged(int index);
	void secondChanged(int index);
	void thirdChanged(int index);
	void fourthChanged(int index);
	void fifthChanged(int index);
	void sixthChanged(int index);
};

#endif // TFFILTER_H
