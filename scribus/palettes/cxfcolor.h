/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef CXFCOLOR_H
#define CXFCOLOR_H

#include <QDomDocument>
#include <QSharedPointer>
#include <QVector>

#include "cxftypes.h"

class  CxfColorSpecification;
class  CxfDocument;
struct ScLab;

class CxfColor
{
public:
	CxfColor(CxfDocument* cxfDoc);
	virtual ~CxfColor() {}

	virtual bool isValid() const;
	virtual CxfColorType type() const { return cxfColorUnknown; }

	const CxfColorSpecification* colorSpecification() const { return m_colorSpec; }
	const CxfDocument* document() const { return m_cxfDoc; }
	
	virtual bool parse(QDomElement& colorElem) = 0;
	virtual void reset();
	
protected:
	CxfDocument* m_cxfDoc;
	const CxfColorSpecification* m_colorSpec;
};

typedef QSharedPointer<CxfColor> CxfColorShPtr;

class CxfColorRGB : public CxfColor
{
public:
	CxfColorRGB(CxfDocument* cxfDoc);
	
	double  maxRange() const { return m_maxRange; }
	
	double  red() const   { return m_values[0]; }
	double  green() const { return m_values[1]; }
	double  blue() const  { return m_values[2]; }
	
	virtual CxfColorType type() const { return cxfColorRGB; }

	virtual bool parse(QDomElement& colorElem);
	virtual void reset();
	
protected:
	double m_maxRange;
	double m_values[3];
};

class CxfColorSRGB : public CxfColorRGB
{
public:
	CxfColorSRGB(CxfDocument* cxfDoc);

	virtual CxfColorType type() const { return cxfColorSRGB; }
};

class CxfColorAdobeRGB : public CxfColorRGB
{
public:
	CxfColorAdobeRGB(CxfDocument* cxfDoc);

	virtual CxfColorType type() const { return cxfColorAdobeRGB; }
};

class CxfColorHTML : public CxfColorRGB
{
public:
	CxfColorHTML(CxfDocument* cxfDoc);

	virtual CxfColorType type() const { return cxfColorHTML; }
	virtual bool parse(QDomElement& colorElem);
};

class CxfColorCMYK : public CxfColor
{
public:
	CxfColorCMYK(CxfDocument* cxfDoc);

	double  cyan() const    { return m_values[0]; }
	double  magenta() const { return m_values[1]; }
	double  yellow() const  { return m_values[2]; }
	double  black() const   { return m_values[3]; }

	virtual CxfColorType type() const { return cxfColorCMYK; }

	virtual bool parse(QDomElement& colorElem);
	virtual void reset();

protected:
	double m_values[4];
};

class CxfColorCIELab : public CxfColor
{
public:
	CxfColorCIELab(CxfDocument* cxfDoc);

	double  L() const { return m_values[0]; }
	double  a() const { return m_values[1]; }
	double  b() const { return m_values[2]; }

	virtual CxfColorType type() const { return cxfColorCIELab; }

	virtual bool parse(QDomElement& colorElem);
	virtual void reset();

protected:
	double m_values[3];
};

class CxfColorCIELCh : public CxfColor
{
public:
	CxfColorCIELCh(CxfDocument* cxfDoc);

	double  L() const { return m_values[0]; }
	double  C() const { return m_values[1]; }
	double  h() const { return m_values[2]; }

	ScLab lab() const;

	virtual CxfColorType type() const { return cxfColorCIELCh; }

	virtual bool parse(QDomElement& colorElem);
	virtual void reset();

protected:
	double m_values[3];
};

class CxfColorCIEXYZ : public CxfColor
{
public:
	CxfColorCIEXYZ(CxfDocument* cxfDoc);

	double  X() const { return m_values[0]; }
	double  Y() const { return m_values[1]; }
	double  Z() const { return m_values[2]; }

	ScLab lab() const;

	virtual CxfColorType type() const { return cxfColorCIELCh; }

	virtual bool parse(QDomElement& colorElem);
	virtual void reset();

protected:
	double m_values[3];
};

class CxfReflectanceSpectrum : public CxfColor
{
public:
	CxfReflectanceSpectrum(CxfDocument* cxfDoc);

	const QVector<double>& values() const { return m_values; }
	virtual CxfColorType type() const { return cxfReflectanceSpectrum; }

	virtual bool parse(QDomElement& colorElem);
	virtual void reset();

	int wavelengthStart() const;
	int wavelengthIncrement() const;
	QVector<int> wavelengths() const;

protected:
	QVector<double> m_values;
	int m_wavelengthStart;
};

#endif
