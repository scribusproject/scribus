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

#include "scmessagebox.h"
#include "scmessagebox.moc"
#include "scribusapp.h"

extern ScribusQApp* ScQApp;


void ScMessageBox::swapButtonValues(int &b0, int &b1, int &b2)
{
	if (ScQApp->reverseDialogButtons())
	{
		//Last button not used, and 2nd one used, swap 1st and 2nd.
		if (b2==0 && b1!=0)
		{
			int t=b1;
			b1=b0;
			b0=t;
		}
		//All three buttons used, swap the outside ones only
		if (b2!=0 && b1!=0 && b0!=0)
		{
			int t=b2;
			b2=b0;
			b0=t;
		}
	}
}

void ScMessageBox::swapButtonValues(QString &b0Text, QString &b1Text, QString &b2Text, int &defaultButton, int &escapeButton)
{
	if (ScQApp->reverseDialogButtons())
	{
		//Last button not used, and 2nd one used, swap 1st and 2nd.
		if (b2Text.isEmpty() && !b1Text.isEmpty())
		{
			QString tText=b1Text;
			b1Text=b0Text;
			b0Text=tText;
			if (defaultButton==0)
				defaultButton=1;
			else
			if (defaultButton==1)
				defaultButton=0;
			if (escapeButton==0)
				escapeButton=1;
			else
			if (escapeButton==1)
				escapeButton=0;
		}
		//All three buttons used, swap the outside ones only
		if (!b2Text.isEmpty() && !b1Text.isEmpty() && !b0Text.isEmpty())
		{
			QString tText=b2Text;
			b2Text=b0Text;
			b0Text=tText;
			if (defaultButton==0)
				defaultButton=2;
			else
			if (defaultButton==2)
				defaultButton=0;
			if (escapeButton==0)
				escapeButton=2;
			else
			if (escapeButton==2)
				escapeButton=0;
		}
	}
}


int ScMessageBox::information ( QWidget * parent, const QString & caption, const QString & text, int button0, int button1, int button2)
{
	int b0=button0, b1=button1, b2=button2;
	swapButtonValues(b0, b1, b2);
	return QMessageBox::information(parent, caption, text, b0, b1, b2);
}

int ScMessageBox::information ( QWidget * parent, const QString & caption, const QString & text, const QString & button0Text, const QString & button1Text, const QString & button2Text, int defaultButtonNumber, int escapeButtonNumber)
{
	QString b0Text=button0Text, b1Text=button1Text, b2Text=button2Text;
	int defaultButton=defaultButtonNumber, escapeButton=escapeButtonNumber;
	swapButtonValues(b0Text, b1Text, b2Text, defaultButton, escapeButton);
	return QMessageBox::information(parent, caption, text, b0Text, b1Text, b2Text, defaultButton, escapeButton);
}

int ScMessageBox::question ( QWidget * parent, const QString & caption, const QString & text, int button0, int button1, int button2)
{
	int b0=button0, b1=button1, b2=button2;
	swapButtonValues(b0, b1, b2);
	return QMessageBox::question(parent, caption, text, b0, b1, b2);
}

int ScMessageBox::question ( QWidget * parent, const QString & caption, const QString & text, const QString & button0Text, const QString & button1Text, const QString & button2Text, int defaultButtonNumber, int escapeButtonNumber)
{
	QString b0Text=button0Text, b1Text=button1Text, b2Text=button2Text;
	int defaultButton=defaultButtonNumber, escapeButton=escapeButtonNumber;
	swapButtonValues(b0Text, b1Text, b2Text, defaultButton, escapeButton);
	return QMessageBox::question(parent, caption, text, b0Text, b1Text, b2Text, defaultButton, escapeButton);
}

int ScMessageBox::warning ( QWidget * parent, const QString & caption, const QString & text, int button0, int button1, int button2)
{
	int b0=button0, b1=button1, b2=button2;
	swapButtonValues(b0, b1, b2);
	return QMessageBox::warning(parent, caption, text, b0, b1, b2);
}

int ScMessageBox::warning ( QWidget * parent, const QString & caption, const QString & text, const QString & button0Text, const QString & button1Text, const QString & button2Text, int defaultButtonNumber, int escapeButtonNumber)
{
	QString b0Text=button0Text, b1Text=button1Text, b2Text=button2Text;
	int defaultButton=defaultButtonNumber, escapeButton=escapeButtonNumber;
	swapButtonValues(b0Text, b1Text, b2Text, defaultButton, escapeButton);
	return QMessageBox::warning(parent, caption, text, b0Text, b1Text, b2Text, defaultButton, escapeButton);
}

int ScMessageBox::critical ( QWidget * parent, const QString & caption, const QString & text, int button0, int button1, int button2)
{
	int b0=button0, b1=button1, b2=button2;
	swapButtonValues(b0, b1, b2);
	return QMessageBox::critical(parent, caption, text, b0, b1, b2);
}

int ScMessageBox::critical ( QWidget * parent, const QString & caption, const QString & text, const QString & button0Text, const QString & button1Text, const QString & button2Text, int defaultButtonNumber, int escapeButtonNumber)
{
	QString b0Text=button0Text, b1Text=button1Text, b2Text=button2Text;
	int defaultButton=defaultButtonNumber, escapeButton=escapeButtonNumber;
	swapButtonValues(b0Text, b1Text, b2Text, defaultButton, escapeButton);
	return QMessageBox::critical(parent, caption, text, b0Text, b1Text, b2Text, defaultButton, escapeButton);
}


