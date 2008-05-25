/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QFile>
#include <QFileInfo>
#include <QByteArray>
#include <QImageReader>
#include <QList>
#include "scimgdataloader_qt.h"

ScImgDataLoader_QT::ScImgDataLoader_QT(void) : ScImgDataLoader()
{
	initSupportedFormatList();
}

void ScImgDataLoader_QT::initSupportedFormatList(void)
{
	m_supportedFormats.clear();
	QList<QByteArray> fmtList = QImageReader::supportedImageFormats();
	for (int i = 0; i < fmtList.count(); i++)
		m_supportedFormats.append( fmtList[i].toLower() );
}

void ScImgDataLoader_QT::loadEmbeddedProfile(const QString& fn, int /*page*/)
{
	m_embeddedProfile.resize(0);
	m_profileComponents = 0;
}

bool ScImgDataLoader_QT::loadPicture(const QString& fn, int /*page*/, int /*res*/, bool /*thumbnail*/)
{
	if (!QFile::exists(fn))
		return false;
	initialize();
	if (m_image.load(fn))
	{
		m_imageInfoRecord.type = ImageTypeOther;
		m_imageInfoRecord.exifDataValid = false;
		float xres = m_image.dotsPerMeterX() * 0.0254;
		float yres = m_image.dotsPerMeterY() * 0.0254;
		int resInf = m_imageInfoRecord.lowResType;
		m_image = m_image.convertToFormat(QImage::Format_ARGB32);
		m_image.setDotsPerMeterX (qMax(2834, (int) (xres / 0.0254)));
		m_image.setDotsPerMeterY (qMax(2834, (int) (yres / 0.0254)));
		m_imageInfoRecord.colorspace = ColorSpaceRGB;
		m_imageInfoRecord.xres = qMax(72, qRound(xres));
		m_imageInfoRecord.yres = qMax(72, qRound(yres));
		m_imageInfoRecord.lowResType = resInf;
		m_imageInfoRecord.BBoxX = 0;
		m_imageInfoRecord.BBoxH = m_image.height();
		return true;
	}
	return false; //TODO: I think this should be false!
}

bool ScImgDataLoader_QT::preloadAlphaChannel(const QString& fn, int /*page*/, int res, bool& hasAlpha)
{
	initialize();
	hasAlpha = false;
	QFileInfo fi = QFileInfo(fn);
	if (!fi.exists())
		return false;
	QString ext = fi.suffix().toLower();
	if ((ext == "jpg") || (ext == "jpeg"))
	{
		hasAlpha = false;
		return true;
	}
	if (m_image.load(fn))
	{
		hasAlpha = m_image.hasAlphaChannel();
		if (m_image.hasAlphaChannel())
			m_image = m_image.convertToFormat(QImage::Format_ARGB32);
		else
			m_image = QImage(); // Discard data immediately
		return true;
	}
	return false;
}
