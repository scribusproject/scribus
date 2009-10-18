/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <cassert>
#include "sccolormngtengine.h"

ScColorMngtEngine::ScColorMngtEngine(ScColorMngtEngineData* data) : m_data(data)
{
	assert(data != NULL);
}

int ScColorMngtEngine::engineID() const
{
	return m_data->engineID();
}

const QString& ScColorMngtEngine::description() const
{
	return m_data->description();
}

const ScColorMngtStrategy& ScColorMngtEngine::strategy() const
{
	return m_data->strategy();
}

void ScColorMngtEngine::setStrategy(const ScColorMngtStrategy& strategy)
{
	m_data->setStrategy(strategy);
}

QList<ScColorProfileInfo> ScColorMngtEngine::getAvailableProfileInfo(const QString& directory, bool recursive)
{
	return m_data->getAvailableProfileInfo(directory, recursive);
}

ScColorProfile ScColorMngtEngine::openProfileFromFile(const QString& filePath)
{
	return m_data->openProfileFromFile(*this, filePath);
}

ScColorProfile ScColorMngtEngine::openProfileFromMem( const QByteArray& array)
{
	return m_data->openProfileFromMem(*this, array);
}

ScColorProfile ScColorMngtEngine::createProfile_sRGB()
{
	return m_data->createProfile_sRGB(*this);
}

ScColorProfile ScColorMngtEngine::createProfile_Lab()
{
	return m_data->createProfile_Lab(*this);
}

ScColorTransform ScColorMngtEngine::createTransform(const ScColorProfile& inputProfile , eColorFormat inputFormat,
	                                         const ScColorProfile& outputProfile, eColorFormat outputFormat, 
                                             eRenderIntent renderIntent, long transformFlags)
{
	return m_data->createTransform(*this, inputProfile, inputFormat, outputProfile, outputFormat, renderIntent, transformFlags);
}

ScColorTransform ScColorMngtEngine::createProofingTransform(const ScColorProfile& inputProfile, eColorFormat inputFormat,
	                                         const ScColorProfile& outputProfile, eColorFormat outputFormat,
											 const ScColorProfile& proofingProfile, eRenderIntent renderIntent, 
                                             eRenderIntent proofingIntent, long transformFlags)
{
	return m_data->createProofingTransform(*this, inputProfile, inputFormat, outputProfile, outputFormat, 
	                                       proofingProfile, renderIntent, proofingIntent, transformFlags);
}

bool ScColorMngtEngine::operator==(const ScColorMngtEngine& other) const
{
	return m_data == other.m_data;
}

bool ScColorMngtEngine::operator!=(const ScColorMngtEngine& other) const
{
	return m_data != other.m_data;
}