/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QEvent>
#include <QHeaderView>
#include <QHelpEvent>
#include <QImage>
#include <QLabel>
#include <QLayout>
#include <QList>
#include <QMenu>
#include <QMessageBox>
#include <QResizeEvent>
#include <QToolTip>
#include <QVariant>
#include <QWidgetAction>
#include <QShortcut>

#include "actionmanager.h"
#include "canvasmode.h"
#include "commonstrings.h"
#include "contextmenu.h"
#include "outlinepalette.h"
#include "page.h"
#include "propertiespalette.h"
#include "scribus.h"
#include "selection.h"
#include "undomanager.h"
#include "util.h"
#include "util_color.h"
#include "util_formats.h"
#include "util_icon.h"


OutlineTreeItem::OutlineTreeItem(OutlineTreeItem* parent, OutlineTreeItem* after) : QTreeWidgetItem(parent, after)
{
	PageObject = NULL;
	PageItemObject = NULL;
	type = -1;
}

OutlineTreeItem::OutlineTreeItem(QTreeWidget* parent, OutlineTreeItem* after) : QTreeWidgetItem(parent, after)
{
	PageObject = NULL;
	PageItemObject = NULL;
	type = -1;
}

OutlineWidget::OutlineWidget(QWidget* parent) : QTreeWidget(parent)
{
}

void OutlineWidget::selectItems(QList<QTreeWidgetItem*> items)
{
	QItemSelection itemSelection;
	for (int i = 0; i < items.count(); ++i)
	{
		QModelIndex index = this->indexFromItem(items.at(i));
		if (index.isValid())
		{
			itemSelection.select(index, index);
		}
	}
	selectionModel()->select(itemSelection, QItemSelectionModel::Select);
}

bool OutlineWidget::viewportEvent(QEvent *event)
{
	if (event->type() == QEvent::ToolTip)
	{
		QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);
		QTreeWidgetItem* it = itemAt(helpEvent->pos());
 		if (it != 0)
 		{
 			OutlineTreeItem *item = (OutlineTreeItem*)it;
 			if (item != NULL)
 			{
 				QString tipText("");
 				if ((item->type == 1) || (item->type == 3) || (item->type == 4))
 				{
 					PageItem *pgItem = item->PageItemObject;
 					switch (pgItem->itemType())
 					{
 						case PageItem::ImageFrame:
 							if (pgItem->asLatexFrame())
								tipText = CommonStrings::itemType_LatexFrame;
 							else
								tipText = CommonStrings::itemType_ImageFrame;
 							break;
 						case PageItem::TextFrame:
 							switch (pgItem->annotation().Type())
 							{
 								case 2:
									tipText = CommonStrings::itemSubType_PDF_PushButton;
 									break;
 								case 3:
									tipText = CommonStrings::itemSubType_PDF_TextField;
 									break;
 								case 4:
									tipText = CommonStrings::itemSubType_PDF_CheckBox;
 									break;
 								case 5:
									tipText = CommonStrings::itemSubType_PDF_ComboBox;
 									break;
 								case 6:
									tipText = CommonStrings::itemSubType_PDF_ListBox;
 									break;
 								case 10:
									tipText = CommonStrings::itemSubType_PDF_TextAnnotation;
 									break;
 								case 11:
									tipText = CommonStrings::itemSubType_PDF_LinkAnnotation;
 									break;
 								default:
									tipText = CommonStrings::itemType_TextFrame;
 									break;
 							}
 							break;
 						case PageItem::Line:
							tipText = CommonStrings::itemType_Line;
 							break;
 						case PageItem::Polygon:
							tipText = CommonStrings::itemType_Polygon;
 							break;
 						case PageItem::PolyLine:
							tipText = CommonStrings::itemType_Polyline;
 							break;
 						case PageItem::PathText:
							tipText = CommonStrings::itemType_PathText;
 							break;
 						default:
 							break;
 					}
					QToolTip::showText(helpEvent->globalPos(), tipText, this);
					return true;
				}
			}
		}
	}
	return QTreeWidget::viewportEvent(event);
}

OutlinePalette::OutlinePalette( QWidget* parent) : ScrPaletteBase( parent, "Tree", false, 0 )
{
//	resize( 220, 240 );
	setMinimumSize( QSize( 220, 240 ) );
	setMaximumSize( QSize( 800, 600 ) );
 
	filterEdit = new QLineEdit;
	filterEdit->setToolTip( tr("Enter a keyword or regular expression to filter the outline.") );
	QShortcut* filterShortcut = new QShortcut( QKeySequence( tr( "Ctrl+F", "Filter the Outline using a keyword" ) ), this );
	filterLabel = new QLabel( tr("Filter:") );
	filterLabel->setBuddy( filterEdit );

	reportDisplay = new OutlineWidget( this );

//	reportDisplay->setGeometry( QRect( 0, 0, 220, 240 ) );
//	reportDisplay->setMinimumSize( QSize( 220, 240 ) );
	reportDisplay->setRootIsDecorated( true );
	reportDisplay->setColumnCount(1);
	reportDisplay->setHeaderLabel( tr("Element"));
	reportDisplay->header()->setClickable( false );
	reportDisplay->header()->setResizeMode( QHeaderView::ResizeToContents );
	reportDisplay->setSortingEnabled(false);
	reportDisplay->setSelectionMode(QAbstractItemView::ExtendedSelection);
	reportDisplay->setContextMenuPolicy(Qt::CustomContextMenu);

	QGridLayout* layout = new QGridLayout;
	layout->addWidget( filterLabel, 0, 0 );
	layout->addWidget( filterEdit, 0, 1 );
	layout->addWidget( reportDisplay, 1, 0, 1, 2 );
	layout->setContentsMargins( 3, 3, 3, 3);
	setLayout( layout );

	unsetDoc();
	imageIcon = loadIcon("22/insert-image.png");
	latexIcon = loadIcon("22/insert-latex.png");
	lineIcon = loadIcon("Stift.xpm");
	textIcon = loadIcon("22/insert-text-frame.png");
	polylineIcon = loadIcon("22/draw-path.png");
	polygonIcon = loadIcon("22/draw-polygon.png");
	groupIcon = loadIcon("u_group.png");
	buttonIcon = loadIcon("22/insert-button.png");
	textFieldIcon = loadIcon("22/text-field.png");
	checkBoxIcon = loadIcon("22/checkbox.png");
	comboBoxIcon = loadIcon("22/combobox.png");
	listBoxIcon = loadIcon("22/list-box.png");
	annotTextIcon = loadIcon("22/pdf-annotations.png");
	annotLinkIcon = loadIcon("goto.png");
	selectionTriggered = false;
	m_MainWindow  = NULL;
	freeObjects   = NULL;
	rootObject    = NULL;
	currentObject = NULL;
	languageChange();
	// signals and slots connections
	connect(reportDisplay, SIGNAL(customContextMenuRequested (const QPoint &)), this, SLOT(slotRightClick(QPoint)));
	connect(reportDisplay, SIGNAL(itemSelectionChanged()), this, SLOT(slotMultiSelect()));
	connect(reportDisplay, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(slotDoRename(QTreeWidgetItem*, int)));
	connect(filterEdit, SIGNAL(textChanged(const QString&)), this, SLOT(filterTree(const QString&)));
	connect(filterShortcut, SIGNAL(activated()), filterEdit, SLOT(setFocus()));
}

void OutlinePalette::setMainWindow(ScribusMainWindow *mw)
{
	m_MainWindow=mw;
	if (m_MainWindow==NULL)
		clearPalette();
}

void OutlinePalette::setDoc(ScribusDoc *newDoc)
{
	if (m_MainWindow==NULL)
		currDoc=NULL;
	else
		currDoc=newDoc;
	if (currDoc==NULL)
		clearPalette();
}

void OutlinePalette::unsetDoc()
{
	currDoc=NULL;
	clearPalette();
}

void OutlinePalette::setPaletteShown(bool visible)
{
	ScrPaletteBase::setPaletteShown(visible);
	if ((visible) && (currDoc != NULL))
		BuildTree();
}

void OutlinePalette::slotRightClick(QPoint point)
{
	if (!m_MainWindow || m_MainWindow->scriptIsRunning())
		return;
	QTreeWidgetItem *ite = reportDisplay->itemAt(point);
	if (ite == NULL)
		return;
	if (!ite->isSelected())
		slotMultiSelect();
	OutlineTreeItem *item = (OutlineTreeItem*)ite;
	
	if (item != NULL)
	{
		if ((item->type == 0) || (item->type == 2))
			createContextMenu(NULL, point.x(), point.y());
		else if ((item->type == 1) || (item->type == 3) || (item->type == 4))
		{
			PageItem *currItem = item->PageItemObject;
			if (currItem!=NULL)
			{
				currentObject = ite;
				createContextMenu(currItem, point.x(), point.y());
			}
		}
	}
}

void OutlinePalette::slotRenameItem()
{
	activateWindow();
	reportDisplay->setFocus();
	reportDisplay->editItem(currentObject);
}

void OutlinePalette::slotDoRename(QTreeWidgetItem *ite , int col)
{
	if (!m_MainWindow || m_MainWindow->scriptIsRunning())
		return;
	disconnect(reportDisplay, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(slotDoRename(QTreeWidgetItem*, int)));
	OutlineTreeItem *item = (OutlineTreeItem*)ite;
	if (item != NULL)
	{
		if ((item->type == 1) || (item->type == 3) || (item->type == 4))
		{
			QString NameOld = item->PageItemObject->itemName();
			QString NameNew = ite->text(col);
			if (NameOld != NameNew)
			{
				if (NameNew == "")
					ite->setText(col, NameOld);
				else
				{
					bool found = false;
					for (int b = 0; b < currDoc->Items->count(); ++b)
					{
						if ((NameNew == currDoc->Items->at(b)->itemName()) && (currDoc->Items->at(b) != item->PageItemObject))
						{
							found = true;
							break;
						}
					}
					if (found)
					{
						QMessageBox::warning(this, CommonStrings::trWarning, "<qt>"+ tr("Name \"%1\" isn't unique.<br/>Please choose another.").arg(NameNew)+"</qt>", CommonStrings::tr_OK);
						ite->setText(col, NameOld);
					}
					else
					{
						item->PageItemObject->setItemName(NameNew);
						m_MainWindow->propertiesPalette->SetCurItem(item->PageItemObject);
						currDoc->setModified(true);
					}
				}
			}
		}
		filterTree();
	}
	connect(reportDisplay, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(slotDoRename(QTreeWidgetItem*, int)));
}

QTreeWidgetItem* OutlinePalette::getListItem(int SNr, int Nr)
{
	OutlineTreeItem *item = 0;
	QTreeWidgetItem *retVal = 0;
	if (currDoc->masterPageMode())
	{
		if (Nr == -1)
		{
			QTreeWidgetItemIterator it( reportDisplay );
			while ( (*it) )
			{
				item = (OutlineTreeItem*)(*it);
				if ((item->type == 0) && (item->PageObject->pageNr() == SNr))
				{
					retVal = (*it);
					break;
				}
				++it;
			}
		}
		else
		{
			QTreeWidgetItemIterator it( reportDisplay );
			while ( (*it) )
			{
				item = (OutlineTreeItem*)(*it);
				if ((item->type == 1) && (static_cast<int>(item->PageItemObject->ItemNr) == Nr))
				{
					retVal = (*it);
					break;
				}
				++it;
			}
		}
	}
	else
	{
		if (Nr == -1)
		{
			QTreeWidgetItemIterator it( reportDisplay );
			while ( (*it) )
			{
				item = (OutlineTreeItem*)(*it);
				if ((item->type == 2) && (item->PageObject->pageNr() == SNr))
				{
					retVal = (*it);
					break;
				}
				++it;
			}
		}
		else
		{
			QTreeWidgetItemIterator it( reportDisplay );
			while ( (*it) )
			{
				item = (OutlineTreeItem*)(*it);
				if (((item->type == 3) || (item->type == 4)) && (static_cast<int>(item->PageItemObject->ItemNr) == Nr))
				{
					retVal = (*it);
					break;
				}
				++it;
			}
		}
	}
	return retVal;
}

void OutlinePalette::slotShowSelect(uint SNr, int Nr)
{
	if (!m_MainWindow || m_MainWindow->scriptIsRunning())
		return;
	if (currDoc==NULL)
		return;
	if (currDoc->isLoading())
		return;
	if (selectionTriggered)
		return;
	disconnect(reportDisplay, SIGNAL(itemSelectionChanged()), this, SLOT(slotMultiSelect()));
	reportDisplay->clearSelection();
	if (currDoc->m_Selection->count() > 0)
	{
		QList<QTreeWidgetItem*> itemSelection;
		uint docSelectionCount = currDoc->m_Selection->count();
		for (uint a = 0; a < docSelectionCount; a++)
		{
			PageItem *item = currDoc->m_Selection->itemAt(a);
			QTreeWidgetItem *retVal = getListItem(item->OwnPage, item->ItemNr);
			if (retVal != 0 && !retVal->isHidden())
				itemSelection.append(retVal);
		}
		reportDisplay->selectItems(itemSelection);
	}
	else
	{
		QTreeWidgetItem *retVal = getListItem(SNr, Nr);
		if (retVal != 0 && !retVal->isHidden())
			retVal->setSelected(true);
	}
	QList<QTreeWidgetItem *> items = reportDisplay->selectedItems();
	if (items.count() > 0)
		reportDisplay->scrollToItem(items[0], QAbstractItemView::EnsureVisible);
	connect(reportDisplay, SIGNAL(itemSelectionChanged()), this, SLOT(slotMultiSelect()));
}

void OutlinePalette::setItemIcon(QTreeWidgetItem *item, PageItem *pgItem)
{
	switch (pgItem->itemType())
	{
	case PageItem::ImageFrame:
 		if (pgItem->asLatexFrame())
			item->setIcon( 0, latexIcon );
 		else
			item->setIcon( 0, imageIcon );
		break;
	case PageItem::TextFrame:
		switch (pgItem->annotation().Type())
		{
			case 2:
				item->setIcon( 0, buttonIcon );
				break;
			case 3:
				item->setIcon( 0, textFieldIcon );
				break;
			case 4:
				item->setIcon( 0, checkBoxIcon );
				break;
			case 5:
				item->setIcon( 0, comboBoxIcon );
				break;
			case 6:
				item->setIcon( 0, listBoxIcon );
				break;
			case 10:
				item->setIcon( 0, annotTextIcon );
				break;
			case 11:
				item->setIcon( 0, annotLinkIcon );
				break;
			default:
				item->setIcon( 0, textIcon );
				break;
		}
		break;
	case PageItem::Line:
		item->setIcon( 0, lineIcon );
		break;
	case PageItem::Polygon:
		item->setIcon( 0, polygonIcon );
		break;
	case PageItem::PolyLine:
		item->setIcon( 0, polylineIcon );
		break;
	case PageItem::PathText:
		item->setIcon( 0, textIcon );
		break;
	default:
		break;
	}
}

void OutlinePalette::reopenTree()
{
	if (!m_MainWindow || m_MainWindow->scriptIsRunning())
		return;
	if (currDoc->OpenNodes.count() == 0)
		return;
	OutlineTreeItem *item = 0;
	QTreeWidgetItemIterator it( reportDisplay );
	while ( (*it) )
	{
		item = (OutlineTreeItem*)(*it);
		for (int olc = 0; olc < currDoc->OpenNodes.count(); olc++)
		{
			if (item->type == currDoc->OpenNodes[olc].type)
			{
				if ((item->type == -3) || (item->type == -2))
					reportDisplay->expandItem((*it));
				else if ((item->type == 0) || (item->type == 2))
				{
					if (item->PageObject == currDoc->OpenNodes[olc].page)
						reportDisplay->expandItem((*it));
				}
				else if ((item->type == 2) || (item->type == 3) || (item->type == 4))
				{
					if (item->PageItemObject == currDoc->OpenNodes[olc].item)
						reportDisplay->expandItem((*it));
				}
			}
		}
		++it;
	}
}

void OutlinePalette::buildReopenVals()
{
	ScribusDoc::OpenNodesList ol;
	if (reportDisplay->model()->rowCount() == 0)
		return;
	currDoc->OpenNodes.clear();
	OutlineTreeItem *item = 0;
	QTreeWidgetItemIterator it( reportDisplay );
	while ( (*it) )
	{
		item = (OutlineTreeItem*)(*it);
		if (item->isExpanded())
		{
			ol.type = item->type;
			ol.page = item->PageObject;
			ol.item = item->PageItemObject;
			currDoc->OpenNodes.append(ol);
		}
		++it;
	}
}

void OutlinePalette::slotMultiSelect()
{
	if (!m_MainWindow || m_MainWindow->scriptIsRunning())
		return;
	if (currDoc==NULL)
		return;
	disconnect(reportDisplay, SIGNAL(itemSelectionChanged()), this, SLOT(slotMultiSelect()));
	selectionTriggered = true;
	QList<QTreeWidgetItem *> items = reportDisplay->selectedItems();
	if (items.count() != 1)
	{
		if (currDoc->appMode == modeEditClip)
			currDoc->view()->requestMode(submodeEndNodeEdit);
		currDoc->m_Selection->delaySignalsOn();
		currDoc->view()->Deselect(true);
		for (int a = 0; a < items.count(); a++)
		{
			QTreeWidgetItem* ite = items[a];
			OutlineTreeItem *item = (OutlineTreeItem*)ite;
			PageItem *pgItem = NULL;
			switch (item->type)
			{
				case 0:
				case 1:
				case 2:
					ite->setSelected(false);
					break;
				case 3:
				case 4:
					pgItem = item->PageItemObject;
					if (!pgItem->isSelected())
					{
						m_MainWindow->closeActiveWindowMasterPageEditor();
						//currDoc->m_Selection->setIsGUISelection(false);
						currDoc->view()->SelectItemNr(pgItem->ItemNr, false, false);
					}
					break;
			}
		}
		/*if (currDoc->m_Selection->count() > 0)
		{
			currDoc->m_Selection->setIsGUISelection(true);
			currDoc->m_Selection->connectItemToGUI();
		}*/
		currDoc->m_Selection->delaySignalsOff();
		currDoc->view()->DrawNew();
	}
	else
		slotSelect(items[0], 0);
	selectionTriggered = false;
	connect(reportDisplay, SIGNAL(itemSelectionChanged()), this, SLOT(slotMultiSelect()));
}

void OutlinePalette::slotSelect(QTreeWidgetItem* ite, int col)
{
	if (!m_MainWindow || m_MainWindow->scriptIsRunning())
		return;
	selectionTriggered = true;
	OutlineTreeItem *item = (OutlineTreeItem*)ite;
	uint pg = 0;
	PageItem *pgItem = NULL;
	switch (item->type)
	{
		case 0:
			emit selectMasterPage(item->PageObject->pageName());
			break;
		case 1:
			if (!currDoc->masterPageMode())
				emit selectMasterPage(item->PageItemObject->OnMasterPage);
			if (currDoc->activeLayer() != item->PageItemObject->LayerNr)
			{
				currDoc->setActiveLayer(item->PageItemObject->LayerNr);
				m_MainWindow->changeLayer(currDoc->activeLayer());
			}
			if (item->PageItemObject->Groups.count() == 0)
				emit selectElement(-1, item->PageItemObject->ItemNr, false);
			else
			{
				if (item->PageItemObject->isGroupControl)
					emit selectElement(-1, item->PageItemObject->ItemNr, false);
				else
					emit selectElement(-1, item->PageItemObject->ItemNr, true);
			}
			break;
		case 2:
			pg = item->PageObject->pageNr();
			m_MainWindow->closeActiveWindowMasterPageEditor();
			emit selectPage(pg);
			break;
		case 3:
		case 4:
			pgItem = item->PageItemObject;
			m_MainWindow->closeActiveWindowMasterPageEditor();
			if (currDoc->activeLayer() != pgItem->LayerNr)
			{
				currDoc->setActiveLayer(pgItem->LayerNr);
				m_MainWindow->changeLayer(currDoc->activeLayer());
			}
			if (pgItem->Groups.count() == 0)
				emit selectElement(pgItem->OwnPage, pgItem->ItemNr, false);
			else
			{
				if (pgItem->isGroupControl)
					emit selectElement(pgItem->OwnPage, pgItem->ItemNr, false);
				else
					emit selectElement(pgItem->OwnPage, pgItem->ItemNr, true);
			}
			break;
		default:
			break;
	}
	selectionTriggered = false;
}
/*
void OutlinePalette::resizeEvent(QResizeEvent *r)
{
	reportDisplay->resize(r->size());
}
*/
void OutlinePalette::BuildTree(bool storeVals)
{
	if (!m_MainWindow || m_MainWindow->scriptIsRunning())
		return;
	if (currDoc==NULL)
		return;
	if (selectionTriggered)
		return;
	disconnect(reportDisplay, SIGNAL(itemSelectionChanged()), this, SLOT(slotMultiSelect()));
	disconnect(reportDisplay, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(slotDoRename(QTreeWidgetItem*, int)));
	setUpdatesEnabled(false);
	if (storeVals)
		buildReopenVals();
	clearPalette();
	QList<PageItem*> subGroupList;
	OutlineTreeItem * item = new OutlineTreeItem( reportDisplay, 0 );
	rootObject = item;
	item->setText( 0, currDoc->DocName.section( '/', -1 ) );
	item->type = -2;
	OutlineTreeItem * pagep = 0;
	freeObjects = 0;
	PageItem* pgItem;
	QString tmp;
	for (int b = 0; b < currDoc->MasterItems.count(); ++b)
	{
		currDoc->MasterItems.at(b)->Dirty = false;
	}
	for (int a = 0; a < static_cast<int>(currDoc->MasterPages.count()); ++a)
	{
		OutlineTreeItem *page = new OutlineTreeItem( item, pagep );
		page->PageObject = currDoc->MasterPages.at(a);
		page->type = 0;
		QString pageNam = currDoc->MasterPages.at(a)->pageName();
		pagep = page;
		for (int b = 0; b < currDoc->MasterItems.count(); ++b)
		{
			pgItem = currDoc->MasterItems.at(b);
			if (((pgItem->OwnPage == a) || (pgItem->OnMasterPage == pageNam)) && (!pgItem->Dirty))
			{
				if (pgItem->Groups.count() == 0)
				{
					OutlineTreeItem *object = new OutlineTreeItem( page, 0 );
					object->PageItemObject = pgItem;
					object->type = 1;
					object->setText(0, pgItem->itemName());
					setItemIcon(object, pgItem);
					object->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
					pgItem->Dirty = true;
				}
				else
				{
					OutlineTreeItem * object = new OutlineTreeItem( page, 0 );
					object->PageItemObject = pgItem;
					object->type = 1;
					if (pgItem->isGroupControl)
						object->setText(0, pgItem->itemName());
					else
						object->setText(0, tr("Group ")+tmp.setNum(pgItem->Groups.top()));
					object->setIcon( 0, groupIcon );
					object->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
					pgItem->Dirty = true;
					subGroupList.clear();
					for (int ga = 0; ga < currDoc->MasterItems.count(); ++ga)
					{
						PageItem* pgItem2 = currDoc->MasterItems.at(ga);
						if ((pgItem2->Groups.count() != 0) && (pgItem2->Groups.top() == pgItem->Groups.top()) && (pgItem2 != pgItem))
							subGroupList.append(pgItem2);
					}
					parseSubGroup(1, object, &subGroupList, 1);
				}
			}
		}
		page->setText(0, currDoc->MasterPages.at(a)->pageName());
	}
	for (int b = 0; b < currDoc->DocItems.count(); ++b)
	{
		currDoc->DocItems.at(b)->Dirty = false;
	}
	for (int a = 0; a < static_cast<int>(currDoc->DocPages.count()); ++a)
	{
		OutlineTreeItem *page = new OutlineTreeItem( item, pagep );
		page->PageObject = currDoc->DocPages.at(a);
		page->type = 2;
		pagep = page;
		for (int b = 0; b < currDoc->DocItems.count(); ++b)
		{
			pgItem = currDoc->DocItems.at(b);
			if ((pgItem->OwnPage == a) && (!pgItem->Dirty))
			{
				if (pgItem->Groups.count() == 0)
				{
					OutlineTreeItem *object = new OutlineTreeItem( page, 0 );
					object->PageItemObject = pgItem;
					object->type = 3;
					object->setText(0, pgItem->itemName());
					setItemIcon(object, pgItem);
					object->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
					pgItem->Dirty = true;
				}
				else
				{
					OutlineTreeItem *object = new OutlineTreeItem( page, 0 );
					object->PageItemObject = pgItem;
					object->type = 3;
					if (pgItem->isGroupControl)
						object->setText(0, pgItem->itemName());
					else
						object->setText(0, tr("Group ")+tmp.setNum(pgItem->Groups.top()));
					object->setIcon( 0, groupIcon );
					object->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
					pgItem->Dirty = true;
					subGroupList.clear();
					for (int ga = 0; ga < currDoc->DocItems.count(); ++ga)
					{
						PageItem* pgItem2 = currDoc->DocItems.at(ga);
						if ((pgItem2->Groups.count() != 0) && (pgItem2->Groups.top() == pgItem->Groups.top()) && (pgItem2 != pgItem))
							subGroupList.append(pgItem2);
					}
					parseSubGroup(1, object, &subGroupList, 3);
				}
			}
		}
		page->setText(0, tr("Page ")+tmp.setNum(a+1));
	}
	bool hasfreeItems = false;
	for (int b = 0; b < currDoc->DocItems.count(); ++b)
	{
		if (currDoc->DocItems.at(b)->OwnPage == -1)
		{
			hasfreeItems = true;
			break;
		}
	}
	if (hasfreeItems)
	{
		OutlineTreeItem *page = new OutlineTreeItem( item, pagep );
		pagep = page;
		freeObjects = page;
		page->type = -3;
		for (int b = 0; b < currDoc->DocItems.count(); ++b)
		{
			pgItem = currDoc->DocItems.at(b);
			if ((pgItem->OwnPage == -1) && (!pgItem->Dirty))
			{
				if (pgItem->Groups.count() == 0)
				{
					OutlineTreeItem *object = new OutlineTreeItem( page, 0 );
					object->PageItemObject = pgItem;
					object->type = 4;
					object->setText(0, pgItem->itemName());
					setItemIcon(object, pgItem);
					object->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
					pgItem->Dirty = true;
				}
				else
				{
					OutlineTreeItem *object = new OutlineTreeItem( page, 0 );
					object->PageItemObject = pgItem;
					object->type = 4;
					if (pgItem->isGroupControl)
						object->setText(0, pgItem->itemName());
					else
						object->setText(0, tr("Group ")+tmp.setNum(pgItem->Groups.top()));
					object->setIcon( 0, groupIcon );
					object->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
					pgItem->Dirty = true;
					subGroupList.clear();
					for (int ga = 0; ga < currDoc->DocItems.count(); ++ga)
					{
						PageItem* pgItem2 = currDoc->DocItems.at(ga);
						if ((pgItem2->Groups.count() != 0) && (pgItem2->Groups.top() == pgItem->Groups.top()) && (pgItem2 != pgItem))
							subGroupList.append(pgItem2);
					}
					parseSubGroup(1, object, &subGroupList, 4);
				}
			}
		}
		page->setText(0, tr("Free Objects"));
	}
	if (storeVals)
		reopenTree();
	setUpdatesEnabled(true);
	filterTree();
	if (currDoc->m_Selection->count() > 0)
		slotShowSelect(0, -1);
	repaint();
	connect(reportDisplay, SIGNAL(itemSelectionChanged()), this, SLOT(slotMultiSelect()));
	connect(reportDisplay, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(slotDoRename(QTreeWidgetItem*, int)));
}

void OutlinePalette::filterTree(const QString& keyword)
{
	OutlineTreeItem *item = NULL;
	QTreeWidgetItemIterator it( reportDisplay );
	while ( (*it) )
	{
		item = dynamic_cast<OutlineTreeItem*>(*it);
		if (item != NULL)
		{
			if ((item->type == 1) || (item->type == 3) || (item->type == 4))
			{
				if (item->PageItemObject->itemName().contains(QRegExp(keyword, Qt::CaseInsensitive)))
					item->setHidden(false);
				else
					item->setHidden(true);
			}
			else
				item->setHidden(false);
		}
		++it;
	}
}

void OutlinePalette::filterTree()
{
	if ( !filterEdit->text().isEmpty() )
		filterTree( filterEdit->text() );
}

void OutlinePalette::parseSubGroup(int level, OutlineTreeItem* object, QList<PageItem*> *subGroupList, int itemType)
{
	QList<PageItem*> *subGroup;
	PageItem *pgItem;
	QString tmp;
	for (int b = 0; b < subGroupList->count(); ++b)
	{
		pgItem = subGroupList->at(b);
		if (!pgItem->Dirty)
		{
			if (static_cast<int>(pgItem->Groups.count()) <= level)
			{
				OutlineTreeItem *grp = new OutlineTreeItem( object, 0 );
				grp->PageItemObject = pgItem;
				grp->type = itemType;
				grp->setText(0, pgItem->itemName());
				setItemIcon(grp, pgItem);
				grp->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
				pgItem->Dirty = true;
			}
			else
			{
				OutlineTreeItem *grp = new OutlineTreeItem( object, 0 );
				grp->PageItemObject = pgItem;
				grp->type = itemType;
				if (pgItem->isGroupControl)
					grp->setText(0, pgItem->itemName());
				else
				grp->setText(0, tr("Group ")+tmp.setNum(pgItem->Groups.at(pgItem->Groups.count()-level-1)));
				grp->setIcon( 0, groupIcon );
				grp->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
				pgItem->Dirty = true;
				subGroup = new QList<PageItem*>;
				subGroup->clear();
				for (int ga = 0; ga < subGroupList->count(); ++ga)
				{
					PageItem* pgItem2 = subGroupList->at(ga);
					if ((static_cast<int>(pgItem2->Groups.count()) > level) && 
						((pgItem2->Groups.at(pgItem2->Groups.count()-level-1)) == (pgItem->Groups.at(pgItem->Groups.count()-level-1))) && (pgItem2 != pgItem))
						subGroup->append(pgItem2);
				}
				parseSubGroup(level+1, grp, subGroup, itemType);
				delete subGroup;
			}
		}
	}
}

void OutlinePalette::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
		languageChange();
	else
		QWidget::changeEvent(e);
}


void OutlinePalette::languageChange()
{
	setWindowTitle( tr("Outline"));
	reportDisplay->setHeaderLabel( tr("Element"));
	filterLabel->setText( tr("Filter:") );
}

void OutlinePalette::clearPalette()
{
	reportDisplay->clear();
}

void OutlinePalette::createContextMenu(PageItem * currItem, double mx, double my)
{
	if (m_MainWindow==NULL || currDoc==NULL)
		return;
	ContextMenu* cmen=NULL;
	qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
	if(currItem!=NULL)
		cmen = new ContextMenu(*(currDoc->m_Selection), m_MainWindow, currDoc);
	else
		cmen = new ContextMenu(m_MainWindow, currDoc, currDoc->currentPage()->xOffset(), currDoc->currentPage()->yOffset());
	if (cmen)
		cmen->exec(QCursor::pos());
	delete cmen;
}
