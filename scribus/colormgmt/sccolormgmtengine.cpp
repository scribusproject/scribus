/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <cassert>
#include "sccolormgmtengine.h"

ScColorMgmtEngine::ScColorMgmtEngine(ScColorMgmtEngineData* data) : m_data(data)
{
	assert(data != NULL);
}

int ScColorMgmtEngine::engineID() const
{
	return m_data->engineID();
}

const QString& ScColorMgmtEngine::description() const
{
	return m_data->description();
}

const ScColorMgmtStrategy& ScColorMgmtEngine::strategy() const
{
	return m_data->strategy();
}

void ScColorMgmtEngine::setStrategy(const ScColorMgmtStrategy& strategy)
{
	m_data->setStrategy(strategy);
}

QList<ScColorProfileInfo> ScColorMgmtEngine::getAvailableProfileInfo(const QString& directory, bool recursive)
{
	return m_data->getAvailableProfileInfo(directory, recursive);
}

ScColorProfile ScColorMgmtEngine::openProfileFromFile(const QString& filePath)
{
	return m_data->openProfileFromFile(*this, filePath);
}

ScColorProfile ScColorMgmtEngine::openProfileFromMem( const QByteArray& array)
{
	return m_data->openProfileFromMem(*this, array);
}

ScColorProfile ScColorMgmtEngine::createProfile_sRGB()
{
	return m_data->createProfile_sRGB(*this);
}

ScColorProfile ScColorMgmtEngine::createProfile_Lab()
{
	return m_data->createProfile_Lab(*this);
}

ScColorTransform ScColorMgmtEngine::createTransform(const ScColorProfile& inputProfile , eColorFormat inputFormat,
	                                         const ScColorProfile& outputProfile, eColorFormat outputFormat, 
                                             eRenderIntent renderIntent, long transformFlags)
{
	return m_data->createTransform(*this, inputProfile, inputFormat, outputProfile, outputFormat, renderIntent, transformFlags);
}

ScColorTransform ScColorMgmtEngine::createProofingTransform(const ScColorProfile& inputProfile, eColorFormat inputFormat,
	                                         const ScColorProfile& outputProfile, eColorFormat outputFormat,
											 const ScColorProfile& proofingProfile, eRenderIntent renderIntent, 
                                             eRenderIntent proofingIntent, long transformFlags)
{
	return m_data->createProofingTransform(*this, inputProfile, inputFormat, outputProfile, outputFormat, 
	                                       proofingProfile, renderIntent, proofingIntent, transformFlags);
}

ScColorSpace ScColorMgmtEngine::createColorSpace(ScColorProfile& profile, eColorFormat colorFormat)
{
	return m_data->createColorSpace(profile, colorFormat);
}

bool ScColorMgmtEngine::operator==(const ScColorMgmtEngine& other) const
{
	return m_data == other.m_data;
}

bool ScColorMgmtEngine::operator!=(const ScColorMgmtEngine& other) const
{
	return m_data != other.m_data;
}