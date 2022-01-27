/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SCE308TABLES_H
#define SCE308TABLES_H

#include <QMap>
#include <QVector>

#include "sccolormgmtstructs.h"

class ScE308Table
{
public:
	virtual ~ScE308Table() {}
	
	// Illuminant and observer for which this table is to be used
	virtual eIlluminant illuminant() const = 0;
	virtual eObserver   observer() const = 0;
	
	virtual int wavelengthStart() const = 0;
	virtual int wavelengthIncrement() const = 0;

	// Wavelengths represented in the data set
	QVector<int> wavelengths() const;

	double wX(int waveLen) const { return m_wx.value(waveLen, 0.0); }
	double wY(int waveLen) const { return m_wy.value(waveLen, 0.0); }
	double wZ(int waveLen) const { return m_wz.value(waveLen, 0.0); }
	
	const QMap<int, double>& weightsX() const { return m_wx; }
	const QMap<int, double>& weightsY() const { return m_wy; }
	const QMap<int, double>& weightsZ() const { return m_wz; }

protected:
	// Weights for computing CIEXYZ X coordinate
	QMap<int, double> m_wx;

	// Weights for computing CIEXYZ Y coordinate
	QMap<int, double> m_wy;

	// Weights for computing CIEXYZ Z coordinate
	QMap<int, double> m_wz;
};

class ScE308Table5_D50_2deg : public ScE308Table
{
public:
	ScE308Table5_D50_2deg();

	virtual eIlluminant illuminant() const { return Illuminant_D50; }
	virtual eObserver   observer()   const { return Observer_2deg; }
	
	virtual int wavelengthStart() const { return 360; }
	virtual int wavelengthIncrement() const { return 10; }
};

class ScE308Table5_D50_10deg : public ScE308Table
{
public:
	ScE308Table5_D50_10deg();

	virtual eIlluminant illuminant() const { return Illuminant_D50; }
	virtual eObserver   observer()   const { return Observer_10deg; }
	
	virtual int wavelengthStart() const { return 360; }
	virtual int wavelengthIncrement() const { return 10; }
};

class ScE308Table6_D50_2deg : public ScE308Table
{
public:
	ScE308Table6_D50_2deg();

	virtual eIlluminant illuminant() const { return Illuminant_D50; }
	virtual eObserver   observer()   const { return Observer_2deg; }
	
	virtual int wavelengthStart() const { return 360; }
	virtual int wavelengthIncrement() const { return 10; }
};

class ScE308Table6_D50_10deg : public ScE308Table
{
public:
	ScE308Table6_D50_10deg();

	virtual eIlluminant illuminant() const { return Illuminant_D50; }
	virtual eObserver   observer()   const { return Observer_10deg; }
	
	virtual int wavelengthStart() const { return 360; }
	virtual int wavelengthIncrement() const { return 10; }
};

#endif
