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
#include "scpage.h"
#include "ui/scrspinbox.h"

class QCheckBox;
class QComboBox;
class QEvent;
class QGroupBox;
class QHBoxLayout;
class QLabel;
class QPushButton;
class QRadioButton;
class QSpacerItem;
class QVBoxLayout;

class SCRIBUS_API ApplyMasterPageDialog : public QDialog
{
	Q_OBJECT

public:
	ApplyMasterPageDialog( QWidget* parent = nullptr );
	~ApplyMasterPageDialog() = default;

	QLabel* masterPageLabel { nullptr };
	QComboBox* masterPageComboBox { nullptr };
	QGroupBox* applyToPageButtonGroup { nullptr };
	QRadioButton* currentPageRadioButton { nullptr };
	QRadioButton* evenPagesRadioButton { nullptr };
	QRadioButton* oddPagesRadioButton { nullptr };
	QRadioButton* allPagesRadioButton { nullptr };
	QCheckBox* useRangeCheckBox { nullptr };
	ScrSpinBox* fromPageSpinBox { nullptr };
	QLabel* toPageLabel { nullptr };
	ScrSpinBox* toPageSpinBox { nullptr };
	QPushButton* okButton { nullptr };
	QPushButton* cancelButton { nullptr };

	virtual void setup(ScribusDoc* doc, const QString& Nam);
	virtual QString getMasterPageName();
	virtual int getPageSelection();
	virtual bool usingRange();
	virtual int getFromPage();
	virtual int getToPage();

protected:
	QVBoxLayout* ApplyMasterPageDialogLayout { nullptr };
	QHBoxLayout* templateNameLayout { nullptr };
	QSpacerItem* spacer2 { nullptr };
	QVBoxLayout* applyToPageButtonGroupLayout { nullptr };
	QHBoxLayout* rangeLayout { nullptr };
	QSpacerItem* spacer3 { nullptr };
	QHBoxLayout* layout8 { nullptr };
	QSpacerItem* spacer1 { nullptr };
	
	void changeEvent(QEvent *e) override;

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
