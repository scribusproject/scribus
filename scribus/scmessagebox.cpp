/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
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
#include "scribuscore.h"

extern ScribusQApp* ScQApp;


int ScMessageBox::swapButtonValues(int &b0, int &b1, int &b2)
{
	int buttonsUsed=1;
	if (ScCore->reverseDialogButtons())
	{
		//Last button not used, and 2nd one used, swap 1st and 2nd.
		if (b2==0 && b1!=0) //Could be: if ((b2==0 || b2==QMessageBox::NoButton) && b1!=0) but NoButton is 0x0000, for now.
		{
			int t=b1;
			b1=b0;
			b0=t;
			buttonsUsed=2;
		}
		//All three buttons used, swap the outside ones only
		if (b2!=0 && b1!=0 && b0!=0)
		{
			int t;
			if (ScCore->isMacGUI())
			{
				// Mac order is "No, Cancel, Yes"
				t = b2;
				b2 = b0;
				b0 = b1;
				b1 = t;
			}
			else 
			{
				// If Unix order is "Cancel, No, Yes"
				t = b2;
				b2=b0;
				b0=t;
			}
			buttonsUsed=3;
		}
	}
	return buttonsUsed;
}

int ScMessageBox::swapButtonValues(QString &b0Text, QString &b1Text, QString &b2Text, int &defaultButton, int &escapeButton)
{
	int buttonsUsed=1;
	if (ScCore->reverseDialogButtons())
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
			buttonsUsed=2;
		}
		//All three buttons used, swap the outside ones only
		if (!b2Text.isEmpty() && !b1Text.isEmpty() && !b0Text.isEmpty())
		{
			QString tText;
			if (ScCore->isMacGUI())
			{
				// Mac order is "No, Cancel, Yes"
				tText = b2Text;
				b2Text = b0Text;
				b0Text = b1Text;
				b1Text = tText;

				switch (defaultButton) {
				case 0:
					defaultButton=2;
					break;
				case 1:

					break;
				case 2:
					defaultButton=0;
				}

				switch (escapeButton) {
				case 0:
					escapeButton=2;
					break;
				case 1:
					escapeButton=0;
					break;
				case 2:
					escapeButton=1;
				}
			}
			else 
			{
				// Unix order is "Cancel, No, Yes"
				tText = b2Text;
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
			buttonsUsed=3;
		}
	}
	return buttonsUsed;
}

int ScMessageBox::swapReturnValues(const int b0Val, const int b1Val, const int b2Val, const int returnVal, const int buttonsUsed)
{
	int retVal=returnVal;
	if (ScCore->reverseDialogButtons())
	{
		//Last button not used, and 2nd one used, swap 1st and 2nd.
		if (buttonsUsed==2)
		{
			if (returnVal==b0Val)
				retVal=b1Val;
			else
			if (returnVal==b1Val)
				retVal=b0Val;
		}
		
		//All three buttons used, swap the outside ones only
		if (buttonsUsed==3)
		{
			if (ScCore->isMacGUI()) {
				if (returnVal == b0Val)
					retVal = b1Val;
				else if (returnVal == b1Val)
					retVal = b2Val;
				else if (returnVal == b2Val)
					retVal = b0Val;
			}
			else
			{
				if (returnVal==b0Val)
					retVal=b2Val;
				else
				if (returnVal==b2Val)
					retVal=b0Val;

			}
		}
	}
	return retVal;
}

int ScMessageBox::information ( QWidget * parent, const QString & caption, const QString & text, int button0, int button1, int button2)
{
	int b0=button0, b1=button1, b2=button2;
	int buttonsUsed=swapButtonValues(b0, b1, b2);
	int stdReturn=QMessageBox::information(parent, caption, text, b0, b1, b2);
	return swapReturnValues(b0, b1, b2, stdReturn, buttonsUsed);
}

int ScMessageBox::information ( QWidget * parent, const QString & caption, const QString & text, const QString & button0Text, const QString & button1Text, const QString & button2Text, int defaultButtonNumber, int escapeButtonNumber)
{
	QString b0Text=button0Text, b1Text=button1Text, b2Text=button2Text;
	int defaultButton=defaultButtonNumber, escapeButton=escapeButtonNumber;
	int buttonsUsed=swapButtonValues(b0Text, b1Text, b2Text, defaultButton, escapeButton);
	int stdReturn=QMessageBox::information(parent, caption, text, b0Text, b1Text, b2Text, defaultButton, escapeButton);
	return swapReturnValues(0, 1, 2, stdReturn, buttonsUsed);
}

int ScMessageBox::question ( QWidget * parent, const QString & caption, const QString & text, int button0, int button1, int button2)
{
	int b0=button0, b1=button1, b2=button2;
	int buttonsUsed=swapButtonValues(b0, b1, b2);
	int stdReturn=QMessageBox::question(parent, caption, text, b0, b1, b2);
	return swapReturnValues(b0, b1, b2, stdReturn, buttonsUsed);
}

int ScMessageBox::question ( QWidget * parent, const QString & caption, const QString & text, const QString & button0Text, const QString & button1Text, const QString & button2Text, int defaultButtonNumber, int escapeButtonNumber)
{
	QString b0Text=button0Text, b1Text=button1Text, b2Text=button2Text;
	int defaultButton=defaultButtonNumber, escapeButton=escapeButtonNumber;
	int buttonsUsed=swapButtonValues(b0Text, b1Text, b2Text, defaultButton, escapeButton);
	int stdReturn=QMessageBox::question(parent, caption, text, b0Text, b1Text, b2Text, defaultButton, escapeButton);
	return swapReturnValues(0, 1, 2, stdReturn, buttonsUsed);
}

int ScMessageBox::warning ( QWidget * parent, const QString & caption, const QString & text, int button0, int button1, int button2)
{
	int b0=button0, b1=button1, b2=button2;
	int buttonsUsed=swapButtonValues(b0, b1, b2);
	int stdReturn=QMessageBox::warning(parent, caption, text, b0, b1, b2);
	return swapReturnValues(b0, b1, b2, stdReturn, buttonsUsed);
}

int ScMessageBox::warning ( QWidget * parent, const QString & caption, const QString & text, const QString & button0Text, const QString & button1Text, const QString & button2Text, int defaultButtonNumber, int escapeButtonNumber)
{
	QString b0Text=button0Text, b1Text=button1Text, b2Text=button2Text;
	int defaultButton=defaultButtonNumber, escapeButton=escapeButtonNumber;
	int buttonsUsed=swapButtonValues(b0Text, b1Text, b2Text, defaultButton, escapeButton);
	int stdReturn=QMessageBox::warning(parent, caption, text, b0Text, b1Text, b2Text, defaultButton, escapeButton);
	return swapReturnValues(0, 1, 2, stdReturn, buttonsUsed);
}

int ScMessageBox::critical ( QWidget * parent, const QString & caption, const QString & text, int button0, int button1, int button2)
{
	int b0=button0, b1=button1, b2=button2;
	int buttonsUsed=swapButtonValues(b0, b1, b2);
	int stdReturn=QMessageBox::critical(parent, caption, text, b0, b1, b2);
	return swapReturnValues(b0, b1, b2, stdReturn, buttonsUsed);
}

int ScMessageBox::critical ( QWidget * parent, const QString & caption, const QString & text, const QString & button0Text, const QString & button1Text, const QString & button2Text, int defaultButtonNumber, int escapeButtonNumber)
{
	QString b0Text=button0Text, b1Text=button1Text, b2Text=button2Text;
	int defaultButton=defaultButtonNumber, escapeButton=escapeButtonNumber;
	int buttonsUsed=swapButtonValues(b0Text, b1Text, b2Text, defaultButton, escapeButton);
	int stdReturn=QMessageBox::critical(parent, caption, text, b0Text, b1Text, b2Text, defaultButton, escapeButton);
	return swapReturnValues(0, 1, 2, stdReturn, buttonsUsed);
}


