/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef FARBMANAGER_H
#define FARBMANAGER_H

#include <qdialog.h>
#include <qlayout.h>
#include <q3listbox.h>
#include <qpushbutton.h>
#include <q3groupbox.h>
#include <qtoolbutton.h>
#include <qlabel.h>
#include <q3popupmenu.h>
#include <qcolor.h>
#include <qtooltip.h>
//Added by qt3to4:
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>

#include "colorsetmanager.h"
#include "scribusapi.h"
#include "scribusstructs.h"
#include "query.h"

class DynamicTip;
class ScribusDoc;
class ColorListBox;

/*! \brief Dialog to manage colors and colorsets.
It provides all operations - color creating, deleting, importing etc.
There are some predefined color sets for user and it is managed here too.
*/
class SCRIBUS_API ColorManager : public QDialog
{
	Q_OBJECT

public:
	ColorManager( QWidget* parent, ColorList doco, ScribusDoc* doc, QString docColSet, QStringList custColSet);
	~ColorManager() {};
	//! \brief A ColorList with all available colors after dialog closing.
	ColorList EditColors;
	//! \brief in doc used colors
	ColorList UsedC;
	//! \brief Color to color replacement mapping
	QMap<QString,QString> replaceMap;
	//! \brief Custom user's color set
	QStringList customColSet;

	/*! \brief Returns the name of the current/selected color set.
	\retval QString selected name. */
	QString getColorSetName();

private:
	QToolButton* LoadColSet;
	ColorListBox* colorListBox;
	Q3GroupBox* ColorsGroup;
	Q3GroupBox* ColsSetGroup;
	QPushButton* importColorsButton;
	QPushButton* newColorButton;
	QPushButton* editColorButton;
	QPushButton* duplicateColorButton;
	QPushButton* deleteColorButton;
	QPushButton* deleteUnusedButton;
	QPushButton* saveButton;
	QPushButton* cancelButton;
	QLabel* textLabel1;
	QPushButton* SaveColSet;
	Q3PopupMenu* CSets;
	QString sColor;
	QColor tmpColor;
	QStringList DontChange;
	DynamicTip* dynTip;
	ScribusDoc* m_Doc;

private slots:
	void saveDefaults();
	void loadDefaults(int id);
	//! \brief Just note: Farbe is German word for Color...
	void importColors();
	void deleteColor();
	void deleteUnusedColors();
	void duplicateColor();
	void newColor();
	void editColor();
	void selColor(Q3ListBoxItem*);
	void selEditColor(Q3ListBoxItem*);
	void updateCList();

protected:
	Q3VBoxLayout* Layout2;
	Q3HBoxLayout* layout5;
	Q3VBoxLayout* layout3;
	Q3VBoxLayout* ColsSetGroupLayout;
	Q3VBoxLayout* Layout1;
	
	ColorSetManager csm;	
	int customSetStartIndex;
};

#endif // FARBMANAGER_H
