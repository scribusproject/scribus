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

/***************************************************************************
 * guides manager for scribus
 * .inspired by tabmanager.cpp and guidemanager.cpp by franz shmid
 * .porting structure and functionality of the tabmanager to the guides
 * .the dialog layout hasn't been modified
 * .first steps in unifying the variable naming schema...
 * .a couple of bugs have been corrected (delete was enabled even when
 *  there were no more guides/tabs)
 * .a couple of enhancements: guides can be "moved"; to delete all the guides,
 *  just keep on clicking on delete; guides can be entered
 *  without using the mouse (click on add, write the measure, press
 *  tab, press space, enter the next guide)
 *  why do the VerXXX variables refer to the horizontal guides? Should
 *  i change this? Should the precision depend on the unit used? Why are
 *  all the methods and properties public? Only LocHor and LocVer should
 *  be public!
 * - bug 356:
 *   a: ok
 *   d: ok
 *   e: ok
 *   f: press tab to make it update! (no focus in/out signal for qspinbox)
 * - added the esc key
 ***************************************************************************/

#include "guidemanager.h"
#include "guidemanager.moc"

#include "units.h"

extern QPixmap loadIcon(QString nam);

GuideManager::GuideManager(
			QWidget* parent,
			QValueList<double> XGuides,
			QValueList<double> YGuides,
			double PageW,
			double PageH,
			double topM,
			double bottomM,
			double leftM,
			double rightM,
			bool GuideLock,
			int Einh
		) : QDialog(parent, "GuideManager", true, 0)
{
	QString tmp;
	setCaption(tr("Manage Guides"));
	setIcon(loadIcon("AppIcon.png"));

	/* Initialise the global variables */
	docUnitIndex = Einh;
	docUnitRatio = unitGetRatioFromIndex(docUnitIndex);
	int decimals = unitGetDecimalsFromIndex(docUnitIndex);

	LocHor = YGuides; // in page XGuides and YGuides are inverted
	LocVer = XGuides;
	LocPageWidth = PageW;
	LocPageHeight = PageH;
	LocLocked = GuideLock;
	LocTop=topM;
	LocBottom=bottomM;
	LocRight=rightM;
	LocLeft=leftM;

	selHor = selVer = -1;

	/* Create the dialog elements */
	GuideManagerLayout = new QVBoxLayout(this, 11, 6, "GuideManagerLayout");

	Layout6 = new QHBoxLayout(0, 0, 6, "Layout6");

	HorGroup = new QGroupBox(this, "HorGroup");
	HorGroup->setTitle( tr("Horizontal Guides"));
	HorGroup->setColumnLayout(0, Qt::Vertical);
	HorGroup->layout()->setSpacing(6);
	HorGroup->layout()->setMargin(11);
	HorGroupLayout = new QVBoxLayout(HorGroup->layout());
	HorGroupLayout->setAlignment(Qt::AlignTop);

	HorList = new QListBox(HorGroup, "HorList");
	HorList->setMinimumSize(QSize(0, 150));
	HorGroupLayout->addWidget(HorList);

	Layout4 = new QHBoxLayout(0, 0, 6, "Layout4");

	TextLabel2 = new QLabel(tr("&Y-Pos:"), HorGroup, "TextLabel2");
	Layout4->addWidget(TextLabel2);

	HorSpin = new MSpinBox(0, LocPageHeight, HorGroup, 4);
	HorSpin->setDecimals(decimals);
	TextLabel2->setBuddy(HorSpin);
	Layout4->addWidget(HorSpin);

	HorGroupLayout->addLayout(Layout4);

	Layout3 = new QHBoxLayout(0, 0, 6, "Layout3");

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
	VerGroupLayout = new QVBoxLayout(VerGroup->layout());
	VerGroupLayout->setAlignment(Qt::AlignTop);

	VerList = new QListBox(VerGroup, "VerList");
	VerList->setMinimumSize(QSize(0, 150));
	VerGroupLayout->addWidget(VerList);

	Layout2 = new QHBoxLayout(0, 0, 6, "Layout2");

	TextLabel1 = new QLabel(tr("&X-Pos:"), VerGroup, "TextLabel1");
	Layout2->addWidget(TextLabel1);

	VerSpin = new MSpinBox(0, LocPageWidth, VerGroup, 4);
	VerSpin->setDecimals(decimals);
	TextLabel1->setBuddy(VerSpin);
	Layout2->addWidget(VerSpin);

	VerGroupLayout->addLayout(Layout2);

	Layout1 = new QHBoxLayout(0, 0, 6, "Layout1");

	VerSet = new QPushButton( tr( "A&dd" ), VerGroup, "VerSet");
	VerSet->setAutoDefault(false);
	Layout1->addWidget(VerSet);

	VerDel = new QPushButton( tr( "De&lete" ), VerGroup, "VerDel");
	VerDel->setAutoDefault(false);
	Layout1->addWidget(VerDel);

	VerGroupLayout->addLayout(Layout1);

	Layout6->addWidget(VerGroup);

	GuideManagerLayout->addLayout(Layout6);


	Layout7 = new QHBoxLayout(0, 0, 6, "Layout7");

	QGroupBox *HorGroup2 = new QGroupBox(this, "HorGroup");
	HorGroup2->setTitle( tr("Rows and Columns"));
	HorGroup2->setColumnLayout(0, Qt::Vertical);
	HorGroup2->layout()->setSpacing(6);
	HorGroup2->layout()->setMargin(11);
	QVBoxLayout *HorGroupLayout2 = new QVBoxLayout(HorGroup2->layout());
	HorGroupLayout2->setAlignment(Qt::AlignTop);

	Layout8 = new QHBoxLayout(0, 0, 6, "Layout8");
	QLabel *TextLabel8 = new QLabel(tr("&Rows:"), HorGroup2, "TextLabel8");
	Layout8->addWidget(TextLabel8);

	RowSpin = new QSpinBox(1, 100, 1, HorGroup2, "rowspin");
	TextLabel8->setBuddy(RowSpin);
	Layout8->addWidget(RowSpin);

	QPushButton *RowSet = new QPushButton( tr( "&Create" ), HorGroup2, "RowSet");
	RowSet->setAutoDefault(false);
	Layout8->addWidget(RowSet);

	Layout9 = new QHBoxLayout(0, 0, 6, "Layout9");
	QLabel *TextLabel9 = new QLabel(tr("C&olumns:"), HorGroup2, "TextLabel9");
	Layout9->addWidget(TextLabel9);

	ColSpin = new QSpinBox(1, 100, 1, HorGroup2, "ColSpin");
	TextLabel9->setBuddy(ColSpin);
	Layout9->addWidget(ColSpin);

	QPushButton *ColSet = new QPushButton( tr( "Cr&eate" ), HorGroup2, "ColSet");
	ColSet->setAutoDefault(false);
	Layout9->addWidget(ColSet);

	Layout10 = new QHBoxLayout(0, 0, 6, "Layout10");
	Marg = new QCheckBox( tr( "&Follow Margins" ), HorGroup2, "marg");
	Layout10->addWidget(Marg);

	QSpacerItem* spacer2 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
	Layout10->addItem(spacer2);

	Layout7->addWidget(HorGroup2);
	HorGroupLayout2->addLayout(Layout8);
	HorGroupLayout2->addLayout(Layout9);
	HorGroupLayout2->addLayout(Layout10);

	GuideManagerLayout->addLayout(Layout7);

	Layout5 = new QHBoxLayout(0, 0, 6, "Layout5");
	Lock = new QCheckBox( tr( "&Lock Guides" ), this, "Lock");
	Lock->setChecked(LocLocked);
	Layout5->addWidget(Lock);

	QSpacerItem* spacer = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
	Layout5->addItem(spacer);

	OK = new QPushButton( tr( "&OK" ), this, "OK");
	OK->setAutoDefault(false);
	Layout5->addWidget(OK );
	Cancel = new QPushButton( tr( "&Cancel" ), this, "Cancel");
	Cancel->setAccel(QKeySequence("Esc"));
	Layout5->addWidget(Cancel);
	Cancel->setAutoDefault(false);

	GuideManagerLayout->addLayout(Layout5);

	/* Initialise the units */
	UnitChange();

	// Create signals and slots connections
	connect( OK, SIGNAL(clicked()), this, SLOT(accept() ));
	connect( Cancel, SIGNAL(clicked()), this, SLOT(reject()));
	connect(HorList, SIGNAL(highlighted(QListBoxItem*)), this, SLOT(selHorIte(QListBoxItem*)));
	connect(VerList, SIGNAL(highlighted(QListBoxItem*)), this, SLOT(selVerIte(QListBoxItem*)));
	connect(HorSet, SIGNAL(clicked()), this, SLOT(AddHorVal()));
	connect(RowSet, SIGNAL(clicked()), this, SLOT(AddRows()));
	connect(ColSet, SIGNAL(clicked()), this, SLOT(AddCols()));

	connect(HorDel, SIGNAL(clicked()), this, SLOT(DelHorVal()));
	connect(HorSpin, SIGNAL(valueChanged(int)), this, SLOT(ChangeHorVal()));
	connect(VerSet, SIGNAL(clicked()), this, SLOT(AddVerVal()));
	connect(VerDel, SIGNAL(clicked()), this, SLOT(DelVerVal()));
	connect(VerSpin, SIGNAL(valueChanged(int)), this, SLOT(ChangeVerVal()));
	connect(Lock, SIGNAL(clicked()), this, SLOT(HandleLock()));

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
	it = LocHor.at(selHor);
	it = LocHor.remove(it);
	if (LocHor.isEmpty())
		selHor = -1;
	else if (selHor > static_cast<int>(LocHor.count()-1))
		selHor--;
	if (selHor == -1)
		HorDel->setEnabled(false);
	UpdateHorList();
}

void GuideManager::DelVerVal()
{
	QValueList<double>::Iterator it;
	it = LocVer.at(selVer);
	it = LocVer.remove(it);
	if (LocVer.isEmpty())
		selVer = -1;
	else if (selVer > static_cast<int>(LocVer.count()-1))
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
	LocHor.prepend(0);
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

void GuideManager::AddRows()
{
	int n = QString(RowSpin->text()).toInt();
	double offset = 0;
	double NewPageHeight = LocPageHeight;

	if (Marg->isChecked())
	{
		NewPageHeight = LocPageHeight - LocTop - LocBottom;
		offset = LocTop;
	}

	double spacing = NewPageHeight / n;
	int selHor = 0;
	QValueList<double>::Iterator it;

	for (int i=1;i<n;i++)
	{
		int n = static_cast<int>(LocHor.count());
		int m = n;
		double curHor = offset + spacing * i;

		if (LocHor.contains(curHor))
			continue;

		for (int i = n - 1; i > 0; i--)
			if (curHor < LocHor[i - 1])
				m = i;

		selHor = m - 1;

		if (m == n)
		{
			LocHor.append(curHor);
			selHor = m - 1;
		}
		else
		{
			it = LocHor.at(selHor);
			LocHor.insert(it,curHor);
		}
	}
	UpdateHorList();
}

void GuideManager::AddCols()
{
	int n = QString(ColSpin->text()).toInt();
	double offset = 0;
	double NewPageWidth = LocPageWidth;

	if (Marg->isChecked())
	{
		NewPageWidth=LocPageWidth-LocLeft-LocRight;
		offset=LocLeft;
	}

	double spacing = NewPageWidth/n;
	int selVer = 0;
	QValueList<double>::Iterator it;

	for (int i=1;i<n;i++)
	{
		int n = static_cast<int>(LocVer.count());
		int m = n;
		double curVer = offset + spacing * i;

		if (LocVer.contains(curVer))
			continue;

		for (int i = n - 1; i > 0; i--)
			if (curVer < LocVer[i - 1])
				m = i;

		selVer = m - 1;

		if (m == n)
		{
			LocVer.append(curVer);
			selVer = m - 1;
		}
		else
		{
			it = LocVer.at(selVer);
			LocVer.insert(it,curVer);
		}
	}
	UpdateVerList();
}


void GuideManager::AddVerVal()
{
	if (HorSpin->hasFocus())
		HorSpin->clearFocus();
	if (VerSpin->hasFocus())
		VerSpin->clearFocus();
	LocVer.prepend(0);
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
	LocLocked = Lock->isChecked();
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
	HorSpin->setValue(LocHor[selHor] * docUnitRatio);
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
	VerSpin->setValue(LocVer[selVer] * docUnitRatio);
	connect(VerSpin, SIGNAL(valueChanged(int)), this, SLOT(ChangeVerVal()));
}

void GuideManager::ChangeHorVal()
{
	int n = static_cast<int>(LocHor.count());
	int m = n;
	double curHor = HorSpin->value() / docUnitRatio;

	QValueList<double>::Iterator it = LocHor.at(selHor);
	LocHor.remove(it);

	for (int i = n - 1; i > 0; i--)
		if (curHor < LocHor[i - 1])
			m = i;

	selHor = m - 1;

	if (m == n)
	{
		LocHor.append(curHor);
		selHor = m - 1;
	}
	else
	{
		it = LocHor.at(selHor);
		LocHor.insert(it,curHor);
	}
	UpdateHorList();
}

void GuideManager::ChangeVerVal()
{
	int n = static_cast<int>(LocVer.count());
	int m = n;
	double curVer = VerSpin->value() / docUnitRatio;

	QValueList<double>::Iterator it = LocVer.at(selVer);
	LocVer.remove(it);

	for (int i = n - 1; i > 0; i--)
		if (curVer < LocVer[i - 1])
			m = i;

	selVer = m - 1;

	if (m == n)
	{
		LocVer.append(curVer);
		selVer = m - 1;
	}
	else
	{
		it = LocVer.at(selVer);
		LocVer.insert(it,curVer);
	}
	UpdateVerList();
}

void GuideManager::UnitChange()
{
	QString tmp = unitGetSuffixFromIndex(docUnitIndex);
	HorSpin->setSuffix(tmp);
	VerSpin->setSuffix(tmp);
	int decimals = unitGetDecimalsFromIndex(docUnitIndex);
	HorSpin->setDecimals(decimals);
	VerSpin->setDecimals(decimals);
}

void GuideManager::UpdateHorList()
{
	disconnect(HorList, SIGNAL(highlighted(QListBoxItem*)), this, SLOT(selHorIte(QListBoxItem*)));
	disconnect(HorSpin, SIGNAL(valueChanged(int)), this, SLOT(ChangeHorVal()));

	QString tmp;
	int precision=unitGetPrecisionFromIndex(docUnitIndex);
	QString suffix=unitGetSuffixFromIndex(docUnitIndex);

	HorList->clear();

	for (uint i = 0; i < LocHor.count(); ++ i)
		HorList->insertItem(tmp.setNum(qRound(LocHor[i] * docUnitRatio * 10000.0) / 10000.0, 'f', precision) + suffix);
	if (LocHor.isEmpty())
		selHor = -1;

	if (selHor != -1)
		HorList->setCurrentItem(selHor);
	HorSpin->setEnabled(selHor != -1 ? true : false);
	HorSpin->setValue(selHor != -1 ? (LocHor[selHor] * docUnitRatio * 10000.0) / 10000.0 : 0);

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

	for (uint i = 0; i < LocVer.count(); ++ i)
		VerList->insertItem(tmp.setNum(qRound(LocVer[i] * docUnitRatio * 10000.0) / 10000.0, 'f', precision) + suffix);
	if (LocVer.isEmpty())
		selVer = -1;

	if (selVer != -1)
		VerList->setCurrentItem(selVer);

	VerSpin->setEnabled(selVer != -1 ? true : false);
	VerSpin->setValue(selVer != -1 ? (LocVer[selVer] * docUnitRatio * 10000.0) / 10000.0 : 0);

	connect(VerList, SIGNAL(highlighted(QListBoxItem*)), this, SLOT(selVerIte(QListBoxItem*)));
	connect(VerSpin, SIGNAL(valueChanged(int)), this, SLOT(ChangeVerVal()));
}
