/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef API_IMAGEEXPORT_H_
#define API_IMAGEEXPORT_H_

#include <QObject>
#include <QtDebug>
#include <QApplication>

#include "scripterimpl.h"

class ImageExport : public QObject
{
	Q_OBJECT

public:
	ImageExport();
	ImageExport(QString dirName, QString type, double scale, double quality, double dpi, bool overwrite);
	virtual ~ImageExport();

public slots:

	QString getType();
	void setType(QString name);

	double getScale();
	void setScale(double scale);

	double getQuality();
	void setQuality(double quality);

	double getDPI();
	void setDPI(double dpi);

	QString getDir();
	void setDir(QString name);
	
	bool isOverWrite();
	void setOverWrite(bool value);
	bool exportInterval(ScribusDoc* doc, std::vector< int >& pageNs);

private:
	QString _name; //exportDir
	QString _type; //bitmapType
	double _scale; //enlargement 0 - 100
	double _quality; //quality 0 - 100
	double _dpi; //dpi
	bool _overwrite;
	bool exportPage(ScribusDoc* doc, uint pageNr, bool single);
	QString getFileName(ScribusDoc* doc, uint pageNr);
};

#endif /*API_IMAGEEXPORT_H_*/