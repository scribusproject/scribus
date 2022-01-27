/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "sclcms2colorprofileimpl.h"
#include "sclcms2colormgmtengineimpl.h"
#include <cstdlib>

ScLcms2ColorProfileImpl::ScLcms2ColorProfileImpl(ScColorMgmtEngine& engine, cmsHPROFILE lcmsProfile)
	: ScColorProfileImplBase(engine), m_profileHandle(lcmsProfile)
{

}

ScLcms2ColorProfileImpl::~ScLcms2ColorProfileImpl()
{
	closeProfile();
}

bool ScLcms2ColorProfileImpl::isNull() const
{
	return (m_profileHandle == nullptr);
}

eColorSpaceType ScLcms2ColorProfileImpl::colorSpace()  const
{
	if (m_profileHandle)
		return ScLcms2ColorMgmtEngineImpl::translateLcmsColorSpaceType( cmsGetColorSpace(m_profileHandle) );
	return ColorSpace_Unknown;
}

eColorSpaceType ScLcms2ColorProfileImpl::connectionSpace()  const
{
	if (m_profileHandle)
		return ScLcms2ColorMgmtEngineImpl::translateLcmsColorSpaceType( cmsGetPCS(m_profileHandle) );
	return ColorSpace_Unknown;
}

eProfileClass ScLcms2ColorProfileImpl::deviceClass() const
{
	if (m_profileHandle)
		return ScLcms2ColorMgmtEngineImpl::translateLcmsProfileClass( cmsGetDeviceClass(m_profileHandle) );
	return Class_Unknown;
}

int ScLcms2ColorProfileImpl::channelsOfColorSpace() const
{
	if (m_profileHandle)
	{
		eColorSpaceType colorspace = ScLcms2ColorMgmtEngineImpl::translateLcmsColorSpaceType( cmsGetColorSpace(m_profileHandle) );
		return ScLcms2ColorMgmtEngineImpl::channelsOfColorspace(colorspace);
	}
	return 0;
}

int ScLcms2ColorProfileImpl::channelsOfConnectionSpace() const
{
	if (m_profileHandle)
	{
		eColorSpaceType colorspace = ScLcms2ColorMgmtEngineImpl::translateLcmsColorSpaceType( cmsGetPCS(m_profileHandle) );
		return ScLcms2ColorMgmtEngineImpl::channelsOfColorspace(colorspace);
	}
	return 0;
}

bool ScLcms2ColorProfileImpl::isSuitableForOutput() const
{
	if (!m_profileHandle)
		return false;

	if (cmsIsMatrixShaper(m_profileHandle))
		return true;

	cmsUInt32Number defaultIntent = cmsGetHeaderRenderingIntent(m_profileHandle);
	return (cmsIsCLUT(m_profileHandle, defaultIntent, LCMS_USED_AS_INPUT) &&
			cmsIsCLUT(m_profileHandle, defaultIntent, LCMS_USED_AS_OUTPUT));
}

QString ScLcms2ColorProfileImpl::productDescription() const
{
	if (!m_productDescription.isEmpty())
		return m_productDescription;

	if (m_profileHandle == nullptr)
		return QString();

#ifdef _WIN32
	cmsUInt32Number descSize = cmsGetProfileInfo(m_profileHandle, cmsInfoDescription, "en", "US", nullptr, 0);
	if (descSize > 0)
	{
		wchar_t* descData = (wchar_t*) malloc(descSize + sizeof(wchar_t));
		if (descData)
			descSize = cmsGetProfileInfo(m_profileHandle, cmsInfoDescription, "en", "US", descData, descSize);
		if (descData && (descSize > 0))
		{
			uint stringLen = descSize / sizeof(wchar_t);
			descData[stringLen] = 0;
			m_productDescription = QString::fromWCharArray(descData);
		}
		free(descData);
	}
#else
	cmsUInt32Number descSize = cmsGetProfileInfoASCII(m_profileHandle, cmsInfoDescription, "en", "US", nullptr, 0);
	if (descSize > 0)
	{
		char* descData = (char*) malloc(descSize + sizeof(char));
		if (descData)
			descSize = cmsGetProfileInfoASCII(m_profileHandle, cmsInfoDescription, "en", "US", descData, descSize);
		if (descData && (descSize > 0))
			m_productDescription = QString(descData);
		free(descData);
	}
#endif

	return m_productDescription;
}

void ScLcms2ColorProfileImpl::closeProfile()
{
	if (m_profileHandle)
	{
		cmsCloseProfile(m_profileHandle);
		m_profileHandle = nullptr;
	}
}

bool ScLcms2ColorProfileImpl::save(QByteArray& profileData) const
{
	if (!m_profileHandle)
		return false;
	profileData.clear();

	// First retrieve profile size
	cmsUInt32Number bytesNeeded = 0;
	bool done = cmsSaveProfileToMem(m_profileHandle, nullptr, &bytesNeeded);
	if (!done)
		return false;

	// Allocate array for profile data
	profileData.resize(bytesNeeded);
	if (profileData.size() != static_cast<int>(bytesNeeded))
		return false;
	done = cmsSaveProfileToMem(m_profileHandle, profileData.data(), &bytesNeeded);

	return done;
}
