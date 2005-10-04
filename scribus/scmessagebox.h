/***************************************************************************
    begin                : Oct 2005
    copyright            : (C) 2005 by Craig Bradney
    email                : cbradney@zip.com.au
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef SCMESSAGEBOX_H
#define SCMESSAGEBOX_H

#include <qmessagebox.h>
#include "scribusapi.h"

class SCRIBUS_API ScMessageBox : public QMessageBox
{
	Q_OBJECT
public:
	static int information ( QWidget * parent, const QString & caption, const QString & text, int button0, int button1 = 0, int button2 = 0 );
	static int information ( QWidget * parent, const QString & caption, const QString & text, const QString & button0Text = QString::null, const QString & button1Text = QString::null, const QString & button2Text = QString::null, int defaultButtonNumber = 0, int escapeButtonNumber = -1 );
	static int question ( QWidget * parent, const QString & caption, const QString & text, int button0, int button1 = 0, int button2 = 0 );
	static int question ( QWidget * parent, const QString & caption, const QString & text, const QString & button0Text = QString::null, const QString & button1Text = QString::null, const QString & button2Text = QString::null, int defaultButtonNumber = 0, int escapeButtonNumber = -1 );
	static int warning ( QWidget * parent, const QString & caption, const QString & text, int button0, int button1, int button2 = 0 );
	static int warning ( QWidget * parent, const QString & caption, const QString & text, const QString & button0Text = QString::null, const QString & button1Text = QString::null, const QString & button2Text = QString::null, int defaultButtonNumber = 0, int escapeButtonNumber = -1 );
	static int critical ( QWidget * parent, const QString & caption, const QString & text, int button0, int button1, int button2 = 0 );
	static int critical ( QWidget * parent, const QString & caption, const QString & text, const QString & button0Text = QString::null, const QString & button1Text = QString::null, const QString & button2Text = QString::null, int defaultButtonNumber = 0, int escapeButtonNumber = -1 );
	
	static void swapButtonValues(int &b0, int &b1, int &b2);
	static void swapButtonValues(QString &b0Text, QString &b1Text, QString &b2Text, int &defaultButton, int &escapeButton);
};

#endif
