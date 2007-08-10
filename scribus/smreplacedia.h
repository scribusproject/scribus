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

class QLabel;
class QComboBox;


/*! \brief Style Manager: replace one style with another one.
A "current--new row" widget.
*/
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


/*! \brief Style Manager replace dialog
*/
class SMReplaceDia : public QDialog, Ui::SMReplaceDia
{
	Q_OBJECT
public:
	SMReplaceDia(const QStringList &toBeDeleted, const QStringList &replaceOptions, QWidget *parent);
	~SMReplaceDia();

	QList<RemoveItem> items();

private:
	QVBoxLayout *layout;
	QHBoxLayout *headerLayout;
	QLabel      *deleteHeader;
	QLabel      *optionsHeader;
	QList<SMRowWidget*>  rowWidgets;
};

#endif
