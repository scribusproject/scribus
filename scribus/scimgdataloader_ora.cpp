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
#include "scimgdataloader_ora.h"
#include "scpainter.h"
#include "util_formats.h"
#include "third_party/zip/scribus_zip.h"

ScImgDataLoader_ORA::ScImgDataLoader_ORA(void) : ScImgDataLoader()
{
	initSupportedFormatList();
}

void ScImgDataLoader_ORA::initSupportedFormatList(void)
{
	m_supportedFormats.clear();
	m_supportedFormats.append("ora");
}

void ScImgDataLoader_ORA::loadEmbeddedProfile(const QString& fn, int /*page*/)
{
	m_embeddedProfile.resize(0);
	m_profileComponents = 0;
}

bool ScImgDataLoader_ORA::loadPicture(const QString& fn, int /*page*/, int /*res*/, bool thumbnail)
{
	if (!QFile::exists(fn))
		return false;
	initialize();
	m_imageInfoRecord.type = ImageTypeOther;
	m_imageInfoRecord.exifDataValid = false;
	ScZipHandler *uz = new ScZipHandler();
	if (!uz->open(fn))
	{
		delete uz;
		return false;
	}
	if (thumbnail)
	{
		if (uz->contains("Thumbnails/thumbnail.png"))
		{
			QByteArray im;
			if (uz->read("Thumbnails/thumbnail.png", im))
			{
				m_image = QImage::fromData(im);
				if (uz->contains("stack.xml"))
				{
					QByteArray f;
					if (uz->read("stack.xml", f))
					{
						QDomDocument designMapDom;
						if(designMapDom.setContent(f))
						{
							QDomElement docElem = designMapDom.documentElement();
							m_imageInfoRecord.exifInfo.height = docElem.attribute("h", "0").toInt();
							m_imageInfoRecord.exifInfo.width = docElem.attribute("w", "0").toInt();
							m_imageInfoRecord.xres = docElem.attribute("xres", "72").toInt();
							m_imageInfoRecord.yres = docElem.attribute("yres", "72").toInt();
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
		if (uz->contains("stack.xml"))
		{
			QByteArray f;
			if (uz->read("stack.xml", f))
			{
				QDomDocument designMapDom;
				if(designMapDom.setContent(f))
				{
					QDomElement docElem = designMapDom.documentElement();
					for(QDomElement drawPag = docElem.firstChildElement(); !drawPag.isNull(); drawPag = drawPag.nextSiblingElement())
					{
						if (drawPag.tagName() == "stack")
						{
							parseStackXML(drawPag);
						}
					}
					m_imageInfoRecord.exifInfo.height = docElem.attribute("h", "0").toInt();
					m_imageInfoRecord.exifInfo.width = docElem.attribute("w", "0").toInt();
					m_imageInfoRecord.xres = docElem.attribute("xres", "72").toInt();
					m_imageInfoRecord.yres = docElem.attribute("yres", "72").toInt();
				}
				m_image = QImage(m_imageInfoRecord.exifInfo.width, m_imageInfoRecord.exifInfo.height, QImage::Format_ARGB32_Premultiplied);
				if (m_image.isNull())
				{
					uz->close();
					delete uz;
					return false;
				}
				m_image.fill( qRgba(0, 0, 0, 0) );
				ScPainter *painter = new ScPainter(&m_image, m_image.width(), m_image.height(), 1, 0);
				painter->setZoomFactor(1);
				while (OraLayerStack.count() > 0)
				{
					oraLayer layer = OraLayerStack.pop();
					if (layer.visible)
					{
						QByteArray lf;
						if (!uz->read(layer.fileName, lf))
							break;
						QImage img = QImage::fromData(lf).convertToFormat(QImage::Format_ARGB32_Premultiplied);
						painter->save();
						painter->setOpacity(layer.opacity);
						if (layer.compositeOp == "svg:multiply")
							painter->setBlendModeFill(3);
						else if (layer.compositeOp == "svg:screen")
							painter->setBlendModeFill(4);
						else if (layer.compositeOp == "svg:overlay")
							painter->setBlendModeFill(5);
						else if (layer.compositeOp == "svg:darken")
							painter->setBlendModeFill(1);
						else if (layer.compositeOp == "svg:lighten")
							painter->setBlendModeFill(2);
						else if (layer.compositeOp == "svg:color-dodge")
							painter->setBlendModeFill(10);
						else if (layer.compositeOp == "svg:color-burn")
							painter->setBlendModeFill(11);
						else if (layer.compositeOp == "svg:hard-light")
							painter->setBlendModeFill(6);
						else if (layer.compositeOp == "svg:soft-light")
							painter->setBlendModeFill(7);
						else if (layer.compositeOp == "svg:difference")
							painter->setBlendModeFill(8);
						else if (layer.compositeOp == "svg:color")
							painter->setBlendModeFill(14);
						else if (layer.compositeOp == "svg:luminosity")
							painter->setBlendModeFill(15);
						else if (layer.compositeOp == "svg:hue")
							painter->setBlendModeFill(12);
						else if (layer.compositeOp == "svg:saturation")
							painter->setBlendModeFill(13);
						else if (layer.compositeOp == "svg:plus")
							painter->setBlendModeFill(16);
						else if (layer.compositeOp == "svg:dst-in")
							painter->setBlendModeFill(17);
						else if (layer.compositeOp == "svg:dst-out")
							painter->setBlendModeFill(18);
						else
							painter->setBlendModeFill(0);
						painter->translate(layer.x, layer.y);
						painter->drawImage(&img);
						painter->restore();
					}
				}
				delete painter;
				m_image = m_image.convertToFormat(QImage::Format_ARGB32);
				m_image.setDotsPerMeterX ((int) (m_imageInfoRecord.xres / 0.0254));
				m_image.setDotsPerMeterY ((int) (m_imageInfoRecord.yres / 0.0254));
				m_imageInfoRecord.BBoxX = 0;
				m_imageInfoRecord.BBoxH = m_image.height();
				m_imageInfoRecord.colorspace = ColorSpaceRGB;
				m_pixelFormat = Format_BGRA_8;
			}
		}
	}
	uz->close();
	delete uz;
	return true; //TODO: I think this should be false!
}

void ScImgDataLoader_ORA::parseStackXML(QDomElement &elem)
{
	for(QDomElement lay = elem.firstChildElement(); !lay.isNull(); lay = lay.nextSiblingElement())
	{
		if (lay.tagName() == "layer")
		{
			oraLayer layer;
			layer.fileName = lay.attribute("src");
			layer.compositeOp = lay.attribute("composite-op");
			layer.opacity = lay.attribute("opacity", "1.0").toDouble();
			layer.visible = lay.attribute("visibility", "visible") == "visible";
			layer.x = lay.attribute("x", "0").toInt();
			layer.y = lay.attribute("y", "0").toInt();
			OraLayerStack.push(layer);
		}
		else if (lay.tagName() == "stack")
		{
			qDebug() << "Parsing SubStack";
			parseStackXML(lay);
		}
	}
}

bool ScImgDataLoader_ORA::preloadAlphaChannel(const QString& fn, int /*page*/, int res, bool& hasAlpha)
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
