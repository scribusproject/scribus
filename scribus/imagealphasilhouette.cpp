/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "imagealphacontour.h"

#include <QTransform>

#include <functional>

namespace {

QPainterPath silhouetteFromMask(const QImage& image,
	const std::function<bool(QRgb)>& included)
{
	QPainterPath path;
	if (image.isNull())
		return path;
	const QImage sample = image.width() > 512 || image.height() > 512
		? image.scaled(512, 512, Qt::KeepAspectRatio, Qt::SmoothTransformation) : image;
	bool hasForeground = false;
	bool hasBackground = false;
	for (int y = 0; y < sample.height(); ++y)
	{
		int runStart = -1;
		for (int x = 0; x <= sample.width(); ++x)
		{
			const bool foreground = x < sample.width() && included(sample.pixel(x, y));
			if (foreground)
			{
				hasForeground = true;
				if (runStart < 0)
					runStart = x;
			}
			else
			{
				if (x < sample.width())
					hasBackground = true;
				if (runStart >= 0)
				{
					path.addRect(runStart, y, x - runStart, 1);
					runStart = -1;
				}
			}
		}
	}
	if (!hasForeground || !hasBackground)
		return {};
	QTransform sampleToImage;
	sampleToImage.scale(double(image.width()) / sample.width(),
		double(image.height()) / sample.height());
	return sampleToImage.map(path.simplified());
}

}

QPainterPath imageAlphaSilhouette(const QImage& image, int threshold)
{
	if (image.isNull() || !image.hasAlphaChannel() || threshold < 1 || threshold > 255)
		return {};
	return silhouetteFromMask(image, [threshold](QRgb pixel) {
		return qAlpha(pixel) >= threshold;
	});
}

QPainterPath imageLuminanceSilhouette(const QImage& image, int threshold)
{
	if (image.isNull() || threshold < 0 || threshold > 255)
		return {};
	return silhouetteFromMask(image, [threshold](QRgb pixel) {
		return qAlpha(pixel) >= 128 && qGray(pixel) <= threshold;
	});
}

QPainterPath imageContrastSilhouette(const QImage& image, int threshold)
{
	if (image.isNull() || threshold < 1 || threshold > 255)
		return {};
	const QRgb corners[] = {
		image.pixel(0, 0), image.pixel(image.width() - 1, 0),
		image.pixel(0, image.height() - 1),
		image.pixel(image.width() - 1, image.height() - 1)
	};
	int red = 0, green = 0, blue = 0;
	for (QRgb corner : corners)
	{
		red += qRed(corner);
		green += qGreen(corner);
		blue += qBlue(corner);
	}
	red /= 4;
	green /= 4;
	blue /= 4;
	const int squaredThreshold = 3 * threshold * threshold;
	return silhouetteFromMask(image, [red, green, blue, squaredThreshold](QRgb pixel) {
		const int dr = qRed(pixel) - red;
		const int dg = qGreen(pixel) - green;
		const int db = qBlue(pixel) - blue;
		return qAlpha(pixel) >= 128 && dr * dr + dg * dg + db * db >= squaredThreshold;
	});
}
