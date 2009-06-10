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

#ifndef IMAGELOADER_H
#define IMAGELOADER_H


#include <QImage>
#include <QObject>
#include <QString>
#include <QThread>

class ImageInformation;
class PictureBrowser;
class PreviewImagesModel;

//a thread to load previewimages
class loadImagesThread : public QThread
{
		Q_OBJECT

	public:
		//sets pointer to calling PictureBrowser object and to the related previewImagesModel
		loadImagesThread ( PictureBrowser *parent, PreviewImagesModel *parentModel );

		//called after the thread has been started
		void run();

	private:
		//contains a pointer to the calling PictureBrowser object
		PictureBrowser *pictureBrowser;
		//contains a pointer to the related PreviewImagesModel
		PreviewImagesModel *pModel;
};


//a helper class to get the signals/slots executed in the right thread
class loadImagesThreadInstance : public QObject
{
		Q_OBJECT

	public:
		loadImagesThreadInstance();

		//contains a pointer to the calling PictureBrowser object
		PictureBrowser *pictureBrowser;
		//contains a pointer to the related PreviewImagesModel
		PreviewImagesModel *pModel;

	signals:
		//emitted when image was loaded
		//parameters:
		//previewImage* loadedImage: pointer to the previewImage object which receives the image
		//const QImage image: the actual image
		//int tpId: unique id the thread has been called with
		void imageLoaded ( int, const QImage, ImageInformation*, int );
		void imageLoadError ( int, int, int );

	private slots:
		//called when an image should be loaded
		//parameters:
		//previewImage* loadImage: pointer to the previewImage object which should receive the image
		//QString path: path to the image
		//int size: desired size of the icon
		//int tpId: unique id
		void processLoadImageJob ( int row, QString path, int size, int tpId );
};




#endif // IMAGELOADER_H
