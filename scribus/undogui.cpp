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

#include <qlayout.h>
#include <qpixmap.h>
#include <qpushbutton.h>
#include <qrect.h>
#include <qstyle.h>
#include <qtooltip.h>
#include <qcheckbox.h>
#include <qfont.h>
#include <qfontmetrics.h>
 
#include "undogui.h"
#include "undogui.moc"
#include "prefsfile.h"
#include "prefscontext.h"
#include "scribus.h"
#include "menumanager.h"

extern QPixmap loadIcon(QString nam);
extern PrefsFile* prefsFile;
extern ScribusApp *ScApp;

UndoGui::UndoGui(QWidget* parent, const char* name, WFlags f) : QWidget(parent, name, f)
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
	ScApp->scrActions["editUndoAction"]->addTo(parent);
	ScApp->scrActions["editRedoAction"]->addTo(parent);
	ScApp->scrMenuMgr->createMenu("undoButtonMenu", "undoButtonMenu");
	ScApp->scrMenuMgr->createMenu("redoButtonMenu", "redoButtonMenu");
	undoMenu=ScApp->scrMenuMgr->getLocalPopupMenu("undoButtonMenu");
	redoMenu=ScApp->scrMenuMgr->getLocalPopupMenu("redoButtonMenu");
	ScApp->scrMenuMgr->addMenuToWidgetOfAction("undoButtonMenu", ScApp->scrActions["editUndoAction"]);
	ScApp->scrMenuMgr->addMenuToWidgetOfAction("redoButtonMenu", ScApp->scrActions["editRedoAction"]);
	QToolButton *undoButton = dynamic_cast<QToolButton*>(ScApp->scrActions["editUndoAction"]->getWidgetAddedTo());
	QToolButton *redoButton = dynamic_cast<QToolButton*>(ScApp->scrActions["editRedoAction"]->getWidgetAddedTo());
	if (undoButton && redoButton)
	{
		undoButton->setPopupDelay(0);
		redoButton->setPopupDelay(0);
	}
	
	ScApp->scrActions["editCut"]->addTo(parent);
	ScApp->scrActions["editCopy"]->addTo(parent);
	ScApp->scrActions["editPaste"]->addTo(parent);
	
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
	ScApp->scrActions["editUndoAction"]->setEnabled(false);
	// BnF disable
	//undoButton->setEnabled(false);
	redoMenu->clear();
	redoItems.clear();
	//Scribus disable;
	ScApp->scrActions["editRedoAction"]->setEnabled(false);			
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
	undoItems.insert(undoItems.begin(), QString(tr("%1: %2", "undo target: action (f.e. Text frame: Resize)"))
                                        .arg(target->getUName()).arg(state->getName()));
	clearRedo();
	updateUndoMenu();
	updateRedoMenu();
}

void UndoWidget::insertRedoItem(UndoObject* target, UndoState* state)
{
	redoItems.push_back(QString(tr("%1: %2", "undo target: action (f.e. Text frame: Resize)"))
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
	ScApp->scrActions["editUndoAction"]->setEnabled(undoMenu->count() != 0);
}

void UndoWidget::updateRedoMenu()
{
	redoMenu->clear();
	for (uint i = 0; i < MENU_HEIGHT && i < redoItems.size(); ++i)
		redoMenu->insertItem(redoItems[i]);
	//BnF
	//redoButton->setEnabled(redoMenu->count() != 0);
	//Scribus
	ScApp->scrActions["editRedoAction"]->setEnabled(redoMenu->count() != 0);
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
: UndoGui(parent, name, Qt::WType_Dialog)
{
	currentSelection = 0;
	redoItems = 0;
	setCaption(tr("Action History"));
	QVBoxLayout* layout = new QVBoxLayout(this, 5, 5, "layout");

	QCheckBox* objectBox = new QCheckBox(tr("Show selected object only"), this, "objectBox");
	layout->addWidget(objectBox);
// 	objectBox->setEnabled(false);

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
	//Save the translated key sequence - hopefully we get the translated one here!
	initialUndoKS = undoButton->accel();
	initialRedoKS = redoButton->accel();
	
	buttonLayout->addWidget(redoButton);
	layout->addLayout(buttonLayout);

	undoPrefs = prefsFile->getContext("undo");
	updateFromPrefs();
	connect(ScApp, SIGNAL(prefsChanged()), this, SLOT(updateFromPrefs()));
	connect(undoButton, SIGNAL(clicked()), this, SLOT(undoClicked()));
	connect(redoButton, SIGNAL(clicked()), this, SLOT(redoClicked()));
	connect(undoList, SIGNAL(highlighted(int)), this, SLOT(undoListClicked(int)));
	connect(undoList, SIGNAL(onItem(QListBoxItem*)), this, SLOT(showToolTip(QListBoxItem*)));
	connect(undoList, SIGNAL(onViewport()), this, SLOT(removeToolTip()));
	connect(objectBox, SIGNAL(toggled(bool)), this, SLOT(objectCheckBoxClicked(bool)));
	connect(ScApp->scrActions["editActionMode"], SIGNAL(toggled(bool)),
	        objectBox, SLOT(setChecked(bool)));
	connect(objectBox, SIGNAL(toggled(bool)), 
			ScApp->scrActions["editActionMode"], SLOT(setOn(bool)));
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
	emit closePalette(false);
	e->ignore();
	hide();
}

void UndoPalette::show()
{
	QWidget::show();
	if (undoPrefs->contains("up_left"))
	{
		int left   = undoPrefs->getInt("up_left", 100);
		int top    = undoPrefs->getInt("up_top", 100);
		int width  = undoPrefs->getInt("up_width", 200);
		int height = undoPrefs->getInt("up_height", 350);
		QRect r(left, top, width, height);
		setGeometry(r);
	}
}

void UndoPalette::updateFromPrefs()
{
	undoButton->setAccel(ScApp->scrActions["editUndoAction"]->accel());
	redoButton->setAccel(ScApp->scrActions["editRedoAction"]->accel());
}

void UndoPalette::hideEvent(QHideEvent*)
{
	storePosition();
}

void UndoPalette::keyPressEvent(QKeyEvent* e)
{
	if (e->key() == Key_Escape)
		hide();
	QWidget::keyPressEvent(e);
}

void UndoPalette::keyReleaseEvent(QKeyEvent* e)
{
	//TODO Do we need to check more meta keys, CTRL, SHIFT, META, UNICODE_ACCEL ? Could just grab the meta keys used
	QKeySequence ks(ALT + e->key());
	//QKeySequence ks(e->key()|e->state());
	if (ks==initialUndoKS)
		undoClicked();
	if (ks==initialRedoKS)
		redoClicked();
	QWidget::keyReleaseEvent(e);
}

void UndoPalette::insertUndoItem(UndoObject* target, UndoState* state)
{
	clearRedo();
	undoList->insertItem(new UndoItem(target->getUName(), state->getName(),
                         state->getDescription(), target->getUPixmap(),
                         state->getPixmap()));
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
                         state->getPixmap()));
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
}

void UndoPalette::clearRedo()
{
	for (int i = (undoList->numRows() - 1); i > currentSelection; --i)
		undoList->removeItem(i);
}

void UndoPalette::storePosition()
{
	QRect r    = frameGeometry();
	int left   = r.left();
	int top    = r.top();
	int width  = r.width();
	int height = r.height();
	undoPrefs->set("up_left", left);
	undoPrefs->set("up_top", top);
	undoPrefs->set("up_width", width);
	undoPrefs->set("up_height", height);
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

void UndoPalette::showToolTip(QListBoxItem *i)
{
	UndoItem *item = dynamic_cast<UndoItem*>(i);
	if (item)
	{
		QString tip = item->getDescription();
		if (tip != 0)
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
	storePosition();
}

/*** UndoPalette::UndoItem ****************************************************/

UndoPalette::UndoItem::UndoItem() : QListBoxItem()
{
	target = "";
	action = "";
	description = "";
	targetpixmap = NULL;
	actionpixmap = NULL;
}

UndoPalette::UndoItem::UndoItem(const UndoItem &another) : QListBoxItem()
{
	target = another.target;
	action = another.action;
	description = another.description;
	targetpixmap = another.targetpixmap;
	actionpixmap = another.actionpixmap;
}

UndoPalette::UndoItem::UndoItem(const QString &targetName,
                                const QString &actionName,
                                const QString &actionDescription,
                                QPixmap *targetPixmap,
                                QPixmap *actionPixmap)
: QListBoxItem()
{
	target = targetName;
	action = actionName;
	description = actionDescription;
	targetpixmap = targetPixmap;
	actionpixmap = actionPixmap;
}

void UndoPalette::UndoItem::paint(QPainter *painter)
{
	if (targetpixmap)
		painter->drawPixmap(5, 5, *targetpixmap);
	if (actionpixmap)
		painter->drawPixmap(5, 5, *actionpixmap);
	QFont f = QFont(painter->font());
	f.setPointSize(f.pointSize() - 2);
	painter->setFont(f);
	painter->drawText(32, QFontMetrics(f).height(), target);
	f.setItalic(true);
	painter->setFont(f);
	painter->drawText(32, (2 * QFontMetrics(painter->font()).height()), action);
}

int UndoPalette::UndoItem::height(const QListBox *lb) const
{
	if (lb)
	{
		int fontHeight = 2 * QFontMetrics(lb->font()).height();
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

int UndoPalette::UndoItem::width(const QListBox *lb) const
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

UndoPalette::UndoItem::~UndoItem()
{

}
