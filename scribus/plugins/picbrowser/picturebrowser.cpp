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
	imageViewArea->setViewMode ( QListView::IconMode );
	imageViewArea->setIconSize ( QSize ( pbSettings.previewIconSize, pbSettings.previewIconSize ) );
	imageViewArea->setGridSize ( QSize ( qRound ( ( 1.1 ) *pbSettings.previewIconSize ), ( qRound ( ( 1.1 ) *pbSettings.previewIconSize ) + pbSettings.previewMode*10 ) ) );
	imageViewArea->setUniformItemSizes ( true );
	imageViewArea->setContextMenuPolicy ( Qt::CustomContextMenu );
	imageViewArea->setSelectionMode ( QAbstractItemView::ExtendedSelection );
	imageViewArea->setMovement ( QListView::Snap );
	imageViewArea->setResizeMode ( QListView::Adjust );
	imageViewArea->setModel ( pModel );

//register item selections
	connect ( imageViewArea->selectionModel(), SIGNAL ( selectionChanged ( const QItemSelection &, const QItemSelection & ) ), this, SLOT ( previewImageSelectionChanged ( const QItemSelection &, const QItemSelection & ) ) );

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
		imageViewArea->setGridSize ( QSize ( qRound ( ( 1.1 ) *pbSettings.previewIconSize ), ( qRound ( ( 1.1 ) *pbSettings.previewIconSize ) + pbSettings.previewMode*10 ) ) );

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

		imageViewArea->setIconSize ( QSize ( pbSettings.previewIconSize, pbSettings.previewIconSize ) );
		imageViewArea->setGridSize ( QSize ( qRound ( ( 1.1 ) *pbSettings.previewIconSize ), ( qRound ( ( 1.1 ) *pbSettings.previewIconSize ) + pbSettings.previewMode*10 ) ) );

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

		imageViewArea->setIconSize ( QSize ( pbSettings.previewIconSize, pbSettings.previewIconSize ) );
		imageViewArea->setGridSize ( QSize ( qRound ( ( 1.1 ) *pbSettings.previewIconSize ), ( qRound ( ( 1.1 ) *pbSettings.previewIconSize ) + pbSettings.previewMode*10 ) ) );

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
			QMessageBox::warning ( this, tr ( "Picture Browser Error" ), tr ( "File not found or file is not a collection file" ) );
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
	QItemSelectionModel *selectionModel = imageViewArea->selectionModel();
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

			if ( tmpImage->imgInfo->valid )
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
			informationFilenameLabel->setText ( "No image selected" );
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


ImageInformation::ImageInformation()
{
}


previewImage::previewImage ( QString imageFile )
{
	filtered = false;
	previewIconCreated = false;
	previewImageLoading = false;

	fileInformation.setFile ( imageFile );
//enable caching
	fileInformation.setCaching ( true );
}


//nothing to do yet
previewImage::~previewImage()
{
}


bool previewImage::createPreviewIcon ( const QImage &image, int size )
{
	QPainter p;
//width/height of our icon (exclusive 1px border)

	previewIcon = QPixmap ( size, size );

	QBrush b ( QColor ( 205,205,205 ), loadIcon ( "testfill.png" ) );

	p.begin ( &previewIcon );

	p.setPen ( QPen ( Qt::black, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin ) );
	p.setBrush ( b );
	p.drawRect ( 0, 0, size-1, size-1 );

	p.drawImage ( ( ( size - image.width() ) / 2 ), ( ( size - image.height() ) / 2 ), image );

	p.end();

	previewIconCreated = true;
	currentSize = size;

	return true;
}


bool previewImage::insertIntoDocument ( ScribusDoc *doc, InsertAFrameData &iafData )
{
//adding imageframe
	doc->itemAddUserFrame ( iafData );

	return true;
}


bool previewImage::insertIntoImageFrame ( ScribusDoc *doc, PageItem *imageFrame )
{
	//inserting image
	if ( !imageFrame->loadImage ( fileInformation.absoluteFilePath(), false, -1, true ) )
	{
		return false;
	}

	return true;
}


previewImages::previewImages ( const QStringList& imageFiles )
{
	createPreviewImagesList ( imageFiles );
}


void previewImages::createPreviewImagesList ( const QStringList& imageFiles )
{
	//if there are already previewimages allocated, remove them
	if ( previewImagesList.size() > 0 )
	{
		clearPreviewImagesList();
	}

	if ( imageFiles.isEmpty() )
	{
		return;
	}


	int s = imageFiles.size();
	previewImage *tmpPreviewImage;

	for ( int i = 0 ; i < s ; ++i )
	{
		tmpPreviewImage = new previewImage ( imageFiles.at ( i ) );
		previewImagesList.append ( tmpPreviewImage );
	}
}


void previewImages::createPreviewImagesList ( const imageCollection *collection )
{
	//if there are already previewimages allocated, remove them
	if ( previewImagesList.size() > 0 )
	{
		clearPreviewImagesList();
	}

	if ( collection->imageFiles.isEmpty() )
	{
		return;
	}


	int s = collection->imageFiles.size();
	previewImage *tmpPreviewImage;

	for ( int i = 0 ; i < s ; ++i )
	{
		tmpPreviewImage = new previewImage ( collection->imageFiles.at ( i ) );
		tmpPreviewImage->tags = collection->tags.at ( i );
		previewImagesList.append ( tmpPreviewImage );
	}
}


void previewImages::clearPreviewImagesList()
{
	int s = previewImagesList.size();

	for ( int i = 0 ; i < s ; ++i )
	{
		delete previewImagesList.at ( i );
	}

	previewImagesList.clear();
}


bool previewImages::toRemove ( bool a, bool b )
{
	if ( a )
	{
		if ( !b )
		{
			return true;
		}
	}
	else if ( b )
	{
		return true;
	}

	return false;
}


void previewImages::filterFileName ( const QString &fileName, bool invert )
{
	previewImage *tmpPreviewImage;
	QRegExp rx ( fileName );
//wildcardmode should be sufficient
	rx.setPatternSyntax ( QRegExp::Wildcard );

	for ( int i = 0 ; i < previewImagesList.size() ; ++i )
	{
		tmpPreviewImage = previewImagesList.at ( i );

		if ( toRemove ( rx.exactMatch ( tmpPreviewImage->fileInformation.fileName() ), invert ) )
		{
			tmpPreviewImage->filtered = true;
		}
	}
}


void previewImages::filterFileSize ( qint64 fileSize, bool smallerThan )
{
	previewImage *tmpPreviewImage;

	for ( int i = 0 ; i < previewImagesList.size() ; ++i )
	{
		tmpPreviewImage = previewImagesList.at ( i );

		if ( toRemove ( ( tmpPreviewImage->fileInformation.size() < fileSize ), smallerThan ) )
		{
			tmpPreviewImage->filtered = true;
		}
	}
}


void previewImages::filterFileType ( const QStringList &types, bool invert )
{
	previewImage *tmpPreviewImage;

	for ( int i = 0 ; i < previewImagesList.size() ; ++i )
	{
		tmpPreviewImage = previewImagesList.at ( i );
		QString type = QString ( "*." ) + tmpPreviewImage->fileInformation.suffix();

		if ( toRemove ( ( types.contains ( type ) ), invert ) )
		{
			tmpPreviewImage->filtered = true;
		}
	}
}


void previewImages::filterFileModified ( const QDateTime &modified, bool smallerThan )
{
	previewImage *tmpPreviewImage;

	for ( int i = 0 ; i < previewImagesList.size() ; ++i )
	{
		tmpPreviewImage = previewImagesList.at ( i );

		if ( toRemove ( ( tmpPreviewImage->fileInformation.lastModified() < modified ), smallerThan ) )
		{
			tmpPreviewImage->filtered = true;
		}
	}
}


//real image needs to be loaded for this info
void previewImages::filterResolution ( qint64 resolution, bool smallerThan )
{
	previewImage *tmpPreviewImage;

	for ( int i = 0 ; i < previewImagesList.size() ; ++i )
	{
		tmpPreviewImage = previewImagesList.at ( i );

		if ( toRemove ( ( resolution < resolution ), smallerThan ) )
		{
			tmpPreviewImage->filtered = true;
		}
	}
}


void previewImages::filterTag ( const QStringList &tags, bool invert )
{
	previewImage *tmpPreviewImage;

	for ( int i = 0 ; i < previewImagesList.size() ; ++i )
	{
		tmpPreviewImage = previewImagesList.at ( i );

		for ( int j = 0 ; j < tags.size() ; ++j )
		{
			if ( toRemove ( tmpPreviewImage->tags.contains ( tags.at ( j ) ), invert ) )
			{
				tmpPreviewImage->filtered = true;
				break;
			}
		}
	}
}


void previewImages::clearFilters()
{
	previewImage *tmpPreviewImage;

	for ( int i = 0 ; i < previewImagesList.size() ; ++i )
	{
		tmpPreviewImage = previewImagesList.at ( i );
		tmpPreviewImage->filtered = false;
	}
}


void previewImages::sortPreviewImages ( const int sort )
{
	switch ( sort )
	{
			//sort by filename
		case 0:
			qSort ( previewImagesList.begin(), previewImagesList.end(), comparePreviewImageFileName );
			break;

		case 1:
			//sort by filedate
			qSort ( previewImagesList.begin(), previewImagesList.end(), comparePreviewImageFileType );
			break;

		case 2:
			//sort by filetype
			qSort ( previewImagesList.begin(), previewImagesList.end(), comparePreviewImageFileDate );
			break;

		case 3:
			//sort by filesize
			qSort ( previewImagesList.begin(), previewImagesList.end(), comparePreviewImageFileSize );
			break;

		case 4:
			//sort by imageresolution
			qSort ( previewImagesList.begin(), previewImagesList.end(), comparePreviewImageResolution );
			break;

		default:
			break;
			//if a wrong argument was specified, sort by name
			qSort ( previewImagesList.begin(), previewImagesList.end(), comparePreviewImageFileName );
	}
}


bool comparePreviewImageFileName ( const previewImage *i1, const previewImage *i2 )
{
	return i1->fileInformation.fileName() < i2->fileInformation.fileName();
}


bool comparePreviewImageFileType ( const previewImage *i1, const previewImage *i2 )
{
	return i1->fileInformation.suffix() < i2->fileInformation.suffix();
}


bool comparePreviewImageFileDate ( const previewImage *i1, const previewImage *i2 )
{
	return i1->fileInformation.lastModified() < i2->fileInformation.lastModified();
}


bool comparePreviewImageFileSize ( const previewImage *i1, const previewImage *i2 )
{
	return i1->fileInformation.size() < i2->fileInformation.size();
}

//real image needs to be loaded for this information!
bool comparePreviewImageResolution ( const previewImage *i1, const previewImage *i2 )
{
//return (resolution) < (resolution);
	return true;
}


PreviewImagesModel::PreviewImagesModel ( QObject *parent ) : QAbstractListModel ( parent )
{
}


PreviewImagesModel::PreviewImagesModel ( PictureBrowser *parent ) : QAbstractListModel ( parent )
{
	pictureBrowser = parent;
	pId = 0;

	createDefaultIcon ( pictureBrowser->pbSettings.previewIconSize );
}


void PreviewImagesModel::createDefaultIcon ( int size )
{
	QPainter p;

	defaultIcon = QPixmap ( size, size );

	QBrush b ( QColor ( 205,205,205 ), loadIcon ( "testfill.png" ) );

	p.begin ( &defaultIcon );

	p.setPen ( QPen ( Qt::black, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin ) );
	p.setBrush ( b );
	p.drawRect ( 0, 0, size-1, size-1 );

	p.end();

	defaultIconSize = size;
}


void PreviewImagesModel::setModelItemsList ( const QList<previewImage *> &previewImagesList )
{
//create new id
	pId++;

	//if there are already items stored, remove them
	if ( modelItemsList.size() > 0 )
	{
		clearModelItemsList();
	}

	pictureBrowser->imagesDisplayed = 0;
	pictureBrowser->imagesFiltered = 0;

	beginInsertRows ( QModelIndex(), 0, previewImagesList.size() );

	previewImage *tmpPreviewImage;

	for ( int i = 0 ; i < previewImagesList.size() ; ++i )
	{
		if ( ! ( pictureBrowser->pbSettings.sortOrder ) )
		{
			tmpPreviewImage = previewImagesList.at ( i );
		}
		else
		{
			tmpPreviewImage = previewImagesList.at ( previewImagesList.size() - i - 1 );
		}

		if ( ! ( tmpPreviewImage->filtered ) )
		{
			modelItemsList.append ( tmpPreviewImage );
			pictureBrowser->imagesDisplayed++;
		}
		else
		{
			pictureBrowser->imagesFiltered++;
		}
	}

	endInsertRows();
}


void PreviewImagesModel::clearModelItemsList()
{
	beginRemoveRows ( QModelIndex(), 0, ( modelItemsList.size() - 1 ) );

	modelItemsList.clear();

	endRemoveRows();
}


QVariant PreviewImagesModel::data ( const QModelIndex &index, int role ) const
{
	if ( !index.isValid() )
	{
		return QVariant();
	}

	int row = index.row();
	previewImage *tmpImage = modelItemsList.at ( row );

	if ( role == Qt::DecorationRole )
	{
		pictureBrowser->currentRow = row;

		//preload icons before and after current icon
		if ( ( row - pictureBrowser->previewIconsVisible ) >= 0 )
		{
			previewImage *tmpImage2 = modelItemsList.at ( row - pictureBrowser->previewIconsVisible );

			if ( ( !tmpImage2->previewIconCreated ) && ( !tmpImage2->previewImageLoading ) )
			{
				tmpImage2->previewImageLoading = true;
				pictureBrowser->callLoadImageThread ( row - pictureBrowser->previewIconsVisible, pId );
			}
		}

		if ( ( row + pictureBrowser->previewIconsVisible ) < modelItemsList.size() )
		{
			previewImage *tmpImage3 = modelItemsList.at ( row + pictureBrowser->previewIconsVisible );

			if ( ( !tmpImage3->previewIconCreated ) && ( !tmpImage3->previewImageLoading ) )
			{
				tmpImage3->previewImageLoading = true;
				pictureBrowser->callLoadImageThread ( row + pictureBrowser->previewIconsVisible, pId );
			}
		}

		if ( !tmpImage->previewIconCreated )
		{
			if ( !tmpImage->previewImageLoading )
			{
				tmpImage->previewImageLoading = true;
				pictureBrowser->callLoadImageThread ( row, pId );
			}

			return QIcon ( defaultIcon );
		}
		else
		{
			return QIcon ( tmpImage->previewIcon );
		}

		return QVariant();
	}
	else if ( role == Qt::DisplayRole )
	{
		if ( pictureBrowser->pbSettings.previewMode == 1 )
		{
			return tmpImage->fileInformation.fileName();
		}
		else
		{
			return QVariant();
		}
	}

	return QVariant();
}


Qt::ItemFlags PreviewImagesModel::flags ( const QModelIndex &index ) const
{
	if ( index.isValid() )
	{
		return ( Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled );
	}

//Qt modeltest requests 0 here
	return 0;
}


int PreviewImagesModel::rowCount ( const QModelIndex &parent ) const
{
	if ( parent.isValid() )
	{
		return 0;
	}
	else
	{
		return modelItemsList.size();
	}
}


QStringList PreviewImagesModel::mimeTypes() const
{
	QStringList types;
	types << "text/uri-list";
	return types;
}


QMimeData *PreviewImagesModel::mimeData ( const QModelIndexList &indexes ) const
{
	QMimeData *mimeData = new QMimeData();
	int row;
	QList<QUrl> urls;
	QString imageFile;

	foreach ( QModelIndex index, indexes )
	{
		if ( index.isValid() )
		{
			row = index.row();

			if ( ( row >= 0 ) && ( row < modelItemsList.size() ) )
			{
				imageFile = modelItemsList.at ( row )->fileInformation.absoluteFilePath();
				urls.append ( QUrl ( imageFile ) );
			}
		}
	}

	mimeData->setUrls ( urls );
	return mimeData;
}


void PreviewImagesModel::processLoadedImage ( int row, const QImage image, ImageInformation* imgInfo, int tpId )
{
	//check if list of files has changed and this job is obsolete
	if ( tpId != pId )
	{
		return;
	}

//emit layoutAboutToBeChanged();

	previewImage *loadedImage = modelItemsList.at ( row );
	loadedImage->previewImageLoading = false;
	loadedImage->createPreviewIcon ( image, pictureBrowser->pbSettings.previewIconSize );

	loadedImage->imgInfo = imgInfo;

//emit layoutChanged();

	QModelIndex changedIndex = index ( row );
	dataChanged ( changedIndex, changedIndex );
}


void PreviewImagesModel::processImageLoadError ( int row, int tpId, int errorcode )
{
	//check if list of files has changed and this job is obsolete
	if ( tpId != pId )
	{
		return;
	}

//emit layoutAboutToBeChanged();

	previewImage *loadedImage = modelItemsList.at ( row );
	loadedImage->previewImageLoading = false;

//emit layoutChanged();
}


imageCollection::imageCollection()
{
}


collections::collections ( QString collectionsName )
{
	name = collectionsName;
}


collectionReaderThread::collectionReaderThread ( QString &xmlFile2, bool importCollection )
{
	categoriesCount = 0;
	type = 0;
	restartThread = false;

	xmlFile = xmlFile2;
	import = importCollection;
}


void collectionReaderThread::readFile()
{
	QFile inputFile ( xmlFile );

	if ( !inputFile.open ( QFile::ReadOnly | QFile::Text ) )
	{
		return;
	}

	setDevice ( &inputFile );

	while ( !atEnd() )
	{
		readNext();

		if ( isStartElement() )
		{
			if ( name() == "picturebrowser" )
			{
				//we have a collectionsdbfile
				if ( attributes().value ( "type" ) == "collectionsset" )
				{
					readCollectionsDb();
					type = 1;
				}
				//we have a collectionfile
				else if ( attributes().value ( "type" ) == "collection" )
				{
					collection = new imageCollection;
					collection->file = xmlFile;

					QString name = attributes().value ( "name" ).toString();

					if ( !name.isEmpty() )
					{
						collection->name = name;
					}
					else
					{
						collection->name = xmlFile;
					}

					readCollectionFile();
					type = 2;
				}
			}
		}
	}
}


void collectionReaderThread::readCollectionsDb()
{
	while ( !atEnd() )
	{
		readNext();

		if ( isEndElement() )
		{
			break;
		}

		if ( isStartElement() )
		{
			if ( name() == "category" )
			{
				QString name = attributes().value ( "name" ).toString();
				collections *tmpCollections = new collections ( name );
				collectionsSet.append ( tmpCollections );

				readCategory();

				categoriesCount++;
			}
			else
			{
				readUnknownElement();
			}
		}
	}
}


void collectionReaderThread::readCollectionFile()
{
	while ( !atEnd() && !restartThread )
	{
		readNext();

		if ( isEndElement() )
		{
			break;
		}

		if ( isStartElement() )
		{
			if ( name() == "image" )
			{
				QString tmpImageFile = attributes().value ( "file" ).toString();
				collection->imageFiles.append ( tmpImageFile );

				readImage();
			}
			else
			{
				readUnknownElement();
			}
		}
	}
}


void collectionReaderThread::readCategory()
{
	while ( !atEnd() )
	{
		readNext();

		if ( isEndElement() )
		{
			break;
		}

		if ( isStartElement() )
		{
			if ( name() == "collection" )
			{
				readCollection();
			}
			else
			{
				readUnknownElement();
			}
		}
	}
}


void collectionReaderThread::readCollection()
{
	collections *tmpCollections = collectionsSet.at ( categoriesCount );

	tmpCollections->collectionFiles.append ( attributes().value ( "file" ).toString() );
	tmpCollections->collectionNames.append ( readElementText() );
}


void collectionReaderThread::readImage()
{
	QStringList tmpTags;

	while ( !atEnd() && !restartThread )
	{
		readNext();

		if ( isEndElement() )
		{
			break;
		}

		if ( isStartElement() )
		{
			if ( name() == "tag" )
			{
				//read tag here
				tmpTags.append ( readElementText() );
			}
			else
			{
				readUnknownElement();
			}
		}
	}

	collection->tags.append ( tmpTags );
}


void collectionReaderThread::readUnknownElement()
{
	while ( !atEnd() )
	{
		readNext();

		if ( isEndElement() )
		{
			break;
		}

		if ( isStartElement() )
		{
			readUnknownElement();
		}
	}
}


void collectionReaderThread::run()
{
	readFile();
}


void collectionReaderThread::restart()
{
	restartThread = true;
}


collectionListReaderThread::collectionListReaderThread ( QStringList &xmlFiles2 )
{
	restartThread = false;

	xmlFiles = xmlFiles2;
}


void collectionListReaderThread::run()
{
	if ( xmlFiles.isEmpty() )
	{
		return;
	}

	xmlFile = xmlFiles.takeAt ( 0 );
	clrt = new collectionReaderThread ( xmlFile, false );
	connect ( clrt, SIGNAL ( finished() ), this, SLOT ( collectionReaderThreadFinished() ) );
	clrt->start();

	exec();
}


void collectionListReaderThread::restart()
{
	restartThread = true;
}


void collectionListReaderThread::collectionReaderThreadFinished()
{
	readCollections.append ( clrt->collection );
	delete clrt;

	if ( xmlFiles.isEmpty() || restartThread )
	{
		quit();
	}
	else
	{
		xmlFile = xmlFiles.takeAt ( 0 );
		clrt = new collectionReaderThread ( xmlFile, false );
		connect ( clrt, SIGNAL ( finished() ), this, SLOT ( collectionReaderThreadFinished() ) );
		clrt->start();
	}
}


collectionsWriterThread::collectionsWriterThread ( QString &xmlFile2, QList<collections *> saveCollections2 )
{
	xmlFile = xmlFile2;
	saveCollections = saveCollections2;
	restartThread = false;
}


void collectionsWriterThread::writeFile()
{
	QFile outputFile ( xmlFile );

	if ( !outputFile.open ( QFile::WriteOnly | QFile::Text ) )
	{
		return;
	}

	setDevice ( &outputFile );

	writeStartDocument();
	writeCharacters ( "\n" );
//writeDTD("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
	writeStartElement ( "picturebrowser" );

	writeAttribute ( "type", "collectionsset" );

	writeCharacters ( "\n" );

	for ( int i = 0 ; i < saveCollections.size() && !restartThread ; ++i )
	{
		writeCategory ( saveCollections.at ( i ) );
	}

	writeEndDocument();
}


void collectionsWriterThread::writeCategory ( const collections *category )
{
	writeStartElement ( "category" );
	writeAttribute ( "name", category->name );
	writeCharacters ( "\n" );

	for ( int i = 0 ; i < category->collectionNames.size() && !restartThread ; ++i )
	{
		writeCollection ( category->collectionNames.at ( i ), category->collectionFiles.at ( i ) );
	}

	writeEndElement();
	writeCharacters ( "\n" );
}


void collectionsWriterThread::writeCollection ( const QString &collectionName, const QString &collectionFile )
{
	writeStartElement ( "collection" );
	writeAttribute ( "file", collectionFile );

	writeCharacters ( collectionName );

	writeEndElement();
	writeCharacters ( "\n" );
}


void collectionsWriterThread::run()
{
	writeFile();
}


void collectionsWriterThread::restart()
{
	restartThread = true;
}



collectionWriterThread::collectionWriterThread ( QString &xmlFile2, imageCollection &saveCollection2 )
{
	xmlFile = xmlFile2;
	saveCollection = saveCollection2;
}


void collectionWriterThread::writeFile()
{
	QFile outputFile ( xmlFile );

	if ( !outputFile.open ( QFile::WriteOnly | QFile::Text ) )
	{
		return;
	}

	setDevice ( &outputFile );

	writeStartDocument();
	writeCharacters ( "\n" );
//writeDTD("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
	writeStartElement ( "picturebrowser" );

	writeAttribute ( "type", "collection" );

	if ( !saveCollection.name.isEmpty() )
	{
		writeAttribute ( "name", saveCollection.name );
	}

	writeCharacters ( "\n" );

	for ( int i = 0 ; i < saveCollection.imageFiles.size() ; ++i )
	{
		writeImage ( saveCollection.imageFiles.at ( i ), saveCollection.tags.at ( i ) );
	}

	writeEndDocument();
}


void collectionWriterThread::writeImage ( const QString &imageFile, const QStringList &tags )
{
	writeStartElement ( "image" );
	writeAttribute ( "file", imageFile );
	writeCharacters ( "\n" );

	writeTags ( tags );

	writeEndElement();
	writeCharacters ( "\n" );
}


void collectionWriterThread::writeTags ( const QStringList &tags )
{
	for ( int i = 0 ; i < tags.size() ; ++i )
	{
		writeStartElement ( "tag" );

		writeCharacters ( tags.at ( i ) );

		writeEndElement();
		writeCharacters ( "\n" );
	}
}


void collectionWriterThread::run()
{
	writeFile();
}


loadImagesThread::loadImagesThread ( PictureBrowser *parent, PreviewImagesModel *parentModel )
{
	pictureBrowser = parent;
	pModel = parentModel;
}


void loadImagesThread::run()
{
	loadImagesThreadInstance help;

	help.pictureBrowser = pictureBrowser;
	help.pModel = pModel;

//register types for slots and signals
	qRegisterMetaType<previewImage *> ( "previewImage*" );
	qRegisterMetaType<ImageInformation *> ( "ImageInformation*" );
	qRegisterMetaType<QImage> ( "QImage" );

	connect ( &help, SIGNAL ( imageLoaded ( int, const QImage, ImageInformation*, int ) ), pModel, SLOT ( processLoadedImage ( int, const QImage, ImageInformation*, int ) ) );
	connect ( &help, SIGNAL ( imageLoadError ( int, int, int ) ), pModel, SLOT ( processImageLoadError ( int, int, int ) ) );
	connect ( pictureBrowser, SIGNAL ( loadImageJob ( int, QString, int, int ) ), &help, SLOT ( processLoadImageJob ( int, QString, int, int ) ) );

	exec();
}


loadImagesThreadInstance::loadImagesThreadInstance()
{
}

void loadImagesThreadInstance::processLoadImageJob ( int row, QString path, int size, int tpId )
{
	//check if list of files has changed and this job is obsolete
	if ( pModel->pId != tpId )
	{
		return;
	}

	if ( qAbs ( row - pictureBrowser->currentRow ) > 2* ( pictureBrowser->previewIconsVisible ) )
	{
		emit imageLoadError ( row, tpId, 0 );
		return;
	}

	ScImage image;
//no realCMYK
	bool mode=false;
//no document needs to be assigned to this
	CMSettings cms ( 0, "", 0 );

	ImageInformation *imgInfo = new ImageInformation;

	//load previewimage
	if ( image.LoadPicture ( path, 1, cms, true, false, ScImage::Thumbnail, 72, &mode ) )
	{
		int ix,iy;
		if ( ( image.imgInfo.exifDataValid ) && ( !image.imgInfo.exifInfo.thumbnail.isNull() ) )
		{
			ix = image.imgInfo.exifInfo.width;
			iy = image.imgInfo.exifInfo.height;
		}
		else
		{
			ix = image.width();
			iy = image.height();
		}
		( *imgInfo ).width = ix;
		( *imgInfo ).height = iy;
		( *imgInfo ).type = image.imgInfo.type;
		( *imgInfo ).colorspace = image.imgInfo.colorspace;
		( *imgInfo ).xdpi = qRound ( image.imgInfo.xres );
		( *imgInfo ).ydpi = qRound ( image.imgInfo.yres );
		( *imgInfo ).layers = image.imgInfo.layerInfo.size();
		( *imgInfo ).embedded = image.imgInfo.isEmbedded;
		( *imgInfo ).profileName = image.imgInfo.profileName;
		( *imgInfo ).valid = true;

		//image is bigger than our icon -> resize
		if ( ( image.width() > ( size-2 ) ) || ( image.height() > ( size-2 ) ) )
		{
			emit imageLoaded ( row, image.scaled ( ( size-2 ), ( size-2 ), Qt::KeepAspectRatio, Qt::SmoothTransformation ), imgInfo, tpId );
		}
		//image is <= our icon -> put it in as it is
		else
		{
			emit imageLoaded ( row, image.qImage().copy(), imgInfo, tpId );
		}
	}
	else
	{
		//emit some errorsignal here
		( *imgInfo ).valid = false;
		emit imageLoaded ( row, QImage(), imgInfo, tpId );
	}
}


findImagesThread::findImagesThread ( const QString& path2, const QStringList& nameFilters2, QDir::SortFlags sort2, bool searchSubfolders2 )
{
	restartThread = false;

	startPath = path2;
	nameFilters = nameFilters2;
	sort = sort2;
	searchSubfolders = searchSubfolders2;
}


void findImagesThread::run()
{
	findFiles ( startPath );
}


void findImagesThread::restart()
{
	restartThread = true;
}


void findImagesThread::findFiles ( const QString& path )
{
	QDir dir ( path );

	if ( dir.exists() )
	{
		QFileInfoList list;
		dir.setFilter ( QDir::AllDirs|QDir::Drives|QDir::Files|QDir::NoDotAndDotDot|QDir::NoSymLinks|QDir::Hidden );
		dir.setNameFilters ( nameFilters );
		dir.setSorting ( sort );

		list = dir.entryInfoList();

		for ( int i = 0 ; ( i < list.size() ) && ( !restartThread ) ; ++i )
		{
			QFileInfo fileInfo = list.at ( i );

			if ( fileInfo.isDir() )
			{
				if ( searchSubfolders )
				{
					findFiles ( fileInfo.canonicalFilePath() );
				}
			}
			else
			{
				imageFiles.append ( fileInfo.canonicalFilePath() );
			}
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
