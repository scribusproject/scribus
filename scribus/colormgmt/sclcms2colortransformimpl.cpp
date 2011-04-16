/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "sclcms2colortransformimpl.h"
#include "sclcms2colormgmtengineimpl.h"

ScLcms2ColorTransformImpl::ScLcms2ColorTransformImpl(cmsHTRANSFORM lcmsTransform)
                         : ScColorTransformData(), m_transformHandle(lcmsTransform)
{

}

ScLcms2ColorTransformImpl::~ScLcms2ColorTransformImpl()
{
	deleteTransform();
}

bool ScLcms2ColorTransformImpl::isNull() const
{
	return (m_transformHandle == NULL);
}

bool ScLcms2ColorTransformImpl::apply(void* input, void* output, uint numElem)
{
	if (m_transformHandle)
	{
		cmsDoTransform(m_transformHandle, input, output, numElem);
		return true;
	}
	return false;
}

bool ScLcms2ColorTransformImpl::apply(QByteArray& input, QByteArray& output, uint numElem)
{
	if (m_transformHandle)
	{
		cmsDoTransform(m_transformHandle, input.data(), output.data(), numElem);
		return true;
	}
	return false;
}

void ScLcms2ColorTransformImpl::changeBufferFormat(eColorFormat inputFormat, eColorFormat outputFormat)
{
	if (m_transformHandle)
	{
		cmsUInt32Number lcmsInputF  = ScLcms2ColorMgmtEngineImpl::translateFormatToLcmsFormat(inputFormat);
		cmsUInt32Number lcmsOutputF = ScLcms2ColorMgmtEngineImpl::translateFormatToLcmsFormat(outputFormat);
		cmsChangeBuffersFormat(m_transformHandle, lcmsInputF, lcmsOutputF);
	}
}

void ScLcms2ColorTransformImpl::deleteTransform(void)
{
	if (m_transformHandle)
	{
		cmsDeleteTransform(m_transformHandle);
		m_transformHandle = NULL;
	}
}

