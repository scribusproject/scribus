#include "tree.h"
#include "tree.moc"
#include <qheader.h>
#include <qlistview.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qmessagebox.h>
#include "scribus.h"
extern QPixmap loadIcon(QString nam);

Tree::Tree( QWidget* parent, ScribusApp* scApp ) : QDialog( parent, "Tree", false, 0 )
{
	ScApp = scApp;
	resize( 220, 240 );
	setMinimumSize( QSize( 220, 240 ) );
	setMaximumSize( QSize( 800, 600 ) );
	setCaption( tr( "Outline" ) );
	setIcon(loadIcon("AppIcon.png"));

	reportDisplay = new QListView( this, "ListView1" );

	reportDisplay->setGeometry( QRect( 0, 0, 220, 240 ) );
	reportDisplay->setMinimumSize( QSize( 220, 240 ) );
	reportDisplay->setRootIsDecorated( true );
	reportDisplay->addColumn( tr("Element"));
	reportDisplay->header()->setClickEnabled( false, reportDisplay->header()->count() - 1 );
	reportDisplay->header()->setResizeEnabled( false, reportDisplay->header()->count() - 1 );
	reportDisplay->addColumn( tr("Type"));
	reportDisplay->header()->setClickEnabled( false, reportDisplay->header()->count() - 1 );
	reportDisplay->header()->setResizeEnabled( false, reportDisplay->header()->count() - 1 );
	reportDisplay->addColumn( tr("Information"));
	reportDisplay->header()->setClickEnabled( false, reportDisplay->header()->count() - 1 );
	reportDisplay->header()->setResizeEnabled( false, reportDisplay->header()->count() - 1 );
	reportDisplay->setSorting(-1);
	reportDisplay->setSelectionMode(QListView::Single);
//	reportDisplay->setDefaultRenameAction(QListView::Accept);
	itemMap.clear();
	pageMap.clear();
	templatePageMap.clear();
	templateItemMap.clear();

	// signals and slots connections
	connect(reportDisplay, SIGNAL(selectionChanged(QListViewItem*)), this, SLOT(slotSelect(QListViewItem*)));
	connect(reportDisplay, SIGNAL(itemRenamed(QListViewItem*, int)), this, SLOT(slotDoRename(QListViewItem*, int)));
	connect(reportDisplay, SIGNAL(rightButtonClicked(QListViewItem *, const QPoint &, int)), this, SLOT(slotRightClick(QListViewItem*, const QPoint &, int)));
}

void Tree::keyPressEvent(QKeyEvent *k)
{
	int KeyMod;
	int kk = k->key();
	switch (k->state())
	{
	case ShiftButton:
		KeyMod = 0x00200000;
		break;
	case AltButton:
		KeyMod = 0x00800000;
		break;
	case ControlButton:
		KeyMod = 0x00400000;
		break;
	default:
		KeyMod = 0;
		break;
	}
	if (kk == Key_F10)
		emit ToggleAllPalettes();
	/*
	if ((kk + KeyMod) == ScApp->Prefs.KeyActions[46].KeyID)
		emit CloseMpal();
	if ((kk + KeyMod) == ScApp->Prefs.KeyActions[47].KeyID)
		emit Schliessen();
	if ((kk + KeyMod) == ScApp->Prefs.KeyActions[48].KeyID)
		emit CloseSpal();
	*/
	QDialog::keyPressEvent(k);
}

void Tree::slotRightClick(QListViewItem* ite, const QPoint &, int)
{
/*	if (ite == NULL)
		return;
	if (ScApp->ScriptRunning)
		return;
	if (vie->Doc->TemplateMode)
		return;
	if (Seiten.containsRef(ite))
		return;
	for (uint e = 0; e < PageObj.count(); ++e)
	{
		if (PageObj.at(e)->Elemente.containsRef(ite))
		{
			ite->startRename(0);
			break;
		}
	} */
}

void Tree::slotDoRename(QListViewItem* ite, int col)
{
/*	if (ScApp->ScriptRunning)
		return;
	if (vie->Doc->TemplateMode)
		return;
	disconnect(ListView1, SIGNAL(itemRenamed(QListViewItem*, int)), this, SLOT(slotDoRename(QListViewItem*, int)));
	int sref, oref;
	for (uint e = 0; e < PageObj.count(); ++e)
	{
		if (PageObj.at(e)->Elemente.containsRef(ite))
		{
			oref = PageObj.at(e)->Elemente.findRef(ite);
			if (oref != -1)
			{
				sref = Seiten.findRef(ite->parent());
				if (sref != -1)
				{
					QString NameOld = vie->Pages.at(sref)->Items.at(oref)->AnName;
					QString NameNew = ite->text(col);
					if (NameOld == NameNew)
						break;
					if (NameNew == "")
					{
						ite->setText(col, NameOld);
						break;
					}
					bool found = false;
					for (uint a = 0; a < vie->Pages.count(); ++a)
					{
						for (uint b = 0; b < vie->Pages.at(a)->Items.count(); ++b)
						{
							if (NameNew == vie->Pages.at(a)->Items.at(b)->AnName)
							{
								found = true;
								break;
							}
						}
						if (found)
							break;
					}
					if (found)
					{
						QMessageBox::warning(this, tr("Warning"), tr("Name \"%1\" isn't unique.\nPlease choose another.").arg(NameNew), tr("OK"));
						ite->setText(col, NameOld);
					}
					else
					{
						vie->Pages.at(sref)->Items.at(oref)->AnName = NameNew;
						vie->Pages.at(sref)->Items.at(oref)->AutoName = false;
						ScApp->slotDocCh(false);
						ScApp->HaveNewSel(vie->Pages.at(sref)->Items.at(oref)->PType);
						break;
					}
				}
			}
		}
	}
	connect(ListView1, SIGNAL(itemRenamed(QListViewItem*, int)), this, SLOT(slotDoRename(QListViewItem*, int))); */
}

void Tree::slotShowSelect(uint SNr, int Nr)
{
/*	if (ScApp->ScriptRunning)
		return;
	if ((vie->Doc->TemplateMode) || (vie->Doc->loading))
		return;
	disconnect(ListView1, SIGNAL(selectionChanged(QListViewItem*)), this, SLOT(slotSelect(QListViewItem*)));
	ListView1->clearSelection();
	if (Nr != -1)
		ListView1->setSelected(PageObj.at(SNr)->Elemente.at(Nr), true);
	else
		ListView1->setSelected(Seiten.at(SNr), true);
	connect(ListView1, SIGNAL(selectionChanged(QListViewItem*)), this, SLOT(slotSelect(QListViewItem*))); */
}

void Tree::slotRemoveElement(uint SNr, uint Nr)
{
/*	if (ScApp->ScriptRunning)
		return;
	if ((vie->Doc->TemplateMode) || (vie->Doc->loading))
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
/*	if (ScApp->ScriptRunning)
		return;
	QString cc, xp, yp, fon, GroupTxt;
	if ((vie->Doc->TemplateMode) || (vie->Doc->loading))
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

void Tree::slotAddElement(uint SNr, uint Nr)
{
/*	if (ScApp->ScriptRunning)
		return;
	if ((vie->Doc->TemplateMode) || (vie->Doc->loading))
		return;
	PageObj.at(SNr)->Elemente.insert(Nr, new QListViewItem(Seiten.at(SNr), "Items"));
	slotUpdateElement(SNr, Nr);
	PageObj.at(SNr)->Elemente.at(Nr)->setRenameEnabled(0, true); */
}

void Tree::slotMoveElement(uint SNr, uint NrOld, uint NrNew)
{
/*	if (ScApp->ScriptRunning)
		return;
	if ((vie->Doc->TemplateMode) || (vie->Doc->loading))
		return;
	QListViewItem* tmp = PageObj.at(SNr)->Elemente.take(NrOld);
	PageObj.at(SNr)->Elemente.insert(NrNew, tmp); */
}

void Tree::slotAddPage(uint Nr)
{
/*	if (ScApp->ScriptRunning)
		return;
	QString cc;
	if (ListView1->childCount() == 0)
		return;
	Seiten.insert(Nr, new QListViewItem(ListView1->firstChild(), "Seiten"));
	Seiten.current()->setText(0, tr("Page")+" "+cc.setNum(Nr+1));
	PageObj.insert(Nr, new Elem);
	rebuildPageD(); */
}

void Tree::slotDelPage(uint Nr)
{
/*	if (ScApp->ScriptRunning)
		return;
	if (vie->Doc->TemplateMode)
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
/*	if (ScApp->ScriptRunning)
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

void Tree::reopenTree(QValueList<int> op)
{
/*	if (ScApp->ScriptRunning)
		return;
	if (op.count() == 0)
		return;
	if (op[0] == 1)
		ListView1->setOpen(ListView1->firstChild(), true);
	for (uint e = 1; e < op.count(); ++e)
	{
		ListView1->setOpen(Seiten.at(op[e]), true);
	} */
}

QValueList<int> Tree::buildReopenVals()
{
	QValueList<int> op;
	op.clear();
/*	if (ListView1->childCount() == 0)
		return op;
	if (ListView1->firstChild()->isOpen())
		op.append(1);
	else
		op.append(0);
	for (uint e = 0; e < Seiten.count(); ++e)
	{
		if (ListView1->isOpen(Seiten.at(e)))
			op.append(e);
	} */
	return op;
}

void Tree::slotSelect(QListViewItem* ite)
{
	if (ScApp->ScriptRunning)
		return;
	if (itemMap.contains(ite))
	{
		if (document->TemplateMode)
			ScApp->ActWin->muster->close();
		emit selectElement(document->DocItems.at(itemMap[ite])->OwnPage, itemMap[ite]);
		return;
	}
	if (groupMap.contains(ite))
	{
		if (document->TemplateMode)
			ScApp->ActWin->muster->close();
		emit selectElement(document->DocItems.at(groupMap[ite])->OwnPage, groupMap[ite]);
		return;
	}
	if (pageMap.contains(ite))
	{
		if (document->TemplateMode)
			ScApp->ActWin->muster->close();
		emit selectPage(pageMap[ite]);
		return;
	}
	if (templatePageMap.contains(ite))
	{
		emit selectTemplatePage(templatePageMap[ite]);
		return;
	}
	if (templateItemMap.contains(ite))
	{
		if (!document->TemplateMode)
			emit selectTemplatePage(document->MasterItems.at(templateItemMap[ite])->OnMasterPage);
		emit selectElement(-1, templateItemMap[ite]);
		return;
	}
	if (templateGroupMap.contains(ite))
	{
		if (!document->TemplateMode)
			emit selectTemplatePage(document->MasterItems.at(templateGroupMap[ite])->OnMasterPage);
		emit selectElement(-1, templateGroupMap[ite]);
		return;
	}
}

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

void Tree::resizeEvent(QResizeEvent *r)
{
	reportDisplay->resize(r->size());
}

void Tree::BuildTree(ScribusDoc *doc)
{
	document = doc;
	if (ScApp->ScriptRunning)
		return;
	disconnect(reportDisplay, SIGNAL(selectionChanged(QListViewItem*)), this, SLOT(slotSelect(QListViewItem*)));
	reportDisplay->clear();
	itemMap.clear();
	pageMap.clear();
	groupMap.clear();
	templateGroupMap.clear();
	templatePageMap.clear();
	templateItemMap.clear();
	QPtrList<PageItem> subGroupList;
	QListViewItem * item = new QListViewItem( reportDisplay, 0 );
	item->setText( 0, doc->DocName );
	QListViewItem * pagep = 0;
	PageItem* pgItem;
	QString tmp;
	for (uint b = 0; b < doc->MasterItems.count(); ++b)
	{
		doc->MasterItems.at(b)->Dirty = false;
	}
	for (int a = 0; a < static_cast<int>(doc->MasterPages.count()); ++a)
	{
		QListViewItem * page = new QListViewItem( item, pagep );
		QString pageNam = doc->MasterPages.at(a)->PageNam;
		templatePageMap.insert(page, pageNam);
		pagep = page;
		for (uint b = 0; b < doc->MasterItems.count(); ++b)
		{
			pgItem = doc->MasterItems.at(b);
			if ((pgItem->OwnPage == a) || (pgItem->OnMasterPage == pageNam))
			{
				if (pgItem->Groups.count() == 0)
				{
					QListViewItem * object = new QListViewItem( page, 0 );
					templateItemMap.insert(object, pgItem->ItemNr);
					object->setText(0, pgItem->itemName());
				}
				else
				{
						QListViewItem * object = new QListViewItem( page, 0 );
						object->setText(0, tr("Group ")+tmp.setNum(pgItem->Groups.top()));
						subGroupList.clear();
						for (uint ga = 0; ga < doc->MasterItems.count(); ++ga)
						{
							PageItem* pgItem2 = doc->MasterItems.at(ga);
							if ((pgItem2->Groups.count() != 0) && (pgItem2->Groups.top() == pgItem->Groups.top()))
								subGroupList.append(pgItem2);
						}
						parseSubGroup(1, object, &subGroupList, true);
						templateGroupMap.insert(object, pgItem->ItemNr);
				}
			}
		}
		page->setText(0, doc->MasterPages.at(a)->PageNam);
	}
	for (uint b = 0; b < doc->DocItems.count(); ++b)
	{
		doc->DocItems.at(b)->Dirty = false;
	}
	for (int a = 0; a < static_cast<int>(doc->DocPages.count()); ++a)
	{
		QListViewItem * page = new QListViewItem( item, pagep );
		pageMap.insert(page, a);
		pagep = page;
		for (uint b = 0; b < doc->DocItems.count(); ++b)
		{
			pgItem = doc->DocItems.at(b);
			if ((pgItem->OwnPage == a) && (!pgItem->Dirty))
			{
				if (pgItem->Groups.count() == 0)
				{
						QListViewItem * object = new QListViewItem( page, 0 );
						object->setText(0, pgItem->itemName());
						itemMap.insert(object, pgItem->ItemNr);
						pgItem->Dirty = true;
				}
				else
				{
						QListViewItem * object = new QListViewItem( page, 0 );
						object->setText(0, tr("Group ")+tmp.setNum(pgItem->Groups.top()));
						subGroupList.clear();
						for (uint ga = 0; ga < doc->DocItems.count(); ++ga)
						{
							PageItem* pgItem2 = doc->DocItems.at(ga);
							if ((pgItem2->Groups.count() != 0) && (pgItem2->Groups.top() == pgItem->Groups.top()))
								subGroupList.append(pgItem2);
						}
						parseSubGroup(1, object, &subGroupList, false);
						groupMap.insert(object, pgItem->ItemNr);
				}
			}
		}
		page->setText(0, tr("Page ")+tmp.setNum(a+1));
	}
	bool hasfreeItems = false;
	for (uint b = 0; b < doc->DocItems.count(); ++b)
	{
		if (doc->DocItems.at(b)->OwnPage == -1)
		{
			hasfreeItems = true;
			break;
		}
	}
	if (hasfreeItems)
	{
		QListViewItem * page = new QListViewItem( item, pagep );
		pagep = page;
		for (uint b = 0; b < doc->DocItems.count(); ++b)
		{
			pgItem = doc->DocItems.at(b);
			if ((pgItem->OwnPage == -1) && (!pgItem->Dirty))
			{
				if (pgItem->Groups.count() == 0)
				{
					QListViewItem * object = new QListViewItem( page, 0 );
					object->setText(0, pgItem->itemName());
					itemMap.insert(object, pgItem->ItemNr);
				}
				else
				{
						QListViewItem * object = new QListViewItem( page, 0 );
						object->setText(0, tr("Group ")+tmp.setNum(pgItem->Groups.top()));
						subGroupList.clear();
						for (uint ga = 0; ga < doc->DocItems.count(); ++ga)
						{
							PageItem* pgItem2 = doc->DocItems.at(ga);
							if ((pgItem2->Groups.count() != 0) && (pgItem2->Groups.top() == pgItem->Groups.top()))
								subGroupList.append(pgItem2);
						}
						parseSubGroup(1, object, &subGroupList, false);
						groupMap.insert(object, pgItem->ItemNr);
				}
			}
		}
		page->setText(0, tr("Free Objects"));
	}
	for (uint b = 0; b < doc->DocItems.count(); ++b)
	{
		doc->DocItems.at(b)->Dirty = false;
	}
	for (uint b = 0; b < doc->MasterItems.count(); ++b)
	{
		doc->MasterItems.at(b)->Dirty = false;
	}
	connect(reportDisplay, SIGNAL(selectionChanged(QListViewItem*)), this, SLOT(slotSelect(QListViewItem*)));
}

void Tree::parseSubGroup(int level, QListViewItem* object, QPtrList<PageItem> *subGroupList, bool onTemplate)
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
					QListViewItem *grp = new QListViewItem( object, 0 );
					grp->setText(0, pgItem->itemName());
					if (onTemplate)
						templateItemMap.insert(grp, pgItem->ItemNr);
					else
						itemMap.insert(grp, pgItem->ItemNr);
					pgItem->Dirty = true;
			}
			else
			{
					QListViewItem *grp = new QListViewItem( object, 0 );
					grp->setText(0, tr("Group ")+tmp.setNum(*pgItem->Groups.at(pgItem->Groups.count()-level-1)));
					subGroup = new QPtrList<PageItem>;
					subGroup->clear();
					for (uint ga = 0; ga < subGroupList->count(); ++ga)
					{
						PageItem* pgItem2 = subGroupList->at(ga);
						if ((static_cast<int>(pgItem2->Groups.count()) > level) && 
							(*(pgItem2->Groups.at(pgItem2->Groups.count()-level-1)) == (*pgItem->Groups.at(pgItem->Groups.count()-level-1))))
							subGroup->append(pgItem2);
					}
					parseSubGroup(level+1, grp, subGroup, onTemplate);
					delete subGroup;
					if (onTemplate)
						templateGroupMap.insert(grp, pgItem->ItemNr);
					else
						groupMap.insert(grp, pgItem->ItemNr);
			}
		}
	}
}


/*

			for (uint ga=0; ga<Doc->Items.count(); ++ga)
			{
				if (Doc->Items.at(ga)->Groups.count() != 0)
				{
					if (Doc->Items.at(ga)->Groups.top() == b->Groups.top())
					{
						if (Doc->Items.at(ga)->ItemNr != b->ItemNr)
						{
							if (SelItem.find(Doc->Items.at(ga)) == -1)
								SelItem.append(Doc->Items.at(ga));
						}
						Doc->Items.at(ga)->Select = true;
						Doc->Items.at(ga)->FrameOnly = true;
						Doc->Items.at(ga)->paintObj();
					}
				}
			}
	disconnect(ListView1, SIGNAL(itemRenamed(QListViewItem*, int)), this, SLOT(slotDoRename(QListViewItem*, int)));
	uint a, b, pagenumwidth;
	QString cc, tmpstr;
	PageObj.clear();
	Seiten.clear();
	ListView1->clear();
	vie = view;
	QListViewItem * item = new QListViewItem( ListView1, 0 );
	item->setText( 0, view->Doc->DocName);
	tmpstr.setNum (view->Doc->Pages.count() );
	pagenumwidth = tmpstr.length();
	for (a = 0; a < view->Doc->Pages.count(); ++a)
	{
		tmpstr.setNum(a+1);
		cc = tmpstr.rightJustify (pagenumwidth, '0');
		Seiten.append(new QListViewItem(item, "Seiten"));
		Seiten.current()->setText(0, tr("Page")+" "+cc);
		PageObj.append(new Elem);
		for (b = 0; b < view->Doc->Items.count(); b++)
		{
			PageObj.current()->Elemente.append(new QListViewItem(Seiten.current(), "Items"));
			slotUpdateElement(a, b);
			PageObj.at(a)->Elemente.at(b)->setRenameEnabled(0, true);
		}
	}
	connect(ListView1, SIGNAL(itemRenamed(QListViewItem*, int)), this, SLOT(slotDoRename(QListViewItem*, int))); */

