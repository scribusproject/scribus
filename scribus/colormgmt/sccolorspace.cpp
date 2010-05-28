/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "sccolorspace.h"

ScColorSpace::ScColorSpace() : m_data(NULL)
{
}

ScColorSpace::ScColorSpace(ScColorSpaceData* data) : m_data(data)
{
}

ScColorSpace::ScColorSpace(const QSharedPointer<ScColorSpaceData>& data) : m_data(data)
{
}

eColorType ScColorSpace::type() const
{
	if (m_data)
		return m_data->type();
	return Color_Unknown;
}

eColorFormat ScColorSpace::colorFormat() const
{
	if (m_data)
		return m_data->colorFormat();
	return Format_Undefined;
}

ScColorProfile ScColorSpace::profile() const
{
	if (m_data)
		return m_data->profile();
	return ScColorProfile();
}

uint ScColorSpace::numChannels(void) const
{
	if (m_data)
		return m_data->numChannels();
	return 0;
}

uint ScColorSpace::bytesPerChannel(void) const
{
	if (m_data)
		return m_data->bytesPerChannel();
	return 0;
}

bool ScColorSpace::hasAlphaChannel(void) const
{
	if (m_data)
		return m_data->hasAlphaChannel();
	return false;
}

uint  ScColorSpace::alphaIndex(void) const
{
	if (m_data)
		return m_data->alphaIndex();
	return 0;
}

void ScColorSpace::flattenAlpha(void* dataIn, uint numElems)
{
	if (m_data)
		m_data->flattenAlpha(dataIn, numElems);
}

ScColorTransform ScColorSpace::createTransform(const ScColorSpace& outputSpace, eRenderIntent renderIntent, 
	                                 long transformFlags)
{
	if (m_data && outputSpace.m_data)
		return m_data->createTransform(*outputSpace.m_data, renderIntent, transformFlags);
	return ScColorTransform();
}

ScColorTransform ScColorSpace::createTransform(const ScColorProfile& outputProfile, eColorFormat outputFormat,
	                                 eRenderIntent renderIntent, long transformFlags)
{
	if (m_data)
		return m_data->createTransform(outputProfile, outputFormat, renderIntent, transformFlags);
	return ScColorTransform();
}

ScColorTransform ScColorSpace::createProofingTransform(const ScColorSpace& outputSpace, const ScColorProfile& proofing, 
	                                         eRenderIntent renderIntent,  eRenderIntent proofingIntent, 
											 long transformFlags)
{
	if (m_data && outputSpace.m_data)
		return m_data->createProofingTransform(*outputSpace.m_data, proofing, renderIntent, proofingIntent, transformFlags);
	return ScColorTransform();
}

ScColorTransform ScColorSpace::createProofingTransform(const ScColorProfile& outputProfile, eColorFormat outputFormat,
											 const ScColorProfile& proofing, eRenderIntent renderIntent, 
	                                         eRenderIntent proofingIntent, long transformFlags)
{
	if (m_data)
		return m_data->createProofingTransform(outputProfile, outputFormat, proofing, renderIntent, proofingIntent, transformFlags);
	return ScColorTransform();
}

bool ScColorSpace::convert(ScColorSpace& data, eRenderIntent renderIntent, long transformFlags, 
	             void* dataIn, void* dataOut, uint numElems, ScColorTransform* lastTrans)
{
	if (m_data && data.m_data)
		return m_data->convert(*data.m_data, renderIntent, transformFlags, dataIn, dataOut, numElems, lastTrans);
	return false;
}

bool ScColorSpace::convert(ScColorSpace& data, eRenderIntent renderIntent, long transformFlags, 
	             void* dataIn, QIODevice* device, uint numElems, ScColorTransform* lastTrans)
{
	if (m_data && data.m_data)
		return m_data->convert(*data.m_data, renderIntent, transformFlags, dataIn, device, numElems, lastTrans);
	return false;
}

bool ScColorSpace::operator==(const ScColorSpace& other) const
{
	return m_data == other.m_data;
}