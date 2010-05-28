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

	// Setter, only for  color management strategy
	virtual void setStrategy(const ScColorMgmtStrategy& strategy);

	// function for getting available profile in a directory
	virtual QList<ScColorProfileInfo> getAvailableProfileInfo(const QString& directory, bool recursive);
	
	// functions for opening icc profiles
	virtual ScColorProfile openProfileFromFile(ScColorMgmtEngine& engine, const QString& filePath);
	virtual ScColorProfile openProfileFromMem (ScColorMgmtEngine& engine, const QByteArray& array);
	
	// functions for creating profiles
	virtual ScColorProfile createProfile_sRGB(ScColorMgmtEngine& engine);
	virtual ScColorProfile createProfile_Lab (ScColorMgmtEngine& engine);
	
	// functions for creating transforms
	virtual ScColorTransform createTransform(ScColorMgmtEngine& colorManagementEngine,
		                                     const ScColorProfile& inputProfile , eColorFormat inputFormat,
	                                         const ScColorProfile& outputProfile, eColorFormat outputFormat,
											 eRenderIntent renderIntent, long transformFlags);
	virtual ScColorTransform createProofingTransform(ScColorMgmtEngine& colorManagementEngine,
		                                     const ScColorProfile& inputProfile , eColorFormat inputFormat,
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

	// Exception class thrown by cmsErrorHandler
	class lcmsException : public std::runtime_error 
	{
	public :
		lcmsException(const char* msg) : std::runtime_error(msg) {}
	};

	/*!
	\brief Simple error handler for use in conjunction with littlecms
	\param ErrorCode error code issued by little cms
	\param ErrorText error message corresponding to the error code
	*/
	static void cmsErrorHandler(cmsContext contextID, cmsUInt32Number ErrorCode, const char *ErrorText);
};

#endif
