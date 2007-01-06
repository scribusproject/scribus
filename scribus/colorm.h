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
#include <qlistbox.h>
#include <qpushbutton.h>
#include <qgroupbox.h>
#include <qtoolbutton.h>
#include <qlabel.h>
#include <qpopupmenu.h>
#include <qcolor.h>
#include <qtooltip.h>

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
	QGroupBox* ColorsGroup;
	QGroupBox* ColsSetGroup;
	QPushButton* LoadF;
	QPushButton* NewF;
	QPushButton* EditF;
	QPushButton* DupF;
	QPushButton* DelF;
	QPushButton* DelU;
	QPushButton* SaveF;
	QPushButton* CancF;
	QLabel* textLabel1;
	QPushButton* SaveColSet;
	QPopupMenu* CSets;
	QString sFarbe;
	QColor tmpFarbe;
	QStringList DontChange;
	DynamicTip* dynTip;
	ScribusDoc* m_Doc;

private slots:
	void saveDefaults();
	void loadDefaults(int id);
	//! \brief Just note: Farbe is German word for Color...
	void loadFarben();
	void delFarbe();
	void delUnused();
	void duplFarbe();
	void neueFarbe();
	void editFarbe();
	void selFarbe(QListBoxItem*);
	void selEditFarbe(QListBoxItem*);
	void updateCList();

protected:
	QVBoxLayout* Layout2;
	QHBoxLayout* layout5;
	QVBoxLayout* layout3;
	QVBoxLayout* ColsSetGroupLayout;
	QVBoxLayout* Layout1;
	
	ColorSetManager csm;	
	int customSetStartIndex;
};

#endif // FARBMANAGER_H
