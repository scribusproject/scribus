/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
 
#ifndef SCCOLORMNGTENGINE_H
#define SCCOLORMNGTENGINE_H

#include <QList>
#include <QSharedPointer>
#include "scribusapi.h"
#include "sccolormngtenginedata.h"

class SCRIBUS_API ScColorMngtEngine
{
public:
	ScColorMngtEngine(ScColorMngtEngineData*);
	
	// Getters
	int   engineID() const;
	const QString& description() const;
	const ScColorMngtStrategy& strategy() const;
	
	// Setters, only for  color management strategy, and purely virtual ;)
	void setStrategy(const ScColorMngtStrategy& strategy);

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
									 
	// color engine equality operator function
	bool operator==(const ScColorMngtEngine& other) const;
	bool operator!=(const ScColorMngtEngine& other) const;
	
protected:
	QSharedPointer<ScColorMngtEngineData> m_data;
};

#endif
