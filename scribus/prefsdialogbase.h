#ifndef PREFSDIALOGBASE_H
#define PREFSDIALOGBASE_H

#include <qvariant.h>
#include <qdialog.h>
#include <q3iconview.h>
#include <qmap.h>
#include <q3widgetstack.h>
#include <qwidget.h>
//Added by qt3to4:
#include <QPixmap>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

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
	Q3WidgetStack* prefsWidgets;
	QPushButton* backToDefaults;
	QPushButton* buttonOk;
	QPushButton* buttonCancel;
	Q3IconView* prefsSelection;
	QMap<Q3IconViewItem*, int> itemMap;
	int counter;

public slots:
	void itemSelected(Q3IconViewItem* ic);

protected:
	QVBoxLayout* prefsLayout;
	QHBoxLayout* layout3;
	QHBoxLayout* layout4;
	QVBoxLayout* layout5;
	QLabel* tabNameLabel;

protected slots:
	virtual void languageChange();

};

#endif
