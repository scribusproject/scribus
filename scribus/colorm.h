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
#include <qlistbox.h>
#include <qpushbutton.h>
#include <qgroupbox.h>
#include <qtoolbutton.h>
#include <qlabel.h>
#include <qpopupmenu.h>
#include <qcolor.h>
#include "scribusdoc.h"
#include "query.h"

class Farbmanager : public QDialog
{ 
	Q_OBJECT

public:
	Farbmanager( QWidget* parent, CListe doco, bool HDoc, QString DcolSet, QStringList Cust );
	~Farbmanager() {};
	CListe EditColors;
	CListe UsedC;
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
	QHBoxLayout* layout4;
	QVBoxLayout* layout3;
	QVBoxLayout* layout7;
	QVBoxLayout* ColsSetGroupLayout;
	QVBoxLayout* Layout1;
};

#endif // FARBMANAGER_H
