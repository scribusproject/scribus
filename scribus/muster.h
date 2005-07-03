/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef MUSTERSEITEN_H
#define MUSTERSEITEN_H

#include <qdialog.h>
#include <qlistbox.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include "scribusdoc.h"
#include "scribusview.h"

class MusterSeiten : public QDialog
{ 
    Q_OBJECT

public:
    MusterSeiten( QWidget* parent, ScribusDoc *doc, ScribusView *view, QString temp);
    ~MusterSeiten() {};
    void closeEvent(QCloseEvent *ce);

private:
    void updateMList(QString nam);
    QListBox* ListBox1;
	QPushButton* LoadM;
    QPushButton* NewB;
    QPushButton* DuplicateB;
    QPushButton* DeleteB;
    QPushButton* ExitB;
    ScribusDoc *Doc;
    ScribusView *View;
    QString sMuster;

private slots:
	void ExitEditor();
	void DuplTemp();
	void DelTemp();
	void NewTemp();
	void loadMpage();
	void selTemplate(QListBoxItem *c);
	
signals:
	void CreateNew(int);
	void LoadPage(QString, int, bool);
	void Fertig();
		
protected:
    QHBoxLayout* MusterSeitenLayout;
    QVBoxLayout* Layout2;
};

#endif // MUSTERSEITEN_H
