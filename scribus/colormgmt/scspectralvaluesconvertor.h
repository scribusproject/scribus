/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SCSPECTRALVALUESCONVERTOR_H
#define SCSPECTRALVALUESCONVERTOR_H

#include <QMap>
#include <QVector>

#include "sccieilluminants.h"
#include "sccieobservers.h"
#include "sccolormgmtstructs.h"

class ScSpectralValuesConvertor
{
public:
	/**
	 * Construct spectral values to CIEXYZ convertor with specified
	 * illuminant and observer
	 */
	ScSpectralValuesConvertor(eIlluminant illuminant, eObserver observer);

	/**
	 * Retrieve CIE XYZ values of illuminant white
	 */
	const ScXYZ& illumunantWhite() const { return m_illuminantWhite; }

	/**
	* Compute CIELab values for specified spectrum
	*/
	ScLab toLab(const QMap<int, double>& spectrum) const;

	/**
	* Compute CIELab values for specified spectrum
	*/
	ScLab toLab(const QVector<int>& wavelengths, const QVector<double>& reflectances) const;

	/**
	* Compute CIEXYZ values for specified spectrum
	*/
	ScXYZ toXYZ(const QMap<int, double>& spectrum) const;

	/**
	 * Compute CIEXYZ values for specified spectrum
	 */
	ScXYZ toXYZ(const QVector<int>& wavelengths, const QVector<double>& reflectances) const;

protected:
	const ScCIEIlluminant& m_illuminant;
	const ScCIEObserver& m_observer;
	ScXYZ m_illuminantWhite;

	ScXYZ computeIlluminantWhite();
};

#endif
