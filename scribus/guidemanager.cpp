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
 * .open questions: should i use GetUnit or directly access the array?
 *  why do the VerXXX variables refer to the horizontal guides? Should
 *  i change this? Should the precision depend on the unit used? Why are
 *  all the methods and properties public? Only LocHor and LocVer should
 *  be public! 
 ***************************************************************************/

#include "guidemanager.h"
#include "guidemanager.moc"

extern QPixmap loadIcon(QString nam);
extern double UmReFaktor;

GuideManager::GuideManager(
        QWidget* parent,
        QValueList<double> XGuides,
        QValueList<double> YGuides,
        double PageW,
        double PageH,
        bool GuideLock,
        int Einh
    ) : QDialog(parent, "GuideManager", true, 0)

{
	QString tmp;
	int decimals;
	setCaption(tr("Manage Guides"));
	setIcon(loadIcon("AppIcon.png"));
	
	/* Initialise the global variables */
	tp[0] = tr(" pt"); 
	tp[1] = tr(" mm"); 
	tp[2] = tr(" in"); 
	tp[3] = tr(" p");
	int   dp[] = {100, 1000, 10000, 100};
	
	Einheit = Einh;
	decimals = dp[Einheit];
	
	LocHor = YGuides; // in page XGuides and YGuides are inverted
	LocVer = XGuides;
	LocPageWidth = PageW;
	LocPageHeight = PageH;
	LocLocked = GuideLock;
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
	HorList->setMinimumSize(QSize(0, 200));
	HorGroupLayout->addWidget(HorList);
	
	Layout4 = new QHBoxLayout(0, 0, 6, "Layout4"); 
	
	TextLabel2 = new QLabel(HorGroup, "TextLabel2");
	TextLabel2->setText( tr("Y-Pos:"));
	Layout4->addWidget(TextLabel2);
	
	HorSpin = new MSpinBox(HorGroup, 4);
	HorSpin->setDecimals(decimals);
	HorSpin->setMaxValue(LocPageHeight);
	HorSpin->setMinValue(0);
	Layout4->addWidget(HorSpin);
	
	HorGroupLayout->addLayout(Layout4);
	
	Layout3 = new QHBoxLayout(0, 0, 6, "Layout3"); 
	
	HorSet = new QPushButton(HorGroup, "HorSet");
	HorSet->setText( tr("Add"));
	HorSet->setAutoDefault(false);
	Layout3->addWidget(HorSet);
	
	HorDel = new QPushButton(HorGroup, "HorDel");
	HorDel->setText(tr("Delete"));
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
	VerList->setMinimumSize(QSize(0, 200));
	VerGroupLayout->addWidget(VerList);
	
	Layout2 = new QHBoxLayout(0, 0, 6, "Layout2"); 
	
	TextLabel1 = new QLabel(VerGroup, "TextLabel1");
	TextLabel1->setText( tr("X-Pos:"));
	Layout2->addWidget(TextLabel1);
	
	VerSpin = new MSpinBox(VerGroup, 4);
	VerSpin->setDecimals(decimals);
	VerSpin->setMaxValue(LocPageWidth);
	VerSpin->setMinValue(0);
	Layout2->addWidget(VerSpin);
	
	VerGroupLayout->addLayout(Layout2);
	
	Layout1 = new QHBoxLayout(0, 0, 6, "Layout1"); 
	
	VerSet = new QPushButton(VerGroup, "VerSet");
	VerSet->setText( tr("Add"));
	VerSet->setAutoDefault(false);
	Layout1->addWidget(VerSet);
	
	VerDel = new QPushButton(VerGroup, "VerDel");
	VerDel->setText( tr("Delete"));
	VerDel->setAutoDefault(false);
	Layout1->addWidget(VerDel);
	
	VerGroupLayout->addLayout(Layout1);
	
	Layout6->addWidget(VerGroup);
	
	GuideManagerLayout->addLayout(Layout6);
	
	Lock = new QCheckBox(this, "Lock");
	Lock->setText( tr("Lock Guides"));
	Lock->setChecked(LocLocked);
	GuideManagerLayout->addWidget(Lock);
	
	Layout5 = new QHBoxLayout(0, 0, 6, "Layout5"); 
	
	QSpacerItem* spacer = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
	Layout5->addItem(spacer);
	
	OK = new QPushButton(this, "OK");
	OK->setText( tr("OK"));
	OK->setDefault(true);
	Layout5->addWidget(OK );
	
	QSpacerItem* spacer_2 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
	Layout5->addItem(spacer_2);
	
	Cancel = new QPushButton(this, "Cancel");
	Cancel->setText( tr("Cancel"));
	Layout5->addWidget(Cancel);
	
	QSpacerItem* spacer_3 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout5->addItem(spacer_3);
	
	GuideManagerLayout->addLayout(Layout5);
	
	/* Initialise the units */
	UnitChange();
	
	// Create signals and slots connections
	connect( OK, SIGNAL(clicked()), this, SLOT(accept() ));
	connect( Cancel, SIGNAL(clicked()), this, SLOT(reject()));
	connect(HorList, SIGNAL(highlighted(QListBoxItem*)), this, SLOT(selHorIte(QListBoxItem*)));
	connect(VerList, SIGNAL(highlighted(QListBoxItem*)), this, SLOT(selVerIte(QListBoxItem*)));
	connect(HorSet, SIGNAL(clicked()), this, SLOT(AddHorVal()));
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
		HorList->setCurrentItem(0);
	else
	{
		HorSpin->setValue(0);
		if (VerList->numRows() > 0)
			VerList->setCurrentItem(0);
		else
			VerSpin->setValue(0);
		HorDel->setEnabled(false);
		VerDel->setEnabled(false);
	} 
}	
void GuideManager::DelHorVal()
{
	QValueList<double>::Iterator it;
	it = LocHor.at(selHor);
	it = LocHor.remove(it);
	selHor = LocHor.isEmpty() ? -1 : static_cast<int>(LocHor.count() - 1);
	if (selHor == -1)
		HorDel->setEnabled(false);
	UpdateHorList();
}
	
void GuideManager::DelVerVal()
{
	QValueList<double>::Iterator it;
	it = LocVer.at(selVer);
	it = LocVer.remove(it);
	selVer = LocVer.isEmpty() ? -1 : static_cast<int>(LocVer.count() - 1);
	if (selVer == -1)
		VerDel->setEnabled(false);
	UpdateVerList();
}
	
void GuideManager::AddHorVal()
{
	LocHor.prepend(0);
	selHor = 0;
	
	VerList->clearSelection();
	VerSpin->setEnabled(false);
	VerDel->setEnabled(false);
	
	HorDel->setEnabled(true);
	UpdateHorList();
	HorSpin->setFocus();
	HorSpin->selectAll();
}
	
void GuideManager::AddVerVal()
{
	LocVer.prepend(0);
	selVer = 0;
	
	HorList->clearSelection();
	HorSpin->setEnabled(false);
	HorDel->setEnabled(false);
	
	VerDel->setEnabled(true);
	UpdateVerList();
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
	HorSpin->setValue(LocHor[selHor] * UmReFaktor);
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
	VerSpin->setValue(LocVer[selVer] * UmReFaktor);
	connect(VerSpin, SIGNAL(valueChanged(int)), this, SLOT(ChangeVerVal()));
}
	
void GuideManager::ChangeHorVal()
{
	int n = static_cast<int>(LocHor.count());
	int m = n;
	double curHor = HorSpin->value() / UmReFaktor;
	
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
	double curVer = VerSpin->value() / UmReFaktor;
	
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
	QString tmp = GetUnit();
	HorSpin->setSuffix(tmp);
	VerSpin->setSuffix(tmp);
}
	
QString GuideManager::GetUnit()
{
	QString tmp = tr(tp[Einheit]);
	return tmp;
}
	
void GuideManager::UpdateHorList()
{
	disconnect(HorList, SIGNAL(highlighted(QListBoxItem*)), this, SLOT(selHorIte(QListBoxItem*)));
	disconnect(HorSpin, SIGNAL(valueChanged(int)), this, SLOT(ChangeHorVal()));
	
	HorList->clear();
	
	QString tmp;
	
	for (uint i = 0; i < LocHor.count(); ++ i)
		HorList->insertItem(tmp.setNum(qRound(LocHor[i] * UmReFaktor * 10000.0) / 10000.0, 'f', 4) +
					 GetUnit());
	if (LocHor.isEmpty())
		selHor = -1;

	if (selHor != -1)
		HorList->setCurrentItem(selHor);
	HorSpin->setEnabled(selHor != -1 ? true : false);
	HorSpin->setValue(selHor != -1 ? (LocHor[selHor] * UmReFaktor * 10000.0) / 10000.0 : 0);

	connect(HorList, SIGNAL(highlighted(QListBoxItem*)), this, SLOT(selHorIte(QListBoxItem*)));
	connect(HorSpin, SIGNAL(valueChanged(int)), this, SLOT(ChangeHorVal()));
}
	
void GuideManager::UpdateVerList()
{
	disconnect(VerList, SIGNAL(highlighted(QListBoxItem*)), this, SLOT(selVerIte(QListBoxItem*)));
	disconnect(VerSpin, SIGNAL(valueChanged(int)), this, SLOT(ChangeVerVal()));
	
	QString tmp;
	
	VerList->clear();
	
	for (uint i = 0; i < LocVer.count(); ++ i)
		VerList->insertItem(tmp.setNum(qRound(LocVer[i] * UmReFaktor * 10000.0) / 10000.0, 'f', 4) +
					 GetUnit());
	if (LocVer.isEmpty())
		selVer = -1;
		
	if (selVer != -1)
		VerList->setCurrentItem(selVer);
		
	VerSpin->setEnabled(selVer != -1 ? true : false);
	VerSpin->setValue(selVer != -1 ? (LocVer[selVer] * UmReFaktor * 10000.0) / 10000.0 : 0);

	connect(VerList, SIGNAL(highlighted(QListBoxItem*)), this, SLOT(selVerIte(QListBoxItem*)));
	connect(VerSpin, SIGNAL(valueChanged(int)), this, SLOT(ChangeVerVal()));
}
