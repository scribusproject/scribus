/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SCCIEILLUMINANTS_H
#define SCCIEILLUMINANTS_H

#include <QMap>
#include <QVector>

class ScCIEIlluminant
{
public:
	virtual ~ScCIEIlluminant() = default;
	
	// Relative illuminant power for specified wavelength
	double power(int waveLen) const { return m_pow.value(waveLen, 0.0); }

	// Wavelengths represented in the data set
	QVector<int> wavelengths() const;
	
protected:
	// Relative spectral power distribution
	QMap<int, double> m_pow;
};

class ScD50Illuminant : public ScCIEIlluminant
{
public:
	ScD50Illuminant();
};

class ScD65Illuminant : public ScCIEIlluminant
{
public:
	ScD65Illuminant();
};

#endif // SCCIEILLUMINANTS_H
