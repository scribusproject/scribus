/***************************************************************************
                              guidemanager.cpp
                             -------------------
    begin                : Fri Jan 30 2004
    copyright            : (C) 2004 by Alessandro Rimoldi
    copyright            : (C) by Franz Schmid
    e-mail               : http://www.ideale.ch/contact
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "scribus.h"
#include "guidemanager.h"
#include "guidemanager.moc"

#include "units.h"
#include <qradiobutton.h>
#include <qlayout.h>
#include <qcursor.h>

#include "commonstrings.h"

extern ScribusApp *ScApp;
extern QPixmap loadIcon(QString nam);

GuideManager::GuideManager(QWidget* parent) : QDialog(parent, "GuideManager", true, 0)
{
	// whatif selection settings
	FPoint SelectionTopLeft = FPoint(0, 0);
	FPoint SelectionBottomRight = FPoint(0, 0);
	FPoint MarginTopLeft(ScApp->doc->pageMargins.Top, ScApp->doc->pageMargins.Bottom);
	FPoint MarginBottomRight(ScApp->doc->pageMargins.Left, ScApp->doc->pageMargins.Right);

	if (ScApp->view->SelItem.count() > 1)
	{
		SelectionTopLeft.setXY(ScApp->view->GroupX - ScApp->doc->ScratchLeft,
							   ScApp->view->GroupY - ScApp->doc->ScratchTop);
		SelectionBottomRight.setXY(ScApp->view->GroupW,
								   ScApp->view->GroupH);
	}
	else if (ScApp->view->SelItem.count() == 1)
	{
		PageItem *currItem = ScApp->view->SelItem.at(0);
		SelectionTopLeft.setXY(currItem->BoundingX - ScApp->doc->ScratchLeft,
							   currItem->BoundingY - ScApp->doc->ScratchTop);
		SelectionBottomRight.setXY(currItem->BoundingW, currItem->BoundingH);
	}

	QString tmp;
	setCaption( tr("Manage Guides"));
	setIcon(loadIcon("AppIcon.png"));

	/* Initialise the global variables */
	docUnitIndex = ScApp->doc->docUnitIndex;
	docUnitRatio = unitGetRatioFromIndex(docUnitIndex);
	int decimals = unitGetDecimalsFromIndex(docUnitIndex);

	horizontalGuides = ScApp->doc->currentPage->YGuides; // in page XGuides and YGuides are inverted
	verticalGuides = ScApp->doc->currentPage->XGuides;
	LocPageWidth = ScApp->doc->currentPage->Width;
	LocPageHeight = ScApp->doc->currentPage->Height;
	lockedGuides = ScApp->doc->GuideLock;

	LocLeft = MarginTopLeft.x();
	LocTop = MarginTopLeft.y();
	LocRight = MarginBottomRight.x();
	LocBottom = MarginBottomRight.y();

	bool selected = true;

	if (SelectionBottomRight != FPoint(0, 0))
	{
		gy = SelectionTopLeft.y();
		gx = SelectionTopLeft.x();
		gw = SelectionBottomRight.x();
		gh = SelectionBottomRight.y();
	}
	else selected=false;

	selHor = selVer = -1;

	/* Create the dialog elements */
	QVBoxLayout *GuideManagerLayout = new QVBoxLayout(this, 11, 6, "GuideManagerLayout");

	QHBoxLayout *Layout6 = new QHBoxLayout(0, 0, 6, "Layout6");

	QGroupBox *HorGroup = new QGroupBox(this, "HorGroup");
	HorGroup->setTitle( tr("Horizontal Guides"));
	HorGroup->setColumnLayout(0, Qt::Vertical);
	HorGroup->layout()->setSpacing(6);
	HorGroup->layout()->setMargin(11);
	QVBoxLayout *HorGroupLayout = new QVBoxLayout(HorGroup->layout());
	HorGroupLayout->setAlignment(Qt::AlignTop);

	HorList = new QListBox(HorGroup, "HorList");
	HorList->setMinimumSize(QSize(0, 150));
	HorGroupLayout->addWidget(HorList);

	QHBoxLayout *Layout4 = new QHBoxLayout(0, 0, 6, "Layout4");

	TextLabel2 = new QLabel( tr("&Y-Pos:"), HorGroup, "TextLabel2");
	Layout4->addWidget(TextLabel2);

	HorSpin = new MSpinBox(0, LocPageHeight, HorGroup, 4);
	HorSpin->setDecimals(decimals);
	TextLabel2->setBuddy(HorSpin);
	Layout4->addWidget(HorSpin);

	HorGroupLayout->addLayout(Layout4);

	QHBoxLayout *Layout3 = new QHBoxLayout(0, 0, 6, "Layout3");

	HorSet = new QPushButton( tr( "&Add" ), HorGroup, "HorSet");
	HorSet->setAutoDefault(false);
	Layout3->addWidget(HorSet);

	HorDel = new QPushButton( tr( "D&elete" ), HorGroup, "HorDel");
	HorDel->setAutoDefault(false);
	Layout3->addWidget(HorDel);

	HorGroupLayout->addLayout(Layout3);

	Layout6->addWidget(HorGroup);

	VerGroup = new QGroupBox(this, "VerGroup");
	VerGroup->setTitle( tr("Vertical Guides"));
	VerGroup->setColumnLayout(0, Qt::Vertical);
	VerGroup->layout()->setSpacing(6);
	VerGroup->layout()->setMargin(11);
	QVBoxLayout *VerGroupLayout = new QVBoxLayout(VerGroup->layout());
	VerGroupLayout->setAlignment(Qt::AlignTop);

	VerList = new QListBox(VerGroup, "VerList");
	VerList->setMinimumSize(QSize(0, 150));
	VerGroupLayout->addWidget(VerList);

	QHBoxLayout *Layout2 = new QHBoxLayout(0, 0, 6, "Layout2");

	TextLabel1 = new QLabel( tr("&X-Pos:"), VerGroup, "TextLabel1");
	Layout2->addWidget(TextLabel1);

	VerSpin = new MSpinBox(0, LocPageWidth, VerGroup, 4);
	VerSpin->setDecimals(decimals);
	TextLabel1->setBuddy(VerSpin);
	Layout2->addWidget(VerSpin);

	VerGroupLayout->addLayout(Layout2);

	QHBoxLayout *Layout1 = new QHBoxLayout(0, 0, 6, "Layout1");

	VerSet = new QPushButton( tr( "A&dd" ), VerGroup, "VerSet");
	VerSet->setAutoDefault(false);
	Layout1->addWidget(VerSet);

	VerDel = new QPushButton( tr( "De&lete" ), VerGroup, "VerDel");
	VerDel->setAutoDefault(false);
	Layout1->addWidget(VerDel);

	VerGroupLayout->addLayout(Layout1);

	Layout6->addWidget(VerGroup);

	GuideManagerLayout->addLayout(Layout6);

	QHBoxLayout *Layout7 = new QHBoxLayout(0, 0, 6, "Layout7");

	// auto guides
	QGroupBox *HorGroup2 = new QGroupBox(this, "HorGroup");
	HorGroup2->setTitle( tr("Rows and Columns - Automatic Guides"));
	HorGroup2->setColumnLayout(0, Qt::Vertical);
	HorGroup2->layout()->setSpacing(6);
	HorGroup2->layout()->setMargin(11);
	QVBoxLayout *HorGroupLayout2 = new QVBoxLayout(HorGroup2->layout());
	HorGroupLayout2->setAlignment(Qt::AlignTop);

	// auto guides basic
	QLabel *TextLabel8 = new QLabel( tr("&Rows:"), HorGroup2, "TextLabel8");
	RowSpin = new QSpinBox(1, 100, 1, HorGroup2, "rowspin");
	TextLabel8->setBuddy(RowSpin);

	QLabel *TextLabel9 = new QLabel( tr("C&olumns:"), HorGroup2, "TextLabel9");
	ColSpin = new QSpinBox(1, 100, 1, HorGroup2, "ColSpin");
	TextLabel9->setBuddy(ColSpin);

	QVBoxLayout *rcLayout = new QVBoxLayout(0, 0, 6, "rcLayout");
	rcLayout->addWidget(TextLabel8);
	rcLayout->addWidget(RowSpin);
	rcLayout->addWidget(TextLabel9);
	rcLayout->addWidget(ColSpin);

	// auto guides gaps
	useRowGap = new QCheckBox( tr("Row &Gap"), HorGroup2, "useRowGap");
	useRowGap->setChecked(false);
	rowGap = new MSpinBox(0, 100, HorGroup2, 4);
	rowGap->setValue(0);
	rowGap->setDecimals(decimals);
	rowGap->setEnabled(false);

	useColGap = new QCheckBox( tr("Colum&n Gap"), HorGroup2, "useColGap");
	useColGap->setChecked(false);
	colGap = new MSpinBox(0, 100, HorGroup2, 4);
	colGap->setValue(0);
	colGap->setDecimals(decimals);
	colGap->setEnabled(false);

	QVBoxLayout *rcGapLayout = new QVBoxLayout(0, 0, 6, "rcGapLayout");
	rcGapLayout->addWidget(useRowGap);
	rcGapLayout->addWidget(rowGap);
	rcGapLayout->addWidget(useColGap);
	rcGapLayout->addWidget(colGap);

	// auto guides formating
	QHBoxLayout *autoGuidesLayout = new QHBoxLayout(0, 0, 6, "autoGuidesLayout");
	autoGuidesLayout->addLayout(rcLayout);
	autoGuidesLayout->addLayout(rcGapLayout);

	QHBoxLayout *Layout10 = new QHBoxLayout(0, 0, 6, "Layout10");
	BGroup = new QHButtonGroup(HorGroup2,"bgroup");
	BGroup->setFrameStyle(QFrame::NoFrame);
	QLabel *TextLabel10 = new QLabel( tr("Refer to:"), HorGroup2, "TextLabel10");
	Layout10->addWidget(TextLabel10);
	QRadioButton *fPage = new QRadioButton( tr( "&Page" ), BGroup, "fpage");
	fPage->setChecked(true);
	(void) new QRadioButton( tr( "&Margins" ), BGroup, "fmargin");
	QRadioButton *fSelect = new QRadioButton( tr( "&Selection" ), BGroup, "fselect");
	fSelect->setEnabled(selected);
	Layout10->addWidget(BGroup);

	QSpacerItem* spacer2 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
	Layout10->addItem(spacer2);

	Layout7->addWidget(HorGroup2);
	HorGroupLayout2->addLayout(autoGuidesLayout);
	HorGroupLayout2->addLayout(Layout10);

	GuideManagerLayout->addLayout(Layout7);

	QHBoxLayout *Layout5 = new QHBoxLayout(0, 0, 6, "Layout5");
	Lock = new QCheckBox( tr( "&Lock Guides" ), this, "Lock");
	Lock->setChecked(lockedGuides);
	Layout5->addWidget(Lock);

	QSpacerItem* spacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
	Layout5->addItem(spacer);

	okButton = new QPushButton( CommonStrings::tr_OK, this, "okButton");
	okButton->setAutoDefault(false);
	cancelButton = new QPushButton( tr( "&Close" ), this, "cancelButton");
	cancelButton->setAutoDefault(false);
	cancelButton->setAccel(QKeySequence("Esc"));
	setButton = new QPushButton( tr("&Update"), this, "setButton");
	setButton->setAutoDefault(false);
	Layout5->addWidget(setButton);
	Layout5->addWidget(okButton);
	Layout5->addWidget(cancelButton);

	GuideManagerLayout->addLayout(Layout5);

	unitChange();

	//tooltips
	QToolTip::add( setButton, "<qt>" + tr("Set the guides in document. Guide manager is still opened but the changes are persistant", "guide manager") + "</qt>");

	// Create signals and slots connections
	connect(okButton, SIGNAL(clicked()), this, SLOT(commitChanges()));
	connect(setButton, SIGNAL(clicked()), this, SLOT(commitEditChanges()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	connect(HorList, SIGNAL(highlighted(QListBoxItem*)), this, SLOT(selHorIte(QListBoxItem*)));
	connect(VerList, SIGNAL(highlighted(QListBoxItem*)), this, SLOT(selVerIte(QListBoxItem*)));
	connect(HorSet, SIGNAL(clicked()), this, SLOT(AddHorVal()));
	connect(HorDel, SIGNAL(clicked()), this, SLOT(DelHorVal()));
	connect(HorSpin, SIGNAL(valueChanged(int)), this, SLOT(ChangeHorVal()));
	connect(VerSet, SIGNAL(clicked()), this, SLOT(AddVerVal()));
	connect(VerDel, SIGNAL(clicked()), this, SLOT(DelVerVal()));
	connect(VerSpin, SIGNAL(valueChanged(int)), this, SLOT(ChangeVerVal()));
	connect(Lock, SIGNAL(clicked()), this, SLOT(HandleLock()));
	connect(useRowGap, SIGNAL(toggled(bool)), this, SLOT(useRowGap_clicked(bool)));
	connect(useColGap, SIGNAL(toggled(bool)), this, SLOT(useColGap_clicked(bool)));

	UpdateHorList();
	UpdateVerList();

	/* Intialise selection in ListBoxes */
	if (HorList->numRows() > 0)
	{
		HorList->setCurrentItem(0);
		HorDel->setEnabled(true);
	}
	else
	{
		HorDel->setEnabled(false);
		HorSpin->setValue(0);
		if (VerList->numRows() > 0)
		{
			VerList->setCurrentItem(0);
			VerDel->setEnabled(true);
		}
		else
		{
			VerSpin->setValue(0);
			VerDel->setEnabled(false);
		}
	}
}

void GuideManager::DelHorVal()
{
	QValueList<double>::Iterator it;
	it = horizontalGuides.at(selHor);
	it = horizontalGuides.remove(it);
	if (horizontalGuides.isEmpty())
		selHor = -1;
	else if (selHor > static_cast<int>(horizontalGuides.count() - 1))
		selHor--;
	if (selHor == -1)
		HorDel->setEnabled(false);
	UpdateHorList();
}

void GuideManager::DelVerVal()
{
	QValueList<double>::Iterator it;
	it = verticalGuides.at(selVer);
	it = verticalGuides.remove(it);
	if (verticalGuides.isEmpty())
		selVer = -1;
	else if (selVer > static_cast<int>(verticalGuides.count() - 1))
		selVer--;
	if (selVer == -1)
		VerDel->setEnabled(false);
	UpdateVerList();
}

void GuideManager::AddHorVal()
{
	if (HorSpin->hasFocus())
	HorSpin->clearFocus();
	if (VerSpin->hasFocus())
	VerSpin->clearFocus();
	horizontalGuides.prepend(0);
	selHor = 0;

	VerList->clearSelection();
	VerSpin->setEnabled(false);
	VerDel->setEnabled(false);

	HorDel->setEnabled(true);
	UpdateHorList();
	HorList->setCurrentItem(0);
	HorList->setSelected(0,true);
	HorSpin->setFocus();
	HorSpin->selectAll();
}

QValueList<double> GuideManager::getRowValues()
{
	int n = QString(RowSpin->text()).toInt();
	double offset = 0;
	double NewPageHeight = LocPageHeight;

	if (BGroup->selectedId() == 1)
	{
		NewPageHeight = LocPageHeight - LocTop - LocBottom;
		offset = LocTop;
	}
	else if (BGroup->selectedId() == 2)
	{
		offset = gy;
		NewPageHeight = gh;
	}

	double spacing = NewPageHeight / n;
	QValueList<double> values;
	for (int i = 1; i < n; ++i)
	{
		if (useRowGap->isChecked())
		{
			values.append(offset + spacing * i + rowGap->value() / 2);
			values.append(offset + spacing * i - rowGap->value() / 2);
		}
		else
			values.append(offset + spacing * i);
	}
	return values;
}

QValueList<double> GuideManager::getColValues()
{
	int n = QString(ColSpin->text()).toInt();
	double offset = 0;
	double NewPageWidth = LocPageWidth;

	if (BGroup->selectedId() == 1)
	{
		NewPageWidth = LocPageWidth - LocLeft - LocRight;
		offset = LocLeft;
	}
	else if (BGroup->selectedId() == 2)
	{
		offset = gx;
		NewPageWidth = gw;
	}

	double spacing = NewPageWidth / n;
	QValueList<double> values;
	for (int i = 1; i < n; ++i)
	{
		if (useColGap->isChecked())
		{
			values.append(offset + spacing * i + colGap->value() / 2);
			values.append(offset + spacing * i - colGap->value() / 2);
		}
		else
			values.append(offset + spacing * i);
	}
	return values;
}

void GuideManager::addRows()
{
	QValueList<double>::iterator it;
	QValueList<double> list = getRowValues();
	for (it = list.begin(); it != list.end(); ++it)
	{
		if (!horizontalGuides.contains((*it)))
			horizontalGuides.append((*it));
	}
	UpdateHorList();
}

void GuideManager::addCols()
{
	QValueList<double>::iterator it;
	QValueList<double> list = getColValues();
	for (it = list.begin(); it != list.end(); ++it)
	{
		if (!verticalGuides.contains((*it)))
			verticalGuides.append((*it));
	}
	UpdateVerList();
}

void GuideManager::AddVerVal()
{
	if (HorSpin->hasFocus())
		HorSpin->clearFocus();
	if (VerSpin->hasFocus())
		VerSpin->clearFocus();
	verticalGuides.prepend(0);
	selVer = 0;

	HorList->clearSelection();
	HorSpin->setEnabled(false);
	HorDel->setEnabled(false);

	VerDel->setEnabled(true);
	UpdateVerList();
	VerList->setCurrentItem(0);
	VerList->setSelected(0,true);
	VerSpin->setFocus();
	VerSpin->selectAll();
}

void GuideManager::HandleLock()
{
	lockedGuides = Lock->isChecked();
}

void GuideManager::selHorIte(QListBoxItem *c)
{
	disconnect(HorSpin, SIGNAL(valueChanged(int)), this, SLOT(ChangeHorVal()));
	selHor = c->listBox()->currentItem();
	HorDel->setEnabled(true);
	VerDel->setEnabled(false);
	VerList->clearSelection();
	VerSpin->setEnabled(false);
	HorSpin->setEnabled(true);
	HorSpin->setValue(horizontalGuides[selHor] * docUnitRatio);
	connect(HorSpin, SIGNAL(valueChanged(int)), this, SLOT(ChangeHorVal()));
}

void GuideManager::selVerIte(QListBoxItem *c)
{
	disconnect(VerSpin, SIGNAL(valueChanged(int)), this, SLOT(ChangeVerVal()));
	selVer = c->listBox()->currentItem();
	VerDel->setEnabled(true);
	HorDel->setEnabled(false);
	HorList->clearSelection();
	HorSpin->setEnabled(false);
	VerSpin->setEnabled(true);
	VerSpin->setValue(verticalGuides[selVer] * docUnitRatio);
	connect(VerSpin, SIGNAL(valueChanged(int)), this, SLOT(ChangeVerVal()));
}

void GuideManager::ChangeHorVal()
{
	int n = static_cast<int>(horizontalGuides.count());
	int m = n;
	double curHor = HorSpin->value() / docUnitRatio;

	QValueList<double>::Iterator it = horizontalGuides.at(selHor);
	horizontalGuides.remove(it);

	for (int i = n - 1; i > 0; i--)
		if (curHor < horizontalGuides[i - 1])
			m = i;

	selHor = m - 1;

	if (m == n)
	{
		horizontalGuides.append(curHor);
		selHor = m - 1;
	}
	else
	{
		it = horizontalGuides.at(selHor);
		horizontalGuides.insert(it,curHor);
	}
	UpdateHorList();
}

void GuideManager::ChangeVerVal()
{
	int n = static_cast<int>(verticalGuides.count());
	int m = n;
	double curVer = VerSpin->value() / docUnitRatio;

	QValueList<double>::Iterator it = verticalGuides.at(selVer);
	verticalGuides.remove(it);

	for (int i = n - 1; i > 0; i--)
		if (curVer < verticalGuides[i - 1])
			m = i;

	selVer = m - 1;

	if (m == n)
	{
		verticalGuides.append(curVer);
		selVer = m - 1;
	}
	else
	{
		it = verticalGuides.at(selVer);
		verticalGuides.insert(it,curVer);
	}
	UpdateVerList();
}

void GuideManager::unitChange()
{
	QString tmp = unitGetSuffixFromIndex(docUnitIndex);
	HorSpin->setSuffix(tmp);
	VerSpin->setSuffix(tmp);
	rowGap->setSuffix(tmp);
	colGap->setSuffix(tmp);
	int decimals = unitGetDecimalsFromIndex(docUnitIndex);
	HorSpin->setDecimals(decimals);
	VerSpin->setDecimals(decimals);
	rowGap->setDecimals(decimals);
	colGap->setDecimals(decimals);
}

void GuideManager::UpdateHorList()
{
	disconnect(HorList, SIGNAL(highlighted(QListBoxItem*)), this, SLOT(selHorIte(QListBoxItem*)));
	disconnect(HorSpin, SIGNAL(valueChanged(int)), this, SLOT(ChangeHorVal()));

	QString tmp;
	int precision=unitGetPrecisionFromIndex(docUnitIndex);
	QString suffix=unitGetSuffixFromIndex(docUnitIndex);

	HorList->clear();

	for (uint i = 0; i < horizontalGuides.count(); ++ i)
		HorList->insertItem(tmp.setNum(qRound(horizontalGuides[i] * docUnitRatio * 10000.0) / 10000.0, 'f', precision) + suffix);
	if (horizontalGuides.isEmpty())
		selHor = -1;

	if (selHor != -1)
		HorList->setCurrentItem(selHor);
	HorSpin->setEnabled(selHor != -1 ? true : false);
	HorSpin->setValue(selHor != -1 ? (horizontalGuides[selHor] * docUnitRatio * 10000.0) / 10000.0 : 0);

	connect(HorList, SIGNAL(highlighted(QListBoxItem*)), this, SLOT(selHorIte(QListBoxItem*)));
	connect(HorSpin, SIGNAL(valueChanged(int)), this, SLOT(ChangeHorVal()));
}

void GuideManager::UpdateVerList()
{
	disconnect(VerList, SIGNAL(highlighted(QListBoxItem*)), this, SLOT(selVerIte(QListBoxItem*)));
	disconnect(VerSpin, SIGNAL(valueChanged(int)), this, SLOT(ChangeVerVal()));

	QString tmp;
	int precision=unitGetPrecisionFromIndex(docUnitIndex);
	QString suffix=unitGetSuffixFromIndex(docUnitIndex);

	VerList->clear();

	for (uint i = 0; i < verticalGuides.count(); ++ i)
		VerList->insertItem(tmp.setNum(qRound(verticalGuides[i] * docUnitRatio * 10000.0) / 10000.0, 'f', precision) + suffix);
	if (verticalGuides.isEmpty())
		selVer = -1;

	if (selVer != -1)
		VerList->setCurrentItem(selVer);

	VerSpin->setEnabled(selVer != -1 ? true : false);
	VerSpin->setValue(selVer != -1 ? (verticalGuides[selVer] * docUnitRatio * 10000.0) / 10000.0 : 0);

	connect(VerList, SIGNAL(highlighted(QListBoxItem*)), this, SLOT(selVerIte(QListBoxItem*)));
	connect(VerSpin, SIGNAL(valueChanged(int)), this, SLOT(ChangeVerVal()));
}

void GuideManager::refreshDoc()
{
	ScApp->doc->currentPage->addXGuides(verticalGuides);
	ScApp->doc->currentPage->addYGuides(horizontalGuides);
	ScApp->doc->lockGuides(lockedGuides);
	ScApp->view->DrawNew();
}

void GuideManager::useRowGap_clicked(bool state)
{
	rowGap->setEnabled(state);
}

void GuideManager::useColGap_clicked(bool state)
{
	colGap->setEnabled(state);
}

void GuideManager::commitChanges()
{
	commitEditChanges();
	return accept();
}

void GuideManager::commitEditChanges()
{
	qApp->setOverrideCursor(QCursor(waitCursor), true);
	if (RowSpin->value() > 1)
		addRows();
	if (ColSpin->value() > 1)
		addCols();
	refreshDoc();
	QApplication::restoreOverrideCursor();
}
