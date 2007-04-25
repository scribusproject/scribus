/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "tree.h"
//#include "tree.moc"
#include <QHeaderView>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qimage.h>
#include <qmessagebox.h>
//Added by qt3to4:
#include <QLabel>
#include <QList>
#include <QResizeEvent>
#include <QMenu>

#include "commonstrings.h"
#include "page.h"
#include "scribus.h"
#include "mpalette.h"
#include "actionmanager.h"
#include "util.h"
#include "selection.h"
#include "undomanager.h"
#include "dynamictip.h"

extern QPixmap loadIcon(QString nam);

TreeItem::TreeItem(TreeItem* parent, TreeItem* after) : QTreeWidgetItem(parent, after)
{
	PageObject = NULL;
	PageItemObject = NULL;
	type = -1;
}

TreeItem::TreeItem(QTreeWidget* parent, TreeItem* after) : QTreeWidgetItem(parent, after)
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

	reportDisplay = new QTreeWidget( this );

	reportDisplay->setGeometry( QRect( 0, 0, 220, 240 ) );
	reportDisplay->setMinimumSize( QSize( 220, 240 ) );
	reportDisplay->setRootIsDecorated( true );
	reportDisplay->setColumnCount(1);
	reportDisplay->setHeaderLabel( tr("Element"));
	reportDisplay->header()->setClickable( false );
	reportDisplay->header()->setResizeMode( QHeaderView::ResizeToContents );
	reportDisplay->setSortingEnabled(false);
	reportDisplay->setSelectionMode(QAbstractItemView::SingleSelection);
	reportDisplay->setContextMenuPolicy(Qt::CustomContextMenu);
//	reportDisplay->setDefaultRenameAction(QTreeWidget::Accept);
	unsetDoc();
	imageIcon = loadIcon("22/insert-image.png");
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
	freeObjects = 0;
	languageChange();
// 	dynTip = new DynamicTip(reportDisplay);
	// signals and slots connections
	connect(reportDisplay, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(slotSelect(QTreeWidgetItem*, int)));
	connect(reportDisplay, SIGNAL(customContextMenuRequested (const QPoint &)), this, SLOT(slotRightClick(QPoint)));
//	connect(reportDisplay, SIGNAL(itemRenamed(QTreeWidgetItem*, int)), this, SLOT(slotDoRename(QTreeWidgetItem*, int)));
//	connect(reportDisplay, SIGNAL(rightButtonClicked(QTreeWidgetItem *, const QPoint &, int)), this, SLOT(slotRightClick(QTreeWidgetItem*, const QPoint &, int)));
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

void Tree::slotRightClick(QPoint point)
{
	if (!m_MainWindow || m_MainWindow->ScriptRunning)
		return;
	QTreeWidgetItem *ite = reportDisplay->itemAt(point);
	if (ite == NULL)
		return;
	slotSelect(ite, 0);
	TreeItem *item = (TreeItem*)ite;
	if (item != NULL)
	{
		if ((item->type == 0) || (item->type == 2))
		{
			QMenu *pmen = new QMenu();
			m_MainWindow->scrActions["viewShowMargins"]->addTo(pmen);
			m_MainWindow->scrActions["viewShowFrames"]->addTo(pmen);
			m_MainWindow->scrActions["viewShowLayerMarkers"]->addTo(pmen);
			m_MainWindow->scrActions["viewShowImages"]->addTo(pmen);
			m_MainWindow->scrActions["viewShowGrid"]->addTo(pmen);
			m_MainWindow->scrActions["viewShowGuides"]->addTo(pmen);
			m_MainWindow->scrActions["viewShowBaseline"]->addTo(pmen);
			m_MainWindow->scrActions["viewShowTextChain"]->addTo(pmen);
			m_MainWindow->scrActions["viewRulerMode"]->addTo(pmen);
			pmen->insertSeparator();
			m_MainWindow->scrActions["viewSnapToGrid"]->addTo(pmen);
			m_MainWindow->scrActions["viewSnapToGuides"]->addTo(pmen);
			m_MainWindow->scrActions["pageManageGuides"]->addTo(pmen);
			m_MainWindow->scrActions["pageManageMargins"]->addTo(pmen);
			if (item->type == 2)
			{
				m_MainWindow->scrActions["pageApplyMasterPage"]->addTo(pmen);
				pmen->insertSeparator();
				m_MainWindow->scrActions["pageDelete"]->addTo(pmen);
			}
			pmen->exec(QCursor::pos());
			delete pmen;
			pmen=NULL;
		}
		else if ((item->type == 1) || (item->type == 3) || (item->type == 4))
		{
			currentObject = ite;
//			currentColumn = col;
			PageItem *currItem = item->PageItemObject;
			QMenu *pmen = new QMenu();
			QMenu *pmen2 = new QMenu();
			QMenu *pmen3 = new QMenu();
			qApp->setOverrideCursor(QCursor(Qt::ArrowCursor), true);
			QMenu *pmen4 = new QMenu();
			QMenu *pmenEditContents = new QMenu();
			QMenu *pmenLevel = new QMenu();
			QMenu *pmenPDF = new QMenu();
			QMenu *pmenResolution = new QMenu();
			bool _isGlobalMode = currDoc->view()->undoManager->isGlobalMode();
			m_MainWindow->scrActions["editActionMode"]->setOn(true);
			uint docSelectionCount = currDoc->m_Selection->count();
			if (docSelectionCount == 1)
				currDoc->view()->undoManager->showObject(currDoc->m_Selection->itemAt(0)->getUId());
			else if (docSelectionCount > 1)
				currDoc->view()->undoManager->showObject(Um::NO_UNDO_STACK);
			else if (docSelectionCount == 0)
				currDoc->view()->undoManager->showObject(currDoc->currentPage()->getUId());
			if ((currItem->itemType() == PageItem::TextFrame) || (currItem->itemType() == PageItem::ImageFrame) || (currItem->itemType() == PageItem::PathText))
			{
/*				Q3ButtonGroup *InfoGroup = new Q3ButtonGroup( this, "InfoGroup" );
				InfoGroup->setFrameShape( Q3ButtonGroup::NoFrame );
				InfoGroup->setFrameShadow( Q3ButtonGroup::Plain );
				InfoGroup->setTitle("");
				InfoGroup->setExclusive( true );
				InfoGroup->setColumnLayout(0, Qt::Vertical );
				InfoGroup->layout()->setSpacing( 0 );
				InfoGroup->layout()->setMargin( 0 );
				Q3GridLayout *InfoGroupLayout = new Q3GridLayout( InfoGroup->layout() );
				InfoGroupLayout->setAlignment( Qt::AlignTop );
				InfoGroupLayout->setSpacing( 2 );
				InfoGroupLayout->setMargin( 0 );
				QString txtC, txtC2;
				QLabel *InfoT = new QLabel(InfoGroup, "ct");
				QLabel *LinCT = new QLabel(InfoGroup, "lt");
				QLabel *LinC = new QLabel(InfoGroup, "lc");
				QLabel *ParCT = new QLabel(InfoGroup, "pt");
				QLabel *ParC = new QLabel(InfoGroup, "pc");
				QLabel *WordCT = new QLabel(InfoGroup, "wt");
				QLabel *WordC = new QLabel(InfoGroup, "wc");
				QLabel *CharCT = new QLabel(InfoGroup, "ct");
				QLabel *CharC = new QLabel(InfoGroup, "cc");
				QLabel *ColCT = new QLabel(InfoGroup, "ct");
				QLabel *ColC = new QLabel(InfoGroup, "cc");
				QLabel *PrintCT = new QLabel(InfoGroup, "nt"); // <a.l.e>
				QLabel *PrintC = new QLabel(InfoGroup, "nc"); // </a.l.e>
				if (currItem->itemType() == PageItem::ImageFrame)
				{
					LinC->hide();
					LinCT->hide();
					if (currItem->PicAvail)
					{
						QFileInfo fi = QFileInfo(currItem->Pfile);
						InfoT->setText( tr("Picture"));
						InfoGroupLayout->addMultiCellWidget( InfoT, 0, 0, 0, 1, Qt::AlignHCenter );
						ParCT->setText( tr("File: "));
						InfoGroupLayout->addWidget( ParCT, 1, 0, Qt::AlignRight );
						ParC->setText(fi.fileName());
						InfoGroupLayout->addWidget( ParC, 1, 1 );
						WordCT->setText( tr("Original PPI: "));
						InfoGroupLayout->addWidget( WordCT, 2, 0, Qt::AlignRight );
						WordC->setText(txtC.setNum(qRound(currItem->pixm.imgInfo.xres))+" x "+txtC2.setNum(qRound(currItem->pixm.imgInfo.yres)));
						InfoGroupLayout->addWidget( WordC, 2, 1 );
						CharCT->setText( tr("Actual PPI: "));
						InfoGroupLayout->addWidget( CharCT, 3, 0, Qt::AlignRight );
						CharC->setText(txtC.setNum(qRound(72.0 / currItem->imageXScale()))+" x "+ txtC2.setNum(qRound(72.0 / currItem->imageYScale())));
						InfoGroupLayout->addWidget( CharC, 3, 1 );
						ColCT->setText( tr("Colorspace: "));
						InfoGroupLayout->addWidget( ColCT, 4, 0, Qt::AlignRight );
						QString cSpace;
						QString ext = fi.extension(false).lower();
						if (((ext == "pdf") || (ext == "eps") || (ext == "epsi") || (ext == "ps")) && (currItem->pixm.imgInfo.type != 7))
							cSpace = tr("Unknown");
						else
						{
							switch (currItem->pixm.imgInfo.colorspace)
							{
								case 0:
									cSpace = tr("RGB");
									break;
								case 1:
									cSpace = tr("CMYK");
									break;
								case 2:
									cSpace = tr("Grayscale");
									break;
								case 3:
									cSpace = tr("Duotone");
									break;
							}
						}
						ColC->setText(cSpace);
						InfoGroupLayout->addWidget( ColC, 4, 1 );
					}
					else
					{
						InfoT->setText( tr("Picture"));
						InfoGroupLayout->addMultiCellWidget( InfoT, 0, 0, 0, 1, Qt::AlignHCenter );
						ParCT->setText( tr("No Image Loaded"));
						InfoGroupLayout->addMultiCellWidget( ParCT, 1, 1, 0, 1, Qt::AlignHCenter );
						ParC->hide();
						WordCT->hide();
						WordC->hide();
						CharCT->hide();
						CharC->hide();
						ColCT->hide();
						ColC->hide();
					}
				}
				if ((currItem->itemType() == PageItem::TextFrame) || (currItem->itemType() == PageItem::PathText))
				{
					int Parag = 0;
					int Words = 0;
					int Chara = 0;
					int ParagN = 0;
					int WordsN = 0;
					int CharaN = 0;
					ColC->hide();
					ColCT->hide();
					if (currItem->itemType() == PageItem::TextFrame)
					{
						if ((currItem->nextInChain() != 0) || (currItem->prevInChain() != 0))
							InfoT->setText( tr("Linked Text"));
						else
							InfoT->setText( tr("Text Frame"));
					}
					else
						InfoT->setText( tr("Text on a Path"));
					InfoGroupLayout->addMultiCellWidget( InfoT, 0, 0, 0, 1, Qt::AlignCenter );
					WordAndPara(currItem, &Words, &Parag, &Chara, &WordsN, &ParagN, &CharaN);
					ParCT->setText( tr("Paragraphs: "));
					InfoGroupLayout->addWidget( ParCT, 1, 0, Qt::AlignRight );
					if (ParagN != 0)
						ParC->setText(txtC.setNum(Parag+ParagN)+" ("+txtC2.setNum(ParagN)+")");
					else
						ParC->setText(txtC.setNum(Parag));
					InfoGroupLayout->addWidget( ParC, 1, 1 );
					LinCT->setText( tr("Lines: "));
					InfoGroupLayout->addWidget( LinCT, 2, 0, Qt::AlignRight );
					LinC->setText(txtC.setNum(currItem->itemText.lines()));
					InfoGroupLayout->addWidget( LinC, 2, 1 );
					WordCT->setText( tr("Words: "));
					InfoGroupLayout->addWidget( WordCT, 3, 0, Qt::AlignRight );
					if (WordsN != 0)
						WordC->setText(txtC.setNum(Words+WordsN)+" ("+txtC2.setNum(WordsN)+")");
					else
						WordC->setText(txtC.setNum(Words));
					InfoGroupLayout->addWidget( WordC, 3, 1 );
					CharCT->setText( tr("Chars: "));
					InfoGroupLayout->addWidget( CharCT, 4, 0, Qt::AlignRight );
					if (CharaN != 0)
						CharC->setText(txtC.setNum(Chara+CharaN)+" ("+txtC2.setNum(CharaN)+")");
					else
						CharC->setText(txtC.setNum(Chara));
					InfoGroupLayout->addWidget( CharC, 4, 1 );
				}
				int row = InfoGroupLayout->numRows();
				PrintCT->setText( tr("Print: "));
				InfoGroupLayout->addWidget( PrintCT, row, 0, Qt::AlignRight );
				if (currItem->printEnabled())
					PrintC->setText( tr("Enabled"));
				else
					PrintC->setText( tr("Disabled"));
				InfoGroupLayout->addWidget( PrintC, row, 1 ); */
// Qt4				pmen4->insertItem(InfoGroup);
				if ((currItem->itemType() == PageItem::ImageFrame) && (currItem->pixm.imgInfo.exifDataValid))
					m_MainWindow->scrActions["itemImageInfo"]->addTo(pmen4);
				pmen->insertItem( tr("In&fo"), pmen4);
			}
			pmen->insertSeparator();
			m_MainWindow->scrActions["editUndoAction"]->addTo(pmen);
			m_MainWindow->scrActions["editRedoAction"]->addTo(pmen);
			if (currItem->itemType() == PageItem::ImageFrame ||
				currItem->itemType() == PageItem::TextFrame ||
				currItem->itemType() == PageItem::PathText)
			{
				pmen->insertSeparator();
				if (currItem->itemType() == PageItem::ImageFrame)
				{
					m_MainWindow->scrActions["fileImportImage"]->addTo(pmen);
					if (currItem->PicAvail)
					{
						if (!currItem->isTableItem)
							m_MainWindow->scrActions["itemAdjustFrameToImage"]->addTo(pmen);
						if (currItem->pixm.imgInfo.valid)
							m_MainWindow->scrActions["itemExtendedImageProperties"]->addTo(pmen);
						m_MainWindow->scrActions["itemUpdateImage"]->addTo(pmen);
					}
					pmen->insertItem( tr("Preview Settings"), pmenResolution);
					m_MainWindow->scrActions["itemImageIsVisible"]->addTo(pmenResolution);
					pmenResolution->insertSeparator();
					m_MainWindow->scrActions["itemPreviewLow"]->addTo(pmenResolution);
					m_MainWindow->scrActions["itemPreviewNormal"]->addTo(pmenResolution);
					m_MainWindow->scrActions["itemPreviewFull"]->addTo(pmenResolution);
					if (currItem->PicAvail && currItem->isRaster)
					{
						m_MainWindow->scrActions["styleImageEffects"]->addTo(pmen);
						m_MainWindow->scrActions["editEditWithImageEditor"]->addTo(pmen);
					}
				}
				if (currItem->itemType() == PageItem::TextFrame)
				{
					m_MainWindow->scrActions["fileImportText"]->addTo(pmen);
					m_MainWindow->scrActions["fileImportAppendText"]->addTo(pmen);
					m_MainWindow->scrActions["toolsEditWithStoryEditor"]->addTo(pmen);
					m_MainWindow->scrActions["insertSampleText"]->addTo(pmen);
				}
				if (currItem->itemType() == PageItem::PathText)
					m_MainWindow->scrActions["toolsEditWithStoryEditor"]->addTo(pmen);
			}
			if (currDoc->m_Selection->count() == 1)
			{
				pmen->insertSeparator();
				m_MainWindow->scrActions["itemAttributes"]->addTo(pmen);
			}	
			if (currItem->itemType() == PageItem::TextFrame)
			{
				if (currDoc->currentPage()->pageName().isEmpty())
				{
					m_MainWindow->scrActions["itemPDFIsAnnotation"]->addTo(pmenPDF);
					m_MainWindow->scrActions["itemPDFIsBookmark"]->addTo(pmenPDF);
					if (currItem->isAnnotation())
					{
						if ((currItem->annotation().Type() == 0) || (currItem->annotation().Type() == 1) || (currItem->annotation().Type() > 9))
							m_MainWindow->scrActions["itemPDFAnnotationProps"]->addTo(pmenPDF);
						else
							m_MainWindow->scrActions["itemPDFFieldProps"]->addTo(pmenPDF);
					}
				}
				pmen->insertItem( tr("&PDF Options"), pmenPDF);
			}
			pmen->insertSeparator();
			m_MainWindow->scrActions["itemLock"]->addTo(pmen);
			m_MainWindow->scrActions["itemLockSize"]->addTo(pmen);
			if (!currItem->isSingleSel)
			{
				m_MainWindow->scrActions["itemSendToScrapbook"]->addTo(pmen);
				m_MainWindow->scrActions["itemSendToPattern"]->addTo(pmen);
				if (currDoc->layerCount() > 1)
				{
					QMap<int,int> layerMap;
					for (Q3ValueList<Layer>::iterator it = currDoc->Layers.begin(); it != currDoc->Layers.end(); ++it)
						layerMap.insert((*it).Level, (*it).LNr);
					int i=layerMap.count()-1;
					while (i>=0)
					{
						if (currDoc->layerLocked(layerMap[i]))
							m_MainWindow->scrLayersActions[QString::number(layerMap[i])]->setEnabled(false);
						else
							m_MainWindow->scrLayersActions[QString::number(layerMap[i])]->setEnabled(true);
						m_MainWindow->scrLayersActions[QString::number(layerMap[i--])]->addTo(pmen3);
					}

					pmen->insertItem( tr("Send to La&yer"), pmen3);
				}
			}
			if ((currItem->Groups.count() != 0) && (currItem->isGroupControl))
				m_MainWindow->scrActions["itemUngroup"]->addTo(pmen);
			if (!currItem->locked())
			{
				if ((!currItem->isTableItem) && (!currItem->isSingleSel))
				{
					pmen->insertItem( tr("Le&vel"), pmenLevel);
					m_MainWindow->scrActions["itemRaiseToTop"]->addTo(pmenLevel);
					m_MainWindow->scrActions["itemRaise"]->addTo(pmenLevel);
					m_MainWindow->scrActions["itemLower"]->addTo(pmenLevel);
					m_MainWindow->scrActions["itemLowerToBottom"]->addTo(pmenLevel);
				}
			}
			if (currDoc->appMode != modeEdit && currDoc->m_Selection->itemsAreSameType()) //Create convertTo Menu
			{
				bool insertConvertToMenu=false;
				if ((currItem->itemType() == PageItem::TextFrame) || (currItem->itemType() == PageItem::PathText))
				{
					insertConvertToMenu=true;
					if (currItem->itemType() == PageItem::PathText)
						m_MainWindow->scrActions["itemConvertToOutlines"]->addTo(pmen2);
					else
					{
						if (currItem->isTableItem)
							m_MainWindow->scrActions["itemConvertToImageFrame"]->addTo(pmen2);
						if (!currItem->isTableItem)
						{
							if ((currItem->prevInChain() == 0) && (currItem->nextInChain() == 0))
								m_MainWindow->scrActions["itemConvertToImageFrame"]->addTo(pmen2);
							m_MainWindow->scrActions["itemConvertToOutlines"]->addTo(pmen2);
							if ((currItem->prevInChain() == 0) && (currItem->nextInChain() == 0))
								m_MainWindow->scrActions["itemConvertToPolygon"]->addTo(pmen2);
						}
					}
				}
				if (currItem->itemType() == PageItem::ImageFrame)
				{
					insertConvertToMenu=true;
					m_MainWindow->scrActions["itemConvertToTextFrame"]->addTo(pmen2);
					if (!currItem->isTableItem)
						m_MainWindow->scrActions["itemConvertToPolygon"]->addTo(pmen2);
				}
				if (currItem->itemType() == PageItem::Polygon)
				{
					insertConvertToMenu=true;
					m_MainWindow->scrActions["itemConvertToBezierCurve"]->addTo(pmen2);
					m_MainWindow->scrActions["itemConvertToImageFrame"]->addTo(pmen2);
					m_MainWindow->scrActions["itemConvertToTextFrame"]->addTo(pmen2);
				}
				bool insertedMenusEnabled = false;
				for (uint pc = 0; pc < pmen2->count(); pc++)
				{
					if (pmen2->isItemEnabled(pmen2->idAt(pc)))
						insertedMenusEnabled = true;
				}
				if ((insertConvertToMenu) && (insertedMenusEnabled))
					pmen->insertItem( tr("Conve&rt to"), pmen2);
			}
			pmen->insertSeparator();
			pmen->insertItem( tr("Rename") , this, SLOT(slotRenameItem()));
			if (!currItem->locked() && !(currItem->isSingleSel))
				m_MainWindow->scrActions["editCut"]->addTo(pmen);
			if (!(currItem->isSingleSel))
				m_MainWindow->scrActions["editCopy"]->addTo(pmen);
			if ((currDoc->appMode == modeEdit) && (m_MainWindow->Buffer2.startsWith("<SCRIBUSTEXT")) && (currItem->itemType() == PageItem::TextFrame))
				m_MainWindow->scrActions["editPaste"]->addTo(pmen);
			if (!currItem->locked() && (currDoc->appMode != modeEdit) && (!(currItem->isSingleSel)))
				pmen->insertItem( tr("&Delete"), currDoc, SLOT(itemSelection_DeleteItem()));
			if ((currItem->itemType() == PageItem::ImageFrame) || (currItem->itemType() == PageItem::TextFrame))
			{
				if (currItem->itemType() == PageItem::ImageFrame)
				{
					if (currItem->PicAvail)
						m_MainWindow->scrActions["editCopyContents"]->addTo(pmenEditContents);
					if (m_MainWindow->contentsBuffer.sourceType==PageItem::ImageFrame)
					{
						m_MainWindow->scrActions["editPasteContents"]->addTo(pmenEditContents);
						m_MainWindow->scrActions["editPasteContentsAbs"]->addTo(pmenEditContents);
					}
					if (currItem->PicAvail)
						m_MainWindow->scrActions["editClearContents"]->addTo(pmenEditContents);
					if ((currItem->PicAvail) || (m_MainWindow->contentsBuffer.sourceType==PageItem::ImageFrame))
						pmen->insertItem( tr("Contents"), pmenEditContents);
				}
				else
				{
					if (currItem->itemText.lines() != 0)
					{
						m_MainWindow->scrActions["editClearContents"]->addTo(pmenEditContents);
						pmen->insertItem( tr("Contents"), pmenEditContents);
					}
				}
			}
			pmen->exec(QCursor::pos());
			m_MainWindow->scrActions["editActionMode"]->setOn(!_isGlobalMode);
			if (_isGlobalMode)
				currDoc->view()->undoManager->showObject(Um::GLOBAL_UNDO_MODE);
			else
			{
				docSelectionCount = currDoc->m_Selection->count();
				if (docSelectionCount == 1)
					currDoc->view()->undoManager->showObject(currDoc->m_Selection->itemAt(0)->getUId());
				else if (docSelectionCount > 1)
					currDoc->view()->undoManager->showObject(Um::NO_UNDO_STACK);
				else if (docSelectionCount == 0)
					currDoc->view()->undoManager->showObject(currDoc->currentPage()->getUId());
			}
			delete pmen;
			delete pmen2;
			delete pmen3;
			delete pmen4;
			delete pmenEditContents;
			delete pmenLevel;
			delete pmenPDF;
			delete pmenResolution;
			pmen=NULL;
			pmen2=NULL;
			pmen3=NULL;
			pmen4=NULL;
			pmenEditContents=NULL;
			pmenLevel=NULL;
			pmenPDF=NULL;
			pmenResolution=NULL;
		}
	}
}

void Tree::slotRenameItem()
{
//	currentObject->startRename(currentColumn);
}

void Tree::slotDoRename(QTreeWidgetItem *ite , int col)
{
	if (!m_MainWindow || m_MainWindow->ScriptRunning)
		return;
/*	disconnect(reportDisplay, SIGNAL(itemRenamed(Q3ListViewItem*, int)), this, SLOT(slotDoRename(Q3ListViewItem*, int)));
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
	connect(reportDisplay, SIGNAL(itemRenamed(QTreeWidgetItem*, int)), this, SLOT(slotDoRename(Q3ListViewItem*, int))); */
}

QTreeWidgetItem* Tree::getListItem(uint SNr, int Nr)
{
	TreeItem *item = 0;
	QTreeWidgetItem *retVal = 0;
	if (currDoc->masterPageMode())
	{
		if (Nr == -1)
		{
			QTreeWidgetItemIterator it( reportDisplay );
			while ( (*it) )
			{
				item = (TreeItem*)(*it);
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
				item = (TreeItem*)(*it);
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
				item = (TreeItem*)(*it);
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
				item = (TreeItem*)(*it);
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
	disconnect(reportDisplay, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(slotSelect(QTreeWidgetItem*, int)));
	reportDisplay->clearSelection();
	QTreeWidgetItem *retVal = getListItem(SNr, Nr);
	if (retVal != 0)
		retVal->setSelected(true);
	connect(reportDisplay, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(slotSelect(QTreeWidgetItem*, int)));
}

void Tree::setItemIcon(QTreeWidgetItem *item, PageItem *pgItem)
{
	switch (pgItem->itemType())
	{
	case PageItem::ImageFrame:
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

void Tree::reopenTree()
{
	if (!m_MainWindow || m_MainWindow->ScriptRunning)
		return;
	if (currDoc->OpenNodes.count() == 0)
		return;
	TreeItem *item = 0;
	QTreeWidgetItemIterator it( reportDisplay );
	while ( (*it) )
	{
		item = (TreeItem*)(*it);
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

void Tree::buildReopenVals()
{
	ScribusDoc::OpenNodesList ol;
	if (reportDisplay->model()->rowCount() == 0)
		return;
	currDoc->OpenNodes.clear();
	TreeItem *item = 0;
	QTreeWidgetItemIterator it( reportDisplay );
	while ( (*it) )
	{
		item = (TreeItem*)(*it);
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

void Tree::slotSelect(QTreeWidgetItem* ite, int col)
{
	if (!m_MainWindow || m_MainWindow->ScriptRunning)
		return;
	disconnect(reportDisplay, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(slotSelect(QTreeWidgetItem*, int)));
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
	connect(reportDisplay, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(slotSelect(QTreeWidgetItem*, int)));
}

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
	disconnect(reportDisplay, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(slotSelect(QTreeWidgetItem*, int)));
	setUpdatesEnabled(false);
	if (storeVals)
		buildReopenVals();
	clearPalette();
	QList<PageItem*> subGroupList;
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
					setItemIcon(object, pgItem);
//					object->setRenameEnabled(0, true);
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
					object->setIcon( 0, groupIcon );
//					object->setRenameEnabled(0, true);
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
					setItemIcon(object, pgItem);
//					object->setRenameEnabled(0, true);
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
					object->setIcon( 0, groupIcon );
//					object->setRenameEnabled(0, true);
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
					setItemIcon(object, pgItem);
//					object->setRenameEnabled(0, true);
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
					object->setIcon( 0, groupIcon );
//					object->setRenameEnabled(0, true);
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
	connect(reportDisplay, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(slotSelect(QTreeWidgetItem*, int)));
}

void Tree::parseSubGroup(int level, TreeItem* object, QList<PageItem*> *subGroupList, int itemType)
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
				TreeItem *grp = new TreeItem( object, 0 );
				grp->PageItemObject = pgItem;
				grp->type = itemType;
				grp->setText(0, pgItem->itemName());
				setItemIcon(grp, pgItem);
//				grp->setRenameEnabled(0, true);
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
				grp->setIcon( 0, groupIcon );
//				grp->setRenameEnabled(0, true);
				pgItem->Dirty = true;
				subGroup = new QList<PageItem*>;
				subGroup->clear();
				for (int ga = 0; ga < subGroupList->count(); ++ga)
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
	reportDisplay->setHeaderLabel( tr("Element"));
}

void Tree::clearPalette()
{
	reportDisplay->clear();
}
