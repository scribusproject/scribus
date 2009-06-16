/*****************************************************************
* Copyright (C) 2009  Pierre Marchand

* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.

* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.

* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
*
******************************************************************/

#include "previewimage.h"
#include "picturebrowser.h"
#include "collection.h"

#include "util_icon.h"
#include "util.h"

#include <QPainter>
#include <QUrl>

// //functions for comparing attributes of previewimages (used for sorting)
bool comparePreviewImageFileName ( const previewImage *i1, const previewImage *i2 );
bool comparePreviewImageFileType ( const previewImage *i1, const previewImage *i2 );
bool comparePreviewImageFileDate ( const previewImage *i1, const previewImage *i2 );
bool comparePreviewImageFileSize ( const previewImage *i1, const previewImage *i2 );
bool comparePreviewImageResolution ( const previewImage *i1, const previewImage *i2 );

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
	if(0 == previewImagesList.count())
		return;
	
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

