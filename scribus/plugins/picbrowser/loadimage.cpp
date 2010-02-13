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


#include "loadimage.h"
#include "previewimage.h"
#include "picturebrowser.h"

#include "fileloader.h"
#include "loadsaveplugin.h"
#include "../../plugins/formatidlist.h"
#include "scimagecachemanager.h"

#include <QMetaType>

loadImagesThread::loadImagesThread ( PictureBrowser *parent, PreviewImagesModel *parentModel )
{
	mutex.lock();
	pictureBrowser = parent;
	pModel = parentModel;
	mutex.unlock();
}


void loadImagesThread::run()
{
//	loadImagesThreadInstance help;

//	help.pictureBrowser = pictureBrowser;
//	help.pModel = pModel;

//register types for slots and signals
	qRegisterMetaType<previewImage *> ( "previewImage*" );
	qRegisterMetaType<ImageInformation *> ( "ImageInformation*" );
	qRegisterMetaType<QImage> ( "QImage" );

//	connect(&help, SIGNAL(imageLoaded(int, const QImage, ImageInformation*, int) ), pModel, SLOT(processLoadedImage(int, const QImage, ImageInformation*, int)), Qt::QueuedConnection);
//	connect(&help, SIGNAL(imageLoadError(int, int, int)), pModel, SLOT(processImageLoadError(int, int, int)), Qt::QueuedConnection);
//	connect(pictureBrowser, SIGNAL(loadImageJob(int, QString, int, int)), &help, SLOT(processLoadImageJob(int, QString, int, int)), Qt::QueuedConnection);

	connect(this, SIGNAL(imageLoaded(int, const QImage, ImageInformation*, int) ), pModel, SLOT(processLoadedImage(int, const QImage, ImageInformation*, int)), Qt::QueuedConnection);
	connect(this, SIGNAL(imageLoadError(int, int, int)), pModel, SLOT(processImageLoadError(int, int, int)), Qt::QueuedConnection);
	connect(pictureBrowser, SIGNAL(loadImageJob(int, QString, int, int)), this, SLOT(processLoadImageJob(int, QString, int, int)), Qt::QueuedConnection);
	exec();
}

/*
loadImagesThreadInstance::loadImagesThreadInstance()
{
}
*/
void loadImagesThread::processLoadImageJob ( int row, QString path, int size, int tpId )
{
	ScImageCacheManager &icm = ScImageCacheManager::instance();
	bool cacheEnabled = icm.enabled();
	icm.setEnabled(false);
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

	QFileInfo fi = QFileInfo(path);
	QString ext = fi.suffix().toLower();
	QStringList allFormatsV = LoadSavePlugin::getExtensionsForPreview(FORMATID_ODGIMPORT);
	if (allFormatsV.contains(ext.toUtf8()))
	{
		FileLoader *fileLoader = new FileLoader(path);
		int testResult = fileLoader->TestFile();
		delete fileLoader;
		if ((testResult != -1) && (testResult >= FORMATID_ODGIMPORT))
		{
			const FileFormat * fmt = LoadSavePlugin::getFormatById(testResult);
			if( fmt )
			{
				QImage im = fmt->readThumbnail(path);
				if (!im.isNull())
				{
					ImageInformation *imgInfo = new ImageInformation;
					( *imgInfo ).width = im.text("XSize").toDouble();
					( *imgInfo ).height = im.text("YSize").toDouble();
					( *imgInfo ).type = 6;
					( *imgInfo ).colorspace = 0;
					( *imgInfo ).xdpi = 72;
					( *imgInfo ).ydpi = 72;
					( *imgInfo ).layers = 0;
					( *imgInfo ).embedded = false;
					( *imgInfo ).profileName = "";
					( *imgInfo ).valid = true;

					if ( ( im.width() > ( size-2 ) ) || ( im.height() > ( size-2 ) ) )
					{
						emit imageLoaded ( row, im.scaled ( ( size-2 ), ( size-2 ), Qt::KeepAspectRatio, Qt::SmoothTransformation ), imgInfo, tpId );
					}
					//image is <= our icon -> put it in as it is
					else
					{
						emit imageLoaded ( row, im.copy(), imgInfo, tpId );
					}
				}
			}
		}
		icm.setEnabled(cacheEnabled);
		return;
	}

	ScImage image;
//no realCMYK
	bool mode=false;
//no document needs to be assigned to this
	CMSettings cms ( 0, "", Intent_Perceptual);
	cms.allowColorManagement(false);
	cms.setUseEmbeddedProfile(true);

	ImageInformation *imgInfo = new ImageInformation;

	//load previewimage
	if ( image.loadPicture ( path, 1, cms, ScImage::Thumbnail, 72, &mode ) )
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
	icm.setEnabled(cacheEnabled);
}

