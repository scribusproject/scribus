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
	ScColorMgmtEngine();
	
	// Getters
	static const QString& description();

	// function for getting available profile in a directory
	static QList<ScColorProfileInfo> getAvailableProfileInfo(const QString& directory, bool recursive);
	
	// functions for opening icc profiles
	static ScColorProfile openProfileFromFile(const QString& filePath);
	static ScColorProfile openProfileFromMem( const QByteArray& array);
	
	// functions for creating profiles
	static ScColorProfile createProfile_sRGB();
	static ScColorProfile createProfile_Lab();
	
	// functions for creating transforms
	static ScColorTransform createTransform(const ScColorProfile& inputProfile , eColorFormat inputFormat,
	                                 const ScColorProfile& outputProfile, eColorFormat outputFormat,
	                                 eRenderIntent renderIntent, long transformFlags);
	static ScColorTransform createProofingTransform(const ScColorProfile& inputProfile , eColorFormat inputFormat,
	                                 const ScColorProfile& outputProfile, eColorFormat outputFormat,
	                                 const ScColorProfile& proofingProfile, eRenderIntent renderIntent, 
									 eRenderIntent proofingIntent, long transformFlags);
	
protected:
	static QSharedPointer<ScColorMgmtEngineData> m_data;
	static QSharedPointer<ScColorMgmtEngineData> data();
};

#endif
