/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QFile>
#include <QFileInfo>
#include <QByteArray>
#include <QList>
#include "scimgdataloader_kra.h"
#include "util_formats.h"

ScImgDataLoader_KRA::ScImgDataLoader_KRA(void) : ScImgDataLoader()
{
	initSupportedFormatList();
}

void ScImgDataLoader_KRA::initSupportedFormatList(void)
{
	m_supportedFormats.clear();
        m_supportedFormats.append("kra");
}

void ScImgDataLoader_KRA::loadEmbeddedProfile(const QString& fn, int /*page*/)
{
	m_embeddedProfile.resize(0);
	m_profileComponents = 0;
}

bool ScImgDataLoader_KRA::loadPicture(const QString& fn, int /*page*/, int /*res*/, bool thumbnail)
{
	if (!QFile::exists(fn))
		return false;
	bool valid = m_imageInfoRecord.isRequest;
	QMap<int, ImageLoadRequest> req = m_imageInfoRecord.RequestProps;
	initialize();
	m_imageInfoRecord.RequestProps = req;
	m_imageInfoRecord.isRequest = valid;
	m_imageInfoRecord.type = ImageTypeOther;
	m_imageInfoRecord.exifDataValid = false;
	m_imageInfoRecord.layerInfo.clear();
	m_imageInfoRecord.PDSpathData.clear();
	ScZipHandler *uz = new ScZipHandler();

    if (!uz->open(fn))
	{
		delete uz;
		return false;
	}

    // if thumbnail is set to true, this function is loading the thumbnail image into the preview area
    // if thumbnail is set to false, this function is loading the main image

	if (thumbnail)
	{
        if (uz->contains("preview.png"))
		{
            QByteArray im;

            if (uz->read("preview.png", im))
			{
                m_image = QImage::fromData(im);

                if (uz->contains("maindoc.xml"))
				{
					QByteArray f;
                    if (uz->read("maindoc.xml", f))
					{
						QDomDocument designMapDom;
						if(designMapDom.setContent(f))
                        {
                            QDomElement docElem = designMapDom.documentElement().firstChildElement("IMAGE");
                            m_imageInfoRecord.exifInfo.height = docElem.attribute("height", "0").toInt();
                            m_imageInfoRecord.exifInfo.width = docElem.attribute("width", "0").toInt();

                            // sanitize values for x-res and y-res. some older versions of Krita had resolutions like "76,0000023"
                            QString xResClean = docElem.attribute("x-res", "72").replace(",", ".");
                            QString yResClean = docElem.attribute("y-res", "72").replace(",", ".");

                            // strip out all decimal stuff since we cannot have that.
                            xResClean = xResClean.split(".").at(0);
                            yResClean = yResClean.split(".").at(0);

                            m_imageInfoRecord.xres = xResClean.toInt();
                            m_imageInfoRecord.yres = yResClean.toInt();
						}
					}
				}
				m_imageInfoRecord.exifDataValid = true;
				m_imageInfoRecord.exifInfo.thumbnail = m_image;
			}
			else
			{

                delete uz;
				return false;
			}
		}
	}
	else
	{
        if (uz->contains("maindoc.xml"))
		{
			QByteArray f;
            if (uz->read("maindoc.xml", f))
			{
                QDomDocument designMapDom;
				if(designMapDom.setContent(f))
				{
                    QDomElement docElem = designMapDom.documentElement().firstChildElement("IMAGE");
                    m_imageInfoRecord.exifInfo.height = docElem.attribute("height", "0").toInt();
                    m_imageInfoRecord.exifInfo.width = docElem.attribute("width", "0").toInt();



                    // sanitize values for x-res and y-res. some older versions of Krita had resolutions like "76,0000023"
                    QString xResClean = docElem.attribute("x-res", "72").replace(",", ".");
                    QString yResClean = docElem.attribute("y-res", "72").replace(",", ".");

                    // strip out all decimal stuff since we cannot have that.
                    xResClean = xResClean.split(".").at(0);
                    yResClean = yResClean.split(".").at(0);


                    m_imageInfoRecord.xres = xResClean.toInt();
                    m_imageInfoRecord.yres = yResClean.toInt();


					m_image = QImage(m_imageInfoRecord.exifInfo.width, m_imageInfoRecord.exifInfo.height, QImage::Format_ARGB32_Premultiplied);
					if (m_image.isNull())
					{
						uz->close();
						delete uz;
						return false;
					}

                    // load the mergedimage.png into the painter
                    if (uz->contains("mergedimage.png"))
                    {
                        QByteArray im;

                        if (uz->read("mergedimage.png", im))
                        {
                            m_image.fill( qRgba(0, 0, 0, 0) );
                            m_image = QImage::fromData(im);
                        }
                    }

                    m_image = m_image.convertToFormat(QImage::Format_ARGB32);
					m_imageInfoRecord.valid = true;
					m_image.setDotsPerMeterX((int) (m_imageInfoRecord.xres / 0.0254));
					m_image.setDotsPerMeterY((int) (m_imageInfoRecord.yres / 0.0254));
					m_imageInfoRecord.BBoxX = 0;
					m_imageInfoRecord.BBoxH = m_image.height();                    
					m_imageInfoRecord.colorspace = ColorSpaceRGB;
					m_pixelFormat = Format_BGRA_8;
				}
			}
		}
	}
	uz->close();
	delete uz;
	return true; //TODO: I think this should be false!
}

bool ScImgDataLoader_KRA::preloadAlphaChannel(const QString& fn, int /*page*/, int res, bool& hasAlpha)
{
	hasAlpha = false;
	if (loadPicture(fn, 0, 0, false))
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
