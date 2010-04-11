/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <stdlib.h>
#include "sccolorspacedata.h"
#include "sccolormgmtengine.h"

ScColorTransform ScColorSpaceData::createTransform(const ScColorSpaceData& outputSpace, eRenderIntent renderIntent, 
	                               long transformFlags)
{
	ScColorTransform colorTransform;
	if (m_profile && outputSpace.profile())
	{
		colorTransform = m_profile.engine().createTransform(m_profile, m_colorFormat, 
		                                    outputSpace.profile(), outputSpace.colorFormat(), 
											renderIntent, transformFlags);
	}
	return colorTransform;
}

ScColorTransform ScColorSpaceData::createTransform(const ScColorProfile& outputProfile, eColorFormat outputFormat,
                                   eRenderIntent renderIntent, long transformFlags)
{
	ScColorTransform colorTransform;
	if (m_profile)
	{
		colorTransform = m_profile.engine().createTransform(m_profile, m_colorFormat, 
		                                    outputProfile, outputFormat, renderIntent, 
											transformFlags);
	}
	return colorTransform;
}

ScColorTransform ScColorSpaceData::createProofingTransform(const ScColorSpaceData& outputSpace, const ScColorProfile& proofing, 
	                               eRenderIntent renderIntent,  eRenderIntent proofingIntent, long transformFlags)
{
	ScColorTransform colorTransform;
	if (m_profile && outputSpace.profile())
	{
		colorTransform = m_profile.engine().createProofingTransform(m_profile, m_colorFormat, 
		                                    outputSpace.profile(), outputSpace.colorFormat(), 
											proofing, renderIntent, proofingIntent, transformFlags);
	}
	return colorTransform;
}

ScColorTransform ScColorSpaceData::createProofingTransform(const ScColorProfile& outputProfile, eColorFormat outputFormat,
                                   const ScColorProfile& proofing, eRenderIntent renderIntent, 
                                   eRenderIntent proofingIntent, long transformFlags)
{
	ScColorTransform colorTransform;
	if (m_profile)
	{
		colorTransform = m_profile.engine().createProofingTransform(m_profile, m_colorFormat, 
		                                    outputProfile, outputFormat, proofing, renderIntent, 
											proofingIntent, transformFlags);
	}
	return colorTransform;
}

bool ScColorSpaceData::convert(ScColorSpaceData& data, eRenderIntent renderIntent, long transformFlags, 
	                     void* dataIn, void* dataOut, uint numElems, ScColorTransform* lastTrans)
{
	ScColorTransform transform;
	if (lastTrans)
		transform = *lastTrans;
	if (!transform)
		transform = createTransform(data, renderIntent, transformFlags);	

	bool success = false;
	if (transform)
	{
		success = transform.apply(dataIn, dataOut, numElems);
		if (!this->hasAlphaChannel() && data.hasAlphaChannel())
			data.flattenAlpha(dataOut, numElems);
		if (lastTrans && (transform != *lastTrans))
			*lastTrans = transform;
		success = true;
	}
	return success;
}

bool ScColorSpaceData::convert(ScColorSpaceData& data, eRenderIntent renderIntent, long transformFlags, 
	                     void* dataIn, QIODevice* device, uint numElems, ScColorTransform* lastTrans)
{
	bool success = false;
	uint numChannels = data.numChannels();
	uint bytesPerChannels = data.bytesPerChannel();
	uint dataSize    = numChannels * bytesPerChannels * numElems;
	if (dataSize == 0)
		return false;
	char *dataBuffer = (char*) malloc(dataSize);
	if (dataBuffer)
	{
		success = convert(data, renderIntent, transformFlags, dataIn, dataBuffer, numElems, lastTrans);
		if (success)
		{
			qint64 written = device->write((const char*) dataBuffer, dataSize);
			success = (written == (qint64) dataSize);
		}
		free(dataBuffer);
	}
	return success;
}