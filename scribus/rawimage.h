/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef RAWIMAGE_H
#define RAWIMAGE_H

#include "scconfig.h"
#include "scribusapi.h"
#include "QByteArray"
#include "QImage"

class SCRIBUS_API RawImage : public QByteArray
{
public:
	RawImage();
	RawImage( int width, int height, int channels);
	~RawImage();
	bool create( int width, int height, int channels);
	int width()  const { return m_width; };
	int height()  const { return m_height; };
	int channels()  const { return m_channels; };
	uchar *bits() const { return (uchar*)data(); };
	uchar *scanLine(int row);
	void setAlpha(int x, int y, int alpha);
	QImage convertToQImage(bool cmyk, bool raw = false);
private:
	int m_width;
	int m_height;
	int m_channels;
};

#endif
