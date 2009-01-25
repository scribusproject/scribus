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
#include "documentchecker.h"
#include "util_icon.h"
#include "util.h"


// readable constants for QTreeWidgetItem column ids
#define COLUMN_ITEM 0
#define COLUMN_PROBLEM 1
#define COLUMN_LAYER 2
// #define COLUMN_INFO 3


static const unsigned char image0_data[] =
    {
        0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d,
        0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x0b,
        0x08, 0x06, 0x00, 0x00, 0x00, 0xa9, 0xac, 0x77, 0x26, 0x00, 0x00, 0x00,
        0xff, 0x49, 0x44, 0x41, 0x54, 0x78, 0x9c, 0x75, 0x91, 0x31, 0x4e, 0xc3,
        0x40, 0x10, 0x45, 0x9f, 0xad, 0x2d, 0xe6, 0x0a, 0xdc, 0xc2, 0x8e, 0x44,
        0x91, 0x83, 0x50, 0x51, 0x91, 0x58, 0x29, 0x42, 0xba, 0x80, 0x28, 0x10,
        0x55, 0xe4, 0x02, 0x45, 0x74, 0x18, 0x24, 0x64, 0xdc, 0xe5, 0x28, 0x31,
        0x92, 0xd1, 0xe2, 0x74, 0x5c, 0x83, 0x72, 0x0a, 0x4b, 0x43, 0xe1, 0x78,
        0xb1, 0x22, 0xb1, 0xcd, 0x6a, 0x67, 0xde, 0xfc, 0x9d, 0x99, 0x1f, 0xf9,
        0xc6, 0x43, 0x07, 0x38, 0xd0, 0x4e, 0x11, 0x27, 0xff, 0xbe, 0xdd, 0x10,
        0xd0, 0x4e, 0x59, 0x2d, 0xdf, 0x8d, 0xd1, 0x29, 0xdf, 0xae, 0xa3, 0x71,
        0xde, 0x89, 0x13, 0x8a, 0x57, 0xec, 0xbb, 0xdd, 0x51, 0x1e, 0x2a, 0x00,
        0x14, 0x10, 0x20, 0x5b, 0x60, 0xd3, 0xf3, 0x35, 0x97, 0x57, 0x3f, 0x91,
        0x38, 0x21, 0xd6, 0x4e, 0x69, 0xdb, 0x82, 0xfa, 0xe2, 0x8c, 0x49, 0xbe,
        0x0d, 0xa0, 0x02, 0xe5, 0xa1, 0xe2, 0xe3, 0xf3, 0x29, 0x28, 0xc7, 0x37,
        0xcb, 0x9d, 0x95, 0x87, 0x0a, 0x5c, 0x8f, 0xc8, 0xb1, 0x85, 0x71, 0x41,
        0xb6, 0x28, 0x4c, 0x9c, 0xe0, 0x14, 0x05, 0xa0, 0xbe, 0xbb, 0x0d, 0xc0,
        0xe9, 0x0d, 0xfd, 0xb0, 0x31, 0x48, 0x48, 0x70, 0x02, 0x4e, 0xf2, 0xcd,
        0x5f, 0xdc, 0x09, 0xf1, 0xf0, 0x75, 0x50, 0x18, 0x81, 0x83, 0xd0, 0xa0,
        0x8c, 0x6f, 0x3c, 0x69, 0x32, 0x33, 0x0f, 0xb6, 0x07, 0xf3, 0x60, 0x96,
        0x6f, 0xcc, 0x1e, 0xb7, 0xb6, 0x07, 0x9b, 0x26, 0x4b, 0xf3, 0xb5, 0xc7,
        0x37, 0xbe, 0xdf, 0x73, 0x9a, 0xae, 0xc9, 0x8e, 0xc3, 0x28, 0xf0, 0x75,
        0xff, 0x80, 0x02, 0xab, 0x64, 0x46, 0x9a, 0xce, 0x83, 0x41, 0x91, 0x6f,
        0x7c, 0x70, 0x2a, 0x5b, 0x14, 0x36, 0x74, 0x2d, 0x08, 0xcf, 0x2f, 0xf3,
        0x68, 0xec, 0xe4, 0x2f, 0x90, 0x07, 0x8c, 0x36, 0xce, 0xc2, 0x57, 0x47,
        0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4e, 0x44, 0xae, 0x42, 0x60, 0x82
    };

static const unsigned char image1_data[] =
    {
        0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d,
        0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x0b,
        0x08, 0x06, 0x00, 0x00, 0x00, 0xa9, 0xac, 0x77, 0x26, 0x00, 0x00, 0x00,
        0xfa, 0x49, 0x44, 0x41, 0x54, 0x78, 0x9c, 0x75, 0x91, 0x31, 0x6e, 0x83,
        0x40, 0x10, 0x45, 0x1f, 0x68, 0x8b, 0xb9, 0x4a, 0x3a, 0x63, 0xc9, 0x05,
        0x25, 0xe7, 0x48, 0x15, 0x8c, 0x28, 0x88, 0x3b, 0xe7, 0x1a, 0xe9, 0x02,
        0x91, 0x2c, 0x42, 0xe7, 0x73, 0x50, 0x19, 0x24, 0xac, 0x35, 0xa5, 0xaf,
        0x91, 0x72, 0x0b, 0xa4, 0x49, 0x41, 0xb2, 0x46, 0x48, 0x99, 0x66, 0x35,
        0x33, 0x7f, 0xdf, 0xd7, 0xee, 0x0f, 0xec, 0x60, 0x61, 0x02, 0x0c, 0xb8,
        0xc9, 0x21, 0x46, 0xfe, 0xed, 0xcd, 0xdf, 0xc0, 0x4d, 0x8e, 0x43, 0xf1,
        0xa5, 0x2c, 0xaa, 0x3e, 0xbd, 0x06, 0xcb, 0xbd, 0x11, 0x23, 0x94, 0x9f,
        0xe8, 0x7d, 0x3c, 0x53, 0x57, 0xcd, 0x2f, 0x11, 0xc4, 0x40, 0x96, 0xa3,
        0xf1, 0xee, 0xc8, 0xf3, 0xcb, 0x77, 0x20, 0x46, 0x08, 0xdd, 0xe4, 0x18,
        0xc7, 0x92, 0x6e, 0xbc, 0xb3, 0x8d, 0x13, 0x2f, 0x74, 0x13, 0xd4, 0xa7,
        0x86, 0xfe, 0xfa, 0xee, 0xc9, 0xe1, 0x5b, 0x71, 0xd6, 0xba, 0x6a, 0xbc,
        0xb5, 0x18, 0x60, 0x75, 0x21, 0xcb, 0x4b, 0x15, 0x23, 0x18, 0x87, 0x03,
        0x03, 0x5d, 0xdf, 0x7a, 0xc1, 0xfa, 0x84, 0xf9, 0xb1, 0x21, 0x88, 0x5f,
        0xb0, 0x12, 0x6e, 0x77, 0xc9, 0x3c, 0x07, 0xc4, 0x08, 0x21, 0xb8, 0x07,
        0x61, 0x25, 0x9c, 0x89, 0x0f, 0x32, 0x76, 0xb0, 0x44, 0x9b, 0x54, 0xed,
        0x80, 0x5e, 0x3a, 0xd4, 0x0e, 0xa8, 0x6a, 0xa2, 0xaa, 0x89, 0x5e, 0x3a,
        0x34, 0xde, 0x14, 0x6a, 0x3b, 0x8b, 0x1d, 0xec, 0xfc, 0x1b, 0x51, 0x74,
        0x24, 0xcb, 0x53, 0xef, 0x70, 0xbb, 0xb6, 0x74, 0x7d, 0xcb, 0xa1, 0x48,
        0x79, 0x8a, 0xf6, 0x3e, 0xa0, 0xc0, 0x0e, 0xd6, 0x27, 0x95, 0xe5, 0xa5,
        0x82, 0x00, 0x0e, 0x41, 0xf8, 0xa8, 0xf6, 0xc1, 0x32, 0xc9, 0x1f, 0x9e,
        0x1c, 0x9d, 0xca, 0x9e, 0x4f, 0x38, 0x5a, 0x00, 0x00, 0x00, 0x00, 0x49,
        0x45, 0x4e, 0x44, 0xae, 0x42, 0x60, 0x82
    };

static const unsigned char image2_data[] =
    {
        0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d,
        0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x0b,
        0x08, 0x06, 0x00, 0x00, 0x00, 0xa9, 0xac, 0x77, 0x26, 0x00, 0x00, 0x01,
        0x02, 0x49, 0x44, 0x41, 0x54, 0x78, 0x9c, 0x75, 0x91, 0xb1, 0x4a, 0xc4,
        0x40, 0x10, 0x86, 0xbf, 0x84, 0x29, 0xc6, 0x37, 0xb8, 0xd6, 0xf2, 0xca,
        0x10, 0xae, 0xc8, 0x2b, 0xf8, 0x0a, 0xa9, 0x84, 0xa4, 0x08, 0xd8, 0x1c,
        0xa4, 0xb4, 0x13, 0xbb, 0x80, 0x85, 0x4a, 0xe0, 0x12, 0xb8, 0x46, 0x4b,
        0x5b, 0x3b, 0xeb, 0xc0, 0x49, 0xd8, 0xd2, 0x57, 0x48, 0x69, 0x21, 0xb8,
        0xc5, 0x42, 0x2c, 0x72, 0xc9, 0xad, 0x88, 0xdb, 0x2c, 0x33, 0x3b, 0xf3,
        0xfd, 0x3f, 0xfb, 0x07, 0xfd, 0xa1, 0x07, 0x07, 0x08, 0x58, 0x67, 0x51,
        0xd1, 0x7f, 0x6b, 0x99, 0x1b, 0xd6, 0x59, 0xae, 0x9b, 0xab, 0x11, 0xef,
        0x54, 0x45, 0x1b, 0xf8, 0xef, 0xa2, 0xa2, 0x74, 0xf2, 0x38, 0xbe, 0x36,
        0x1f, 0x0c, 0xb9, 0x01, 0x01, 0x1c, 0xa8, 0x40, 0x59, 0x67, 0x63, 0xba,
        0x4d, 0x58, 0x7f, 0xa5, 0x81, 0x8a, 0x12, 0x5a, 0x67, 0x79, 0xb9, 0x37,
        0x3c, 0xed, 0x2c, 0x7d, 0x12, 0x79, 0x16, 0x60, 0x28, 0x0c, 0xcf, 0x77,
        0xdd, 0x42, 0x0e, 0x6f, 0xf7, 0xe5, 0x38, 0xe4, 0x06, 0x3d, 0x4a, 0xeb,
        0x91, 0x3c, 0x2b, 0x7c, 0x16, 0x86, 0xb2, 0xce, 0x46, 0x15, 0x45, 0xbe,
        0x9d, 0x05, 0x81, 0xb8, 0x3b, 0x59, 0xf0, 0x6f, 0xeb, 0x26, 0x88, 0x75,
        0x96, 0xd0, 0xf7, 0xe8, 0x0f, 0xaa, 0x40, 0xbf, 0x89, 0xa6, 0x1a, 0x50,
        0x51, 0xc2, 0x5f, 0x04, 0x6f, 0xe1, 0x6d, 0x13, 0x4d, 0x53, 0x3e, 0xb9,
        0x2a, 0xda, 0x60, 0x55, 0x47, 0x7f, 0x88, 0xca, 0x64, 0xed, 0x7c, 0x9f,
        0x50, 0xe5, 0xed, 0xe9, 0x37, 0xd2, 0x6d, 0xc4, 0xbc, 0x60, 0x1d, 0xc4,
        0xef, 0x86, 0xb8, 0x33, 0xac, 0x9a, 0x88, 0x8b, 0x62, 0xbd, 0x04, 0x14,
        0xf4, 0x87, 0x7e, 0x49, 0xaa, 0xac, 0xb3, 0x71, 0x56, 0x38, 0x13, 0xe5,
        0xe6, 0xf2, 0x21, 0xf0, 0x93, 0xfc, 0x01, 0x78, 0x2a, 0x84, 0x98, 0x28,
        0xe8, 0xa9, 0x58, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4e, 0x44, 0xae,
        0x42, 0x60, 0x82
    };

CheckDocument::CheckDocument( QWidget* parent, bool modal )
	: ScrPaletteBase( parent, "checkDocument", modal, 0 ),
	m_Doc(0),
// 	globalGraveError(false),
	pageGraveError(false),
	itemError(false),
	minResDPI(0),
	maxResDPI(0)
{
	QPixmap img;
	img.loadFromData( image0_data, sizeof( image0_data ), "PNG" );
	graveError = img;
	img.loadFromData( image1_data, sizeof( image1_data ), "PNG" );
	onlyWarning = img;
	img.loadFromData( image2_data, sizeof( image2_data ), "PNG" );
	noErrors = img;
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
// 	reportDisplay->setColumnCount(2);
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
		emit selectElement(m_Doc->DocItems.at(itemMap[ite])->OwnPage, itemMap[ite]);
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
			emit selectMasterPage(m_Doc->MasterItems.at(masterPageItemMap[ite])->OnMasterPage);
		emit selectElement(-1, masterPageItemMap[ite]);
		return;
	}
}

void CheckDocument::doReScan()
{
	newScan(curCheckProfile->currentText());
}

void CheckDocument::newScan(const QString& name)
{
	clearErrorList();
	if (m_Doc==0)
		return;
	m_Doc->curCheckProfile = name;
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
	item->setText(COLUMN_LAYER, m_Doc->Layers.at(pageItem->LayerNr).Name);
	item->setData(COLUMN_LAYER, Qt::DecorationRole, m_Doc->Layers.at(pageItem->LayerNr).markerColor);
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
	disconnect(curCheckProfile, SIGNAL(activated(const QString&)),
	           this, SLOT(newScan(const QString&)));
	curCheckProfile->clear();
	clearErrorList();

	if (m_Doc==0)
		return;

	minResDPI = qRound(doc->checkerProfiles[doc->curCheckProfile].minResolution);
	maxResDPI = qRound(doc->checkerProfiles[doc->curCheckProfile].maxResolution);
	
	CheckerPrefsList::Iterator it;
	CheckerPrefsList::Iterator itend=doc->checkerProfiles.end();
	for (it = doc->checkerProfiles.begin(); it != itend ; ++it)
		curCheckProfile->addItem(it.key());
	setCurrentComboItem(curCheckProfile, doc->curCheckProfile);

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
							break;
						case BlendMode:
							errorText->setText(COLUMN_ITEM, tr("Blendmode used"));
							break;
						default:
							break;
					}
					errorText->setIcon(COLUMN_ITEM, graveError );
				}
				layer->setText(COLUMN_ITEM,tr("Layer \"%1\"").arg(doc->layerName(docLayerErrorsIt.key())));
				layer->setIcon(COLUMN_ITEM, graveError );
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
			bool pageGraveError = false;
			QTreeWidgetItem * page = new QTreeWidgetItem( masterPageRootItem);//, pagep );
			masterPageMap.insert(page, doc->MasterPages.at(mPage)->pageName());
// 			pagep = page;
			QMap<int, errorCodes>::Iterator masterItemErrorsIt;
			for (masterItemErrorsIt = doc->masterItemErrors.begin();
				 masterItemErrorsIt != doc->masterItemErrors.end();
				 ++masterItemErrorsIt)
			{
				if ((doc->MasterItems.at(masterItemErrorsIt.key())->OwnPage == mPage)
					|| (doc->MasterItems.at(masterItemErrorsIt.key())->OnMasterPage == doc->MasterPages.at(mPage)->pageName()))
				{
					hasError = true;
					QTreeWidgetItem * object = new QTreeWidgetItem( page);
					masterPageItemMap.insert(object, doc->MasterItems.at(masterItemErrorsIt.key())->ItemNr);
					object->setText(COLUMN_ITEM, doc->MasterItems.at(masterItemErrorsIt.key())->itemName());
					errorCodes::Iterator it3;
					if (masterItemErrorsIt.value().count() == 1)
					{
						it3 = masterItemErrorsIt.value().begin();
						buildItem(object, it3.key(), doc->MasterItems.at(masterItemErrorsIt.key()));
					}
					else
					{
						for (it3 = masterItemErrorsIt.value().begin(); it3 != masterItemErrorsIt.value().end(); ++it3)
						{
							QTreeWidgetItem * errorText = new QTreeWidgetItem( object, 0 );
							buildItem(errorText, it3.key(), doc->MasterItems.at(masterItemErrorsIt.key()));
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
				page->setIcon(COLUMN_ITEM, noErrors );
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
			bool pageGraveError = false;
			QTreeWidgetItem * page = new QTreeWidgetItem( reportDisplay);//, pagep );
			pageMap.insert(page, aPage);
// 			pagep = page;
			QMap<int, errorCodes>::Iterator docItemErrorsIt;
			for (docItemErrorsIt = doc->docItemErrors.begin();
				 docItemErrorsIt != doc->docItemErrors.end();
				 ++docItemErrorsIt)
			{
				if (doc->DocItems.at(docItemErrorsIt.key())->OwnPage == aPage)
				{
					hasError = true;
					itemError = false;
					QTreeWidgetItem * object = new QTreeWidgetItem(page);
					object->setText(COLUMN_ITEM, doc->DocItems.at(docItemErrorsIt.key())->itemName());
					itemMap.insert(object, doc->DocItems.at(docItemErrorsIt.key())->ItemNr);
					errorCodes::Iterator it3;
					if (docItemErrorsIt.value().count() == 1)
					{
						it3 = docItemErrorsIt.value().begin();
						buildItem(object, it3.key(), doc->DocItems.at(docItemErrorsIt.key()));
					}
					else
					{
						for (it3 = docItemErrorsIt.value().begin(); it3 != docItemErrorsIt.value().end(); ++it3)
						{
							QTreeWidgetItem * errorText = new QTreeWidgetItem( object);
							buildItem(errorText, it3.key(), doc->DocItems.at(docItemErrorsIt.key()));
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
				page->setIcon( 0, noErrors );
			page->setText(COLUMN_ITEM, tr("Page ")+tmp.setNum(aPage+1));
		}
		// END of PAGES

		// FREE ITEMS **********************************************8888
		QMap<int, errorCodes>::Iterator freeItemsErrorsIt;
		bool hasfreeItems = false;
		for (freeItemsErrorsIt = doc->docItemErrors.begin();
			 freeItemsErrorsIt != doc->docItemErrors.end();
			 ++freeItemsErrorsIt)
		{
			if (doc->DocItems.at(freeItemsErrorsIt.key())->OwnPage == -1)
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
				if (doc->DocItems.at(freeItemsErrorsIt.key())->OwnPage == -1)
				{
					hasError = true;
					QTreeWidgetItem * object = new QTreeWidgetItem(freeItem);
					object->setText(0, doc->DocItems.at(freeItemsErrorsIt.key())->itemName());
					itemMap.insert(object, doc->DocItems.at(freeItemsErrorsIt.key())->ItemNr);
					errorCodes::Iterator it3;
					if (freeItemsErrorsIt.value().count() == 1)
					{
						it3 = freeItemsErrorsIt.value().begin();
						buildItem(object, it3.key(), doc->DocItems.at(freeItemsErrorsIt.key()));
					}
					else
					{
						for (it3 = freeItemsErrorsIt.value().begin(); it3 != freeItemsErrorsIt.value().end(); ++it3)
						{
							QTreeWidgetItem * errorText = new QTreeWidgetItem( object);
							buildItem(errorText, it3.key(), doc->DocItems.at(freeItemsErrorsIt.key()));
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
