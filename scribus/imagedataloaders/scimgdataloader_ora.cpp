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
#include "util_formats.h"

ScImgDataLoader_ORA::ScImgDataLoader_ORA(void) : ScImgDataLoader(),
	m_inSubLayer(0),
	m_layerCount(0)
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
					m_inSubLayer = 0;
					m_layerCount = 0;
					QDomElement docElem = designMapDom.documentElement();
					m_imageInfoRecord.exifInfo.height = docElem.attribute("h", "0").toInt();
					m_imageInfoRecord.exifInfo.width = docElem.attribute("w", "0").toInt();
					m_imageInfoRecord.xres = docElem.attribute("xres", "72").toInt();
					m_imageInfoRecord.yres = docElem.attribute("yres", "72").toInt();
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
					for(QDomElement drawPag = docElem.firstChildElement(); !drawPag.isNull(); drawPag = drawPag.nextSiblingElement())
					{
						if (drawPag.tagName() == "stack")
						{
							parseStackXML(drawPag, painter, uz);
						}
					}
					delete painter;
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

void ScImgDataLoader_ORA::parseStackXML(QDomElement &elem, ScPainter* painter, ScZipHandler *uz)
{
	for(QDomElement lay = elem.lastChildElement(); !lay.isNull(); lay = lay.previousSiblingElement())
	{
		if (lay.tagName() == "layer")
		{
			struct PSDLayer layer;
			QString layerName = lay.attribute("name", QString("layer%1").arg(m_layerCount+1));
			QString compositeOp = lay.attribute("composite-op");
			double opacity = lay.attribute("opacity", "1.0").toDouble();
			int x = lay.attribute("x", "0").toInt();
			int y = lay.attribute("y", "0").toInt();
			bool visible = lay.attribute("visibility", "visible") == "visible";
			QString fileName = lay.attribute("src");
			QImage img = QImage();
			if (!fileName.isEmpty())
			{
				QByteArray lf;
				if (!uz->read(fileName, lf))
					break;
				img = QImage::fromData(lf).convertToFormat(QImage::Format_ARGB32_Premultiplied);
			}
			QImage imt;
			double sx = img.width() / 40.0;
			double sy = img.height() / 40.0;
			imt = sy < sx ?  img.scaled(qRound(img.width() / sx), qRound(img.height() / sx), Qt::IgnoreAspectRatio, Qt::SmoothTransformation) :
				  img.scaled(qRound(img.width() / sy), qRound(img.height() / sy), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
			if (m_inSubLayer == 0)
			{
				if ((m_imageInfoRecord.isRequest) && (m_imageInfoRecord.RequestProps.contains(m_layerCount)))
					opacity = m_imageInfoRecord.RequestProps[m_layerCount].opacity / 255.0;
				if ((m_imageInfoRecord.isRequest) && (m_imageInfoRecord.RequestProps.contains(m_layerCount)))
					visible = m_imageInfoRecord.RequestProps[m_layerCount].visible;
				if ((m_imageInfoRecord.isRequest) && (m_imageInfoRecord.RequestProps.contains(m_layerCount)))
					compositeOp = m_imageInfoRecord.RequestProps[m_layerCount].blend;
				layer.layerName = layerName;
				layer.channelType.clear();
				layer.channelLen.clear();
				layer.opacity = qRound(opacity * 255);
				layer.blend = blendModeToString(compositeOp);
				layer.maskYpos = 0;
				layer.maskXpos = 0;
				layer.maskHeight = 0;
				layer.maskWidth = 0;
				layer.flags = visible ? 0 : 2;
				layer.thumb = imt.copy();
				m_imageInfoRecord.layerInfo.append(layer);
				m_layerCount++;
			}
			if (visible)
			{
				painter->save();
				painter->setOpacity(opacity);
				painter->setBlendModeFill(blendModeToInt(compositeOp));
				painter->translate(x, y);
				painter->drawImage(&img);
				painter->restore();
			}
		}
		else if (lay.tagName() == "stack")
		{
			struct PSDLayer layer;
			QString layerName = lay.attribute("name", QString("layer%1").arg(m_layerCount+1));
			QString compositeOp = lay.attribute("composite-op");
			double opacity = lay.attribute("opacity", "1.0").toDouble();
			int x = lay.attribute("x", "0").toInt();
			int y = lay.attribute("y", "0").toInt();
			bool visible = lay.attribute("visibility", "visible") == "visible";
			if (m_inSubLayer == 0)
			{
				if ((m_imageInfoRecord.isRequest) && (m_imageInfoRecord.RequestProps.contains(m_layerCount)))
					opacity = m_imageInfoRecord.RequestProps[m_layerCount].opacity / 255.0;
				if ((m_imageInfoRecord.isRequest) && (m_imageInfoRecord.RequestProps.contains(m_layerCount)))
					visible = m_imageInfoRecord.RequestProps[m_layerCount].visible;
				if ((m_imageInfoRecord.isRequest) && (m_imageInfoRecord.RequestProps.contains(m_layerCount)))
					compositeOp = m_imageInfoRecord.RequestProps[m_layerCount].blend;
				layer.layerName = layerName;
				layer.channelType.clear();
				layer.channelLen.clear();
				layer.opacity = qRound(opacity * 255);
				layer.blend = blendModeToString(compositeOp);
				layer.maskYpos = 0;
				layer.maskXpos = 0;
				layer.maskHeight = 0;
				layer.maskWidth = 0;
				layer.flags = visible ? 0 : 2;
				m_imageInfoRecord.layerInfo.append(layer);
				m_layerCount++;
			}
			m_inSubLayer++;
			if (visible)
			{
				painter->save();
				painter->beginLayer(opacity, blendModeToInt(compositeOp));
				painter->translate(x, y);
				parseStackXML(lay, painter, uz);
				painter->endLayer();
				painter->restore();
			}
			m_inSubLayer--;
		}
	}
}

int ScImgDataLoader_ORA::blendModeToInt(QString compositeOp)
{
	int ret = 0;
	if (compositeOp == "svg:darken")
		ret = 1;
	else if (compositeOp == "svg:lighten")
		ret = 2;
	else if (compositeOp == "svg:multiply")
		ret = 3;
	else if (compositeOp == "svg:screen")
		ret = 4;
	else if (compositeOp == "svg:overlay")
		ret = 5;
	else if (compositeOp == "svg:hard-light")
		ret = 6;
	else if (compositeOp == "svg:soft-light")
		ret = 7;
	else if (compositeOp == "svg:difference")
		ret = 8;
	else if (compositeOp == "svg:color-dodge")
		ret = 10;
	else if (compositeOp == "svg:color-burn")
		ret = 11;
	else if (compositeOp == "svg:color")
		ret = 14;
	else if (compositeOp == "svg:luminosity")
		ret = 15;
	else if (compositeOp == "svg:hue")
		ret = 12;
	else if (compositeOp == "svg:saturation")
		ret = 13;
	else if (compositeOp == "svg:plus")
		ret = 16;
	else if (compositeOp == "svg:dst-in")
		ret = 17;
	else if (compositeOp == "svg:dst-out")
		ret = 18;
	else if (compositeOp == "norm")
		ret = 0;
	else if (compositeOp == "dark")
		ret = 1;
	else if (compositeOp == "lite")
		ret = 2;
	else if (compositeOp == "mul ")
		ret = 3;
	else if (compositeOp == "scrn")
		ret = 4;
	else if (compositeOp == "over")
		ret = 5;
	else if (compositeOp == "hLit")
		ret = 6;
	else if (compositeOp == "sLit")
		ret = 7;
	else if (compositeOp == "diff")
		ret = 8;
	else if (compositeOp == "div ")
		ret = 10;
	else if (compositeOp == "idiv")
		ret = 11;
	else if (compositeOp == "colr")
		ret = 14;
	else if (compositeOp == "lum ")
		ret = 15;
	else if (compositeOp == "hue ")
		ret = 12;
	else if (compositeOp == "sat ")
		ret = 13;
	else if (compositeOp == "plus")
		ret = 16;
	else if (compositeOp == "dsti")
		ret = 17;
	else if (compositeOp == "dsto")
		ret = 18;
	return ret;
}

QString ScImgDataLoader_ORA::blendModeToString(QString compositeOp)
{
	QString ret = compositeOp;
	if (compositeOp == "svg:darken")
		ret = "dark";
	else if (compositeOp == "svg:lighten")
		ret = "lite";
	else if (compositeOp == "svg:multiply")
		ret = "mul ";
	else if (compositeOp == "svg:screen")
		ret = "scrn";
	else if (compositeOp == "svg:overlay")
		ret = "over";
	else if (compositeOp == "svg:hard-light")
		ret = "hLit";
	else if (compositeOp == "svg:soft-light")
		ret = "sLit";
	else if (compositeOp == "svg:difference")
		ret = "diff";
	else if (compositeOp == "svg:color-dodge")
		ret = "div ";
	else if (compositeOp == "svg:color-burn")
		ret = "idiv";
	else if (compositeOp == "svg:color")
		ret = "colr";
	else if (compositeOp == "svg:luminosity")
		ret = "lum ";
	else if (compositeOp == "svg:hue")
		ret = "hue ";
	else if (compositeOp == "svg:saturation")
		ret = "sat ";
	else if (compositeOp == "svg:plus")
		ret = "plus";
	else if (compositeOp == "svg:dst-in")
		ret = "dsti";
	else if (compositeOp == "svg:dst-out")
		ret = "dsto";
	return ret;
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
