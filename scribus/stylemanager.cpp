/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QEvent>
#include <QMenu>
#include <QToolTip>
#include <QMessageBox>

#include "prefsmanager.h"
#include "prefsfile.h"
#include "commonstrings.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "stylemanager.h"
#include "shortcutwidget.h"
#include "styleview.h"
#include "styleitem.h"
#include "smreplacedia.h"
#include "selection.h"
#include "customfdialog.h"
#include "smtextstyles.h"
#include "smlinestyle.h"
#include "smlinestylewidget.h"
#include "smstyleimport.h"
#include "fileloader.h"
#include "scraction.h"


const QString StyleManager::SEPARATOR = "$$$$"; // dumb but it works

StyleManager::StyleManager(QWidget *parent, const char *name)
	: ScrPaletteBase(parent, name), item_(0), widget_(0),
	shortcutWidget_(0), currentType_(QString::null), isEditMode_(true), doc_(0)
{
	setupUi(this);

	uniqueLabel->hide();
	rightFrame->hide();

	applyButton->setEnabled(false);
	resetButton->setEnabled(false);
	layout_ = new QGridLayout(mainFrame);
	newPopup_ = new QMenu(newButton);
	rightClickPopup_ = new QMenu(styleView);
	newButton->setMenu(newPopup_);
	QString pname(name);
	if (pname.isEmpty())
		pname = "styleManager";
	prefs_ = PrefsManager::instance()->prefsFile->getContext(pname);
	isEditMode_ = true;
	isStoryEditMode_ = false;
	editPosition_.setX(prefs_->getInt("eX", x()));
	editPosition_.setY(prefs_->getInt("eY", y()));

	newButton->setEnabled(false);
	cloneButton->setEnabled(false);
	importButton->setEnabled(false);
	deleteButton->setEnabled(false);
	rightClickPopup_->setEnabled(false);
	newPopup_->setEnabled(false);

	connect(newPopup_, SIGNAL(triggered(QAction*)), this, SLOT(slotNewPopup(QAction*)));
	connect(okButton, SIGNAL(clicked()), this, SLOT(slotOk()));
	connect(importButton, SIGNAL(clicked()), this, SLOT(slotImport()));
	connect(resetButton, SIGNAL(clicked()), this, SLOT(slotClean()));
	connect(applyButton, SIGNAL(clicked()), this, SLOT(slotApply()));
	connect(deleteButton, SIGNAL(clicked()), this, SLOT(slotDelete()));
	connect(cloneButton, SIGNAL(clicked()), this, SLOT(slotClone()));
	connect(newButton, SIGNAL(clicked()), this, SLOT(slotNew()));
	connect(styleView, SIGNAL(customContextMenuRequested(const QPoint &)),
			this, SLOT(slotRightClick(const QPoint &)));
	connect(styleView, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)),
			this, SLOT(slotDoubleClick(QTreeWidgetItem *, int)));

	languageChange();
	slotOk();
}

void StyleManager::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
	else
		QWidget::changeEvent(e);
}

void StyleManager::languageChange()
{
/***********************************/
/*      Begin Tooltips             */
/***********************************/
// These are for general Style Manager widgets (not for c/pstyles except the name field

	// for the "<< Done" button when in edit mode
	exitEditModeOk_ = tr("Apply all changes and exit edit mode");
	// for the "Edit >>" button when not in edit mode
	enterEditModeOk_= tr("Edit styles");

	nameEdit->setToolTip(     tr("Name of the selected style"));
	resetButton->setToolTip(  tr("Reset all changes"));
	applyButton->setToolTip(  tr("Apply all changes"));
	newButton->setToolTip(    tr("Create a new style"));
	importButton->setToolTip( tr("Import styles from another document"));
	cloneButton->setToolTip(  tr("Clone selected style"));
	deleteButton->setToolTip( tr("Delete selected styles"));

/***********************************/
/*      End Tooltips               */
/***********************************/

	nameLabel->setText( tr("Name:"));
	resetButton->setText( tr("&Reset"));
	applyButton->setText( tr("&Apply"));
	doneText= tr("&Done");
	editText= tr("&Edit");
	setOkButtonText();
	newButton->setText( tr("&New"));
	importButton->setText( tr("&Import"));
	cloneButton->setText( tr("&Clone"));
	deleteButton->setText( tr("&Delete"));

	if (isEditMode_)
		okButton->setToolTip( exitEditModeOk_);
	else
		okButton->setToolTip( enterEditModeOk_);

	if (shortcutWidget_)
		shortcutWidget_->languageChange();

	newPopup_->clear();
	QStringList popupStrings;
	for (int i = 0; i < items_.count(); ++i)
	{
		popupStrings << items_.at(i)->typeNameSingular();
		items_.at(i)->languageChange();
		styleClassesPS_[items_.at(i)->typeNamePlural()] = items_.at(i)->typeNameSingular();
		styleClassesSP_[items_.at(i)->typeNameSingular()] = items_.at(i)->typeNamePlural();
	}
	popupStrings.sort();
	for (int i = 0; i < popupStrings.count(); ++i)
		newPopup_->addAction(popupStrings[i]);

	styleView->clear();
	for (int i = 0; i < items_.count(); ++i)
		addNewType(items_.at(i));
	styleView->resizeColumnToContents(0);

	rightClickPopup_->clear();
	rcpNewId_ = rightClickPopup_->addMenu(newPopup_);
	rcpNewId_->setText( tr("New"));
	rightClickPopup_->addAction( tr("Import"), this, SLOT(slotImport()));
	rightClickPopup_->addSeparator();
	rcpEditId_ = rightClickPopup_->addAction( tr("Edit"), this, SLOT(slotEdit()));
	rcpCloneId_ = rightClickPopup_->addAction( tr("Clone"), this, SLOT(slotClone()));
	rcpToScrapId_ = rightClickPopup_->addAction( tr("Send to Scrapbook"), this, SLOT(slotScrap()));
	rightClickPopup_->addSeparator();
	rcpDeleteId_ = rightClickPopup_->addAction( tr("Delete"), this, SLOT(slotDelete()));
}

void StyleManager::unitChange()
{
	if (doc_)
	{
		for (int i = 0; i < items_.count(); ++i)
			items_.at(i)->unitChange();

		slotSetupWidget();
	}
}

void StyleManager::setOkButtonText()
{
	if (!isStoryEditMode_)
		okButton->setText(isEditMode_ ? "<< " + doneText : editText + " >>");
	else
		okButton->setText(CommonStrings::tr_OK);
}

void StyleManager::setDoc(ScribusDoc *doc)
{
	if (doc)
	{
		doc_ = doc;
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
		doc_ = 0;
		newButton->setEnabled(false);
		cloneButton->setEnabled(false);
		importButton->setEnabled(false);
		deleteButton->setEnabled(false);
		rightClickPopup_->setEnabled(false);
		newPopup_->setEnabled(false);
	}

	// clear the style list and reload from new doc
	styleView->clear();
	styleActions_.clear();
	for (int i = 0; i < items_.count(); ++i)
	{
		items_.at(i)->currentDoc(doc);
		addNewType(items_.at(i)); // forces a reload
		if (doc_)
			items_.at(i)->unitChange();
	}
	styleView->resizeColumnToContents(0);
}

void StyleManager::updateColorList()
{
	for (int i = 0; i < items_.count(); ++i)
	{
		items_.at(i)->currentDoc(doc_);
		items_.at(i)->reload();
	}
}

void StyleManager::addStyle(StyleItem *item)
{
	items_.append(item);
	addNewType(item);
	languageChange(); // upgrade the popup menu with the new item
	connect(item, SIGNAL(selectionDirty()), this, SLOT(slotDirty()));
}

void StyleManager::slotApply()
{
	if (applyButton->isEnabled())
	{
		for (int i = 0; i < items_.count(); ++i)
			items_.at(i)->apply();
	}

	slotClean();
}

void StyleManager::slotDelete()
{
	if (!isEditMode_)
		slotOk(); // switch to edit mode before deleting

	QStringList selected;

	if (!rcStyle_.isNull())
		selected << rcStyle_;
	else
	{
		QTreeWidgetItemIterator it(styleView, QTreeWidgetItemIterator::Selected);
		while (*it)
		{
			selected << (*it)->text(0);
			++it;
		}
	}
	if (!item_ || selected.isEmpty())
		return; // nothing to delete

	QStringList tmp;
	QList<StyleName> styles = item_->styles(false); // get list from cache
	for (int i = 0; i < styles.count(); ++i)
		tmp << styles[i].first;
	SMReplaceDia *dia = new SMReplaceDia(selected, tmp, this);
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
	if (!doc_)
		return;

	PrefsContext* dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
	QString wdir = dirs->get("editformats", ".");
	CustomFDialog dia(this, wdir, tr("Open"), tr("documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)"));
	if (dia.exec() == QDialog::Accepted)
	{
		QString selectedFile = dia.selectedFile();
		dirs->set("editformats", selectedFile.left(selectedFile.lastIndexOf("/")));

		StyleSet<ParagraphStyle> tmpParaStyles;
		StyleSet<CharStyle> tmpCharStyles;
		QMap<QString, multiLine> tmpLineStyles;

		doc_->loadStylesFromFile(selectedFile, &tmpParaStyles, &tmpCharStyles, &tmpLineStyles);

// FIXME Once all styles are derived from Style remove this and make a proper
//       implementation
// Start hack

		SMParagraphStyle *pstyle = 0;
		SMCharacterStyle *cstyle = 0;
		SMLineStyle      *lstyle = 0;
		for (int i = 0; i < items_.count(); ++i)
		{
			pstyle = dynamic_cast<SMParagraphStyle*>(items_.at(i));
			if (pstyle)
				break;
		}
		for (int i = 0; i < items_.count(); ++i)
		{
			cstyle = dynamic_cast<SMCharacterStyle*>(items_.at(i));
			if (cstyle)
				break;
		}
		for (int i = 0; i < items_.count(); ++i)
		{
			lstyle = dynamic_cast<SMLineStyle*>(items_.at(i));
			if (lstyle)
				break;
		}

		Q_ASSERT(pstyle && cstyle && lstyle);

		SMStyleImport *dia2 = new SMStyleImport(this, &tmpParaStyles, &tmpCharStyles, &tmpLineStyles);
// end hack

		QList<QPair<QString, QString> > selected;
		if (dia2->exec())
		{
			if (!isEditMode_)
				slotOk();
			QStringList neededColors;
			neededColors.clear();

			foreach (QString aStyle, dia2->paragraphStyles())
			{
				ParagraphStyle& sty(tmpParaStyles[tmpParaStyles.find(aStyle)]);
				if (dia2->clashRename())
				{
					sty.setName(pstyle->getUniqueName(sty.name()));
					pstyle->tmpStyles()->create(sty);
				}
				else
				{
					if (pstyle->tmpStyles()->find(sty.name()) >= 0)
						(*(pstyle->tmpStyles()))[pstyle->tmpStyles()->find(/*it.data()*/aStyle)] = sty;
					else
						pstyle->tmpStyles()->create(sty);
				}
				selected << QPair<QString, QString>(pstyle->typeName(), sty.name());
				if ((!doc_->PageColors.contains(sty.charStyle().strokeColor())) && (!neededColors.contains(sty.charStyle().strokeColor())))
					neededColors.append(sty.charStyle().strokeColor());
				if ((!doc_->PageColors.contains(sty.charStyle().fillColor())) && (!neededColors.contains(sty.charStyle().fillColor())))
					neededColors.append(sty.charStyle().fillColor());
			}

			foreach (QString aStyle, dia2->characterStyles())
			{
				CharStyle& sty(tmpCharStyles[tmpCharStyles.find(/*it.data()*/aStyle)]);
				if (dia2->clashRename())
				{
					sty.setName(cstyle->getUniqueName(sty.name()));
					cstyle->tmpStyles()->create(sty);
				}
				else
				{
					if (cstyle->tmpStyles()->find(sty.name()) >= 0)
						(*(cstyle->tmpStyles()))[cstyle->tmpStyles()->find(/*it.data()*/aStyle)] = sty;
					else
						cstyle->tmpStyles()->create(sty);
				}
				selected << QPair<QString, QString>(cstyle->typeName(), sty.name());
				if ((!doc_->PageColors.contains(sty.strokeColor())) && (!neededColors.contains(sty.strokeColor())))
					neededColors.append(sty.strokeColor());
				if ((!doc_->PageColors.contains(sty.fillColor())) && (!neededColors.contains(sty.fillColor())))
					neededColors.append(sty.fillColor());
			}

			foreach (QString aStyle, dia2->lineStyles())
			{
				multiLine &sty = tmpLineStyles[/*it.data()*/aStyle];
				QString styName = aStyle;

				if (dia2->clashRename())
					styName = lstyle->getUniqueName(aStyle);

				lstyle->tmpLines[styName] = sty;
				selected << QPair<QString, QString>(lstyle->typeName(), styName);

				for (int i = 0; i < sty.count(); ++i)
				{
					if ((!doc_->PageColors.contains(sty[i].Color)) && (!neededColors.contains(sty[i].Color)))
						neededColors.append(sty[i].Color);
				}
			}

			if (!neededColors.isEmpty())
			{
				FileLoader fl(selectedFile);
				if (fl.TestFile() == -1)
				{ //TODO put in nice user warning
					delete dia2;
					return;
				}
				ColorList LColors;
				if (fl.ReadColors(selectedFile, LColors))
				{
					ColorList::Iterator itc;
					for (itc = LColors.begin(); itc != LColors.end(); ++itc)
					{
						if (neededColors.contains(itc.key()))
							doc_->PageColors.insert(itc.key(), itc.value());
					}
				}
			}
		}
		delete dia2;
// Start hack part 2
		pstyle->currentDoc(doc_);
		cstyle->currentDoc(doc_);
// end hack part 2
		reloadStyleView(false);
		setSelection(selected);
		slotDirty();
		slotSetupWidget();
	}
	else
		return;
}

void StyleManager::setSelection(const QList<QPair<QString, QString> > &selected)
{
	styleView->clearSelection();
	
	QTreeWidgetItemIterator it(styleView, QTreeWidgetItemIterator::Selectable);
	StyleViewItem *item;

	while (*it)
	{
		item = dynamic_cast<StyleViewItem*>(*it);
		if (item)
		{
			for (int i = 0; i < selected.count(); ++i)
			{
				if (item->rootName() == selected[i].first && item->text(NAME_COL) == selected[i].second)
				{
					styleView->setCurrentItem(item);
					item->setSelected(true);
				}
			}
		}
		++it;
	}
}

void StyleManager::slotEdit()
{
	if (!isEditMode_)
		slotOk(); // switch to edit mode for cloning

	if (!rcStyle_.isNull())
	{
		QTreeWidgetItemIterator it(styleView);
		while (*it)
		{
			StyleViewItem *item = dynamic_cast<StyleViewItem*>(*it);
			if (item && !item->isRoot())
			{
				if (item->rootName() == styleClassesSP_[rcType_] &&
				    item->text(0) == rcStyle_)
				{
					styleView->setCurrentItem(item);
					item->setSelected(true);
					break;
				}
			}
			++it;
		}
		rcStyle_ = QString::null;
		rcType_ = QString::null;
	}
}


void StyleManager::slotClone()
{
	if (!isEditMode_)
		slotOk(); // switch to edit mode for cloning

	if (!rcStyle_.isNull())
	{
		QTreeWidgetItemIterator it(styleView);
		while (*it)
		{
			StyleViewItem *item = dynamic_cast<StyleViewItem*>(*it);
			if (item && !item->isRoot())
			{
				if (item->rootName() == styleClassesSP_[rcType_] &&
				    item->text(0) == rcStyle_)
				{
					styleView->setCurrentItem(item);
					item->setSelected(true);
					break;
				}
			}
			++it;
		}
		rcStyle_ = QString::null;
		rcType_ = QString::null;
	}

	QTreeWidgetItemIterator it(styleView, QTreeWidgetItemIterator::Selected);
	QList<QPair<QString, QString> > names;

	while (*it)
	{ // can't create styles here cause createNewStyle() alters the selection
		StyleViewItem *item = dynamic_cast<StyleViewItem*>(*it);
		if (item)
			names << QPair<QString, QString>(item->rootName(), item->text(NAME_COL));
		++it;
	}

	for (int i = 0; i < names.count(); ++i)
		createNewStyle(names[i].first, names[i].second);
}

void StyleManager::slotScrap()
{
	
}


void StyleManager::slotNew()
{
//#5334: Dont open into edit mdoe until user has selected a style type
// 	if (!isEditMode_)
// 		slotOk(); // switch to edit mode for a new style

	// TODO maybe there's something more clever for this
	newPopup_->exec(newButton->mapToGlobal(QPoint(0, newButton->height() + 2)));
}

void StyleManager::slotNewPopup(QAction *i)
{
	if (!isEditMode_)
		slotOk(); // switch to edit mode for a new style

	QString typeName = rcType_;
	if (typeName.isNull())
		typeName = i->text();
	else if (i->text().isNull())
		return;

	rcType_ = QString::null;
	rcStyle_ = QString::null;

	createNewStyle(typeName);
}

void StyleManager::slotNewPopup()
{
	slotNewPopup(rcpNewId_);
}

void StyleManager::slotRightClick(/*StyleViewItem *item, */const QPoint &point/*, int col*/)
{
	StyleViewItem *item = static_cast<StyleViewItem*>(styleView->currentItem());
	rcStyle_ = QString::null;
	rcType_ = QString::null;

	if (isEditMode_ && item) // make item the only selection if in edit mode
	{                        // default behaviour for right clicking is not to select the item
		styleView->clearSelection();
		styleView->setCurrentItem(item);
		item->setSelected(true);
	}

	if (item && !item->isRoot())
	{
		rightClickPopup_->removeAction(rcpNewId_);
		rcpNewId_ = rightClickPopup_->addAction( tr("New %1").arg(styleClassesPS_[item->rootName()]),
												this, SLOT(slotNewPopup()));
		rcpDeleteId_->setEnabled(true);
		rcpEditId_->setEnabled(true);
		rcpCloneId_->setEnabled(true);
		rcpToScrapId_->setEnabled(true);
		rcStyle_ = item->text(0);
		rcType_ = styleClassesPS_[item->rootName()];
		loadType(styleClassesPS_[item->rootName()]);
	}
	else if (item && item->isRoot())
	{
		rightClickPopup_->removeAction(rcpNewId_);
		rcpNewId_ = rightClickPopup_->addAction( tr("New %1").arg(styleClassesPS_[item->text(0)]),
												this, SLOT(slotNewPopup()));
		rcpDeleteId_->setEnabled(false);
		rcpEditId_->setEnabled(false);
		rcpCloneId_->setEnabled(false);
		rcpToScrapId_->setEnabled(false);
		rcType_ = styleClassesPS_[item->text(0)];
		loadType(rcType_);
	}
	else
	{
		rightClickPopup_->removeAction(rcpNewId_);
		rcpNewId_ = rightClickPopup_->addMenu(newPopup_);
		rcpNewId_->setText( tr("New"));
		rcpDeleteId_->setEnabled(false);
		rcpEditId_->setEnabled(false);
		rcpCloneId_->setEnabled(false);
		rcpToScrapId_->setEnabled(false);
	}

	rightClickPopup_->exec(styleView->mapToGlobal(point));
}

void StyleManager::slotDoubleClick(QTreeWidgetItem *item, /*const QPoint &point, */int col)
{
	rcStyle_ = QString::null;
	rcType_ = QString::null;

	if (isEditMode_ && item) // make item the only selection if in edit mode
	{
		styleView->clearSelection();
		styleView->setCurrentItem(item);
		item->setSelected(true);
		return; // work done, already in edit mode
	}

	StyleViewItem *sitem = dynamic_cast<StyleViewItem*>(item);
	if (sitem && !sitem->isRoot())
	{
		rcType_ = sitem->rootName();
		rcStyle_ = sitem->text(0);
		slotEdit(); // switch to edit mode
	}
	else if (sitem && sitem->isRoot())
	{
		//slotOk will recreate all items so sitem is invalid after that call
		QString itext = sitem->text(0);
		if (!isEditMode_)
			slotOk(); 
		createNewStyle(itext);
	}
	rcStyle_ = QString::null;
	rcType_ = QString::null;
}

void StyleManager::createNewStyle(const QString &typeName, const QString &fromParent)
{
	if (!doc_)
		return;
	loadType(typeName); // get the right style class
	Q_ASSERT(item_);
	QString newName = fromParent.isNull() ?
			item_->newStyle() : item_->newStyle(fromParent);
	if (newName.isNull())
		return;
	StyleViewItem *root = 0;
	QTreeWidgetItemIterator it(styleView, QTreeWidgetItemIterator::NotSelectable);
	while (*it)
	{
		StyleViewItem *item = dynamic_cast<StyleViewItem*>(*it);
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
	assert(root);
	styleView->clearSelection();
	StyleViewItem *newItem = new StyleViewItem(root, newName, item_->typeName());
	Q_CHECK_PTR(newItem);
	newItem->setDirty(true);
	styleView->setCurrentItem(newItem);
	newItem->setSelected(true);
	slotSetupWidget();
	nameEdit->setFocus();
	nameEdit->selectAll();
	applyButton->setEnabled(true);
	resetButton->setEnabled(true);
}

// open or close edit mode
void StyleManager::slotOk()
{
	static bool isFirst = true;
	if (isEditMode_)
	{
		disconnect(styleView, SIGNAL(itemSelectionChanged()), this, SLOT(slotSetupWidget()));
		slotApply();
		styleView->setSelectionMode(QAbstractItemView::MultiSelection);
		okButton->setText(QString("%1 >>").arg( tr("&Edit")));
		editFrame->hide();
		applyButton->hide();
		resetButton->hide();
		rightFrame->hide();
		isEditMode_ = false;
		for (int i = 0; i < items_.count(); ++i)
		{
			items_.at(i)->apply();
			items_.at(i)->editMode(false);
		}
		okButton->setToolTip( enterEditModeOk_);
		slotClean();
		slotDocSelectionChanged();

		if (!isFirst)
			move(editPosition_);
		prefs_->set("isEditMode", isEditMode_);
		connect(styleView, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
		        this, SLOT(slotApplyStyle(QTreeWidgetItem*,QTreeWidgetItem*)));
		connect(styleView, SIGNAL(itemClicked(QTreeWidgetItem*,int)),
				this, SLOT(slotApplyStyle(QTreeWidgetItem*,int)));
		if (isStoryEditMode_)
		{
			isStoryEditMode_=false;
			hide();
		}
	}
	else
	{
		disconnect(styleView, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
				   this, SLOT(slotApplyStyle(QTreeWidgetItem*,QTreeWidgetItem*)));
		disconnect(styleView, SIGNAL(itemClicked(QTreeWidgetItem*,int)),
				   this, SLOT(slotApplyStyle(QTreeWidgetItem*,int)));

		slotSetupWidget();
		styleView->setSelectionMode(QAbstractItemView::ExtendedSelection);
		editPosition_.setX(x());
		editPosition_.setY(y());
		prefs_->set("eX", x());
		prefs_->set("eY", y());
		editFrame->show();
		applyButton->show();
		resetButton->show();
		rightFrame->show();
		isEditMode_ = true;
		for (int i = 0; i < items_.count(); ++i)
			items_.at(i)->editMode(true);
		okButton->setToolTip( exitEditModeOk_);
		slotClean();

		prefs_->set("isEditMode", isEditMode_);
		connect(styleView, SIGNAL(itemSelectionChanged()), this, SLOT(slotSetupWidget()));
	}
	setOkButtonText();
	isFirst = false;

	resize(1, 1); // megahack to keep palette small
	updateGeometry();
	adjustSize();
}

void StyleManager::addNewType(StyleItem *item, bool loadFromDoc)
{
	if (item) {
		item_ = item;

		QList<StyleName> styles = item_->styles(loadFromDoc);
		StyleViewItem *rootItem = new StyleViewItem(styleView, item_->typeName());
		styleView->expandItem(rootItem);
		QMap<QString, StyleViewItem*> sitems;

		for (int i = 0; i < styles.count(); ++i) // set the list of styles of this type
		{
			StyleViewItem *sitem;
			if (styles[i].second.isNull())
			{
				sitem = new StyleViewItem(rootItem, styles[i].first, item_->typeName());
			}
			else if (sitems.contains(styles[i].second))
			{
				StyleViewItem *parent = sitems[styles[i].second];
				sitem = new StyleViewItem(parent, styles[i].first, item_->typeName());
				styleView->expandItem(parent);
			}
			else 
			{
				bool postpone = false;
				// search if parent is in remaing styles
				for (int j = i+1; j < styles.count(); ++j)
				{
					if (styles[j].first == styles[i].second)
					{
						styles.append(styles[i]); // postpone
						postpone = true;
					}
				}
				if (postpone)
					continue;
				else 
				{
					qDebug(QString("stylemanager: unknown parent '%1' of %2 style '%3'").arg(styles[i].second).arg(item_->typeName()).arg(styles[i].first).toLatin1().constData());
					sitem = new StyleViewItem(rootItem, styles[i].first, item_->typeName());
				}
			}
			
			sitems[styles[i].first] = sitem;
			sitem->setText(SHORTCUT_COL, item_->shortcut(sitem->text(NAME_COL)));
			
			QString key = sitem->rootName() + SEPARATOR + sitem->text(NAME_COL);
			if (styleActions_.contains(key))
				continue;

			styleActions_[key] =
				new ScrAction(ScrAction::DataQString, QPixmap(), QPixmap(), "",	sitem->text(SHORTCUT_COL), doc_->view(), 0, 0.0, key);
			connect(styleActions_[key], SIGNAL(triggeredData(QString)),
					this, SLOT(slotApplyStyle(QString)));
		}
	}
}

void StyleManager::slotDirty()
{
	QTreeWidgetItemIterator it(styleView, QTreeWidgetItemIterator::Selected);

	while (*it)
	{
		StyleViewItem *item = dynamic_cast<StyleViewItem*>(*it);
		if (item)
		{
			item->setDirty(true);
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
	QTreeWidgetItemIterator it(styleView);

	while (*it)
	{
		StyleViewItem *item = dynamic_cast<StyleViewItem*>(*it);
		if (item)
			item->setDirty(false);
		++it;
	}

	if (isEditMode_)
	{
		StyleItem *tmp = item_;

		for (int i = 0; i < items_.count(); ++i)
		{
			item_ = items_.at(i);
			reloadStyleView();
		}
		slotSetupWidget();
		item_ = tmp;
	}
	applyButton->setEnabled(false);
	resetButton->setEnabled(false);
	uniqueLabel->hide();
}

void StyleManager::reloadStyleView(bool loadFromDoc)
{
	QTreeWidgetItemIterator it(styleView, QTreeWidgetItemIterator::Selected);
	QList<QPair<QString, QString> > selected;

	while (*it)
	{
		StyleViewItem *item = dynamic_cast<StyleViewItem*>(*it);
		if (item)
			selected << QPair<QString, QString>(item->rootName(), item->text(NAME_COL));
		++it;
	}

	styleView->clear();
	if (item_ && loadFromDoc)
		item_->reload();

	for (int i = 0; i < items_.count(); ++i)
		addNewType(items_.at(i), false);

	QTreeWidgetItemIterator it2(styleView, QTreeWidgetItemIterator::Selectable);

	while (*it2)
	{
		StyleViewItem *item = dynamic_cast<StyleViewItem*>(*it2);
		if (item)
		{
			for (int i = 0; i < selected.count(); ++i)
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

	styleView->resizeColumnToContents(0);
// 	styleView->repaint();
}

void StyleManager::insertShortcutPage(QTabWidget *twidget)
{
	if (twidget)
	{
		if (!shortcutWidget_)
		{
			shortcutWidget_ = new ShortcutWidget(0);
			connect(shortcutWidget_, SIGNAL(newKey(const QString&)),
					this, SLOT(slotShortcutChanged(const QString&)));
		}
		twidget->addTab(shortcutWidget_, tr("Shortcut"));
	}
}

void StyleManager::slotNameChanged(const QString& name)
{
	if (item_ && !nameIsUnique(name))
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


	if (item_)
	{
		item_->nameChanged(name);
		updateActionName(styleView->currentItem()->text(NAME_COL), name);
		styleView->currentItem()->setText(NAME_COL, name);
		applyButton->setEnabled(true);
		resetButton->setEnabled(true);
	}
}


void StyleManager::updateActionName(const QString &oldName, const QString &newName)
{
	if (!item_)
		return;
	QString oldKey = item_->typeName() + SEPARATOR + oldName;
	QString newKey = item_->typeName() + SEPARATOR + newName;

	if (styleActions_.contains(oldKey))
	{
		ScrAction *a = styleActions_[oldKey];
		disconnect(a, SIGNAL(triggeredData(QString)), this, SLOT(slotApplyStyle(QString)));
		ScrAction *b = new ScrAction(ScrAction::DataQString, QPixmap(), QPixmap(), "",
			               a->shortcut(), doc_->view(), 0, 0.0, newKey);
		styleActions_.remove(oldKey);
		delete a;
		styleActions_[newKey] = b;
		connect(b, SIGNAL(triggeredData(QString)), this, SLOT(slotApplyStyle(QString)));
	}
}

void StyleManager::slotShortcutChanged(const QString& shortcut)
{
	if (!doc_)
		return;

	StyleViewItem *sitem = dynamic_cast<StyleViewItem*>(styleView->currentItem());
	if (!sitem)
		return;

	if (!shortcut.isNull() && shortcutExists(shortcut))
	{
		QMessageBox::information(this, CommonStrings::trWarning,
		                         tr("This key sequence is already in use"),
		                         CommonStrings::tr_OK);
		shortcutWidget_->setShortcut(item_->shortcut(sitem->text(NAME_COL)));
		return;
	}

	sitem->setText(SHORTCUT_COL, shortcut.isNull() ? "" : shortcut);
	QString key = sitem->rootName() + SEPARATOR + sitem->text(NAME_COL);
	if (styleActions_.contains(key))
		styleActions_[key]->setShortcut(shortcut);
	else
	{
		styleActions_[key] =
			new ScrAction(ScrAction::DataQString, QPixmap(), QPixmap(), "", shortcut, doc_->view(), 0, 0.0, key);
		connect(styleActions_[key], SIGNAL(triggeredData(QString)),
		        this, SLOT(slotApplyStyle(QString)));
	}

	if (item_)
		item_->setShortcut(shortcut);
}

bool StyleManager::shortcutExists(const QString &keys)
{
	QKeySequence key(keys);

	QMap<QString, QPointer<ScrAction> >::iterator it;
	for (it = styleActions_.begin(); it != styleActions_.end(); ++it)
	{
		if ((*it)->shortcut() == key)
			return true;
	}

	ApplicationPrefs *prefsData=&(PrefsManager::instance()->appPrefs);
	for (QMap<QString,Keys>::Iterator it=prefsData->KeyActions.begin();
	     it!=prefsData->KeyActions.end(); ++it)
	{
		if (key.matches(it.value().keySequence) != QKeySequence::NoMatch)
			return true;
	}

	return false;
}

void StyleManager::slotApplyStyle(QString keyString)
{
	if (isEditMode_)
		return;

	QStringList slist = keyString.split(SEPARATOR, QString::SkipEmptyParts);
	Q_ASSERT(slist.count() == 2);

	loadType(slist[0]);
	item_->toSelection(slist[1]);
	slotDocSelectionChanged();
}

bool StyleManager::nameIsUnique(const QString &name)
{
	QList<StyleName> names = item_->styles(false);
	for (int i = 0; i < names.count(); ++i)
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

	if (typeName.isNull() && widget_)
		widget_->setEnabled(false); // nothing selected or two or more different types
	else if (!item_ || item_->typeName() != typeName || widget_ != item_->widget())
		loadType(typeName); // new type selected
	else if (widget_ && !widget_->isEnabled())
		widget_->setEnabled(true);

	disconnect(nameEdit, SIGNAL(textChanged(const QString&)),
	           this, SLOT(slotNameChanged(const QString&)));
	if (!typeName.isNull())
	{
		item_->selected(selection.second);
		if (selection.second.count() > 1)
		{
			nameEdit->setText( tr("More than one style selected"));
			nameEdit->setEnabled(false);
			shortcutWidget_->setEnabled(false);
			shortcutWidget_->setShortcut(QString::null);
		}
		else
		{
			nameEdit->setText(selection.second[0]);
			nameEdit->setEnabled(true);
			shortcutWidget_->setEnabled(true);
			shortcutWidget_->setShortcut(item_->shortcut(selection.second[0]));
		}
	}
	else
	{
		nameEdit->setText("");
		nameEdit->setEnabled(false);
	}

	connect(nameEdit, SIGNAL(textChanged(const QString&)),
	        this, SLOT(slotNameChanged(const QString&)));

}

void StyleManager::slotApplyStyle(QTreeWidgetItem *item)
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

void StyleManager::slotApplyStyle(QTreeWidgetItem *newitem, QTreeWidgetItem *)
{
	slotApplyStyle(newitem);
}

void StyleManager::slotApplyStyle(QTreeWidgetItem *item, int)
{
	slotApplyStyle(item);
}

void StyleManager::slotDocSelectionChanged()
{
	if (isEditMode_)
		return; // don't track changes when in edit mode

	disconnect(styleView, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)),
	           this, SLOT(slotApplyStyle(QTreeWidgetItem*, QTreeWidgetItem*)));

	styleView->clearSelection();

	QList<QPair<QString, QString> > selected;

	for (int i = 0; i < items_.count(); ++i)
		selected << QPair<QString, QString>(items_.at(i)->typeName(), items_.at(i)->fromSelection());
	
	QTreeWidgetItemIterator it(styleView, QTreeWidgetItemIterator::Selectable);
	StyleViewItem *item;

	while (*it)
	{
		item = dynamic_cast<StyleViewItem*>(*it);
		if (item)
		{
			for (int i = 0; i < selected.count(); ++i)
			{
				if (item->rootName() == selected[i].first && item->text(NAME_COL) == selected[i].second)
				{
					styleView->setCurrentItem(item);
					item->setSelected(true);
				}
			}
		}
		++it;
	}

	connect(styleView, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
	        this, SLOT(slotApplyStyle(QTreeWidgetItem*,QTreeWidgetItem*)));
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
	QString typeName(QString::null);
	QStringList styleNames;

	if (rcStyle_.isNull())
	{
		QTreeWidgetItemIterator it(styleView, QTreeWidgetItemIterator::Selected);
		while (*it)
		{
			StyleViewItem *item = dynamic_cast<StyleViewItem*>(*it);
			if (!item)
			{
				++it;
				continue;
			}
			else if (typeName.isNull())
				typeName = item->rootName();
			else if (!typeName.isNull() && typeName != item->rootName())
			{
				typeName = QString::null;
				break; // two different types selected returning null
			}
	
			if (!item->isRoot())
				styleNames << item->text(NAME_COL);
	
			++it;
		}
	}
	else // right click selection which doesn't show in the gui
	{
		typeName = rcType_;
		styleNames << rcStyle_;
	}
	return QPair<QString, QStringList>(typeName, styleNames);
}

// sets the current type to name including item_ and the main widget
// for editing styles
void StyleManager::loadType(const QString &name)
{
	item_ = 0;
	for (int i = 0; i < items_.count(); ++i)
	{
		if (items_.at(i)->typeNameSingular() == name || items_.at(i)->typeName() == name)
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
		layout_->removeWidget(widget_);
//		widget_->reparent(0,0, QPoint(0,0), false);
		// show the widget for the new style type
		if (shortcutWidget_)
			widget_->removeTab(widget_->indexOf(shortcutWidget_));
	}
	widget_ = item_->widget(); // show the widget for the style type
	insertShortcutPage(widget_);
	widget_->setParent(mainFrame);
	layout_->addWidget(widget_, 0, 0);
	layout()->activate();
	widget_->resize(widget_->minimumSizeHint());
	widget_->show();
}

void StyleManager::hideEvent(QHideEvent *e)
{
	prefs_->set("eX", x());
	prefs_->set("eY", y());
	prefs_->set("isEditMode", isEditMode_);
	prefs_->set("InitX", x());
	prefs_->set("InitY", y());
	storeVisibility(false);
	storePosition();
	ScrPaletteBase::hideEvent(e);
	emit closed();
}

void StyleManager::closeEvent(QCloseEvent *e)
{
	prefs_->set("eX", x());
	prefs_->set("eY", y());
	prefs_->set("isEditMode", isEditMode_);
	prefs_->set("InitX", x());
	prefs_->set("InitY", y());
	storeVisibility(false);
	storePosition();
	ScrPaletteBase::closeEvent(e);
	emit closed();
}

void StyleManager::showEvent(QShowEvent *e)
{
	static bool isFirst = true;
	if (isModal())
	{
		isStoryEditMode_=true;
		applyButton->setEnabled(false);
		slotEdit();
	}
	setOkButtonText();
	ScrPaletteBase::showEvent(e);
	if (isFirst)
	{
		QPoint p(prefs_->getInt("InitX", x()), prefs_->getInt("InitY", y()));
		move(p);
		isFirst = false;
	}
}

StyleManager::~StyleManager()
{
	prefs_->set("eX", x());
	prefs_->set("eY", y());
	prefs_->set("isEditMode", isEditMode_);
	prefs_->set("InitX", x());
	prefs_->set("InitY", y());
	storeVisibility(this->isVisible());
	storePosition();
}
