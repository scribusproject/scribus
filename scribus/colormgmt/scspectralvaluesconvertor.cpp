/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <cassert>
#include "lcms2.h"

#include "scspectralvaluesconvertor.h"
#include "scciedata.h"

ScSpectralValuesConvertor::ScSpectralValuesConvertor(eIlluminant illuminant, eObserver observer)
                         : m_illuminant(ScCIEData::instance().cieIlluminant(illuminant)),
                           m_observer(ScCIEData::instance().cieObserver(observer))
{
	m_illuminantWhite = computeIlluminantWhite();
}

ScXYZ ScSpectralValuesConvertor::computeIlluminantWhite()
{
	int wl;
	double illPower;
	ScXYZ illWhite = { 0.0, 0.0, 0.0 };

	const QVector<int> wavelengths = m_illuminant.wavelengths();
	for (int i = 0; i < wavelengths.count(); ++i)
	{
		wl = wavelengths.at(i);
		illPower = m_illuminant.power(wl);

		illWhite.X += illPower * m_observer.xVal(wl);
		illWhite.Y += illPower * m_observer.yVal(wl);
		illWhite.Z += illPower * m_observer.zVal(wl);
	}

	double norm = 1.0 / illWhite.Y;
	illWhite.X *= norm;
	illWhite.Y  = 1.0;
	illWhite.Z *= norm;

	return illWhite;
}

ScLab ScSpectralValuesConvertor::toLab(const QMap<int, double>& spectrum) const
{
	ScXYZ cieXYZ = toXYZ(spectrum);

	cmsCIEXYZ cmsXYZ = { cieXYZ.X, cieXYZ.Y, cieXYZ.Z };
	cmsCIEXYZ cmsWhite = { m_illuminantWhite.X, m_illuminantWhite.Y, m_illuminantWhite.Z };
	
	cmsCIELab cmsLab = { 0.0, 0.0, 0.0 };
	cmsXYZ2Lab(&cmsWhite, &cmsLab, &cmsXYZ);

	ScLab cieLab = { cmsLab.L, cmsLab.a, cmsLab.b };
	return cieLab;
}

ScLab ScSpectralValuesConvertor::toLab(const QVector<int>& wavelengths, const QVector<double>& reflectances) const
{
	ScXYZ cieXYZ = toXYZ(wavelengths, reflectances);

	cmsCIEXYZ cmsXYZ = { cieXYZ.X, cieXYZ.Y, cieXYZ.Z };
	cmsCIEXYZ cmsWhite = { m_illuminantWhite.X, m_illuminantWhite.Y, m_illuminantWhite.Z };

	cmsCIELab cmsLab = { 0.0, 0.0, 0.0 };
	cmsXYZ2Lab(&cmsWhite, &cmsLab, &cmsXYZ);

	ScLab cieLab = { cmsLab.L, cmsLab.a, cmsLab.b };
	return cieLab;
}

ScXYZ ScSpectralValuesConvertor::toXYZ(const QMap<int, double>& spectrum) const
{
	int wl;
	double illPower, reflectance;
	ScXYZ cieXYZ = { 0.0, 0.0, 0.0 };
	ScXYZ illXYZ = { 0.0, 0.0, 0.0 };

	// The set of wavelength from color sample may not be the same than the one used
	// by the illuminant and observer datasets, so we compute some normalization
	// factors here
	QMap<int, double>::const_iterator iter;
	for (iter = spectrum.begin(); iter != spectrum.end(); iter++)
	{
		wl = iter.key();
		illPower = m_illuminant.power(wl);

		illXYZ.X += illPower * m_observer.xVal(wl);
		illXYZ.Y += illPower * m_observer.yVal(wl);
		illXYZ.Z += illPower * m_observer.zVal(wl);
	}

	double normX = m_illuminantWhite.X / illXYZ.X;
	double normY = m_illuminantWhite.Y / illXYZ.Y;
	double normZ = m_illuminantWhite.Z / illXYZ.Z;

	// Now compute XYZ tristimulus values from color sample
	for (iter = spectrum.begin(); iter != spectrum.end(); iter++)
	{
		wl = iter.key();
		reflectance = iter.value();
		illPower = m_illuminant.power(wl);

		// Fix possible measurement artifacts
		reflectance = qMax(0.0, reflectance);

		cieXYZ.X += reflectance * illPower * m_observer.xVal(wl);
		cieXYZ.Y += reflectance * illPower * m_observer.yVal(wl);
		cieXYZ.Z += reflectance * illPower * m_observer.zVal(wl);
	}

	cieXYZ.X *= normX;
	cieXYZ.Y *= normY;
	cieXYZ.Z *= normZ;

	// Fix possible computation artificats
	cieXYZ.X = qMax(0.0, cieXYZ.X);
	cieXYZ.Y = qMax(0.0, cieXYZ.Y);
	cieXYZ.Z = qMax(0.0, cieXYZ.Z);

	return cieXYZ;
}

ScXYZ ScSpectralValuesConvertor::toXYZ(const QVector<int>& wavelengths, const QVector<double>& reflectances) const
{
	int wl;
	double illPower, reflectance;
	ScXYZ cieXYZ = { 0.0, 0.0, 0.0 };
	ScXYZ illXYZ = { 0.0, 0.0, 0.0 };

	assert(wavelengths.count() == reflectances.count());

	// The set of wavelength from color sample may not be the same than the one used
	// by the illuminant and observer datasets, so we compute some normalization
	// factors here
	for (int i = 0; i < wavelengths.count(); ++i)
	{
		wl = wavelengths.at(i);
		illPower = m_illuminant.power(wl);

		illXYZ.X += illPower * m_observer.xVal(wl);
		illXYZ.Y += illPower * m_observer.yVal(wl);
		illXYZ.Z += illPower * m_observer.zVal(wl);
	}

	double normX = m_illuminantWhite.X / illXYZ.X;
	double normY = m_illuminantWhite.Y / illXYZ.Y;
	double normZ = m_illuminantWhite.Z / illXYZ.Z;

	// Now compute XYZ tristimulus values from color sample
	for (int i = 0; i < wavelengths.count(); ++i)
	{
		wl = wavelengths.at(i);
		reflectance = reflectances.at(i);
		illPower = m_illuminant.power(wl);

		// Fix possible measurement artifacts
		reflectance = qMax(0.0, reflectance);

		cieXYZ.X += reflectance * illPower * m_observer.xVal(wl);
		cieXYZ.Y += reflectance * illPower * m_observer.yVal(wl);
		cieXYZ.Z += reflectance * illPower * m_observer.zVal(wl);
	}

	cieXYZ.X *= normX;
	cieXYZ.Y *= normY;
	cieXYZ.Z *= normZ;

	// Fix possible computation artificats
	cieXYZ.X = qMax(0.0, cieXYZ.X);
	cieXYZ.Y = qMax(0.0, cieXYZ.Y);
	cieXYZ.Z = qMax(0.0, cieXYZ.Z);

	return cieXYZ;
}