/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
 
#ifndef SCCOLORMNGTENGINEDATA_H
#define SCCOLORMNGTENGINEDATA_H

#include <QByteArray>
#include <QString>

#include "sccolormngtstructs.h"
#include "sccolorprofile.h"
#include "sccolortransform.h"
#include "sccolortransformpool.h"

class ScColorMngtEngine;

class ScColorMngtEngineData
{
protected:
	int     m_engineID;
	QString m_description;
	ScColorMngtStrategy  m_strategy;
	ScColorTransformPool m_transformPool;
	
public:

	ScColorMngtEngineData(const QString& desc, int uniqueID) : m_engineID(uniqueID), 
	                                                           m_description(desc),
	                                                           m_transformPool(uniqueID)
	{}
	
	// Getters
	int   engineID() const { return m_engineID; }
	const QString& description() const { return m_description; }
	const ScColorMngtStrategy& strategy() const { return m_strategy; }
	
	// Setter, only for  color management strategy, and purely virtual ;)
	virtual void setStrategy(const ScColorMngtStrategy& strategy) = 0;

	// function for getting available profile in a directory
	virtual QList<ScColorProfileInfo> getAvailableProfileInfo(const QString& directory, bool recursive) = 0;
	
	// functions for opening icc profiles
	virtual ScColorProfile openProfileFromFile(ScColorMngtEngine& engine, const QString& filePath) = 0;
	virtual ScColorProfile openProfileFromMem (ScColorMngtEngine& engine, const QByteArray& array) = 0;
	
	// functions for creating profiles
	virtual ScColorProfile createProfile_sRGB(ScColorMngtEngine& engine) = 0;
	virtual ScColorProfile createProfile_Lab (ScColorMngtEngine& engine) = 0;
	
	// functions for creating transforms
	virtual ScColorTransform createTransform(ScColorMngtEngine& colorManagementEngine, 
		                                     const ScColorProfile& inputProfile , eColorFormat inputFormat,
	                                         const ScColorProfile& outputProfile, eColorFormat outputFormat,
											 eRenderIntent renderIntent, long transformFlags) = 0;
	virtual ScColorTransform createProofingTransform(ScColorMngtEngine& colorManagementEngine,
		                                     const ScColorProfile& inputProfile , eColorFormat inputFormat,
	                                         const ScColorProfile& outputProfile, eColorFormat outputFormat,
											 const ScColorProfile& proofing, eRenderIntent renderIntent, 
	                                         eRenderIntent proofingIntent, long transformFlags) = 0;
};

#endif
