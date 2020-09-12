/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
 
#ifndef SCCOLORMGMTENGINEDATA_H
#define SCCOLORMGMTENGINEDATA_H

#include <QByteArray>
#include <QString>

#include "sccolormgmtstructs.h"
#include "sccolorprofile.h"
#include "sccolorspace.h"
#include "sccolortransform.h"
#include "sccolortransformpool.h"

class ScColorMgmtEngine;

class ScColorMgmtEngineData
{
protected:
	int     m_engineID;
	QString m_description;
	ScColorMgmtStrategy  m_strategy;
	
public:

	ScColorMgmtEngineData(const QString& desc, int uniqueID) : m_engineID(uniqueID), 
	                                                           m_description(desc)
	{}
	virtual ~ScColorMgmtEngineData() = default;
	
	// Getters
	int   engineID() const { return m_engineID; }
	const QString& description() const { return m_description; }
	const ScColorMgmtStrategy& strategy() const { return m_strategy; }
	
	// Setter, only for  color management strategy, and purely virtual ;)
	virtual void setStrategy(const ScColorMgmtStrategy& strategy) = 0;

	// function for getting available profile in a directory
	virtual QList<ScColorProfileInfo> getAvailableProfileInfo(const QString& directory, bool recursive) = 0;
	
	// functions for opening icc profiles
	virtual ScColorProfile openProfileFromFile(ScColorMgmtEngine& engine, const QString& filePath) = 0;
	virtual ScColorProfile openProfileFromMem (ScColorMgmtEngine& engine, const QByteArray& array) = 0;
	
	// functions for creating profiles
	virtual ScColorProfile createProfile_sRGB(ScColorMgmtEngine& engine) = 0;
	virtual ScColorProfile createProfile_Lab (ScColorMgmtEngine& engine) = 0;
	
	// functions for creating transforms
	virtual ScColorTransform createTransform(ScColorMgmtEngine& colorManagementEngine, 
		                                     const ScColorProfile& inputProfile , eColorFormat inputFormat,
	                                         const ScColorProfile& outputProfile, eColorFormat outputFormat,
											 eRenderIntent renderIntent, long transformFlags) = 0;
	virtual ScColorTransform createProofingTransform(ScColorMgmtEngine& colorManagementEngine,
		                                     const ScColorProfile& inputProfile , eColorFormat inputFormat,
	                                         const ScColorProfile& outputProfile, eColorFormat outputFormat,
											 const ScColorProfile& proofing, eRenderIntent renderIntent, 
	                                         eRenderIntent proofingIntent, long transformFlags) = 0;

	virtual ScColorSpace createColorSpace(ScColorProfile& profile, eColorFormat colorFormat);

	static int channelsOfColorspace(eColorSpaceType colorspace);
};

#endif
