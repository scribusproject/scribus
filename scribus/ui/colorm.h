/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef FARBMANAGER_H
#define FARBMANAGER_H

#include <QDialog>
class QHBoxLayout;
class QVBoxLayout;
class QLabel;
class QPushButton;
class QGroupBox;
class QToolButton;
class QColor;
class QComboBox;
class QListWidgetItem;

#include "colorsetmanager.h"
#include "scribusapi.h"
#include "query.h"
#include "sccolor.h" 

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

	/*! \brief Returns currently selected ScColor.
	It's used e.g. in BarcodeGenerator plugin.
	*/
	ScColor selectedColor();
	/*! \brief Returns name of the currently selected color.
	It's used e.g. in BarcodeGenerator plugin.
	*/
	QString selectedColorName();

private:
	void updateButtons();
	ColorListBox* colorListBox;
	QGroupBox* ColorsGroup;
	QGroupBox* ColsSetGroup;
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
	QComboBox* LoadColSet;
	QString sColor;
	QColor tmpColor;
	QStringList DontChange;
	ScribusDoc* m_Doc;

private slots:
	void saveDefaults();
	void loadDefaults(const QString &txt);
	//! \brief Just note: Farbe is German word for Color...
	void importColors();
	void deleteColor();
	void deleteUnusedColors();
	void duplicateColor();
	void newColor();
	void editColor();
	void selColor(QListWidgetItem*);
	void selEditColor(QListWidgetItem*);
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
