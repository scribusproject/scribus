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

#include <algorithm>

#include <QPainter>
#include <QRegExp>
#include <QUrl>

#include "previewimage.h"
#include "picturebrowser.h"
#include "collection.h"

#include "iconmanager.h"
#include "util.h"

// //functions for comparing attributes of previewimages (used for sorting)
bool comparePreviewImageFileName ( const PreviewImage *i1, const PreviewImage *i2 );
bool comparePreviewImageFileType ( const PreviewImage *i1, const PreviewImage *i2 );
bool comparePreviewImageFileDate ( const PreviewImage *i1, const PreviewImage *i2 );
bool comparePreviewImageFileSize ( const PreviewImage *i1, const PreviewImage *i2 );
bool comparePreviewImageResolution ( const PreviewImage *i1, const PreviewImage *i2 );

PreviewImage::PreviewImage ( const QString& imageFile )
{
	fileInformation.setFile(imageFile);
	fileInformation.setCaching(true);
}

//nothing to do yet
PreviewImage::~PreviewImage()
{
}

bool PreviewImage::createPreviewIcon(const QImage& image, int size)
{
	QPainter p;
	//width/height of our icon (exclusive 1px border)

	previewIcon = QPixmap(size, size);

	QBrush b(QColor(205, 205, 205), IconManager::instance().loadPixmap("testfill"));

	p.begin(&previewIcon);

	p.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
	p.setBrush(b);
	p.drawRect(0, 0, size - 1, size - 1);

	p.drawImage(((size - image.width()) / 2), ((size - image.height()) / 2), image);

	p.end();

	previewIconCreated = true;
	currentSize = size;

	return true;
}

bool PreviewImage::insertIntoDocument(ScribusDoc* doc, InsertAFrameData& iafData)
{
	//adding imageframe
	doc->itemAddUserFrame(iafData);

	return true;
}

bool PreviewImage::insertIntoImageFrame(ScribusDoc* doc, PageItem* imageFrame)
{
	//inserting image
	return imageFrame->loadImage(fileInformation.absoluteFilePath(), false, -1, true);
}

PreviewImages::PreviewImages(const QStringList& imageFiles)
{
	createPreviewImagesList(imageFiles);
}

void PreviewImages::createPreviewImagesList(const QStringList& imageFiles)
{
	//if there are already previewimages allocated, remove them
	if (!previewImagesList.empty())
		clearPreviewImagesList();

	if (imageFiles.isEmpty())
		return;

	int s = imageFiles.size();
	for (int i = 0; i < s; ++i)
	{
		auto* tmpPreviewImage = new PreviewImage(imageFiles.at(i));
		previewImagesList.append(tmpPreviewImage);
	}
}

void PreviewImages::createPreviewImagesList(const ImageCollection* collection)
{
	//if there are already previewimages allocated, remove them
	if (!previewImagesList.empty())
		clearPreviewImagesList();

	if (collection->imageFiles.isEmpty())
		return;

	int s = collection->imageFiles.size();
	for (int i = 0; i < s; ++i)
	{
		auto* tmpPreviewImage = new PreviewImage(collection->imageFiles.at(i));
		tmpPreviewImage->tags = collection->tags.at(i);
		previewImagesList.append(tmpPreviewImage);
	}
}

void PreviewImages::clearPreviewImagesList()
{
	int s = previewImagesList.size();

	for (int i = 0; i < s; ++i)
		delete previewImagesList.at(i);
	previewImagesList.clear();
}

bool PreviewImages::toRemove(bool a, bool b)
{
	if (a)
	{
		if (!b)
		{
			return true;
		}
	}
	else if (b)
	{
		return true;
	}

	return false;
}

void PreviewImages::filterFileName(const QString& fileName, bool invert)
{
	QRegExp rx(fileName);
	//wildcardmode should be sufficient
	rx.setPatternSyntax(QRegExp::Wildcard);

	for (PreviewImage* tmpPreviewImage : std::as_const(previewImagesList))
	{
		if (toRemove(rx.exactMatch(tmpPreviewImage->fileInformation.fileName()), invert))
			tmpPreviewImage->filtered = true;
	}
}

void PreviewImages::filterFileSize(qint64 fileSize, bool smallerThan)
{
	for (PreviewImage* tmpPreviewImage : std::as_const(previewImagesList))
	{
		if (toRemove((tmpPreviewImage->fileInformation.size() < fileSize), smallerThan))
			tmpPreviewImage->filtered = true;
	}
}

void PreviewImages::filterFileType(const QStringList& types, bool invert)
{
	for (PreviewImage* tmpPreviewImage : std::as_const(previewImagesList))
	{
		QString type = QString("*.") + tmpPreviewImage->fileInformation.suffix();
		if (toRemove(types.contains(type), invert))
			tmpPreviewImage->filtered = true;
	}
}

void PreviewImages::filterFileModified(const QDateTime& modified, bool smallerThan)
{
	for (PreviewImage* tmpPreviewImage : std::as_const(previewImagesList))
	{
		if (toRemove((tmpPreviewImage->fileInformation.lastModified() < modified), smallerThan))
			tmpPreviewImage->filtered = true;
	}
}

//real image needs to be loaded for this info
void PreviewImages::filterResolution(qint64 resolution, bool smallerThan)
{
	for (PreviewImage* tmpPreviewImage : std::as_const(previewImagesList))
	{
		if (!tmpPreviewImage->imgInfo)
			continue;
		int imgResolution = qMin(tmpPreviewImage->imgInfo->xdpi, tmpPreviewImage->imgInfo->ydpi);

		if (toRemove((imgResolution < resolution), smallerThan))
			tmpPreviewImage->filtered = true;
	}
}

void PreviewImages::filterTag(const QStringList& tags, bool invert)
{
	for (PreviewImage* tmpPreviewImage : std::as_const(previewImagesList))
	{
		for (const QString& tag : tags)
		{
			if (toRemove(tmpPreviewImage->tags.contains(tag), invert))
			{
				tmpPreviewImage->filtered = true;
				break;
			}
		}
	}
}

void PreviewImages::clearFilters()
{
	for (PreviewImage* tmpPreviewImage : std::as_const(previewImagesList))
		tmpPreviewImage->filtered = false;
}

void PreviewImages::sortPreviewImages(const int sort)
{
	if (previewImagesList.isEmpty())
		return;

	switch (sort)
	{
		//sort by filename
	case 0:
		std::sort(previewImagesList.begin(), previewImagesList.end(), comparePreviewImageFileName);
		break;

	case 1:
		//sort by filedate
		std::sort(previewImagesList.begin(), previewImagesList.end(), comparePreviewImageFileType);
		break;

	case 2:
		//sort by filetype
		std::sort(previewImagesList.begin(), previewImagesList.end(), comparePreviewImageFileDate);
		break;

	case 3:
		//sort by filesize
		std::sort(previewImagesList.begin(), previewImagesList.end(), comparePreviewImageFileSize);
		break;

	case 4:
		//sort by imageresolution
		std::sort(previewImagesList.begin(), previewImagesList.end(), comparePreviewImageResolution);
		break;

	default:
		break;
		//if a wrong argument was specified, sort by name
		std::sort(previewImagesList.begin(), previewImagesList.end(), comparePreviewImageFileName);
	}
}

bool comparePreviewImageFileName(const PreviewImage* i1, const PreviewImage* i2)
{
	return i1->fileInformation.fileName() < i2->fileInformation.fileName();
}

bool comparePreviewImageFileType(const PreviewImage* i1, const PreviewImage* i2)
{
	return i1->fileInformation.suffix() < i2->fileInformation.suffix();
}

bool comparePreviewImageFileDate(const PreviewImage* i1, const PreviewImage* i2)
{
	return i1->fileInformation.lastModified() < i2->fileInformation.lastModified();
}

bool comparePreviewImageFileSize(const PreviewImage* i1, const PreviewImage* i2)
{
	return i1->fileInformation.size() < i2->fileInformation.size();
}

//real image needs to be loaded for this information!
bool comparePreviewImageResolution(const PreviewImage* i1, const PreviewImage* i2)
{
	//return (resolution) < (resolution);
	return true;
}

PreviewImagesModel::PreviewImagesModel(QObject* parent) : QAbstractListModel(parent)
{
}

PreviewImagesModel::PreviewImagesModel(PictureBrowser* parent) : QAbstractListModel(parent)
{
	pictureBrowser = parent;

	createDefaultIcon(pictureBrowser->pbSettings.previewIconSize);
}

void PreviewImagesModel::createDefaultIcon(int size)
{
	QPainter p;

	defaultIcon = QPixmap(size, size);

	QBrush b(QColor(205, 205, 205), IconManager::instance().loadPixmap("testfill"));

	p.begin(&defaultIcon);

	p.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
	p.setBrush(b);
	p.drawRect(0, 0, size - 1, size - 1);

	p.end();

	defaultIconSize = size;
}

void PreviewImagesModel::setModelItemsList(const QList<PreviewImage*>& previewImagesList)
{
	//create new id
	pId++;

	//if there are already items stored, remove them
	if (!modelItemsList.empty())
		clearModelItemsList();

	pictureBrowser->imagesDisplayed = 0;
	pictureBrowser->imagesFiltered = 0;

	beginInsertRows(QModelIndex(), 0, previewImagesList.size());

	PreviewImage* tmpPreviewImage;

	for (int i = 0; i < previewImagesList.size(); ++i)
	{
		if (!(pictureBrowser->pbSettings.sortOrder))
			tmpPreviewImage = previewImagesList.at(i);
		else
			tmpPreviewImage = previewImagesList.at(previewImagesList.size() - i - 1);

		if (!(tmpPreviewImage->filtered))
		{
			modelItemsList.append(tmpPreviewImage);
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
	beginRemoveRows(QModelIndex(), 0, modelItemsList.size() - 1);

	modelItemsList.clear();

	endRemoveRows();
}

QVariant PreviewImagesModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid())
		return QVariant();

	int row = index.row();
	PreviewImage* tmpImage = modelItemsList.at(row);

	if (role == Qt::DecorationRole)
	{
		pictureBrowser->currentRow = row;

		if (!tmpImage->previewIconCreated)
		{
			if (!tmpImage->previewImageLoading)
			{
				tmpImage->previewImageLoading = true;
				pictureBrowser->callLoadImageThread(row, pId);
			}

			return QIcon(defaultIcon);
		}
		return QIcon(tmpImage->previewIcon);
	}
	if (role == Qt::DisplayRole)
	{
		if (pictureBrowser->pbSettings.previewMode == 1)
			return tmpImage->fileInformation.fileName();
		return QVariant();
	}

	return QVariant();
}

Qt::ItemFlags PreviewImagesModel::flags(const QModelIndex& index) const
{
	if (index.isValid())
		return (Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);

	//Qt modeltest requests 0 here
	return Qt::NoItemFlags;
}

int PreviewImagesModel::rowCount(const QModelIndex& parent) const
{
	if (parent.isValid())
		return 0;
	return modelItemsList.size();
}

QStringList PreviewImagesModel::mimeTypes() const
{
	QStringList types;
	types << "text/uri-list";
	return types;
}

QMimeData* PreviewImagesModel::mimeData(const QModelIndexList& indexes) const
{
	QMimeData* mimeData = new QMimeData();
	QList<QUrl> urls;

	for (const QModelIndex& index : indexes)
	{
		if (!index.isValid())
			continue;

		int row = index.row();
		if ((row < 0) || (row >= modelItemsList.size()))
			continue;

		QString imageFile = modelItemsList.at(row)->fileInformation.absoluteFilePath();
		urls.append(QUrl::fromLocalFile(imageFile));
	}

	mimeData->setUrls(urls);
	return mimeData;
}

void PreviewImagesModel::processLoadedImage(int row, const QImage& image, ImageInformation* imgInfo, int tpId)
{
	//check if list of files has changed and this job is obsolete
	if (tpId != pId)
		return;

	//emit layoutAboutToBeChanged();

	PreviewImage* loadedImage = modelItemsList.at(row);
	loadedImage->previewImageLoading = false;
	loadedImage->createPreviewIcon(image, pictureBrowser->pbSettings.previewIconSize);

	loadedImage->imgInfo = imgInfo;

	//emit layoutChanged();

	QModelIndex changedIndex = index(row);
	dataChanged(changedIndex, changedIndex);
}

void PreviewImagesModel::processImageLoadError(int row, int tpId, int errorcode)
{
	//check if list of files has changed and this job is obsolete
	if (tpId != pId)
		return;

	//emit layoutAboutToBeChanged();

	PreviewImage* loadedImage = modelItemsList.at(row);
	loadedImage->previewImageLoading = false;

	//emit layoutChanged();
}
