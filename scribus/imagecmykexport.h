/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef IMAGECMYKEXPORT_H
#define IMAGECMYKEXPORT_H

#include <QByteArray>
#include <QImage>
#include <QString>

// The ARGB32 channels contain C, M, Y and K respectively, as produced by
// ScImage::OutputProfile. Never overwrites an existing destination.
bool writeCMYKTiffCopy(const QString& destination, const QImage& cmyk,
	const QByteArray& iccProfile, int xResolution, int yResolution, QString* error = nullptr);

#endif
