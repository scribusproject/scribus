/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef PCONSOLE_H
#define PCONSOLE_H

// #include <qdialog.h>
#include <qvariant.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <conswin.h>

class PConsole : public QWidget
{ 
    Q_OBJECT

public:
    PConsole( QWidget* parent );
    ~PConsole() {};
    void closeEvent(QCloseEvent *ce);
    ConsWin* OutWin;

signals:
		void Schliessen();

protected:
    QVBoxLayout* PConsoleLayout;
};

#endif // PCONSOLE_H
