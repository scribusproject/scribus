/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef TFFILTER_H
#define TFFILTER_H

#include <qwidget.h>
#include <qcombobox.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qframe.h>
#include <qlayout.h>
#include <qlabel.h>
#include <prefstable.h>
#include <prefscontext.h>

enum Action {
	REMOVE,
	REPLACE,
	APPLY
};

enum Style {
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
