/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "tree.h"
#include "tree.moc"
#include <qheader.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qmessagebox.h>

#include "commonstrings.h"
#include "page.h"
#include "scribus.h"
#include "mpalette.h"

extern QPixmap loadIcon(QString nam);

TreeItem::TreeItem(TreeItem* parent, TreeItem* after) : QListViewItem(parent, after)
{
	PageObject = NULL;
	PageItemObject = NULL;
	type = -1;
}

TreeItem::TreeItem(QListView* parent, TreeItem* after) : QListViewItem(parent, after)
{
	PageObject = NULL;
	PageItemObject = NULL;
	type = -1;
}

Tree::Tree( QWidget* parent) : ScrPaletteBase( parent, "Tree", false, 0 )
{
	resize( 220, 240 );
	setMinimumSize( QSize( 220, 240 ) );
	setMaximumSize( QSize( 800, 600 ) );

	reportDisplay = new QListView( this, "ListView1" );

	reportDisplay->setGeometry( QRect( 0, 0, 220, 240 ) );
	reportDisplay->setMinimumSize( QSize( 220, 240 ) );
	reportDisplay->setRootIsDecorated( true );
	idElemCol=reportDisplay->addColumn("Element"); //Use width from initial untranslated string, translation is set with languageChange()
	reportDisplay->header()->setClickEnabled( false, reportDisplay->header()->count() - 1 );
	reportDisplay->header()->setResizeEnabled( false, reportDisplay->header()->count() - 1 );
	reportDisplay->setSorting(-1);
/*	reportDisplay->addColumn( tr("Type"));
	reportDisplay->header()->setClickEnabled( false, reportDisplay->header()->count() - 1 );
	reportDisplay->header()->setResizeEnabled( false, reportDisplay->header()->count() - 1 );
	reportDisplay->addColumn( tr("Information"));
	reportDisplay->header()->setClickEnabled( false, reportDisplay->header()->count() - 1 );
	reportDisplay->header()->setResizeEnabled( false, reportDisplay->header()->count() - 1 );
	reportDisplay->setSorting(-1); */
	reportDisplay->setSelectionMode(QListView::Single);
	reportDisplay->setDefaultRenameAction(QListView::Accept);
	unsetDoc();
	imageIcon = loadIcon("Bild.xpm");
	lineIcon = loadIcon("Stift.xpm");
	textIcon = loadIcon("Text.xpm");
	polylineIcon = loadIcon("beziertool.png");
	polygonIcon = loadIcon("spline.png");
	groupIcon = loadIcon("u_group.png");
	selectionTriggered = false;
	freeObjects = 0;
	languageChange();
	// signals and slots connections
	connect(reportDisplay, SIGNAL(selectionChanged(QListViewItem*)), this, SLOT(slotSelect(QListViewItem*)));
	connect(reportDisplay, SIGNAL(itemRenamed(QListViewItem*, int)), this, SLOT(slotDoRename(QListViewItem*, int)));
	connect(reportDisplay, SIGNAL(rightButtonClicked(QListViewItem *, const QPoint &, int)), this, SLOT(slotRightClick(QListViewItem*, const QPoint &, int)));
}

void Tree::setMainWindow(ScribusMainWindow *mw)
{
	m_MainWindow=mw;
	if (m_MainWindow==NULL)
		clearPalette();
}

void Tree::setDoc(ScribusDoc *newDoc)
{
	if (m_MainWindow==NULL)
		currDoc=NULL;
	else
		currDoc=newDoc;
	if (currDoc==NULL)
		clearPalette();
}

void Tree::unsetDoc()
{
	currDoc=NULL;
	clearPalette();
}

void Tree::setPaletteShown(bool visible)
{
	ScrPaletteBase::setPaletteShown(visible);
	if ((visible) && (currDoc != NULL))
		BuildTree();
}

void Tree::slotRightClick(QListViewItem *ite, const QPoint &, int col)
{
	if (ite == NULL)
		return;
	if (!m_MainWindow || m_MainWindow->ScriptRunning)
		return;
	TreeItem *item = (TreeItem*)ite;
	if (item != NULL)
	{
		if ((item->type == 1) || (item->type == 3) || (item->type == 4))
			ite->startRename(col);
	}
}

void Tree::slotDoRename(QListViewItem *ite , int col)
{
	if (!m_MainWindow || m_MainWindow->ScriptRunning)
		return;
	disconnect(reportDisplay, SIGNAL(itemRenamed(QListViewItem*, int)), this, SLOT(slotDoRename(QListViewItem*, int)));
	TreeItem *item = (TreeItem*)ite;
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
					for (uint b = 0; b < currDoc->Items->count(); ++b)
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
						item->PageItemObject->AutoName = false;
						m_MainWindow->propertiesPalette->SetCurItem(item->PageItemObject);
						currDoc->setModified(true);
						reportDisplay->setSelected(ite, true);
					}
				}
			}
		}
	}
	connect(reportDisplay, SIGNAL(itemRenamed(QListViewItem*, int)), this, SLOT(slotDoRename(QListViewItem*, int)));
}

QListViewItem* Tree::getListItem(uint SNr, int Nr)
{
	TreeItem *item = 0;
	QListViewItem *retVal = 0;
	if (currDoc->masterPageMode())
	{
		if (Nr == -1)
		{
			QListViewItemIterator it( reportDisplay );
			while ( it.current() )
			{
				item = (TreeItem*)it.current();
				if ((item->type == 0) && (item->PageObject->pageNr() == SNr))
				{
					retVal = it.current();
					break;
				}
				++it;
			}
		}
		else
		{
			QListViewItemIterator it( reportDisplay );
			while ( it.current() )
			{
				item = (TreeItem*)it.current();
				if ((item->type == 1) && (static_cast<int>(item->PageItemObject->ItemNr) == Nr))
				{
					retVal = it.current();
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
			QListViewItemIterator it( reportDisplay );
			while ( it.current() )
			{
				item = (TreeItem*)it.current();
				if ((item->type == 2) && (item->PageObject->pageNr() == SNr))
				{
					retVal = it.current();
					break;
				}
				++it;
			}
		}
		else
		{
			QListViewItemIterator it( reportDisplay );
			while ( it.current() )
			{
				item = (TreeItem*)it.current();
				if (((item->type == 3) || (item->type == 4)) && (static_cast<int>(item->PageItemObject->ItemNr) == Nr))
				{
					retVal = it.current();
					break;
				}
				++it;
			}
		}
	}
	return retVal;
}

void Tree::slotShowSelect(uint SNr, int Nr)
{
	if (!m_MainWindow || m_MainWindow->ScriptRunning)
		return;
	if (currDoc==NULL)
		return;
	if (currDoc->isLoading())
		return;
	if (selectionTriggered)
		return;
	disconnect(reportDisplay, SIGNAL(selectionChanged(QListViewItem*)), this, SLOT(slotSelect(QListViewItem*)));
	reportDisplay->clearSelection();
	QListViewItem *retVal = getListItem(SNr, Nr);
	if (retVal != 0)
		reportDisplay->setSelected(retVal, true);
	connect(reportDisplay, SIGNAL(selectionChanged(QListViewItem*)), this, SLOT(slotSelect(QListViewItem*)));
}

void Tree::slotRemoveElement(uint , uint )
{
/*	if (!m_MainWindow || m_MainWindow->ScriptRunning)
		return;
	if ((vie->Doc->masterPageMode) || (vie->Doc->loading))
		return;
	if (PageObj.count() != 0)
	{
		if (PageObj.at(SNr)->Elemente.count() != 0)
		{
			delete PageObj.at(SNr)->Elemente.at(Nr);
			PageObj.at(SNr)->Elemente.take(Nr);
		}
	} */
}

void Tree::slotUpdateElement(uint SNr, uint Nr)
{
	if (!m_MainWindow || m_MainWindow->ScriptRunning)
		return;
/*	QListViewItem *item = getListItem(SNr, Nr);
	if (!item)
	{
		qDebug("Tree::slotUpdateElement(%i,%i): NULL item from getListItem(...)",SNr,Nr);
		return;
	}
	PageItem* pgItem;
	if (currDoc->masterPageMode())
		pgItem = currDoc->MasterItems.at(Nr);
	else
		pgItem = currDoc->DocItems.at(Nr);
	if ((pgItem->Groups.count() != 0) && (!pgItem->isSingleSel))
		return;
	setItemIcon(item, pgItem->itemType()); */
/*	QString cc, xp, yp, fon, GroupTxt;
	if ((vie->Doc->masterPageMode) || (vie->Doc->loading))
		return;
	if (SNr > Seiten.count()-1)
		return;
	if ( Nr > PageObj.at(SNr)->Elemente.count()-1)
		return;
	disconnect(ListView1, SIGNAL(itemRenamed(QListViewItem*, int)), this, SLOT(slotDoRename(QListViewItem*, int)));
	disconnect(ListView1, SIGNAL(selectionChanged(QListViewItem*)), this, SLOT(slotSelect(QListViewItem*)));
	PageObj.at(SNr)->Elemente.at(Nr)->setText(0, vie->Pages.at(SNr)->Items.at(Nr)->AnName);
	xp = tr("X:")+" "+cc.setNum(vie->Pages.at(SNr)->Items.at(Nr)->Xpos);
	yp = tr("Y:")+" "+cc.setNum(vie->Pages.at(SNr)->Items.at(Nr)->Ypos);
	fon = tr("Font:")+" "+vie->Pages.at(SNr)->Items.at(Nr)->IFont;
	if (vie->Pages.at(SNr)->Items.at(Nr)->Groups.count() != 0)
	{
		GroupTxt = tr("Group ")+cc.setNum(vie->Pages.at(SNr)->Items.at(Nr)->Groups.top())+" ";
		xp.prepend(GroupTxt);
	}
	switch (vie->Pages.at(SNr)->Items.at(Nr)->PType)
	{
	case 2:
		PageObj.at(SNr)->Elemente.at(Nr)->setText(1, tr("Image"));
		PageObj.at(SNr)->Elemente.at(Nr)->setText(2, xp+" "+yp+" "+vie->Pages.at(SNr)->Items.at(Nr)->Pfile);
		break;
	case 4:
		PageObj.at(SNr)->Elemente.at(Nr)->setText(1, tr("Text"));
		PageObj.at(SNr)->Elemente.at(Nr)->setText(2, xp+" "+yp+" "+fon);
		break;
	case 5:
		PageObj.at(SNr)->Elemente.at(Nr)->setText(1, tr("Line"));
		PageObj.at(SNr)->Elemente.at(Nr)->setText(2, xp+" "+yp);
		break;
	case 6:
		PageObj.at(SNr)->Elemente.at(Nr)->setText(1, tr("Polygon"));
		PageObj.at(SNr)->Elemente.at(Nr)->setText(2, xp+" "+yp);
		break;
	case 7:
		PageObj.at(SNr)->Elemente.at(Nr)->setText(1, tr("Polyline"));
		PageObj.at(SNr)->Elemente.at(Nr)->setText(2, xp+" "+yp);
		break;
	case 8:
		PageObj.at(SNr)->Elemente.at(Nr)->setText(1, tr("PathText"));
		PageObj.at(SNr)->Elemente.at(Nr)->setText(2, xp+" "+yp+" "+fon);
		break;
	}
	connect(ListView1, SIGNAL(itemRenamed(QListViewItem*, int)), this, SLOT(slotDoRename(QListViewItem*, int)));
	connect(ListView1, SIGNAL(selectionChanged(QListViewItem*)), this, SLOT(slotSelect(QListViewItem*))); */
}

void Tree::setItemIcon(QListViewItem *item, int typ)
{
	switch (typ)
	{
	case PageItem::ImageFrame:
		item->setPixmap( 0, imageIcon );
		break;
	case PageItem::TextFrame:
		item->setPixmap( 0, textIcon );
		break;
	case PageItem::Line:
		item->setPixmap( 0, lineIcon );
		break;
	case PageItem::Polygon:
		item->setPixmap( 0, polygonIcon );
		break;
	case PageItem::PolyLine:
		item->setPixmap( 0, polylineIcon );
		break;
	case PageItem::PathText:
		item->setPixmap( 0, textIcon );
		break;
	default:
		break;
	}
}

void Tree::slotAddElement(PageItem *item)
{
	if (!m_MainWindow || m_MainWindow->ScriptRunning)
		return;
	if (currDoc->isLoading())
		return;
/*	if (!item->OnMasterPage.isEmpty())
	{
		if (masterPageItemMapRev.contains(item->ItemNr))
			return;
	}
	else
	{
		if (itemMapRev.contains(item->ItemNr))
			return;
	}
	disconnect(reportDisplay, SIGNAL(selectionChanged(QListViewItem*)), this, SLOT(slotSelect(QListViewItem*)));
	QListViewItem * object;
	if (!item->OnMasterPage.isEmpty())
	{
		QListViewItem * page = masterPageMapRev[item->OnMasterPage];
		object = new QListViewItem( page, 0 );
		masterPageItemMap.insert(object, item->ItemNr);
		masterPageItemMapRev.insert(item->ItemNr, object);
	}
	else
	{
		if (item->OwnPage != -1)
		{
			QListViewItem * page = pageMapRev[item->OwnPage];
			object = new QListViewItem( page, 0 );
			itemMap.insert(object, item->ItemNr);
			itemMapRev.insert(item->ItemNr, object);
		}
		else
		{
			QListViewItem * page = freeObjects;
			if (freeObjects == 0)
			{
				page = new QListViewItem( rootObject, 0 );
				page->setText(0, tr("Free items"));
				freeObjects = page;
			}
			object = new QListViewItem( page, 0 );
			itemMap.insert(object, item->ItemNr);
			itemMapRev.insert(item->ItemNr, object);
		}
	}
	object->setText(0, item->itemName());
	setItemIcon(object, item->itemType());
	reportDisplay->clearSelection();
	reportDisplay->setSelected(object, true);
	connect(reportDisplay, SIGNAL(selectionChanged(QListViewItem*)), this, SLOT(slotSelect(QListViewItem*))); */
}

void Tree::slotMoveElement(uint , uint , uint )
{
/*	if (!m_MainWindow || m_MainWindow->ScriptRunning)
		return;
	if ((vie->Doc->masterPageMode) || (vie->Doc->loading))
		return;
	QListViewItem* tmp = PageObj.at(SNr)->Elemente.take(NrOld);
	PageObj.at(SNr)->Elemente.insert(NrNew, tmp); */
}

void Tree::slotAddPage(uint )
{
/*	if (!m_MainWindow || m_MainWindow->ScriptRunning)
		return;
	QString cc;
	if (ListView1->childCount() == 0)
		return;
	Seiten.insert(Nr, new QListViewItem(ListView1->firstChild(), "Seiten"));
	Seiten.current()->setText(0, tr("Page")+" "+cc.setNum(Nr+1));
	PageObj.insert(Nr, new Elem);
	rebuildPageD(); */
}

void Tree::slotDelPage(uint )
{
/*	if (!m_MainWindow || m_MainWindow->ScriptRunning)
		return;
	if (vie->Doc->masterPageMode)
		return;
	if (Seiten.count() != 0)
	{
		delete Seiten.at(Nr);
		Seiten.take(Nr);
		PageObj.take(Nr);
		rebuildPageD();
	} */
}

void Tree::rebuildPageD()
{
/*	if (!m_MainWindow || m_MainWindow->ScriptRunning)
		return;
	QString cc,tmpstr;
	uint pagenumwidth;

	tmpstr.setNum( Seiten.count() );
	pagenumwidth=tmpstr.length();
	for (uint e = 0; e < Seiten.count(); ++e)
	{
		tmpstr.setNum(e+1);
		cc = tmpstr.rightJustify (pagenumwidth, '0');
		Seiten.at(e)->setText(0, tr("Page")+" "+cc);
	} */
}

void Tree::reopenTree()
{
	if (!m_MainWindow || m_MainWindow->ScriptRunning)
		return;
	if (currDoc->OpenNodes.count() == 0)
		return;
	TreeItem *item = 0;
	QListViewItemIterator it( reportDisplay );
	while ( it.current() )
	{
		item = (TreeItem*)it.current();
		for (uint olc = 0; olc < currDoc->OpenNodes.count(); olc++)
		{
			if (item->type == currDoc->OpenNodes[olc].type)
			{
				if ((item->type == -3) || (item->type == -2))
					reportDisplay->setOpen(it.current(), true);
				else if ((item->type == 0) || (item->type == 2))
				{
					if (item->PageObject == currDoc->OpenNodes[olc].page)
						reportDisplay->setOpen(it.current(), true);
				}
				else if ((item->type == 2) || (item->type == 3) || (item->type == 4))
				{
					if (item->PageItemObject == currDoc->OpenNodes[olc].item)
						reportDisplay->setOpen(it.current(), true);
				}
			}
		}
		++it;
	}
}

void Tree::buildReopenVals()
{
	ScribusDoc::OpenNodesList ol;
	if (reportDisplay->childCount() == 0)
		return;
	currDoc->OpenNodes.clear();
	TreeItem *item = 0;
	QListViewItemIterator it( reportDisplay );
	while ( it.current() )
	{
		item = (TreeItem*)it.current();
		if (item->isOpen())
		{
			ol.type = item->type;
			ol.page = item->PageObject;
			ol.item = item->PageItemObject;
			currDoc->OpenNodes.append(ol);
		}
		++it;
	}
}

void Tree::slotSelect(QListViewItem* ite)
{
	if (!m_MainWindow || m_MainWindow->ScriptRunning)
		return;
	disconnect(reportDisplay, SIGNAL(selectionChanged(QListViewItem*)), this, SLOT(slotSelect(QListViewItem*)));
	selectionTriggered = true;
	TreeItem *item = (TreeItem*)ite;
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
	connect(reportDisplay, SIGNAL(selectionChanged(QListViewItem*)), this, SLOT(slotSelect(QListViewItem*)));
}

/*
void Tree::closeEvent(QCloseEvent *ce)
{
	emit Schliessen();
	ce->accept();
}

void Tree::reject()
{
	emit Schliessen();
	QDialog::reject();
}
*/
void Tree::resizeEvent(QResizeEvent *r)
{
	reportDisplay->resize(r->size());
}

void Tree::BuildTree(bool storeVals)
{
	if (!m_MainWindow || m_MainWindow->ScriptRunning)
		return;
	if (currDoc==NULL)
		return;
	if (selectionTriggered)
		return;
	disconnect(reportDisplay, SIGNAL(selectionChanged(QListViewItem*)), this, SLOT(slotSelect(QListViewItem*)));
	setUpdatesEnabled(false);
	if (storeVals)
		buildReopenVals();
	clearPalette();
	QPtrList<PageItem> subGroupList;
	TreeItem * item = new TreeItem( reportDisplay, 0 );
	rootObject = item;
	item->setText( 0, currDoc->DocName.section( '/', -1 ) );
	item->type = -2;
	TreeItem * pagep = 0;
	freeObjects = 0;
	PageItem* pgItem;
	QString tmp;
	for (uint b = 0; b < currDoc->MasterItems.count(); ++b)
	{
		currDoc->MasterItems.at(b)->Dirty = false;
	}
	for (int a = 0; a < static_cast<int>(currDoc->MasterPages.count()); ++a)
	{
		TreeItem *page = new TreeItem( item, pagep );
		page->PageObject = currDoc->MasterPages.at(a);
		page->type = 0;
		QString pageNam = currDoc->MasterPages.at(a)->pageName();
		pagep = page;
		for (uint b = 0; b < currDoc->MasterItems.count(); ++b)
		{
			pgItem = currDoc->MasterItems.at(b);
			if ((pgItem->OwnPage == a) || (pgItem->OnMasterPage == pageNam))
			{
				if (pgItem->Groups.count() == 0)
				{
					TreeItem *object = new TreeItem( page, 0 );
					object->PageItemObject = pgItem;
					object->type = 1;
					object->setText(0, pgItem->itemName());
					setItemIcon(object, pgItem->itemType());
					object->setRenameEnabled(0, true);
					pgItem->Dirty = true;
				}
				else
				{
					TreeItem * object = new TreeItem( page, 0 );
					object->PageItemObject = pgItem;
					object->type = 1;
					if (pgItem->isGroupControl)
						object->setText(0, pgItem->itemName());
					else
						object->setText(0, tr("Group ")+tmp.setNum(pgItem->Groups.top()));
					object->setPixmap( 0, groupIcon );
					object->setRenameEnabled(0, true);
					pgItem->Dirty = true;
					subGroupList.clear();
					for (uint ga = 0; ga < currDoc->MasterItems.count(); ++ga)
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
	for (uint b = 0; b < currDoc->DocItems.count(); ++b)
	{
		currDoc->DocItems.at(b)->Dirty = false;
	}
	for (int a = 0; a < static_cast<int>(currDoc->DocPages.count()); ++a)
	{
		TreeItem *page = new TreeItem( item, pagep );
		page->PageObject = currDoc->DocPages.at(a);
		page->type = 2;
		pagep = page;
		for (uint b = 0; b < currDoc->DocItems.count(); ++b)
		{
			pgItem = currDoc->DocItems.at(b);
			if ((pgItem->OwnPage == a) && (!pgItem->Dirty))
			{
				if (pgItem->Groups.count() == 0)
				{
					TreeItem *object = new TreeItem( page, 0 );
					object->PageItemObject = pgItem;
					object->type = 3;
					object->setText(0, pgItem->itemName());
					setItemIcon(object, pgItem->itemType());
					object->setRenameEnabled(0, true);
					pgItem->Dirty = true;
				}
				else
				{
					TreeItem *object = new TreeItem( page, 0 );
					object->PageItemObject = pgItem;
					object->type = 3;
					if (pgItem->isGroupControl)
						object->setText(0, pgItem->itemName());
					else
						object->setText(0, tr("Group ")+tmp.setNum(pgItem->Groups.top()));
					object->setPixmap( 0, groupIcon );
					object->setRenameEnabled(0, true);
					pgItem->Dirty = true;
					subGroupList.clear();
					for (uint ga = 0; ga < currDoc->DocItems.count(); ++ga)
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
	for (uint b = 0; b < currDoc->DocItems.count(); ++b)
	{
		if (currDoc->DocItems.at(b)->OwnPage == -1)
		{
			hasfreeItems = true;
			break;
		}
	}
	if (hasfreeItems)
	{
		TreeItem *page = new TreeItem( item, pagep );
		pagep = page;
		freeObjects = page;
		page->type = -3;
		for (uint b = 0; b < currDoc->DocItems.count(); ++b)
		{
			pgItem = currDoc->DocItems.at(b);
			if ((pgItem->OwnPage == -1) && (!pgItem->Dirty))
			{
				if (pgItem->Groups.count() == 0)
				{
					TreeItem *object = new TreeItem( page, 0 );
					object->PageItemObject = pgItem;
					object->type = 4;
					object->setText(0, pgItem->itemName());
					setItemIcon(object, pgItem->itemType());
					object->setRenameEnabled(0, true);
					pgItem->Dirty = true;
				}
				else
				{
					TreeItem *object = new TreeItem( page, 0 );
					object->PageItemObject = pgItem;
					object->type = 4;
					if (pgItem->isGroupControl)
						object->setText(0, pgItem->itemName());
					else
						object->setText(0, tr("Group ")+tmp.setNum(pgItem->Groups.top()));
					object->setPixmap( 0, groupIcon );
					object->setRenameEnabled(0, true);
					pgItem->Dirty = true;
					subGroupList.clear();
					for (uint ga = 0; ga < currDoc->DocItems.count(); ++ga)
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
	repaint();
	connect(reportDisplay, SIGNAL(selectionChanged(QListViewItem*)), this, SLOT(slotSelect(QListViewItem*)));
}

void Tree::parseSubGroup(int level, TreeItem* object, QPtrList<PageItem> *subGroupList, int itemType)
{
	QPtrList<PageItem> *subGroup;
	PageItem *pgItem;
	QString tmp;
	for (uint b = 0; b < subGroupList->count(); ++b)
	{
		pgItem = subGroupList->at(b);
		if (!pgItem->Dirty)
		{
			if (static_cast<int>(pgItem->Groups.count()) <= level)
			{
				TreeItem *grp = new TreeItem( object, 0 );
				grp->PageItemObject = pgItem;
				grp->type = itemType;
				grp->setText(0, pgItem->itemName());
				setItemIcon(grp, pgItem->itemType());
				grp->setRenameEnabled(0, true);
				pgItem->Dirty = true;
			}
			else
			{
				TreeItem *grp = new TreeItem( object, 0 );
				grp->PageItemObject = pgItem;
				grp->type = itemType;
				if (pgItem->isGroupControl)
					grp->setText(0, pgItem->itemName());
				else
				grp->setText(0, tr("Group ")+tmp.setNum(*pgItem->Groups.at(pgItem->Groups.count()-level-1)));
				grp->setPixmap( 0, groupIcon );
				grp->setRenameEnabled(0, true);
				pgItem->Dirty = true;
				subGroup = new QPtrList<PageItem>;
				subGroup->clear();
				for (uint ga = 0; ga < subGroupList->count(); ++ga)
				{
					PageItem* pgItem2 = subGroupList->at(ga);
					if ((static_cast<int>(pgItem2->Groups.count()) > level) && 
						(*(pgItem2->Groups.at(pgItem2->Groups.count()-level-1)) == (*pgItem->Groups.at(pgItem->Groups.count()-level-1))) && (pgItem2 != pgItem))
						subGroup->append(pgItem2);
				}
				parseSubGroup(level+1, grp, subGroup, itemType);
				delete subGroup;
			}
		}
	}
}

void Tree::languageChange()
{
	setCaption( tr("Outline"));
	reportDisplay->setColumnText(idElemCol, tr("Element"));
}

void Tree::clearPalette()
{
	//GUI
	reportDisplay->clear();
}
