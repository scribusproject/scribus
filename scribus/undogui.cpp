/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
 *   Copyright (C) 2005 by Riku Leino                                      *
 *   tsoots@gmail.com                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <QLayout>
#include <QPixmap>
#include <QPushButton>
#include <QRect>
#include <QStyle>
#include <QToolTip>
#include <QCheckBox>
#include <QFont>
#include <QFontMetrics>
#include <QHBoxLayout>
#include <QVBoxLayout>
 
#include "undogui.h"
#include "prefsmanager.h"
#include "prefsfile.h"
#include "prefscontext.h"
#include "scribuscore.h"
#include "menumanager.h"
#include "scraction.h"
#include "util_icon.h"

UndoGui::UndoGui(QWidget* parent, const char* name, Qt::WFlags f) : ScrPaletteBase(parent, name, f)
{

}

/*** UndoWidget ***************************************************************/

UndoWidget::UndoWidget(QWidget* parent, const char* name)
: UndoGui(parent, name)
{
	/* BnF standard toolbar buttons
	QHBoxLayout* layout = new QHBoxLayout(this, 0, 0, "layout");


	undoButton = new QToolButton(this, "undoButton");
	undoButton->setIconSet(loadIcon("u_undo.png"));
	undoButton->setUsesTextLabel(false);
	QToolTip::add(undoButton, tr("Undo"));
	layout->addWidget(undoButton);
	undoMenu = new QPopupMenu(undoButton, "undoMenu");
	undoButton->setPopup(undoMenu);
	undoButton->setPopupDelay(0);
	undoButton->setAutoRaise(true);

	redoButton = new QToolButton(this, "redoButton");
	redoButton->setIconSet(loadIcon("u_redo.png"));
	redoButton->setUsesTextLabel(false);
	QToolTip::add(redoButton, tr("Redo"));
	layout->addWidget(redoButton);
	redoMenu = new QPopupMenu(redoButton, "redoMenu");
	redoButton->setPopup(redoMenu);
	redoButton->setPopupDelay(0);
	redoButton->setAutoRaise(true);
	*/
	//Scribus action based toolbar button construction
	parent->addAction(ScCore->primaryMainWindow()->scrActions["editUndoAction"]);
	parent->addAction(ScCore->primaryMainWindow()->scrActions["editRedoAction"]);
	ScCore->primaryMainWindow()->scrMenuMgr->createMenu("undoButtonMenu", "undoButtonMenu");
	ScCore->primaryMainWindow()->scrMenuMgr->createMenu("redoButtonMenu", "redoButtonMenu");
	undoMenu=ScCore->primaryMainWindow()->scrMenuMgr->getLocalPopupMenu("undoButtonMenu");
	redoMenu=ScCore->primaryMainWindow()->scrMenuMgr->getLocalPopupMenu("redoButtonMenu");
	ScCore->primaryMainWindow()->scrMenuMgr->addMenuToWidgetOfAction("undoButtonMenu", ScCore->primaryMainWindow()->scrActions["editUndoAction"]);
	ScCore->primaryMainWindow()->scrMenuMgr->addMenuToWidgetOfAction("redoButton/*Menu*/", ScCore->primaryMainWindow()->scrActions["editRedoAction"]);
	
	parent->addAction(ScCore->primaryMainWindow()->scrActions["editCut"]);
	parent->addAction(ScCore->primaryMainWindow()->scrActions["editCopy"]);
	parent->addAction(ScCore->primaryMainWindow()->scrActions["editPaste"]);
	ScCore->primaryMainWindow()->scrMenuMgr->addMenuToWidgetOfAction("EditPasteRecent", ScCore->primaryMainWindow()->scrActions["editPaste"]);
	
	/* BnF Undo buttons
	connect(undoButton, SIGNAL(clicked()), this, SLOT(undoClicked()));
	connect(redoButton, SIGNAL(clicked()), this, SLOT(redoClicked()));
	*/
	connect(undoMenu, SIGNAL(activated(int)), this, SLOT(undoMenuClicked(int)));
	connect(redoMenu, SIGNAL(activated(int)), this, SLOT(redoMenuClicked(int)));
}

void UndoWidget::clear()
{
	undoMenu->clear();
	undoItems.clear();
	//Scribus disable
	ScCore->primaryMainWindow()->scrActions["editUndoAction"]->setEnabled(false);
	// BnF disable
	//undoButton->setEnabled(false);
	redoMenu->clear();
	redoItems.clear();
	//Scribus disable;
	ScCore->primaryMainWindow()->scrActions["editRedoAction"]->setEnabled(false);
	// BnF disable
	//redoButton->setEnabled(false);
}

void UndoWidget::undoClicked()
{
	if (undoItems.size() > 0)
		emit undo(1);
}

void UndoWidget::redoClicked()
{
	if (redoItems.size() > 0)
		emit redo(1);
}

void UndoWidget::undoMenuClicked(int id)
{
	int steps = undoMenu->indexOf(id) + 1;
	emit undo(steps);
}

void UndoWidget::redoMenuClicked(int id)
{
	int steps = redoMenu->indexOf(id) + 1;
	emit redo(steps);
}

void UndoWidget::insertUndoItem(UndoObject* target, UndoState* state)
{
	undoItems.insert(undoItems.begin(), QString( tr("%1: %2", "undo target: action (f.e. Text frame: Resize)"))
                                        .arg(target->getUName()).arg(state->getName()));
	clearRedo();
	updateUndoMenu();
	updateRedoMenu();
}

void UndoWidget::insertRedoItem(UndoObject* target, UndoState* state)
{
	redoItems.push_back(QString( tr("%1: %2", "undo target: action (f.e. Text frame: Resize)"))
                        .arg(target->getUName()).arg(state->getName()));
	updateRedoMenu();
	updateUndoMenu();
}

void UndoWidget::clearRedo()
{
	redoItems.erase(redoItems.begin(), redoItems.end());
	updateRedoMenu();
}

void UndoWidget::updateUndoMenu()
{
	undoMenu->clear();
	for (uint i = 0; i < MENU_HEIGHT && i < undoItems.size(); ++i)
		undoMenu->insertItem(undoItems[i]);
	//BnF
	//undoButton->setEnabled(undoMenu->count() != 0);
	//SCribus
	//ScMW->scrActions["editUndoAction"]->setEnabled(undoMenu->count() != 0);
	updateUndoActions();
}

void UndoWidget::updateRedoMenu()
{
	redoMenu->clear();
	for (uint i = 0; i < MENU_HEIGHT && i < redoItems.size(); ++i)
		redoMenu->insertItem(redoItems[i]);
	//BnF
	//redoButton->setEnabled(redoMenu->count() != 0);
	//Scribus
	//ScMW->scrActions["editRedoAction"]->setEnabled(redoMenu->count() != 0);
	updateUndoActions();
}

void UndoWidget::updateUndoActions()
{
	ScCore->primaryMainWindow()->scrActions["editUndoAction"]->setEnabled(undoMenu->count() != 0);
	ScCore->primaryMainWindow()->scrActions["editRedoAction"]->setEnabled(redoMenu->count() != 0);
}

void UndoWidget::updateUndo(int steps)
{
	for (int i = 0; i < steps; ++i)
	{
		redoItems.insert(redoItems.begin(), undoItems[0]);
		undoItems.erase(undoItems.begin());	
	}
	updateUndoMenu();
	updateRedoMenu();
}

void UndoWidget::updateRedo(int steps)
{
	for (int i = 0; i < steps; ++i)
	{
		undoItems.insert(undoItems.begin(), redoItems[0]);
		redoItems.erase(redoItems.begin());	
	}
	updateUndoMenu();
	updateRedoMenu();
}

void UndoWidget::popBack()
{
	if (undoItems.size() > 0)
	{
		undoItems.erase(undoItems.end() - 1);
		updateUndoMenu();
	}
}

UndoWidget::~UndoWidget()
{
	
}

/*** UndoPalette **************************************************************/

UndoPalette::UndoPalette(QWidget* parent, const char* name)
: UndoGui(parent, name)
{
	currentSelection = 0;
	redoItems = 0;
	QVBoxLayout* layout = new QVBoxLayout(this, 5, 5, "layout");

	objectBox = new QCheckBox(this, "objectBox");
	layout->addWidget(objectBox);
// 	objectBox->setEnabled(false);

	undoList = new Q3ListBox(this, "undoList");
	undoList->setMultiSelection(false);
	undoList->setSelectionMode(Q3ListBox::Single);
	layout->addWidget(undoList);
	
	QHBoxLayout* buttonLayout = new QHBoxLayout(0, 0, 5, "buttonLayout"); 
	undoButton = new QPushButton(loadIcon("16/edit-undo.png"), "", this, "undoButton");
	buttonLayout->addWidget(undoButton);
	redoButton = new QPushButton(loadIcon("16/edit-redo.png"), "", this, "redoButton");
	buttonLayout->addWidget(redoButton);
	//Save the translated key sequence - hopefully we get the translated one here!
	initialUndoKS = undoButton->accel();
	initialRedoKS = redoButton->accel();
	layout->addLayout(buttonLayout);

	updateFromPrefs();
	languageChange();
	connect(PrefsManager::instance(), SIGNAL(prefsChanged()), this, SLOT(updateFromPrefs()));
	connect(undoButton, SIGNAL(clicked()), this, SLOT(undoClicked()));
	connect(redoButton, SIGNAL(clicked()), this, SLOT(redoClicked()));
	connect(undoList, SIGNAL(highlighted(int)), this, SLOT(undoListClicked(int)));
	connect(undoList, SIGNAL(onItem(Q3ListBoxItem*)), this, SLOT(showToolTip(Q3ListBoxItem*)));
	connect(undoList, SIGNAL(onViewport()), this, SLOT(removeToolTip()));
	connect(objectBox, SIGNAL(toggled(bool)), this, SLOT(objectCheckBoxClicked(bool)));
	connect(ScCore->primaryMainWindow()->scrActions["editActionMode"], SIGNAL(toggled(bool)),
	        objectBox, SLOT(setChecked(bool)));
	connect(objectBox, SIGNAL(toggled(bool)), 
			ScCore->primaryMainWindow()->scrActions["editActionMode"], SLOT(setOn(bool)));
}

void UndoPalette::clear()
{
	undoList->clear();
	undoList->insertItem( tr("Initial State"));
	undoButton->setEnabled(false);
	redoButton->setEnabled(false);
}

void UndoPalette::updateFromPrefs()
{
	undoButton->setAccel(ScCore->primaryMainWindow()->scrActions["editUndoAction"]->accel());
	redoButton->setAccel(ScCore->primaryMainWindow()->scrActions["editRedoAction"]->accel());
}

void UndoPalette::languageChange()
{
	setCaption( tr("Action History"));	
	objectBox->setText( tr("Show selected object only"));
	undoButton->setText( tr("&Undo"));
	redoButton->setText( tr("&Redo"));
}

void UndoPalette::insertUndoItem(UndoObject* target, UndoState* state)
{
	clearRedo();
	undoList->insertItem(new UndoItem(target->getUName(), state->getName(),
                         state->getDescription(), target->getUPixmap(),
                         state->getPixmap(), true));
	currentSelection = undoList->numRows() - 1;
	updateList();
}

void UndoPalette::insertRedoItem(UndoObject* target, UndoState* state)
{
	if (undoList->count() == 1)
	{
		undoList->setSelected(0, true);
		currentSelection = 0;
	}
	undoList->insertItem(new UndoItem(target->getUName(), state->getName(),
                         state->getDescription(), target->getUPixmap(),
                         state->getPixmap(), false));
	updateList();
}

void UndoPalette::updateUndo(int steps)
{
	if (undoList->currentItem() == currentSelection)
	{
		currentSelection -= steps;
		updateList();
	}
}

void UndoPalette::updateRedo(int steps)
{
	if (undoList->currentItem() == currentSelection)
	{
		currentSelection += steps;
		updateList();
	}
}

void UndoPalette::popBack()
{
	if (undoList->count() > 1)
	{
		undoList->removeItem(0);
		currentSelection = undoList->numRows() - 1;
	}
}

void UndoPalette::updateList()
{
	undoList->setCurrentItem(currentSelection);
	undoList->setSelected(currentSelection, true);
	redoButton->setEnabled(currentSelection < undoList->numRows() - 1);
	undoButton->setEnabled(currentSelection > 0);
	if (!undoList->itemVisible(currentSelection))
		undoList->setBottomItem(currentSelection);
	for (int i = 0; i < undoList->numRows(); ++i)
	{
		UndoItem *item = dynamic_cast<UndoItem*>(undoList->item(i));
		if (!item)
			continue;

		item->setUndoAction(currentSelection >= i);
	}
}

void UndoPalette::updateUndoActions()
{
	//ScMW->scrActions["editUndoAction"]->setEnabled(currentSelection > 0);
	//ScMW->scrActions["editRedoAction"]->setEnabled(currentSelection < undoList->numRows() - 1);
}

void UndoPalette::clearRedo()
{
	for (int i = (undoList->numRows() - 1); i > currentSelection; --i)
		undoList->removeItem(i);
}

void UndoPalette::undoClicked()
{
	emit undo(1);
}

void UndoPalette::redoClicked()
{
	emit redo(1);
}

void UndoPalette::undoListClicked(int i)
{
	if (i == currentSelection || (i == 0 && undoList->count() == 1))
		return;
	if (i > currentSelection)
		emit redo(i - currentSelection);
	else if (i < currentSelection)
		emit undo(currentSelection - i);
	currentSelection = i;
	updateList();
}

void UndoPalette::objectCheckBoxClicked(bool on)
{
	emit objectMode(on);
}

void UndoPalette::showToolTip(Q3ListBoxItem *i)
{
	UndoItem *item = dynamic_cast<UndoItem*>(i);
	if (item)
	{
		QString tip = item->getDescription();
		if (tip.isNull())
		  QToolTip::add(undoList, tip);
	}
	else
		removeToolTip();
}

void UndoPalette::removeToolTip()
{
	QToolTip::remove(undoList);
}

UndoPalette::~UndoPalette()
{
	
}

/*** UndoPalette::UndoItem ****************************************************/

UndoPalette::UndoItem::UndoItem() : Q3ListBoxItem()
{
	target = "";
	action = "";
	description = "";
	targetpixmap = NULL;
	actionpixmap = NULL;
	isUndoAction_ = true;
}

UndoPalette::UndoItem::UndoItem(const UndoItem &another) : Q3ListBoxItem()
{
	target = another.target;
	action = another.action;
	description = another.description;
	targetpixmap = another.targetpixmap;
	actionpixmap = another.actionpixmap;
	isUndoAction_ = another.isUndoAction_;
}

UndoPalette::UndoItem::UndoItem(const QString &targetName,
                                const QString &actionName,
                                const QString &actionDescription,
                                QPixmap *targetPixmap,
                                QPixmap *actionPixmap,
                                bool isUndoAction)
: Q3ListBoxItem(),
targetpixmap(targetPixmap),
actionpixmap(actionPixmap),
target(targetName),
action(actionName),
description(actionDescription),
isUndoAction_(isUndoAction)
{

}

void UndoPalette::UndoItem::paint(QPainter *painter)
{
	if (targetpixmap)
		painter->drawPixmap(5, 5, *targetpixmap);
	if (actionpixmap)
		painter->drawPixmap(5, 5, *actionpixmap);
	QPen  p = QPen(painter->pen());
	QFont f = QFont(painter->font());
	painter->setFont(f);
	if (isUndoAction_)
	{
		p.setColor(QColor("black"));
		f.setItalic(false);
	}
	else
	{
		p.setColor(QColor("darkGray"));
		f.setItalic(true);
	}
	painter->setPen(p);
	painter->setFont(f);
	painter->drawText(32, QFontMetrics(f).height(), target);
	painter->drawText(32, (2 * QFontMetrics(painter->font()).height()), action);
}

int UndoPalette::UndoItem::height(const Q3ListBox *lb) const
{
	if (lb)
	{
		int fontHeight = 2 * QFontMetrics(lb->font()).lineSpacing() + 2;
		if (actionpixmap)
			return fontHeight > (10 + actionpixmap->height()) ?
                   fontHeight : (10 + actionpixmap->height());
		else if (targetpixmap)
			return fontHeight > (10 + targetpixmap->height()) ?
                   fontHeight : (10 + targetpixmap->height());
		else
			return fontHeight;
	}
	else
		return 0;
}

int UndoPalette::UndoItem::width(const Q3ListBox *lb) const
{
	if (lb)
		return target.length() > action.length() ?
               39 + QFontMetrics(lb->font()).width(target) :
               39 + QFontMetrics(lb->font()).width(action);
	else
		return 0;
}

QString UndoPalette::UndoItem::getDescription()
{
  return description;
}

bool UndoPalette::UndoItem::isUndoAction()
{
	return isUndoAction_;
}

void UndoPalette::UndoItem::setUndoAction(bool isUndo)
{
	isUndoAction_ = isUndo;
}

UndoPalette::UndoItem::~UndoItem()
{

}
