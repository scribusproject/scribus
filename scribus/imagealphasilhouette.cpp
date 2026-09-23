/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "imagealphacontour.h"

#include <QTransform>

QPainterPath imageAlphaSilhouette(const QImage& image, int threshold)
{
	QPainterPath path;
	if (image.isNull() || !image.hasAlphaChannel() || threshold < 1 || threshold > 255)
		return path;
	const QImage sample = image.width() > 512 || image.height() > 512
		? image.scaled(512, 512, Qt::KeepAspectRatio, Qt::SmoothTransformation) : image;
	bool hasOpaque = false;
	bool hasTransparent = false;
	for (int y = 0; y < sample.height(); ++y)
	{
		int runStart = -1;
		for (int x = 0; x <= sample.width(); ++x)
		{
			const bool opaque = x < sample.width() && qAlpha(sample.pixel(x, y)) >= threshold;
			if (opaque)
			{
				hasOpaque = true;
				if (runStart < 0)
					runStart = x;
			}
			else
			{
				if (x < sample.width())
					hasTransparent = true;
				if (runStart >= 0)
				{
					path.addRect(runStart, y, x - runStart, 1);
					runStart = -1;
				}
			}
		}
	}
	if (!hasOpaque || !hasTransparent)
		return {};
	QTransform sampleToImage;
	sampleToImage.scale(double(image.width()) / sample.width(),
		double(image.height()) / sample.height());
	return sampleToImage.map(path.simplified());
}
