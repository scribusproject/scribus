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

#include "scribusapi.h"
#include "scribusstructs.h"
#include "query.h"

class DynamicTip;


/*! \brief Very nice list box with color names and samples.
It's inherited from QListBox with all its methods and properties.
I create it as separate class because it's used now in ColorManager
and ColorWheel too. You can see it in Extras/Color Wheel or in
Edit/Colors dialogs in action.
\author Petr Vanek <petr@yarpen.cz>
*/
class SCRIBUS_API ColorListBox : public QListBox
{
	Q_OBJECT

	public:
		/*! \brief Standard QListBox like constructor.
		Just there are initialized pixmaps for icon drawing. */
		ColorListBox(QWidget * parent = 0, const char * name = 0, WFlags f = 0);

		/*! \brief Fill the list box with values taken from list.
		The list is cleared itself. Then is rendered an icon with
		color attributes (RGB/CMYK/Spot etc.).
		\param list a ColorList to present. */
		void updateBox(ColorList list);
	
	private:
		//! \brief Icons for color characteristics to render.
		QPixmap alertIcon;
		QPixmap cmykIcon;
		QPixmap rgbIcon;
		QPixmap spotIcon;
		QPixmap regIcon;
};


/*! \brief Dialog to manage colors and colorsets.
It provides all operations - color creating, deleting, importing etc.
There are some predefined color sets for user and it is managed here too.
*/
class SCRIBUS_API ColorManager : public QDialog
{
	Q_OBJECT

public:
	ColorManager( QWidget* parent, ColorList doco, bool haveDoc, QString docColSet, QStringList custColSet);
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
	bool HaveDoc;
	DynamicTip* dynTip;

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
};

#endif // FARBMANAGER_H
