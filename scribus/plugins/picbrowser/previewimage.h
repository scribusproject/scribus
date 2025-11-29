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

class ImageCollection;
class PageItem;
class PictureBrowser;

//holds information about an image
class ImageInformation
{
	public:
		ImageInformation() = default;

		int width { 0 };
		int height { 0 };
		int type { 0 };
		int colorspace { 0 };
		int xdpi { 72 };
		int ydpi { 72 };
		int layers { 0 };
		bool embedded { false };
		QString profileName;
		bool valid { false };
};


//class for a single previewimage, contains all information related to the image
class PreviewImage
{
	public:
		//constructor, sets filepath for an image
		PreviewImage(const QString& imageFile);
		~PreviewImage();

		//creates the previewicon in desired size in pixels
		//parameters:
		//const QImage &image: image to create icon from
		//int size: the desired iconsize
		bool createPreviewIcon(const QImage& image, int size);

		// creates a new imageframe on a page and inserts the image
		bool insertIntoDocument(ScribusDoc* doc, InsertAFrameData& iafData);
		// inserts the image into a given imageframe
		bool insertIntoImageFrame(ScribusDoc* doc, PageItem* imageFrame);

		//filter attribute. if filter settings match, its set to true and no longer considered for display/sorting
		bool filtered { false };
		//contains fileinformation
		QFileInfo fileInformation;
		//keeps imageinformation
		ImageInformation* imgInfo { nullptr };
		//current size of previewIcon
		int currentSize { 1 };
		//tells whether previewIcon is loading in thread
		bool previewImageLoading { false };
		//tells if previewIcon was created previously or needs to be loaded
		bool previewIconCreated { false };
		//contains previewicon which is to be shown
		QPixmap previewIcon;
		//tags
		QStringList tags;
};


//class for managing a set of PreviewImage objects
class PreviewImages
{
	public:
		//constructor, adds imageFiles to this set
		PreviewImages(const QStringList& imageFiles);

		//creates previewimage objects from a QStringList
		void createPreviewImagesList(const QStringList& imageFiles);
		//creates previewimage objects from ImageCollection
		void createPreviewImagesList(const ImageCollection* collection);
		//deletes all entries
		void clearPreviewImagesList();

		//little helper function to reduce code
		bool toRemove(bool a, bool b);
		//filters by filename, invert specifies whether to include or to remove file fileName
		void filterFileName(const QString& fileName, bool invert);
		//filters by filesize, smallerThan specifies whether to include or to remove files smaller than fileSize
		void filterFileSize(qint64 fileSize, bool smallerThan);
		//filters by filetype, invert specifies whether to include or to remove files with fileExtension
		void filterFileType(const QStringList& types, bool invert);
		//filters by date last modified, smallerThan specifies whether to include or to remove files younger than modified
		void filterFileModified(const QDateTime& modified, bool smallerThan);
		//filters by imageresolution, smallerThan specifies whether to include or to remove images with lower resolution
		void filterResolution(qint64 resolution, bool smallerThan);
		//filters by tag, invert specifies whether to include or to remove item with tag
		void filterTag(const QStringList& tags, bool invert);
		//clears all previously applied filters
		void clearFilters();

		//sorts previewImagesList after sort criteria
		//0: sort by filename
		//1: sort by fileextension
		//2: sort by date last modified
		//3: sort by filesize
		void sortPreviewImages(const int sort);

		//all our precious previewimages are indexed here
		QList<PreviewImage *> previewImagesList;
};


//a model which holds the previewimages and is connected to QListView
class PreviewImagesModel : public QAbstractListModel
{
		Q_OBJECT

	public:
		PreviewImagesModel(QObject* parent = nullptr);
		//saves pointer to calling PictureBrowser instance, for later access
		PreviewImagesModel(PictureBrowser* parent);

		//creates the default icon which is displayed when image is not loaded yet
		//parameters:
		//int size: size of the icon to be created
		void createDefaultIcon(int size);
		//sets the modelitemslist to previewImagesList, ignores filtered images
		void setModelItemsList(const QList<PreviewImage*>& previewImagesList);
		//removes all items
		void clearModelItemsList();

		//returns previewicons and, depending on previewmode, additional text
		QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
		//returns flags for items
		Qt::ItemFlags flags(const QModelIndex& index) const override;
		//returns the number of previewimages
		int rowCount(const QModelIndex& parent) const override;

		QMimeData* mimeData(const QModelIndexList& indexes) const override;
		QStringList mimeTypes() const override;

		//all our precious previewimages are indexed here
		QList<PreviewImage *> modelItemsList;
		//a unique id telling the thread whether to cancel current loading operations because files have changed
		int pId { 0 };

	private slots:
		//called when imageloadingthread has finished
		//parameters:
		//PreviewImage* loadedImage: pointer to identify the loaded image
		//const QImage image: the actual image
		//int tpId: the id the thread has been called from
		void processLoadedImage(int row, const QImage& image, ImageInformation* imgInfo, int tpId);
		void processImageLoadError(int row, int tpId, int errorcode);

	private:
		//pointer to the picturebrowser class
		PictureBrowser *pictureBrowser { nullptr };
		//default icon
		QPixmap defaultIcon;
		//icon size of defaultIcon
		int defaultIconSize { 128 };
	
};

#endif // PREVIEWIMAGE_H
