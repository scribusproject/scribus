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

#include "undogui.h"
#include <qlayout.h>
#include <qpixmap.h>
#include <qpushbutton.h>
#include <qtooltip.h>
#include <qcheckbox.h>

extern QPixmap loadIcon(QString nam);

UndoGui::UndoGui(QWidget* parent, const char* name, WFlags f) : QWidget(parent, name, f)
{

}

/*** UndoWidget ***************************************************************/

UndoWidget::UndoWidget(QWidget* parent, const char* name)
: UndoGui(parent, name)
{
	QHBoxLayout* layout = new QHBoxLayout(this, 0, 0, "layout");

	undoButton = new QToolButton(this, "undoButton");
	undoButton->setIconSet(loadIcon("u_undo.png"));
	undoButton->setUsesTextLabel(false);
	QToolTip::add(undoButton, tr("Undo"));
	layout->addWidget(undoButton);
	undoMenu = new QPopupMenu(undoButton, "undoMenu");
	undoButton->setPopup(undoMenu);
	undoButton->setPopupDelay(0);

	redoButton = new QToolButton(this, "redoButton");
	redoButton->setIconSet(loadIcon("u_redo.png"));
	redoButton->setUsesTextLabel(false);
	QToolTip::add(redoButton, tr("Redo"));
	layout->addWidget(redoButton);
	redoMenu = new QPopupMenu(redoButton, "redoMenu");
	redoButton->setPopup(redoMenu);
	redoButton->setPopupDelay(0);
	
	connect(undoButton, SIGNAL(clicked()), this, SLOT(undoClicked()));
	connect(redoButton, SIGNAL(clicked()), this, SLOT(redoClicked()));
	connect(undoMenu, SIGNAL(activated(int)), this, SLOT(undoMenuClicked(int)));
	connect(redoMenu, SIGNAL(activated(int)), this, SLOT(redoMenuClicked(int)));
}

void UndoWidget::clear()
{
	undoMenu->clear();
	undoItems.clear();
	undoButton->setEnabled(false);
	redoMenu->clear();
	redoItems.clear();
	redoButton->setEnabled(false);
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
	undoItems.insert(undoItems.begin(), state->getName());
	clearRedoMenu();
	updateUndoMenu();
}

void UndoWidget::insertRedoItem(UndoObject* target, UndoState* state)
{
	redoItems.push_back(state->getName());
	updateRedoMenu();
}

void UndoWidget::clearRedoMenu()
{
	redoItems.erase(redoItems.begin(), redoItems.end());
	updateRedoMenu();
}

void UndoWidget::updateUndoMenu()
{
	undoMenu->clear();
	for (uint i = 0; i < MENU_HEIGHT && i < undoItems.size(); ++i)
		undoMenu->insertItem(undoItems[i]);
	undoButton->setEnabled(undoMenu->count() != 0);
}

void UndoWidget::updateRedoMenu()
{
	redoMenu->clear();
	for (uint i = 0; i < MENU_HEIGHT && i < redoItems.size(); ++i)
		redoMenu->insertItem(redoItems[i]);
	redoButton->setEnabled(redoMenu->count() != 0);
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
	undoItems.erase(undoItems.end() - 1);
}

UndoWidget::~UndoWidget()
{
	
}

/*** UndoPalette **************************************************************/

UndoPalette::UndoPalette(QWidget* parent, const char* name)
: UndoGui(parent, name, Qt::WType_Dialog)
{
	currentSelection = 0;
	redoItems = 0;
	setCaption(tr("Undo History"));
	QVBoxLayout* layout = new QVBoxLayout(this, 5, 5, "layout");

	QCheckBox* objectBox = new QCheckBox(tr("Show selected object only"), this, "objectBox");
	layout->addWidget(objectBox);

	undoList = new QListBox(this, "undoList");
	undoList->setMultiSelection(false);
	undoList->setSelectionMode(QListBox::Single);
	layout->addWidget(undoList);
	
	QHBoxLayout* buttonLayout = new QHBoxLayout(0, 0, 5, "buttonLayout");
	undoButton = new QPushButton(loadIcon("u_undo.png"),
                                         tr("&Undo"), this,
                                         "undoButton");
	buttonLayout->addWidget(undoButton);
	redoButton = new QPushButton(loadIcon("u_redo.png"),
                                         tr("&Redo"), this,
                                         "redoButton");
	buttonLayout->addWidget(redoButton);
	layout->addLayout(buttonLayout);

	connect(undoButton, SIGNAL(clicked()), this, SLOT(undoClicked()));
	connect(redoButton, SIGNAL(clicked()), this, SLOT(redoClicked()));
	connect(undoList, SIGNAL(highlighted(int)), this, SLOT(undoListClicked(int)));
}

void UndoPalette::clear()
{
	undoList->clear();
	undoList->insertItem(tr("Initial State"));
	undoButton->setEnabled(false);
	redoButton->setEnabled(false);
}

void UndoPalette::closeEvent(QCloseEvent* e)
{
	e->ignore();
	hide();
}

void UndoPalette::insertUndoItem(UndoObject* target, UndoState* state)
{
	removeRedoItems();
	if (state->getPixmap())
		undoList->insertItem(*(state->getPixmap()), "  " + target->getUName() + ": " + state->getName());
	else
		undoList->insertItem("  " + target->getUName() + ": " + state->getName());
	currentSelection = undoList->numRows() - 1;
	updateList();
}

void UndoPalette::insertRedoItem(UndoObject* target, UndoState* state)
{
	if (state->getPixmap())
		undoList->insertItem(*(state->getPixmap()), "  " + target->getUName() + ": " + state->getName());
	else
		undoList->insertItem("  " + target->getUName() + ": " + state->getName());
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
	undoList->removeItem(0);
	currentSelection = undoList->numRows() - 1;
}

void UndoPalette::updateList()
{
	undoList->setSelected(currentSelection, true);
	redoButton->setEnabled(currentSelection < undoList->numRows() - 1);
	undoButton->setEnabled(currentSelection > 0);
	if (!undoList->itemVisible(currentSelection))
		undoList->setBottomItem(currentSelection);
}

void UndoPalette::removeRedoItems()
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
	if (i == currentSelection)
		return;
	if (i > currentSelection)
		emit redo(i - currentSelection);
	else if (i < currentSelection)
		emit undo(currentSelection - i);
	currentSelection = i;
	updateList();
}

UndoPalette::~UndoPalette()
{

}
