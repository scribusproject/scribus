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
 				QString tipText = "";
 				if ((item->type == 1) || (item->type == 3) || (item->type == 4))
 				{
 					PageItem *pgItem = item->PageItemObject;
 					switch (pgItem->itemType())
 					{
 						case PageItem::ImageFrame:
 							if (pgItem->asLatexFrame())
 								tipText = QObject::tr("LaTex Frame");
 							else
 								tipText = QObject::tr("Image");
 							break;
 						case PageItem::TextFrame:
 							switch (pgItem->annotation().Type())
 							{
 								case 2:
 									tipText = QObject::tr("PDF Push Button");
 									break;
 								case 3:
 									tipText = QObject::tr("PDF Text Field");
 									break;
 								case 4:
 									tipText = QObject::tr("PDF Check Box");
 									break;
 								case 5:
 									tipText = QObject::tr("PDF Combo Box");
 									break;
 								case 6:
 									tipText = QObject::tr("PDF List Box");
 									break;
 								case 10:
 									tipText = QObject::tr("PDF Text Annotation");
 									break;
 								case 11:
 									tipText = QObject::tr("PDF Link Annotation");
 									break;
 								default:
 									tipText = QObject::tr("Text");
 									break;
 							}
 							break;
 						case PageItem::Line:
 							tipText = QObject::tr("Line");
 							break;
 						case PageItem::Polygon:
 							tipText = QObject::tr("Polygon");
 							break;
 						case PageItem::PolyLine:
 							tipText = QObject::tr("Polyline");
 							break;
 						case PageItem::PathText:
 							tipText = QObject::tr("PathText");
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
	resize( 220, 240 );
	setMinimumSize( QSize( 220, 240 ) );
	setMaximumSize( QSize( 800, 600 ) );

	reportDisplay = new OutlineWidget( this );

	reportDisplay->setGeometry( QRect( 0, 0, 220, 240 ) );
	reportDisplay->setMinimumSize( QSize( 220, 240 ) );
	reportDisplay->setRootIsDecorated( true );
	reportDisplay->setColumnCount(1);
	reportDisplay->setHeaderLabel( tr("Element"));
	reportDisplay->header()->setClickable( false );
	reportDisplay->header()->setResizeMode( QHeaderView::ResizeToContents );
	reportDisplay->setSortingEnabled(false);
	reportDisplay->setSelectionMode(QAbstractItemView::ExtendedSelection);
	reportDisplay->setContextMenuPolicy(Qt::CustomContextMenu);
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
	if (!m_MainWindow || m_MainWindow->ScriptRunning)
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
	/*
	if (item != NULL)
	{
		if ((item->type == 0) || (item->type == 2))
		{
			QMenu *pmen = new QMenu();
			pmen->addAction(m_MainWindow->scrActions["viewShowMargins"]);
			pmen->addAction(m_MainWindow->scrActions["viewShowFrames"]);
			pmen->addAction(m_MainWindow->scrActions["viewShowLayerMarkers"]);
			pmen->addAction(m_MainWindow->scrActions["viewShowImages"]);
			pmen->addAction(m_MainWindow->scrActions["viewShowGrid"]);
			pmen->addAction(m_MainWindow->scrActions["viewShowGuides"]);
			pmen->addAction(m_MainWindow->scrActions["viewShowBaseline"]);
			pmen->addAction(m_MainWindow->scrActions["viewShowTextChain"]);
			pmen->addAction(m_MainWindow->scrActions["viewRulerMode"]);
			pmen->addSeparator();
			pmen->addAction(m_MainWindow->scrActions["viewSnapToGrid"]);
			pmen->addAction(m_MainWindow->scrActions["viewSnapToGuides"]);
			pmen->addAction(m_MainWindow->scrActions["pageManageGuides"]);
			pmen->addAction(m_MainWindow->scrActions["pageManageMargins"]);
			if (item->type == 2)
			{
				pmen->addAction(m_MainWindow->scrActions["pageApplyMasterPage"]);
				pmen->addSeparator();
				pmen->addAction(m_MainWindow->scrActions["pageDelete"]);
			}
			pmen->exec(QCursor::pos());
			delete pmen;
			pmen=NULL;
		}
		else if ((item->type == 1) || (item->type == 3) || (item->type == 4))
		{
			currentObject = ite;
			PageItem *currItem = item->PageItemObject;
			QMenu *pmen = new QMenu();
			QMenu *pmen2 = new QMenu();
			QMenu *pmen3 = new QMenu();
			qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
			QMenu *pmen4 = new QMenu();
			QMenu *pmenEditContents = new QMenu();
			QMenu *pmenLevel = new QMenu();
			QMenu *pmenPDF = new QMenu();
			QMenu *pmenResolution = new QMenu();
			bool _isGlobalMode = currDoc->view()->undoManager->isGlobalMode();
			m_MainWindow->scrActions["editActionMode"]->setChecked(true);
			uint docSelectionCount = currDoc->m_Selection->count();
			if (docSelectionCount == 1)
				currDoc->view()->undoManager->showObject(currDoc->m_Selection->itemAt(0)->getUId());
			else if (docSelectionCount > 1)
				currDoc->view()->undoManager->showObject(Um::NO_UNDO_STACK);
			else if (docSelectionCount == 0)
				currDoc->view()->undoManager->showObject(currDoc->currentPage()->getUId());
			if ((currItem->itemType() == PageItem::TextFrame) || (currItem->itemType() == PageItem::ImageFrame) || (currItem->itemType() == PageItem::PathText))
			{
				QFrame *InfoGroup = new QFrame(this);
				QGridLayout *InfoGroupLayout = new QGridLayout( InfoGroup );
				InfoGroupLayout->setAlignment( Qt::AlignTop );
				InfoGroupLayout->setSpacing( 2 );
				InfoGroupLayout->setMargin( 0 );
				QString txtC, txtC2;
				QLabel *InfoT = new QLabel(InfoGroup);
				QLabel *LinCT = new QLabel(InfoGroup);
				QLabel *LinC = new QLabel(InfoGroup);
				QLabel *ParCT = new QLabel(InfoGroup);
				QLabel *ParC = new QLabel(InfoGroup);
				QLabel *WordCT = new QLabel(InfoGroup);
				QLabel *WordC = new QLabel(InfoGroup);
				QLabel *CharCT = new QLabel(InfoGroup);
				QLabel *CharC = new QLabel(InfoGroup);
				QLabel *ColCT = new QLabel(InfoGroup);
				QLabel *ColC = new QLabel(InfoGroup);
				QLabel *PrintCT = new QLabel(InfoGroup); // <a.l.e>
				QLabel *PrintC = new QLabel(InfoGroup); // </a.l.e>
				if (currItem->itemType() == PageItem::ImageFrame)
				{
					LinC->hide();
					LinCT->hide();
					if (currItem->PicAvail)
					{
						QFileInfo fi = QFileInfo(currItem->Pfile);
						InfoT->setText( tr("Picture"));
						InfoGroupLayout->addWidget( InfoT, 0, 0, 1, 2, Qt::AlignHCenter );
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
						QString ext = fi.suffix().toLower();
						if ((extensionIndicatesPDF(ext) || extensionIndicatesEPSorPS(ext)) && (currItem->pixm.imgInfo.type != ImageType7))
							cSpace = tr("Unknown");
						else
							cSpace=colorSpaceText(currItem->pixm.imgInfo.colorspace);
						ColC->setText(cSpace);
						InfoGroupLayout->addWidget( ColC, 4, 1 );
					}
					else
					{
						InfoT->setText( tr("Picture"));
						InfoGroupLayout->addWidget( InfoT, 0, 0, 1, 2, Qt::AlignHCenter );
						ParCT->setText( tr("No Image Loaded"));
						InfoGroupLayout->addWidget( ParCT, 1, 0, 1, 2, Qt::AlignHCenter );
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
					InfoGroupLayout->addWidget( InfoT, 0, 0, 1, 2, Qt::AlignCenter );
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
				int row = InfoGroupLayout->rowCount();
				PrintCT->setText( tr("Export: "));
				InfoGroupLayout->addWidget( PrintCT, row, 0, Qt::AlignRight );
				if (currItem->printEnabled())
					PrintC->setText( tr("Enabled"));
				else
					PrintC->setText( tr("Disabled"));
				InfoGroupLayout->addWidget( PrintC, row, 1 );
				QWidgetAction* MenAct = new QWidgetAction(this);
				MenAct->setDefaultWidget(InfoGroup);
				pmen4->addAction(MenAct);
				if ((currItem->itemType() == PageItem::ImageFrame) && (currItem->pixm.imgInfo.exifDataValid))
					pmen4->addAction(m_MainWindow->scrActions["itemImageInfo"]);
				QAction *sub = pmen->addMenu(pmen4);
				sub->setText( tr("In&fo"));
			}
			pmen->addSeparator();
			pmen->addAction(m_MainWindow->scrActions["editUndoAction"]);
			pmen->addAction(m_MainWindow->scrActions["editRedoAction"]);
			if (currItem->itemType() == PageItem::ImageFrame ||
				currItem->itemType() == PageItem::TextFrame ||
				currItem->itemType() == PageItem::PathText)
			{
				pmen->addSeparator();
				if (currItem->asLatexFrame())
				{
					pmen->addAction(m_MainWindow->scrActions["itemAdjustFrameToImage"]);
					pmen->addAction(m_MainWindow->scrActions["itemUpdateImage"]);
					pmen->addAction(m_MainWindow->scrActions["editEditWithLatexEditor"]);
				} else if (currItem->itemType() == PageItem::ImageFrame)
				{
					pmen->addAction(m_MainWindow->scrActions["fileImportImage"]);
					if (currItem->PicAvail)
					{
						if (!currItem->isTableItem)
							pmen->addAction(m_MainWindow->scrActions["itemAdjustFrameToImage"]);
						if (currItem->pixm.imgInfo.valid)
							pmen->addAction(m_MainWindow->scrActions["itemExtendedImageProperties"]);
						pmen->addAction(m_MainWindow->scrActions["itemUpdateImage"]);
					}
					QAction *sub = pmen->addMenu(pmenResolution);
					sub->setText( tr("Preview Settings"));
					pmenResolution->addAction(m_MainWindow->scrActions["itemImageIsVisible"]);
					pmenResolution->addSeparator();
					pmenResolution->addAction(m_MainWindow->scrActions["itemPreviewLow"]);
					pmenResolution->addAction(m_MainWindow->scrActions["itemPreviewNormal"]);
					pmenResolution->addAction(m_MainWindow->scrActions["itemPreviewFull"]);
					if (currItem->PicAvail && currItem->isRaster)
					{
						pmen->addAction(m_MainWindow->scrActions["styleImageEffects"]);
						pmen->addAction(m_MainWindow->scrActions["editEditWithImageEditor"]);
					}
				}
				if (currItem->itemType() == PageItem::TextFrame)
				{
					pmen->addAction(m_MainWindow->scrActions["fileImportText"]);
					pmen->addAction(m_MainWindow->scrActions["fileImportAppendText"]);
					pmen->addAction(m_MainWindow->scrActions["toolsEditWithStoryEditor"]);
					pmen->addAction(m_MainWindow->scrActions["insertSampleText"]);
				}
				if (currItem->itemType() == PageItem::PathText)
					pmen->addAction(m_MainWindow->scrActions["toolsEditWithStoryEditor"]);
			}
			if (currDoc->m_Selection->count() == 1)
			{
				pmen->addSeparator();
				pmen->addAction(m_MainWindow->scrActions["itemAttributes"]);
			}	
			if (currItem->itemType() == PageItem::TextFrame)
			{
				if (currDoc->currentPage()->pageName().isEmpty())
				{
					pmenPDF->addAction(m_MainWindow->scrActions["itemPDFIsAnnotation"]);
					pmenPDF->addAction(m_MainWindow->scrActions["itemPDFIsBookmark"]);
					if (currItem->isAnnotation())
					{
						if ((currItem->annotation().Type() == 0) || (currItem->annotation().Type() == 1) || (currItem->annotation().Type() > 9))
							pmenPDF->addAction(m_MainWindow->scrActions["itemPDFAnnotationProps"]);
						else
							pmenPDF->addAction(m_MainWindow->scrActions["itemPDFFieldProps"]);
					}
				}
				pmen->addMenu(pmenPDF)->setText( tr("&PDF Options"));
			}
			pmen->addSeparator();
			pmen->addAction(m_MainWindow->scrActions["itemLock"]);
			pmen->addAction(m_MainWindow->scrActions["itemLockSize"]);
			if (!currItem->isSingleSel)
			{
				pmen->addAction(m_MainWindow->scrActions["itemSendToScrapbook"]);
				pmen->addAction(m_MainWindow->scrActions["itemSendToPattern"]);
				if (currDoc->layerCount() > 1)
				{
					QMap<int,int> layerMap;
					for (ScLayers::iterator it = currDoc->Layers.begin(); it != currDoc->Layers.end(); ++it)
						layerMap.insert((*it).Level, (*it).LNr);
					int i=layerMap.count()-1;
					while (i>=0)
					{
						if (currDoc->layerLocked(layerMap[i]))
							m_MainWindow->scrLayersActions[QString::number(layerMap[i])]->setEnabled(false);
						else
							m_MainWindow->scrLayersActions[QString::number(layerMap[i])]->setEnabled(true);
						pmen3->addAction(m_MainWindow->scrLayersActions[QString::number(layerMap[i--])]);
					}

					pmen->addMenu(pmen3)->setText( tr("Send to La&yer"));
				}
			}
			if (currDoc->m_Selection->count() > 1)
			{
				bool isGroup = true;
				int firstElem = -1;
				if (currItem->Groups.count() != 0)
					firstElem = currItem->Groups.top();
				for (uint bx = 0; bx < docSelectionCount; ++bx)
				{
					if (currDoc->m_Selection->itemAt(bx)->Groups.count() != 0)
					{
						if (currDoc->m_Selection->itemAt(bx)->Groups.top() != firstElem)
							isGroup = false;
					}
					else
						isGroup = false;
				}
				if (!isGroup)
					pmen->addAction(m_MainWindow->scrActions["itemGroup"]);
			}
			if ((currItem->Groups.count() != 0) && (currItem->isGroupControl))
				pmen->addAction(m_MainWindow->scrActions["itemUngroup"]);
			if (!currItem->locked())
			{
				if ((!currItem->isTableItem) && (!currItem->isSingleSel))
				{
					pmen->addMenu(pmenLevel)->setText( tr("Le&vel"));
					pmenLevel->addAction(m_MainWindow->scrActions["itemRaiseToTop"]);
					pmenLevel->addAction(m_MainWindow->scrActions["itemRaise"]);
					pmenLevel->addAction(m_MainWindow->scrActions["itemLower"]);
					pmenLevel->addAction(m_MainWindow->scrActions["itemLowerToBottom"]);
				}
			}
			if (currDoc->appMode != modeEdit && currDoc->m_Selection->itemsAreSameType()) //Create convertTo Menu
			{
				bool insertConvertToMenu=false;
				if ((currItem->itemType() == PageItem::TextFrame) || (currItem->itemType() == PageItem::PathText))
				{
					insertConvertToMenu=true;
					if (currItem->itemType() == PageItem::PathText)
						pmen2->addAction(m_MainWindow->scrActions["itemConvertToOutlines"]);
					else
					{
						if (currItem->isTableItem)
							pmen2->addAction(m_MainWindow->scrActions["itemConvertToImageFrame"]);
						if (!currItem->isTableItem)
						{
							if ((currItem->prevInChain() == 0) && (currItem->nextInChain() == 0))
								pmen2->addAction(m_MainWindow->scrActions["itemConvertToImageFrame"]);
							pmen2->addAction(m_MainWindow->scrActions["itemConvertToOutlines"]);
							if ((currItem->prevInChain() == 0) && (currItem->nextInChain() == 0))
								pmen2->addAction(m_MainWindow->scrActions["itemConvertToPolygon"]);
						}
					}
				}
				if (currItem->itemType() == PageItem::ImageFrame)
				{
					insertConvertToMenu=true;
					pmen2->addAction(m_MainWindow->scrActions["itemConvertToTextFrame"]);
					if (!currItem->isTableItem)
						pmen2->addAction(m_MainWindow->scrActions["itemConvertToPolygon"]);
				}
				if (currItem->itemType() == PageItem::Polygon)
				{
					insertConvertToMenu=true;
					pmen2->addAction(m_MainWindow->scrActions["itemConvertToBezierCurve"]);
					pmen2->addAction(m_MainWindow->scrActions["itemConvertToImageFrame"]);
					pmen2->addAction(m_MainWindow->scrActions["itemConvertToTextFrame"]);
				}
				bool insertedMenusEnabled = false;
				QList<QAction*> actList = pmen2->actions();
				for (int pc = 0; pc < actList.count(); pc++)
				{
					if (actList[pc]->isEnabled())
						insertedMenusEnabled = true;
				}
				if ((insertConvertToMenu) && (insertedMenusEnabled))
					pmen->addMenu(pmen2)->setText( tr("Conve&rt to"));
			}
			pmen->addSeparator();
			if (currDoc->m_Selection->count() == 1)
				pmen->addAction( tr("Rename") , this, SLOT(slotRenameItem()));
			if (!currItem->locked() && !(currItem->isSingleSel))
				pmen->addAction(m_MainWindow->scrActions["editCut"]);
			if (!(currItem->isSingleSel))
				pmen->addAction(m_MainWindow->scrActions["editCopy"]);
			if ((currDoc->appMode == modeEdit) && (m_MainWindow->Buffer2.startsWith("<SCRIBUSTEXT")) && (currItem->itemType() == PageItem::TextFrame))
				pmen->addAction(m_MainWindow->scrActions["editPaste"]);
			if (!currItem->locked() && (currDoc->appMode != modeEdit) && (!(currItem->isSingleSel)))
				pmen->addAction( tr("&Delete"), currDoc, SLOT(itemSelection_DeleteItem()));
			if ((currItem->itemType() == PageItem::ImageFrame) || (currItem->itemType() == PageItem::TextFrame))
			{
				if (currItem->itemType() == PageItem::ImageFrame)
				{
					if (currItem->PicAvail)
						pmenEditContents->addAction(m_MainWindow->scrActions["editCopyContents"]);
					if (m_MainWindow->contentsBuffer.sourceType==PageItem::ImageFrame)
					{
						pmenEditContents->addAction(m_MainWindow->scrActions["editPasteContents"]);
						pmenEditContents->addAction(m_MainWindow->scrActions["editPasteContentsAbs"]);
					}
					if (currItem->PicAvail)
						pmenEditContents->addAction(m_MainWindow->scrActions["editClearContents"]);
					if ((currItem->PicAvail) || (m_MainWindow->contentsBuffer.sourceType==PageItem::ImageFrame))
						pmen->addMenu(pmenEditContents)->setText( tr("Contents"));
				}
				else
				{
					if (currItem->itemText.lines() != 0)
					{
						pmenEditContents->addAction(m_MainWindow->scrActions["editClearContents"]);
						pmen->addMenu(pmenEditContents)->setText( tr("Contents"));
					}
				}
			}
			pmen->exec(QCursor::pos());
			m_MainWindow->scrActions["editActionMode"]->setChecked(!_isGlobalMode);
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
	*/
}

void OutlinePalette::slotRenameItem()
{
	activateWindow();
	reportDisplay->setFocus();
	reportDisplay->editItem(currentObject);
}

void OutlinePalette::slotDoRename(QTreeWidgetItem *ite , int col)
{
	if (!m_MainWindow || m_MainWindow->ScriptRunning)
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
						item->PageItemObject->AutoName = false;
						m_MainWindow->propertiesPalette->SetCurItem(item->PageItemObject);
						currDoc->setModified(true);
					}
				}
			}
		}
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
	if (!m_MainWindow || m_MainWindow->ScriptRunning)
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
		uint docSelectionCount = currDoc->m_Selection->count();
		for (uint a = 0; a < docSelectionCount; a++)
		{
			PageItem *item = currDoc->m_Selection->itemAt(a);
			QTreeWidgetItem *retVal = getListItem(item->OwnPage, item->ItemNr);
			if (retVal != 0)
				retVal->setSelected(true);
		}
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
	if (!m_MainWindow || m_MainWindow->ScriptRunning)
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
	if (!m_MainWindow || m_MainWindow->ScriptRunning)
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
						currDoc->m_Selection->setIsGUISelection(false);
						currDoc->view()->SelectItemNr(pgItem->ItemNr, false, false);
					}
					break;
			}
		}
		if (currDoc->m_Selection->count() > 0)
		{
			currDoc->m_Selection->setIsGUISelection(true);
			currDoc->m_Selection->connectItemToGUI();
		}
		currDoc->view()->DrawNew();
	}
	else
		slotSelect(items[0], 0);
	selectionTriggered = false;
	connect(reportDisplay, SIGNAL(itemSelectionChanged()), this, SLOT(slotMultiSelect()));
}

void OutlinePalette::slotSelect(QTreeWidgetItem* ite, int col)
{
	if (!m_MainWindow || m_MainWindow->ScriptRunning)
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
}

void OutlinePalette::resizeEvent(QResizeEvent *r)
{
	reportDisplay->resize(r->size());
}

void OutlinePalette::BuildTree(bool storeVals)
{
	if (!m_MainWindow || m_MainWindow->ScriptRunning)
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
	if (currDoc->m_Selection->count() > 0)
		slotShowSelect(0, -1);
	repaint();
	connect(reportDisplay, SIGNAL(itemSelectionChanged()), this, SLOT(slotMultiSelect()));
	connect(reportDisplay, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(slotDoRename(QTreeWidgetItem*, int)));
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
		cmen = new ContextMenu(m_MainWindow, currDoc, mx, my);
	if (cmen)
		cmen->exec(QCursor::pos());
	delete cmen;
}
