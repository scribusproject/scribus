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
#include <q3iconview.h>
#include <qmap.h>
#include <q3widgetstack.h>
#include <qwidget.h>
//Added by qt3to4:
#include <Q3VBoxLayout>
#include <Q3GridLayout>
#include <QPixmap>
#include <Q3HBoxLayout>
#include <QLabel>

#include "scribusapi.h"

class Q3VBoxLayout;
class Q3HBoxLayout;
class Q3GridLayout;
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
	Q3WidgetStack* prefsWidgets;
	QPushButton* backToDefaults;
	//! \brief Apply changes but don't close the dialog.
	QPushButton* applyChangesButton;
	QPushButton* buttonOk;
	QPushButton* buttonCancel;
	QPushButton* saveButton;
	Q3IconView* prefsSelection;
	QMap<Q3IconViewItem*, int> itemMap;
	int counter;

public slots:
	void itemSelected(Q3IconViewItem* ic);

protected:
	Q3VBoxLayout* prefsLayout;
	Q3HBoxLayout* layout3;
	Q3HBoxLayout* layout4;
	Q3VBoxLayout* layout5;
	QLabel* tabNameLabel;

protected slots:
	virtual void languageChange();
	virtual void saveButton_clicked();
};

#endif
