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
#include "prefsmanager.h"
#include "prefsfile.h"
#include "prefscontext.h"
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
	splitter->setResizeMode(leftFrame, QSplitter::KeepSize);
	splitter->setResizeMode(rightFrame, QSplitter::Stretch);
	uniqueLabel->hide();
	rightFrame->hide();
	styleView->header()->hide();
	applyButton->setEnabled(false);
	resetButton->setEnabled(false);
	layout_ = new QGridLayout(mainFrame);
	newPopup_ = new QPopupMenu(newButton, "newPopup_");
	rightClickPopup_ = new QPopupMenu(styleView, "rightClickPopup_");
	newButton->setPopup(newPopup_);
	QString pname(name);
	if (pname == QString::null || pname.length() == 0 || pname.isEmpty())
		pname = "styleManager";
	prefs_ = PrefsManager::instance()->prefsFile->getContext(pname);
	isEditMode_ = !prefs_->getBool("isEditMode", false);
	splitterSizes_ << prefs_->getInt("Splitter1W", 190);
	splitterSizes_ << prefs_->getInt("Splitter2W", 472);
	height_ = prefs_->getInt("SplitterH", 440);
	editPosition_.setX(prefs_->getInt("eX", x()));
	editPosition_.setY(prefs_->getInt("eY", y()));
	adjustSize();

	connect(newPopup_, SIGNAL(activated(int)), this, SLOT(slotNewPopup(int)));
	connect(okButton, SIGNAL(clicked()), this, SLOT(slotOk()));
	connect(resetButton, SIGNAL(clicked()), this, SLOT(slotClean()));
	connect(applyButton, SIGNAL(clicked()), this, SLOT(slotApply()));
	connect(deleteButton, SIGNAL(clicked()), this, SLOT(slotDelete()));
	connect(cloneButton, SIGNAL(clicked()), this, SLOT(slotClone()));
	connect(newButton, SIGNAL(clicked()), this, SLOT(slotNew()));
	connect(styleView, SIGNAL(rightButtonClicked(QListViewItem*, const QPoint&, int)),
			this, SLOT(slotRightClick(QListViewItem*, const QPoint&, int)));

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
	QStringList popupStrings;
	for (uint i = 0; i < items_.count(); ++i)
	{
		popupStrings << items_.at(i)->typeNameSingular();
		items_.at(i)->languageChange();
	}
	popupStrings.sort();
	for (uint i = 0; i < popupStrings.count(); ++i)
		newPopup_->insertItem(popupStrings[i]);

	styleView->clear();
	for (uint i = 0; i < items_.count(); ++i)
		addNewType(items_.at(i));

	rightClickPopup_->clear();
	rightClickPopup_->insertItem(tr("New"), newPopup_);
	rightClickPopup_->insertItem(tr("Import"), this, SLOT(slotImport()));
	rightClickPopup_->insertSeparator();
	rcpCloneId_ = rightClickPopup_->insertItem(tr("Clone"), this, SLOT(slotClone()));
	rcpToScrapId_ = rightClickPopup_->insertItem(tr("Send to Scrapbook"), this, SLOT(slotScrap()));
	rightClickPopup_->insertSeparator();
	rcpDeleteId_ = rightClickPopup_->insertItem(tr("Delete"), this, SLOT(slotDelete()));
}

void StyleManager::currentDoc(ScribusDoc *doc)
{
	if (doc)
	{
		newButton->setEnabled(true);
		cloneButton->setEnabled(true);
		importButton->setEnabled(true);
		deleteButton->setEnabled(true);
		rightClickPopup_->setEnabled(true);
		newPopup_->setEnabled(true);
		connect(doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(slotDocSelectionChanged()));
	}
	else
	{
		newButton->setEnabled(false);
		cloneButton->setEnabled(false);
		importButton->setEnabled(false);
		deleteButton->setEnabled(false);
		rightClickPopup_->setEnabled(false);
		newPopup_->setEnabled(false);
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
	if (applyButton->isEnabled())
	{
		for (uint i = 0; i < items_.count(); ++i)
			items_.at(i)->apply();
	}

	slotClean();
}

void StyleManager::slotDelete()
{
	if (!isEditMode_)
		slotOk(); // switch to edit mode for deletion

	QStringList selected;
	QListViewItemIterator it(styleView, QListViewItemIterator::Selected);
	while (it.current()) {
		selected << it.current()->text(0);
		++it;
	}
	if (selected.isEmpty())
		return; // nothing to delete

	QStringList tmp;
	QValueList<StyleName> styles = item_->styles(false); // get list from cache
	for (uint i = 0; i < styles.count(); ++i)
		tmp << styles[i].first;
	SMReplaceDia *dia = new SMReplaceDia(selected, tmp);
	if (dia->exec() && item_)
	{
		item_->deleteStyles(dia->items());
		applyButton->setEnabled(true);
		resetButton->setEnabled(true);
		reloadStyleView(false);
	}

	delete dia;
}


void StyleManager::slotImport()
{
	
}


void StyleManager::slotClone()
{
	if (!isEditMode_)
		slotOk(); // switch to edit mode for cloning

	QListViewItemIterator it(styleView, QListViewItemIterator::Selected);
	QValueList<QPair<QString, QString> > names;

	while (it.current())
	{ // can't create styles here cause createNewStyle() alters the selection
		StyleViewItem *item = dynamic_cast<StyleViewItem*>(it.current());
		if (item)
			names << QPair<QString, QString>(item->rootName(), item->text(NAME_COL));
		++it;
	}

	for (uint i = 0; i < names.count(); ++i)
		createNewStyle(names[i].first, names[i].second);
}

void StyleManager::slotScrap()
{
	
}


void StyleManager::slotNew()
{
	if (!isEditMode_)
		slotOk(); // switch to edit mode for a new style

	// TODO maybe there's something more clever for this
	newPopup_->exec(newButton->mapToGlobal(QPoint(0, newButton->height() + 2)));
}

void StyleManager::slotNewPopup(int i)
{
	if (!isEditMode_)
		slotOk(); // switch to edit mode for a new style

	QString typeName = newPopup_->text(i);
	
	createNewStyle(typeName);
}

void StyleManager::slotRightClick(QListViewItem *item, const QPoint &point, int col)
{
	StyleViewItem *sitem = dynamic_cast<StyleViewItem*>(item);
	if (sitem && !sitem->isRoot())
	{
		styleView->clearSelection();
		styleView->setCurrentItem(item);
		item->setSelected(true);
		rightClickPopup_->setItemEnabled(rcpDeleteId_, true);
		rightClickPopup_->setItemEnabled(rcpCloneId_, true);
		rightClickPopup_->setItemEnabled(rcpToScrapId_, true);
	}
	else
	{
		rightClickPopup_->setItemEnabled(rcpDeleteId_, false);
		rightClickPopup_->setItemEnabled(rcpCloneId_, false);
		rightClickPopup_->setItemEnabled(rcpToScrapId_, false);
	}

	rightClickPopup_->exec(point);
}

void StyleManager::createNewStyle(const QString &typeName, const QString &fromParent)
{
	for (uint i = 0; i < items_.count(); ++i)
	{
		if (items_.at(i)->typeNameSingular() == typeName)
		{
			item_ = items_.at(i);
			break;
		}
	}
	Q_ASSERT(item_);

	QString newName = fromParent == QString::null ?
			item_->newStyle() : item_->newStyle(fromParent);

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
	nameEdit->setFocus();
	nameEdit->selectAll();
	applyButton->setEnabled(true);
	resetButton->setEnabled(true);
}

void StyleManager::slotOk()
{
	if (isEditMode_)
	{
		disconnect(styleView, SIGNAL(selectionChanged()), this, SLOT(slotSetupWidget()));
		slotApply();
		styleView->setSelectionMode(QListView::Multi);
		okButton->setText(QString("%1 >>").arg(tr("&Edit")));
		editFrame->hide();
		applyButton->hide();
		resetButton->hide();
		editButtonsFrame->hide();
		rightFrame->hide();
		isEditMode_ = false;
		adjustSize();
		splitter->setSizes(splitterSizes_);
		resize(splitter->width(), height_);
		move(editPosition_);
		for (uint i = 0; i < items_.count(); ++i)
		{
			items_.at(i)->apply();
			items_.at(i)->editMode(false);
		}
		QToolTip::remove(okButton);
		slotClean();
		slotDocSelectionChanged();
		splitterSizes_ = splitter->sizes();
		prefs_->set("Splitter1W", splitterSizes_[0]);
		prefs_->set("Splitter2W", splitterSizes_[1]);
		height_ = height();
		prefs_->set("SplitterH", height_);
		prefs_->set("isEditMode", isEditMode_);
		connect(styleView, SIGNAL(selectionChanged(QListViewItem*)),
		        this, SLOT(slotApplyStyle(QListViewItem*)));
		connect(styleView, SIGNAL(clicked(QListViewItem*)),
				this, SLOT(slotApplyStyle(QListViewItem*)));
	}
	else
	{
		disconnect(styleView, SIGNAL(selectionChanged(QListViewItem*)),
		           this, SLOT(slotApplyStyle(QListViewItem*)));
		disconnect(styleView, SIGNAL(clicked(QListViewItem*)),
				this, SLOT(slotApplyStyle(QListViewItem*)));
		slotSetupWidget();
		styleView->setSelectionMode(QListView::Extended);
		editPosition_.setX(x());
		editPosition_.setY(y());
		prefs_->set("eX", x());
		prefs_->set("eY", y());
		okButton->setText(QString("<< %1").arg(tr("&Done")));
		editFrame->show();
		applyButton->show();
		resetButton->show();
		editButtonsFrame->show();
		rightFrame->show();
		isEditMode_ = true;
		adjustSize();
		splitter->setSizes(splitterSizes_);
		resize(splitter->width(), height_);
		for (uint i = 0; i < items_.count(); ++i)
			items_.at(i)->editMode(true);
		QToolTip::add(okButton, tr("Apply all changes and exit edit mode"));
		slotClean();
		splitterSizes_ = splitter->sizes();
		prefs_->set("Splitter1W", splitterSizes_[0]);
		prefs_->set("Splitter2W", splitterSizes_[1]);
		height_ = height();
		prefs_->set("SplitterH", height_);
		prefs_->set("isEditMode", isEditMode_);
		connect(styleView, SIGNAL(selectionChanged()), this, SLOT(slotSetupWidget()));
	}
}

// f.e. Line Styles --> Paragraph Styles
void StyleManager::addNewType(StyleItem *item, bool loadFromDoc)
{
	if (item) {
		item_ = item;

		QValueList<StyleName> styles = item_->styles(loadFromDoc);
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
	applyButton->setEnabled(true);
	resetButton->setEnabled(true);
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
		reloadStyleView();
		slotSetupWidget();
	}
	applyButton->setEnabled(false);
	resetButton->setEnabled(false);
	uniqueLabel->hide();
}

void StyleManager::reloadStyleView(bool loadFromDoc)
{
	QListViewItemIterator it(styleView, QListViewItemIterator::Selected);
	QValueList<QPair<QString, QString> > selected;

	while (it.current())
	{
		StyleViewItem *item = dynamic_cast<StyleViewItem*>(it.current());
		if (item)
			selected << QPair<QString, QString>(item->rootName(), item->text(NAME_COL));
		++it;
	}

	styleView->clear();
	if (item_ && loadFromDoc)
		item_->reload();

	for (uint i = 0; i < items_.count(); ++i)
		addNewType(items_.at(i), false);

	QListViewItemIterator it2(styleView, QListViewItemIterator::Selectable);

	while (it2.current())
	{
		StyleViewItem *item = dynamic_cast<StyleViewItem*>(it2.current());
		if (item)
		{
			for (uint i = 0; i < selected.count(); ++i)
			{
				if (selected[i].first == item->rootName() &&
				    selected[i].second == item->text(NAME_COL))
				{
					item->setDirty(false);
					styleView->setCurrentItem(item);
					item->setSelected(true);
					break;
				}
			}
		}
		++it2;
	}

	styleView->repaint();
}

void StyleManager::insertShortcutPage(QTabWidget *twidget)
{
	if (twidget)
	{
		if (!shortcutWidget_)
		{
			shortcutWidget_ = new ShortcutWidget(0, "shortcutWidget_");
			connect(shortcutWidget_, SIGNAL(newKey(const QString&)),
					this, SLOT(slotShortcutChanged(const QString&)));
		}
		twidget->addTab(shortcutWidget_, tr("Shortcut"));
	}
}

void StyleManager::slotNameChanged(const QString& name)
{
	if (!nameIsUnique(name))
	{
		uniqueLabel->show();
		okButton->setEnabled(false);
		applyButton->setEnabled(false);
		return;
	}
	else if (uniqueLabel->isVisible())
	{
		uniqueLabel->hide();
		okButton->setEnabled(true);
		applyButton->setEnabled(true);
	}

	styleView->currentItem()->setText(NAME_COL, name);
	if (item_)
		item_->nameChanged(name);

	applyButton->setEnabled(true);
	resetButton->setEnabled(true);
}

void StyleManager::slotShortcutChanged(const QString& shortcut)
{
	styleView->currentItem()->setText(SHORTCUT_COL, shortcut == QString::null ? "" : shortcut);
// 	if (item_)
// 		item_->shortcutChanged(shortcut);
}

bool StyleManager::nameIsUnique(const QString &name)
{
	QValueList<StyleName> names = item_->styles(false);
	for (uint i = 0; i < names.count(); ++i)
	{
		if (names[i].first == name)
			return false;
	}
	return true;
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

	disconnect(nameEdit, SIGNAL(textChanged(const QString&)),
	           this, SLOT(slotNameChanged(const QString&)));
	if (typeName != QString::null)
	{
		if (selection.second.count() > 1)
		{
			nameEdit->setText("More than one style selected");
			nameEdit->setEnabled(false);
			shortcutWidget_->setEnabled(false);
		}
		else
		{
			nameEdit->setText(selection.second[0]);
			nameEdit->setEnabled(true);
			shortcutWidget_->setEnabled(true);
		}
		item_->selected(selection.second);
	}
	else
	{
		nameEdit->setText("");
		nameEdit->setEnabled(false);
	}
	connect(nameEdit, SIGNAL(textChanged(const QString&)),
	        this, SLOT(slotNameChanged(const QString&)));
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

void StyleManager::slotDocStylesChanged()
{
	qDebug("slotDocStylesChanged()");
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
	splitterSizes_ = splitter->sizes();
	prefs_->set("Splitter1W", splitterSizes_[0]);
	prefs_->set("Splitter2W", splitterSizes_[1]);
	height_ = height();
	prefs_->set("SplitterH", height_);
	prefs_->set("isEditMode", isEditMode_);
	if (isEditMode_)
	{
		prefs_->set("eX", x());
		prefs_->set("eY", y());
	}
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
	keyDisplay->setMinimumHeight(40);

	keyGroupLayout->addMultiCellWidget( keyDisplay, 3, 4, 0, 2 );

	setKeyButton = new QPushButton( tr( "Set &Key" ), keyGroup, "setKeyButton" );
	setKeyButton->setToggleButton(true);

	keyGroupLayout->addMultiCellWidget( setKeyButton, 0, 2, 1, 1, Qt::AlignCenter );
	keyManagerLayout->addWidget( keyGroup );

	connect(noKey, SIGNAL(clicked()), this, SLOT(setNoKey()));
	connect(setKeyButton, SIGNAL(clicked()), this, SLOT(setKeyText()));
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
				userDef->setChecked(true);
				releaseKeyboard();
				emit newKey(keyDisplay->text());
		}
	}
	if (setKeyButton->isOn())
	{
		keyDisplay->setText(Part0+Part1+Part2+Part3+Part4);
	}
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
	{
		keyDisplay->setText("");
		emit newKey(QString::null);
	}
}

ShortcutWidget::~ShortcutWidget()
{

}




