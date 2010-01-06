/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "sclcmscolorprofileimpl.h"

ScLcmsColorProfileImpl::ScLcmsColorProfileImpl(ScColorMgmtEngine& engine, cmsHPROFILE lcmsProfile)
                      : ScColorProfileImplBase(engine), m_profileHandle(lcmsProfile)
{

}

ScLcmsColorProfileImpl::~ScLcmsColorProfileImpl()
{
	closeProfile();
}

bool ScLcmsColorProfileImpl::isNull() const
{
	return (m_profileHandle == NULL);
}

icColorSpaceSignature ScLcmsColorProfileImpl::colorSpace()  const
{
	if (m_profileHandle)
		return cmsGetColorSpace(m_profileHandle);
	return ((icColorSpaceSignature) 0);
}

icProfileClassSignature ScLcmsColorProfileImpl::deviceClass() const
{
	if (m_profileHandle)
		return cmsGetDeviceClass(m_profileHandle);
	return ((icProfileClassSignature) 0);
}

QString ScLcmsColorProfileImpl::productDescription() const
{
	if (m_productDescription.isEmpty())
	{
		if (m_profileHandle)
		{
			const char* desc = cmsTakeProductDesc(m_profileHandle);
			m_productDescription = QString(desc);
			return m_productDescription;
		}
	}
	return m_productDescription;
}

void ScLcmsColorProfileImpl::closeProfile(void)
{
	if (m_profileHandle)
	{
		cmsCloseProfile(m_profileHandle);
		m_profileHandle = NULL;
	}
}