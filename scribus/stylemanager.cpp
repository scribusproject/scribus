/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "stylemanager.h"
#include "stylemanager.moc"
#include "styleitem.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "smreplacedia.h"
#include "styleitem.h"
#include "selection.h"
#include <qheader.h>
#include <qlabel.h>
#include <qlistview.h>
#include <qtabwidget.h>
#include <qtoolbutton.h>
#include <qevent.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qpushbutton.h>
#include <qsplitter.h>
#include <qsize.h>
#include <qfont.h>
#include <qpainter.h>
#include <qtooltip.h>
#include <qpair.h>

StyleManager::StyleManager(QWidget *parent, const char *name) : SMBase(parent, name), item_(0), widget_(0), shortcutWidget_(0), currentType_(QString::null), isEditMode_(true)
{
	splitter->setMinimumWidth(0);
	styleView->header()->hide();
	layout_ = new QGridLayout(mainFrame);
	resize(QSize(500, 400));
	noEditSize_ = QSize(166, 400);
	noEditSizes_ = splitter->sizes();
	newPopup_ = new QPopupMenu(newButton, "newPopup_");
	newButton->setPopup(newPopup_);

	connect(newPopup_, SIGNAL(activated(int)), this, SLOT(slotNewPopup(int)));
	connect(okButton, SIGNAL(clicked()), this, SLOT(slotOk()));
	connect(resetButton, SIGNAL(clicked()), this, SLOT(slotClean()));
	connect(applyButton, SIGNAL(clicked()), this, SLOT(slotApply()));
	connect(deleteButton, SIGNAL(clicked()), this, SLOT(slotDelete()));
	connect(newButton, SIGNAL(clicked()), this, SLOT(slotNew()));

	languageChange();
	slotOk();
}

void StyleManager::languageChange()
{
	nameLabel->setText(tr("Name:"));

	resetButton->setText(tr("&Reset"));
	applyButton->setText(tr("&Apply"));
	okButton->setText(isEditMode_ ? tr("<< &Done") : tr("&Edit >>"));
	newButton->setText(tr("&New"));
	importButton->setText(tr("&Import"));
	cloneButton->setText(tr("&Clone"));
	deleteButton->setText(tr("&Delete"));

	QToolTip::add(resetButton, tr("Reset all changes"));
	QToolTip::add(applyButton, tr("Apply all changes"));
	if (isEditMode_)
		QToolTip::add(okButton, tr("Apply all changes and exit edit mode"));
	QToolTip::add(newButton, tr("Create a new style"));
	QToolTip::add(importButton, tr("Import styles from another document"));
	QToolTip::add(cloneButton, tr("Clone selected style"));
	QToolTip::add(deleteButton, tr("Delete selected styles"));

	if (shortcutWidget_)
		shortcutWidget_->languageChange();

	newPopup_->clear();
	for (uint i = 0; i < items_.count(); ++i)
	{
		newPopup_->insertItem(items_.at(i)->typeNameSingular());
		items_.at(i)->languageChange();
	}

	styleView->clear();
	for (uint i = 0; i < items_.count(); ++i)
		addNewType(items_.at(i));
}

void StyleManager::currentDoc(ScribusDoc *doc)
{
	static bool hasBeenConnected = false;
	if (doc && doc->m_Selection && !hasBeenConnected)
	{
		connect(doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(slotDocSelectionChanged()));
		hasBeenConnected = true;
	}

	// clear the style list and reload from new doc
	styleView->clear();
	for (uint i = 0; i < items_.count(); ++i)
	{
		items_.at(i)->currentDoc(doc);
		addNewType(items_.at(i)); // forces a reload
	}
}

void StyleManager::addStyle(StyleItem *item)
{
	items_.append(item);
	addNewType(item);
	connect(item, SIGNAL(selectionDirty()), this, SLOT(slotDirty()));
}

void StyleManager::slotApply()
{
	for (uint i = 0; i < items_.count(); ++i)
		items_.at(i)->apply();

	slotClean();
}

void StyleManager::slotDelete()
{
	QStringList selected;
	QListViewItemIterator it(styleView, QListViewItemIterator::Selected);
	while (it.current()) {
		selected << it.current()->text(0);
		++it;
	}
	if (selected.isEmpty())
		return; // nothing to delete

	QStringList tmp;
	QValueList<StyleName> styles = item_->styles();
	for (uint i = 0; i < styles.count(); ++i)
		tmp << styles[i].first;
	SMReplaceDia *dia = new SMReplaceDia(selected, tmp);
	if (dia->exec())
	{
		if (item_)
			item_->deleteStyles(dia->items());
	}
	delete dia;

	// TODO update styleView
}


void StyleManager::slotImport()
{
	
}


void StyleManager::slotClone()
{
	
}


void StyleManager::slotNew()
{
	// TODO maybe there's something more clever for this
	newPopup_->exec(newButton->mapToGlobal(QPoint(0, newButton->height() + 2)));
}

void StyleManager::slotNewPopup(int i)
{
	QString typeName = newPopup_->text(i);
	for (uint i = 0; i < items_.count(); ++i)
	{
		if (items_.at(i)->typeNameSingular() == typeName)
		{
			item_ = items_.at(i);
			break;
		}
	}
	Q_ASSERT(item_);
	QString newName = item_->newStyle();

	if (newName == QString::null)
		return;

	QListViewItem *root = 0;
	QListViewItemIterator it(styleView, QListViewItemIterator::NotSelectable);
	while (it.current())
	{
		StyleViewItem *item = dynamic_cast<StyleViewItem*>(it.current());
		if (item)
		{
			if (item->text(NAME_COL) == item_->typeName())
			{
				root = item;
				break;
			}
		}
		++it;
	}
	Q_ASSERT(root);
	styleView->clearSelection();
	StyleViewItem *newItem = new StyleViewItem(root, newName, item_->typeName());
	Q_CHECK_PTR(newItem);
	newItem->setDirty(true);
	styleView->setCurrentItem(newItem);
	newItem->setSelected(true);
	newItem->repaint();
	slotSetupWidget();
}

void StyleManager::slotOk()
{
	if (isEditMode_)
	{
		disconnect(styleView, SIGNAL(selectionChanged()), this, SLOT(slotSetupWidget()));
		slotApply();
		styleView->setSelectionMode(QListView::Multi);
		editSize_ = size();
		editSizes_ = splitter->sizes();
		okButton->setText(QString("%1 >>").arg(tr("&Edit")));
		editFrame->hide();
		applyButton->hide();
		resetButton->hide();
		editButtonsFrame->hide();
		isEditMode_ = false;
		adjustSize();
		resize(noEditSize_);
		splitter->setSizes(noEditSizes_);
		for (uint i = 0; i < items_.count(); ++i)
		{
			items_.at(i)->apply();
			items_.at(i)->editMode(false);
		}
		QToolTip::remove(okButton);
		slotClean();
		slotDocSelectionChanged();
		connect(styleView, SIGNAL(currentChanged(QListViewItem*)),
		        this, SLOT(slotApplyStyle(QListViewItem*)));
	}
	else
	{
		disconnect(styleView, SIGNAL(currentChanged(QListViewItem*)),
		           this, SLOT(slotApplyStyle(QListViewItem*)));
		slotSetupWidget();
		styleView->setSelectionMode(QListView::Extended);
		noEditSize_ = size();
		noEditSizes_ = splitter->sizes();
		okButton->setText(QString("<< %1").arg(tr("&Done")));
		editFrame->show();
		applyButton->show();
		resetButton->show();
		editButtonsFrame->show();
		isEditMode_ = true;
		adjustSize();
		resize(editSize_);
		splitter->setSizes(editSizes_);
		for (uint i = 0; i < items_.count(); ++i)
			items_.at(i)->editMode(true);
		QToolTip::add(okButton, tr("Apply all changes and exit edit mode"));
		slotClean();
		connect(styleView, SIGNAL(selectionChanged()), this, SLOT(slotSetupWidget()));
	}
}

// f.e. Line Styles --> Paragraph Styles
void StyleManager::addNewType(StyleItem *item)
{
	if (item) {
		item_ = item;

		QValueList<StyleName> styles = item_->styles();
		QListViewItem *rootItem = new StyleViewItem(styleView, item_->typeName());
		rootItem->setOpen(true);
		for (uint i = 0; i < styles.count(); ++i) // set the list of styles of this type
		{
			if (styles[i].second == QString::null)
				new StyleViewItem(rootItem, styles[i].first, item_->typeName());
			else // TODO Search the parent and insert accordingly
				new StyleViewItem(rootItem, styles[i].first, item_->typeName());
		}
	}

}

void StyleManager::slotDirty()
{
	QListViewItemIterator it(styleView, QListViewItemIterator::Selected);

	while (it.current()) {
		StyleViewItem *item = dynamic_cast<StyleViewItem*>(it.current());
		if (item)
		{
			item->setDirty(true);
			item->repaint();
			applyButton->setEnabled(true);
			resetButton->setEnabled(true);
		}
		++it;
	}
}

void StyleManager::slotClean()
{
	QListViewItemIterator it(styleView);

	while (it.current()) {
		StyleViewItem *item = dynamic_cast<StyleViewItem*>(it.current());
		if (item)
		{
			item->setDirty(false);
			item->repaint();
		}
		++it;
	}

	if (isEditMode_ && item_)
	{
		item_->reload();
		slotSetupWidget();
	}
}

void StyleManager::insertShortcutPage(QTabWidget *twidget)
{
	if (twidget)
	{
		if (!shortcutWidget_)
			shortcutWidget_ = new ShortcutWidget(0, "shortcutWidget_");
		twidget->addTab(shortcutWidget_, tr("Shortcut"));
	}
}

void StyleManager::slotNameChanged(const QString& name)
{
	styleView->currentItem()->setText(0, name);
	if (item_)
		item_->nameChanged(name);
}


// setups the selected type and edit widgets related to it
void StyleManager::slotSetupWidget()
{
	QPair<QString, QStringList> selection = namesFromSelection();
	QString typeName = selection.first;

	if (typeName == QString::null && widget_)
		widget_->setEnabled(false); // nothing selected or two or more different types
	else if (!item_ || item_->typeName() != typeName || widget_ != item_->widget())
		loadType(typeName); // new type selected
	else if (widget_ && !widget_->isEnabled())
		widget_->setEnabled(true);

	if (typeName != QString::null)
		item_->selected(selection.second);

}

void StyleManager::slotApplyStyle(QListViewItem *item)
{
	StyleViewItem *sitem = dynamic_cast<StyleViewItem*>(item);

	if (isEditMode_ || !sitem || sitem->isRoot())
		return; // don't apply a style in edit mode or if there was no item/type selected

	styleView->clearSelection();

	if (!item_ || item_->typeName() != sitem->rootName())
		loadType(sitem->rootName()); // load the type where we want to apply this style

	Q_ASSERT(item_);

	item_->toSelection(sitem->text(NAME_COL)); // apply selected style to the selection

	slotDocSelectionChanged();
}

void StyleManager::slotDocSelectionChanged()
{
	if (isEditMode_)
		return; // don't track changes when in edit mode

	disconnect(styleView, SIGNAL(currentChanged(QListViewItem*)),
	           this, SLOT(slotApplyStyle(QListViewItem*)));

	styleView->clearSelection();

	QValueList<QPair<QString, QString> > selected;

	for (uint i = 0; i < items_.count(); ++i)
		selected << QPair<QString, QString>(items_.at(i)->typeName(), items_.at(i)->fromSelection());
	
	QListViewItemIterator it(styleView, QListViewItemIterator::Selectable);
	StyleViewItem *item;

	while (it.current())
	{
		item = dynamic_cast<StyleViewItem*>(it.current());
		if (item)
		{
			for (uint i = 0; i < selected.count(); ++i)
			{
				if (item->rootName() == selected[i].first && item->text(NAME_COL) == selected[i].second)
				{
					styleView->setCurrentItem(item);
					item->setSelected(true);
					item->repaint();
				}
			}
		}
		++it;
	}

	styleView->triggerUpdate();

	connect(styleView, SIGNAL(currentChanged(QListViewItem*)),
	        this, SLOT(slotApplyStyle(QListViewItem*)));
}

// QPair.first == QString::null if nothing is selected or if
// there are items from more than one type in the selection
// if selection is valid (only from single type) QPair.first will
// include the type name and QPair.second will have all the selected
// stylenames in it
QPair<QString, QStringList> StyleManager::namesFromSelection()
{
	QString typeName = QString::null;
	QStringList styleNames;
	QListViewItemIterator it(styleView, QListViewItemIterator::Selected);
	while (it.current())
	{
		StyleViewItem *item = dynamic_cast<StyleViewItem*>(it.current());
		if (!item)
		{
			++it;
			continue;
		}
		else if (typeName == QString::null)
			typeName = item->rootName();
		else if (typeName != QString::null && typeName != item->rootName())
		{
			typeName = QString::null;
			break; // two different types selected returning null
		}

		if (!item->isRoot())
			styleNames << item->text(NAME_COL);

		++it;
	}
	return QPair<QString, QStringList>(typeName, styleNames);
}

// sets the current type to name including item_ and the main widget
// for editing styles
void StyleManager::loadType(const QString &name)
{
	item_ = 0;
	for (uint i = 0; i < items_.count(); ++i)
	{
		if (items_.at(i)->typeName() == name)
		{
			item_ = items_.at(i);
			break;
		}
	}

	if (!item_)
		return;

	if (widget_)
	{   // remove the old style type's widget
		widget_->hide();
		layout_->remove(widget_);
		widget_->reparent(0,0, QPoint(0,0), false);
	}
	// show the widget for the new style type
	if (shortcutWidget_)
		widget_->removePage(shortcutWidget_);
	widget_ = item_->widget(); // show the widget for the style type
	insertShortcutPage(widget_);
	widget_->reparent(mainFrame, 0, QPoint(0,0), true);
	layout_->addWidget(widget_, 0, 0);
}

void StyleManager::hideEvent(QHideEvent *e)
{
	SMBase::hideEvent(e);
	emit closed();
}

StyleManager::~StyleManager()
{
	
}

/*** StyleViewItem *******************************************************************/

StyleViewItem::StyleViewItem(QListView *view, const QString &text) : QListViewItem(view, text),
isRoot_(true), isDirty_(false), parentName_(QString::null), rootName_(QString::null)
{
	setSelectable(false);
}

StyleViewItem::StyleViewItem(QListViewItem *parent, const QString &text, const QString &rootName)
: QListViewItem(parent, text),
  isRoot_(false), isDirty_(false), parentName_(parent->text(0)), rootName_(rootName)
{

}

void StyleViewItem::paintCell(QPainter *p, const QColorGroup &cg, int column, int width, int align)
{
	if (column == 0)
	{
		QFont f(p->font());
		f.setBold(isDirty_);
		p->setFont(f);
	}

	QListViewItem::paintCell(p, cg, column, width, align);
}

void StyleViewItem::setDirty(bool isDirty)
{
	isDirty_ = isDirty;
}

bool StyleViewItem::isDirty()
{
	return isDirty_;
}

bool StyleViewItem::isRoot()
{
	return isRoot_;
}

QString StyleViewItem::parentName()
{
	return parentName_;
}

QString StyleViewItem::rootName()
{
	return rootName_;
}

StyleViewItem::~StyleViewItem()
{

}

/*** ShortcutWidget ******************************************************************/

ShortcutWidget::ShortcutWidget(QWidget *parent, const char *name) : QWidget(parent, name)
{
	Part0 = "";
	Part1 = "";
	Part2 = "";
	Part3 = "";
	keyCode = 0;
	keyManagerLayout = new QVBoxLayout( this, 0, 6); 
	keyManagerLayout->setAlignment( Qt::AlignHCenter );

	keyGroup = new QButtonGroup( this, "keyGroup" );
	keyGroup->setFrameStyle(0);
	keyGroup->setColumnLayout(0, Qt::Vertical );
	keyGroup->layout()->setSpacing( 0 );
	keyGroup->layout()->setMargin( 0 );
	keyGroupLayout = new QGridLayout( keyGroup->layout() );
	keyGroupLayout->setAlignment( Qt::AlignTop );
	keyGroupLayout->setSpacing( 6 );
	keyGroupLayout->setMargin( 11 );

	noKey = new QRadioButton( tr( "&No Key" ), keyGroup, "noKey" );
	keyGroupLayout->addMultiCellWidget( noKey, 0, 1, 0, 1 );
	userDef = new QRadioButton( tr( "&User Defined Key" ), keyGroup, "userDef" );
	keyGroupLayout->addWidget( userDef, 2, 0 );

	keyDisplay = new QLabel( tr( "ALT+SHIFT+T" ), keyGroup, "keyDisplay" );
	keyDisplay->setFrameShape( QLabel::Panel );
	keyDisplay->setFrameShadow( QLabel::Sunken );
	keyDisplay->setAlignment( static_cast<int>( QLabel::AlignCenter ) );

	keyGroupLayout->addMultiCellWidget( keyDisplay, 3, 4, 0, 2 );

	setKeyButton = new QPushButton( tr( "Set &Key" ), keyGroup, "setKeyButton" );
	setKeyButton->setToggleButton(true);

	keyGroupLayout->addMultiCellWidget( setKeyButton, 0, 2, 1, 1, Qt::AlignCenter );
	keyManagerLayout->addWidget( keyGroup );

	connect( noKey, SIGNAL(clicked()), this, SLOT(setNoKey()));
	connect( setKeyButton, SIGNAL(clicked()), this, SLOT(setKeyText()));
}

void ShortcutWidget::languageChange()
{
	noKey->setText(tr("&No Key"));
	userDef->setText(tr("&User Defined Key"));
	setKeyButton->setText(tr("Set &Key"));
	
}

bool ShortcutWidget::event( QEvent* ev )
{
	bool ret = QWidget::event( ev );
	if ( ev->type() == QEvent::KeyPress )
		keyPressEvent((QKeyEvent*)ev);
	if ( ev->type() == QEvent::KeyRelease )
		keyReleaseEvent((QKeyEvent*)ev);
	return ret;
}

void ShortcutWidget::keyPressEvent(QKeyEvent *k)
{
	if (setKeyButton->isOn())
	{
		QStringList tl;
		if (!keyDisplay->text().isEmpty())
		{
			tl = tl.split("+", keyDisplay->text());
			Part4 = tl[tl.count()-1];
			if (Part4 == tr("Alt") || Part4 == tr("Ctrl") || Part4 == tr("Shift") || Part4 == tr("Meta"))
				Part4 = "";
		}
		else
			Part4 = "";
		switch (k->key())
		{
			case Key_Meta:
				Part0 = tr("Meta+");
				keyCode |= META;
				break;
			case Key_Shift:
				Part3 = tr("Shift+");
				keyCode |= SHIFT;
				break;
			case Key_Alt:
				Part2 = tr("Alt+");
				keyCode |= ALT;
				break;
			case Key_Control:
				Part1 = tr("Ctrl+");
				keyCode |= CTRL;
				break;
			default:
				keyCode |= k->key();
//				qDebug(QString("got key %1 %2").arg(keyCode).arg(getKeyText(keyCode)));
				keyDisplay->setText(getKeyText(keyCode));
// 				if (checkKey(keyCode))
// 				{
// 					QMessageBox::information(this,
// 											CommonStrings::trWarning,
// 											tr("This key sequence is already in use"),
// 											CommonStrings::tr_OK);
// 					keyTable->setText(currRow, 1, "");
// 					keyDisplay->setText("");
// 					if (currentKeyMapRow!=NULL)
// 						currentKeyMapRow.data().keySequence=QKeySequence();
// 					noKey->setChecked(true);
// 				}
// 				else
// 				{
// 					QKeySequence newKeySequence(keyCode);
// 					keyTable->setText(currRow, 1, QString(newKeySequence));
// 					if (currentKeyMapRow!=NULL)
// 						currentKeyMapRow.data().keySequence=newKeySequence;
// 					userDef->setChecked(true);
// 				}
				setKeyButton->setOn(false);
				releaseKeyboard();
		}
	}
	if (setKeyButton->isOn())
		keyDisplay->setText(Part0+Part1+Part2+Part3+Part4);
}

void ShortcutWidget::keyReleaseEvent(QKeyEvent *k)
{
	if (setKeyButton->isOn())
	{
		if (!keyDisplay->text().isEmpty())
		{
			QStringList tl;
			tl = tl.split("+", keyDisplay->text());
			Part4 = tl[tl.count()-1];
			if (Part4 == tr("Alt") || Part4 == tr("Ctrl") || Part4 == tr("Shift") || Part4 == tr("Meta"))
				Part4 = "";
		}
		else
			Part4 = "";
		if (k->key() == Key_Meta)
		{
			Part0 = "";
			keyCode &= ~META;
		}
		if (k->key() == Key_Shift)
		{
			Part3 = "";
			keyCode &= ~SHIFT;
		}
		if (k->key() == Key_Alt)
		{
			Part2 = "";
			keyCode &= ~ALT;
		}
		if (k->key() == Key_Control)
		{
			Part1 = "";
			keyCode &= ~CTRL;
		}
		keyDisplay->setText(Part0+Part1+Part2+Part3+Part4);
	}
}

QString ShortcutWidget::getKeyText(int KeyC)
{
	if ((KeyC & ~(Qt::META | Qt::CTRL | Qt::ALT | Qt::SHIFT)) == 0)
		return "";
	// on OSX Qt translates modifiers to forsaken symbols, arrows and the like
	// we prefer plain English
	QString res;
	if ((KeyC & Qt::META) != 0)
		res += "Meta+";
	if ((KeyC & Qt::CTRL) != 0)
		res += "Ctrl+";
	if ((KeyC & Qt::ALT) != 0)
		res += "Alt+";
	if ((KeyC & Qt::SHIFT) != 0)
		res += "Shift+";
	return res + QString(QKeySequence(KeyC & ~(Qt::META | Qt::CTRL | Qt::ALT | Qt::SHIFT)));
}

void ShortcutWidget::setKeyText()
{
	if (setKeyButton->isOn())
	{
		keyCode = 0;
		Part0 = "";
		Part1 = "";
		Part2 = "";
		Part3 = "";
		Part4 = "";
		grabKeyboard();
	}
	else
		releaseKeyboard();
}

void ShortcutWidget::setNoKey()
{
	if (noKey->isChecked())
		keyDisplay->setText("");
}

ShortcutWidget::~ShortcutWidget()
{

}




