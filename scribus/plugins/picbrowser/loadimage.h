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

#ifndef LOADIMAGE_H
#define LOADIMAGE_H

#include <QImage>
#include <QObject>
#include <QString>
#include <QThread>
#include <QMutex>

class ImageInformation;
class PictureBrowser;
class PreviewImagesModel;

//a thread to load previewimages
class LoadImagesThread : public QThread
{
		Q_OBJECT

	public:
		//sets pointer to calling PictureBrowser object and to the related previewImagesModel
		LoadImagesThread(PictureBrowser* parent, PreviewImagesModel* parentModel);

		//called after the thread has been started
		void run() override;

	private:
		QMutex mutex;
		//contains a pointer to the calling PictureBrowser object
		PictureBrowser *pictureBrowser { nullptr };
		//contains a pointer to the related PreviewImagesModel
		PreviewImagesModel *pModel { nullptr };

	signals:
		//emitted when image was loaded
		//parameters:
		//PreviewImage* loadedImage: pointer to the PreviewImage object which receives the image
		//const QImage image: the actual image
		//int tpId: unique id the thread has been called with
		void imageLoaded(int, const QImage, ImageInformation*, int);
		void imageLoadError(int, int, int);

	private slots:
		//called when an image should be loaded
		//parameters:
		//PreviewImage* loadImage: pointer to the PreviewImage object which should receive the image
		//QString path: path to the image
		//int size: desired size of the icon
		//int tpId: unique id
		void processLoadImageJob(int row, const QString& path, int size, int tpId);
};

#endif // IMAGELOADER_H
