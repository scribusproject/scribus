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

#include <qvariant.h>
#include <qdialog.h>
#include "scribusdoc.h"
#include "page.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QComboBox;
class QButtonGroup;
class QRadioButton;
class QCheckBox;
class QSpinBox;
class QPushButton;

class ApplyMasterPageDialog : public QDialog
{
	Q_OBJECT

public:
	ApplyMasterPageDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
	~ApplyMasterPageDialog();

	QLabel* masterPageLabel;
	QComboBox* masterPageComboBox;
	QButtonGroup* applyToPageButtonGroup;
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
