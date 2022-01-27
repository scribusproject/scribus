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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.             *
 ***************************************************************************/
#include <QCheckBox>
#include <QDebug>
#include <QEvent>
#include <QPainter>
#include <QPushButton>
#include <QVBoxLayout>

#include "iconmanager.h"
#include "prefsmanager.h"
#include "scraction.h"
#include "scribuscore.h"
#include "ui/scmwmenumanager.h"
#include "undogui.h"


UndoGui::UndoGui(QWidget* parent, const char* name, Qt::WindowFlags f) : ScDockPalette(parent, name, f)
{
	languageChange();
}

void UndoGui::languageChange()
{
	setWindowTitle( tr("Action History"));
}

/*** UndoWidget ***************************************************************/

UndoWidget::UndoWidget(QWidget* parent, const char* name) : UndoGui(parent, name)
{
	auto &actions = ScCore->primaryMainWindow()->scrActions;
	auto menuManager = ScCore->primaryMainWindow()->scrMenuMgr;

	//Scribus action based toolbar button construction
	parent->addAction(actions["editUndoAction"]);
	parent->addAction(actions["editRedoAction"]);

	menuManager->createMenu("undoButtonMenu", "undoButtonMenu");
	menuManager->createMenu("redoButtonMenu", "redoButtonMenu");
	undoMenu = menuManager->undoMenu();
	redoMenu = menuManager->redoMenu();

	parent->addAction(actions["editCut"]);
	parent->addAction(actions["editCopy"]);
	parent->addAction(actions["editPaste"]);

	connect(undoMenu, SIGNAL(triggered(QAction*)), this, SLOT(undoMenuClicked(QAction*)));
	connect(redoMenu, SIGNAL(triggered(QAction*)), this, SLOT(redoMenuClicked(QAction*)));
}

void UndoWidget::clear()
{
	undoMenu->clear();
	undoItems.clear();
	//Scribus disable
	ScCore->primaryMainWindow()->scrActions["editUndoAction"]->setEnabled(false);
	redoMenu->clear();
	redoItems.clear();
	//Scribus disable;
	ScCore->primaryMainWindow()->scrActions["editRedoAction"]->setEnabled(false);
}

void UndoWidget::undoClicked()
{
	if (!undoItems.empty())
		emit undo(1);
}

void UndoWidget::redoClicked()
{
	if (!redoItems.empty())
		emit redo(1);
}

void UndoWidget::undoMenuClicked(QAction *id)
{
	int steps = undoMenu->actions().indexOf(id) + 1;
	emit undo(steps);
}

void UndoWidget::redoMenuClicked(QAction *id)
{
	int steps = redoMenu->actions().indexOf(id) + 1;
	emit redo(steps);
}

void UndoWidget::insertUndoItem(UndoObject* target, UndoState* state)
{
	undoItems.insert(undoItems.begin(), QString( tr("%1: %2", "undo target: action (f.e. Text frame: Resize)")).arg(target->getUName(), state->getName()));
	clearRedo();
	updateUndoMenu();
	updateRedoMenu();
}

void UndoWidget::insertRedoItem(UndoObject* target, UndoState* state)
{
	redoItems.push_back(QString( tr("%1: %2", "undo target: action (f.e. Text frame: Resize)")).arg(target->getUName(), state->getName()));
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
		undoMenu->addAction(undoItems[i]);
	updateUndoActions();
}

void UndoWidget::updateRedoMenu()
{
	redoMenu->clear();
	for (uint i = 0; i < MENU_HEIGHT && i < redoItems.size(); ++i)
		redoMenu->addAction(redoItems[i]);
	updateUndoActions();
}

void UndoWidget::updateUndoActions()
{
	ScCore->primaryMainWindow()->scrActions["editUndoAction"]->setEnabled(undoMenu->actions().count() != 0);
	ScCore->primaryMainWindow()->scrActions["editRedoAction"]->setEnabled(redoMenu->actions().count() != 0);
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
	if (!undoItems.empty())
	{
		undoItems.erase(undoItems.end() - 1);
		updateUndoMenu();
	}
}

/*** UndoPalette **************************************************************/

UndoPalette::UndoPalette(QWidget* parent, const char* name) : UndoGui(parent, name)
{
	setObjectName(QString::fromLocal8Bit(name));
	setMinimumSize( QSize(220, 240) );
	setSizePolicy( QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));

	container = new QWidget(this);
	QVBoxLayout* layout = new QVBoxLayout(container);
	layout->setContentsMargins(3, 3, 3, 3);
	layout->setSpacing(3);
	objectBox = new QCheckBox(this);
	layout->addWidget(objectBox);

	undoList = new QListWidget(this);
	undoList->setSelectionMode(QAbstractItemView::SingleSelection);
	undoList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	layout->addWidget(undoList);

	QHBoxLayout* buttonLayout = new QHBoxLayout;
	buttonLayout->setContentsMargins(0, 0, 0, 0);
	buttonLayout->setSpacing(6);
	undoButton = new QPushButton(IconManager::instance().loadPixmap("16/edit-undo.png"), "", this);
	buttonLayout->addWidget(undoButton);
	redoButton = new QPushButton(IconManager::instance().loadPixmap("16/edit-redo.png"), "", this);
	buttonLayout->addWidget(redoButton);
	//Save the translated key sequence - hopefully we get the translated one here!
	initialUndoKS = undoButton->shortcut();
	initialRedoKS = redoButton->shortcut();
	layout->addLayout(buttonLayout);
	setWidget(container);

	updateFromPrefs();
	languageChange();
	connect(&PrefsManager::instance(), SIGNAL(prefsChanged()), this, SLOT(updateFromPrefs()));
	connect(undoButton, SIGNAL(clicked()), this, SLOT(undoClicked()));
	connect(redoButton, SIGNAL(clicked()), this, SLOT(redoClicked()));
	connect(undoList, SIGNAL(currentRowChanged(int)), this, SLOT(undoListClicked(int)));
	connect(undoList, SIGNAL(itemEntered(QListWidgetItem*)), this, SLOT(showToolTip(QListWidgetItem*)));
	connect(undoList, SIGNAL(viewportEntered()), this, SLOT(removeToolTip()));
	connect(objectBox, SIGNAL(toggled(bool)), this, SLOT(objectCheckBoxClicked(bool)));
	connect(ScCore->primaryMainWindow()->scrActions["editActionMode"], SIGNAL(toggled(bool)),
	        objectBox, SLOT(setChecked(bool)));
	connect(objectBox, SIGNAL(toggled(bool)),
			ScCore->primaryMainWindow()->scrActions["editActionMode"], SLOT(setChecked(bool)));
}

void UndoPalette::clear()
{
	disconnect(undoList, SIGNAL(currentRowChanged(int)), this, SLOT(undoListClicked(int)));
	undoList->clear();
	undoList->addItem( tr("Initial State"));
	undoButton->setEnabled(false);
	redoButton->setEnabled(false);
	connect(undoList, SIGNAL(currentRowChanged(int)), this, SLOT(undoListClicked(int)));
}

void UndoPalette::updateFromPrefs()
{
	undoButton->setShortcut(ScCore->primaryMainWindow()->scrActions["editUndoAction"]->shortcut());
	redoButton->setShortcut(ScCore->primaryMainWindow()->scrActions["editRedoAction"]->shortcut());
}

void UndoPalette::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
		languageChange();
	else
		UndoGui::changeEvent(e);
}

void UndoPalette::languageChange()
{
	UndoGui::languageChange();

	objectBox->setText( tr("Show Selected Object Only"));
	undoButton->setText( tr("&Undo"));
	redoButton->setText( tr("&Redo"));
	
	objectBox->setToolTip( "<qt>" + tr( "Show the action history for the selected item only. This changes the effect of the undo/redo buttons to act on the object or document." ) + "</qt>" );
	undoButton->setToolTip( "<qt>" + tr( "Undo the last action for either the current object or the document" ) + "</qt>");
	redoButton->setToolTip( "<qt>" + tr( "Redo the last action for either the current object or the document" ) + "</qt>");
}

void UndoPalette::insertUndoItem(UndoObject* target, UndoState* state)
{
	clearRedo();
	undoList->addItem(new UndoItem(target->getUName(), state->getName(),
                         state->getDescription(), target->getUPixmap(),
                         state->getPixmap(), true));
	currentSelection = undoList->count() - 1;
	updateList();
}

void UndoPalette::insertRedoItem(UndoObject* target, UndoState* state)
{
	if (undoList->count() == 1)
	{
		undoList->setCurrentItem(undoList->item(0));
		currentSelection = 0;
	}
	undoList->addItem(new UndoItem(target->getUName(), state->getName(),
                         state->getDescription(), target->getUPixmap(),
                         state->getPixmap(), false));
	updateList();
}

void UndoPalette::updateUndo(int steps)
{
	if (undoList->row(undoList->currentItem()) == currentSelection)
	{
		currentSelection -= steps;
		updateList();
	}
}

void UndoPalette::updateRedo(int steps)
{
	if (undoList->row(undoList->currentItem()) == currentSelection)
	{
		currentSelection += steps;
		updateList();
	}
}

void UndoPalette::popBack()
{
	if (undoList->count() > 1)
	{
		delete undoList->takeItem(0);
		currentSelection = undoList->count() - 1;
	}
}

void UndoPalette::updateList()
{
	undoList->setCurrentRow(currentSelection);
	redoButton->setEnabled(currentSelection < undoList->count() - 1);
	undoButton->setEnabled(currentSelection > 0);
	undoList->scrollToItem(undoList->item(currentSelection));
	for (int i = 0; i < undoList->count(); ++i)
	{
		auto *item = dynamic_cast<UndoItem*>(undoList->item(i));
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
	for (int i = (undoList->count() - 1); i > currentSelection; --i)
		delete undoList->takeItem(i);
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

void UndoPalette::showToolTip(QListWidgetItem *i)
{
	auto *item = dynamic_cast<UndoItem*>(i);
	if (item)
	{
		QString tip = item->getDescription();
		if (!tip.isNull()) /*TODO: Doesn't make sense! */
		  undoList->setToolTip(tip);
	}
	else
		removeToolTip();
}

void UndoPalette::removeToolTip()
{
	undoList->setToolTip("");
}

/*** UndoPalette::UndoItem ****************************************************/

UndoPalette::UndoItem::UndoItem(const UndoItem &another)
	: m_targetPixmap(another.m_targetPixmap),
	  m_actionPixmap(another.m_actionPixmap),
	  m_target(another.m_target),
	  m_action(another.m_action),
	  m_description(another.m_description),
	  m_isUndoAction(another.m_isUndoAction)
{

}

UndoPalette::UndoItem::UndoItem(const QString &targetName,
                                const QString &actionName,
                                const QString &actionDescription,
                                QPixmap *targetPixmap,
                                QPixmap *actionPixmap,
                                bool isUndoAction,
                                QListWidget * parent
							   )
	: QListWidgetItem(parent),
	m_targetPixmap(targetPixmap),
	m_actionPixmap(actionPixmap),
	m_target(targetName),
	m_action(actionName),
	m_description(actionDescription),
	m_isUndoAction(isUndoAction)
{
	/*TODO: 16x16 is hardcoded, because images automatically scaled by QIcon are no longer recognizable 
	would be better to have the icons designed for 16x16*/
	if (targetPixmap)
	{
		QPixmap pixmap;
		if (!targetPixmap->isNull())
			pixmap = targetPixmap->scaled(16, 16);
		if (actionPixmap && !actionPixmap->isNull())
		{
			QPainter p;
			p.begin(&pixmap);
			p.drawPixmap(0,0, actionPixmap->scaled(16, 16));
			p.end();
		}
		setIcon(pixmap);
	}
	else
	{
		if (actionPixmap && !actionPixmap->isNull())
			setIcon(actionPixmap->scaled(16, 16));
	}
	setText(tr("%1 - %2\n%3").arg(targetName, actionName, actionDescription));
}

QString UndoPalette::UndoItem::getDescription() const
{
  return m_description;
}

bool UndoPalette::UndoItem::isUndoAction() const
{
	return m_isUndoAction;
}

void UndoPalette::UndoItem::setUndoAction(bool isUndo)
{
	m_isUndoAction = isUndo;
	QFont f = font();
	f.setItalic(!m_isUndoAction);
	setFont(f);
}
