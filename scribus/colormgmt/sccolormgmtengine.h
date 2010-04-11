/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
 
#ifndef SCCOLORMGMTENGINE_H
#define SCCOLORMGMTENGINE_H

#include <QList>
#include <QSharedPointer>
#include "scribusapi.h"
#include "sccolormgmtenginedata.h"

class SCRIBUS_API ScColorMgmtEngine
{
public:
	ScColorMgmtEngine(ScColorMgmtEngineData*);
	
	// Getters
	int   engineID() const;
	const QString& description() const;
	const ScColorMgmtStrategy& strategy() const;
	
	// Setters, only for  color management strategy, and purely virtual ;)
	void setStrategy(const ScColorMgmtStrategy& strategy);

	// function for getting available profile in a directory
	QList<ScColorProfileInfo> getAvailableProfileInfo(const QString& directory, bool recursive);
	
	// functions for opening icc profiles
	ScColorProfile openProfileFromFile(const QString& filePath);
	ScColorProfile openProfileFromMem( const QByteArray& array);
	
	// functions for creating profiles
	ScColorProfile createProfile_sRGB();
	ScColorProfile createProfile_Lab();
	
	// functions for creating transforms
	ScColorTransform createTransform(const ScColorProfile& inputProfile , eColorFormat inputFormat,
	                                 const ScColorProfile& outputProfile, eColorFormat outputFormat,
	                                 eRenderIntent renderIntent, long transformFlags);
	ScColorTransform createProofingTransform(const ScColorProfile& inputProfile , eColorFormat inputFormat,
	                                 const ScColorProfile& outputProfile, eColorFormat outputFormat,
	                                 const ScColorProfile& proofingProfile, eRenderIntent renderIntent, 
									 eRenderIntent proofingIntent, long transformFlags);

	ScColorSpace createColorSpace(ScColorProfile& profile, eColorFormat colorFormat);
									 
	// color engine equality operator function
	bool operator==(const ScColorMgmtEngine& other) const;
	bool operator!=(const ScColorMgmtEngine& other) const;
	
protected:
	QSharedPointer<ScColorMgmtEngineData> m_data;
};

#endif
