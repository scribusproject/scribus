/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                  scmessagebox.cpp  -  replacement for QMessageBox
                             -------------------
    begin                : Oct 2005
    copyright            : (C) 2005 by Craig Bradney
    email                : cbradney@zip.com.au
    copyright            : (C) 2014 by William Bader
    email                : williambader@hotmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QMessageBox>
#include <iostream>

#include "scconfig.h"
#include "scribuscore.h"
#include "ui/scmessagebox.h"

void ScMessageBox::initScMessageBox(void)
{
	messageTitle = "";
	defaultBatchPushButton = NULL;
	defaultBatchStandardButton = QMessageBox::NoButton;
}

// Constructors for the property-based API

ScMessageBox::ScMessageBox(QWidget *parent):
			QMessageBox(parent)
{
	initScMessageBox();
}

ScMessageBox::ScMessageBox(Icon icon, const QString &title, const QString &text, StandardButtons buttons, QWidget *parent, Qt::WindowFlags flags):
			QMessageBox(icon, title, text, buttons, parent, flags)
{
	initScMessageBox();
	messageTitle = title;
}

// Find a default button in batch mode
//	Take the default batch button if present
//	Otherwise take the normal default button if present
//	Otherwise look for a standard button

QMessageBox::StandardButton ScMessageBox::findDefaultButton(QMessageBox::StandardButtons buttons,
				QMessageBox::StandardButton defaultButton, QMessageBox::StandardButton defaultBatchButton)
{
	QMessageBox::StandardButton result = ((defaultBatchButton != QMessageBox::NoButton)? defaultBatchButton: defaultButton);
	if (result == QMessageBox::NoButton)
	{
		QMessageBox::StandardButton buttonList[] = { QMessageBox::Yes, QMessageBox::Ok,
					QMessageBox::Cancel, QMessageBox::Close, QMessageBox::Discard,
					QMessageBox::No, QMessageBox::Abort, QMessageBox::Ignore, QMessageBox::NoButton };
		for (int i = 0; buttonList[i] != QMessageBox::NoButton; i++)
		{
			if ((buttons & buttonList[i]) != 0)
			{
				result = buttonList[i];
				break;
			}
		}
	}
	return result;
}

void ScMessageBox::setDefaultBatchButton(QPushButton *button)
{
	defaultBatchPushButton = button;
}

void ScMessageBox::setDefaultBatchButton(StandardButton button)
{
	defaultBatchStandardButton = button;
}

int ScMessageBox::exec()
{
	if (ScCore->usingGUI())
		return QMessageBox::exec();
	QString msg = messageTitle + text() + " " + informativeText() + " " + detailedText();
	std::cerr << msg.toLocal8Bit().data() << std::endl;
	return static_cast<int>(findDefaultButton(QMessageBox::standardButtons(), QMessageBox::NoButton, defaultBatchStandardButton));
}

QAbstractButton *ScMessageBox::clickedButton() const
{
	if (ScCore->usingGUI())
		return QMessageBox::clickedButton();
	return defaultBatchPushButton? defaultBatchPushButton: QMessageBox::defaultButton();
}

// Static function API
//	These functions correspond to the QMessageBox functions
//	except that they have an additional optional parameter
//	to set the returned button when there is no GUI.

QMessageBox::StandardButton ScMessageBox::information(QWidget *parent, const QString &title, const QString &text,
				StandardButtons buttons, StandardButton defaultButton, StandardButton defaultBatchButton)
{
	if (ScCore->usingGUI())
		return QMessageBox::information(parent, title, text, buttons, defaultButton);
	std::cerr << title.toLocal8Bit().data() << " " << text.toLocal8Bit().data() << std::endl;
	return findDefaultButton(buttons, defaultButton, defaultBatchButton);
}

QMessageBox::StandardButton ScMessageBox::question(QWidget *parent, const QString &title, const QString &text,
				StandardButtons buttons, StandardButton defaultButton, StandardButton defaultBatchButton)
{
	if (ScCore->usingGUI())
		return QMessageBox::question(parent, title, text, buttons, defaultButton);
	std::cerr << title.toLocal8Bit().data() << " " << text.toLocal8Bit().data() << std::endl;
	return findDefaultButton(buttons, defaultButton, defaultBatchButton);
}

QMessageBox::StandardButton ScMessageBox::warning(QWidget *parent, const QString &title, const QString &text,
				StandardButtons buttons, StandardButton defaultButton, StandardButton defaultBatchButton)
{
	if (ScCore->usingGUI())
		return QMessageBox::warning(parent, title, text, buttons, defaultButton);
	std::cerr << title.toLocal8Bit().data() << " " << text.toLocal8Bit().data() << std::endl;
	return findDefaultButton(buttons, defaultButton, defaultBatchButton);
}

QMessageBox::StandardButton ScMessageBox::critical(QWidget *parent, const QString &title, const QString &text,
				StandardButtons buttons, StandardButton defaultButton, StandardButton defaultBatchButton)
{
	if (ScCore->usingGUI())
		return QMessageBox::critical(parent, title, text, buttons, defaultButton);
	std::cerr << title.toLocal8Bit().data() << " " << text.toLocal8Bit().data() << std::endl;
	return findDefaultButton(buttons, defaultButton, defaultBatchButton);
}

void ScMessageBox::about(QWidget *parent, const QString &title, const QString &text)
{
	if (ScCore->usingGUI())
	{
		QMessageBox::about(parent, title, text);
		return;
	}
	std::cerr << title.toLocal8Bit().data() << " " << text.toLocal8Bit().data() << std::endl;
}

void ScMessageBox::aboutQt(QWidget *parent, const QString &title)
{
	if (ScCore->usingGUI())
	{
		QMessageBox::aboutQt(parent, title);
		return;
	}
	std::cerr << title.toLocal8Bit().data() << std::endl;
}
