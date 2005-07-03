/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef MISSINGFONT_H
#define MISSINGFONT_H

#include <qdialog.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include "scribusdoc.h"
#include "fontcombo.h"

class DmF : public QDialog
{
    Q_OBJECT

public:
    DmF( QWidget* parent, QString fon, preV *Prefs );
    ~DmF() {};

    QPushButton* PushButton1;
    QLabel* TextLabel6;
    QLabel* PixmapLabel1;
    FontCombo* Replace;
    QLabel* TextLabel1;
    QLabel* TextLabel4;
    QString Ersatz;

public slots:
    virtual void NeuerFont(const QString& e);

protected:
    QHBoxLayout* MissingFontLayout;
    QGridLayout* Layout1;

};

#endif // MISSINGFONT_H
