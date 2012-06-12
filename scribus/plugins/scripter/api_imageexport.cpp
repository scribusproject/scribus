/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "api_imageexport.h"
#include "utils.h"
#include "util.h"

ImageExport::ImageExport() : QObject(COLLECTOR)
{
	qDebug() << "ImageExport loaded";
	setObjectName("ImageExport");
	_dpi = 72;
	_quality = -1;
	_scale = 100.0;
	_name = QDir::currentPath();
	_type = QString("PNG");
	_overwrite = false;
}

ImageExport::ImageExport(QString dirName, QString type, double scale, double quality, double dpi, bool overwrite)
{
	setDir(dirName);
	setType(type);
	setScale(scale);
	setQuality(quality);
	setDPI(dpi);
	setOverWrite(overwrite);
}

QString ImageExport::getType()
{
	return _type;
}

void ImageExport::setType(QString type)
{
	if (type.isEmpty())
		RAISE("The image type cannot be empty string.");
	_type = type;
}

double ImageExport::getScale()
{
	return _scale;
}

void ImageExport::setScale(double scale)
{
	_scale = scale;
}

double ImageExport::getQuality()
{
	return _quality;
}

void ImageExport::setQuality(double quality)
{
	_quality = quality;
}

double ImageExport::getDPI()
{
	return _dpi;
}

void ImageExport::setDPI(double dpi)
{
	_dpi = dpi;
}

QString ImageExport::getDir()
{
	return _name;
}

void ImageExport::setDir(QString name)
{
	if (name.isEmpty())
	{
		RAISE("The directory name should not be empty string.");
	}
	

	_name = QDir::fromNativeSeparators(name);
	
	QFileInfo fi(_name);
	if(!fi.isDir())
	{
		RAISE("The target location must be an existing directory");
	}
	
	if(!fi.isWritable())
	{
		RAISE("The target location must be writable");
	}
}

bool ImageExport::isOverWrite()
{
	return _overwrite;
}

QString ImageExport::getFileName(ScribusDoc* doc, uint pageNr)
{
	return QDir::cleanPath(QDir::toNativeSeparators(_name + "/" + getFileNameByPage(doc, pageNr, _type.toLower())));
}

bool ImageExport::exportPage(ScribusDoc* doc, uint pageNr, bool single = true)
{
	uint over   = 0;
	bool saved = false, doFileSave = true;
	QString fileName(getFileName(doc, pageNr));

	if (!doc->Pages->at(pageNr))
		return false;
	ScPage* page = doc->Pages->at(pageNr);

	/* a little magic here - I need to compute the "maxGr" value...
	* We need to know the right size of the page for landscape,
	* portrait and user defined sizes.
	*/
	double pixmapSize = (page->height() > page->width()) ? page->height() : page->width();
	QImage im(doc->view()->PageToPixmap(pageNr, qRound(pixmapSize * _scale * (_dpi / 72.0) / 100.0), false));
	if (im.isNull())
	{
		RAISE("Insufficient memory for this image size.");
		return false;
	}
	int dpm = qRound(100.0 / 2.54 * _dpi);
	im.setDotsPerMeterY(dpm);
	im.setDotsPerMeterX(dpm);
	if (QFile::exists(fileName) && !overwrite)
	{
		RAISE("File exists and overwrite is set to false");
	}
	if (doFileSave)
		saved = im.save(fileName, _type.toLocal8Bit().constData(), _quality);
	if (!saved && doFileSave)
	{
		RAISE("Error writing the output file(s).");
	}
	return saved;
}

bool ImageExport::exportInterval(ScribusDoc* doc, std::vector< int > &pageNs)
{
	for (uint a = 0; a < pageNs.size(); ++a)
	{
		if (!exportPage(doc, pageNs[a]-1, false))
			return false;
	}
	return true;
}

void ImageExport::setOverWrite(bool value)
{
	_overwrite = value;
}

ImageExport::~ImageExport()
{
	qDebug() << "ImageExport deleted";
}
