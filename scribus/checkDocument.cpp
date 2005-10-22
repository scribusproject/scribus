#include "checkDocument.h"
#include "checkDocument.moc"

#include <qvariant.h>
#include <qheader.h>
#include <qlistview.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qmap.h>
#include <qlabel.h>

#include "sccombobox.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "page.h"
#include "documentchecker.h"

extern QPixmap loadIcon(QString nam);


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

CheckDocument::CheckDocument( QWidget* parent, bool modal )  : ScrPaletteBase( parent, "checkDocument", modal, 0 )
{
	QImage img;
	img.loadFromData( image0_data, sizeof( image0_data ), "PNG" );
	graveError = img;
	img.loadFromData( image1_data, sizeof( image1_data ), "PNG" );
	onlyWarning = img;
	img.loadFromData( image2_data, sizeof( image2_data ), "PNG" );
	noErrors = img;
	setIcon(loadIcon("AppIcon.png"));
	checkDocumentLayout = new QVBoxLayout( this, 10, 5, "checkDocumentLayout");

	layout1 = new QHBoxLayout( 0, 0, 5, "layout1");
	textLabel1 = new QLabel( this, "textLabel1" );
	layout1->addWidget( textLabel1 );
	curCheckProfile = new ScComboBox( false, this, "Profiles" );
	layout1->addWidget( curCheckProfile );
	checkDocumentLayout->addLayout( layout1 );
	reportDisplay = new QListView( this, "reportDisplay" );
	reportDisplay->addColumn("Items");
	reportDisplay->header()->setClickEnabled( false, reportDisplay->header()->count() - 1 );
	reportDisplay->header()->setResizeEnabled( false, reportDisplay->header()->count() - 1 );
	reportDisplay->addColumn("Problems");
	reportDisplay->header()->setClickEnabled( false, reportDisplay->header()->count() - 1 );
	reportDisplay->header()->setResizeEnabled( false, reportDisplay->header()->count() - 1 );
	reportDisplay->setSorting(-1);
	checkDocumentLayout->addWidget( reportDisplay );
	layout2 = new QHBoxLayout( 0, 0, 5, "layou2");
	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout2->addItem( spacer );
	ignoreErrors = new QPushButton(this, "ignoreErrors" );
	layout2->addWidget( ignoreErrors );
	checkDocumentLayout->addLayout( layout2 );
	ignoreErrors->hide();
	noButton = true;
	checkMode = 0;
	languageChange();
	itemMap.clear();
	pageMap.clear();
	masterPageMap.clear();
	masterPageItemMap.clear();
	resize( QSize(306, 259).expandedTo(minimumSizeHint()) );
	clearWState( WState_Polished );
	connect(ignoreErrors, SIGNAL(clicked()), this, SIGNAL(ignoreAllErrors()));
	connect(curCheckProfile, SIGNAL(activated(const QString&)), this, SLOT(newScan(const QString&)));
}
/*
void CheckDocument::closeEvent(QCloseEvent *ce)
{
	emit closePal(false);
	QDialog::closeEvent(ce);
}

void CheckDocument::keyPressEvent(QKeyEvent *ke)
{
	QDialog::keyPressEvent(ke);
}
*/
void CheckDocument::slotSelect(QListViewItem* ite)
{
	if (itemMap.contains(ite))
	{
		ScApp->closeActiveWindowMasterPageEditor();
		emit selectElement(document->DocItems.at(itemMap[ite])->OwnPage, itemMap[ite]);
		return;
	}
	if (pageMap.contains(ite))
	{
		ScApp->closeActiveWindowMasterPageEditor();
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
		if (!document->masterPageMode)
			emit selectMasterPage(document->MasterItems.at(masterPageItemMap[ite])->OnMasterPage);
		emit selectElement(-1, masterPageItemMap[ite]);
		return;
	}
}

void CheckDocument::newScan(const QString& name)
{
	clearErrorList();
	document->curCheckProfile = name;
	DocumentChecker docChecker;
	docChecker.checkDocument(document);
	buildErrorList(document);
}

void CheckDocument::clearErrorList()
{
	disconnect(reportDisplay, SIGNAL(selectionChanged(QListViewItem*)), this, SLOT(slotSelect(QListViewItem*)));
	reportDisplay->clear();
	itemMap.clear();
	pageMap.clear();
	masterPageMap.clear();
	masterPageItemMap.clear();
}

void CheckDocument::buildErrorList(ScribusDoc *doc)
{
	document = doc;
	disconnect(curCheckProfile, SIGNAL(activated(const QString&)), this, SLOT(newScan(const QString&)));
	curCheckProfile->clear();
	CheckerPrefsList::Iterator it;
	CheckerPrefsList::Iterator itend=doc->checkerProfiles.end();
	for (it = doc->checkerProfiles.begin(); it != itend ; ++it)
		curCheckProfile->insertItem(it.key());
	curCheckProfile->setCurrentText(doc->curCheckProfile);
	connect(curCheckProfile, SIGNAL(activated(const QString&)), this, SLOT(newScan(const QString&)));
	QString missingGlyph = tr("Glyphs missing");
	QString textOverflow = tr("Text overflow");
	QString notOnPage = tr("Object is not on a Page");
	QString missingImg = tr("Missing Image");
	QString lowDPI = tr("Image has a DPI-Value less than %1 DPI").arg(qRound(doc->checkerProfiles[doc->curCheckProfile].minResolution));
	QString transpar = tr("Object has transparency");
	QString annot = tr("Object is a PDF Annotation or Field");
	QString rasterPDF = tr("Object is a placed PDF");
	reportDisplay->clear();
	reportDisplay->setSorting(-1);
	itemMap.clear();
	pageMap.clear();
	QListViewItem * item = new QListViewItem( reportDisplay, 0 );
	item->setText( 0, tr( "Document" ) );
	if ((doc->docItemErrors.count() == 0) && (doc->masterItemErrors.count() == 0))
	{
		item->setPixmap( 0, noErrors );
		item->setText( 1, tr( "No Problems found" ) );
	}
	else
	{
		bool hasError = false;
		bool hasGraveError = false;
		QListViewItem * pagep = 0;
		for (int a = 0; a < static_cast<int>(doc->MasterPages.count()); ++a)
		{
			QString tmp;
			hasError = false;
			bool pageGraveError = false;
			QListViewItem * page = new QListViewItem( item, pagep );
			masterPageMap.insert(page, doc->MasterPages.at(a)->PageNam);
			pagep = page;
			QMap<int, errorCodes>::Iterator it2;
			for (it2 = doc->masterItemErrors.begin(); it2 != doc->masterItemErrors.end(); ++it2)
			{
				if ((doc->MasterItems.at(it2.key())->OwnPage == a) || (doc->MasterItems.at(it2.key())->OnMasterPage == doc->MasterPages.at(a)->PageNam))
				{
					hasError = true;
					bool itemError = false;
					QListViewItem * object = new QListViewItem( page, 0 );
					masterPageItemMap.insert(object, doc->MasterItems.at(it2.key())->ItemNr);
					object->setText(0, doc->MasterItems.at(it2.key())->itemName());
					errorCodes::Iterator it3;
					if (it2.data().count() == 1)
					{
						it3 = it2.data().begin();
						switch (it3.key())
						{
						case 1:
							object->setText(1, missingGlyph);
							hasGraveError = true;
							pageGraveError = true;
							itemError = true;
							break;
						case 2:
							object->setText(1, textOverflow);
							break;
						case 3:
							object->setText(1, notOnPage);
							break;
						case 4:
							object->setText(1, missingImg);
							hasGraveError = true;
							pageGraveError = true;
							itemError = true;
							break;
						case 5:
							object->setText(1, lowDPI);
							break;
						case 6:
							object->setText(1, transpar);
							hasGraveError = true;
							pageGraveError = true;
							itemError = true;
							break;
						case 7:
							object->setText(1, annot);
							break;
						case 8:
							object->setText(1, rasterPDF);
							break;
						}
					}
					else
					{
						for (it3 = it2.data().begin(); it3 != it2.data().end(); ++it3)
						{
							QListViewItem * errorText = new QListViewItem( object, 0 );
							switch (it3.key())
							{
							case 1:
								errorText->setText(1, missingGlyph);
								errorText->setPixmap( 0, graveError );
								hasGraveError = true;
								pageGraveError = true;
								itemError = true;
								break;
							case 2:
								errorText->setText(1, textOverflow);
								errorText->setPixmap( 0, onlyWarning );
								break;
							case 3:
								errorText->setText(1, notOnPage);
								errorText->setPixmap( 0, onlyWarning );
								break;
							case 4:
								errorText->setText(1, missingImg);
								errorText->setPixmap( 0, graveError );
								hasGraveError = true;
								pageGraveError = true;
								itemError = true;
								break;
							case 5:
								errorText->setText(1, lowDPI);
								errorText->setPixmap( 0, onlyWarning );
								break;
							case 6:
								errorText->setText(1, transpar);
								errorText->setPixmap( 0, graveError );
								hasGraveError = true;
								pageGraveError = true;
								itemError = true;
								break;
							case 7:
								errorText->setText(1, annot);
								errorText->setPixmap( 0, onlyWarning );
								break;
							case 8:
								errorText->setText(1, rasterPDF);
								errorText->setPixmap( 0, onlyWarning );
								break;
							}
						}
						object->setOpen( true );
					}
					if (itemError)
						object->setPixmap( 0, graveError );
					else
						object->setPixmap( 0, onlyWarning );
				}
			}
			if (hasError)
			{
				if (pageGraveError)
					page->setPixmap( 0, graveError );
				else
					page->setPixmap( 0, onlyWarning );
				page->setOpen( true );
			}
			else
				page->setPixmap( 0, noErrors );
			page->setText(0, doc->MasterPages.at(a)->PageNam);
		}
		for (int a = 0; a < static_cast<int>(doc->DocPages.count()); ++a)
		{
			QString tmp;
			hasError = false;
			bool pageGraveError = false;
			QListViewItem * page = new QListViewItem( item, pagep );
			pageMap.insert(page, a);
			pagep = page;
			QMap<int, errorCodes>::Iterator it2;
			for (it2 = doc->docItemErrors.begin(); it2 != doc->docItemErrors.end(); ++it2)
			{
				if (doc->DocItems.at(it2.key())->OwnPage == a)
				{
					hasError = true;
					bool itemError = false;
					QListViewItem * object = new QListViewItem( page, 0 );
					object->setText(0, doc->DocItems.at(it2.key())->itemName());
					itemMap.insert(object, doc->DocItems.at(it2.key())->ItemNr);
					errorCodes::Iterator it3;
					if (it2.data().count() == 1)
					{
						it3 = it2.data().begin();
						switch (it3.key())
						{
						case 1:
							object->setText(1, missingGlyph);
							hasGraveError = true;
							pageGraveError = true;
							itemError = true;
							break;
						case 2:
							object->setText(1, textOverflow);
							break;
						case 3:
							object->setText(1, notOnPage);
							break;
						case 4:
							object->setText(1, missingImg);
							hasGraveError = true;
							pageGraveError = true;
							itemError = true;
							break;
						case 5:
							object->setText(1, lowDPI);
							break;
						case 6:
							object->setText(1, transpar);
							hasGraveError = true;
							pageGraveError = true;
							itemError = true;
							break;
						case 7:
							object->setText(1, annot);
							break;
						case 8:
							object->setText(1, rasterPDF);
							break;
						}
					}
					else
					{
						for (it3 = it2.data().begin(); it3 != it2.data().end(); ++it3)
						{
							QListViewItem * errorText = new QListViewItem( object, 0 );
							switch (it3.key())
							{
							case 1:
								errorText->setText(1, missingGlyph);
								errorText->setPixmap( 0, graveError );
								hasGraveError = true;
								pageGraveError = true;
								itemError = true;
								break;
							case 2:
								errorText->setText(1, textOverflow);
								errorText->setPixmap( 0, onlyWarning );
								break;
							case 3:
								errorText->setText(1, notOnPage);
								errorText->setPixmap( 0, onlyWarning );
								break;
							case 4:
								errorText->setText(1, missingImg);
								errorText->setPixmap( 0, graveError );
								hasGraveError = true;
								pageGraveError = true;
								itemError = true;
								break;
							case 5:
								errorText->setText(1, lowDPI);
								errorText->setPixmap( 0, onlyWarning );
								break;
							case 6:
								errorText->setText(1, transpar);
								errorText->setPixmap( 0, graveError );
								hasGraveError = true;
								pageGraveError = true;
								itemError = true;
								break;
							case 7:
								errorText->setText(1, annot);
								errorText->setPixmap( 0, onlyWarning );
								break;
							case 8:
								errorText->setText(1, rasterPDF);
								errorText->setPixmap( 0, onlyWarning );
								break;
							}
						}
						object->setOpen( true );
					}
					if (itemError)
						object->setPixmap( 0, graveError );
					else
						object->setPixmap( 0, onlyWarning );
				}
			}
			if (hasError)
			{
				if (pageGraveError)
					page->setPixmap( 0, graveError );
				else
					page->setPixmap( 0, onlyWarning );
				page->setOpen( true );
			}
			else
				page->setPixmap( 0, noErrors );
			page->setText(0, tr("Page ")+tmp.setNum(a+1));
		}
		QMap<int, errorCodes>::Iterator it2;
		bool hasfreeItems = false;
		for (it2 = doc->docItemErrors.begin(); it2 != doc->docItemErrors.end(); ++it2)
		{
			if (doc->DocItems.at(it2.key())->OwnPage == -1)
			{
				hasfreeItems = true;
				break;
			}
		}
		if (hasfreeItems)
		{
			bool hasError = false;
			bool pageGraveError = false;
			QListViewItem * page = new QListViewItem( item, pagep );
			pagep = page;
			for (it2 = doc->docItemErrors.begin(); it2 != doc->docItemErrors.end(); ++it2)
			{
				if (doc->DocItems.at(it2.key())->OwnPage == -1)
				{
					hasError = true;
					QListViewItem * object = new QListViewItem( page, 0 );
					object->setText(0, doc->DocItems.at(it2.key())->itemName());
					itemMap.insert(object, doc->DocItems.at(it2.key())->ItemNr);
					errorCodes::Iterator it3;
					if (it2.data().count() == 1)
					{
						it3 = it2.data().begin();
						switch (it3.key())
						{
						case 1:
							object->setText(1, missingGlyph);
							hasGraveError = true;
							pageGraveError = true;
							break;
						case 2:
							object->setText(1, textOverflow);
							break;
						case 3:
							object->setText(1, notOnPage);
							break;
						case 4:
							object->setText(1, missingImg);
							hasGraveError = true;
							pageGraveError = true;
							break;
						case 5:
							object->setText(1, lowDPI);
							break;
						case 6:
							object->setText(1, transpar);
							hasGraveError = true;
							pageGraveError = true;
							break;
						case 7:
							object->setText(1, annot);
							break;
						case 8:
							object->setText(1, rasterPDF);
							break;
						}
					}
					else
					{
						for (it3 = it2.data().begin(); it3 != it2.data().end(); ++it3)
						{
							QListViewItem * errorText = new QListViewItem( object, 0 );
							switch (it3.key())
							{
							case 1:
								errorText->setText(1, missingGlyph);
								errorText->setPixmap( 0, graveError );
								hasGraveError = true;
								pageGraveError = true;
								break;
							case 2:
								errorText->setText(1, textOverflow);
								errorText->setPixmap( 0, onlyWarning );
								break;
							case 3:
								errorText->setText(1, notOnPage);
								errorText->setPixmap( 0, onlyWarning );
								break;
							case 4:
								errorText->setText(1, missingImg);
								errorText->setPixmap( 0, graveError );
								hasGraveError = true;
								pageGraveError = true;
								break;
							case 5:
								errorText->setText(1, lowDPI);
								errorText->setPixmap( 0, onlyWarning );
								break;
							case 6:
								errorText->setText(1, transpar);
								errorText->setPixmap( 0, graveError );
								hasGraveError = true;
								pageGraveError = true;
								break;
							case 7:
								errorText->setText(1, annot);
								errorText->setPixmap( 0, onlyWarning );
								break;
							case 8:
								errorText->setText(1, rasterPDF);
								errorText->setPixmap( 0, onlyWarning );
								break;
							}
						}
						object->setOpen( true );
					}
					if (pageGraveError)
						object->setPixmap( 0, graveError );
					else
						object->setPixmap( 0, onlyWarning );
				}
			}
			if (hasError)
			{
				if (pageGraveError)
					page->setPixmap( 0, graveError );
				else
					page->setPixmap( 0, onlyWarning );
				page->setOpen( true );
			}
			else
				page->setPixmap( 0, noErrors );
			page->setText(0, tr("Free Objects"));
		}
		if (hasGraveError)
			item->setPixmap( 0, graveError );
		else
			item->setPixmap( 0, onlyWarning );
		item->setText( 1, tr( "Problems found" ) );
		item->setOpen( true );
	}
	if (noButton)
		ignoreErrors->hide();
	else
		ignoreErrors->show();
	connect(reportDisplay, SIGNAL(selectionChanged(QListViewItem*)), this, SLOT(slotSelect(QListViewItem*)));
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void CheckDocument::languageChange()
{
	setCaption( tr( "Preflight Verifier" ) );
	reportDisplay->header()->setLabel( 0, tr( "Items" ) );
	reportDisplay->header()->setLabel( 1, tr( "Problems" ) );
	
	textLabel1->setText( tr("Current Profile:"));
	ignoreErrors->setText( tr( "&Ignore Errors" ));
}

