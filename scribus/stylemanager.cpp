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
#include "smtextstyles.h"
#include "smlinestyle.h"
#include "styleitem.h"
#include "selection.h"
#include "prefsmanager.h"
#include "prefsfile.h"
#include "prefscontext.h"
#include "commonstrings.h"
#include "customfdialog.h"
#include "fileloader.h"
#include "smstyleimport.h"
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
#include <qmessagebox.h>

const QString StyleManager::SEPARATOR = "$$$$"; // dumb but it works

StyleManager::StyleManager(QWidget *parent, const char *name) : SMBase(parent, name), item_(0), widget_(0), shortcutWidget_(0), currentType_(QString::null), isEditMode_(true)
{
	splitter->setMinimumWidth(0);
	splitter->setResizeMode(leftFrame, QSplitter::KeepSize);
	splitter->setResizeMode(rightFrame, QSplitter::Stretch);
	uniqueLabel->hide();
	rightFrame->hide();
	svLayout = new QHBoxLayout(styleViewFrame);
	styleView = new StyleView(styleViewFrame);
	svLayout->addWidget(styleView);
	styleView->addColumn( tr("Name"));
	styleView->addColumn( tr("Shortcut"));
	styleView->setColumnWidthMode(NAME_COL, QListView::Maximum);
	styleView->setColumnWidthMode(SHORTCUT_COL, QListView::Maximum);
	styleView->setColumnWidth(NAME_COL, 0);
	styleView->setColumnWidth(SHORTCUT_COL, 0);
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
	editPosition_.setX(prefs_->getInt("eX", x()));
	editPosition_.setY(prefs_->getInt("eY", y()));

	width_ = prefs_->getInt("Width", 212);
	widthEm_ = prefs_->getInt("WidthEM", 500);
	height_ = prefs_->getInt("Height", 450);
	widthLeft_ = prefs_->getInt("WidthLeft", 192);
	widthRight_ = prefs_->getInt("WidthRight", 474);

	connect(newPopup_, SIGNAL(activated(int)), this, SLOT(slotNewPopup(int)));
	connect(okButton, SIGNAL(clicked()), this, SLOT(slotOk()));
	connect(importButton, SIGNAL(clicked()), this, SLOT(slotImport()));
	connect(resetButton, SIGNAL(clicked()), this, SLOT(slotClean()));
	connect(applyButton, SIGNAL(clicked()), this, SLOT(slotApply()));
	connect(deleteButton, SIGNAL(clicked()), this, SLOT(slotDelete()));
	connect(cloneButton, SIGNAL(clicked()), this, SLOT(slotClone()));
	connect(newButton, SIGNAL(clicked()), this, SLOT(slotNew()));
	connect(styleView, SIGNAL(rightButtonClicked(QListViewItem*, const QPoint&, int)),
			this, SLOT(slotRightClick(QListViewItem*, const QPoint&, int)));
	connect(styleView, SIGNAL(doubleClicked(QListViewItem*, const QPoint&, int)),
			this, SLOT(slotDoubleClick(QListViewItem*, const QPoint&, int)));

	languageChange();
	slotOk();
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

	QToolTip::add(nameEdit,     tr("Name of the selected style"));
	QToolTip::add(resetButton,  tr("Reset all changes"));
	QToolTip::add(applyButton,  tr("Apply all changes"));
	QToolTip::add(newButton,    tr("Create a new style"));
	QToolTip::add(importButton, tr("Import styles from another document"));
	QToolTip::add(cloneButton,  tr("Clone selected style"));
	QToolTip::add(deleteButton, tr("Delete selected styles"));

/***********************************/
/*      End Tooltips               */
/***********************************/

	nameLabel->setText( tr("Name:"));
	resetButton->setText( tr("&Reset"));
	applyButton->setText( tr("&Apply"));
	okButton->setText(isEditMode_ ? tr("<< &Done") : tr("&Edit >>"));
	newButton->setText( tr("&New"));
	importButton->setText( tr("&Import"));
	cloneButton->setText( tr("&Clone"));
	deleteButton->setText( tr("&Delete"));
	if (isEditMode_)
		QToolTip::add(okButton, exitEditModeOk_);
	else
		QToolTip::add(okButton, enterEditModeOk_);

	if (shortcutWidget_)
		shortcutWidget_->languageChange();

	newPopup_->clear();
	QStringList popupStrings;
	for (uint i = 0; i < items_.count(); ++i)
	{
		popupStrings << items_.at(i)->typeNameSingular();
		items_.at(i)->languageChange();
		styleClassesPS_[items_.at(i)->typeNamePlural()] = items_.at(i)->typeNameSingular();
		styleClassesSP_[items_.at(i)->typeNameSingular()] = items_.at(i)->typeNamePlural();
	}
	popupStrings.sort();
	for (uint i = 0; i < popupStrings.count(); ++i)
		newPopup_->insertItem(popupStrings[i]);

	styleView->clear();
	for (uint i = 0; i < items_.count(); ++i)
		addNewType(items_.at(i));

	rightClickPopup_->clear();
	rcpNewId_ = rightClickPopup_->insertItem( tr("New"), newPopup_);
	rightClickPopup_->insertItem( tr("Import"), this, SLOT(slotImport()));
	rightClickPopup_->insertSeparator();
	rcpEditId_ = rightClickPopup_->insertItem( tr("Edit"), this, SLOT(slotEdit()));
	rcpCloneId_ = rightClickPopup_->insertItem( tr("Clone"), this, SLOT(slotClone()));
	rcpToScrapId_ = rightClickPopup_->insertItem( tr("Send to Scrapbook"), this, SLOT(slotScrap()));
	rightClickPopup_->insertSeparator();
	rcpDeleteId_ = rightClickPopup_->insertItem( tr("Delete"), this, SLOT(slotDelete()));
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
		slotOk(); // switch to edit mode before deleting

	QStringList selected;

	if (rcStyle_ != QString::null)
		selected << rcStyle_;
	else
	{
		QListViewItemIterator it(styleView, QListViewItemIterator::Selected);
		while (it.current()) {
			selected << it.current()->text(0);
			++it;
		}
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
	if (!doc_)
		return;

	PrefsContext* dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
	QString wdir = dirs->get("editformats", ".");
	CustomFDialog dia(this, wdir, tr("Open"), tr("documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)"));
	if (dia.exec() == QDialog::Accepted)
	{
		QString selectedFile = dia.selectedFile();
		dirs->set("editformats", selectedFile.left(selectedFile.findRev("/")));

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
		for (uint i = 0; i < items_.count(); ++i)
		{
			pstyle = dynamic_cast<SMParagraphStyle*>(items_.at(i));
			if (pstyle)
				break;
		}
		for (uint i = 0; i < items_.count(); ++i)
		{
			cstyle = dynamic_cast<SMCharacterStyle*>(items_.at(i));
			if (cstyle)
				break;
		}
		for (uint i = 0; i < items_.count(); ++i)
		{
			lstyle = dynamic_cast<SMLineStyle*>(items_.at(i));
			if (lstyle)
				break;
		}

		Q_ASSERT(pstyle && cstyle && lstyle);

		ImportDialog *dia2 = new ImportDialog(this, &tmpParaStyles, &tmpCharStyles, &tmpLineStyles);
// end hack

		QValueList<QPair<QString, QString> > selected;
		if (dia2->exec())
		{
			if (!isEditMode_)
				slotOk();
			QStringList neededColors;
			neededColors.clear();
			QMap<QCheckListItem*, QString>::Iterator it;
			for (it = dia2->storedStyles.begin(); it != dia2->storedStyles.end(); ++it)
			{
				ParagraphStyle& sty(tmpParaStyles[tmpParaStyles.find(it.data())]);
				if (it.key()->isOn())
				{
					if (dia2->clashRename())
					{
						sty.setName(pstyle->getUniqueName(sty.name()));
						pstyle->tmpStyles()->create(sty);
					}
					else
					{
						if (pstyle->tmpStyles()->find(sty.name()) >= 0)
							(*(pstyle->tmpStyles()))[pstyle->tmpStyles()->find(it.data())] = sty;
						else
							pstyle->tmpStyles()->create(sty);
					}
					selected << QPair<QString, QString>(pstyle->typeName(), sty.name());
					if ((!doc_->PageColors.contains(sty.charStyle().strokeColor())) && (!neededColors.contains(sty.charStyle().strokeColor())))
						neededColors.append(sty.charStyle().strokeColor());
					if ((!doc_->PageColors.contains(sty.charStyle().fillColor())) && (!neededColors.contains(sty.charStyle().fillColor())))
						neededColors.append(sty.charStyle().fillColor());
				}
			}

			for (it = dia2->storedCharStyles.begin(); it != dia2->storedCharStyles.end(); ++it)
			{
				CharStyle& sty(tmpCharStyles[tmpCharStyles.find(it.data())]);
				if (it.key()->isOn())
				{
					if (dia2->clashRename())
					{
						sty.setName(cstyle->getUniqueName(sty.name()));
						cstyle->tmpStyles()->create(sty);
					}
					else
					{
						if (cstyle->tmpStyles()->find(sty.name()) >= 0)
							(*(cstyle->tmpStyles()))[cstyle->tmpStyles()->find(it.data())] = sty;
						else
							cstyle->tmpStyles()->create(sty);
					}
					selected << QPair<QString, QString>(cstyle->typeName(), sty.name());
					if ((!doc_->PageColors.contains(sty.strokeColor())) && (!neededColors.contains(sty.strokeColor())))
						neededColors.append(sty.strokeColor());
					if ((!doc_->PageColors.contains(sty.fillColor())) && (!neededColors.contains(sty.fillColor())))
						neededColors.append(sty.fillColor());
				}
			}

			for (it = dia2->storedLineStyles.begin(); it != dia2->storedLineStyles.end(); ++it)
			{
				multiLine &sty = tmpLineStyles[it.data()];
				QString styName = it.data();

				if (it.key()->isOn())
				{
					if (dia2->clashRename())
						styName = lstyle->getUniqueName(styName);

					lstyle->tmpLines[styName] = sty;
					selected << QPair<QString, QString>(lstyle->typeName(), styName);
					
					for (uint i = 0; i < sty.count(); ++i)
					{
						if ((!doc_->PageColors.contains(sty[i].Color)) && (!neededColors.contains(sty[i].Color)))
							neededColors.append(sty[i].Color);
					}
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
							doc_->PageColors.insert(itc.key(), itc.data());
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

void StyleManager::setSelection(const QValueList<QPair<QString, QString> > &selected)
{
	styleView->clearSelection();
	
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
}

void StyleManager::slotEdit()
{
	if (!isEditMode_)
		slotOk(); // switch to edit mode for cloning

	if (rcStyle_ != QString::null)
	{
		QListViewItemIterator it(styleView);
		while (it.current())
		{
			StyleViewItem *item = dynamic_cast<StyleViewItem*>(it.current());
			if (item && !item->isRoot())
			{
				if (item->rootName() == styleClassesSP_[rcType_] &&
				    item->text(0) == rcStyle_)
				{
					styleView->setCurrentItem(item);
					item->setSelected(true);
					item->repaint();
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

	if (rcStyle_ != QString::null)
	{
		QListViewItemIterator it(styleView);
		while (it.current())
		{
			StyleViewItem *item = dynamic_cast<StyleViewItem*>(it.current());
			if (item && !item->isRoot())
			{
				if (item->rootName() == styleClassesSP_[rcType_] &&
				    item->text(0) == rcStyle_)
				{
					styleView->setCurrentItem(item);
					item->setSelected(true);
					item->repaint();
					break;
				}
			}
			++it;
		}
		rcStyle_ = QString::null;
		rcType_ = QString::null;
	}

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

	QString typeName = rcType_;
	if (typeName == QString::null)
		typeName = newPopup_->text(i);
	else if (typeName == QString::null && i < 0)
		return; // nothing to create

	rcType_ = QString::null;
	rcStyle_ = QString::null;

	createNewStyle(typeName);
}

void StyleManager::slotRightClick(QListViewItem *item, const QPoint &point, int col)
{
	rcStyle_ = QString::null;
	rcType_ = QString::null;

	if (isEditMode_ && item) // make item the only selection if in edit mode
	{                        // default behaviour for right clicking is not to select the item
		styleView->clearSelection();
		styleView->setCurrentItem(item);
		item->setSelected(true);
		item->repaint();
	}

	StyleViewItem *sitem = dynamic_cast<StyleViewItem*>(item);
	if (sitem && !sitem->isRoot())
	{
		rightClickPopup_->removeItem(rcpNewId_);
		rcpNewId_ = rightClickPopup_->insertItem( tr("New %1").arg(styleClassesPS_[sitem->rootName()]),
		                                         this, SLOT(slotNewPopup(int)), 0, -1, 0);
		rightClickPopup_->setItemEnabled(rcpDeleteId_, true);
		rightClickPopup_->setItemEnabled(rcpEditId_, true);
		rightClickPopup_->setItemEnabled(rcpCloneId_, true);
		rightClickPopup_->setItemEnabled(rcpToScrapId_, true);
		rcStyle_ = sitem->text(0);
		rcType_ = styleClassesPS_[sitem->rootName()];
		loadType(styleClassesPS_[sitem->rootName()]);
	}
	else if (sitem && sitem->isRoot())
	{
		rightClickPopup_->removeItem(rcpNewId_);
		rcpNewId_ = rightClickPopup_->insertItem( tr("New %1").arg(styleClassesPS_[sitem->text(0)]),
		                                         this, SLOT(slotNewPopup(int)), 0, -1, 0);
		rightClickPopup_->setItemEnabled(rcpDeleteId_, false);
		rightClickPopup_->setItemEnabled(rcpEditId_, false);
		rightClickPopup_->setItemEnabled(rcpCloneId_, false);
		rightClickPopup_->setItemEnabled(rcpToScrapId_, false);
		rcType_ = styleClassesPS_[sitem->text(0)];
		loadType(rcType_);
	}
	else
	{
		rightClickPopup_->removeItem(rcpNewId_);
		rcpNewId_ = rightClickPopup_->insertItem( tr("New"), newPopup_, -1, 0);
		rightClickPopup_->setItemEnabled(rcpDeleteId_, false);
		rightClickPopup_->setItemEnabled(rcpEditId_, false);
		rightClickPopup_->setItemEnabled(rcpCloneId_, false);
		rightClickPopup_->setItemEnabled(rcpToScrapId_, false);
	}

	rightClickPopup_->exec(point);
}

void StyleManager::slotDoubleClick(QListViewItem *item, const QPoint &point, int col)
{
	rcStyle_ = QString::null;
	rcType_ = QString::null;

	if (isEditMode_ && item) // make item the only selection if in edit mode
	{
		styleView->clearSelection();
		styleView->setCurrentItem(item);
		item->setSelected(true);
		item->repaint();
		return; // work done, already in edit mode
	}

	StyleViewItem *sitem = dynamic_cast<StyleViewItem*>(item);
	if (sitem && !sitem->isRoot())
	{
		rcType_ = sitem->rootName();
		rcStyle_ = sitem->text(0);
		slotOk(); // switch to edit mode
	}
	else if (sitem && sitem->isRoot())
	{
		rcType_ = sitem->text(0);
		slotNewPopup(-1);
	}
	rcStyle_ = QString::null;
	rcType_ = QString::null;
}

void StyleManager::createNewStyle(const QString &typeName, const QString &fromParent)
{
	loadType(typeName); // get the right style class
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

// open or close edit mode
void StyleManager::slotOk()
{
	static bool isFirst = true;
	if (isEditMode_)
	{
		disconnect(styleView, SIGNAL(selectionChanged()), this, SLOT(slotSetupWidget()));
		widthEm_ = width();
		prefs_->set("WidthEM", widthEm_);
		height_ = height();
		prefs_->set("Height", height_);
		QValueList<int> l = splitter->sizes();
		widthLeft_ = l[0];
		widthRight_ = l[1];
		prefs_->set("WidthLeft", widthLeft_);
		prefs_->set("WidthRight", widthRight_);
		slotApply();
		styleView->setSelectionMode(QListView::Multi);
		okButton->setText(QString("%1 >>").arg( tr("&Edit")));
		editFrame->hide();
		applyButton->hide();
		resetButton->hide();
// 		editButtonsFrame->hide();
		rightFrame->hide();
		isEditMode_ = false;
		for (uint i = 0; i < items_.count(); ++i)
		{
			items_.at(i)->apply();
			items_.at(i)->editMode(false);
		}
		QToolTip::add(okButton, enterEditModeOk_);
		slotClean();
		slotDocSelectionChanged();
		adjustSize();
		resize(width_, height_);
		if (!isFirst)
			move(editPosition_);
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
		if (!isFirst)
		{
			width_ = width();
			prefs_->set("Width", width_);
		}
		height_ = height();
		prefs_->set("Height", height_);
		slotSetupWidget();
		styleView->setSelectionMode(QListView::Extended);
		editPosition_.setX(x());
		editPosition_.setY(y());
		prefs_->set("eX", x());
		prefs_->set("eY", y());
		okButton->setText(QString("<< %1").arg( tr("&Done")));
		editFrame->show();
		applyButton->show();
		resetButton->show();
// 		editButtonsFrame->show();
		rightFrame->show();
		isEditMode_ = true;
		for (uint i = 0; i < items_.count(); ++i)
			items_.at(i)->editMode(true);
		QToolTip::add(okButton, exitEditModeOk_);
		slotClean();
		adjustSize();
		QValueList<int> l;
		l << widthLeft_ << widthRight_;
		splitter->setSizes(l);
		resize(widthEm_, height_);
		prefs_->set("isEditMode", isEditMode_);
		connect(styleView, SIGNAL(selectionChanged()), this, SLOT(slotSetupWidget()));
	}
	isFirst = false;
}

void StyleManager::addNewType(StyleItem *item, bool loadFromDoc)
{
	if (item) {
		item_ = item;

		QValueList<StyleName> styles = item_->styles(loadFromDoc);
		StyleViewItem *rootItem = new StyleViewItem(styleView, item_->typeName());
		rootItem->setOpen(true);
		QMap<QString, StyleViewItem*> sitems;

		for (uint i = 0; i < styles.count(); ++i) // set the list of styles of this type
		{
			StyleViewItem *sitem;
			if (styles[i].second == QString::null)
			{
				sitem = new StyleViewItem(rootItem, styles[i].first, item_->typeName());
				sitems[styles[i].first] = sitem;
				sitem->setText(SHORTCUT_COL, item_->shortcut(sitem->text(NAME_COL)));
			}
			else
			{
				StyleViewItem *parent = 0;
				if (sitems.contains(styles[i].second))
					parent = sitems[styles[i].second];
				else
				{
					styles.append(styles[i]);
					continue;
				}

				sitem = new StyleViewItem(parent, styles[i].first, item_->typeName());
				sitems[styles[i].first] = sitem;
				sitem->setText(SHORTCUT_COL, item_->shortcut(sitem->text(NAME_COL)));
				parent->setOpen(true);
			}

			QString key = sitem->rootName() + SEPARATOR + sitem->text(NAME_COL);
			if (styleActions_.contains(key))
				continue;

			styleActions_[key] =
				new ScrAction(ScrAction::DataQString, QIconSet(), "",
							sitem->text(SHORTCUT_COL), doc_->view(), key, 0, 0.0, key);
			connect(styleActions_[key], SIGNAL(activatedData(QString)),
					this, SLOT(slotApplyStyle(QString)));
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

	if (isEditMode_)
	{
		StyleItem *tmp = item_;

		for (uint i = 0; i < items_.count(); ++i)
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


	if (item_)
	{
		item_->nameChanged(name);
		updateActionName(styleView->currentItem()->text(NAME_COL), name);
	}

	styleView->currentItem()->setText(NAME_COL, name);
	applyButton->setEnabled(true);
	resetButton->setEnabled(true);
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
		disconnect(a, SIGNAL(activatedData(QString)), this, SLOT(slotApplyStyle(QString)));
		ScrAction *b = new ScrAction(ScrAction::DataQString, QIconSet(), "",
			               a->accel(), doc_->view(), newKey, 0, 0.0, newKey);
		styleActions_.remove(oldKey);
		delete a;
		styleActions_[newKey] = b;
		connect(b, SIGNAL(activatedData(QString)), this, SLOT(slotApplyStyle(QString)));
	}
}

void StyleManager::slotShortcutChanged(const QString& shortcut)
{
	if (!doc_)
		return;

	StyleViewItem *sitem = dynamic_cast<StyleViewItem*>(styleView->currentItem());
	if (!sitem)
		return;

	if (shortcut != QString::null && shortcutExists(shortcut))
	{
		QMessageBox::information(this, CommonStrings::trWarning,
		                         tr("This key sequence is already in use"),
		                         CommonStrings::tr_OK);
		shortcutWidget_->setShortcut(item_->shortcut(sitem->text(NAME_COL)));
		return;
	}

	sitem->setText(SHORTCUT_COL, shortcut == QString::null ? "" : shortcut);
	QString key = sitem->rootName() + SEPARATOR + sitem->text(NAME_COL);
	if (styleActions_.contains(key))
		styleActions_[key]->setAccel(shortcut);
	else
	{
		styleActions_[key] =
			new ScrAction(ScrAction::DataQString, QIconSet(), "",
			              shortcut, doc_->view(), key, 0, 0.0, key);
		connect(styleActions_[key], SIGNAL(activatedData(QString)),
		        this, SLOT(slotApplyStyle(QString)));
	}

	if (item_)
		item_->setShortcut(shortcut);
}

bool StyleManager::shortcutExists(const QString &keys)
{
	QKeySequence key(keys);

	QMap<QString, QGuardedPtr<ScrAction> >::iterator it;
	for (it = styleActions_.begin(); it != styleActions_.end(); ++it)
	{
		if ((*it)->accel() == key)
			return true;
	}

	ApplicationPrefs *prefsData=&(PrefsManager::instance()->appPrefs);
	for (QMap<QString,Keys>::Iterator it=prefsData->KeyActions.begin();
	     it!=prefsData->KeyActions.end(); ++it)
	{
		if (key.matches(it.data().keySequence) != Qt::NoMatch)
			return true;
	}

	return false;
}

void StyleManager::slotApplyStyle(QString keyString)
{
	if (isEditMode_)
		return;

	QStringList slist = QStringList::split(SEPARATOR, keyString);
	Q_ASSERT(slist.count() == 2);

	loadType(slist[0]);
	item_->toSelection(slist[1]);
	slotDocSelectionChanged();
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

	if (rcStyle_ == QString::null)
	{
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
	for (uint i = 0; i < items_.count(); ++i)
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
	if (isEditMode_)
	{
		QValueList<int> l = splitter->sizes();
		widthLeft_ = l[0];
		widthRight_ = l[1];
		widthEm_ = width();
	}
	else
		width_ = width();

	height_ = height();
	prefs_->set("eX", x());
	prefs_->set("eY", y());
	prefs_->set("isEditMode", isEditMode_);
	prefs_->set("Width", width_);
	prefs_->set("WidthEM", widthEm_);
	prefs_->set("WidthLeft", widthLeft_);
	prefs_->set("WidthRight", widthRight_);
	prefs_->set("Height", height_);
	prefs_->set("InitX", x());
	prefs_->set("InitY", y());
	storeVisibility(false);
	storePosition();
	SMBase::hideEvent(e);
	emit closed();
}

void StyleManager::closeEvent(QCloseEvent *e)
{
	if (isEditMode_)
	{
		QValueList<int> l = splitter->sizes();
		widthLeft_ = l[0];
		widthRight_ = l[1];
		widthEm_ = width();
	}
	else
		width_ = width();

	height_ = height();
	prefs_->set("eX", x());
	prefs_->set("eY", y());
	prefs_->set("isEditMode", isEditMode_);
	prefs_->set("Width", width_);
	prefs_->set("WidthEM", widthEm_);
	prefs_->set("WidthLeft", widthLeft_);
	prefs_->set("WidthRight", widthRight_);
	prefs_->set("Height", height_);
	prefs_->set("InitX", x());
	prefs_->set("InitY", y());
	storeVisibility(false);
	storePosition();
	SMBase::closeEvent(e);
	emit closed();
}

void StyleManager::showEvent(QShowEvent *e)
{
	static bool isFirst = true;
	SMBase::showEvent(e);
	if (isFirst)
	{
		QPoint p(prefs_->getInt("InitX", x()), prefs_->getInt("InitY", y()));
		move(p);
		isFirst = false;
	}
}

StyleManager::~StyleManager()
{
	if (isEditMode_)
	{
		QValueList<int> l = splitter->sizes();
		widthLeft_ = l[0];
		widthRight_ = l[1];
		widthEm_ = width();
	}
	else
		width_ = width();

	height_ = height();
	prefs_->set("eX", x());
	prefs_->set("eY", y());
	prefs_->set("isEditMode", isEditMode_);
	prefs_->set("Width", width_);
	prefs_->set("WidthEM", widthEm_);
	prefs_->set("WidthLeft", widthLeft_);
	prefs_->set("WidthRight", widthRight_);
	prefs_->set("InitX", x());
	prefs_->set("InitY", y());
	prefs_->set("Height", height_);
	storeVisibility(this->isVisible());
	storePosition();
}



/*** StyleViewItem *******************************************************************/

StyleView::StyleView(QWidget *parent) : QListView(parent, "StyleView")
{

}

StyleView::~StyleView()
{

}

void StyleView::contentsMousePressEvent(QMouseEvent *e)
{
	if (e->button() == Qt::RightButton)
		emit rightButtonClicked(itemAt(e->pos()), e->globalPos(), 0);
	else
		QListView::contentsMousePressEvent(e);
}

void StyleView::contentsMouseDoubleClickEvent(QMouseEvent *e)
{
	if (e->button() == Qt::LeftButton)
		emit doubleClicked(itemAt(e->pos()), e->globalPos(), 0);
	else
		QListView::contentsMouseDoubleClickEvent(e);
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

	languageChange();

	keyGroupLayout->addMultiCellWidget( setKeyButton, 0, 2, 1, 1, Qt::AlignCenter );
	keyManagerLayout->addWidget( keyGroup );

	connect(noKey, SIGNAL(clicked()), this, SLOT(setNoKey()));
	connect(setKeyButton, SIGNAL(clicked()), this, SLOT(setKeyText()));
}

void ShortcutWidget::languageChange()
{
/***********************************/
/*      Begin Tooltips             */
/***********************************/

	QToolTip::add(noKey,        tr("No shortcut for the style")); // set no shortcut for this style
	QToolTip::add(userDef,      tr("Style has user defined shortcut")); // not sure what this thing does
	QToolTip::add(setKeyButton, tr("Assign a shortcut for the style")); // activate shorcut assigning

/***********************************/
/*      End Tooltips               */
/***********************************/

	noKey->setText( tr("&No Key"));
	userDef->setText( tr("&User Defined Key"));
	setKeyButton->setText( tr("Set &Key"));
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

void ShortcutWidget::setShortcut(const QString &shortcut)
{
	disconnect(noKey, SIGNAL(clicked()), this, SLOT(setNoKey()));
	disconnect(setKeyButton, SIGNAL(clicked()), this, SLOT(setKeyText()));

	setKeyButton->setOn(false);
	if (shortcut.length() > 0)
	{
		userDef->setChecked(true);
		keyDisplay->setText(shortcut);
	}
	else
	{
		noKey->setChecked(true);
		keyDisplay->setText("");
	}

	connect(noKey, SIGNAL(clicked()), this, SLOT(setNoKey()));
	connect(setKeyButton, SIGNAL(clicked()), this, SLOT(setKeyText()));
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
