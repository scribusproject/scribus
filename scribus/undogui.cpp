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
	setWindowTitle( tr("Action History"));
}

void UndoGui::languageChange()
{
	setWindowTitle( tr("Action History"));
}

/*** UndoWidget ***************************************************************/

UndoWidget::UndoWidget(QWidget* parent, const char* name) : UndoGui(parent, name)
{
	/* BnF standard toolbar buttons
	QHBoxLayout* layout = new QHBoxLayout(this, 0, 0, "layout");


	undoButton = new QToolButton(this, "undoButton");
	undoButton->setIconSet(loadIcon("u_undo.png"));
	undoButton->setUsesTextLabel(false);
	undoButton->setToolTip(tr("Undo"));
	layout->addWidget(undoButton);
	undoMenu = new QPopupMenu(undoButton, "undoMenu");
	undoButton->setPopup(undoMenu);
	undoButton->setPopupDelay(0);
	undoButton->setAutoRaise(true);

	redoButton = new QToolButton(this, "redoButton");
	redoButton->setIconSet(loadIcon("u_redo.png"));
	redoButton->setUsesTextLabel(false);
	redoButton->setToolTip(tr("Redo"));
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
	//undoMenu=ScCore->primaryMainWindow()->scrMenuMgr->getLocalPopupMenu("undoButtonMenu");
	//redoMenu=ScCore->primaryMainWindow()->scrMenuMgr->getLocalPopupMenu("redoButtonMenu");
	undoMenu=ScCore->primaryMainWindow()->scrMenuMgr->undoMenu();
	redoMenu=ScCore->primaryMainWindow()->scrMenuMgr->redoMenu();
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
	connect(undoMenu, SIGNAL(triggered(QAction *)), this, SLOT(undoMenuClicked(QAction *)));
	connect(redoMenu, SIGNAL(triggered(QAction *)), this, SLOT(redoMenuClicked(QAction *)));
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
		undoMenu->addAction(undoItems[i]);
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
		redoMenu->addAction(redoItems[i]);
	//BnF
	//redoButton->setEnabled(redoMenu->count() != 0);
	//Scribus
	//ScMW->scrActions["editRedoAction"]->setEnabled(redoMenu->count() != 0);
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

UndoPalette::UndoPalette(QWidget* parent, const char* name) : UndoGui(parent, name)
{
	setObjectName(QString::fromLocal8Bit(name));
	setSizePolicy( QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));

	m_container = new QWidget(this);
	m_currentSelection = 0;
	m_redoItems = 0;
	QVBoxLayout* layout = new QVBoxLayout(m_container);
	layout->setMargin(5);
	layout->setSpacing(5);
	m_objectBox = new QCheckBox(this);
	layout->addWidget(m_objectBox);
// 	objectBox->setEnabled(false);

	m_undoList = new QListWidget(this);
// 	undoList->setMultiSelection(false);
// 	undoList->setSelectionMode(QListWidget::Single);
	m_undoList->setSelectionMode(QAbstractItemView::SingleSelection);
	m_undoList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	layout->addWidget(m_undoList);

	QHBoxLayout* buttonLayout = new QHBoxLayout;
	buttonLayout->setMargin(0);
	buttonLayout->setSpacing(5);
	m_undoButton = new QPushButton(IconManager::instance()->loadPixmap("16/edit-undo.png"), "", this);
	buttonLayout->addWidget(m_undoButton);
	m_redoButton = new QPushButton(IconManager::instance()->loadPixmap("16/edit-redo.png"), "", this);
	buttonLayout->addWidget(m_redoButton);
	//Save the translated key sequence - hopefully we get the translated one here!
	m_initialUndoKS = m_undoButton->shortcut();
	m_initialRedoKS = m_redoButton->shortcut();
	layout->addLayout(buttonLayout);
	setWidget(m_container);

	updateFromPrefs();
	languageChange();
	connect(PrefsManager::instance(), SIGNAL(prefsChanged()), this, SLOT(updateFromPrefs()));
	connect(m_undoButton, SIGNAL(clicked()), this, SLOT(undoClicked()));
	connect(m_redoButton, SIGNAL(clicked()), this, SLOT(redoClicked()));
// 	connect(undoList, SIGNAL(highlighted(int)), this, SLOT(undoListClicked(int)));
	connect(m_undoList, SIGNAL(currentRowChanged(int)), this, SLOT(undoListClicked(int)));
// 	connect(undoList, SIGNAL(onItem(Q3ListBoxItem*)), this, SLOT(showToolTip(Q3ListBoxItem*)));
	connect(m_undoList, SIGNAL(itemEntered(QListWidgetItem*)), this, SLOT(showToolTip(QListWidgetItem*)));
// 	connect(undoList, SIGNAL(onViewport()), this, SLOT(removeToolTip()));
	connect(m_undoList, SIGNAL(viewportEntered()), this, SLOT(removeToolTip()));
	connect(m_objectBox, SIGNAL(toggled(bool)), this, SLOT(objectCheckBoxClicked(bool)));
	connect(ScCore->primaryMainWindow()->scrActions["editActionMode"], SIGNAL(toggled(bool)),
	        m_objectBox, SLOT(setChecked(bool)));
	connect(m_objectBox, SIGNAL(toggled(bool)),
			ScCore->primaryMainWindow()->scrActions["editActionMode"], SLOT(setChecked(bool)));
}

void UndoPalette::clear()
{
// 	qDebug() << "UndoPalette::clear start";
	disconnect(m_undoList, SIGNAL(currentRowChanged(int)), this, SLOT(undoListClicked(int)));
	m_undoList->clear();
	m_undoList->addItem( tr("Initial State"));
	m_undoButton->setEnabled(false);
	m_redoButton->setEnabled(false);
	connect(m_undoList, SIGNAL(currentRowChanged(int)), this, SLOT(undoListClicked(int)));
// 	qDebug() << "UndoPalette::clear end";
}

void UndoPalette::updateFromPrefs()
{
// 	qDebug() << "UndoPalette::updateFromPrefs start";
	m_undoButton->setShortcut(ScCore->primaryMainWindow()->scrActions["editUndoAction"]->shortcut());
	m_redoButton->setShortcut(ScCore->primaryMainWindow()->scrActions["editRedoAction"]->shortcut());
// 	qDebug() << "UndoPalette::updateFromPrefs end";
}

void UndoPalette::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
	else
		UndoGui::changeEvent(e);
}

void UndoPalette::languageChange()
{
	UndoGui::languageChange();

	m_objectBox->setText( tr("Show Selected Object Only"));
	m_undoButton->setText( tr("&Undo"));
	m_redoButton->setText( tr("&Redo"));
	
	m_objectBox->setToolTip( "<qt>" + tr( "Show the action history for the selected item only. This changes the effect of the undo/redo buttons to act on the object or document." ) + "</qt>" );
	m_undoButton->setToolTip( "<qt>" + tr( "Undo the last action for either the current object or the document" ) + "</qt>");
	m_redoButton->setToolTip( "<qt>" + tr( "Redo the last action for either the current object or the document" ) + "</qt>");
}

void UndoPalette::insertUndoItem(UndoObject* target, UndoState* state)
{
// 	qDebug() << "UndoPalette::insertUndoItem start";
	clearRedo();
	m_undoList->addItem(new UndoItem(target->getUName(), state->getName(),
                         state->getDescription(), target->getUPixmap(),
                         state->getPixmap(), true));
	m_currentSelection = m_undoList->count() - 1;
	updateList();
// 	qDebug() << "UndoPalette::insertUndoItem end";
}

void UndoPalette::insertRedoItem(UndoObject* target, UndoState* state)
{
// 	qDebug() << "UndoPalette::insertRedoItem start";
	if (m_undoList->count() == 1)
	{
// 		qDebug() << "UndoPalette::insertRedoItem undoList->count == 1";
// 		undoList->setSelected(0, true);
		m_undoList->setCurrentItem(m_undoList->item(0));
		m_currentSelection = 0;
	}
	m_undoList->addItem(new UndoItem(target->getUName(), state->getName(),
                         state->getDescription(), target->getUPixmap(),
                         state->getPixmap(), false));
	updateList();
// 	qDebug() << "UndoPalette::insertRedoItem end";
}

void UndoPalette::updateUndo(int steps)
{
// 	qDebug() << "UndoPalette::updateUndo start";
	if (m_undoList->row(m_undoList->currentItem()) == m_currentSelection)
	{
		m_currentSelection -= steps;
		updateList();
	}
// 	qDebug() << "UndoPalette::updateUndo end";
}

void UndoPalette::updateRedo(int steps)
{
// 	qDebug() << "UndoPalette::updateRedo start";
	if (m_undoList->row(m_undoList->currentItem()) == m_currentSelection)
	{
		m_currentSelection += steps;
		updateList();
	}
// 	qDebug() << "UndoPalette::updateRedo end";
}

void UndoPalette::popBack()
{
// 	qDebug() << "UndoPalette::popBack start";
	if (m_undoList->count() > 1)
	{
// 		undoList->removeItem(0);
		delete m_undoList->takeItem(0);
		m_currentSelection = m_undoList->count() - 1;
	}
// 	qDebug() << "UndoPalette::popBack end";
}

void UndoPalette::updateList()
{
// 	qDebug() << "UndoPalette::updateList start";
// 	undoList->setCurrentItem(currentSelection);
	m_undoList->setCurrentRow(m_currentSelection);
// 	undoList->setSelected(currentSelection, true);
	m_redoButton->setEnabled(m_currentSelection < m_undoList->count() - 1);
	m_undoButton->setEnabled(m_currentSelection > 0);
// 	if (!undoList->item(currentSelection)->isVisible()) //itemVisible(currentSelection))
// 		undoList->setBottomItem(currentSelection);
	m_undoList->scrollToItem(m_undoList->item(m_currentSelection));
	for (int i = 0; i < m_undoList->count(); ++i)
	{
		UndoItem *item = dynamic_cast<UndoItem*>(m_undoList->item(i));
		if (!item)
			continue;

		item->setUndoAction(m_currentSelection >= i);
	}
// 	qDebug() << "UndoPalette::updateList end";
}

void UndoPalette::updateUndoActions()
{
	//ScMW->scrActions["editUndoAction"]->setEnabled(currentSelection > 0);
	//ScMW->scrActions["editRedoAction"]->setEnabled(currentSelection < undoList->numRows() - 1);
}

void UndoPalette::clearRedo()
{
// 	qDebug() << "UndoPalette::clearRedo start";
	for (int i = (m_undoList->count() - 1); i > m_currentSelection; --i)
// 		undoList->removeItem(i);
		delete m_undoList->takeItem(i);
// 	qDebug() << "UndoPalette::clearRedo end";
}

void UndoPalette::undoClicked()
{
// 	qDebug() << "UndoPalette::undoClicked emitted";
	emit undo(1);
}

void UndoPalette::redoClicked()
{
// 	qDebug() << "UndoPalette::redoClicked emitted";
	emit redo(1);
}

void UndoPalette::undoListClicked(int i)
{
// 	qDebug() << "UndoPalette::undoListClicked start";
	if (i == m_currentSelection || (i == 0 && m_undoList->count() == 1))
		return;
	if (i > m_currentSelection)
		emit redo(i - m_currentSelection);
	else if (i < m_currentSelection)
		emit undo(m_currentSelection - i);
	m_currentSelection = i;
	updateList();
// 	qDebug() << "UndoPalette::undoListClicked end";
}

void UndoPalette::objectCheckBoxClicked(bool on)
{
// 	qDebug() << "UndoPalette::objectCheckBoxClicked start";
	emit objectMode(on);
// 	qDebug() << "UndoPalette::objectCheckBoxClicked end";
}

void UndoPalette::showToolTip(QListWidgetItem *i)
{
// 	qDebug() << "UndoPalette::showToolTip start";
	UndoItem *item = dynamic_cast<UndoItem*>(i);
	if (item)
	{
		QString tip = item->getDescription();
		if (!tip.isNull()) /*TODO: Doesn't make sense! */
		  m_undoList->setToolTip(tip);
	}
	else
		removeToolTip();
// 	qDebug() << "UndoPalette::showToolTip end";
}

void UndoPalette::removeToolTip()
{
// 	qDebug() << "UndoPalette::removeToolTip start";
	m_undoList->setToolTip("");
// 	qDebug() << "UndoPalette::removeToolTip end";
}

UndoPalette::~UndoPalette()
{

}

/*** UndoPalette::UndoItem ****************************************************/

UndoPalette::UndoItem::UndoItem() : QListWidgetItem()
{
	m_target = "";
	m_action = "";
	m_description = "";
	m_targetpixmap = NULL;
	m_actionpixmap = NULL;
	m_isUndoAction_ = true;
}

UndoPalette::UndoItem::UndoItem(const UndoItem &another) : QListWidgetItem()
{
	m_target = another.m_target;
	m_action = another.m_action;
	m_description = another.m_description;
	m_targetpixmap = another.m_targetpixmap;
	m_actionpixmap = another.m_actionpixmap;
	m_isUndoAction_ = another.m_isUndoAction_;
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
	m_targetpixmap(targetPixmap),
	m_actionpixmap(actionPixmap),
	m_target(targetName),
	m_action(actionName),
	m_description(actionDescription),
	m_isUndoAction_(isUndoAction)
{
	/*TODO: 16x16 is hardcoded, because images automatically scaled by QIcon are no longer recognizable 
	would be better to have the icons designed for 16x16*/
	if (!targetPixmap) {
		if (actionPixmap) {
			setIcon(actionPixmap->scaled(16,16));
		}
	} else {
		QPixmap pixmap = targetPixmap->scaled(16,16);
		if (actionPixmap) {
			QPainter p;
			p.begin(&pixmap);
			p.drawPixmap(0,0, actionPixmap->scaled(16,16));
			p.end();
		}
		setIcon(pixmap);
	}
	setText(tr("%1 - %2\n%3").arg(targetName).arg(actionName).arg(actionDescription));
}

//void UndoPalette::UndoItem::paint(QPainter *painter)
//{
// 	if (targetpixmap)
// 		painter->drawPixmap(5, 5, *targetpixmap);
// 	if (actionpixmap)
// 		painter->drawPixmap(5, 5, *actionpixmap);
// 	QPen  p = QPen(painter->pen());
// 	QFont f = QFont(painter->font());
// 	painter->setFont(f);
// 	if (isUndoAction_)
// 	{
// 		p.setColor(QColor("black"));
// 		f.setItalic(false);
// 	}
// 	else
// 	{
// 		p.setColor(QColor("darkGray"));
// 		f.setItalic(true);
// 	}
// 	painter->setPen(p);
// 	painter->setFont(f);
// 	painter->drawText(32, QFontMetrics(f).height(), target);
// 	painter->drawText(32, (2 * QFontMetrics(painter->font()).height()), action);
//}

/*
int UndoPalette::UndoItem::height(const QListWidget *lb) const
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

int UndoPalette::UndoItem::width(const QListWidget *lb) const
{
	if (lb)
		return target.length() > action.length() ?
               39 + QFontMetrics(lb->font()).width(target) :
               39 + QFontMetrics(lb->font()).width(action);
	else
		return 0;
}
*/

QString UndoPalette::UndoItem::getDescription()
{
  return m_description;
}

bool UndoPalette::UndoItem::isUndoAction()
{
	return m_isUndoAction_;
}

void UndoPalette::UndoItem::setUndoAction(bool isUndo)
{
	m_isUndoAction_ = isUndo;
	QFont f = font();
	f.setItalic(!m_isUndoAction_);
	setFont(f);
}

UndoPalette::UndoItem::~UndoItem()
{

}
