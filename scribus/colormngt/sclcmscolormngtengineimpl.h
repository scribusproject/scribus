/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SCLCMSCOLORMNGTENGINEIMPL_H
#define SCLCMSCOLORMNGTENGINEIMPL_H

#include "lcms.h"
#include "sccolormngtenginedata.h"
#include "sccolorprofilecache.h"

class ScLcmsColorMngtEngineImpl : public ScColorMngtEngineData
{
public:
	ScLcmsColorMngtEngineImpl();

	// Setter, only for  color management strategy
	virtual void setStrategy(const ScColorMngtStrategy& strategy);
	
	// functions for opening icc profiles
	virtual ScColorProfile openProfileFromFile(ScColorMngtEngine& engine, const QString& filePath);
	virtual ScColorProfile openProfileFromMem (ScColorMngtEngine& engine, const QByteArray& array);
	
	// functions for creating profiles
	virtual ScColorProfile createProfile_sRGB(ScColorMngtEngine& engine);
	virtual ScColorProfile createProfile_Lab (ScColorMngtEngine& engine);
	
	// functions for creating transforms
	virtual ScColorTransform createTransform(ScColorMngtEngine& colorManagementEngine,
		                                     const ScColorProfile& inputProfile , eColorFormat inputFormat,
	                                         const ScColorProfile& outputProfile, eColorFormat outputFormat,
											 eRenderIntent renderIntent, long transformFlags);
	virtual ScColorTransform createProofingTransform(ScColorMngtEngine& colorManagementEngine,
		                                     const ScColorProfile& inputProfile , eColorFormat inputFormat,
	                                         const ScColorProfile& outputProfile, eColorFormat outputFormat,
											 const ScColorProfile& proofing, eRenderIntent renderIntent, 
	                                         eRenderIntent proofingIntent, long transformFlags);

protected:

	static QSharedPointer<ScColorProfileCache> m_profileCache;

	DWORD translateFlagsToLcmsFlags(long flags);
	DWORD translateFormatToLcmsFormat(eColorFormat format);
	int   translateIntentToLcmsIntent(eRenderIntent intent, eRenderIntent defaut = Intent_Relative_Colorimetric);
};

#endif
