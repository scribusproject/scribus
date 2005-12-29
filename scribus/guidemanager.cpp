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
#include "selection.h"
#include "guidemanager.h"
#include "guidemanager.moc"
#include "page.h"
#include "units.h"
#include "commonstrings.h"
#include "pagestructs.h"

#include <qradiobutton.h>
#include <qlayout.h>
#include <qcursor.h>
#include <qlistview.h>
#include <qdialog.h>
#include <qvaluelist.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qlistview.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qtooltip.h>
#include <qhbuttongroup.h>


extern QPixmap loadIcon(QString nam);


GuideManager::GuideManager(QWidget* parent) : QDialog(parent, "GuideManager", true, 0)
{
	setCaption( tr("Manage Guides"));
	setIcon(loadIcon("AppIcon.png"));

	/* Initialise the global variables */
	resetMarginsForPage();
	docUnitIndex = ScMW->doc->unitIndex();
	docUnitPrecision = unitGetPrecisionFromIndex(docUnitIndex);
	docUnitRatio = unitGetRatioFromIndex(docUnitIndex);
	docUnitDecimals = unitGetDecimalsFromIndex(docUnitIndex);
	suffix = unitGetSuffixFromIndex(docUnitIndex);

	/* Create the dialog elements */
	QVBoxLayout *guideManagerLayout = new QVBoxLayout(this, 11, 6, "guideManagerLayout");

	QHBoxLayout *Layout6 = new QHBoxLayout(0, 0, 6, "Layout6");

	QGroupBox *horGroup = new QGroupBox(this, "horGroup");
	horGroup->setTitle( tr("Horizontal Guides"));
	horGroup->setColumnLayout(0, Qt::Vertical);
	horGroup->layout()->setSpacing(6);
	horGroup->layout()->setMargin(11);
	QVBoxLayout *horGroupLayout = new QVBoxLayout(horGroup->layout());
	horGroupLayout->setAlignment(Qt::AlignTop);

	horList = new QListView(horGroup, "horList");
	horList->addColumn(tr("Guide"));
	horList->addColumn(tr("Unit"));
	horList->setMinimumSize(QSize(0, 150));
	horList->setSelectionMode(QListView::Extended);
	horList->setAllColumnsShowFocus(true);
	horList->setSorting(0);
	horGroupLayout->addWidget(horList);

	QHBoxLayout *Layout4 = new QHBoxLayout(0, 0, 6, "Layout4");

	TextLabel2 = new QLabel( tr("&Y-Pos:"), horGroup, "TextLabel2");
	Layout4->addWidget(TextLabel2);

	horSpin = new MSpinBox(0, locPageHeight, horGroup, 4);
	TextLabel2->setBuddy(horSpin);
	Layout4->addWidget(horSpin);

	horGroupLayout->addLayout(Layout4);

	QHBoxLayout *Layout3 = new QHBoxLayout(0, 0, 6, "Layout3");

	horSet = new QPushButton( tr( "&Add" ), horGroup, "horSet");
	horSet->setAutoDefault(false);
	Layout3->addWidget(horSet);

	horDel = new QPushButton( tr( "D&elete" ), horGroup, "horDel");
	horDel->setAutoDefault(false);
	Layout3->addWidget(horDel);

	horGroupLayout->addLayout(Layout3);

	Layout6->addWidget(horGroup);

	verGroup = new QGroupBox(this, "verGroup");
	verGroup->setTitle( tr("Vertical Guides"));
	verGroup->setColumnLayout(0, Qt::Vertical);
	verGroup->layout()->setSpacing(6);
	verGroup->layout()->setMargin(11);
	QVBoxLayout *verGroupLayout = new QVBoxLayout(verGroup->layout());
	verGroupLayout->setAlignment(Qt::AlignTop);

	verList = new QListView(verGroup, "verList");
	verList->addColumn(tr("Guide"));
	verList->addColumn(tr("Unit"));
	verList->setMinimumSize(QSize(0, 150));
	verList->setSorting(0);
	verList->setSelectionMode(QListView::Extended);
	verList->setAllColumnsShowFocus(true);
	verGroupLayout->addWidget(verList);

	QHBoxLayout *Layout2 = new QHBoxLayout(0, 0, 6, "Layout2");

	TextLabel1 = new QLabel( tr("&X-Pos:"), verGroup, "TextLabel1");
	Layout2->addWidget(TextLabel1);

	verSpin = new MSpinBox(0, locPageWidth, verGroup, 4);
	TextLabel1->setBuddy(verSpin);
	Layout2->addWidget(verSpin);

	verGroupLayout->addLayout(Layout2);

	QHBoxLayout *Layout1 = new QHBoxLayout(0, 0, 6, "Layout1");

	verSet = new QPushButton( tr( "A&dd" ), verGroup, "verSet");
	verSet->setAutoDefault(false);
	Layout1->addWidget(verSet);

	verDel = new QPushButton( tr( "De&lete" ), verGroup, "verDel");
	verDel->setAutoDefault(false);
	Layout1->addWidget(verDel);

	verGroupLayout->addLayout(Layout1);

	Layout6->addWidget(verGroup);

	guideManagerLayout->addLayout(Layout6);

	QHBoxLayout *Layout7 = new QHBoxLayout(0, 0, 6, "Layout7");

	// auto guides
	QGroupBox *horGroup2 = new QGroupBox(this, "horGroup");
	horGroup2->setTitle( tr("Rows and Columns - Automatic Guides"));
	horGroup2->setColumnLayout(0, Qt::Vertical);
	horGroup2->layout()->setSpacing(6);
	horGroup2->layout()->setMargin(11);
	QVBoxLayout *horGroupLayout2 = new QVBoxLayout(horGroup2->layout());
	horGroupLayout2->setAlignment(Qt::AlignTop);

	// auto guides basic
	QLabel *TextLabel8 = new QLabel( tr("&Rows:"), horGroup2, "TextLabel8");
	rowSpin = new QSpinBox(1, 100, 1, horGroup2, "rowSpin");
	TextLabel8->setBuddy(rowSpin);

	QLabel *TextLabel9 = new QLabel( tr("C&olumns:"), horGroup2, "TextLabel9");
	colSpin = new QSpinBox(1, 100, 1, horGroup2, "colSpin");
	TextLabel9->setBuddy(colSpin);

	QVBoxLayout *rcLayout = new QVBoxLayout(0, 0, 6, "rcLayout");
	rcLayout->addWidget(TextLabel8);
	rcLayout->addWidget(rowSpin);
	rcLayout->addWidget(TextLabel9);
	rcLayout->addWidget(colSpin);

	// auto guides gaps
	useRowGap = new QCheckBox( tr("Row &Gap"), horGroup2, "useRowGap");
	useRowGap->setChecked(false);
	rowGap = new MSpinBox(0, 100, horGroup2, 4);
	rowGap->setValue(0);
	rowGap->setEnabled(false);

	useColGap = new QCheckBox( tr("Colum&n Gap"), horGroup2, "useColGap");
	useColGap->setChecked(false);
	colGap = new MSpinBox(0, 100, horGroup2, 4);
	colGap->setValue(0);
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
	bGroup = new QHButtonGroup(horGroup2, "bGroup");
	bGroup->setFrameStyle(QFrame::NoFrame);
	QLabel *TextLabel10 = new QLabel( tr("Refer to:"), horGroup2, "TextLabel10");
	Layout10->addWidget(TextLabel10);
	QRadioButton *fPage = new QRadioButton( tr( "&Page" ), bGroup, "fpage");
	fPage->setChecked(true);
	(void) new QRadioButton( tr( "&Margins" ), bGroup, "fmargin");
	QRadioButton *fSelect = new QRadioButton( tr( "&Selection" ), bGroup, "fselect");
	fSelect->setEnabled(selected);
	Layout10->addWidget(bGroup);

	QSpacerItem* spacer2 = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
	Layout10->addItem(spacer2);

	Layout7->addWidget(horGroup2);
	horGroupLayout2->addLayout(autoGuidesLayout);
	horGroupLayout2->addLayout(Layout10);

	guideManagerLayout->addLayout(Layout7);

	QHBoxLayout *Layout5 = new QHBoxLayout(0, 0, 6, "Layout5");
	lockedCheckBox = new QCheckBox( tr( "&Lock Guides" ), this, "lockedCheckBox");
	lockedCheckBox->setChecked(ScMW->doc->GuideLock);
	Layout5->addWidget(lockedCheckBox);

	allPages = new QCheckBox(tr("&Apply to All Pages"), this, "allPages");
	allPages->setChecked(false);
	Layout5->addWidget(allPages);

	QHBoxLayout *buttonLayout = new QHBoxLayout(0, 0, 6, "buttonLayout");
	QSpacerItem* spacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
	buttonLayout->addItem(spacer);

	okButton = new QPushButton( CommonStrings::tr_OK, this, "okButton");
	okButton->setAutoDefault(false);
	cancelButton = new QPushButton( tr( "&Close" ), this, "cancelButton");
	cancelButton->setAutoDefault(false);
	cancelButton->setAccel(QKeySequence("Esc"));
	setButton = new QPushButton( tr("&Update"), this, "setButton");
	setButton->setAutoDefault(false);
	buttonLayout->addWidget(setButton);
	buttonLayout->addWidget(okButton);
	buttonLayout->addWidget(cancelButton);

	guideManagerLayout->addLayout(Layout5);
	guideManagerLayout->addLayout(buttonLayout);

	// set current guides
	setGuidesFromList(horList, ScMW->doc->currentPage->YGuides);
	setGuidesFromList(verList, ScMW->doc->currentPage->XGuides);
	unitChange();

	//tooltips
	QToolTip::add( setButton, "<qt>" + tr("Set the guides in document. Guide manager is still opened but the changes are persistant", "guide manager") + "</qt>");

	// Create signals and slots connections
	connect(okButton, SIGNAL(clicked()), this, SLOT(commitChanges()));
	connect(setButton, SIGNAL(clicked()), this, SLOT(commitEditChanges()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	connect(horSet, SIGNAL(clicked()), this, SLOT(AddHorVal()));
	connect(horDel, SIGNAL(clicked()), this, SLOT(DelHorVal()));
	connect(horSpin, SIGNAL(valueChanged(int)), this, SLOT(ChangeHorVal()));
	connect(horList, SIGNAL(currentChanged(QListViewItem*)), this, SLOT(horList_currentChanged(QListViewItem*)));
	connect(verSet, SIGNAL(clicked()), this, SLOT(AddVerVal()));
	connect(verDel, SIGNAL(clicked()), this, SLOT(DelVerVal()));
	connect(verSpin, SIGNAL(valueChanged(int)), this, SLOT(ChangeVerVal()));
	connect(verList, SIGNAL(currentChanged(QListViewItem*)), this, SLOT(verList_currentChanged(QListViewItem*)));
	connect(useRowGap, SIGNAL(toggled(bool)), this, SLOT(useRowGap_clicked(bool)));
	connect(useColGap, SIGNAL(toggled(bool)), this, SLOT(useColGap_clicked(bool)));
}

void GuideManager::DelHorVal()
{
	/* previous item pointer to ensure that ++it
	runs before item goes deleted */
	QListViewItem *itemToDelete;
	QListViewItemIterator it(horList, QListViewItemIterator::Selected);
	while (it.current())
	{
		itemToDelete = it.current();
		++it;
		if (itemToDelete)
		{
			horList->takeItem(itemToDelete);
			delete itemToDelete;
		}
	}
}

void GuideManager::DelVerVal()
{
	QListViewItem *itemToDelete;
	QListViewItemIterator it(verList, QListViewItemIterator::Selected);
	while (it.current())
	{
		itemToDelete = it.current();
		++it;
		if (itemToDelete)
		{
			verList->takeItem(itemToDelete);
			delete itemToDelete;
		}
	}
}

void GuideManager::AddHorVal()
{
	QString tmp;
	tmp = tmp.setNum(0.0, 'f', docUnitPrecision);
	QListViewItem *item = new QListViewItem(horList, tmp, suffix);
	horList->insertItem(item);
	horList->setCurrentItem(item);
	horList->clearSelection();
	horList->setSelected(item, true);
	horSpin->setFocus();
	horSpin->selectAll();
}

void GuideManager::AddVerVal()
{
	QString tmp;
	tmp = tmp.setNum(0.0, 'f', docUnitPrecision);
	QListViewItem *item = new QListViewItem(verList, tmp, suffix);
	verList->insertItem(item);
	verList->clearSelection();
	verList->setCurrentItem(item);
	verList->setSelected(item, true);
	verSpin->setFocus();
	verSpin->selectAll();
}

void GuideManager::resetMarginsForPage()
{
	locPageWidth = ScMW->doc->currentPage->width();
	locPageHeight = ScMW->doc->currentPage->height();
	// whatif selection settings
	FPoint selectionTopLeft = FPoint(0, 0);
	FPoint selectionBottomRight = FPoint(0, 0);

	int docSelectionCount = ScMW->doc->selection->count();
	// multiselection
	if (docSelectionCount > 1)
	{
		selectionTopLeft.setXY(ScMW->view->GroupX - ScMW->doc->currentPage->xOffset(),
							   ScMW->view->GroupY - ScMW->doc->currentPage->yOffset());
		selectionBottomRight.setXY(ScMW->view->GroupW,
								   ScMW->view->GroupH);
	}
	// only one item selected
	else if (docSelectionCount == 1)
	{
		PageItem *currItem = ScMW->doc->selection->itemAt(0);
		selectionTopLeft.setXY(currItem->xPos() - ScMW->doc->currentPage->xOffset(),
							   currItem->yPos() - ScMW->doc->currentPage->yOffset());
		selectionBottomRight.setXY(currItem->width(), currItem->height());
	}

	selected = true;
	if (selectionBottomRight != FPoint(0, 0))
	{
		gy = selectionTopLeft.y();
		gx = selectionTopLeft.x();
		gw = selectionBottomRight.x();
		gh = selectionBottomRight.y();
	}
	else selected = false;
	
	locTop = ScMW->doc->pageMargins.Top;
	locBottom = ScMW->doc->pageMargins.Bottom;

	PageLocation pageLocation = ScMW->doc->locationOfPage(ScMW->doc->currentPageNumber());
	switch (pageLocation)
	{
		case MiddlePage :
			locLeft = ScMW->doc->pageMargins.Left;
			locRight = ScMW->doc->pageMargins.Left;
			break;
		case LeftPage:
			locLeft = ScMW->doc->pageMargins.Right;
			locRight = ScMW->doc->pageMargins.Left;
			break;
		case RightPage:
			locRight = ScMW->doc->pageMargins.Right;
			locLeft = ScMW->doc->pageMargins.Left;
			break;
		default:
			locRight = 0;
			locLeft = 0;
	}
}

void GuideManager::addRows()
{
	resetMarginsForPage();
	horList->clear();
	int n = QString(rowSpin->text()).toInt();
	double offset = 0;
	double newPageHeight = locPageHeight;

	if (bGroup->selectedId() == 1)
	{
		newPageHeight = locPageHeight - locTop - locBottom;
		offset = locTop;
	}
	else if (bGroup->selectedId() == 2)
	{
		offset = gy;
		newPageHeight = gh;
	}

	double spacing = newPageHeight / n;
	QValueList<double> values;
	for (int i = 1; i < n; ++i)
	{
		if (useRowGap->isChecked())
		{
			double gapValue = value2pts(rowGap->value(), ScMW->doc->unitIndex());
			values.append(offset + (spacing * i) + (gapValue / 2.0));
			values.append(offset + (spacing * i) - (gapValue / 2.0));
		}
		else
			values.append(offset + (spacing * i));
	}
	setGuidesFromList(horList, values);
}

void GuideManager::addCols()
{
	resetMarginsForPage();
	verList->clear();
	int n = QString(colSpin->text()).toInt();
	double offset = 0;
	double newPageWidth = locPageWidth;

	if (bGroup->selectedId() == 1)
	{
		newPageWidth = locPageWidth - locLeft - locRight;
		offset = locLeft;
	}
	else if (bGroup->selectedId() == 2)
	{
		offset = gx;
		newPageWidth = gw;
	}

	double spacing = newPageWidth / n;
	QValueList<double> values;
	for (int i = 1; i < n; ++i)
	{
		if (useColGap->isChecked())
		{
			double gapValue = value2pts(colGap->value(), ScMW->doc->unitIndex());
			values.append(offset + spacing * i + (gapValue / 2.0));
			values.append(offset + spacing * i - (gapValue / 2.0));
		}
		else
			values.append(offset + spacing * i);
	}
	setGuidesFromList(verList, values);
}

void GuideManager::ChangeHorVal()
{
	QListViewItem *item = horList->currentItem();
	if (item == 0)
		return;
	QString tmp;
	tmp = tmp.setNum(horSpin->value(), 'f', docUnitPrecision);
	item->setText(0, tmp);
}

void GuideManager::ChangeVerVal()
{
	QListViewItem *item = verList->currentItem();
	if (item == 0)
		return;
	QString tmp;
	tmp = tmp.setNum(verSpin->value(), 'f', docUnitPrecision);
	item->setText(0, tmp);
}

void GuideManager::unitChange()
{
	horSpin->setSuffix(suffix);
	verSpin->setSuffix(suffix);
	rowGap->setSuffix(suffix);
	colGap->setSuffix(suffix);
	horSpin->setDecimals(docUnitDecimals);
	verSpin->setDecimals(docUnitDecimals);
	rowGap->setDecimals(docUnitDecimals);
	colGap->setDecimals(docUnitDecimals);
}

void GuideManager::refreshDoc()
{
	if (rowSpin->value() > 1)
		addRows();
	if (colSpin->value() > 1)
		addCols();
	QValueList<double> tmp = getValuesFromList(verList);
	ScMW->doc->currentPage->addXGuides(tmp);
	tmp = getValuesFromList(horList);
	ScMW->doc->currentPage->addYGuides(tmp);
	ScMW->doc->lockGuides(lockedCheckBox->isChecked());
	ScMW->view->DrawNew();
}

void GuideManager::refreshWholeDoc()
{
	int origPage = ScMW->doc->currentPage->pageNr();

	for (uint i = 0; i < ScMW->doc->Pages->count(); ++i)
	{
		ScMW->view->GotoPage(i);
		refreshDoc();
	}
	ScMW->view->GotoPage(origPage);
	ScMW->view->DrawNew();
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
	if (allPages->isChecked())
		refreshWholeDoc();
	else
		refreshDoc();
	QApplication::restoreOverrideCursor();
}

void GuideManager::setGuidesFromList(QListView *w, QValueList<double> guides)
{
	QValueList<double>::iterator it;
	QString tmp;

	for (it = guides.begin(); it != guides.end(); ++it)
	{
		tmp = tmp.setNum((*it) * docUnitRatio , 'f', docUnitPrecision);
		QListViewItem *item = new QListViewItem(w, tmp, suffix);
		w->insertItem(item);
	}
	w->setCurrentItem(w->firstChild());
	w->setSelected(w->firstChild(), true);
}

QValueList<double> GuideManager::getValuesFromList(QListView *w)
{
	QValueList<double> ret;
	QListViewItemIterator it(w);

	while (it.current())
	{
		ret.append(it.current()->text(0).toDouble() / docUnitRatio);
		++it;
	}
	return ret;
}

void GuideManager::verList_currentChanged(QListViewItem *item)
{
	double val;
	if (!item)
		val = 0.0;
	else
		val = item->text(0).toDouble();
	verSpin->setValue(val);
}

void GuideManager::horList_currentChanged(QListViewItem *item)
{
	double val;
	if (!item)
		val = 0.0;
	else
		val = item->text(0).toDouble();
	horSpin->setValue(val);
}
