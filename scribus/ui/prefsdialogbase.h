/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PREFSDIALOGBASE_H
#define PREFSDIALOGBASE_H

#include <QDialog>
#include <QListWidget>
#include <QMap>

class QEvent;
class QHBoxLayout;
class QLabel;
class QListWidgetItem;
class QPushButton;
class QStackedWidget;
class QVBoxLayout;

#include "scribusapi.h"


class SCRIBUS_API OptionListWidget : public QListWidget
{
	Q_OBJECT

public:
	OptionListWidget(QWidget* parent);
	~OptionListWidget() {};
	void arrangeIcons();
};

class SCRIBUS_API PrefsDialogBase : public QDialog
{
	Q_OBJECT

public:
	PrefsDialogBase( QWidget* parent = 0 );
	~PrefsDialogBase() {};
	
	virtual void changeEvent(QEvent *e);
	
	int addItem(QString name, QPixmap icon, QWidget *tab);
//	void arrangeIcons();
	QStackedWidget* prefsWidgets;
	QPushButton* backToDefaults;
	//! \brief Apply changes but don't close the dialog.
	QPushButton* applyChangesButton;
	QPushButton* buttonOk;
	QPushButton* buttonCancel;
	QPushButton* saveButton;
	OptionListWidget* prefsSelection;
	QMap<QListWidgetItem*, int> itemMap;
	int counter;

public slots:
	void itemSelected(QListWidgetItem* ic);

signals:
	void aboutToShow(QWidget *);

protected:
	QVBoxLayout* prefsLayout;
	QHBoxLayout* layout3;
	QHBoxLayout* layout4;
	QVBoxLayout* layout5;
	QLabel* tabNameLabel;

protected slots:
	virtual void languageChange();
	virtual void saveButton_clicked();
};

#endif
