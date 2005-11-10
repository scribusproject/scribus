/****************************************************************************
** Form interface generated from reading ui file 'Color.ui'
**
** Created: Mon Apr 23 19:09:02 2001
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
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
#include <QPixmap>
#include <QHBoxLayout>
#include <QVBoxLayout>

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
	Q3ListBox* ListBox1;
	Q3GroupBox* ColorsGroup;
	Q3GroupBox* ColsSetGroup;
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
	Q3PopupMenu* CSets;
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
	void selFarbe(Q3ListBoxItem*);
	void selEditFarbe(Q3ListBoxItem*);
	void updateCList();

protected:
	QVBoxLayout* Layout2;
	QHBoxLayout* layout5;
	QVBoxLayout* layout3;
	QVBoxLayout* ColsSetGroupLayout;
	QVBoxLayout* Layout1;
};

#endif // FARBMANAGER_H
