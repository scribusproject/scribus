/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "checkDocument.h"

#include <QEvent>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QImage>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QSpacerItem>
#include <QToolTip>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVBoxLayout>


#include "sccombobox.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "page.h"
#include "prefsmanager.h"
#include "documentchecker.h"
#include "util_icon.h"
#include "util.h"


// readable constants for QTreeWidgetItem column ids
#define COLUMN_ITEM 0
#define COLUMN_PROBLEM 1
#define COLUMN_LAYER 2
// #define COLUMN_INFO 3

CheckDocument::CheckDocument( QWidget* parent, bool modal )
	: ScrPaletteBase( parent, "checkDocument", modal, 0 ),
	m_Doc(0),
	pageGraveError(false),
	itemError(false),
	minResDPI(0),
	maxResDPI(0)
{
	showPagesWithoutErrors=PrefsManager::instance()->appPrefs.verifierPrefs.showPagesWithoutErrors;
	showNonPrintingLayerErrors=PrefsManager::instance()->appPrefs.verifierPrefs.showNonPrintingLayerErrors;

	graveError = loadIcon("22/dialog-error.png");
	onlyWarning = loadIcon("22/dialog-warning.png");
	noErrors = loadIcon("ok.png");
	checkDocumentLayout = new QVBoxLayout( this );
	checkDocumentLayout->setMargin(5);
	checkDocumentLayout->setSpacing(5);

	layout1 = new QHBoxLayout;
	layout1->setMargin(0);
	layout1->setSpacing(5);
	textLabel1 = new QLabel( this );
	layout1->addWidget( textLabel1 );
	curCheckProfile = new ScComboBox( this );
	layout1->addWidget( curCheckProfile );
	checkDocumentLayout->addLayout( layout1 );

	reportDisplay = new QTreeWidget( this );
	reportDisplay->header()->setClickable( false );
	reportDisplay->header()->setMovable( false );
	reportDisplay->setSortingEnabled(false);
	reportDisplay->setAlternatingRowColors(true);
	checkDocumentLayout->addWidget( reportDisplay );

	layout2 = new QHBoxLayout;
	layout2->setMargin(0);
	layout2->setSpacing(5);
	reScan = new QPushButton(this );
	layout2->addWidget( reScan );
	QSpacerItem* spacer = new QSpacerItem( 2, 2, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout2->addItem( spacer );
	ignoreErrors = new QPushButton(this );
	layout2->addWidget( ignoreErrors );
	checkDocumentLayout->addLayout( layout2 );
	setIgnoreEnabled(false);
	checkMode = checkNULL;
	languageChange();
	itemMap.clear();
	pageMap.clear();
	masterPageMap.clear();
	masterPageItemMap.clear();
	resize( QSize(320, 260).expandedTo(minimumSizeHint()) );

	connect(ignoreErrors, SIGNAL(clicked()), this, SIGNAL(ignoreAllErrors()));
	connect(curCheckProfile, SIGNAL(activated(const QString&)), this, SLOT(newScan(const QString&)));
	connect(reScan, SIGNAL(clicked()), this, SLOT(doReScan()));
}

void CheckDocument::slotSelect(QTreeWidgetItem* ite)
{
	if (itemMap.contains(ite))
	{
		ScCore->primaryMainWindow()->closeActiveWindowMasterPageEditor();
		emit selectElementByItem(itemMap[ite], true);
		return;
	}
	if (pageMap.contains(ite))
	{
		ScCore->primaryMainWindow()->closeActiveWindowMasterPageEditor();
		emit selectPage(pageMap[ite]);
		return;
	}
	if (masterPageMap.contains(ite))
	{
		emit selectMasterPage(masterPageMap[ite]);
		return;
	}
	if (masterPageItemMap.contains(ite))
	{
		if (!m_Doc->masterPageMode())
			emit selectMasterPage(masterPageItemMap[ite]->OnMasterPage);
		emit selectElementByItem(masterPageItemMap[ite], true);
		return;
	}
}

void CheckDocument::doReScan()
{
	showPagesWithoutErrors=PrefsManager::instance()->appPrefs.verifierPrefs.showPagesWithoutErrors;
	showNonPrintingLayerErrors=PrefsManager::instance()->appPrefs.verifierPrefs.showNonPrintingLayerErrors;
	newScan(curCheckProfile->currentText());
}

void CheckDocument::newScan(const QString& name)
{
	clearErrorList();
	if (m_Doc==0)
		return;
	m_Doc->setCurCheckProfile(name);
	DocumentChecker::checkDocument(m_Doc);
	buildErrorList(m_Doc);
}

void CheckDocument::clearErrorList()
{
	disconnect(reportDisplay, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(slotSelect(QTreeWidgetItem*)));
	reportDisplay->clear();
	reportDisplay->setSortingEnabled(false);
	itemMap.clear();
	pageMap.clear();
	masterPageMap.clear();
	masterPageItemMap.clear();
}

void CheckDocument::buildItem(QTreeWidgetItem * item,
							   PreflightError errorType,
							   PageItem * pageItem)
{
	Q_ASSERT_X(item != 0, "CheckDocument::buildItem",
				"No reference to QTreeWidgetItem item");
	Q_ASSERT_X(pageItem != 0, "CheckDocument::buildItem",
				"No reference to PageItem pageItem");

	switch (errorType)
	{
		case MissingGlyph:
			item->setText(COLUMN_PROBLEM, missingGlyph);
			item->setIcon(COLUMN_ITEM, graveError );
// 			globalGraveError = true;
			pageGraveError = true;
			itemError = true;
			break;
		case TextOverflow:
			item->setText(COLUMN_PROBLEM, textOverflow);
			item->setIcon(COLUMN_ITEM, onlyWarning );
			break;
		case ObjectNotOnPage:
			item->setText(COLUMN_PROBLEM, notOnPage);
			item->setIcon(COLUMN_ITEM, onlyWarning );
			break;
		case MissingImage:
			if (pageItem->externalFile().isEmpty())
			{
				item->setText(COLUMN_PROBLEM, emptyImg);
				item->setIcon(COLUMN_ITEM, onlyWarning );
			}
			else
			{
				item->setText(COLUMN_PROBLEM, missingImg);
				item->setIcon(COLUMN_ITEM, graveError );
// 				globalGraveError = true;
				pageGraveError = true;
			}
			break;
		case ImageDPITooLow:
		{
			int xres = qRound(72.0 / pageItem->imageXScale());
			int yres = qRound(72.0 / pageItem->imageYScale());
			item->setText(COLUMN_PROBLEM, lowDPI.arg(minResDPI).arg(xres).arg(yres));
			item->setIcon(COLUMN_ITEM, onlyWarning );
			break;
		}
		case ImageDPITooHigh:
		{
			int xres = qRound(72.0 / pageItem->imageXScale());
			int yres = qRound(72.0 / pageItem->imageYScale());
			item->setText(COLUMN_PROBLEM, highDPI.arg(maxResDPI).arg(xres).arg(yres));
			item->setIcon(COLUMN_ITEM, onlyWarning );
			break;
		}
		case PartFilledImageFrame:
			item->setText(COLUMN_PROBLEM, partFilledImageFrame);
			item->setIcon(COLUMN_ITEM, onlyWarning);
			break;
		case Transparency:
			item->setText(COLUMN_PROBLEM, transpar);
			item->setIcon(COLUMN_ITEM, graveError );
// 			globalGraveError = true;
			pageGraveError = true;
			itemError = true;
			break;
		case PDFAnnotField:
			item->setText(COLUMN_PROBLEM, annot);
			item->setIcon(COLUMN_ITEM, onlyWarning );
			break;
		case PlacedPDF:
			item->setText(COLUMN_PROBLEM, rasterPDF);
			item->setIcon(COLUMN_ITEM, onlyWarning );
			break;
		case ImageIsGIF:
			item->setText(COLUMN_PROBLEM, isGIF);
			item->setIcon(COLUMN_ITEM, onlyWarning);
			//errorText->setToolTip( isGIFtoolTip);
			break;
		case WrongFontInAnnotation:
			item->setText(COLUMN_PROBLEM, WrongFont);
			item->setIcon(COLUMN_ITEM, graveError );
// 			globalGraveError = true;
			pageGraveError = true;
			itemError = true;
			break;
		default:
			break;
	};
	// additional informations
	const ScLayer* layer = m_Doc->Layers.layerByID(pageItem->LayerID);
	if (layer)
	{
		item->setText(COLUMN_LAYER, layer->Name);
		item->setData(COLUMN_LAYER, Qt::DecorationRole, layer->markerColor);
	}
// 	if (pageItem->asTextFrame())
// 	{
// 		int l = pageItem->itemText.length();
// 		// preview of the text
// 		item->setText(COLUMN_INFO, pageItem->itemText.text(0, l > 20 ? 20 : l));
// 	}
// 	else if (pageItem->asImageFrame())
// 		item->setText(COLUMN_INFO, pageItem->externalFile());
}

void CheckDocument::buildErrorList(ScribusDoc *doc)
{
// 	bool resultError = false;
	m_Doc = doc;
	disconnect(curCheckProfile, SIGNAL(activated(const QString&)), this, SLOT(newScan(const QString&)));
	curCheckProfile->clear();
	clearErrorList();

	if (m_Doc==0)
		return;

	minResDPI = qRound(doc->checkerProfiles()[doc->curCheckProfile()].minResolution);
	maxResDPI = qRound(doc->checkerProfiles()[doc->curCheckProfile()].maxResolution);
	
	CheckerPrefsList::Iterator it;
	CheckerPrefsList::Iterator itend=doc->checkerProfiles().end();
	for (it = doc->checkerProfiles().begin(); it != itend ; ++it)
		curCheckProfile->addItem(it.key());
	setCurrentComboItem(curCheckProfile, doc->curCheckProfile());

	if ((doc->docItemErrors.count() == 0)
		 && (doc->masterItemErrors.count() == 0)
		 && (doc->docLayerErrors.count() == 0))
	{
		QTreeWidgetItem * documentItem = new QTreeWidgetItem( reportDisplay );
		documentItem->setText(COLUMN_ITEM, tr( "Document" ) );
		documentItem->setIcon(COLUMN_ITEM, noErrors );
		documentItem->setText(COLUMN_PROBLEM, tr( "No Problems found" ) );
		ignoreErrors->setText( tr("OK"));
	}
	else
	{
// 		resultError = true;
		bool hasError = false;
// 		globalGraveError = false;
		bool layoutGraveError = false;
		itemError = false;
// 		QTreeWidgetItem * pagep = 0;
		// LAYERS **********************************************8
		QTreeWidgetItem * layerItem = new QTreeWidgetItem(reportDisplay);
		layerItem->setText(COLUMN_ITEM, tr("Layers"));

		if (doc->docLayerErrors.count() != 0)
		{
			QMap<int, errorCodes>::Iterator docLayerErrorsIt;
			errorCodes::Iterator layerErrorsIt;

			for (docLayerErrorsIt = doc->docLayerErrors.begin();
				 docLayerErrorsIt != doc->docLayerErrors.end();
				 ++docLayerErrorsIt)
			{
				QTreeWidgetItem * layer = new QTreeWidgetItem(layerItem);//, pagep );
				for (layerErrorsIt = docLayerErrorsIt.value().begin();
					 layerErrorsIt != docLayerErrorsIt.value().end(); ++layerErrorsIt)
				{
					QTreeWidgetItem * errorText = new QTreeWidgetItem( layer, 0 );
					switch (layerErrorsIt.key())
					{
						case Transparency:
							errorText->setText(COLUMN_ITEM, tr("Transparency used"));
							errorText->setIcon(COLUMN_ITEM, graveError );
							layoutGraveError = true;
							break;
						case BlendMode:
							errorText->setText(COLUMN_ITEM, tr("Blendmode used"));
							errorText->setIcon(COLUMN_ITEM, graveError );
							layoutGraveError = true;
							break;
						case OffConflictLayers:
							errorText->setText(COLUMN_ITEM, tr("Print/Visible Mismatch"));
							errorText->setIcon(COLUMN_ITEM, onlyWarning );
							break;
						default:
							break;
					}
//					errorText->setIcon(COLUMN_ITEM, graveError );
				}
				layer->setText(COLUMN_ITEM,tr("Layer \"%1\"").arg(doc->layerName(docLayerErrorsIt.key())));
				if (layoutGraveError)
					layer->setIcon(COLUMN_ITEM, graveError );
				else
					layer->setIcon(COLUMN_ITEM, onlyWarning );
				layer->setText(COLUMN_PROBLEM, tr("Issue(s): %1").arg(doc->docLayerErrors[docLayerErrorsIt.key()].count()));
				layer->setExpanded(true);
// 				pagep = layer;
// 				globalGraveError = true;
			}
			layerItem->setExpanded(true);
		}
		// END of LAYERS

		// Master Pages *****************************************************
		QTreeWidgetItem * masterPageRootItem = new QTreeWidgetItem(reportDisplay);
		masterPageRootItem->setText(COLUMN_ITEM, tr("Master Pages"));
		int mpErrorCount = 0;

		for (int mPage = 0; mPage < doc->MasterPages.count(); ++mPage)
		{
			QString tmp;
			hasError = false;
			pageGraveError = false;
			QTreeWidgetItem * page=NULL;
			if (showPagesWithoutErrors)
			{
				page = new QTreeWidgetItem( masterPageRootItem);
				masterPageMap.insert(page, doc->MasterPages.at(mPage)->pageName());
			}
// 			pagep = page;
			QMap<PageItem*, errorCodes>::Iterator masterItemErrorsIt;
			for (masterItemErrorsIt = doc->masterItemErrors.begin();
				 masterItemErrorsIt != doc->masterItemErrors.end();
				 ++masterItemErrorsIt)
			{
				if (((masterItemErrorsIt.key()->OwnPage == mPage)
					|| (masterItemErrorsIt.key()->OnMasterPage == doc->MasterPages.at(mPage)->pageName()))
					&&
					((showNonPrintingLayerErrors) ||
					(!showNonPrintingLayerErrors && doc->layerPrintable(masterItemErrorsIt.key()->LayerID)))
					)
				{
					if (!showPagesWithoutErrors && page==NULL)
					{
						page = new QTreeWidgetItem( masterPageRootItem);
						masterPageMap.insert(page, doc->MasterPages.at(mPage)->pageName());
					}
					hasError = true;
					QTreeWidgetItem * object = new QTreeWidgetItem( page);
					masterPageItemMap.insert(object, masterItemErrorsIt.key());
					object->setText(COLUMN_ITEM, masterItemErrorsIt.key()->itemName());
					errorCodes::Iterator it3;
					if (masterItemErrorsIt.value().count() == 1)
					{
						it3 = masterItemErrorsIt.value().begin();
						buildItem(object, it3.key(), masterItemErrorsIt.key());
					}
					else
					{
						for (it3 = masterItemErrorsIt.value().begin(); it3 != masterItemErrorsIt.value().end(); ++it3)
						{
							QTreeWidgetItem * errorText = new QTreeWidgetItem( object, 0 );
							buildItem(errorText, it3.key(), masterItemErrorsIt.key());
						}
						object->setExpanded( true );
					}
					if (itemError)
						object->setIcon(COLUMN_ITEM, graveError );
					else
						object->setIcon(COLUMN_ITEM, onlyWarning );
				}
			}
			if (hasError)
			{
				++mpErrorCount;
				if (pageGraveError)
					page->setIcon(COLUMN_ITEM, graveError );
				else
					page->setIcon(COLUMN_ITEM, onlyWarning );
				page->setExpanded( true );
			}
			else
			{
				if (showPagesWithoutErrors && page!=NULL)
					page->setIcon(COLUMN_ITEM, noErrors );
			}
			if (page!=NULL)
				page->setText(COLUMN_ITEM, doc->MasterPages.at(mPage)->pageName());
		}
		masterPageRootItem->setExpanded(true);
		masterPageRootItem->setText(COLUMN_PROBLEM, tr("Issue(s): %1").arg(mpErrorCount));
		// END of MASTER PAGES

		// PAGES ********************************8
		for (int aPage = 0; aPage < doc->DocPages.count(); ++aPage)
		{
			QString tmp;
			hasError = false;
			pageGraveError = false;
			QTreeWidgetItem * page=NULL;
			if (showPagesWithoutErrors)
			{
				page = new QTreeWidgetItem( reportDisplay);
				pageMap.insert(page, aPage);
			}
// 			pagep = page;
			QMap<PageItem*, errorCodes>::Iterator docItemErrorsIt;
			for (docItemErrorsIt = doc->docItemErrors.begin();
				 docItemErrorsIt != doc->docItemErrors.end();
				 ++docItemErrorsIt)
			{
				if (docItemErrorsIt.key()->OwnPage == aPage &&
					((showNonPrintingLayerErrors) ||
					(!showNonPrintingLayerErrors && doc->layerPrintable(docItemErrorsIt.key()->LayerID)))
					)
				{
					if (!showPagesWithoutErrors && page==NULL)
					{
						page = new QTreeWidgetItem( reportDisplay);
						pageMap.insert(page, aPage);
					}
					hasError = true;
					itemError = false;
					QTreeWidgetItem * object = new QTreeWidgetItem(page);
					object->setText(COLUMN_ITEM, docItemErrorsIt.key()->itemName());
					itemMap.insert(object, docItemErrorsIt.key());
					errorCodes::Iterator it3;
					if (docItemErrorsIt.value().count() == 1)
					{
						it3 = docItemErrorsIt.value().begin();
						buildItem(object, it3.key(), docItemErrorsIt.key());
					}
					else
					{
						for (it3 = docItemErrorsIt.value().begin(); it3 != docItemErrorsIt.value().end(); ++it3)
						{
							QTreeWidgetItem * errorText = new QTreeWidgetItem( object);
							buildItem(errorText, it3.key(), docItemErrorsIt.key());
						}
						object->setExpanded( true );
					}
					if (itemError)
						object->setIcon(COLUMN_ITEM, graveError );
					else
						object->setIcon(COLUMN_ITEM, onlyWarning );
				}
			}
			if (hasError)
			{
				if (pageGraveError)
					page->setIcon(COLUMN_ITEM, graveError );
				else
					page->setIcon(COLUMN_ITEM, onlyWarning );
				page->setExpanded( true );
			}
			else
			{
				if (showPagesWithoutErrors && page!=NULL)
					page->setIcon( 0, noErrors );
			}
			if (page!=NULL)
				page->setText(COLUMN_ITEM, tr("Page ")+tmp.setNum(aPage+1));
		}
		// END of PAGES

		// FREE ITEMS **********************************************8888
		QMap<PageItem*, errorCodes>::Iterator freeItemsErrorsIt;
		bool hasfreeItems = false;
		for (freeItemsErrorsIt = doc->docItemErrors.begin();
			 freeItemsErrorsIt != doc->docItemErrors.end();
			 ++freeItemsErrorsIt)
		{
			if (freeItemsErrorsIt.key()->OwnPage == -1)
			{
				hasfreeItems = true;
				break;
			}
		}
		if (hasfreeItems)
		{
			bool hasError = false;
			bool pageGraveError = false;
			QTreeWidgetItem * freeItem = new QTreeWidgetItem( reportDisplay);//, pagep );
// 			pagep = page;
			for (freeItemsErrorsIt = doc->docItemErrors.begin();
				 freeItemsErrorsIt != doc->docItemErrors.end();
				 ++freeItemsErrorsIt)
			{
				if (freeItemsErrorsIt.key()->OwnPage == -1)
				{
					hasError = true;
					QTreeWidgetItem * object = new QTreeWidgetItem(freeItem);
					object->setText(0, freeItemsErrorsIt.key()->itemName());
					itemMap.insert(object, freeItemsErrorsIt.key());
					errorCodes::Iterator it3;
					if (freeItemsErrorsIt.value().count() == 1)
					{
						it3 = freeItemsErrorsIt.value().begin();
						buildItem(object, it3.key(), freeItemsErrorsIt.key());
					}
					else
					{
						for (it3 = freeItemsErrorsIt.value().begin(); it3 != freeItemsErrorsIt.value().end(); ++it3)
						{
							QTreeWidgetItem * errorText = new QTreeWidgetItem( object);
							buildItem(errorText, it3.key(), freeItemsErrorsIt.key());
						}
						object->setExpanded( true );
					}
					if (pageGraveError)
						object->setIcon(COLUMN_ITEM, graveError );
					else
						object->setIcon(COLUMN_ITEM, onlyWarning );
				}
			}
			if (hasError)
			{
				if (pageGraveError)
					freeItem->setIcon(COLUMN_ITEM, graveError );
				else
					freeItem->setIcon(COLUMN_ITEM, onlyWarning );
				freeItem->setExpanded( true );
			}
			else
				freeItem->setIcon(COLUMN_ITEM, noErrors );
			freeItem->setText(COLUMN_ITEM, tr("Free Objects"));
		}
		// END of FREE ITEMS

// 		if (globalGraveError)
// 			documentItem->setIcon(COLUMN_ITEM, graveError );
// 		else
// 			documentItem->setIcon(COLUMN_ITEM, onlyWarning );
// 		documentItem->setText(COLUMN_PROBLEM, tr( "Problems found" ) );
// 		documentItem->setExpanded( true );
		ignoreErrors->setText( tr("&Ignore Errors"));
	}

	reportDisplay->resizeColumnToContents(COLUMN_ITEM);
	reportDisplay->resizeColumnToContents(COLUMN_PROBLEM);
	reportDisplay->resizeColumnToContents(COLUMN_LAYER);
// 	reportDisplay->resizeColumnToContents(COLUMN_INFO);
	connect(curCheckProfile, SIGNAL(activated(const QString&)), this, SLOT(newScan(const QString&)));
	connect(reportDisplay, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(slotSelect(QTreeWidgetItem*)));
}

void CheckDocument::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
	else
		QWidget::changeEvent(e);
}

void CheckDocument::languageChange()
{
	setWindowTitle( tr( "Preflight Verifier" ) );
	QStringList headerLabels;
	headerLabels << tr("Items") << tr("Problems")
	             << tr("Layer");// << tr("Information");
	reportDisplay->setHeaderLabels(headerLabels);
	reportDisplay->setColumnCount(headerLabels.count());

	textLabel1->setText( tr("Current Profile:"));
	ignoreErrors->setText( tr("&Ignore Errors"));
	reScan->setText( tr("Check again"));
	
	textLabel1->setToolTip( "<qt>"+ tr( "Preflight profile to base the report generation on. Options can be set in Document Setup or Preferences") + "</qt>");
	ignoreErrors->setToolTip( "<qt>"+ tr( "Ignore found errors and continue to export or print. Be sure to understand the errors you are ignoring before continuing.") + "</qt>");
	reScan->setToolTip( "<qt>"+ tr( "Rerun the document scan to check corrections you may have made" ) + "</qt>");
	
	missingGlyph = tr("Glyphs missing");
	textOverflow = tr("Text overflow");
	notOnPage = tr("Object is not on a Page");
	missingImg = tr("Missing Image");
	emptyImg = tr("Empty Image Frame");
	lowDPI = tr("Image resolution below %1 DPI,\ncurrently %2 x %3 DPI");
	highDPI = tr("Image resolution above %1 DPI,\ncurrently %2 x %3 DPI");
	transpar = tr("Object has transparency");
	annot = tr("Object is a PDF Annotation or Field");
	rasterPDF = tr("Object is a placed PDF");
	isGIF = tr("Image is GIF");
	WrongFont = tr("Annotation uses a non TrueType font");
	partFilledImageFrame = tr("Image dimension is smaller than its frame");
	//isGIFtoolTip = "<qt>" + tr("GIF images are not reccomended for print. See the online docs for more info") + "</qt>";
}

void CheckDocument::setIgnoreEnabled(bool state)
{
	noButton = !state;
	ignoreErrors->setShown(state);
}

bool CheckDocument::isIgnoreEnabled()
{
	return !noButton;
}
