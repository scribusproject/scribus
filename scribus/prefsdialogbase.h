/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PREFSDIALOGBASE_H
#define PREFSDIALOGBASE_H

#include <qvariant.h>
#include <qdialog.h>
#include <qiconview.h>
#include <qmap.h>
#include <qwidgetstack.h>
#include <qwidget.h>

#include "scribusapi.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QPushButton;
class QLabel;

class SCRIBUS_API PrefsDialogBase : public QDialog
{
	Q_OBJECT

public:
	PrefsDialogBase( QWidget* parent = 0 );
	~PrefsDialogBase() {};
	int addItem(QString name, QPixmap icon, QWidget *tab);
	void arrangeIcons();
	QWidgetStack* prefsWidgets;
	QPushButton* backToDefaults;
	QPushButton* buttonOk;
	QPushButton* buttonCancel;
	QPushButton* saveButton;
	QIconView* prefsSelection;
	QMap<QIconViewItem*, int> itemMap;
	int counter;

public slots:
	void itemSelected(QIconViewItem* ic);

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
