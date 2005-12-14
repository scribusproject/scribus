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

class SCRIBUS_API Farbmanager : public QDialog
{ 
	Q_OBJECT

public:
	Farbmanager( QWidget* parent, ColorList doco, bool HDoc, QString DcolSet, QStringList Cust );
	~Farbmanager() {};
	ColorList EditColors;
	ColorList UsedC;
	QMap<QString,QString> Ersatzliste;
	QToolButton* LoadColSet;
	QStringList CColSet;

private:
	QListBox* ListBox1;
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
	QPixmap alertIcon;
	QPixmap cmykIcon;
	QPixmap rgbIcon;
	QPixmap spotIcon;
	QPixmap regIcon;
	DynamicTip* dynTip;

private slots:
	void saveDefaults();
	void loadDefaults(int id);
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
