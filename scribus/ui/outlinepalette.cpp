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
#include <QSignalMapper>
#include <QShortcut>

#include "actionmanager.h"
#include "canvasmode.h"
#include "commonstrings.h"
#include "contextmenu.h"
#include "outlinepalette.h"
#include "propertiespalette.h"
#include "scpage.h"
#include "scribus.h"
#include "selection.h"
#include "layers.h"
#include "undomanager.h"
#include "util.h"
#include "util_color.h"
#include "util_formats.h"
#include "util_icon.h"


OutlineTreeItem::OutlineTreeItem(OutlineTreeItem* parent, OutlineTreeItem* after) : QTreeWidgetItem(parent, after)
{
	PageObject = NULL;
	PageItemObject = NULL;
	DocObject = NULL;
	type = -1;
	LayerID = -1;
}

OutlineTreeItem::OutlineTreeItem(QTreeWidget* parent, OutlineTreeItem* after) : QTreeWidgetItem(parent, after)
{
	PageObject = NULL;
	PageItemObject = NULL;
	DocObject = NULL;
	type = -1;
	LayerID = -1;
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
				if (item->type == 5)
				{
					tipText += "<b>" + tr("Layer is:") + "</b><br><br>";
					if (item->DocObject->activeLayer() == item->LayerID)
						tipText += tr("active") + "<br>";
					if (item->DocObject->layerVisible(item->LayerID))
						tipText += tr("visible") + "<br>";
					else
						tipText += tr("invisible") + "<br>";
					if (item->DocObject->layerPrintable(item->LayerID))
						tipText += tr("printing");
					else
						tipText += tr("non printing");
					if (item->DocObject->layerLocked(item->LayerID))
						tipText += "<br>" + tr("locked");
				}
				else if ((item->type == 1) || (item->type == 3) || (item->type == 4))
 				{
 					PageItem *pgItem = item->PageItemObject;
 					switch (pgItem->itemType())
 					{
 						case PageItem::ImageFrame:
 							if (pgItem->asLatexFrame())
								tipText = CommonStrings::itemType_LatexFrame;
#ifdef HAVE_OSG
 							else if (pgItem->asOSGFrame())
								tipText = CommonStrings::itemType_OSGFrame;
#endif
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
 						case PageItem::Arc:
							tipText = CommonStrings::itemType_Arc;
							break;
 						case PageItem::Polygon:
							tipText = CommonStrings::itemType_Polygon;
							break;
						case PageItem::RegularPolygon:
							tipText = CommonStrings::itemType_RegularPolygon;
 							break;
 						case PageItem::PolyLine:
							tipText = CommonStrings::itemType_Polyline;
							break;
						case PageItem::Spiral:
							tipText = CommonStrings::itemType_Spiral;
 							break;
 						case PageItem::PathText:
							tipText = CommonStrings::itemType_PathText;
 							break;
 						case PageItem::Symbol:
							tipText = CommonStrings::itemType_Symbol;
 							break;
						case PageItem::Group:
							tipText = CommonStrings::itemType_Group;
							break;
						case PageItem::Table:
							tipText = CommonStrings::itemType_Table;
							break;
 						default:
 							break;
					}
				}
				QToolTip::showText(helpEvent->globalPos(), tipText, this);
				return true;
			}
		}
	}
	return QTreeWidget::viewportEvent(event);
}

OutlinePalette::OutlinePalette( QWidget* parent) : ScDockPalette( parent, "Tree", 0)
{
//	resize( 220, 240 );
	setMinimumSize( QSize( 220, 240 ) );
	setObjectName(QString::fromLocal8Bit("Tree"));
	setSizePolicy( QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
	containerWidget = new QWidget(this);
 
	filterEdit = new QLineEdit;
	filterEdit->setToolTip( tr("Enter a keyword or regular expression to filter the outline.") );
//	QShortcut* filterShortcut = new QShortcut( QKeySequence( tr( "Ctrl+F", "Filter the Outline using a keyword" ) ), this );
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
	containerWidget->setLayout( layout );
	setWidget( containerWidget );

	unsetDoc();
	imageIcon = loadIcon("22/insert-image.png");
	latexIcon = loadIcon("22/insert-latex.png");
	lineIcon = loadIcon("Stift.xpm");
	textIcon = loadIcon("22/insert-text-frame.png");
	polylineIcon = loadIcon("22/draw-path.png");
	polygonIcon = loadIcon("22/draw-polygon.png");
	tableIcon = loadIcon("22/insert-table.png");
	groupIcon = loadIcon("u_group.png");
	buttonIcon = loadIcon("22/insert-button.png");
	textFieldIcon = loadIcon("22/text-field.png");
	checkBoxIcon = loadIcon("22/checkbox.png");
	comboBoxIcon = loadIcon("22/combobox.png");
	listBoxIcon = loadIcon("22/list-box.png");
	annotTextIcon = loadIcon("22/pdf-annotations.png");
	annotLinkIcon = loadIcon("goto.png");
	annot3DIcon = loadIcon("22/annot3d.png");
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
//	connect(filterShortcut, SIGNAL(activated()), filterEdit, SLOT(setFocus()));
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
	ScDockPalette::setPaletteShown(visible);
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
		else if (item->type == 5)
		{
			QMenu *pmenu = new QMenu();
			QAction *actVis;
			QAction *actPrint;
			QAction *actLock;
			if (item->DocObject->activeLayer() != item->LayerID)
			{
				QAction *actActive;
				QSignalMapper *signalMapper = new QSignalMapper(this);
				actActive = pmenu->addAction( tr("Active"));
				actActive->setCheckable(true);
				actActive->setChecked(false);
				signalMapper->setMapping(actActive, item->LayerID);
				connect(actActive, SIGNAL(triggered()), signalMapper, SLOT(map()));
				connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(setActiveLayer(int)));
			}
			QSignalMapper *signalMapper2 = new QSignalMapper(this);
			actVis = pmenu->addAction( tr("Visible"));
			actVis->setCheckable(true);
			actVis->setChecked(item->DocObject->layerVisible(item->LayerID));
			signalMapper2->setMapping(actVis, item->LayerID);
			connect(actVis, SIGNAL(triggered()), signalMapper2, SLOT(map()));
			connect(signalMapper2, SIGNAL(mapped(int)), this, SLOT(setLayerVisible(int)));

			QSignalMapper *signalMapper3 = new QSignalMapper(this);
			actPrint = pmenu->addAction( tr("Printing"));
			actPrint->setCheckable(true);
			actPrint->setChecked(item->DocObject->layerPrintable(item->LayerID));
			signalMapper3->setMapping(actPrint, item->LayerID);
			connect(actPrint, SIGNAL(triggered()), signalMapper3, SLOT(map()));
			connect(signalMapper3, SIGNAL(mapped(int)), this, SLOT(setLayerPrintable(int)));

			QSignalMapper *signalMapper4 = new QSignalMapper(this);
			actLock = pmenu->addAction( tr("Locked"));
			actLock->setCheckable(true);
			actLock->setChecked(item->DocObject->layerLocked(item->LayerID));
			signalMapper4->setMapping(actLock, item->LayerID);
			connect(actLock, SIGNAL(triggered()), signalMapper4, SLOT(map()));
			connect(signalMapper4, SIGNAL(mapped(int)), this, SLOT(setLayerLocked(int)));
			qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
			pmenu->exec(QCursor::pos());
			delete pmenu;
		}
	}
}

void OutlinePalette::setActiveLayer(int layerID)
{
	currDoc->setActiveLayer(layerID);
	currDoc->scMW()->changeLayer(currDoc->activeLayer());
}

void OutlinePalette::setLayerVisible(int layerID)
{
	currDoc->setLayerVisible(layerID, !currDoc->layerVisible(layerID));
	currDoc->scMW()->showLayer();
	currDoc->scMW()->layerPalette->rebuildList();
}

void OutlinePalette::setLayerLocked(int layerID)
{
	currDoc->setLayerLocked(layerID, !currDoc->layerLocked(layerID));
	currDoc->scMW()->layerPalette->rebuildList();
}

void OutlinePalette::setLayerPrintable(int layerID)
{
	currDoc->setLayerPrintable(layerID, !currDoc->layerPrintable(layerID));
	currDoc->scMW()->layerPalette->rebuildList();
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
					QList<PageItem*> allItems;
					for (int a = 0; a < currDoc->Items->count(); ++a)
					{
						PageItem *currItem = currDoc->Items->at(a);
						if (currItem->isGroup())
							allItems = currItem->getItemList();
						else
							allItems.append(currItem);
						for (int ii = 0; ii < allItems.count(); ii++)
						{
							PageItem* ite = allItems.at(ii);
							if ((NameNew == ite->itemName()) && (ite != item->PageItemObject))
							{
								found = true;
								break;
							}
						}
						allItems.clear();
					}
					if (found)
					{
						QMessageBox::warning(this, CommonStrings::trWarning, "<qt>"+ tr("Name \"%1\" isn't unique.<br/>Please choose another.").arg(NameNew)+"</qt>", CommonStrings::tr_OK);
						ite->setText(col, NameOld);
					}
					else
					{
						item->PageItemObject->setItemName(NameNew);
						m_MainWindow->propertiesPalette->setCurrentItem(item->PageItemObject);
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
			if (retVal != 0)
				itemSelection.append(retVal);
		}
		reportDisplay->selectItems(itemSelection);
	}
	else
	{
		QTreeWidgetItem *retVal = getListItem(SNr, Nr);
		if (retVal != 0)
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
#ifdef HAVE_OSG
		else if (pgItem->asOSGFrame())
			item->setIcon( 0, annot3DIcon );
#endif
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
	case PageItem::Arc:
	case PageItem::Polygon:
	case PageItem::RegularPolygon:
		item->setIcon( 0, polygonIcon );
		break;
	case PageItem::PolyLine:
	case PageItem::Spiral:
		item->setIcon( 0, polylineIcon );
		break;
	case PageItem::PathText:
		item->setIcon( 0, textIcon );
		break;
	case PageItem::Symbol:
		item->setIcon( 0, polygonIcon );
		break;
	case PageItem::Table:
		item->setIcon( 0, tableIcon );
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
				case 5:
					ite->setSelected(false);
					break;
				case 3:
				case 4:
					pgItem = item->PageItemObject;
					if (!pgItem->isSelected())
					{
						m_MainWindow->closeActiveWindowMasterPageEditor();
						currDoc->view()->SelectItemNr(pgItem->ItemNr, false, false);
					}
					break;
				default:
					break;
			}
		}
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
			pgItem = item->PageItemObject;
			if (item->PageItemObject->isGroup())
				emit selectElementByItem(pgItem, false);
			else
				emit selectElementByItem(pgItem, true);
			break;
		case 2:
			if (!currDoc->symbolEditMode())
			{
				pg = item->PageObject->pageNr();
				m_MainWindow->closeActiveWindowMasterPageEditor();
				emit selectPage(pg);
			}
			break;
		case 3:
		case 4:
			pgItem = item->PageItemObject;
			m_MainWindow->closeActiveWindowMasterPageEditor();
			if (pgItem->isGroup())
				emit selectElementByItem(pgItem, false);
			else
				emit selectElementByItem(pgItem, true);
			break;
		default:
			break;
	}
	selectionTriggered = false;
}

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
	OutlineTreeItem * item = new OutlineTreeItem( reportDisplay, 0 );
	rootObject = item;
	item->setText( 0, currDoc->DocName.section( '/', -1 ) );
	item->type = -2;
	OutlineTreeItem * pagep = 0;
	freeObjects = 0;
	PageItem* pgItem;
	QString tmp;
	if (currDoc->symbolEditMode())
	{
		OutlineTreeItem *page = new OutlineTreeItem( item, pagep );
		page->PageObject = currDoc->Pages->at(0);
		page->type = 2;
		pagep = page;
		for (int b = 0; b < currDoc->Items->count(); ++b)
		{
			pgItem = currDoc->Items->at(b);
			if (!pgItem->isGroup())
			{
				OutlineTreeItem *object = new OutlineTreeItem( page, 0 );
				object->PageItemObject = pgItem;
				object->type = 3;
				object->setText(0, pgItem->itemName());
				setItemIcon(object, pgItem);
				object->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
			}
			else
			{
				OutlineTreeItem *object = new OutlineTreeItem( page, 0 );
				object->PageItemObject = pgItem;
				object->type = 3;
				object->setText(0, pgItem->itemName());
				object->setIcon( 0, groupIcon );
				object->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
				parseSubGroup(object, &pgItem->groupItemList, 3);
			}
		}
		page->setText(0, currDoc->getEditedSymbol());
	}
	else
	{
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
				if (((pgItem->OwnPage == a) || (pgItem->OnMasterPage == pageNam)))
				{
					if (!pgItem->isGroup())
					{
						OutlineTreeItem *object = new OutlineTreeItem( page, 0 );
						object->PageItemObject = pgItem;
						object->type = 1;
						object->setText(0, pgItem->itemName());
						setItemIcon(object, pgItem);
						object->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
					}
					else
					{
						OutlineTreeItem * object = new OutlineTreeItem( page, 0 );
						object->PageItemObject = pgItem;
						object->type = 1;
						object->setText(0, pgItem->itemName());
						object->setIcon( 0, groupIcon );
						object->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
						parseSubGroup(object, &pgItem->groupItemList, 1);
					}
				}
			}
			page->setText(0, currDoc->MasterPages.at(a)->pageName());
		}
		bool hasfreeItems = false;
		for (int a = 0; a < static_cast<int>(currDoc->DocPages.count()); ++a)
		{
			OutlineTreeItem *page = new OutlineTreeItem( item, pagep );
			page->PageObject = currDoc->DocPages.at(a);
			page->type = 2;
			pagep = page;
			int layerCount = currDoc->layerCount();
			if (layerCount > 1)
			{
				QList<PageItem*> pgItems;
				for (int b = 0; b < currDoc->DocItems.count(); ++b)
				{
					pgItem = currDoc->DocItems.at(b);
					if (pgItem->OwnPage == -1)
						hasfreeItems = true;
					if (pgItem->OwnPage == a)
						pgItems.append(pgItem);
				}
				ScLayer layer;
				layer.ID = 0;
				for (int layerLevel = 0; layerLevel < layerCount; ++layerLevel)
				{
					currDoc->Layers.levelToLayer(layer, layerLevel);
					OutlineTreeItem *ObjLayer = new OutlineTreeItem( page, 0 );
					ObjLayer->type = 5;
					ObjLayer->LayerID = layer.ID;
					ObjLayer->DocObject = currDoc;
					ObjLayer->setText(0, tr("Layer: \"") + layer.Name + "\"");
					for (int it = 0; it < pgItems.count(); ++it)
					{
						pgItem = pgItems.at(it);
						if (pgItem->LayerID != layer.ID)
							continue;
						if (!pgItem->isGroup())
						{
							OutlineTreeItem *object = new OutlineTreeItem( ObjLayer, 0 );
							object->PageItemObject = pgItem;
							object->type = 3;
							object->setText(0, pgItem->itemName());
							setItemIcon(object, pgItem);
							object->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
						}
						else
						{
							OutlineTreeItem *object = new OutlineTreeItem( ObjLayer, 0 );
							object->PageItemObject = pgItem;
							object->type = 3;
							object->setText(0, pgItem->itemName());
							object->setIcon( 0, groupIcon );
							object->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
							parseSubGroup(object, &pgItem->groupItemList, 3);
						}
					}
				}
			}
			else
			{
				for (int b = 0; b < currDoc->DocItems.count(); ++b)
				{
					pgItem = currDoc->DocItems.at(b);
					if (pgItem->OwnPage == -1)
						hasfreeItems = true;
					if (pgItem->OwnPage == a)
					{
						if (!pgItem->isGroup())
						{
							OutlineTreeItem *object = new OutlineTreeItem( page, 0 );
							object->PageItemObject = pgItem;
							object->type = 3;
							object->setText(0, pgItem->itemName());
							setItemIcon(object, pgItem);
							object->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
						}
						else
						{
							OutlineTreeItem *object = new OutlineTreeItem( page, 0 );
							object->PageItemObject = pgItem;
							object->type = 3;
							object->setText(0, pgItem->itemName());
							object->setIcon( 0, groupIcon );
							object->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
							parseSubGroup(object, &pgItem->groupItemList, 3);
						}
					}
				}
			}
			page->setText(0, tr("Page ")+tmp.setNum(a+1));
		}
		if (hasfreeItems)
		{
			OutlineTreeItem *page = new OutlineTreeItem( item, pagep );
			pagep = page;
			freeObjects = page;
			page->type = -3;
			int layerCount = currDoc->layerCount();
			if (layerCount > 1)
			{
				QList<PageItem*> pgItems;
				for (int b = 0; b < currDoc->DocItems.count(); ++b)
				{
					pgItem = currDoc->DocItems.at(b);
					if (pgItem->OwnPage == -1)
						pgItems.append(pgItem);
				}
				ScLayer layer;
				layer.ID = 0;
				for (int layerLevel = 0; layerLevel < layerCount; ++layerLevel)
				{
					currDoc->Layers.levelToLayer(layer, layerLevel);
					OutlineTreeItem *ObjLayer = new OutlineTreeItem( page, 0 );
					ObjLayer->type = 5;
					ObjLayer->LayerID = layer.ID;
					ObjLayer->DocObject = currDoc;
					ObjLayer->setText(0, tr("Layer: \"") + layer.Name + "\"");
					for (int it = 0; it < pgItems.count(); ++it)
					{
						pgItem = pgItems.at(it);
						if (pgItem->LayerID != layer.ID)
							continue;
						if (!pgItem->isGroup())
						{
							OutlineTreeItem *object = new OutlineTreeItem( ObjLayer, 0 );
							object->PageItemObject = pgItem;
							object->type = 3;
							object->setText(0, pgItem->itemName());
							setItemIcon(object, pgItem);
							object->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
						}
						else
						{
							OutlineTreeItem *object = new OutlineTreeItem( ObjLayer, 0 );
							object->PageItemObject = pgItem;
							object->type = 3;
							object->setText(0, pgItem->itemName());
							object->setIcon( 0, groupIcon );
							object->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
							parseSubGroup(object, &pgItem->groupItemList, 4);
						}
					}
				}
			}
			else
			{
				for (int b = 0; b < currDoc->DocItems.count(); ++b)
				{
					pgItem = currDoc->DocItems.at(b);
					if (pgItem->OwnPage == -1)
					{
						if (!pgItem->isGroup())
						{
							OutlineTreeItem *object = new OutlineTreeItem( page, 0 );
							object->PageItemObject = pgItem;
							object->type = 4;
							object->setText(0, pgItem->itemName());
							setItemIcon(object, pgItem);
							object->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
						}
						else
						{
							OutlineTreeItem *object = new OutlineTreeItem( page, 0 );
							object->PageItemObject = pgItem;
							object->type = 4;
							object->setText(0, pgItem->itemName());
							object->setIcon( 0, groupIcon );
							object->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
							parseSubGroup(object, &pgItem->groupItemList, 4);
						}
					}
				}
			}
			page->setText(0, tr("Free Objects"));
		}
	}
	if (storeVals)
		reopenTree();
	setUpdatesEnabled(true);
	if (currDoc->m_Selection->count() > 0)
		slotShowSelect(0, -1);
	filterTree();
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

void OutlinePalette::parseSubGroup(OutlineTreeItem* object, QList<PageItem*> *subGroupList, int itemType)
{
	PageItem *pgItem;
	for (int b = 0; b < subGroupList->count(); ++b)
	{
		pgItem = subGroupList->at(b);
		if (!pgItem->isGroup())
		{
			OutlineTreeItem *grp = new OutlineTreeItem( object, 0 );
			grp->PageItemObject = pgItem;
			grp->type = itemType;
			grp->setText(0, pgItem->itemName());
			setItemIcon(grp, pgItem);
			grp->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
		}
		else
		{
			OutlineTreeItem *grp = new OutlineTreeItem( object, 0 );
			grp->PageItemObject = pgItem;
			grp->type = itemType;
			grp->setText(0, pgItem->itemName());
			grp->setIcon( 0, groupIcon );
			grp->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
			parseSubGroup(grp, &pgItem->groupItemList, itemType);
		}
	}
}

void OutlinePalette::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
		languageChange();
	else
		ScDockPalette::changeEvent(e);
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
