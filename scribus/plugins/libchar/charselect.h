#ifndef QUERY_H
#define QUERY_H

#include <qdialog.h>

class QLabel;
class QPushbutton;
class QTable;
class QLayout;
class QToolTip;
class QStringList;
class QComboBox;
class QFont;
class ScribusApp;

/** Calls the Plugin with the main Application window as parent
  * and the main Application Class as parameter */
extern "C" void Run(QWidget *d, ScribusApp *plug);
/** Returns the Name of the Plugin.
  * This name appears in the relevant Menue-Entrys */
extern "C" QString Name();
/** Returns the Type of the Plugin.
  * 1 = the Plugin is a normal Plugin, which appears in the Extras Menue
  * 2 = the Plugins is a import Plugin, which appears in the Import Menue
  * 3 = the Plugins is a export Plugin, which appears in the Export Menue */
extern "C" int Type();
extern "C" int ID();

class Zoom : public QDialog
{
Q_OBJECT

public:
	Zoom( QWidget* parent, QPixmap pix, uint val);
	~Zoom() {};
	void paintEvent(QPaintEvent *);
	QPixmap pixm;
	QString valu;
};

class ChTable;

class CharSelect : public QDialog
{
	Q_OBJECT

public:
	CharSelect( QWidget* parent, PageItem *item, ScribusApp *plug );
	~CharSelect() {};
	ChTable* zTabelle;
	QLabel* sample;
	QPushButton* insertButton;
	QPushButton* deleteButton;
	QPushButton* closeButton;
	PageItem *ite;
	ScribusApp *ap;
	QValueList<uint> characters;
	QString chToIns;
	int maxCount;

public slots:
	void newChar(int r, int c);
	void delEdit();
	void insChar();

protected:
	QVBoxLayout* zAuswahlLayout;
	QHBoxLayout* layout1;
};

class ChTable : public QTable
{
    Q_OBJECT

public:
	ChTable(CharSelect* parent, ScribusApp *pl);
	~ChTable() {};
	bool mPressed;
	Zoom* dia;
	ScribusApp *ap;
	CharSelect* par;
	int maxCount;

signals:
	void selectChar(int, int);

protected:
	virtual void contentsMouseReleaseEvent(QMouseEvent *m);
	virtual void contentsMousePressEvent(QMouseEvent* e);
};
#endif // QUERY_H
