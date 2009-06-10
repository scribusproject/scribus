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

#include <QMetaType>

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

