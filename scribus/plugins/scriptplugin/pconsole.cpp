/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "pconsole.h"
#include "pconsole.moc"
#include "qpixmap.h"
extern QPixmap loadIcon(QString nam);

PConsole::PConsole( QWidget* parent )
		: QWidget( parent, "PConsole", WType_TopLevel )
{
	resize( 431, 306 );
	setCaption( tr( "Script Console" ) );
	setIcon(loadIcon("AppIcon.png"));
	PConsoleLayout = new QVBoxLayout( this, 11, 6, "PConsoleLayout");
	OutWin = new ConsWin(this);
	OutWin->setMinimumSize( QSize( 300, 0 ) );
	PConsoleLayout->addWidget( OutWin );
	connect( OutWin, SIGNAL( closeFromKeyB() ), this, SLOT( close() ) );
}

void PConsole::closeEvent(QCloseEvent *ce)
{
	emit Schliessen();
	ce->accept();
}

