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
#include "sce308tables.h"

class ScSpectralValuesConvertor
{
public:
	/**
	 * Construct spectral values to CIEXYZ convertor with specified
	 * illuminant and observer
	 */
	ScSpectralValuesConvertor(eIlluminant illuminant, eObserver observer);

	/**
	* Construct spectral values to CIEXYZ convertor with specified E308 table
	*/
	ScSpectralValuesConvertor(const ScE308Table& e308table);

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
	ScXYZ m_illuminantWhite;

	QMap<int, double> m_weightsX;
	QMap<int, double> m_weightsY;
	QMap<int, double> m_weightsZ;

	ScXYZ computeIlluminantWhite(const ScCIEIlluminant& illuminant, const ScCIEObserver& observer);
	ScXYZ computeIlluminantWhite(const ScE308Table& e380table);
	void  computeWeights(const ScCIEIlluminant& illuminant, const ScCIEObserver& observer);
};

#endif
