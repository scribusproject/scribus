/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef IMAGEALPHACONTOUR_H
#define IMAGEALPHACONTOUR_H

#include <QImage>
#include <QPainterPath>
#include <QString>

#include "scribusapi.h"

class PageItem;

// Pixel-coordinate silhouette. Empty for opaque, transparent or invalid input.
QPainterPath imageAlphaSilhouette(const QImage& image, int threshold = 128);

// Replace the frame's editable contour without altering the linked image.
// When enableWrap is true, contour text flow is enabled in the same undo step.
SCRIBUS_API bool generateImageAlphaContour(PageItem* item, int threshold,
	double padding, bool enableWrap, QString* error = nullptr);

#endif
