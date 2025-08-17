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
	explicit CxfColor(CxfDocument* cxfDoc);
	virtual ~CxfColor() = default;

	virtual bool isValid() const;
	virtual CxfColorType type() const { return cxfColorUnknown; }

	const CxfColorSpecification* colorSpecification() const { return m_colorSpec; }
	const CxfDocument* document() const { return m_cxfDoc; }
	
	virtual bool parse(QDomElement& colorElem) = 0;
	virtual void reset();
	
protected:
	CxfDocument* m_cxfDoc { nullptr };
	const CxfColorSpecification* m_colorSpec { nullptr };
};

using CxfColorShPtr = QSharedPointer<CxfColor>;

class CxfColorRGB : public CxfColor
{
public:
	explicit CxfColorRGB(CxfDocument* cxfDoc);
	
	double  maxRange() const { return m_maxRange; }
	
	double  red() const   { return m_values[0]; }
	double  green() const { return m_values[1]; }
	double  blue() const  { return m_values[2]; }
	
	CxfColorType type() const override { return cxfColorRGB; }

	bool parse(QDomElement& colorElem) override;
	void reset() override;
	
protected:
	double m_maxRange { 255.0 };
	double m_values[3] { 0.0, 0.0, 0.0 };
};

class CxfColorSRGB : public CxfColorRGB
{
public:
	explicit CxfColorSRGB(CxfDocument* cxfDoc);

	CxfColorType type() const override { return cxfColorSRGB; }
};

class CxfColorAdobeRGB : public CxfColorRGB
{
public:
	explicit CxfColorAdobeRGB(CxfDocument* cxfDoc);

	CxfColorType type() const override { return cxfColorAdobeRGB; }
};

class CxfColorHTML : public CxfColorRGB
{
public:
	explicit CxfColorHTML(CxfDocument* cxfDoc);

	CxfColorType type() const override { return cxfColorHTML; }
	bool parse(QDomElement& colorElem) override;
};

class CxfColorCMYK : public CxfColor
{
public:
	explicit CxfColorCMYK(CxfDocument* cxfDoc);

	double  cyan() const    { return m_values[0]; }
	double  magenta() const { return m_values[1]; }
	double  yellow() const  { return m_values[2]; }
	double  black() const   { return m_values[3]; }

	CxfColorType type() const override { return cxfColorCMYK; }

	bool parse(QDomElement& colorElem) override;
	void reset() override;

protected:
	double m_values[4] { 0.0, 0.0, 0.0, 0.0 };
};

class CxfColorCIELab : public CxfColor
{
public:
	explicit CxfColorCIELab(CxfDocument* cxfDoc);

	double  L() const { return m_values[0]; }
	double  a() const { return m_values[1]; }
	double  b() const { return m_values[2]; }

	CxfColorType type() const override { return cxfColorCIELab; }

	bool parse(QDomElement& colorElem) override;
	void reset() override;

protected:
	double m_values[3] { 0.0, 0.0, 0.0 };
};

class CxfColorCIELCh : public CxfColor
{
public:
	explicit CxfColorCIELCh(CxfDocument* cxfDoc);

	double  L() const { return m_values[0]; }
	double  C() const { return m_values[1]; }
	double  h() const { return m_values[2]; }

	ScLab lab() const;

	CxfColorType type() const override { return cxfColorCIELCh; }

	bool parse(QDomElement& colorElem) override;
	void reset() override;

protected:
	double m_values[3] { 0.0, 0.0, 0.0 };
};

class CxfColorCIEXYZ : public CxfColor
{
public:
	explicit CxfColorCIEXYZ(CxfDocument* cxfDoc);

	double  X() const { return m_values[0]; }
	double  Y() const { return m_values[1]; }
	double  Z() const { return m_values[2]; }

	ScLab lab() const;

	CxfColorType type() const override { return cxfColorCIELCh; }

	bool parse(QDomElement& colorElem) override;
	void reset() override;

protected:
	double m_values[3] { 0.0, 0.0, 0.0 };
};

class CxfReflectanceSpectrum : public CxfColor
{
public:
	explicit CxfReflectanceSpectrum(CxfDocument* cxfDoc);

	const QVector<double>& values() const { return m_values; }
	CxfColorType type() const override { return cxfReflectanceSpectrum; }

	bool parse(QDomElement& colorElem) override;
	void reset() override;

	int wavelengthStart() const;
	int wavelengthIncrement() const;
	QVector<int> wavelengths() const;

protected:
	QVector<double> m_values;
	int m_wavelengthStart { 0 };
};

#endif
