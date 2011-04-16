/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <cassert>
#include "scconfig.h"
#include "sccolormgmtengine.h"

#ifdef HAVE_LCMS21
#include "sclcms2colormgmtengineimpl.h"
#else
#include "sclcmscolormgmtengineimpl.h"
#endif

QSharedPointer<ScColorMgmtEngineData> ScColorMgmtEngine::m_data;

QSharedPointer<ScColorMgmtEngineData> ScColorMgmtEngine::data()
{
#ifdef HAVE_LCMS21
	if (!m_data)
		m_data = QSharedPointer<ScColorMgmtEngineData>( new ScLcms2ColorMgmtEngineImpl() );
#else
	if (!m_data)
		m_data = QSharedPointer<ScColorMgmtEngineData>( new ScLcmsColorMgmtEngineImpl() );
#endif
	assert( !m_data.isNull() );
	return m_data;
}

const QString& ScColorMgmtEngine::description()
{
	return data()->description();
}

QList<ScColorProfileInfo> ScColorMgmtEngine::getAvailableProfileInfo(const QString& directory, bool recursive)
{
	return data()->getAvailableProfileInfo(directory, recursive);
}

ScColorProfile ScColorMgmtEngine::openProfileFromFile(const QString& filePath)
{
	return data()->openProfileFromFile(filePath);
}

ScColorProfile ScColorMgmtEngine::openProfileFromMem( const QByteArray& array)
{
	return data()->openProfileFromMem(array);
}

ScColorProfile ScColorMgmtEngine::createProfile_sRGB()
{
	return data()->createProfile_sRGB();
}

ScColorProfile ScColorMgmtEngine::createProfile_Lab()
{
	return data()->createProfile_Lab();
}

ScColorTransform ScColorMgmtEngine::createTransform(const ScColorProfile& inputProfile , eColorFormat inputFormat,
	                                         const ScColorProfile& outputProfile, eColorFormat outputFormat, 
                                             eRenderIntent renderIntent, long transformFlags)
{
	return data()->createTransform(inputProfile, inputFormat, outputProfile, outputFormat, renderIntent, transformFlags);
}

ScColorTransform ScColorMgmtEngine::createProofingTransform(const ScColorProfile& inputProfile, eColorFormat inputFormat,
	                                         const ScColorProfile& outputProfile, eColorFormat outputFormat,
											 const ScColorProfile& proofingProfile, eRenderIntent renderIntent, 
                                             eRenderIntent proofingIntent, long transformFlags)
{
	return data()->createProofingTransform(inputProfile, inputFormat, outputProfile, outputFormat, 
	                                       proofingProfile, renderIntent, proofingIntent, transformFlags);
}
