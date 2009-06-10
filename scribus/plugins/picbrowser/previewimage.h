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

#ifndef PREVIEWIMAGE_H
#define PREVIEWIMAGE_H

#include <QAbstractListModel>
#include <QDateTime>
#include <QImage>
#include <QFileInfo>
#include <QMimeData>
#include <QModelIndex>
#include <QPixmap>
#include <QString>
#include <QStringList>
#include <QVariant>

#include "scribusdoc.h"

class imageCollection;
class PageItem;
class PictureBrowser;


//holds information about an image
class ImageInformation
{
	public:
		ImageInformation();

		int width, height;
		int type;
		int colorspace;
		int xdpi, ydpi;
		int layers;
		bool embedded;
		QString profileName;
		bool valid;
};


//class for a single previewimage, contains all information related to the image
class previewImage
{
	public:
		//constructor, sets filepath for an image
		previewImage ( QString imageFile );
		~previewImage();

		//creates the previewicon in desired size in pixels
		//parameters:
		//const QImage &image: image to create icon from
		//int size: the desired iconsize
		bool createPreviewIcon ( const QImage &image, int size );

		//creates a new imageframe on a page and inserts the image
		bool insertIntoDocument ( ScribusDoc *doc, InsertAFrameData &iafData );
		//inserts the image into a given imageframe
		bool insertIntoImageFrame ( ScribusDoc *doc, PageItem *imageFrame );

		//filter attribute. if filter settings match, its set to true and no longer considered for display/sorting
		bool filtered;
		//contains fileinformation
		QFileInfo fileInformation;
		//keeps imageinformation
		ImageInformation *imgInfo;
		//current size of previewIcon
		int currentSize;
		//tells wether previewIcon is loading in thread
		bool previewImageLoading;
		//tells if previewIcon was created previously or needs to be loaded
		bool previewIconCreated;
		//contains previewicon which is to be shown
		QPixmap previewIcon;
		//tags
		QStringList tags;
};


//class for managing a set of previewImage objects
class previewImages
{
	public:
		//constructor, adds imageFiles to this set
		previewImages ( const QStringList& imageFiles );

		//creates previewimage objects from a QStringList
		void createPreviewImagesList ( const QStringList& imageFiles );
		//creates previewimage objects from imageCollection
		void createPreviewImagesList ( const imageCollection *collection );
		//deletes all entries
		void clearPreviewImagesList();

		//little helper function to reduce code
		bool toRemove ( bool a, bool b );
		//filters by filename, invert specifies wether to include or to remove file fileName
		void filterFileName ( const QString &fileName, bool invert );
		//filters by filesize, smallerThan specifies wether to include or to remove files smaller than fileSize
		void filterFileSize ( qint64 fileSize, bool smallerThan );
		//filters by filetype, invert specifies wether to include or to remove files with fileExtension
		void filterFileType ( const QStringList &types, bool invert );
		//filters by date last modified, smallerThan specifies wether to include or to remove files younger than modified
		void filterFileModified ( const QDateTime &modified, bool smallerThan );
		//filters by imageresolution, smallerThan specifies wether to include or to remove images with lower resolution
		void filterResolution ( qint64 resolution, bool smallerThan );
		//filters by tag, invert specifies wether to include or to remove item with tag
		void filterTag ( const QStringList &tags, bool invert );
		//clears all previously applied filters
		void clearFilters();

		//sorts previewImagesList after sort criteria
		//0: sort by filename
		//1: sort by fileextension
		//2: sort by date last modified
		//3: sort by filesize
		void sortPreviewImages ( const int sort );

		//all our precious previewimages are indexed here
		QList<previewImage *> previewImagesList;
};


//a model which holds the previewimages and is connected to QListView
class PreviewImagesModel : public QAbstractListModel
{
		Q_OBJECT

	public:
		PreviewImagesModel ( QObject *parent = 0 );
		//saves pointer to calling PictureBrowser instance, for later access
		PreviewImagesModel ( PictureBrowser *parent );

		//creates the default icon which is displayed when image is not loaded yet
		//parameters:
		//int size: size of the icon to be created
		void createDefaultIcon ( int size );
		//sets the modelitemslist to previewImagesList, ignores filtered images
		void setModelItemsList ( const QList<previewImage *> &previewImagesList );
		//removes all items
		void clearModelItemsList();

		//returns previewicons and, depending on previewmode, additional text
		QVariant data ( const QModelIndex &index, int role = Qt::DisplayRole ) const;
		//returns flags for items
		Qt::ItemFlags flags ( const QModelIndex &index ) const;
		//returns the number of previewimages
		int rowCount ( const QModelIndex &parent ) const;

		QMimeData *mimeData ( const QModelIndexList &indexes ) const;
		QStringList mimeTypes() const;

		//all our precious previewimages are indexed here
		QList<previewImage *> modelItemsList;
		//a unique id telling the thread wether to cancel current loading operations because files have changed
		int pId;

	private slots:
		//called when imageloadingthread has finished
		//parameters:
		//previewImage* loadedImage: pointer to identify the loaded image
		//const QImage image: the actual image
		//int tpId: the id the thread has been called from
		void processLoadedImage ( int row, const QImage image, ImageInformation* imgInfo, int tpId );
		void processImageLoadError ( int row, int tpId, int errorcode );

	private:
		//pointer to the picturebrowser class
		PictureBrowser *pictureBrowser;
		//default icon
		QPixmap defaultIcon;
		//icon size of defaultIcon
		int defaultIconSize;
};

#endif // PREVIEWIMAGE_H
