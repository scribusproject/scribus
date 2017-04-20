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

#include "documentchecker.h"
#include "prefsmanager.h"
#include "sccombobox.h"
#include "scpage.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "iconmanager.h"
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

	graveError = IconManager::instance()->loadPixmap("22/dialog-error.png");
	onlyWarning = IconManager::instance()->loadPixmap("22/dialog-warning.png");
	noErrors = IconManager::instance()->loadPixmap("ok.png");
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
	reportDisplay->header()->setSectionsClickable(false );
	reportDisplay->header()->setSectionsMovable( false );
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
	posMap.clear();
	pageMap.clear();
	masterPageMap.clear();
	masterPageItemMap.clear();
	resize( QSize(320, 260).expandedTo(minimumSizeHint()) );

	connect(ignoreErrors, SIGNAL(clicked()), this, SIGNAL(ignoreAllErrors()));
	connect(curCheckProfile, SIGNAL(activated(const QString&)), this, SLOT(newScan(const QString&)));
	connect(reScan, SIGNAL(clicked()), this, SLOT(doReScan()));
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

	textLabel1->setToolTip( "<qt>"+ tr( "Preflight profile to base the report generation on. Options can be set in Document Setup or Preferences.") + "</qt>");
	ignoreErrors->setToolTip( "<qt>"+ tr( "Ignore found errors and continue to export or print. Be sure to understand the errors you are ignoring before continuing.") + "</qt>");
	reScan->setToolTip( "<qt>"+ tr( "Rerun the document scan to check corrections you may have made" ) + "</qt>");

	warnMap.clear();
	warnMap.insert(PV_ANNOTATION,				qMakePair(tr("Object is a PDF Annotation or Field"),					tr("Indicates that editorial changes have been made to a PDF are still present or your PDF contains unprintable annotation items. They may cause issues in professional printing. Also helpful reminder if you are wanting to publish a final draft without editorial relics.")));
	warnMap.insert(PV_APPLIED_MASTER_DIFF_SIDE,	qMakePair(tr("Applied master page has different page destination (left, middle, right side)"),
																														tr("Have you applied the correct Master Page?")));
	warnMap.insert(PV_EMPTY_IMAGE_FRAME,		qMakePair(tr("Empty Image Frame"),										tr("If you have created an image frame, there is the presumption that you planned to put an image in it")));
	warnMap.insert(PV_EMPTY_TEXT_FRAME,			qMakePair(tr("Empty Text Frame"),										tr("If you have created a text frame, there is the presumption that you planned to put text in it")));
	warnMap.insert(PV_FONT_NOT_EMBEDDED,		qMakePair(tr("Imported document contains non-embedded fonts"),			tr("When some imported document uses non-embedded fonts, then their rendering will be wrong, unless by chance you have them installed on their system, but that cannot be guaranteed in case you want to share the resulting document")));
	warnMap.insert(PV_HIGH_DPI,					qMakePair(tr("Image resolution above %1 DPI,\ncurrently %2 x %3 DPI"),	tr("This is a user definable setting serving as a caution for high resolution images, which may lead to unnecessarily large files")));
	warnMap.insert(PV_IMAGE_FRAME_PART_FILLED,	qMakePair(tr("Image dimension is smaller than its frame"),				tr("The image does not fit the whole space you reserved for it. Maybe this is intended, or maybe this is caused by bad inner placement or scale. The result will either be a cropped image or white space around the image.")));
	warnMap.insert(PV_IS_GIF,					qMakePair(tr("Image is GIF"),											tr("This warning alerts you that you are using a bitmap based graphic format that is typically not used for high resolution images (.gif is one of those). This may result in poor viewing quality (for example: when commercially printed, viewed on a high-resolution screens, etc...). If your PDF will be printed commercially, there are some printing systems that will have difficulty printing these types of images.")));
	warnMap.insert(PV_LOW_DPI,					qMakePair(tr("Image resolution below %1 DPI,\ncurrently %2 x %3 DPI"),	tr("This is a user definable setting serving as a caution for low resolution images, which may lead to poor quality output")));
	warnMap.insert(PV_MISSING_GLYPH,			qMakePair(tr("Glyphs missing"),											tr("You have one or more characters which do not have a corresponding glyph in your chosen font")));
	warnMap.insert(PV_MISSING_IMAGE,			qMakePair(tr("Missing Image"),											tr("The assigned image file cannot be found")));
	warnMap.insert(PV_NON_ON_PAGE,				qMakePair(tr("Object is not on a Page"),								tr("An object is placed somewhere outside of the page borders, it will not be printed and might be missing somewhere")));
	warnMap.insert(PV_NOT_CMYK_SPOT,			qMakePair(tr("Object colorspace is not CMYK or spot"),					tr("PDF supports many different ways to represent the color of any object including RGB, CMYK and Spot (aka Separation) colors. Some of the PDF standards, such as PDF/X-1a, require the only CMYK and Spot colors be used.")));
	warnMap.insert(PV_RASTER_PDF,				qMakePair(tr("Object is a placed PDF"),									tr("The warning is verifying for you that there is a PDF loaded into an Image Frame, where it will be rasterized or converted to a bitmap. Its resolution may be less than ideal. See PDF Export to learn how to minimize this problem.")));
	warnMap.insert(PV_TEXT_OVERFLOW,			qMakePair(tr("Text overflow"),											tr("There is more text than can show in the frame as sized. Nonvisible excess characters like spaces and carriage returns may trigger this if nothing appears to be missing.")));
	warnMap.insert(PV_TRANSPARENCY,				qMakePair(tr("Object has transparency"),								tr("This warning indicates that your document contains images that have a transparent layer. This is really only an issue if using older printing profiles or PostScript. It is safe to ignore this when exporting to PDF version greater than 1.4.")));
	warnMap.insert(PV_WRONG_FONT,				qMakePair(tr("Annotation uses a non-TrueType font"),					tr("Annotations support only a standard set of fonts. Choose another one.")));
	warnMap.insert(PV_LAYER_TRANSPARENCY,		qMakePair(tr("Transparency used"),										tr("This layer uses transparency, only an issue if using older printing profiles. You may safely ignore this when using modern printing methods, or exporting to PDF version greater than 1.4.")));
	warnMap.insert(PV_LAYER_BLENDMODE,			qMakePair(tr("Blendmode used"),											tr("This layer uses blendmodes which relies on transparency, only an issue if using older printing profiles. You may safely ignore this when using modern printing methods, or exporting to PDF version greater than 1.4.")));
	warnMap.insert(PV_LAYER_PRINTVIS_MISMATCH,	qMakePair(tr("Print/Visible mismatch"),									tr("This layer uses transparency, only an issue if using older printing profiles. You may safely ignore this when using modern printing methods, or exporting to PDF version greater than 1.4.")));
}


void CheckDocument::setDoc(ScribusDoc *doc)
{
	m_Doc = doc;
	clearErrorList();
	CheckerPrefsList::Iterator it;
	CheckerPrefsList::Iterator itend=doc->checkerProfiles().end();
	for (it = doc->checkerProfiles().begin(); it != itend ; ++it)
		curCheckProfile->addItem(it.key());
	setCurrentComboItem(curCheckProfile, doc->curCheckProfile());
}

void CheckDocument::slotSelect(QTreeWidgetItem* ite)
{
	if (itemMap.contains(ite))
	{
		// #10537 Check item has not been destroyed before requesting its selection
		if (itemMap[ite].isNull())
			return;
		ScCore->primaryMainWindow()->closeActiveWindowMasterPageEditor();
		m_Doc->setActiveLayer(itemMap[ite]->LayerID);
		ScCore->primaryMainWindow()->changeLayer(m_Doc->activeLayer());
		if (itemMap[ite]->isTextFrame())
			emit selectElement(itemMap[ite], true, posMap[ite]);
		else
			emit selectElementByItem(itemMap[ite], true);
		return;
	}
	if (pageMap.contains(ite))
	{
		// #10537 Get page index from pointer in case user has deleted a page
		// after preflight has been run
		int pageIndex = m_Doc->DocPages.indexOf(pageMap[ite]);
		if (pageIndex < 0)
			return;
		ScCore->primaryMainWindow()->closeActiveWindowMasterPageEditor();
		emit selectPage(pageIndex);
		return;
	}
	if (masterPageMap.contains(ite))
	{
		// #10537 Get page index from pointer in case user has deleted a page
		// after preflight has been run
		int pageIndex = m_Doc->MasterPages.indexOf(masterPageMap[ite]);
		if (pageIndex < 0)
			return;
		emit selectMasterPage(masterPageMap[ite]->pageName());
		return;
	}
	if (masterPageItemMap.contains(ite))
	{
		// #10537 Check item has not been destroyed before requesting its selection
		if (masterPageItemMap[ite].isNull())
			return;
		if (!m_Doc->masterPageMode())
			emit selectMasterPage(masterPageItemMap[ite]->OnMasterPage);
		m_Doc->setActiveLayer(masterPageItemMap[ite]->LayerID);
		ScCore->primaryMainWindow()->changeLayer(m_Doc->activeLayer());
		if (masterPageItemMap[ite]->isTextFrame())
			emit selectElement(masterPageItemMap[ite], true, posMap[ite]);
		else
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
	posMap.clear();
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
			item->setText(COLUMN_PROBLEM, warnMap[PV_MISSING_GLYPH].first);
			item->setToolTip(COLUMN_PROBLEM, warnMap[PV_MISSING_GLYPH].second);
			item->setIcon(COLUMN_ITEM, graveError );
			pageGraveError = true;
			itemError = true;
			break;
		case TextOverflow:
			item->setText(COLUMN_PROBLEM, warnMap[PV_TEXT_OVERFLOW].first);
			item->setToolTip(COLUMN_PROBLEM, warnMap[PV_TEXT_OVERFLOW].second);
			item->setIcon(COLUMN_ITEM, onlyWarning );
			break;
		case ObjectNotOnPage:
			item->setText(COLUMN_PROBLEM, warnMap[PV_NON_ON_PAGE].first);
			item->setToolTip(COLUMN_PROBLEM, warnMap[PV_NON_ON_PAGE].second);
			item->setIcon(COLUMN_ITEM, onlyWarning );
			break;
		case MissingImage:
			if (pageItem->externalFile().isEmpty())
			{
				item->setText(COLUMN_PROBLEM, warnMap[PV_EMPTY_IMAGE_FRAME].first);
				item->setToolTip(COLUMN_PROBLEM, warnMap[PV_EMPTY_IMAGE_FRAME].second);
				item->setIcon(COLUMN_ITEM, onlyWarning );
			}
			else
			{
				item->setText(COLUMN_PROBLEM, warnMap[PV_MISSING_IMAGE].first);
				item->setToolTip(COLUMN_PROBLEM, warnMap[PV_MISSING_IMAGE].second);
				item->setIcon(COLUMN_ITEM, graveError );
				pageGraveError = true;
			}
			break;
		case ImageDPITooLow:
		{
			int xres = qRound(72.0 / pageItem->imageXScale());
			int yres = qRound(72.0 / pageItem->imageYScale());
			item->setText(COLUMN_PROBLEM, warnMap[PV_LOW_DPI].first.arg(minResDPI).arg(xres).arg(yres));
			item->setToolTip(COLUMN_PROBLEM, warnMap[PV_LOW_DPI].second);
			item->setIcon(COLUMN_ITEM, onlyWarning );
			break;
		}
		case ImageDPITooHigh:
		{
			int xres = qRound(72.0 / pageItem->imageXScale());
			int yres = qRound(72.0 / pageItem->imageYScale());
			item->setText(COLUMN_PROBLEM, warnMap[PV_HIGH_DPI].first.arg(maxResDPI).arg(xres).arg(yres));
			item->setToolTip(COLUMN_PROBLEM, warnMap[PV_HIGH_DPI].second);
			item->setIcon(COLUMN_ITEM, onlyWarning );
			break;
		}
		case PartFilledImageFrame:
			item->setText(COLUMN_PROBLEM, warnMap[PV_IMAGE_FRAME_PART_FILLED].first);
			item->setToolTip(COLUMN_PROBLEM, warnMap[PV_IMAGE_FRAME_PART_FILLED].second);
			item->setIcon(COLUMN_ITEM, onlyWarning);
			break;
		case Transparency:
			item->setText(COLUMN_PROBLEM, warnMap[PV_TRANSPARENCY].first);
			item->setToolTip(COLUMN_PROBLEM, warnMap[PV_TRANSPARENCY].second);
			item->setIcon(COLUMN_ITEM, graveError );
			pageGraveError = true;
			itemError = true;
			break;
		case PDFAnnotField:
			item->setText(COLUMN_PROBLEM, warnMap[PV_ANNOTATION].second);
			item->setToolTip(COLUMN_PROBLEM, warnMap[PV_ANNOTATION].second);
			item->setIcon(COLUMN_ITEM, onlyWarning );
			break;
		case PlacedPDF:
			item->setText(COLUMN_PROBLEM, warnMap[PV_RASTER_PDF].first);
			item->setToolTip(COLUMN_PROBLEM, warnMap[PV_RASTER_PDF].second);
			item->setIcon(COLUMN_ITEM, onlyWarning );
			break;
		case ImageIsGIF:
			item->setText(COLUMN_PROBLEM, warnMap[PV_IS_GIF].first);
			item->setToolTip(COLUMN_PROBLEM, warnMap[PV_IS_GIF].second);
			item->setIcon(COLUMN_ITEM, onlyWarning);
			break;
		case WrongFontInAnnotation:
			item->setText(COLUMN_PROBLEM, warnMap[PV_WRONG_FONT].first);
			item->setToolTip(COLUMN_PROBLEM, warnMap[PV_WRONG_FONT].second);
			item->setIcon(COLUMN_ITEM, graveError );
			pageGraveError = true;
			itemError = true;
			break;
		case NotCMYKOrSpot:
			item->setText(COLUMN_PROBLEM, warnMap[PV_NOT_CMYK_SPOT].first);
			item->setToolTip(COLUMN_PROBLEM, warnMap[PV_NOT_CMYK_SPOT].second);
			item->setIcon(COLUMN_ITEM, onlyWarning);
			itemError = true;
			break;
		case FontNotEmbedded:
			item->setText(COLUMN_PROBLEM, warnMap[PV_FONT_NOT_EMBEDDED].first);
			item->setToolTip(COLUMN_PROBLEM, warnMap[PV_FONT_NOT_EMBEDDED].second);
			item->setIcon(COLUMN_ITEM, graveError);
			itemError = true;
			break;
		case EmptyTextFrame:
			item->setText(COLUMN_PROBLEM, warnMap[PV_EMPTY_TEXT_FRAME].first);
			item->setToolTip(COLUMN_PROBLEM, warnMap[PV_EMPTY_TEXT_FRAME].second);
			item->setIcon(COLUMN_ITEM, onlyWarning);
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
	if (!doc->hasPreflightErrors()
	    //this flag is used by documentchecker as indicator for marks change after updating
		&& !doc->notesChanged())
	{
		QTreeWidgetItem * documentItem = new QTreeWidgetItem( reportDisplay );
		documentItem->setText(COLUMN_ITEM, tr( "Document" ) );
		documentItem->setIcon(COLUMN_ITEM, noErrors );
		documentItem->setText(COLUMN_PROBLEM, tr( "No Problems found" ) );
		ignoreErrors->setText( tr("OK"));
	}
	else
	{
		bool hasError = false;
		bool layoutGraveError = false;
		itemError = false;


		// MARKS ***********************************************
		if (doc->notesChanged())
		{
			QTreeWidgetItem * marksItem = new QTreeWidgetItem(reportDisplay);
			marksItem->setText(0, tr("After Marks update document was changed"));
			marksItem->setIcon(COLUMN_ITEM, onlyWarning );
			doc->setNotesChanged(false);
		}

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
				QTreeWidgetItem * layer = new QTreeWidgetItem(layerItem);
				for (layerErrorsIt = docLayerErrorsIt.value().begin();
					 layerErrorsIt != docLayerErrorsIt.value().end(); ++layerErrorsIt)
				{
					QTreeWidgetItem * errorText = new QTreeWidgetItem( layer, 0 );
					switch (layerErrorsIt.key())
					{
						case Transparency:
							errorText->setText(COLUMN_ITEM, warnMap[PV_LAYER_TRANSPARENCY].first);
							errorText->setToolTip(COLUMN_ITEM, warnMap[PV_LAYER_TRANSPARENCY].second);
							errorText->setIcon(COLUMN_ITEM, graveError );
							layoutGraveError = true;
							break;
						case BlendMode:
							errorText->setText(COLUMN_ITEM, warnMap[PV_LAYER_BLENDMODE].first);
							errorText->setToolTip(COLUMN_ITEM, warnMap[PV_LAYER_BLENDMODE].second);
							errorText->setIcon(COLUMN_ITEM, graveError );
							layoutGraveError = true;
							break;
						case OffConflictLayers:
							errorText->setText(COLUMN_ITEM, warnMap[PV_LAYER_PRINTVIS_MISMATCH].first);
							errorText->setToolTip(COLUMN_ITEM, warnMap[PV_LAYER_PRINTVIS_MISMATCH].second);
							errorText->setIcon(COLUMN_ITEM, onlyWarning );
							break;
						default:
							break;
					}
				}
				layer->setText(COLUMN_ITEM,tr("Layer \"%1\"").arg(doc->layerName(docLayerErrorsIt.key())));
				if (layoutGraveError)
					layer->setIcon(COLUMN_ITEM, graveError );
				else
					layer->setIcon(COLUMN_ITEM, onlyWarning );
				layer->setText(COLUMN_PROBLEM, tr("Issues: %1").arg(doc->docLayerErrors[docLayerErrorsIt.key()].count()));
				layer->setExpanded(true);
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
			hasError = false;
			pageGraveError = false;
			QTreeWidgetItem * page=NULL;
			if (showPagesWithoutErrors)
			{
				page = new QTreeWidgetItem( masterPageRootItem);
				masterPageMap.insert(page, doc->MasterPages.at(mPage));
			}

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
						masterPageMap.insert(page, doc->MasterPages.at(mPage));
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
						posMap.insert(object, it3.value());
					}
					else
					{
						for (it3 = masterItemErrorsIt.value().begin(); it3 != masterItemErrorsIt.value().end(); ++it3)
						{
							QTreeWidgetItem * errorText = new QTreeWidgetItem( object, 0 );
							buildItem(errorText, it3.key(), masterItemErrorsIt.key());
							masterPageItemMap.insert(errorText, masterItemErrorsIt.key());
							posMap.insert(object, it3.value());
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
		masterPageRootItem->setText(COLUMN_PROBLEM, tr("Issues: %1").arg(mpErrorCount));
		// END of MASTER PAGES

		// PAGES ********************************8
		for (int aPage = 0; aPage < doc->DocPages.count(); ++aPage)
		{
			int pageErrorCount=0;
			QString tmp;
			hasError = false;
			pageGraveError = false;
			QTreeWidgetItem * page=NULL;
			if (showPagesWithoutErrors)
			{
				page = new QTreeWidgetItem( reportDisplay);
				pageMap.insert(page, doc->DocPages.at(aPage));
			}

			QMap<int, errorCodes>::Iterator pageErrorsIt;
			for (pageErrorsIt = doc->pageErrors.begin();
				 pageErrorsIt != doc->pageErrors.end();
				 ++pageErrorsIt)
			{
				if (pageErrorsIt.key() == aPage)
				{
					if (page==NULL)
					{
						page = new QTreeWidgetItem( reportDisplay);
						pageMap.insert(page, doc->DocPages.at(aPage));
					}
					QTreeWidgetItem * errorText = new QTreeWidgetItem(page);
					errorText->setText(COLUMN_PROBLEM, warnMap[PV_APPLIED_MASTER_DIFF_SIDE].first);
					errorText->setToolTip(COLUMN_PROBLEM, warnMap[PV_APPLIED_MASTER_DIFF_SIDE].second);
					errorText->setIcon(COLUMN_ITEM, onlyWarning );
					pageMap.insert(errorText, doc->DocPages.at(aPage));
					hasError=true;
					page->setExpanded( true );
					++pageErrorCount;
				}
			}

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
						pageMap.insert(page, doc->DocPages.at(aPage));
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
						posMap.insert(object, it3.value());
						++pageErrorCount;
					}
					else
					{
						for (it3 = docItemErrorsIt.value().begin(); it3 != docItemErrorsIt.value().end(); ++it3)
						{
							QTreeWidgetItem * errorText = new QTreeWidgetItem( object);
							buildItem(errorText, it3.key(), docItemErrorsIt.key());
							itemMap.insert(errorText, docItemErrorsIt.key());
							posMap.insert(object, it3.value());
							++pageErrorCount;
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
				page->setText(COLUMN_PROBLEM, tr( "Issues: %1" ).arg(pageErrorCount) );
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

		// FREE ITEMS **************************************************
		QMap<PageItem*, errorCodes>::Iterator freeItemsErrorsIt;
		bool hasfreeItems = false;
		for (freeItemsErrorsIt = doc->docItemErrors.begin(); freeItemsErrorsIt != doc->docItemErrors.end(); ++freeItemsErrorsIt)
		{
			if (doc->OnPage(freeItemsErrorsIt.key()) == -1)
			{
				hasfreeItems = true;
				break;
			}
		}
		if (hasfreeItems)
		{
			bool hasError = false;
			bool pageGraveError = false;
			QTreeWidgetItem * freeItem = new QTreeWidgetItem( reportDisplay);
			for (freeItemsErrorsIt = doc->docItemErrors.begin(); freeItemsErrorsIt != doc->docItemErrors.end(); ++freeItemsErrorsIt)
			{
				if (doc->OnPage(freeItemsErrorsIt.key()) == -1)
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
						posMap.insert(object, it3.value());
					}
					else
					{
						for (it3 = freeItemsErrorsIt.value().begin(); it3 != freeItemsErrorsIt.value().end(); ++it3)
						{
							QTreeWidgetItem * errorText = new QTreeWidgetItem( object);
							buildItem(errorText, it3.key(), freeItemsErrorsIt.key());
							itemMap.insert(errorText, freeItemsErrorsIt.key());
							posMap.insert(object, it3.value());
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

		ignoreErrors->setText( tr("&Ignore Errors"));
	}

	reportDisplay->resizeColumnToContents(COLUMN_ITEM);
	reportDisplay->resizeColumnToContents(COLUMN_PROBLEM);
	reportDisplay->resizeColumnToContents(COLUMN_LAYER);
	connect(curCheckProfile, SIGNAL(activated(const QString&)), this, SLOT(newScan(const QString&)));
	connect(reportDisplay, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(slotSelect(QTreeWidgetItem*)));
}

void CheckDocument::setIgnoreEnabled(bool state)
{
	noButton = !state;
	ignoreErrors->setVisible(state);
}

bool CheckDocument::isIgnoreEnabled()
{
	return !noButton;
}
