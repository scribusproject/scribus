/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SCLCMS2COLORMGMTENGINEIMPL_H
#define SCLCMS2COLORMGMTENGINEIMPL_H

#include <stdexcept>

#include "lcms2.h"
#include "sccolormgmtenginedata.h"
#include "sccolorprofilecache.h"

class ScLcms2ColorMgmtEngineImpl : public ScColorMgmtEngineData
{
	friend class ScLcms2ColorProfileImpl;
	friend class ScLcms2ColorTransformImpl;

public:
	ScLcms2ColorMgmtEngineImpl();

	// function for getting available profile in a directory
	virtual QList<ScColorProfileInfo> getAvailableProfileInfo(const QString& directory, bool recursive);
	
	// functions for opening icc profiles
	virtual ScColorProfile openProfileFromFile(const QString& filePath);
	virtual ScColorProfile openProfileFromMem (const QByteArray& array);
	
	// functions for creating profiles
	virtual ScColorProfile createProfile_sRGB();
	virtual ScColorProfile createProfile_Lab ();
	
	// functions for creating transforms
	virtual ScColorTransform createTransform(const ScColorProfile& inputProfile , eColorFormat inputFormat,
	                                         const ScColorProfile& outputProfile, eColorFormat outputFormat,
											 eRenderIntent renderIntent, long transformFlags);
	virtual ScColorTransform createProofingTransform(const ScColorProfile& inputProfile , eColorFormat inputFormat,
	                                         const ScColorProfile& outputProfile, eColorFormat outputFormat,
											 const ScColorProfile& proofing, eRenderIntent renderIntent, 
	                                         eRenderIntent proofingIntent, long transformFlags);

protected:

	// Color profile cache
	static QSharedPointer<ScColorProfileCache> m_profileCache;

	// Color transform cache
	static QSharedPointer<ScColorTransformPool> m_transformPool;

	static cmsUInt32Number translateFlagsToLcmsFlags(long flags);
	static cmsUInt32Number translateFormatToLcmsFormat(eColorFormat format);
	static int translateIntentToLcmsIntent(eRenderIntent intent, eRenderIntent defaut = Intent_Relative_Colorimetric);
	static eColorSpaceType translateLcmsColorSpaceType(cmsColorSpaceSignature);
	static eProfileClass   translateLcmsProfileClass(cmsProfileClassSignature);

	/*!
	\brief Simple error handler for use in conjunction with littlecms
	\param ErrorCode error code issued by little cms
	\param ErrorText error message corresponding to the error code
	*/
	static void cmsErrorHandler(cmsContext contextID, cmsUInt32Number ErrorCode, const char *ErrorText);
};

#endif
