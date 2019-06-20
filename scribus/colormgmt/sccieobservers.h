/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SCCIEOBSERVERS_H
#define SCCIEOBSERVERS_H

#include <QMap>
#include <QVector>

class ScCIEObserver
{
public:
	virtual ~ScCIEObserver() = default;

	// Wavelengths represented in the data set
	QVector<int> wavelengths() const;

	double xVal(int waveLen) const { return m_x.value(waveLen, 0.0); }
	double yVal(int waveLen) const { return m_y.value(waveLen, 0.0); }
	double zVal(int waveLen) const { return m_z.value(waveLen, 0.0); }

protected:
	// Tristimulus values for computing CIEXYZ X coordinate
	QMap<int, double> m_x;

	// Tristimulus values for computing CIEXYZ Y coordinate
	QMap<int, double> m_y;

	// Tristimulus values for computing CIEXYZ Z coordinate
	QMap<int, double> m_z;
};

class ScCIE1931Observer : public ScCIEObserver
{
public:
	ScCIE1931Observer();
};

class ScCIE1964Observer : public ScCIEObserver
{
public:
	ScCIE1964Observer();
};

#endif // SCCIEOBSERVERS_H
