/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QDebug>
#include <QEvent>
#include <QMenu>
#include <QToolTip>
#include <QMessageBox>

#include "commonstrings.h"
#include "fileloader.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "scraction.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "selection.h"
#include "shortcutwidget.h"
#include "smcellstyle.h"
#include "smlinestyle.h"
#include "smlinestylewidget.h"
#include "smreplacedia.h"
#include "smstyleimport.h"
#include "smtablestyle.h"
#include "smtextstyles.h"
#include "styleitem.h"
#include "stylemanager.h"
#include "ui/customfdialog.h"
#include "ui/scmessagebox.h"
#include "ui/styleview.h"

const QString StyleManager::SEPARATOR = "$$$$"; // dumb but it works

StyleManager::StyleManager(QWidget *parent, const char *name)
	: ScrPaletteBase(parent, name), m_item(0), m_widget(0),
	m_shortcutWidget(0), m_currentType(QString::null), m_isEditMode(true), m_doc(0)
{
	setupUi(this);
	styleView->hideColumn(SHORTCUT_COL);
	styleView->setSelectionMode(QAbstractItemView::ExtendedSelection);
	uniqueLabel->hide();
	rightFrame->hide();

	applyButton->setEnabled(false);
	resetButton->setEnabled(false);
	m_layout = new QGridLayout(mainFrame);
	m_newPopup = new QMenu(newButton);
	m_rightClickPopup = new QMenu(styleView);
	newButton->setMenu(m_newPopup);
	QString pname(name);
	if (pname.isEmpty())
		pname = "styleManager";
	m_prefs = PrefsManager::instance()->prefsFile->getContext(pname);
	m_isEditMode = true;
	m_isStoryEditMode = false;
	m_editPosition.setX(m_prefs->getInt("eX", x()));
	m_editPosition.setY(m_prefs->getInt("eY", y()));

	newButton->setEnabled(false);
	cloneButton->setEnabled(false);
	importButton->setEnabled(false);
	deleteButton->setEnabled(false);
	m_rightClickPopup->setEnabled(false);
	m_newPopup->setEnabled(false);
	
	m_selectedStyleAction = 0;

	connect(m_newPopup, SIGNAL(triggered(QAction*)), this, SLOT(slotNewPopup(QAction*)));
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
	setWindowTitle( tr( "Style Manager" ) );
/***********************************/
/*      Begin Tooltips             */
/***********************************/
// These are for general Style Manager widgets (not for c/pstyles except the name field

	// for the "<< Done" button when in edit mode
	m_exitEditModeOk = tr("Apply all changes and exit edit mode");
	// for the "Edit >>" button when not in edit mode
	m_enterEditModeOk= tr("Edit styles");

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
	m_doneText= tr("&Done");
	m_editText= tr("&Edit");
	setOkButtonText();
	newButton->setText( tr("&New"));
	importButton->setText( tr("&Import"));
	cloneButton->setText( tr("&Clone"));
	deleteButton->setText( tr("&Delete"));

	if (m_isEditMode)
		okButton->setToolTip( m_exitEditModeOk);
	else
		okButton->setToolTip( m_enterEditModeOk);

	if (m_shortcutWidget)
		m_shortcutWidget->languageChange();

	m_newPopup->clear();
	QStringList popupStrings;
	for (int i = 0; i < m_items.count(); ++i)
	{
		popupStrings << m_items.at(i)->typeNameSingular();
		m_items.at(i)->languageChange();
		m_styleClassesPS[m_items.at(i)->typeNamePlural()] = m_items.at(i)->typeNameSingular();
		m_styleClassesSP[m_items.at(i)->typeNameSingular()] = m_items.at(i)->typeNamePlural();
	}
	//12193 turn off sort
	//popupStrings.sort();
	for (int i = 0; i < popupStrings.count(); ++i)
	{
		// #13088 : some desktop environment, noticeably KDE Plasma will modify action text
		// on the fly to add shortcut keys/text mnemonics. So we have to store the popup
		// strings in the action data. See also https://bugs.kde.org/show_bug.cgi?id=337491
		QAction* newAction = m_newPopup->addAction(popupStrings[i]);
		newAction->setData(popupStrings[i]);
	}

	styleView->clear();
	for (int i = 0; i < m_items.count(); ++i)
		addNewType(m_items.at(i));
	styleView->resizeColumnToContents(0);

	m_rightClickPopup->clear();
	m_rcpNewId = m_rightClickPopup->addMenu(m_newPopup);
	m_rcpNewId->setText( tr("New"));
	m_rightClickPopup->addAction( tr("Import"), this, SLOT(slotImport()));
	m_rightClickPopup->addSeparator();
	m_rcpEditId = m_rightClickPopup->addAction( tr("Edit"), this, SLOT(slotEdit()));
	m_rcpCloneId = m_rightClickPopup->addAction( tr("Clone"), this, SLOT(slotClone()));
//	m_rcpToScrapId = m_rightClickPopup->addAction( tr("Send to Scrapbook"), this, SLOT(slotScrap()));
	m_rightClickPopup->addSeparator();
	m_rcpDeleteId = m_rightClickPopup->addAction( tr("Delete"), this, SLOT(slotDelete()));
}

void StyleManager::unitChange()
{
	if (m_doc)
	{
		for (int i = 0; i < m_items.count(); ++i)
			m_items.at(i)->unitChange();

		slotSetupWidget();
	}
}

template<class ItemType> 
ItemType* StyleManager::item()
{
	for (int i = 0; i < m_items.count(); ++i)
	{
		StyleItem* item = m_items[i];
		ItemType*  typedItem = dynamic_cast<ItemType*>(item);
		if (typedItem)
			return typedItem;
	}
	return 0;
}

void StyleManager::setOkButtonText()
{
	if (!m_isStoryEditMode)
		okButton->setText(m_isEditMode ? "<< " + m_doneText : m_editText + " >>");
	else
		okButton->setText(CommonStrings::tr_OK);
}

void StyleManager::setDoc(ScribusDoc *doc)
{
	ScribusDoc* oldDoc = m_doc;
	bool hasDoc = (doc != NULL);
	if (m_doc && (m_doc != doc))
		disconnect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(slotDocSelectionChanged()));
	m_doc = doc;
	newButton->setEnabled(hasDoc);
	cloneButton->setEnabled(hasDoc);
	importButton->setEnabled(hasDoc);
	deleteButton->setEnabled(hasDoc);
	m_rightClickPopup->setEnabled(hasDoc);
	m_newPopup->setEnabled(hasDoc);
	if (m_doc && (m_doc != oldDoc) && this->isVisible())
		connect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(slotDocSelectionChanged()));

	// clear the style list and reload from new doc
	styleView->clear();
	if (m_selectedStyleAction)
	{
		m_rightClickPopup->removeAction(m_selectedStyleAction);
		m_selectedStyleAction = 0;
	}
	m_styleActions.clear();
	for (int i = 0; i < m_items.count(); ++i)
	{
		m_items.at(i)->setCurrentDoc(doc);
		addNewType(m_items.at(i)); // forces a reload
		if (m_doc)
			m_items.at(i)->unitChange();
	}
	styleView->resizeColumnToContents(0);
}

void StyleManager::updateColorList()
{
	for (int i = 0; i < m_items.count(); ++i)
	{
		m_items.at(i)->setCurrentDoc(m_doc);
		m_items.at(i)->reload();
	}
}

void StyleManager::addStyle(StyleItem *item)
{
	m_items.append(item);
	addNewType(item);
	languageChange(); // upgrade the popup menu with the new item
	connect(item, SIGNAL(selectionDirty()), this, SLOT(slotDirty()));
}

void StyleManager::slotApply()
{
	if (applyButton->isEnabled())
	{
		// #13390 : we have to proceed with some order here
		SMCharacterStyle* charStyleItem = this->item<SMCharacterStyle>();
		if (charStyleItem)
			charStyleItem->apply();
		SMParagraphStyle* paraStyleItem = this->item<SMParagraphStyle>();
		if (paraStyleItem)
			paraStyleItem->apply();
		SMCellStyle* cellStyleItem = this->item<SMCellStyle>();
		if (cellStyleItem)
			cellStyleItem->apply();
		SMTableStyle* tableStyleItem = this->item<SMTableStyle>();
		if (tableStyleItem)
			tableStyleItem->apply();

		for (int i = 0; i < m_items.count(); ++i)
		{
			StyleItem* item = m_items.at(i);
			if (item == charStyleItem || item == paraStyleItem)
				continue;
			if (item == cellStyleItem || item == tableStyleItem)
				continue;
			m_items.at(i)->apply();
		}
		if (m_doc)
			m_doc->view()->DrawNew();
	}
	slotClean();
}

void StyleManager::slotDelete()
{
	if (!m_isEditMode)
		slotOk(); // switch to edit mode before deleting

	QStringList selected;

	if (!m_rcStyle.isNull())
		selected << m_rcStyle;
	else
	{
		QTreeWidgetItemIterator it(styleView, QTreeWidgetItemIterator::Selected);
		while (*it)
		{
			selected << (*it)->text(0);
			++it;
		}
	}
	if (!m_item || selected.isEmpty())
		return; // nothing to delete

	QStringList tmp;
	QList<StyleName> styles = m_item->styles(false); // get list from cache
	for (int i = 0; i < styles.count(); ++i)
		tmp << styles[i].first;
	SMReplaceDia dia(selected, tmp, this);
	if (dia.exec() && m_item)
	{
		m_item->deleteStyles(dia.items());
		applyButton->setEnabled(true);
		resetButton->setEnabled(true);
		reloadStyleView(false);
		slotOk();
	}
}


void StyleManager::slotImport()
{
	if (!m_doc)
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
		QHash<QString, multiLine> tmpLineStyles;

		m_doc->loadStylesFromFile(selectedFile, &tmpParaStyles, &tmpCharStyles, &tmpLineStyles);

// FIXME Once all styles are derived from Style remove this and make a proper
//       implementation
// Start hack

		SMParagraphStyle *pstyle = 0;
		SMCharacterStyle *cstyle = 0;
		SMLineStyle      *lstyle = 0;
		for (int i = 0; i < m_items.count(); ++i)
		{
			pstyle = dynamic_cast<SMParagraphStyle*>(m_items.at(i));
			if (pstyle)
				break;
		}
		for (int i = 0; i < m_items.count(); ++i)
		{
			cstyle = dynamic_cast<SMCharacterStyle*>(m_items.at(i));
			if (cstyle)
				break;
		}
		for (int i = 0; i < m_items.count(); ++i)
		{
			lstyle = dynamic_cast<SMLineStyle*>(m_items.at(i));
			if (lstyle)
				break;
		}

		Q_ASSERT(pstyle && cstyle && lstyle);

		SMStyleImport *dia2 = new SMStyleImport(this, &tmpParaStyles, &tmpCharStyles, &tmpLineStyles);
// end hack

//#7315 		QList<QPair<QString, QString> > selected;
		if (dia2->exec())
		{
//#7385 			if (!m_isEditMode)
//#7385 				slotOk();
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
//#7315 				selected << QPair<QString, QString>(pstyle->typeName(), sty.name());
				if ((!m_doc->PageColors.contains(sty.charStyle().strokeColor())) && (!neededColors.contains(sty.charStyle().strokeColor())))
					neededColors.append(sty.charStyle().strokeColor());
				if ((!m_doc->PageColors.contains(sty.charStyle().fillColor())) && (!neededColors.contains(sty.charStyle().fillColor())))
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
//#7315 				selected << QPair<QString, QString>(cstyle->typeName(), sty.name());
				if ((!m_doc->PageColors.contains(sty.strokeColor())) && (!neededColors.contains(sty.strokeColor())))
					neededColors.append(sty.strokeColor());
				if ((!m_doc->PageColors.contains(sty.fillColor())) && (!neededColors.contains(sty.fillColor())))
					neededColors.append(sty.fillColor());
			}

			foreach (QString aStyle, dia2->lineStyles())
			{
				multiLine &sty = tmpLineStyles[/*it.data()*/aStyle];
				QString styName = aStyle;

				if (dia2->clashRename())
					styName = lstyle->getUniqueName(aStyle);

				lstyle->m_tmpLines[styName] = sty;
//#7315 				selected << QPair<QString, QString>(lstyle->typeName(), styName);

				for (int i = 0; i < sty.count(); ++i)
				{
					if ((!m_doc->PageColors.contains(sty[i].Color)) && (!neededColors.contains(sty[i].Color)))
						neededColors.append(sty[i].Color);
				}
			}

			if (!neededColors.isEmpty())
			{
				FileLoader fl(selectedFile);
				if (fl.testFile() == -1)
				{ //TODO put in nice user warning
					delete dia2;
					return;
				}
				ColorList LColors;
				if (fl.readColors(LColors))
				{
					ColorList::Iterator itc;
					for (itc = LColors.begin(); itc != LColors.end(); ++itc)
					{
						if (neededColors.contains(itc.key()))
							m_doc->PageColors.insert(itc.key(), itc.value());
					}
				}
			}
		}
		delete dia2;
// Start hack part 2
		pstyle->setCurrentDoc(m_doc);
		cstyle->setCurrentDoc(m_doc);
// end hack part 2
		reloadStyleView(false);
//#7315 		setSelection(selected);
		slotDirty();
//#7315		slotSetupWidget();
		slotApply();//#7315
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
	if (!m_isEditMode)
		slotOk(); // switch to edit mode for cloning

	if (!m_rcStyle.isNull())
	{
		QTreeWidgetItemIterator it(styleView);
		while (*it)
		{
			StyleViewItem *item = dynamic_cast<StyleViewItem*>(*it);
			if (item && !item->isRoot())
			{
				if (item->rootName() == m_styleClassesSP[m_rcType] &&
				    item->text(0) == m_rcStyle)
				{
					styleView->setCurrentItem(item);
					item->setSelected(true);
					break;
				}
			}
			++it;
		}
		m_rcStyle = QString::null;
		m_rcType = QString::null;
	}
}


void StyleManager::slotClone()
{
	if (!m_isEditMode)
		slotOk(); // switch to edit mode for cloning

	if (!m_rcStyle.isNull())
	{
		QTreeWidgetItemIterator it(styleView);
		while (*it)
		{
			StyleViewItem *item = dynamic_cast<StyleViewItem*>(*it);
			if (item && !item->isRoot())
			{
				if (item->rootName() == m_styleClassesSP[m_rcType] &&
				    item->text(0) == m_rcStyle)
				{
					styleView->setCurrentItem(item);
					item->setSelected(true);
					break;
				}
			}
			++it;
		}
		m_rcStyle = QString::null;
		m_rcType = QString::null;
	}

	QTreeWidgetItemIterator it(styleView, QTreeWidgetItemIterator::Selected);
	QList<QPair<QString, QString> > names;

	while (*it)
	{ // can't create styles here cause createNewStyle() alters the selection
		StyleViewItem *item = dynamic_cast<StyleViewItem*>(*it);
		if (item && !item->isRoot())
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
//#5334: Don't open into edit mdoe until user has selected a style type
// 	if (!m_isEditMode)
// 		slotOk(); // switch to edit mode for a new style

	// TODO maybe there's something more clever for this
	m_newPopup->exec(newButton->mapToGlobal(QPoint(0, newButton->height() + 2)));
}

void StyleManager::slotNewPopup(QAction *action)
{
	if (!m_isEditMode)
		slotOk(); // switch to edit mode for a new style

	QString typeName = m_rcType;
	QString actionType = action->data().toString();
	if (actionType.isEmpty())
		actionType = action->text();
	if (typeName.isEmpty())
		typeName = actionType;
	else if (actionType.isEmpty())
		return;

	m_rcType = QString::null;
	m_rcStyle = QString::null;

	createNewStyle(typeName);
}

void StyleManager::slotNewPopup()
{
	slotNewPopup(m_rcpNewId);
}

void StyleManager::slotRightClick(/*StyleViewItem *item, */const QPoint &point/*, int col*/)
{
	StyleViewItem *item = static_cast<StyleViewItem*>(styleView->currentItem());
	m_rcStyle = QString::null;
	m_rcType = QString::null;

	if (m_isEditMode && item) // make item the only selection if in edit mode
	{                        // default behaviour for right clicking is not to select the item
		styleView->clearSelection();
		styleView->setCurrentItem(item);
		item->setSelected(true);
	}

	if (item && !item->isRoot())
	{
		m_rightClickPopup->removeAction(m_rcpNewId);
		m_rcpNewId = m_rightClickPopup->addAction( tr("New %1").arg(m_styleClassesPS[item->rootName()]),
												this, SLOT(slotNewPopup()));
		m_rcpDeleteId->setEnabled(true);
		m_rcpEditId->setEnabled(true);
		m_rcpCloneId->setEnabled(true);
//		m_rcpToScrapId->setEnabled(true);
		m_rcStyle = item->text(0);
		m_rcType = m_styleClassesPS[item->rootName()];
		loadType(m_styleClassesPS[item->rootName()]);
		

		// Add "Apply" menu entry
		if (m_selectedStyleAction)
		{
			m_rightClickPopup->removeAction(m_selectedStyleAction);
			m_selectedStyleAction = 0;
		}
		if (styleView->selectedItems().count() == 1)
		{
			QString key = item->rootName() + SEPARATOR + item->text(NAME_COL);
			if (m_styleActions.contains(key))
			{
				m_selectedStyleAction = m_styleActions[key];
				if (m_selectedStyleAction)
				{
					m_rightClickPopup->insertAction(m_rightClickPopup->actions().first(), m_selectedStyleAction);
				}
			}
		}
		
	}
	else if (item && item->isRoot())
	{
		m_rightClickPopup->removeAction(m_rcpNewId);
		m_rcpNewId = m_rightClickPopup->addAction( tr("New %1").arg(m_styleClassesPS[item->text(0)]),
												this, SLOT(slotNewPopup()));
		m_rcpDeleteId->setEnabled(false);
		m_rcpEditId->setEnabled(false);
		m_rcpCloneId->setEnabled(false);
//		m_rcpToScrapId->setEnabled(false);
		m_rcType = m_styleClassesPS[item->text(0)];
		loadType(m_rcType);
		
		if (m_selectedStyleAction)
		{
			m_rightClickPopup->removeAction(m_selectedStyleAction);
			m_selectedStyleAction = 0;
		}
	}
	else
	{
		m_rightClickPopup->removeAction(m_rcpNewId);
		m_rcpNewId = m_rightClickPopup->addMenu(m_newPopup);
		m_rcpNewId->setText( tr("New"));
		m_rcpDeleteId->setEnabled(false);
		m_rcpEditId->setEnabled(false);
		m_rcpCloneId->setEnabled(false);
//		m_rcpToScrapId->setEnabled(false);
		
		if (m_selectedStyleAction)
		{
			m_rightClickPopup->removeAction(m_selectedStyleAction);
			m_selectedStyleAction = 0;
		}
	}

	m_rightClickPopup->exec(styleView->mapToGlobal(point));
}

void StyleManager::slotDoubleClick(QTreeWidgetItem *item, /*const QPoint &point, */int col)
{
	m_rcStyle = QString::null;
	m_rcType = QString::null;

	if (m_isEditMode && item) // make item the only selection if in edit mode
	{
		styleView->clearSelection();
		styleView->setCurrentItem(item);
		item->setSelected(true);
		return; // work done, already in edit mode
	}

	StyleViewItem *sitem = dynamic_cast<StyleViewItem*>(item);
	if (sitem && !sitem->isRoot())
	{
		m_rcType  = m_styleClassesPS[sitem->rootName()];
		m_rcStyle = sitem->text(0);
		slotEdit(); // switch to edit mode
	}
	else if (sitem && sitem->isRoot())
	{
		//slotOk will recreate all items so sitem is invalid after that call
		QString itext = sitem->text(0);
		if (!m_isEditMode)
			slotOk(); 
		createNewStyle(itext);
	}
	m_rcStyle = QString::null;
	m_rcType = QString::null;
}

void StyleManager::createNewStyle(const QString &typeName, const QString &fromParent)
{
	if (!m_doc)
		return;
	loadType(typeName); // get the right style class
	Q_ASSERT(m_item);
	
	QString newName = fromParent.isNull() ?
			m_item->newStyle() : m_item->newStyle(fromParent);
// 	qDebug() << "created new style:" << newName << " : " << m_item->isDefaultStyle(newName);
	if (newName.isNull())
		return;
	StyleViewItem *root = 0;
	QTreeWidgetItemIterator it(styleView, QTreeWidgetItemIterator::NotSelectable);
	while (*it)
	{
		StyleViewItem *item = dynamic_cast<StyleViewItem*>(*it);
		if (item)
		{
			if (item->text(NAME_COL) == m_item->typeName())
			{
				root = item;
				break;
			}
		}
		++it;
	}
	assert(root);
	if (!fromParent.isEmpty())
	{
		QTreeWidgetItemIterator it(root, QTreeWidgetItemIterator::Selectable);
		while (*it)
		{
			StyleViewItem *item = dynamic_cast<StyleViewItem*>(*it);
			if (item && item->text(NAME_COL) == fromParent)
			{
				StyleViewItem *parent = dynamic_cast<StyleViewItem*>((*it)->parent());
				if (parent)
				{
					root = parent;
					break;
				}
			}
			++it;
		}
	}
	styleView->clearSelection();
	StyleViewItem *newItem = new StyleViewItem(root, newName, m_item->typeName());
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
	if (m_isEditMode)
	{
		disconnect(styleView, SIGNAL(itemSelectionChanged()), this, SLOT(slotSetupWidget()));
		slotApply();

		okButton->setText(QString("%1 >>").arg( tr("&Edit")));
		editFrame->hide();
		applyButton->hide();
		resetButton->hide();
		rightFrame->hide();
		m_isEditMode = false;
		for (int i = 0; i < m_items.count(); ++i)
		{
			m_items.at(i)->apply();
			m_items.at(i)->editMode(false);
		}
		okButton->setToolTip( m_enterEditModeOk);
		slotClean();
		slotDocSelectionChanged();

		if (!isFirst)
			move(m_editPosition);
		m_prefs->set("isEditMode", m_isEditMode);
// 		qDebug() <<"CONNECT slotOK";
// 		connect(styleView, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
// 		        this, SLOT(slotApplyStyle(QTreeWidgetItem*,QTreeWidgetItem*)));
// 		connect(styleView, SIGNAL(itemClicked(QTreeWidgetItem*,int)),
// 				this, SLOT(slotApplyStyle(QTreeWidgetItem*,int)));
		if (m_isStoryEditMode)
		{
			m_isStoryEditMode=false;
			hide();
		}
	}
	else
	{
// 		qDebug() <<"DISCONNECT slotOK";
// 		disconnect(styleView, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
// 				   this, SLOT(slotApplyStyle(QTreeWidgetItem*,QTreeWidgetItem*)));
// 		disconnect(styleView, SIGNAL(itemClicked(QTreeWidgetItem*,int)),
// 				   this, SLOT(slotApplyStyle(QTreeWidgetItem*,int)));

		slotSetupWidget();

		m_editPosition.setX(x());
		m_editPosition.setY(y());
		m_prefs->set("eX", x());
		m_prefs->set("eY", y());
		editFrame->show();
		applyButton->show();
		resetButton->show();
		rightFrame->show();
		m_isEditMode = true;
		for (int i = 0; i < m_items.count(); ++i)
			m_items.at(i)->editMode(true);
		okButton->setToolTip( m_exitEditModeOk);
		slotClean();

		m_prefs->set("isEditMode", m_isEditMode);
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
		m_item = item;

		QList<StyleName> styles = m_item->styles(loadFromDoc);
		StyleViewItem *rootItem = new StyleViewItem(styleView, m_item->typeName());
		styleView->expandItem(rootItem);
		QMap<QString, StyleViewItem*> sitems;

		for (int i = 0; i < styles.count(); ++i) // set the list of styles of this type
		{
			StyleViewItem *sitem;
			if (styles[i].second.isNull())
			{
				sitem = new StyleViewItem(rootItem, styles[i].first, m_item->typeName());
			}
			else if (sitems.contains(styles[i].second))
			{
				StyleViewItem *parent = sitems[styles[i].second];
				sitem = new StyleViewItem(parent, styles[i].first, m_item->typeName());
				styleView->expandItem(parent);
			}
			else 
			{
				bool postpone = false;
				// search if parent is in remaining styles
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
					qDebug() << QString("stylemanager: unknown parent '%1' of %2 style '%3'").arg(styles[i].second).arg(m_item->typeName()).arg(styles[i].first);
					sitem = new StyleViewItem(rootItem, styles[i].first, m_item->typeName());
				}
			}
			
			sitems[styles[i].first] = sitem;
			QString shortcutValue(m_item->shortcut(sitem->text(NAME_COL)));
			sitem->setText(SHORTCUT_COL, shortcutValue);
			
			QString key = sitem->rootName() + SEPARATOR + sitem->text(NAME_COL);
			if (m_styleActions.contains(key))
				continue;

			m_styleActions[key] = new ScrAction(ScrAction::DataQString, QPixmap(), QPixmap(), tr("&Apply"), shortcutValue, m_doc->view(), key);
			connect(m_styleActions[key], SIGNAL(triggeredData(QString)), this, SLOT(slotApplyStyle(QString)));
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

	if (m_isEditMode)
	{
		StyleItem *tmp = m_item;

		for (int i = 0; i < m_items.count(); ++i)
		{
			m_item = m_items.at(i);
			reloadStyleView();
		}
		slotSetupWidget();
		m_item = tmp;
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
	if (m_item && loadFromDoc)
		m_item->reload();

	for (int i = 0; i < m_items.count(); ++i)
		addNewType(m_items.at(i), false);

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

//#7318 Do not auto resize vertically 	styleView->resizeColumnToContents(0);
// 	styleView->repaint();
}

void StyleManager::insertShortcutPage(QTabWidget *twidget)
{
	if (twidget)
	{
		if (!m_shortcutWidget)
		{
			m_shortcutWidget = new ShortcutWidget(0);
			m_shortcutWidget->setAllowedModifiers(Qt::META|Qt::CTRL|Qt::SHIFT|Qt::ALT,0);
			connect(m_shortcutWidget, SIGNAL(newKey(const QString&)),
					this, SLOT(slotShortcutChanged(const QString&)));
		}
		twidget->addTab(m_shortcutWidget, tr("Shortcut"));
	}
}

void StyleManager::slotNameChanged(const QString& name)
{
	if (m_item && name.isEmpty())
	{
		okButton->setEnabled(false);
		applyButton->setEnabled(false);
		return;
	}
	if (m_item && !nameIsUnique(name))
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

	if (m_item)
	{
		m_item->nameChanged(name);
		updateActionName(styleView->currentItem()->text(NAME_COL), name);
		styleView->currentItem()->setText(NAME_COL, name);
		applyButton->setEnabled(true);
		resetButton->setEnabled(true);
	}
}


void StyleManager::updateActionName(const QString &oldName, const QString &newName)
{
	if (!m_item)
		return;
	QString oldKey = m_item->typeName() + SEPARATOR + oldName;
	QString newKey = m_item->typeName() + SEPARATOR + newName;

	if (m_styleActions.contains(oldKey))
	{
		ScrAction *a = m_styleActions[oldKey];
		disconnect(a, SIGNAL(triggeredData(QString)), this, SLOT(slotApplyStyle(QString)));
		ScrAction *b = new ScrAction(ScrAction::DataQString, QPixmap(), QPixmap(), tr("&Apply"),
						   a->shortcut(), m_doc->view(), newKey);
		m_styleActions.remove(oldKey);
		if (m_selectedStyleAction == a)
		{
			m_rightClickPopup->removeAction(m_selectedStyleAction);
			m_selectedStyleAction = 0;
		}
		delete a;
		m_styleActions[newKey] = b;
		connect(b, SIGNAL(triggeredData(QString)), this, SLOT(slotApplyStyle(QString)));
	}
}

void StyleManager::slotShortcutChanged(const QString& shortcut)
{
	if (!m_doc)
		return;

	StyleViewItem *sitem = dynamic_cast<StyleViewItem*>(styleView->currentItem());
	if (!sitem)
		return;

	if (!shortcut.isNull() && shortcutExists(shortcut))
	{
		ScMessageBox::information(this, CommonStrings::trWarning,
		                         tr("This key sequence is already in use"));
		if (m_shortcutWidget)
			m_shortcutWidget->setShortcut(m_item->shortcut(sitem->text(NAME_COL)));
		return;
	}

	sitem->setText(SHORTCUT_COL, shortcut.isNull() ? "" : shortcut);
	QString key = sitem->rootName() + SEPARATOR + sitem->text(NAME_COL);
	if (m_styleActions.contains(key))
		m_styleActions[key]->setShortcut(shortcut);
	else
	{
		m_styleActions[key] = new ScrAction(ScrAction::DataQString, QPixmap(), QPixmap(), tr("&Apply"), shortcut, m_doc->view(), key);
		connect(m_styleActions[key], SIGNAL(triggeredData(QString)), this, SLOT(slotApplyStyle(QString)));
	}

	if (m_item)
		m_item->setShortcut(shortcut);
}

bool StyleManager::shortcutExists(const QString &keys)
{
	QKeySequence key(keys);

	QMap<QString, QPointer<ScrAction> >::iterator it;
	for (it = m_styleActions.begin(); it != m_styleActions.end(); ++it)
	{
		if ((*it)->shortcut() == key)
			return true;
	}

	ApplicationPrefs *prefsData=&(PrefsManager::instance()->appPrefs);
	for (QMap<QString,Keys>::Iterator it=prefsData->keyShortcutPrefs.KeyActions.begin();
		 it!=prefsData->keyShortcutPrefs.KeyActions.end(); ++it)
	{
		if (key.matches(it.value().keySequence) != QKeySequence::NoMatch)
			return true;
	}

	return false;
}

void StyleManager::slotApplyStyle(QString keyString)
{
	if (m_isEditMode)
		return;

	QStringList slist = keyString.split(SEPARATOR, QString::SkipEmptyParts);
	Q_ASSERT(slist.count() == 2);

	loadType(slist[0]);
	m_item->toSelection(slist[1]);
	slotDocSelectionChanged();

	m_rcStyle = QString::null;
	m_rcType  = QString::null;
}

bool StyleManager::nameIsUnique(const QString &name)
{
	QList<StyleName> names = m_item->styles(false);
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
// 	qDebug()<<"slotSetupWidget"<<selection.first<<selection.second.join("|");
	if (typeName.isNull() && m_widget)
		m_widget->setEnabled(false); // nothing selected or two or more different types
	else if (!m_item || m_item->typeName() != typeName || m_widget != m_item->widget())
		loadType(typeName); // new type selected
	else if (m_widget && !m_widget->isEnabled())
		m_widget->setEnabled(true);

	disconnect(nameEdit, SIGNAL(textChanged(const QString&)),
	           this, SLOT(slotNameChanged(const QString&)));
	if (!typeName.isNull())
	{
		m_item->selected(selection.second);
		if (selection.second.count() > 1)
		{
			nameEdit->setText( tr("More than one style selected"));
			nameEdit->setEnabled(false);
			if (m_shortcutWidget)
			{
				m_shortcutWidget->setEnabled(false);
				m_shortcutWidget->setShortcut(QString::null);
			}
		}
		else
		{
			nameEdit->setText(selection.second[0]);
// 			qDebug() << selection.second[0] << m_item->isDefaultStyle(selection.second[0]);
			nameEdit->setEnabled(! (m_item->isDefaultStyle(selection.second[0])));
			if (m_shortcutWidget)
			{
				m_shortcutWidget->setEnabled(true);
				m_shortcutWidget->setShortcut(m_item->shortcut(selection.second[0]));
			}
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

	if (m_isEditMode || !sitem || sitem->isRoot())
		return; // don't apply a style in edit mode or if there was no item/type selected

	styleView->clearSelection();

	if (!m_item || m_item->typeName() != sitem->rootName())
		loadType(sitem->rootName()); // load the type where we want to apply this style

	Q_ASSERT(m_item);

	m_item->toSelection(sitem->text(NAME_COL)); // apply selected style to the selection

	slotDocSelectionChanged();

	m_rcStyle = QString::null;
	m_rcType  = QString::null;
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
	if (m_isEditMode)
		return; // don't track changes when in edit mode

// 	qDebug() << "Style Manager : doc selection changed";

// 	qDebug() <<"DISCONNECT slotDocSelectionChanged";
// 	disconnect(styleView, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)),
// 	           this, SLOT(slotApplyStyle(QTreeWidgetItem*, QTreeWidgetItem*)));

	styleView->clearSelection();

	QList<QPair<QString, QString> > selected;

	for (int i = 0; i < m_items.count(); ++i)
		selected << QPair<QString, QString>(m_items.at(i)->typeName(), m_items.at(i)->fromSelection());
	
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
// 	qDebug() <<"CONNECT slotDocSelectionChanged";
// 	connect(styleView, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
// 	        this, SLOT(slotApplyStyle(QTreeWidgetItem*,QTreeWidgetItem*)));
}

void StyleManager::slotDocStylesChanged()
{
	qDebug() << "slotDocStylesChanged()";
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
	if (m_rcStyle.isNull())
	{
		foreach(QTreeWidgetItem * it, styleView->selectedItems())
		{
			StyleViewItem *item = dynamic_cast<StyleViewItem*>(it);
			if (!item)
			{
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
		}
	}
	else // right click selection which doesn't show in the gui
	{
		typeName = m_rcType;
		styleNames << m_rcStyle;
	}
	return QPair<QString, QStringList>(typeName, styleNames);
}

// sets the current type to name including m_item and the main widget
// for editing styles
void StyleManager::loadType(const QString &name)
{
	m_item = 0;
	for (int i = 0; i < m_items.count(); ++i)
	{
		if (m_items.at(i)->typeNameSingular() == name || m_items.at(i)->typeName() == name)
		{
			m_item = m_items.at(i);
			break;
		}
	}
	if (!m_item)
		return;

	if (m_widget)
	{   // remove the old style type's widget
		m_widget->hide();
		m_layout->removeWidget(m_widget);
//		m_widget->reparent(0,0, QPoint(0,0), false);
		// show the widget for the new style type
		if (m_shortcutWidget)
			m_widget->removeTab(m_widget->indexOf(m_shortcutWidget));
	}
	m_widget = m_item->widget(); // show the widget for the style type
	//<<#8230: Hide the shortcut page as it does not work
	//insertShortcutPage(m_widget);
	//>>
	m_widget->setParent(mainFrame);
	m_layout->addWidget(m_widget, 0, 0);
	layout()->activate();
	m_widget->resize(m_widget->minimumSizeHint());
	m_widget->show();
}

void StyleManager::hideEvent(QHideEvent *e)
{
	if (m_doc)
		disconnect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(slotDocSelectionChanged()));
	m_prefs->set("eX", x());
	m_prefs->set("eY", y());
	m_prefs->set("isEditMode", m_isEditMode);
	m_prefs->set("InitX", x());
	m_prefs->set("InitY", y());
	storeVisibility(false);
	storePosition();
	ScrPaletteBase::hideEvent(e);
	emit closed();
}

void StyleManager::closeEvent(QCloseEvent *e)
{
	m_prefs->set("eX", x());
	m_prefs->set("eY", y());
	m_prefs->set("isEditMode", m_isEditMode);
	m_prefs->set("InitX", x());
	m_prefs->set("InitY", y());
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
		m_isStoryEditMode=true;
		applyButton->setEnabled(false);
		slotEdit();
	}
	setOkButtonText();
	ScrPaletteBase::showEvent(e);
	if (isFirst)
	{
		QPoint p(m_prefs->getInt("InitX", x()), m_prefs->getInt("InitY", y()));
		move(p);
		isFirst = false;
	}
	if (m_doc)
		connect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(slotDocSelectionChanged()));
}

StyleManager::~StyleManager()
{
	m_prefs->set("eX", x());
	m_prefs->set("eY", y());
	m_prefs->set("isEditMode", m_isEditMode);
	m_prefs->set("InitX", x());
	m_prefs->set("InitY", y());
	storeVisibility(this->isVisible());
	storePosition();

	while (m_items.count() > 0)
	{
		StyleItem* styleItem = m_items.takeAt(0);
		delete styleItem;
	}
}
