/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SMREPLACEDIA_H
#define SMREPLACEDIA_H

#include "smreplacediabase.h"
#include "styleitem.h"

class QString;
class QStringList;
class QLabel;
class QComboBox;
class QHBoxLayout;

class SMRowWidget : public QWidget
{
	Q_OBJECT
public:
	SMRowWidget(const QString &toBeDeleted, QStringList replaceOptions, QWidget *parent);
	~SMRowWidget();

	QString toBeDeleted();
	QString replaceWith();

private:
	QHBoxLayout *layout;
	QLabel      *deleteLabel;
	QComboBox   *optionsCombo;
};

class SMReplaceDia : public SMReplaceDiaBase {
	Q_OBJECT
public:
	SMReplaceDia(const QStringList &toBeDeleted, const QStringList &replaceOptions);
	~SMReplaceDia();

	QValueList<RemoveItem> items();

private:
	QVBoxLayout *layout;
	QHBoxLayout *headerLayout;
	QLabel      *deleteHeader;
	QLabel      *optionsHeader;
	QPtrList<SMRowWidget>  rowWidgets;
};

#endif
