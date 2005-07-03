/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef QUERY_H
#define QUERY_H

#include <qdialog.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>

class Query : public QDialog
{
    Q_OBJECT

public:
    Query( QWidget* parent=0, const char* name=0, bool modal = FALSE, WFlags fl=0, QString text=0, 
			QString titel=0 );
    ~Query() {};

    QPushButton* PushButton1;
    QPushButton* PushButton2;
    QLineEdit* Answer;
    QLabel* Frage;

public slots:
	void Leave();

protected:
    QVBoxLayout* QueryLayout;
    QHBoxLayout* Layout2;
    QHBoxLayout* Layout1;
};

#endif // QUERY_H
