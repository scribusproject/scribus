/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef APPLYMASTERPAGEDIALOG_H
#define APPLYMASTERPAGEDIALOG_H

#include <QDialog>
#include <QString>

#include "scribusapi.h"
#include "scribusdoc.h"
#include "page.h"

class QCheckBox;
class QComboBox;
class QEvent;
class QGroupBox;
class QHBoxLayout;
class QLabel;
class QPushButton;
class QRadioButton;
class QSpacerItem;
class QSpinBox;
class QVBoxLayout;

class SCRIBUS_API ApplyMasterPageDialog : public QDialog
{
	Q_OBJECT

public:
	ApplyMasterPageDialog( QWidget* parent = 0 );
	~ApplyMasterPageDialog();
	
	virtual void changeEvent(QEvent *e);

	QLabel* masterPageLabel;
	QComboBox* masterPageComboBox;
	QGroupBox* applyToPageButtonGroup;
	QRadioButton* currentPageRadioButton;
	QRadioButton* evenPagesRadioButton;
	QRadioButton* oddPagesRadioButton;
	QRadioButton* allPagesRadioButton;
	QCheckBox* useRangeCheckBox;
	QSpinBox* fromPageSpinBox;
	QLabel* toPageLabel;
	QSpinBox* toPageSpinBox;
	QPushButton* okButton;
	QPushButton* cancelButton;

	virtual void setup( ScribusDoc * view, QString Nam );
	virtual QString getMasterPageName();
	virtual int getPageSelection();
	virtual bool usingRange();
	virtual int getFromPage();
	virtual int getToPage();

protected:
	QVBoxLayout* ApplyMasterPageDialogLayout;
	QHBoxLayout* templateNameLayout;
	QSpacerItem* spacer2;
	QVBoxLayout* applyToPageButtonGroupLayout;
	QHBoxLayout* rangeLayout;
	QSpacerItem* spacer3;
	QHBoxLayout* layout8;
	QSpacerItem* spacer1;

protected slots:
	virtual void languageChange();

private slots:
	virtual void checkRangeFrom();
	virtual void checkRangeTo();
	virtual void enableRange( bool enabled );
	virtual void rangeSelectable();
	virtual void singleSelectable();

};

#endif // APPLYMASTERPAGEDIALOG_H
