/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "sclcms2colorprofileimpl.h"
#include "sclcms2colormgmtengineimpl.h"
#include <cstdlib>

ScLcms2ColorProfileImpl::ScLcms2ColorProfileImpl(cmsHPROFILE lcmsProfile)
                       : ScColorProfileData(), m_profileHandle(lcmsProfile)
{

}

ScLcms2ColorProfileImpl::~ScLcms2ColorProfileImpl()
{
	closeProfile();
}

bool ScLcms2ColorProfileImpl::isNull() const
{
	return (m_profileHandle == NULL);
}

eColorSpaceType ScLcms2ColorProfileImpl::colorSpace()  const
{
	if (m_profileHandle)
		return ScLcms2ColorMgmtEngineImpl::translateLcmsColorSpaceType( cmsGetColorSpace(m_profileHandle) );
	return ColorSpace_Unknown;
}

eProfileClass ScLcms2ColorProfileImpl::deviceClass() const
{
	if (m_profileHandle)
		return ScLcms2ColorMgmtEngineImpl::translateLcmsProfileClass( cmsGetDeviceClass(m_profileHandle) );
	return Class_Unknown;
}

QString ScLcms2ColorProfileImpl::productDescription() const
{
	if (m_productDescription.isEmpty())
	{
		if (m_profileHandle)
		{
#ifdef _WIN32
			cmsUInt32Number descSize = cmsGetProfileInfo(m_profileHandle, cmsInfoDescription, "en", "US", NULL, 0);
			if (descSize > 0)
			{
				wchar_t* descData = (wchar_t*) malloc(descSize + sizeof(wchar_t));
				descSize = cmsGetProfileInfo(m_profileHandle, cmsInfoDescription, "en", "US", descData, descSize);
				if (descSize > 0)
				{
					uint stringLen = descSize / sizeof(wchar_t);
					descData[stringLen] = 0;
					if (sizeof(wchar_t) == sizeof(QChar)) {
						m_productDescription = QString::fromUtf16((ushort *) descData);
					} else {
						m_productDescription = QString::fromUcs4((uint *) descData);
					}
					free(descData);
				}
			}
#else
			cmsUInt32Number descSize = cmsGetProfileInfoASCII(m_profileHandle, cmsInfoDescription, "en", "US", NULL, 0);
			if (descSize > 0)
			{
				char* descData = (char*) malloc(descSize + sizeof(char));
				descSize = cmsGetProfileInfoASCII(m_profileHandle, cmsInfoDescription, "en", "US", descData, descSize);
				if (descSize > 0)
				{
					m_productDescription = QString(descData);
					free(descData);
				}
			}
#endif
		}
	}
	return m_productDescription;
}

void ScLcms2ColorProfileImpl::closeProfile(void)
{
	if (m_profileHandle)
	{
		cmsCloseProfile(m_profileHandle);
		m_profileHandle = NULL;
	}
}