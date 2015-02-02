/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QFile>
#include <QList>
#include <QDebug>
#include <QVector>

#include <cmath>
#include <cstdlib>

#include <stdio.h>
#include "scimgdataloader_wpg.h"
#include "third_party/wpg/WPGStreamImplementation.h"

ScrPainterIm::ScrPainterIm(): libwpg::WPGPaintInterface()
{
}

void ScrPainterIm::startGraphics(double width, double height)
{
	currentPen = QPen(Qt::black, 1);
	currentBrush = Qt::black;
	Coords = QPainterPath();
	*image = QImage(qRound(width * 72), qRound(height * 72), QImage::Format_ARGB32);
	image->fill(qRgba(255, 255, 255, 255));
	imagePainter.begin(image);
	imagePainter.setRenderHint(QPainter::Antialiasing , true);
	fillrule = true;
	gradientAngle = 0.0;
	isGradient = false;
	fillSet = false;
	strokeSet = false;
	currentGradient = QLinearGradient();
//	qDebug() << "Start Graphics" << qRound(width * 72) << qRound(height * 72);
}

void ScrPainterIm::endGraphics()
{
	imagePainter.end();
//	qDebug() << "End Graphics";
}

void ScrPainterIm::startLayer(unsigned int id)
{
}

void ScrPainterIm::endLayer(unsigned int id)
{
}

void ScrPainterIm::setPen(const libwpg::WPGPen& pen)
{
	QVector<double> dashArray;
	dashArray.clear();
	Qt::PenJoinStyle lineJoin;
	Qt::PenCapStyle lineEnd;
	double LineW = 72 * pen.width;
	QColor foreColor = QColor(pen.foreColor.red, pen.foreColor.green, pen.foreColor.blue, 255 - pen.foreColor.alpha);
	if(!pen.solid)
	{
		dashArray.clear();
		for(unsigned i = 0; i < pen.dashArray.count(); i++)
		{
			dashArray.append(pen.dashArray.at(i)*LineW);
		}
	}
	switch (pen.joinstyle)
	{
		case 1:
			lineJoin = Qt::BevelJoin;
			break;
		case 2:
			lineJoin = Qt::MiterJoin;
			break;
		case 3:
			lineJoin = Qt::RoundJoin;
			break;
		default:
			lineJoin = Qt::MiterJoin;
			break;
	}
	switch (pen.capstyle)
	{
		case 0:
			lineEnd = Qt::FlatCap;
			break;
		case 1:
			lineEnd = Qt::RoundCap;
			break;
		case 2:
			lineEnd = Qt::SquareCap;
			break;
		default:
			lineEnd = Qt::FlatCap;
			break;
	}
	if(!pen.solid)
	{
		currentPen = QPen(foreColor, LineW, Qt::SolidLine, lineEnd, lineJoin);
		currentPen.setDashPattern(dashArray);
	}
	else
		currentPen = QPen(foreColor, LineW, Qt::SolidLine, lineEnd, lineJoin);
	strokeSet = true;
//	qDebug() << "Set Pen" << pen.foreColor.red << pen.foreColor.green << pen.foreColor.blue << pen.foreColor.alpha;
}

void ScrPainterIm::setBrush(const libwpg::WPGBrush& brush)
{
	isGradient = false;
	if(brush.style == libwpg::WPGBrush::Solid)
	{
		currentBrush = QColor(brush.foreColor.red, brush.foreColor.green, brush.foreColor.blue, 255 - brush.foreColor.alpha);
	}
	else if (brush.style == libwpg::WPGBrush::Gradient)
	{
		gradientAngle = brush.gradient.angle();
		isGradient = true;
		currentGradient = QLinearGradient();
		for(unsigned c = 0; c < brush.gradient.count(); c++)
		{
			QColor stopC = QColor(brush.gradient.stopColor(c).red, brush.gradient.stopColor(c).green, brush.gradient.stopColor(c).blue);
			double pos = qBound(0.0, fabs(brush.gradient.stopOffset(c)), 1.0);
			currentGradient.setColorAt(pos, stopC);
		}
	}
	else if (brush.style == libwpg::WPGBrush::NoBrush)
		currentBrush = QColor(Qt::NoBrush);
	fillSet = true;
}

void ScrPainterIm::setFillRule(FillRule rule)
{
	if(rule == libwpg::WPGPaintInterface::WindingFill)
		fillrule = false;
	else
		fillrule = true;
//	qDebug() << "Fill Rule " << fillrule;
}

void ScrPainterIm::drawRectangle(const libwpg::WPGRect& rect, double rx, double ry)
{
	imagePainter.setBrush(currentBrush);
	imagePainter.setPen(currentPen);
	QRectF rr = QRectF(rect.x1 * 72, rect.y1 * 72, rect.width() * 72, rect.height() * 72);
	if (isGradient)
	{
		QTransform m1;
		m1.rotate(-gradientAngle);
		currentGradient.setStart(rr.x(), rr.y());
		QPointF target = m1.map(QPointF(rr.x(), rr.bottom()));
		currentGradient.setFinalStop(target);
		imagePainter.setBrush(currentGradient);
	}
	if ((rx > 0) || (ry > 0))
		imagePainter.drawRoundedRect(rr, 72 * rx, 72 * ry);
	else
		imagePainter.drawRect(rr);
//	qDebug() << "Draw Rect " << rr;
}

void ScrPainterIm::drawEllipse(const libwpg::WPGPoint& center, double rx, double ry)
{
	imagePainter.setBrush(currentBrush);
	imagePainter.setPen(currentPen);
	QRectF rr = QRectF(0, 0, rx * 144.0, ry * 144.0);
	if (isGradient)
	{
		QTransform m1;
		m1.rotate(-gradientAngle);
		currentGradient.setStart(rr.x(), rr.y());
		QPointF target = m1.map(QPointF(rr.x(), rr.bottom()));
		currentGradient.setFinalStop(target);
		imagePainter.setBrush(currentGradient);
	}
	rr.translate(72*(center.x - rx), 72*(center.y - ry));
	imagePainter.drawEllipse(rr);
//	qDebug() << "Draw Ellipse" << rr;
}

void ScrPainterIm::drawPolygon(const libwpg::WPGPointArray& vertices, bool closed)
{
	if(vertices.count() < 2)
		return;
	imagePainter.setBrush(currentBrush);
	imagePainter.setPen(currentPen);
	Coords = QPainterPath();
	Coords.moveTo(72 * vertices[0].x, 72 * vertices[0].y);
	for(unsigned i = 1; i < vertices.count(); i++)
	{
		Coords.lineTo(72 * vertices[i].x, 72 * vertices[i].y);
	}
	if (isGradient)
	{
		QRectF rr = Coords.boundingRect();
		QTransform m1;
		m1.rotate(-gradientAngle);
		currentGradient.setStart(rr.x(), rr.y());
		QPointF target = m1.map(QPointF(rr.x(), rr.bottom()));
		currentGradient.setFinalStop(target);
		imagePainter.setBrush(currentGradient);
	}
	if (!closed)
		imagePainter.setBrush(Qt::NoBrush);
	else
		Coords.closeSubpath();
	imagePainter.drawPath(Coords);
//	qDebug() << "Draw Polygon";
}

void ScrPainterIm::drawPath(const libwpg::WPGPath& path)
{
	Coords = QPainterPath();
	for(unsigned i = 0; i < path.count(); i++)
	{
		libwpg::WPGPathElement element = path.element(i);
		libwpg::WPGPoint point = element.point;
		switch(element.type)
		{
			case libwpg::WPGPathElement::MoveToElement:
				Coords.moveTo(72 * point.x, 72 * point.y);
				break;
			case libwpg::WPGPathElement::LineToElement:
				Coords.lineTo(72 * point.x, 72 * point.y);
				break;
			case libwpg::WPGPathElement::CurveToElement:
				Coords.cubicTo(72*element.extra1.x, 72*element.extra1.y, 72*element.extra2.x, 72*element.extra2.y, 72 * point.x, 72 * point.y);
				break;
			default:
				break;
		}
	}
	if (!path.filled)
		imagePainter.setBrush(Qt::NoBrush);
	else
	{
		if (isGradient)
		{
			QRectF rr = Coords.boundingRect();
			QTransform m1;
			m1.rotate(-gradientAngle);
			currentGradient.setStart(rr.x(), rr.y());
			QPointF target = m1.map(QPointF(rr.x(), rr.bottom()));
			currentGradient.setFinalStop(target);
			imagePainter.setBrush(currentGradient);
		}
		else
			imagePainter.setBrush(currentBrush);
	}
	if (!path.framed)
			imagePainter.setPen(Qt::NoPen);
		else
			imagePainter.setPen(currentPen);
	if(path.closed)
		Coords.closeSubpath();
	imagePainter.drawPath(Coords);
//	qDebug() << "Draw Path" << currentPen << currentBrush;
}

void ScrPainterIm::drawBitmap(const libwpg::WPGBitmap& bitmap, double hres, double vres)
{
	QImage img = QImage(bitmap.width(), bitmap.height(), QImage::Format_RGB32);
	for(int x = 0; x < bitmap.width(); x++)
	{
		for(int y = 0; y < bitmap.height(); y++)
		{
			libwpg::WPGColor color = bitmap.pixel(x, y);
			img.setPixel(x, y, qRgb(color.red, color.green, color.blue));
		}
	}
	double w = (bitmap.rect.x2 - bitmap.rect.x1) * 72.0;
	double h = (bitmap.rect.y2 - bitmap.rect.y1) * 72.0;
	imagePainter.drawImage(QRectF(bitmap.rect.x1 * 72, bitmap.rect.y1 * 72, w, h), img);
//	qDebug() << "Draw Bitmap";
}

void ScrPainterIm::drawImageObject(const libwpg::WPGBinaryData& /*binaryData*/)
{
//	qDebug() << "drawBinaryData";
}


ScImgDataLoader_WPG::ScImgDataLoader_WPG(void) : ScImgDataLoader()
{
	initSupportedFormatList();
}

void ScImgDataLoader_WPG::initSupportedFormatList(void)
{
	m_supportedFormats.clear();
	m_supportedFormats.append( "wpg" );
}

bool ScImgDataLoader_WPG::preloadAlphaChannel(const QString& fn, int /*page*/, int res, bool& hasAlpha)
{
	initialize();
	hasAlpha = false;
	return true;
}

void ScImgDataLoader_WPG::loadEmbeddedProfile(const QString& fn, int /*page*/)
{
	m_embeddedProfile.resize(0);
	m_profileComponents = 0;
}

bool ScImgDataLoader_WPG::loadPicture(const QString& fn, int /*page*/, int /*res*/, bool /*thumbnail*/)
{
	QFile file(fn);
	if ( !file.exists() )
	{
		qDebug() << "File " << QFile::encodeName(fn).data() << " does not exist" << endl;
		return false;
	}
	if ( !file.open( QIODevice::ReadOnly ) )
	{
		qDebug() << "Cannot open file " << QFile::encodeName(fn).data() << endl;
		return false;
	}
	QByteArray ba = file.readAll();
	file.close();
	initialize();

	libwpg::WPGMemoryStream input(ba.constData(), ba.size());
	if (!libwpg::WPGraphics::isSupported(&input))
	{
		fprintf(stderr, "ERROR: Unsupported file format (unsupported version) or file is encrypted!\n");
		return false;
	}
	ScrPainterIm painter;
	m_image = QImage();
	painter.image = &m_image;
	libwpg::WPGraphics::parse(&input, &painter);
	m_imageInfoRecord.type = ImageTypeOther;
	m_imageInfoRecord.exifDataValid = false;
	m_imageInfoRecord.xres = 72;
	m_imageInfoRecord.yres = 72;
	m_imageInfoRecord.BBoxX = 0;
	m_imageInfoRecord.colorspace = ColorSpaceRGB;
	m_imageInfoRecord.BBoxH = m_image.height();
	m_pixelFormat = Format_BGRA_8;
	return true;
}
