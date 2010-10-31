/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SCLCMSCOLORMGMTENGINEIMPL_H
#define SCLCMSCOLORMGMTENGINEIMPL_H

#include <stdexcept>

#include "lcms.h"
#include "sccolormgmtenginedata.h"
#include "sccolorprofilecache.h"

class ScLcmsColorMgmtEngineImpl : public ScColorMgmtEngineData
{
	friend class ScLcmsColorProfileImpl;
	friend class ScLcmsColorTransformImpl;

public:
	ScLcmsColorMgmtEngineImpl();

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

	static DWORD translateFlagsToLcmsFlags(long flags);
	static DWORD translateFormatToLcmsFormat(eColorFormat format);
	static int translateIntentToLcmsIntent(eRenderIntent intent, eRenderIntent defaut = Intent_Relative_Colorimetric);
	static eColorSpaceType translateLcmsColorSpaceType(icColorSpaceSignature);
	static eProfileClass   translateLcmsProfileClass(icProfileClassSignature);

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
	static int cmsErrorHandler(int ErrorCode, const char *ErrorText);
};

#endif
