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
#include <qcolor.h>
#include "scribusdoc.h"
#include "query.h"

class Farbmanager : public QDialog
{ 
    Q_OBJECT

public:
    Farbmanager( QWidget* parent, CListe doco );
    ~Farbmanager() {};
  	CListe EditColors;
  	QMap<QString,QString> Ersatzliste;
private:
    QListBox* ListBox1;
    QPushButton* LoadF;
    QPushButton* NewF;
    QPushButton* EditF;
    QPushButton* DupF;
    QPushButton* DelF;
    QPushButton* SaveF;
    QPushButton* CancF;
    QString sFarbe;
    QColor tmpFarbe;
		QStringList DontChange;

private slots:
		void loadFarben();
    void delFarbe();
    void duplFarbe();
    void neueFarbe();
    void editFarbe();
    void selFarbe(QListBoxItem*);
    void updateCList();

protected:
    QHBoxLayout* Layout2;
    QVBoxLayout* Layout1;
};

#endif // FARBMANAGER_H
