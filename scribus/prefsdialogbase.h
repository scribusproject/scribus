#ifndef PREFSDIALOGBASE_H
#define PREFSDIALOGBASE_H

#include <qvariant.h>
#include <qdialog.h>
#include <qlistbox.h>
#include <qwidgetstack.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QPushButton;

class PrefsDialogBase : public QDialog
{
	Q_OBJECT

public:
	PrefsDialogBase( QWidget* parent = 0 );
	~PrefsDialogBase() {};

	QListBox* prefsSelection;
	QWidgetStack* prefsWidgets;
	QPushButton* buttonOk;
	QPushButton* buttonCancel;

protected:
	QVBoxLayout* prefsLayout;
	QHBoxLayout* layout3;
	QHBoxLayout* layout4;

protected slots:
	virtual void languageChange();

};

#endif
