/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QtGui>
#include <QMessageBox>
#include <iostream>
#include "picturebrowser.h"
#include "collection.h"
#include "findimage.h"
#include "previewimage.h"
#include "loadimage.h"

#include "prefsfile.h"
#include "prefsmanager.h"

//provides loadIcon()
#include "util_icon.h"

PictureBrowser::PictureBrowser ( ScribusDoc* doc, QWidget *parent ) : QDialog ( parent )
{
	setupUi ( this );

//set Scribusdoc
	srcDoc = doc;
	ScMW = doc->scMW();

//load settings
	pbSettings.load();


	connect ( navigationBox, SIGNAL ( currentIndexChanged ( int ) ), this, SLOT ( navigate ( int ) ) );
	connect ( sortCombobox, SIGNAL ( currentIndexChanged ( int ) ), this, SLOT ( sortChanged ( int ) ) );
	connect ( previewModeCombobox, SIGNAL ( currentIndexChanged ( int ) ), this, SLOT ( previewModeChanged ( int ) ) );
	connect ( moreButton, SIGNAL ( clicked() ), this, SLOT ( moreButtonClicked() ) );
	connect ( resetSettingsButton, SIGNAL ( clicked() ), this, SLOT ( resetSettingsButtonClicked() ) );
	connect ( sortOrderButton, SIGNAL ( clicked() ), this, SLOT ( sortOrderButtonClicked() ) );
	connect ( actionsGoButton, SIGNAL ( clicked() ), this, SLOT ( actionsGoButtonClicked() ) );

	connect ( zoomPlusButton, SIGNAL ( clicked() ), this, SLOT ( zoomPlusButtonClicked() ) );
	connect ( zoomMinusButton, SIGNAL ( clicked() ), this, SLOT ( zoomMinusButtonClicked() ) );

//close button
	connect ( closeButton, SIGNAL ( clicked() ), this, SLOT ( accept() ) );

	connect ( tabWidget, SIGNAL ( currentChanged ( int ) ), this, SLOT ( tabWidgetCurrentChanged ( int ) ) );

//"goto page" button in documentbrowser
	connect ( gotoPageButton, SIGNAL ( clicked() ), this, SLOT ( gotoPageButtonClicked() ) );

	connect ( folderView, SIGNAL ( activated ( const QModelIndex & ) ), this, SLOT ( dirChosen ( const QModelIndex & ) ) );
	connect ( folderView, SIGNAL ( clicked ( const QModelIndex & ) ), this, SLOT ( dirChosen ( const QModelIndex & ) ) );
	connect ( folderView, SIGNAL ( doubleClicked ( const QModelIndex & ) ), this, SLOT ( dirChosen ( const QModelIndex & ) ) );

//changing page
	connect ( this, SIGNAL ( selectPage ( int ) ), ScCore->primaryMainWindow(), SLOT ( selectPagesFromOutlines ( int ) ) );
	connect ( this, SIGNAL ( selectMasterPage ( QString ) ), ScCore->primaryMainWindow(), SLOT ( manageMasterPages ( QString ) ) );

//always-on-top checkbox
	connect ( alwaysOnTopCheckbox, SIGNAL ( stateChanged ( int ) ), this, SLOT ( alwaysOnTopCheckboxStateChanged() ) );
//save settings checkbox
	connect ( saveSettingsCheckbox, SIGNAL ( stateChanged ( int ) ), this, SLOT ( saveSettingsCheckboxStateChanged() ) );

//folderbrowser:include subdirs
	connect ( subdirsCheckbox, SIGNAL ( stateChanged ( int ) ), this, SLOT ( subdirsCheckboxStateChanged() ) );

	loadIcons();

	setSettings();

	navigationStack->setCurrentIndex ( 0 );


//maybe add QDir::Readable, although users might wonder where their folders are
	folderModel.setFilter ( QDir::AllDirs|QDir::Drives|QDir::NoDotAndDotDot|QDir::NoSymLinks/*|QDir::Hidden*/ );
	folderView->setModel ( &folderModel );

// this should give a little performance boost
	folderView->setUniformRowHeights ( true );
	folderView->setCurrentIndex ( folderModel.index ( QDir::currentPath() ) );
	folderView->scrollTo ( folderView->currentIndex(), QAbstractItemView::PositionAtTop );
	folderView->resizeColumnToContents ( 0 );

//this needs to be calculated later from height/width of QListView!
	previewIconsVisible = 8;
	folderBrowserIncludeSubdirs = false;

	imagesDisplayed = 0;
	imagesFiltered = 0;
	previewIconIndex = -1;
	insertCustomPosition = false;
	insertCustomSize = false;
	currentRow = 0;

//set up previewimages instance, fill with last shown images later
	pImages = new previewImages ( QStringList() );

//set threadpointer to 0
	fit = 0;
	crt = 0;
	cdbwt = 0;
	clrt = 0;


	pModel = new PreviewImagesModel ( this );

//create loadImagesThread instance, connect and run it
	lit = new loadImagesThread ( this, pModel );
	lit->start();

	connect ( imageViewArea, SIGNAL ( clicked ( const QModelIndex & ) ), this, SLOT ( previewIconClicked ( const QModelIndex & ) ) );
	connect ( imageViewArea, SIGNAL ( doubleClicked ( const QModelIndex & ) ), this, SLOT ( previewIconDoubleClicked ( const QModelIndex & ) ) );

	imageViewArea->SetIconSize ( QSize ( pbSettings.previewIconSize, pbSettings.previewIconSize ) );
	imageViewArea->SetGridSize ( QSize ( qRound ( ( 1.1 ) *pbSettings.previewIconSize ), ( qRound ( ( 1.1 ) *pbSettings.previewIconSize ) + pbSettings.previewMode*10 ) ) );
	imageViewArea->SetModel ( pModel );

//register item selections
	connect ( imageViewArea->SelectionModel(), SIGNAL ( selectionChanged ( const QItemSelection &, const QItemSelection & ) ), this, SLOT ( previewImageSelectionChanged ( const QItemSelection &, const QItemSelection & ) ) );

//documentbrowser setup
	connect ( documentWidget, SIGNAL ( itemActivated ( QTreeWidgetItem * , int ) ), this, SLOT ( documentChosen ( QTreeWidgetItem * , int ) ) );
	connect ( documentWidget, SIGNAL ( itemClicked ( QTreeWidgetItem * , int ) ), this, SLOT ( documentChosen ( QTreeWidgetItem * , int ) ) );

	documentWidget->setColumnCount ( 1 );
	documentWidget->setHeaderLabels ( QStringList ( "Pages" ) );


//collectionsbrowser setup
	connect ( collectionsWidget, SIGNAL ( itemActivated ( QTreeWidgetItem * , int ) ), this, SLOT ( collectionChosen ( QTreeWidgetItem * , int ) ) );
	connect ( collectionsWidget, SIGNAL ( itemClicked ( QTreeWidgetItem * , int ) ), this, SLOT ( collectionChosen ( QTreeWidgetItem * , int ) ) );
	connect ( collectionsWidget, SIGNAL ( itemChanged ( QTreeWidgetItem * , int ) ), this, SLOT ( collectionsWidgetItemEdited ( QTreeWidgetItem * , int ) ) );
	connect ( collectionsNewCategoryButton, SIGNAL ( clicked() ), this, SLOT ( collectionsNewCategoryButtonClicked() ) );
	connect ( collectionsNewButton, SIGNAL ( clicked() ), this, SLOT ( collectionsNewButtonClicked() ) );
	connect ( collectionsImportButton, SIGNAL ( clicked() ), this, SLOT ( collectionsImportButtonClicked() ) );
	connect ( collectionsExportButton, SIGNAL ( clicked() ), this, SLOT ( collectionsExportButtonClicked() ) );
	connect ( collectionsDeleteButton, SIGNAL ( clicked() ), this, SLOT ( collectionsDeleteButtonClicked() ) );
	connect ( collectionsRenameButton, SIGNAL ( clicked() ), this, SLOT ( collectionsRenameButtonClicked() ) );

	connect ( collectionsAddImagesButton, SIGNAL ( clicked() ), this, SLOT ( collectionsAddImagesButtonClicked() ) );
	connect ( collectionsRemoveImagesButton, SIGNAL ( clicked() ), this, SLOT ( collectionsRemoveImagesButtonClicked() ) );
	connect ( collectionsTagImagesButton, SIGNAL ( clicked() ), this, SLOT ( collectionsTagImagesButtonClicked() ) );
	connect ( collectionsAddImagesOkButton, SIGNAL ( clicked() ), this, SLOT ( collectionsAddImagesOkButtonClicked() ) );
	connect ( collectionsAddImagesCancelButton, SIGNAL ( clicked() ), this, SLOT ( collectionsAddImagesCancelButtonClicked() ) );

	connect ( collectionsSetTagsButton, SIGNAL ( clicked() ), this, SLOT ( collectionsSetTagsButtonClicked() ) );
	connect ( collectionsAddNewTagButton, SIGNAL ( clicked() ), this, SLOT ( collectionsAddNewTagButtonClicked() ) );


	collectionsWidget->setColumnCount ( 1 );
	collectionsWidget->setHeaderLabels ( QStringList ( "Name" ) );

	cdbFile = ScPaths::instance().getPluginDataDir() + QString ( "collectionsdb.xml" );
//emtpy initialization, maybe accessed before collection is actually loaded
	currCollection = new imageCollection;
	currCollection->name = "empty";
	currCollection->file = "empty";

	crt = new collectionReaderThread ( cdbFile, false );
	connect ( crt, SIGNAL ( finished() ), this, SLOT ( collectionReaderThreadFinished() ) );
	crt->start();

	connect ( insertPositionCombobox, SIGNAL ( currentIndexChanged ( int ) ), this, SLOT ( insertPositionComboboxChanged ( int ) ) );
	connect ( insertSizeCombobox, SIGNAL ( currentIndexChanged ( int ) ), this, SLOT ( insertSizeComboboxChanged ( int ) ) );
	connect ( insertImageButton, SIGNAL ( clicked() ), this, SLOT ( insertImageButtonClicked() ) );
	connect ( insertPagesCombobox, SIGNAL ( checkstateChanged ( int ) ), this, SLOT ( insertPagesComboboxCheckstateChanged ( int ) ) );
	insertPagesCombobox->addItem ( "Current Page", 1 );
	insertPagesCombobox->addItem ( "All Pages", 0 );

	for ( int i = 0 ; i < ( int ) ( srcDoc->Pages->count() ) ; ++i )
	{
		insertPagesCombobox->addItem ( QString ( "Page %1" ).arg ( i+1 ), 0 );
	}

	insertFramesCombobox->addItem ( "All Frames", 0 );
	insertFramesCombobox->addItem ( "All Empty Frames", 0 );


	PageItem *pItem;

	for ( int i = 0; i < srcDoc->MasterItems.count(); ++i )
	{
		pItem = srcDoc->MasterItems.at ( i );

		if ( ( pItem->itemType() == PageItem::ImageFrame ) && ( !pItem->asLatexFrame() ) )
		{
			QString itemText;

			if ( pItem->PictureIsAvailable )
			{
				itemText = QString ( "Frame %1" ).arg ( i+1 );
			}
			else
			{
				itemText = QString ( "Frame %1 (empty)" ).arg ( i+1 );
			}

			insertFramesCombobox->addItem ( itemText, 0 );
		}
	}

	for ( int i = 0; i < srcDoc->Items->count(); ++i )
	{
		pItem = srcDoc->Items->at ( i );

		if ( ( pItem->itemType() == PageItem::ImageFrame ) && ( !pItem->asLatexFrame() ) )
		{
			QString itemText;

			if ( pItem->PictureIsAvailable )
			{
				itemText = QString ( "Frame %1" ).arg ( i+1 );
			}
			else
			{
				itemText = QString ( "Frame %1 (empty)" ).arg ( i+1 );
			}

			insertFramesCombobox->addItem ( itemText, 0 );
		}
	}


//set namefilters for global use
	nameFilters << "*.jpg" << "*.jpeg" << "*.gif" << "*.png" << "*.ps" << "*.psd" << "*.tif" << "*.tiff" << "*.xpm" << "*.eps" << "*.epsf" << "*.epsi" << "*.eps2" << "*.eps3" << "*.epi" << "*.epg";


//filter/search setup
	connect ( filterTargetCombobox, SIGNAL ( currentIndexChanged ( int ) ), this, SLOT ( filterTargetComboboxChanged ( int ) ) );
	connect ( filterCriteriaCombobox, SIGNAL ( currentIndexChanged ( int ) ), this, SLOT ( filterCriteriaComboboxChanged ( int ) ) );
	connect ( filterFilterButton, SIGNAL ( clicked() ), this, SLOT ( filterFilterButtonClicked() ) );
	connect ( filterClearButton, SIGNAL ( clicked() ), this, SLOT ( filterClearButtonClicked() ) );
	connect ( filterAddCriterionButton, SIGNAL ( clicked() ), this, SLOT ( filterAddCriterionButtonClicked() ) );
	connect ( filterSearchDirButton, SIGNAL ( clicked() ), this, SLOT ( filterSearchDirButtonClicked() ) );

	filters = new imageFilters;


	filterTypeCombobox->addItem ( "All Supported Formats", 1 );

	for ( int i = 0 ; i < nameFilters.size() ; ++i )
	{
		filterTypeCombobox->addItem ( nameFilters.at ( i ), 1 );
	}

//fix: scrollperitem doesnt work
//imageViewArea->setLayoutMode(QListView::Batched);
//imageViewArea->setBatchSize(4);
//imageViewArea->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
//imageViewArea->setHorizontalScrollMode(QAbstractItemView::ScrollPerItem);
}


PictureBrowser::~PictureBrowser()
{
}


void PictureBrowser::callLoadImageThread ( int row, int pId )
{
	previewImage *imageToLoad = pModel->modelItemsList.at ( row );

	emit loadImageJob ( row, imageToLoad->fileInformation.absoluteFilePath(), pbSettings.previewIconSize, pId );
}


void PictureBrowser::navigate ( int index )
{
	//this check shouldnt be necessary but you never know...
	if ( ( index >= 0 ) && ( index < 3 ) )
	{
		if ( index == 2 )
		{
			updateDocumentbrowser();
		}

		navigationStack->setCurrentIndex ( index );
	}
}


void PictureBrowser::previewIconClicked ( const QModelIndex &index )
{
	int row = index.row();

	if ( row >= 0 )
	{
		previewIconIndex = row;
		updateInformationTab ( previewIconIndex );
	}
}


void PictureBrowser::previewIconDoubleClicked ( const QModelIndex &index )
{
	int row = index.row();

	if ( row >= 0 )
	{
		ScribusDoc *currentDoc ( ScMW->doc );
		Imagedialog *id ( new Imagedialog ( pImages->previewImagesList.at ( row )->fileInformation.absoluteFilePath(), currentDoc ,this ) );

		if ( id )
		{
			id->setAttribute ( Qt::WA_DeleteOnClose );
			id->show();
			id->raise();
			id->activateWindow();
		}
	}
}


void PictureBrowser::sortChanged ( int index )
{
	//this check shouldnt be necessary but you never know...
	if ( ( index >= 0 ) && ( index < 4 ) )
	{
		pbSettings.sortSetting = index;

		if ( saveSettingsCheckbox->isChecked() )
		{
			pbSettings.save();
		}

		updateBrowser ( false, true, false );
	}
}


void PictureBrowser::previewModeChanged ( int index )
{
	if ( ( index >= 0 ) && ( index < 2 ) )
	{
		pbSettings.previewMode = index;
		imageViewArea->SetGridSize ( QSize ( qRound ( ( 1.1 ) *pbSettings.previewIconSize ), ( qRound ( ( 1.1 ) *pbSettings.previewIconSize ) + pbSettings.previewMode*10 ) ) );

		if ( saveSettingsCheckbox->isChecked() )
		{
			pbSettings.save();
		}
	}
}


// enlarge dialog to show more information/options
//FIXME: still quick and dirty, professional animation required
void PictureBrowser::moreButtonClicked()
{
	// dialog is not enlarged -> enlarge, change buttontext/icon
	if ( !pbSettings.showMore )
	{
		expandDialog ( true );
		pbSettings.showMore = true;

		if ( tabWidget->currentIndex() == 0 )
		{
			updateInformationTab ( previewIconIndex );
		}
	}
	// dialog is enlarged, set back to original size, change buttontext/icon
	else
	{
		expandDialog ( false );
		pbSettings.showMore = false;
	}

	if ( saveSettingsCheckbox->isChecked() )
	{
		pbSettings.save();
	}
}


void PictureBrowser::resetSettingsButtonClicked()
{
	pbSettings.reset();
	setSettings();
}


void PictureBrowser::actionsGoButtonClicked()
{
	int index = actionsCombobox->currentIndex();

	if ( ( previewIconIndex < 0 ) || ( previewIconIndex > pImages->previewImagesList.size() ) )
	{
		QMessageBox::warning ( this, tr ( "Picture Browser Error" ), tr ( "No Image(s) Selected" ) );
		return;
	}

	if ( index == 0 )
	{
		previewImage *tmpImage;

		//image from pModel needed here
		tmpImage = pModel->modelItemsList.at ( previewIconIndex );
		InsertAFrameData iafData;

		iafData.frameType=PageItem::ImageFrame;
		iafData.source=tmpImage->fileInformation.absoluteFilePath();
		iafData.locationType = 0;
		iafData.pageList = QString ( "" );
		iafData.positionType = 0;
		iafData.sizeType = 0;
		iafData.x = 0;
		iafData.y = 0;
		iafData.width = 0;
		iafData.height = 0;
		//iafData.impsetup=m_ImportSetup;
		iafData.columnCount = 0;
		iafData.columnGap = 0;
		iafData.linkTextFrames = false;
		iafData.linkToExistingFrame = false;
		iafData.linkToExistingFramePtr=NULL;

		tmpImage->insertIntoDocument ( srcDoc, iafData );
	}
	else if ( index == 1 )
	{
		if ( !pbSettings.showMore )
		{
			expandDialog ( true );
		}

		if ( tabWidget->currentIndex() != 3 )
		{
			tabWidget->setCurrentIndex ( 3 );
		}
	}
	else if ( index == 2 )
	{
		navigationBox->setCurrentIndex ( 1 );
		collectionsStackedwidget->setCurrentIndex ( 1 );
		updateCollectionsWidget ( true );
		collectionsWidget->blockSignals ( true );
	}
}


void PictureBrowser::sortOrderButtonClicked()
{
	if ( !pbSettings.sortOrder )
	{
		pbSettings.sortOrder = true;
		sortOrderButton->setIcon ( *iconArrowUp );
	}
	else
	{
		pbSettings.sortOrder = false;
		sortOrderButton->setIcon ( *iconArrowDown );
	}

	if ( saveSettingsCheckbox->isChecked() )
	{
		pbSettings.save();
	}

	updateBrowser ( false, false, false );
}


void PictureBrowser::zoomPlusButtonClicked()
{
	if ( pbSettings.previewIconSize < 500 )
	{
		pbSettings.previewIconSize += 20;

		imageViewArea->SetIconSize ( QSize ( pbSettings.previewIconSize, pbSettings.previewIconSize ) );
		imageViewArea->SetGridSize ( QSize ( qRound ( ( 1.1 ) *pbSettings.previewIconSize ), ( qRound ( ( 1.1 ) *pbSettings.previewIconSize ) + pbSettings.previewMode*10 ) ) );

		pModel->createDefaultIcon ( pbSettings.previewIconSize );

		if ( saveSettingsCheckbox->isChecked() )
		{
			pbSettings.save();
		}

		updateBrowser ( false, false, true );
	}
}


void PictureBrowser::zoomMinusButtonClicked()
{
	if ( pbSettings.previewIconSize > 50 )
	{
		pbSettings.previewIconSize -= 20;

		imageViewArea->SetIconSize ( QSize ( pbSettings.previewIconSize, pbSettings.previewIconSize ) );
		imageViewArea->SetGridSize ( QSize ( qRound ( ( 1.1 ) *pbSettings.previewIconSize ), ( qRound ( ( 1.1 ) *pbSettings.previewIconSize ) + pbSettings.previewMode*10 ) ) );

		pModel->createDefaultIcon ( pbSettings.previewIconSize );

		if ( saveSettingsCheckbox->isChecked() )
		{
			pbSettings.save();
		}

		updateBrowser ( false, false, true );
	}
}


void PictureBrowser::tabWidgetCurrentChanged ( int index )
{
	if ( index == 0 )
	{
		updateInformationTab ( previewIconIndex );
	}
}


void PictureBrowser::gotoPageButtonClicked()
{
	QTreeWidgetItem *item = documentWidget->currentItem();

	if ( !item )
	{
		return;
	}

	int id = item->data ( 0, Qt::UserRole ).toInt();

	if ( id > 0 )
	{
		id--;
	}

	ScCore->primaryMainWindow()->closeActiveWindowMasterPageEditor();
	emit selectPage ( id );
}


void PictureBrowser::alwaysOnTopCheckboxStateChanged()
{
	pbSettings.alwaysOnTop = alwaysOnTopCheckbox->isChecked();

	if ( saveSettingsCheckbox->isChecked() )
	{
		pbSettings.save();
	}

	setAlwaysOnTop ( pbSettings.alwaysOnTop );
}


void PictureBrowser::saveSettingsCheckboxStateChanged()
{
	pbSettings.saveSettings = saveSettingsCheckbox->isChecked();
	pbSettings.save();
}


void PictureBrowser::subdirsCheckboxStateChanged()
{
	folderBrowserIncludeSubdirs = subdirsCheckbox->isChecked();
}


// processes chosen dirs in the folderbrowser
void PictureBrowser::dirChosen ( const QModelIndex &index )
{
	// as long as the folder doesnt change, only the first signal (click, doubleclick or activated) is processed
	if ( !tmpindex.isValid() || ( tmpindex != index ) )
	{
		tmpindex = index;

		currPath = folderModel.filePath ( index );

		if ( !fit )
		{
			fit = new findImagesThread ( currPath, nameFilters, QDir::Name, folderBrowserIncludeSubdirs );
			connect ( fit, SIGNAL ( finished() ), this, SLOT ( findImagesThreadFinished() ) );
			fit->start();
		}
		else
		{
			fit->restart();
		}
	}
}


void PictureBrowser::documentChosen ( QTreeWidgetItem * item, int column )
{
	PageItem *pItem;
	QStringList imageFiles;

	int id = item->data ( 0, Qt::UserRole ).toInt();


	for ( int i = 0; i < srcDoc->MasterItems.count(); ++i )
	{
		pItem = srcDoc->MasterItems.at ( i );

		if ( ( pItem->itemType() == PageItem::ImageFrame ) && ( pItem->PictureIsAvailable ) && ( !pItem->asLatexFrame() ) )
		{
			if ( ( id == 0 ) || ( ( id-1 ) == pItem->OwnPage ) )
			{
				imageFiles.append ( pItem->Pfile );
			}
		}
	}

	for ( int i = 0; i < srcDoc->Items->count(); ++i )
	{
		pItem = srcDoc->Items->at ( i );

		if ( ( pItem->itemType() == PageItem::ImageFrame ) && ( pItem->PictureIsAvailable ) && ( !pItem->asLatexFrame() ) )
		{
			if ( ( id == 0 ) || ( ( id-1 ) == pItem->OwnPage ) )
			{
				imageFiles.append ( pItem->Pfile );
			}
		}
	}

//todo: check if item is selected: item->isSelected()


	pImages->createPreviewImagesList ( imageFiles );

	updateBrowser ( true, true, false );
}


void PictureBrowser::collectionChosen ( QTreeWidgetItem * item, int column )
{
	QString collectionFile = item->data ( 0, Qt::UserRole ).toString();

	if ( collectionFile == "Category" )
	{
		return;
	}

	currCollectionFile = collectionFile;

	if ( !crt )
	{
		crt = new collectionReaderThread ( currCollectionFile, false );
		connect ( crt, SIGNAL ( finished() ), this, SLOT ( collectionReaderThreadFinished() ) );
		crt->start();
	}
	else
	{
		crt->restart();
	}
}


void PictureBrowser::collectionsWidgetItemEdited ( QTreeWidgetItem * item, int column )
{
	saveCollectionsDb();
}


void PictureBrowser::findImagesThreadFinished()
{
	if ( fit->restartThread )
	{
		delete fit;

		//maybe the state of folderBrowserIncludeSubdirs needs to be saved when canceling the old thread
		fit = new findImagesThread ( currPath, nameFilters, QDir::Name, folderBrowserIncludeSubdirs );
		connect ( fit, SIGNAL ( finished() ), this, SLOT ( findImagesThreadFinished() ) );
		fit->start();
	}
	else
	{
		pImages->createPreviewImagesList ( fit->imageFiles );

		updateBrowser ( true, true, false );

		delete fit;
		fit = 0;
	}
}


void PictureBrowser::collectionReaderThreadFinished()
{
	if ( crt->restartThread )
	{
		bool import = crt->import;
		delete crt;

		crt = new collectionReaderThread ( currCollectionFile, import );
		connect ( crt, SIGNAL ( finished() ), this, SLOT ( collectionReaderThreadFinished() ) );
		crt->start();

		return;
	}

	switch ( crt->type )
	{
		case 0:
// 			QMessageBox::warning ( this, tr ( "Picture Browser Error" ), tr ( "File not found or file is not a collection file" ) );
			break;

		case 1:
			collectionsDb += crt->collectionsSet;

			updateCollectionsWidget ( false );
			updateCollectionsAddImagesCombobox();
			break;

		case 2:
			if ( crt->import )
			{
				collectionsWidget->blockSignals ( true );

				QTreeWidgetItem *currItem = collectionsWidget->currentItem();

				if ( !currItem )
				{
					currItem = collectionsWidget->topLevelItem ( 0 );

					if ( !currItem )
					{
						QMessageBox::warning ( this, tr ( "Picture Browser Error" ), tr ( "You have to create a category first" ) );
						return;
					}
				}

				QTreeWidgetItem *tmpItem;

				if ( currItem->parent() )
				{
					tmpItem = new QTreeWidgetItem ( currItem->parent(), QStringList ( crt->collection->name ) );
				}
				else
				{
					tmpItem = new QTreeWidgetItem ( currItem, QStringList ( crt->collection->name ) );
				}

				tmpItem->setFlags ( Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsEnabled );
				tmpItem->setData ( 0, Qt::UserRole, crt->collection->file );
				tmpItem->setIcon ( 0, *iconCollection );
				collectionsWidget->blockSignals ( false );

				collectionsWidget->setCurrentItem ( tmpItem );
				saveCollectionsDb();
			}

			currCollection = crt->collection;
			pImages->createPreviewImagesList ( currCollection );

			//update view
			updateBrowser ( true, true, false );
			updateCollectionsAddImagesCombobox();
			break;

		default:
			break;
	}

	delete crt;
	crt = 0;
}


void PictureBrowser::collectionListReaderThreadFinished()
{
	/*	if(clrt->restartThread)
		{
		delete clrt;

		clrt = new collectionListReaderThread(collections);
		connect(clrt, SIGNAL(finished()), this, SLOT(collectionListReaderThreadFinished()));
		clrt->start();
		}
		else
		{
		pImages->createPreviewImagesList(clrt->imageFiles);

		updateBrowser(true, true, false);

		delete clrt;
		clrt = 0;
		}*/
}


void PictureBrowser::collectionReaderThreadListFinishedSave()
{
	collectionReaderThread *tmpCrt;
	collectionWriterThread *tmpCwt;
	imageCollection *tmpCollection;

	for ( int i = 0 ; i < crtList.size() ; ++i )
	{
		tmpCrt = crtList.at ( i );

		if ( tmpCrt->isFinished() )
		{
			QStringList tmpTags;

			if ( !tmpCrt->type )
			{
				QMessageBox::warning ( this, tr ( "Picture Browser Error" ), QString ( "A collection was not found:\n%1\nit will be created" ).arg ( tmpCrt->xmlFile ) );
				tmpCollection = new imageCollection;
				tmpCollection->imageFiles = tmpCrt->addImages;
			}
			else
			{
				tmpCollection = tmpCrt->collection;
				tmpCollection->imageFiles += tmpCrt->addImages;
			}

			//add empty tags for list consistency
			for ( int j = 0 ; j < tmpCrt->addImages.size() ; ++j )
			{
				tmpCollection->tags.append ( tmpTags );
			}

			tmpCwt = new collectionWriterThread ( tmpCrt->xmlFile, *tmpCollection );
			connect ( tmpCwt, SIGNAL ( finished() ), this, SLOT ( collectionWriterThreadListFinished() ) );
			cwtList.append ( tmpCwt );
			tmpCwt->start();

			delete tmpCollection;
			delete crtList.takeAt ( i );
		}
	}
}


void PictureBrowser::collectionsDbWriterThreadFinished()
{
	if ( cdbwt->restartThread )
	{
		delete cdbwt;

		cdbwt = new collectionsWriterThread ( cdbFile, collectionsDb );
		connect ( cdbwt, SIGNAL ( finished() ), this, SLOT ( collectionsDbWriterThreadFinished() ) );
		cdbwt->start();
	}
	else
	{
		delete cdbwt;
		cdbwt = 0;
	}
}


void PictureBrowser::collectionWriterThreadListFinished()
{
	for ( int i = 0 ; i < cwtList.size() ; ++i )
	{
		if ( cwtList.at ( i )->isFinished() )
		{
			delete cwtList.takeAt ( i );
		}
	}
}


void PictureBrowser::insertPagesComboboxCheckstateChanged ( int row )
{
	int tmpState = insertPagesCombobox->checkstate ( 1 );

	if ( row == 1 )
	{
		int itemsCount = insertPagesCombobox->count();

		for ( int i = 2 ; i < itemsCount ; ++i )
		{
			insertPagesCombobox->setCheckstate ( i, tmpState );
		}
	}
	else if ( ( tmpState == 1 ) && ( row > 1 ) )
	{
		insertPagesCombobox->setCheckstate ( 1, 0 );
	}

	insertPagesCombobox->setCurrentIndex ( row );
}


void PictureBrowser::insertImageButtonClicked()
{
	if ( ( previewIconIndex < 0 ) || ( previewIconIndex > pModel->modelItemsList.size() ) )
	{
		QMessageBox::warning ( this, tr ( "Picture Browser Error" ), tr ( "No image(s) selected" ) );
		return;
	}

	previewImage *tmpImage;

//image from pModel needed here
	tmpImage = pModel->modelItemsList.at ( previewIconIndex );
	InsertAFrameData iafData;

	iafData.frameType=PageItem::ImageFrame;
	iafData.source=tmpImage->fileInformation.absoluteFilePath();

	QString pageList = "";

	if ( insertPagesCombobox->checkstate ( 1 ) == 1 )
	{
		iafData.locationType = 1;
	}
	else
	{
		iafData.locationType = 2;

		//current page has been selected
		if ( insertPagesCombobox->checkstate ( 0 ) == 1 )
		{
			int currPage = srcDoc->currentPageNumber() + 1;

			//prevent double insert, only add current page to pagelist if the page isn't selected yet
			if ( insertPagesCombobox->checkstate ( currPage + 1 ) == 0 )
			{
				pageList += QString ( "%1," ).arg ( currPage );
			}
		}

		for ( int i = 2 ; i < insertPagesCombobox->count() ; ++i )
		{
			if ( insertPagesCombobox->checkstate ( i ) == 1 )
			{
				pageList += QString ( "%1," ).arg ( i-1 );
			}
		}

		if ( pageList.isEmpty() )
		{
			QMessageBox::warning ( this, tr ( "Picture Browser Error" ), tr ( "No page/image frame selected" ) );
			return;
		}

		pageList.chop ( 1 );
	}

	iafData.pageList = pageList;
	iafData.positionType = insertPositionCombobox->currentIndex();
	iafData.sizeType = insertSizeCombobox->currentIndex();
	iafData.x = insertPositionXSpinbox->value();
	iafData.y = insertPositionYSpinbox->value();
	iafData.width = insertWidthSpinbox->value();
	iafData.height = insertHeightSpinbox->value();

	/*dont need all this crap */
//iafData.impsetup=m_ImportSetup;
	iafData.columnCount = 0;
	iafData.columnGap = 0;
	iafData.linkTextFrames = false;
	iafData.linkToExistingFrame = false;
	iafData.linkToExistingFramePtr=NULL;

	tmpImage->insertIntoDocument ( srcDoc, iafData );
}


void PictureBrowser::insertPositionComboboxChanged ( int index )
{
	if ( ( index == 3 ) && !insertCustomPosition )
	{
		insertPositionXSpinbox->setEnabled ( true );
		insertPositionYSpinbox->setEnabled ( true );
		insertPositionXLabel->setEnabled ( true );
		insertPositionYLabel->setEnabled ( true );
		insertCustomPosition = true;
	}
	else if ( insertCustomPosition )
	{
		insertPositionXSpinbox->setEnabled ( false );
		insertPositionYSpinbox->setEnabled ( false );
		insertPositionXLabel->setEnabled ( false );
		insertPositionYLabel->setEnabled ( false );
		insertCustomPosition = false;
	}
}


void PictureBrowser::insertSizeComboboxChanged ( int index )
{
	if ( ( index == 3 ) && !insertCustomSize )
	{
		insertWidthSpinbox->setEnabled ( true );
		insertHeightSpinbox->setEnabled ( true );
		insertWidthLabel->setEnabled ( true );
		insertHeightLabel->setEnabled ( true );
		insertCustomSize = true;
	}
	else if ( insertCustomSize )
	{
		insertWidthSpinbox->setEnabled ( false );
		insertHeightSpinbox->setEnabled ( false );
		insertWidthLabel->setEnabled ( false );
		insertHeightLabel->setEnabled ( false );
		insertCustomSize = false;
	}
}


void PictureBrowser::filterCriteriaComboboxChanged ( int index )
{
	//this check shouldnt be necessary but you never know...
	if ( ( index >= 0 ) && ( index < 5 ) )
	{
		filterStackedwidget->setCurrentIndex ( index );
	}
}


void PictureBrowser::filterTargetComboboxChanged ( int index )
{
	if ( ( index >= 0 ) && ( index < 3 ) )
	{
		filterTargetStackedWidget->setCurrentIndex ( index );
	}

	if ( index == 1 )
	{
		filterFilterButton->setText ( "Search" );
	}
	else
	{
		filterFilterButton->setText ( "Apply Filters" );
	}
}


void PictureBrowser::filterFilterButtonClicked()
{
	if ( filterTargetCombobox->currentIndex() == 1 )
	{
		QString searchDir = filterSearchLineedit->text();
		QDir dir ( searchDir );

		if ( !dir.exists() )
		{
			QMessageBox::warning ( this, tr ( "Picture Browser Error" ), tr ( "Directory does not exist" ) );
			return;
		}

		currPath = searchDir;

		if ( !fit )
		{
			fit = new findImagesThread ( currPath, nameFilters, QDir::Name, true );
			connect ( fit, SIGNAL ( finished() ), this, SLOT ( findImagesThreadFinished() ) );
			fit->start();
		}
		else
		{
			fit->restart();
		}
	}
	else if ( filterTargetCombobox->currentIndex() == 2 )
	{
		/*	if(!clrt)
			{
			clrt = new collectionListReaderThread(collections);
			connect(clrt, SIGNAL(finished()), this, SLOT(collectionListReaderThreadFinished()));
			clrt->start();
			}
			else
			{
			clrt->restart();
			}*/
	}
	else
	{
		updateBrowser ( true, false, false );
	}
}


void PictureBrowser::filterClearButtonClicked()
{
	pImages->clearFilters();
	filterFiltersListwidget->clear();

	delete filters;
	filters = new imageFilters;

	updateBrowser ( true, false, false );
}


void PictureBrowser::filterAddCriterionButtonClicked()
{
	QListWidgetItem *newItem = new QListWidgetItem;

	newItem->setFlags ( Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled );
	newItem->setCheckState ( Qt::Checked );

	int index = filterCriteriaCombobox->currentIndex();
	QString filterText = "Empty Filter";

	if ( index == 0 )
	{
		if ( filterNameCombobox->currentIndex() == 0 )
		{
			filterText = QString ( "Name contains \"%1\"" ).arg ( filterNameLineedit->text() );
			filters->nameInverts.append ( true );
		}
		else
		{
			filterText = QString ( "Name does not contain \"%1\"" ).arg ( filterNameLineedit->text() );
			filters->nameInverts.append ( false );
		}

		filters->nameFilters.append ( filterNameLineedit->text() );
	}
	else if ( index == 1 )
	{
		if ( filterDateCombobox->currentIndex() == 0 )
		{
			filterText = QString ( "Newer than %1" ).arg ( filterDateDatetimeedit->dateTime().toString ( "dd.MM.yyyy hh:mm" ) );
			filters->dateInverts.append ( false );
		}
		else
		{
			filterText = QString ( "Older than %1" ).arg ( filterDateDatetimeedit->dateTime().toString ( "dd.MM.yyyy hh:mm" ) );
			filters->dateInverts.append ( true );
		}

		filters->dateFilters.append ( filterDateDatetimeedit->dateTime() );
	}
	else if ( index == 2 )
	{
		if ( filterSizeCombobox->currentIndex() == 0 )
		{
			filterText = QString ( "Smaller than %1 KB" ).arg ( filterSizeSpinbox->value() );
			filters->sizeInverts.append ( true );
		}
		else
		{
			filterText = QString ( "Bigger than %1 KB" ).arg ( filterSizeSpinbox->value() );
			filters->sizeInverts.append ( false );
		}

		filters->sizeFilters.append ( filterSizeSpinbox->value() );
	}
	else if ( index == 3 )
	{
		QStringList types;
		filterText = QString ( "Allowed types: " );

		if ( filterTypeCombobox->checkstate ( 0 ) == 1 )
		{
			filterText += QString ( "All supported types (really a useful filter...)" );
			types = nameFilters;
		}
		else
		{
			int itemsCount = filterTypeCombobox->count();

			for ( int i = 1 ; i < itemsCount ; ++i )
			{
				if ( filterTypeCombobox->checkstate ( i ) == 1 )
				{
					filterText += QString ( "\"%1\", " ).arg ( nameFilters.at ( i - 1 ) );
					types.append ( nameFilters.at ( i - 1 ) );
				}
			}
		}

		filters->typeFilters.append ( types );
	}
	else if ( index == 4 )
	{
		QStringList tags;
		filterText = QString ( "Has tags: " );

		int itemsCount = filterTagsCombobox->count();

		for ( int i = 1 ; i < itemsCount ; ++i )
		{
			if ( filterTagsCombobox->checkstate ( i ) == 1 )
			{
				filterText += QString ( "\"%1\", " ).arg ( filterTagsCombobox->itemText ( i ) );
				tags.append ( filterTagsCombobox->itemText ( i ) );
			}
		}

		filters->tagFilters.append ( tags );
	}

	filters->filterMap.append ( index );

	newItem->setText ( filterText );
	filterFiltersListwidget->addItem ( newItem );
}


void PictureBrowser::filterSearchDirButtonClicked()
{
	QString searchDir = QFileDialog::getExistingDirectory ( this, tr ( "Select Directory" ), QDir::rootPath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks );
	filterSearchLineedit->setText ( searchDir );
}


void PictureBrowser::collectionsNewCategoryButtonClicked()
{
	collectionsWidget->blockSignals ( true );

	QTreeWidgetItem *tmpCategory;

	tmpCategory = new QTreeWidgetItem ( collectionsWidget, QStringList ( "New Category" ) );
	tmpCategory->setFlags ( Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsEnabled );
	tmpCategory->setData ( 0, Qt::UserRole, QString ( "Category" ) );
	tmpCategory->setExpanded ( true );

	collectionsWidget->blockSignals ( false );

	collectionsWidget->setCurrentItem ( tmpCategory );
	collectionsWidget->editItem ( tmpCategory );

	saveCollectionsDb();
}


void PictureBrowser::collectionsNewButtonClicked()
{
	collectionsWidget->blockSignals ( true );

	QTreeWidgetItem *currItem = collectionsWidget->currentItem();

	if ( !currItem )
	{
		currItem = collectionsWidget->topLevelItem ( 0 );

		if ( !currItem )
		{
			QMessageBox::warning ( this, tr ( "Picture Browser Error" ), tr ( "You have to create a category first" ) );
			return;
		}
	}

	QTreeWidgetItem *parentItem, *tmpItem;
	QString newCollectionFile = ScPaths::instance().getPluginDataDir();

	parentItem = currItem->parent();

	if ( parentItem )
	{
		tmpItem = new QTreeWidgetItem ( parentItem, QStringList ( "New Collection" ) );
		newCollectionFile += QString ( "c%1_%2.sic" ).arg ( collectionsWidget->indexOfTopLevelItem ( parentItem ) ).arg ( parentItem->childCount() );
	}
	else
	{
		tmpItem = new QTreeWidgetItem ( currItem, QStringList ( "New Collection" ) );
		newCollectionFile += QString ( "c%1_%2.sic" ).arg ( collectionsWidget->indexOfTopLevelItem ( currItem ) ).arg ( currItem->childCount() );
	}

	tmpItem->setFlags ( Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsEnabled );
	tmpItem->setData ( 0, Qt::UserRole, newCollectionFile );
	tmpItem->setIcon ( 0, *iconCollection );
	collectionsWidget->blockSignals ( false );

	collectionsWidget->setCurrentItem ( tmpItem );
	collectionsWidget->editItem ( tmpItem );

	saveCollectionsDb();

	imageCollection tmpCollection;
	collectionWriterThread *tmpCwt;
	tmpCwt = new collectionWriterThread ( newCollectionFile, tmpCollection );
	connect ( tmpCwt, SIGNAL ( finished() ), this, SLOT ( collectionWriterThreadListFinished() ) );
	cwtList.append ( tmpCwt );
	tmpCwt->start();
}


void PictureBrowser::collectionsImportButtonClicked()
{
	QString fileName = QFileDialog::getOpenFileName ( this, tr ( "Import Image Collection" ), QDir::rootPath(), tr ( "Scribus ImageCollection (*.sic)" ) );

	if ( !fileName.isEmpty() )
	{
		currCollectionFile = fileName;

		if ( !crt )
		{
			crt = new collectionReaderThread ( currCollectionFile, true );
			connect ( crt, SIGNAL ( finished() ), this, SLOT ( collectionReaderThreadFinished() ) );
			crt->start();
		}
		else
		{
			crt->restart();
		}
	}
}


void PictureBrowser::collectionsExportButtonClicked()
{
	QString fileName = QFileDialog::getSaveFileName ( this, tr ( "Export Image Collection" ), QDir::rootPath(), tr ( "Scribus ImageCollection (*.sic)" ) );

	QTreeWidgetItem *currItem = collectionsWidget->currentItem();

	if ( !currItem )
	{
		QMessageBox::warning ( this, tr ( "Picture Browser Error" ), tr ( "You have to select something you want to export" ) );
		return;
	}

	//collection
	if ( currItem->parent() )
	{
		collectionWriterThread *tmpCwt;
		tmpCwt = new collectionWriterThread ( fileName, *currCollection );
		connect ( tmpCwt, SIGNAL ( finished() ), this, SLOT ( collectionWriterThreadListFinished() ) );
		cwtList.append ( tmpCwt );
		tmpCwt->start();
	}
	//category
	else
	{

	}
}


void PictureBrowser::collectionsDeleteButtonClicked()
{
	QTreeWidgetItem *currItem = collectionsWidget->currentItem();

	if ( !currItem )
	{
		return;
	}

	delete currItem;

	saveCollectionsDb();
}


void PictureBrowser::collectionsRenameButtonClicked()
{
	QTreeWidgetItem *currItem = collectionsWidget->currentItem();

	if ( !currItem )
	{
		return;
	}

	collectionsWidget->editItem ( currItem );
}


void PictureBrowser::collectionsAddImagesButtonClicked()
{
	collectionsStackedwidget->setCurrentIndex ( 1 );
	updateCollectionsWidget ( true );
	collectionsWidget->blockSignals ( true );
}


void PictureBrowser::collectionsSetTagsButtonClicked()
{
	for ( int i = 0 ; i < collectionsTagImagesCombobox->count() ; ++i )
	{
		if ( collectionsTagImagesCombobox->checkstate ( i ) == 0 )
		{
			for ( int j = 0 ; j < selectedIndexes.size() ; ++j )
			{
				pImages->previewImagesList.at ( selectedIndexes.at ( j ) )->tags.removeAll ( collectionsTagImagesCombobox->itemText ( i ) );
			}
		}
		else if ( collectionsTagImagesCombobox->checkstate ( i ) == 1 )
		{
			for ( int j = 0 ; j < selectedIndexes.size() ; ++j )
			{
				if ( !pImages->previewImagesList.at ( selectedIndexes.at ( j ) )->tags.contains ( collectionsTagImagesCombobox->itemText ( i ) ) )
				{
					pImages->previewImagesList.at ( selectedIndexes.at ( j ) )->tags.append ( collectionsTagImagesCombobox->itemText ( i ) );
				}
			}
		}
	}


	collectionWriterThread *tmpCwt;

	currCollection->imageFiles.clear();
	currCollection->tags.clear();

	for ( int i = 0 ; i < pImages->previewImagesList.size() ; ++i )
	{
		currCollection->imageFiles.append ( pImages->previewImagesList.at ( i )->fileInformation.absoluteFilePath() );
		currCollection->tags.append ( pImages->previewImagesList.at ( i )->tags );
	}

	tmpCwt = new collectionWriterThread ( currCollection->file, *currCollection );
	connect ( tmpCwt, SIGNAL ( finished() ), this, SLOT ( collectionWriterThreadListFinished() ) );
	cwtList.append ( tmpCwt );
	tmpCwt->start();
}


void PictureBrowser::collectionsAddNewTagButtonClicked()
{
	QString newTag = collectionsAddNewTagLineedit->text();

	if ( !newTag.isEmpty() )
	{
		collectionsTagImagesCombobox->addItem ( newTag, 1 );
	}
	else
	{
		QMessageBox::warning ( this, tr ( "Picture Browser Error" ), tr ( "No tag entered" ) );
	}
}


void PictureBrowser::collectionsRemoveImagesButtonClicked()
{
	collectionWriterThread *tmpCwt;
	QList<previewImage *> tmpPreviewImagesList;

	for ( int i = 0 ; i < selectedIndexes.size() ; ++i )
	{
		tmpPreviewImagesList.append ( pImages->previewImagesList.takeAt ( selectedIndexes.at ( i ) ) );
	}

//update view
	updateBrowser ( false, false, false );

	for ( int i = 0 ; i < tmpPreviewImagesList.size() ; ++i )
	{
		delete tmpPreviewImagesList.at ( i );
	}

	currCollection->imageFiles.clear();
	currCollection->tags.clear();

	for ( int i = 0 ; i < pImages->previewImagesList.size() ; ++i )
	{
		currCollection->imageFiles.append ( pImages->previewImagesList.at ( i )->fileInformation.absoluteFilePath() );
		currCollection->tags.append ( pImages->previewImagesList.at ( i )->tags );
	}

	tmpCwt = new collectionWriterThread ( currCollection->file, *currCollection );
	connect ( tmpCwt, SIGNAL ( finished() ), this, SLOT ( collectionWriterThreadListFinished() ) );
	cwtList.append ( tmpCwt );
	tmpCwt->start();
}


void PictureBrowser::collectionsTagImagesButtonClicked()
{
}


void PictureBrowser::collectionsAddImagesOkButtonClicked()
{
	QTreeWidgetItem *tmpItem, *tmpItem2;
	QString collectionFile;
	collectionReaderThread *tmpCrt;

	QStringList addImages;

	for ( int k = 0 ; k < selectedIndexes.size() ; ++k )
	{
		addImages.append ( pModel->modelItemsList.at ( selectedIndexes.at ( k ) )->fileInformation.absoluteFilePath() );
	}


	for ( int i = 0 ; i < collectionsWidget->topLevelItemCount() ; ++i )
	{
		tmpItem = collectionsWidget->topLevelItem ( i );

		for ( int j = 0 ; j < tmpItem->childCount() ; ++j )
		{
			tmpItem2 = tmpItem->child ( j );

			if ( tmpItem2->checkState ( 0 ) == Qt::Checked )
			{
				//add images here
				collectionFile = tmpItem2->data ( 0, Qt::UserRole ).toString();

				tmpCrt = new collectionReaderThread ( collectionFile, false );
				tmpCrt->addImages = addImages;
				connect ( tmpCrt, SIGNAL ( finished() ), this, SLOT ( collectionReaderThreadListFinishedSave() ) );
				crtList.append ( tmpCrt );
				tmpCrt->start();
			}
		}
	}

	collectionsStackedwidget->setCurrentIndex ( 0 );
	updateCollectionsWidget ( false );
	collectionsWidget->blockSignals ( false );
}


void PictureBrowser::collectionsAddImagesCancelButtonClicked()
{
	collectionsStackedwidget->setCurrentIndex ( 0 );
	updateCollectionsWidget ( false );
	collectionsWidget->blockSignals ( false );
}


void PictureBrowser::updateBrowser ( bool filter, bool sort, bool reload )
{
	if ( filter )
	{
		applyFilters();
	}

	if ( sort )
	{
		pImages->sortPreviewImages ( pbSettings.sortSetting );
	}

	if ( reload )
	{
		previewImage *tmpImage;

		//force reload, check if needed is necessary
		for ( int i = 0 ; i < pImages->previewImagesList.size() ; ++i )
		{
			tmpImage = pImages->previewImagesList.at ( i );
			tmpImage->previewImageLoading = false;
			tmpImage->previewIconCreated = false;
		}
	}

	pModel->setModelItemsList ( pImages->previewImagesList );
	statusLabel->setText ( QString ( "%1 image(s) displayed, %2 image(s) filtered" ).arg ( imagesDisplayed ).arg ( imagesFiltered ) );
}


void PictureBrowser::loadIcons()
{
	iconArrowUp = new QIcon ( loadIcon ( "pitr_green_arrows_set_1.svg" ) );
	iconArrowDown = new QIcon ( loadIcon ( "pitr_green_arrows_set_5.svg" ) );
	iconFolderBrowser = new QIcon ( loadIcon ( "sarxos_Simple_Folder_Seek.svg" ) );
	iconCollectionsBrowser = new QIcon ( loadIcon ( "sarxos_Simple_Green_Yellow_Blue_Violet_Folders.svg" ) );
	iconDocumentBrowser = new QIcon ( loadIcon ( "sarxos_Paper_Sheets.svg" ) );
	iconDocument = new QIcon ( loadIcon ( "doc.png" ) );
	iconCollection = new QIcon ( loadIcon ( "cmyk.png" ) );
	iconOk = new QIcon ( loadIcon ( "ok22.png" ) );
	iconClose = new QIcon ( loadIcon ( "/16/close.png" ) );
	iconZoomPlus = new QIcon ( loadIcon ( "viewmagin.png" ) );
	iconZoomMinus = new QIcon ( loadIcon ( "viewmagout.png" ) );
	iconNew = new QIcon ( loadIcon ( "/16/insert-table.png" ) );
	iconNew2 = new QIcon ( loadIcon ( "/16/document-new.png" ) );
	iconEdit = new QIcon ( loadIcon ( "/16/document-properties.png" ) );
	iconRemove = new QIcon ( loadIcon ( "/16/edit-cut.png" ) );
	iconLoad = new QIcon ( loadIcon ( "/16/document-open.png" ) );
	iconSave = new QIcon ( loadIcon ( "/16/document-save.png" ) );
	iconPlus = new QIcon ( loadIcon ( "/16/list-add.png" ) );
	iconMinus = new QIcon ( loadIcon ( "/16/list-remove.png" ) );
	iconPen = new QIcon ( loadIcon ( "/16/story-editor.png" ) );

	collectionsNewCategoryButton->setIcon ( *iconNew );
	collectionsNewButton->setIcon ( *iconNew2 );
	collectionsRenameButton->setIcon ( *iconEdit );
	collectionsDeleteButton->setIcon ( *iconRemove );
	collectionsImportButton->setIcon ( *iconLoad );
	collectionsExportButton->setIcon ( *iconSave );
	collectionsAddImagesButton->setIcon ( *iconPlus );
	collectionsRemoveImagesButton->setIcon ( *iconMinus );
	collectionsTagImagesButton->setIcon ( *iconPen );
	collectionsAddImagesOkButton->setIcon ( *iconOk );
	collectionsAddImagesCancelButton->setIcon ( *iconClose );

	navigationBox->setItemIcon ( 0, *iconFolderBrowser );
	navigationBox->setItemIcon ( 1, *iconCollectionsBrowser );
	navigationBox->setItemIcon ( 2, *iconDocumentBrowser );

	zoomPlusButton->setIcon ( *iconZoomPlus );
	zoomMinusButton->setIcon ( *iconZoomMinus );

	actionsGoButton->setIcon ( *iconOk );

	closeButton->setIcon ( *iconClose );

	moreButton->setIcon ( *iconArrowDown );
}


void PictureBrowser::setSettings()
{
//set settings
	if ( pbSettings.saveSettings )
	{
		saveSettingsCheckbox->setCheckState ( Qt::Checked );
	}

	if ( pbSettings.showMore )
	{
		expandDialog ( true );
	}
	else
	{
		expandDialog ( false );
	}

	if ( pbSettings.sortOrder )
	{
		sortOrderButton->setIcon ( *iconArrowUp );
	}
	else
	{
		sortOrderButton->setIcon ( *iconArrowDown );
	}

	sortCombobox->setCurrentIndex ( pbSettings.sortSetting );
	previewModeCombobox->setCurrentIndex ( pbSettings.previewMode );

	if ( pbSettings.alwaysOnTop )
	{
		alwaysOnTopCheckbox->setCheckState ( Qt::Checked );
		setAlwaysOnTop ( true );
	}
}

void PictureBrowser::changedDocument ( ScribusDoc* doc )
{
	srcDoc = doc;
	updateDocumentbrowser();
	actionsGoButton->setEnabled ( true );
	insertImageButton->setEnabled ( true );
}

void PictureBrowser::closedDocument()
{
	documentWidget->clear();
	documentItems.clear();
	srcDoc = NULL;
	pImages->clearPreviewImagesList();
	updateBrowser ( false, false, false );
	actionsGoButton->setEnabled ( false );
	insertImageButton->setEnabled ( false );
}

void PictureBrowser::updateDocumentbrowser()
{
	documentWidget->clear();
	documentItems.clear();
	QTreeWidgetItem* allpages=new QTreeWidgetItem ( documentWidget, QStringList ( "All Pages" ) );
	allpages->setData ( 0, Qt::UserRole, 0 );
	allpages->setExpanded ( true );
	documentWidget->addTopLevelItem ( allpages );

	QTreeWidgetItem *tmpItem;

	for ( int i = 0 ; i < ( int ) ( srcDoc->Pages->count() ) ; ++i )
	{
		tmpItem = new QTreeWidgetItem ( allpages, QStringList ( QString ( "Page %1" ).arg ( i+1 ) ) );
		tmpItem->setData ( 0, Qt::UserRole, ( i+1 ) );
		tmpItem->setIcon ( 0, *iconDocument );
		documentItems.append ( tmpItem );
	}

	documentWidget->insertTopLevelItems ( 0, documentItems );
}


void PictureBrowser::previewImageSelectionChanged ( const QItemSelection & selected, const QItemSelection & deselected )
{
	QItemSelectionModel *selectionModel = imageViewArea->SelectionModel();
	QModelIndexList selection = selectionModel->selectedIndexes();
	int tmpIndex;


	selectedIndexes.clear();

	for ( int i = 0 ; i < selection.size() ; ++i )
	{
		tmpIndex = selection.at ( i ).row();

		//take filtered images into account
		for ( int j = 0 ; ( j <= tmpIndex ) && ( j < pImages->previewImagesList.size() ) ; ++j )
		{
			if ( pImages->previewImagesList.at ( j )->filtered )
			{
				tmpIndex++;
			}
		}

		selectedIndexes.append ( tmpIndex );
	}

	updateTagImagesTab();
}


void PictureBrowser::updateInformationTab ( int index )
{
	if ( pbSettings.showMore && ( tabWidget->currentIndex() == 0 ) )
	{
		if ( ( index >= 0 ) && ( index < pModel->modelItemsList.size() ) )
		{
			previewImage *tmpImage;
			tmpImage = pModel->modelItemsList.at ( index );

			informationFilenameLabel->setText ( tmpImage->fileInformation.fileName() );
			informationFilepathLabel->setText ( tmpImage->fileInformation.absolutePath() );
			informationFilesizeLabel->setText ( QString ( "%1 Bytes" ).arg ( tmpImage->fileInformation.size() ) );
			informationFiledateLabel->setText ( tmpImage->fileInformation.lastModified().toString ( "dd.MM.yyyy hh:mm:ss" ) );

			if(tmpImage->previewImageLoading)
				informationFilenameLabel->setText (tr("Image still loading"));
			else if ( tmpImage->imgInfo->valid )
			{
				QString format;

				switch ( tmpImage->imgInfo->type )
				{
					case 0:
						format = tr ( "JPG" );
						break;

					case 1:
						format = tr ( "TIFF" );
						break;

					case 2:
						format = tr ( "PSD" );
						break;

					case 3:
						format = tr ( "EPS/PS" );
						break;

					case 4:
						format = tr ( "PDF" );
						break;

					case 5:
						format = tr ( "JPG2000" );
						break;

					case 6:
						format = tmpImage->fileInformation.suffix().toUpper();
						break;

					case 7:
						format = tr ( "emb. PSD" );
						break;

					default:
						format = tr ( "not available" );
						break;
				}

				informationFormatLabel->setText ( format );
				informationColorspaceLabel->setText ( colorSpaceText ( tmpImage->imgInfo->colorspace ) );
				informationDpiLabel->setText ( QString ( "%1 x %2" ).arg ( tmpImage->imgInfo->xdpi ).arg ( tmpImage->imgInfo->ydpi ) );
				informationWidthLabel->setText ( QString ( "%1 Pixels" ).arg ( tmpImage->imgInfo->width ) );
				informationHeightLabel->setText ( QString ( "%1 Pixels" ).arg ( tmpImage->imgInfo->height ) );
				informationLayersLabel->setText ( QString ( "%1" ).arg ( tmpImage->imgInfo->layers ) );

				if ( tmpImage->imgInfo->embedded )
				{
					informationEmbeddedLabel->setText ( QString ( "Yes" ) );
				}
				else
				{
					informationEmbeddedLabel->setText ( QString ( "No" ) );
				}

				informationProfilenameLabel->setText ( QString ( "%1" ).arg ( tmpImage->imgInfo->profileName ) );
			}
		}
		else
		{
			informationFilenameLabel->setText ( tr("No image selected") );
		}
	}
}


void PictureBrowser::updateCollectionsWidget ( bool addImages )
{
	collectionsWidget->blockSignals ( true );
	collectionsWidget->clear();

	for ( int i = 0 ; i < collectionsDb.size() ; ++i )
	{
		collections *tmpCollections = collectionsDb.at ( i );

		QTreeWidgetItem *tmpCategory;
		tmpCategory = new QTreeWidgetItem ( collectionsWidget, QStringList ( tmpCollections->name ) );
		tmpCategory->setFlags ( Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsEnabled );
		tmpCategory->setData ( 0, Qt::UserRole, QString ( "Category" ) );
		tmpCategory->setExpanded ( true );

		for ( int j = 0 ; j < tmpCollections->collectionNames.size() ; ++j )
		{
			QTreeWidgetItem *tmpItem;
			tmpItem = new QTreeWidgetItem ( tmpCategory, QStringList ( tmpCollections->collectionNames.at ( j ) ) );

			if ( addImages )
			{
				tmpItem->setFlags ( Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled );
				tmpItem->setCheckState ( 0, Qt::Unchecked );
			}
			else
			{
				tmpItem->setFlags ( Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsEnabled );
			}

			tmpItem->setData ( 0, Qt::UserRole, tmpCollections->collectionFiles.at ( j ) );
			tmpItem->setIcon ( 0, *iconCollection );
		}
	}

	collectionsWidget->blockSignals ( false );
}


void PictureBrowser::updateCollectionsAddImagesCombobox()
{
	/*
	collections *tmpCollections;

		for(int i = 0 ; i < collectionsDb.size() ; ++i)
		{
		tmpCollections = collectionsDb.at(i);

			for(int j = 0 ; j < tmpCollections->collectionNames.size() ; ++j)
			{
			collectionsAddImagesCombobox->addItem(QString("%1/%2").arg(tmpCollections->name).arg(tmpCollections->collectionNames.at(j)), false);
			}
		}*/
}


void PictureBrowser::expandDialog ( bool expand )
{
	if ( expand )
	{
		tabWidget->show();

		resize ( 872, 550 );
		moreButton->setText ( "Hide" );
		moreButton->setIcon ( *iconArrowUp );
	}
	else
	{
		tabWidget->hide();

		resize ( 872, 385 );
		moreButton->setText ( "More" );
		moreButton->setIcon ( *iconArrowDown );
	}
}


void PictureBrowser::setAlwaysOnTop ( bool alwaysOnTop )
{
	Qt::WindowFlags flags = windowFlags();

//save current position of the window
	QPoint pos = mapToParent ( QPoint ( 0, 0 ) );

	if ( alwaysOnTop )
	{
		flags |= Qt::WindowStaysOnTopHint;
	}
	else
	{
		flags ^= Qt::WindowStaysOnTopHint;
	}

	this->setWindowFlags ( flags );

//move to previous position
	move ( pos );

//FIXME: calling setWindowFlags sets window invisible (odd behaviour, documented). causes flickering
	this->show();
}


void PictureBrowser::saveCollectionsDb()
{
	QTreeWidgetItem *tmpItem, *tmpItem2;
	collections *tmpCollections;

	for ( int i = 0 ; i < collectionsDb.size() ; ++i )
	{
		delete collectionsDb.at ( i );
	}

	collectionsDb.clear();


	for ( int i = 0 ; i < collectionsWidget->topLevelItemCount() ; ++i )
	{
		tmpItem = collectionsWidget->topLevelItem ( i );

		tmpCollections = new collections ( tmpItem->text ( 0 ) );
		collectionsDb.append ( tmpCollections );

		for ( int j = 0 ; j < tmpItem->childCount() ; ++j )
		{
			tmpItem2 = tmpItem->child ( j );
			tmpCollections->collectionNames.append ( tmpItem2->text ( 0 ) );
			tmpCollections->collectionFiles.append ( tmpItem2->data ( 0, Qt::UserRole ).toString() );
		}
	}


	if ( !cdbwt )
	{
		cdbwt = new collectionsWriterThread ( cdbFile, collectionsDb );
		connect ( cdbwt, SIGNAL ( finished() ), this, SLOT ( collectionsDbWriterThreadFinished() ) );
		cdbwt->start();
	}
	else
	{
		cdbwt->restart();
	}
}


void PictureBrowser::applyFilters()
{
	QListWidgetItem *item;
	int c[5] = {0, 0, 0, 0, 0};
	int filterType;

	pImages->clearFilters();

	for ( int i = 0 ; i < filterFiltersListwidget->count() ; ++i )
	{
		item = filterFiltersListwidget->item ( i );
		filterType = filters->filterMap.at ( i );

		if ( item->checkState() == Qt::Checked )
		{
			switch ( filterType )
			{
				case 0:
					if ( c[0] < filters->nameFilters.size() )
					{
						pImages->filterFileName ( filters->nameFilters.at ( c[0] ), filters->nameInverts.at ( c[0] ) );
					}
					break;

				case 1:
					if ( c[1] < filters->dateFilters.size() )
					{
						pImages->filterFileModified ( filters->dateFilters.at ( c[1] ), filters->dateInverts.at ( c[1] ) );
					}
					break;

				case 2:
					if ( c[2] < filters->sizeFilters.size() )
					{
						pImages->filterFileSize ( ( filters->sizeFilters.at ( c[2] ) *1024 ), filters->sizeInverts.at ( c[2] ) );
					}
					break;

				case 3:
					if ( c[3] < filters->typeFilters.size() )
					{
						pImages->filterFileType ( filters->typeFilters.at ( c[3] ), true );
					}
				case 4:
					if ( c[4] < filters->tagFilters.size() )
					{
						pImages->filterTag ( filters->tagFilters.at ( c[4] ), true );
					}
					break;
			}
		}

		c[filterType]++;
	}
}


void PictureBrowser::updateTagImagesTab()
{
	QStringList tmpTags, tmpTagList;


	collectionsSelectedImagesListwidget->clear();
	collectionsTagImagesCombobox->clear();


	for ( int i = 0 ; ( i < selectedIndexes.size() ) && ( i < currCollection->imageFiles.size() ) ; ++i )
	{
		new QListWidgetItem ( pImages->previewImagesList.at ( selectedIndexes.at ( i ) )->fileInformation.absoluteFilePath(), collectionsSelectedImagesListwidget );
	}

	for ( int i = 0 ; i < currCollection->tags.size() ; ++i )
	{
		tmpTags = currCollection->tags.at ( i );

		for ( int j = 0 ; j < tmpTags.size() ; ++j )
		{
			if ( !tmpTagList.contains ( tmpTags.at ( j ) ) )
			{
				tmpTagList.append ( tmpTags.at ( j ) );
			}
		}
	}


	for ( int i = 0 ; i < tmpTagList.size() ; ++i )
	{
		collectionsTagImagesCombobox->addItem ( tmpTagList.at ( i ), 0 );

		int tagCount=0;

		for ( int j = 0 ; j < selectedIndexes.size() ; ++j )
		{
			if ( pImages->previewImagesList.at ( selectedIndexes.at ( j ) )->tags.contains ( tmpTagList.at ( i ) ) )
			{
				tagCount++;
			}
			else if ( tagCount > 0 )
			{
				break;
			}
		}

		if ( tagCount == selectedIndexes.size() )
		{
			collectionsTagImagesCombobox->setCheckstate ( i, 1 );
		}
		else if ( tagCount > 0 )
		{
			collectionsTagImagesCombobox->setCheckstate ( i, 2 );
		}
	}
}


imageFilters::imageFilters()
{
}


picturebrowserSettings::picturebrowserSettings()
{
}


void picturebrowserSettings::load()
{
	PrefsContext *picturebrowserPluginPrefs = PrefsManager::instance()->prefsFile->getPluginContext ( "picturebrowser" );

	saveSettings = picturebrowserPluginPrefs->getBool ( "pb_savesettings", true );
	showMore = picturebrowserPluginPrefs->getBool ( "pb_showmore", false );
	sortOrder = picturebrowserPluginPrefs->getBool ( "pb_sortorder", false );
	sortSetting = picturebrowserPluginPrefs->getInt ( "pb_sortsetting", 0 );
	previewMode = picturebrowserPluginPrefs->getInt ( "pb_previewmode", 0 );
	previewIconSize = picturebrowserPluginPrefs->getInt ( "pb_previewiconsize", 128 );
	alwaysOnTop = picturebrowserPluginPrefs->getBool ( "pb_alwaysontop", false );

// default value "dog" will be used if "s" doesn't already exist
//QString s = myPluginPrefs->get("s", "dog");
}


void picturebrowserSettings::save()
{
	PrefsContext *picturebrowserPluginPrefs = PrefsManager::instance()->prefsFile->getPluginContext ( "picturebrowser" );

	picturebrowserPluginPrefs->set ( "pb_savesettings", saveSettings );
	picturebrowserPluginPrefs->set ( "pb_showmore", showMore );
	picturebrowserPluginPrefs->set ( "pb_sortorder", sortOrder );
	picturebrowserPluginPrefs->set ( "pb_sortsetting", sortSetting );
	picturebrowserPluginPrefs->set ( "pb_previewmode", previewMode );
	picturebrowserPluginPrefs->set ( "pb_previewiconsize", previewIconSize );
	picturebrowserPluginPrefs->set ( "pb_alwaysontop", alwaysOnTop );

//picturebrowserPluginPrefs->set("previewMode", "cat");
}


void picturebrowserSettings::reset()
{
	saveSettings = true;
	showMore = false;
	sortOrder = false;
	sortSetting = 0;
	previewMode = 0;
	previewIconSize = 128;
	alwaysOnTop = false;

	save();
}
