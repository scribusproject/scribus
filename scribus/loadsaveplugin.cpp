#include "loadsaveplugin.h"

QValueList<LoadSavePlugin::FormatSupport> LoadSavePlugin::formats;

LoadSavePlugin::LoadSavePlugin()
	: ScActionPlugin()
{
}

LoadSavePlugin::~LoadSavePlugin()
{
}

// STATIC method - return a list of all existing formats
const QValueList<LoadSavePlugin::FormatSupport> & LoadSavePlugin::supportedFormats()
{
	return formats;
}

bool LoadSavePlugin::saveFile(const QString & /* fileName */,
							  const LoadSavePlugin::FormatSupport & /* fmt */)
{
	return false;
}

bool LoadSavePlugin::loadFile(const QString & /* fileName */,
							  const LoadSavePlugin::FormatSupport & /* fmt */)
{
	return false;
}

void LoadSavePlugin::registerFormat(const LoadSavePlugin::FormatSupport & fmt)
{
	// Must be no existing format of that name owned by this plugin
	Q_ASSERT(findFormat(fmt.internalName, this) == formats.end());
	formats.append(fmt);
}

void LoadSavePlugin::unregisterFormat(const QCString & name)
{
	QValueList<LoadSavePlugin::FormatSupport>::iterator it(findFormat(name, this));
	Q_ASSERT(it != formats.end());
	formats.remove(it);
}

void LoadSavePlugin::unregisterAll()
{
	QValueList<LoadSavePlugin::FormatSupport>::iterator it(formats.begin());
	QValueList<LoadSavePlugin::FormatSupport>::iterator itEnd(formats.end());
	while (it != itEnd)
	{
		if ((*it).plug == this)
			it = formats.remove(it);
		else
			++it;
	}
}

QValueList<LoadSavePlugin::FormatSupport>::iterator
LoadSavePlugin::findFormat(const QCString & name,
						   LoadSavePlugin* plug,
						   QValueList<LoadSavePlugin::FormatSupport>::iterator it)
{
	QValueList<LoadSavePlugin::FormatSupport>::iterator itEnd(formats.end());
	for ( ; it != itEnd ; ++it )
	{
		if ( ((*it).internalName == name) && ((plug == 0) || (plug == (*it).plug)) )
			return it;
	}
	return itEnd;
}
