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
#ifndef SCMESSAGEBOX_H
#define SCMESSAGEBOX_H

#include <QMessageBox>

#include "scribusapi.h"

 /**
  * \brief This class provides alternate versions of QMessageBox functions
  * that write to log files when Scribus does not have a GUI.
  * This class is identical to QMessageBox except that provides an optional default batch button.
  * In batch mode, functions return the default batch button when it is not NoButton,
  * otherwise they return the default button when it is not NoButton,
  * otherwise they return one of the standard buttons.
  * If a dialog asks "Are you sure that you want to do this?",
  * the default button can be No to keep users from accidentally destroying data,
  * while the batch button can be Yes to allow scripts to do what they need.
  * This class does not include QMessageBox functions marked obsolete in Qt5.
  * Use non-obsolete functions instead.
  */
class SCRIBUS_API ScMessageBox : public QMessageBox
{
	Q_OBJECT

public:
	// Property-based API
	ScMessageBox(QWidget *parent = 0);
	ScMessageBox(QMessageBox::Icon icon, const QString &title, const QString &text,
		QMessageBox::StandardButtons buttons = QMessageBox::NoButton, QWidget *parent = 0,
		Qt::WindowFlags flags = Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
	int exec();
	QAbstractButton *clickedButton() const;
	void setDefaultBatchButton(QPushButton *button);
	void setDefaultBatchButton(StandardButton button);

	// Static function API
	static QMessageBox::StandardButton information(QWidget *parent, const QString &title, const QString &text,
			StandardButtons buttons = Ok, StandardButton defaultButton = NoButton, StandardButton defaultBatchButton = NoButton);

	static QMessageBox::StandardButton question(QWidget *parent, const QString &title, const QString &text,
			StandardButtons buttons = StandardButtons(Yes | No), StandardButton defaultButton = NoButton, StandardButton defaultBatchButton = NoButton);

	static QMessageBox::StandardButton warning(QWidget *parent, const QString &title, const QString &text,
			StandardButtons buttons = Ok, StandardButton defaultButton = NoButton, StandardButton defaultBatchButton = NoButton);

	static QMessageBox::StandardButton critical(QWidget *parent, const QString &title, const QString &text,
			StandardButtons buttons = Ok, StandardButton defaultButton = NoButton, StandardButton defaultBatchButton = NoButton);

	static void about(QWidget *parent, const QString &title, const QString &text);
	static void aboutQt(QWidget *parent, const QString &title = QString());

private:
	// Saved fields for the property-based API
	QString messageTitle;
	QPushButton *defaultBatchPushButton;
	StandardButton defaultBatchStandardButton;

	// Initialize private variables
	void initScMessageBox(void);

	// Find the appropriate default button
	static QMessageBox::StandardButton findDefaultButton(QMessageBox::StandardButtons buttons, QMessageBox::StandardButton defaultButton, StandardButton defaultBatchButton);
};

#endif
