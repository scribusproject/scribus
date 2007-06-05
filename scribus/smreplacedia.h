/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SMREPLACEDIA_H
#define SMREPLACEDIA_H

#include "ui_smreplacedia.h"
#include "styleitem.h"
//Added by qt3to4:
#include <Q3PtrList>
#include <QLabel>
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>

class QString;
class QStringList;
class QLabel;
class QComboBox;
class Q3HBoxLayout;

class SMRowWidget : public QWidget
{
	Q_OBJECT
public:
	SMRowWidget(const QString &toBeDeleted, QStringList replaceOptions, QWidget *parent);
	~SMRowWidget();

	QString toBeDeleted();
	QString replaceWith();

private:
	Q3HBoxLayout *layout;
	QLabel      *deleteLabel;
	QComboBox   *optionsCombo;
};

class SMReplaceDia : public QDialog, Ui::SMReplaceDia {
	Q_OBJECT
public:
	SMReplaceDia(const QStringList &toBeDeleted, const QStringList &replaceOptions, QWidget *parent);
	~SMReplaceDia();

	QList<RemoveItem> items();

private:
	Q3VBoxLayout *layout;
	Q3HBoxLayout *headerLayout;
	QLabel      *deleteHeader;
	QLabel      *optionsHeader;
	Q3PtrList<SMRowWidget>  rowWidgets;
};

#endif
