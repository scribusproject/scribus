/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef STENCILREADER_H
#define STENCILREADER_H

#include <QDomElement>
#include <QPixmap>

#include "scribusapi.h"
#include "scribusstructs.h"

class SCRIBUS_API StencilReader
{
public:
	StencilReader();
	~StencilReader() {};
	QString createShape(QString data);
	void parseGroup(QDomDocument &data, QDomElement &group, QDomNode &DOC, int groupElemCounter, int &groupElemCounter2, double Dx, double Dy);
	void parseGroupProperties(QDomDocument &data, QDomElement &group, QDomNode &DOC, int &groupElemCounter, double &minXCoor, double &minYCoor, double &maxXCoor, double &maxYCoor, bool &firstCheck);
	double parseUnit(const QString &unit);
	const char * getCoord( const char *ptr, double &number );
	bool parseSVG( const QString &s, FPointArray *ite );
	void svgClosePath(FPointArray *i);
	void svgMoveTo(double x1, double y1);
	void svgLineTo(FPointArray *i, double x1, double y1);
	void svgCurveToCubic(FPointArray *i, double x1, double y1, double x2, double y2, double x3, double y3);
	QPixmap createPreview(QString data);
	void writeDefaults(QDomElement &ob);
	QString createObjects(QString data);
	ColorList PageColors;
	bool FirstM, WasM, PathClosed;
	double CurrX, CurrY, StartX, StartY, Conversion;
	int PathLen;
};
#endif
