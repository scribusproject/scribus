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

StyleManager::StyleManager(QWidget *parent, const char *name) : SMBase(parent, name), item_(0), widget_(0), shortcutWidget_(0), currentType_(QString::null), isEditMode_(true)
{
	splitter->setMinimumWidth(0);
	styleView->header()->hide();
	layout_ = new QGridLayout(mainFrame);
	resize(QSize(500, 400));
	noEditSize_ = QSize(166, 400);
	noEditSizes_ = splitter->sizes();
	slotOk();
}

void StyleManager::currentDoc(ScribusDoc *doc)
{
	static bool hasBeenConnected = false;
	if (doc && doc->m_Selection && !hasBeenConnected)
	{
		connect(doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(slotSelectionChanged()));
		hasBeenConnected = true;
	}

	// clear the style list and reload from new doc
	styleView->clear();
	for (uint i = 0; i < items_.count(); ++i)
	{
		items_.at(i)->currentDoc(doc);
		slotNewType(items_.at(i)); // forces a reload
	}
}

void StyleManager::languageChange()
{

}

void StyleManager::addStyle(StyleItem *item)
{
	items_.append(item);
	slotNewType(item);
}

void StyleManager::slotApply()
{
	for (uint i = 0; i < items_.count(); ++i)
		items_.at(i)->apply();
}

void StyleManager::slotDelete()
{
	QStringList selected;
	QListViewItemIterator it(styleView, QListViewItemIterator::Selected);
	while (it.current()) {
		selected << it.current()->text(0);
		++it;
	}
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
	slotStyleChanged(); // force an update of style list
}


void StyleManager::slotImport()
{
	
}


void StyleManager::slotClone()
{
	
}


void StyleManager::slotNew()
{
	
}

void StyleManager::slotOk()
{
	if (isEditMode_)
	{
		slotApply();
		editSize_ = size();
		editSizes_ = splitter->sizes();
		buttonOk->setText(QString("%1 >>").arg(tr("&Edit")));
		editFrame->hide();
		applyButton->hide();
		buttonCancel->hide();
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
	}
	else
	{
		noEditSize_ = size();
		noEditSizes_ = splitter->sizes();
		buttonOk->setText(QString("<< %1").arg(tr("&Done")));
		editFrame->show();
		applyButton->show();
		buttonCancel->show();
		editButtonsFrame->show();
		isEditMode_ = true;
		adjustSize();
		resize(editSize_);
		splitter->setSizes(editSizes_);
		for (uint i = 0; i < items_.count(); ++i)
			items_.at(i)->editMode(true);
	}
}

void StyleManager::slotStyleChanged()
{
	QStringList selected;
	QListViewItemIterator it(styleView, QListViewItemIterator::Selected);
	bool twoTypes = false;
	QString type = QString::null;
	while (it.current()) {
		selected << it.current()->text(0);
		++it;
		StyleViewItem *item = dynamic_cast<StyleViewItem*>(it.current());
		if (item)
		{
			if (type == QString::null)
				type = item->rootName();
			else if (type != item->rootName())
			{
				twoTypes = true;
				break;
			}
		}
	}

	int count = selected.count();
	disconnect(nameEdit, SIGNAL(textChanged(const QString&)),
	           this, SLOT(slotNameChanged(const QString&)));
	if (twoTypes) {
		nameEdit->setText("");
		nameEdit->setEnabled(false);
		widget_->setEnabled(false);
		shortcutWidget_->setEnabled(false);
	} else if (count > 1) {
		nameEdit->setText(tr("More than one item selected"));
		nameEdit->setEnabled(false);
		widget_->setEnabled(true);
		shortcutWidget_->setEnabled(false);
	} else if (count == 1) {
		nameEdit->setEnabled(true);
		nameEdit->setText(selected[0]);
		widget_->setEnabled(true);
		shortcutWidget_->setEnabled(true);
	} else {

		nameEdit->setText("");
		nameEdit->setEnabled(false);
		widget_->setEnabled(false);
		shortcutWidget_->setEnabled(false);
	}
	connect(nameEdit, SIGNAL(textChanged(const QString&)),
	        this, SLOT(slotNameChanged(const QString&)));
	if (item_) {
		item_->selected(selected);
	}
}

// f.e. Line Styles --> Paragraph Styles
void StyleManager::slotNewType(StyleItem *item)
{
	if (item) {
		item_ = item;

		QValueList<StyleName> styles = item_->styles();
		QListViewItem *rootItem = new StyleViewItem(styleView, item_->typeName());
		for (uint i = 0; i < styles.count(); ++i) // set the list of styles of this type
		{
			if (styles[i].second == QString::null)
				new StyleViewItem(rootItem, styles[i].first, item_->typeName());
			else // TODO Search the parent and insert accordingly
				new StyleViewItem(rootItem, styles[i].first, item_->typeName());
		}

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

void StyleManager::slotPageChanged(QWidget */*widget*/)
{

}

void StyleManager::slotNameChanged(const QString& name)
{
	styleView->currentItem()->setText(0, name);
	if (item_)
		item_->nameChanged(name);
}

void StyleManager::slotSelectionChanged()
{
	if (isHidden())
		return;

	QString parentName = QString::null;
	QString styleName = QString::null;
	styleView->clearSelection();

	for (uint i = 0; i < items_.count(); ++i)
	{
		parentName = items_.at(i)->typeName();
		styleName = items_.at(i)->fromSelection();
		if (styleName != QString::null)
		{
			QListViewItem *item = styleView->findItem(styleName, 0);
			if (item)
			{
				QListViewItem *root = item->parent();
				if (!root)
					continue;
				while (root->parent())
					root = root->parent();
				if (parentName == root->text(0))
				{
					styleView->setCurrentItem(item);
					item->setSelected(true);
					item->repaint();
				}
			}
		}
	}
}

void StyleManager::itemClicked(QListViewItem *item)
{
	QListViewItem *root = 0;
	StyleItem     *sitem = 0;
	if (isEditMode_ && item)
	{
		root = item->parent();
		if (root == 0)
			root = item;

		while (root->parent())
			root = root->parent();
		if (root)
		{
			for (uint i = 0; i < items_.count(); ++i)
			{
				if (items_.at(i)->typeName() == root->text(0))
				{
					sitem = items_.at(i);
					break;
				}
			}
			if (sitem)
			{
				if (item_ == sitem)
					return; // no need to update anything already selected style
				item_ = sitem;
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
		}

	}
	else if (!isEditMode_ && item) // see if we need to apply style
	{
	
	}
	else // if (!item)
	{
	
	}
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

StyleViewItem::StyleViewItem(QListView *view, const QString &text)
: QListViewItem(view, text), isRoot_(true), parentName_(QString::null), rootName_(QString::null)
{
	setSelectable(false);
}

StyleViewItem::StyleViewItem(QListViewItem *parent, const QString &text, const QString &rootName)
: QListViewItem(parent, text), isRoot_(false), parentName_(parent->text(0)), rootName_(rootName)
{

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




