/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCCIEDATA_H
#define SCCIEDATA_H

#include "sccieilluminants.h"
#include "sccieobservers.h"
#include "sccolormgmtstructs.h"

class ScCIEData
{
public:
	/**
	 * @brief Get a reference to the ScCIEData instance
	 *        creating it if necessary.
	 * @sa ScCIEData::destroy()
	 */
	static const ScCIEData& instance();
	
	/**
	 * @brief Destroy the ScCIEData instance, if any.
	 * @sa ScCIEData::instance()
	 */
	static void destroy();

	/**
	* @brief Retrieve specific CIE illuminant
	*/
	const ScCIEIlluminant& cieIlluminant(eIlluminant illuminant) const;

	/**
	* @brief Retrieve specific CIE illuminant
	*/
	const ScCIEObserver& cieObserver(eObserver observer) const;
	
	/**
	 * @brief Retrieve data for CIE 1931 2° observer.
	 */
	const ScCIE1931Observer& cie1931Observer() const { return m_cie1931Observer; }
	
	/**
	 * @brief Retrieve data for CIE 1964 10° observer.
	 */
	const ScCIE1964Observer& cie1964Observer() const { return m_cie1964Observer; }

	/**
	 * @brief Retrieve data for CIE D50 illuminant.
	 */
	const ScD50Illuminant& d50Illuminant() const { return m_d50Illuminant; }

	/**
	* @brief Retrieve data for CIE D65 illuminant.
	*/
	const ScD65Illuminant& d65Illuminant() const { return m_d65Illuminant; }
	
protected:
	/** @brief Constructor. Use ScCIEData::instance() instead. */
	ScCIEData() = default;
	
	/** @brief Pointer to existing instance of ScCIEData, if any. */
	static ScCIEData* m_instance;
	
	/** @brief CIE 1931 2° observer. */
	ScCIE1931Observer m_cie1931Observer;
	
	/** @brief CIE 1964 10° observer. */
	ScCIE1964Observer m_cie1964Observer;

	/** @brief CIE D50 illuminant. */
	ScD50Illuminant m_d50Illuminant;

	/** @brief CIE D65 illuminant. */
	ScD65Illuminant m_d65Illuminant;
};

#endif
