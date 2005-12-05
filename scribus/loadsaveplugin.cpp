#include "loadsaveplugin.h"

QValueList<FileFormat> LoadSavePlugin::formats;

LoadSavePlugin::LoadSavePlugin()
	: ScPlugin()
{
}

LoadSavePlugin::~LoadSavePlugin()
{
}

// STATIC method - return a list of all existing formats
const QValueList<FileFormat> & LoadSavePlugin::supportedFormats()
{
	return formats;
}

const FileFormat * LoadSavePlugin::getFormatById(const int id)
{
	QValueList<FileFormat>::iterator it(findFormat(id));
	if (it == formats.end())
		return 0;
	else
		return &(*it);
}

const QStringList LoadSavePlugin::fileDialogLoadFilter()
{
	return getDialogFilter(true);
}

const QStringList LoadSavePlugin::fileDialogSaveFilter()
{
	return getDialogFilter(false);
}

const QStringList LoadSavePlugin::getDialogFilter(bool forLoad)
{
	QValueList<FileFormat>::const_iterator it(formats.constBegin());
	QValueList<FileFormat>::const_iterator itEnd(formats.constEnd());
	QStringList filterList;
	// We know the list is sorted by id, then priority, so we can just take the
	// highest priority entry for each ID, and we can start with the first entry
	// in the list.
	filterList.append((*it).filter);
	unsigned int lastID = (*it).formatId;
	++it;
	for ( ; it != itEnd ; ++it )
		// Find the next load/save (as appropriate) plugin for the next format type
		if ( (forLoad ? (*it).load : (*it).save) && ((*it).formatId > lastID) )
		{
			// And add it to the filter list, since we know it's 
			// the highest priority because of the sort order.
			filterList.append((*it).filter);
			lastID = (*it).formatId;
		}
	filterList.append(tr("All Files (*)"));
	return filterList;
}

bool LoadSavePlugin::saveFile(const QString & /* fileName */,
							  const FileFormat & /* fmt */)
{
	return false;
}

bool LoadSavePlugin::loadFile(const QString & /* fileName */,
							  const FileFormat & /* fmt */)
{
	return false;
}

void LoadSavePlugin::registerFormat(const FileFormat & fmt)
{
	// We insert the format in a very specific location so that the formats
	// list is sorted by ascending id, then descending priority.
	// We first look for entries with equal or greater ID, then equal or
	// lesser priority, and insert before the first item that is of either:
	//     - Equal ID and lesser or equal priority; or
	//     - Greater ID
	// If we don't find one, we insert before the end iterator, ie append.
	QValueList<FileFormat>::iterator it(formats.begin());
	QValueList<FileFormat>::iterator itEnd(formats.end());
	while (it != itEnd)
	{
		if ( ( ((*it).formatId == fmt.formatId) && ((*it).priority <= fmt.priority) ) ||
			 ((*it).formatId > fmt.formatId)) 
				break;
		++it;
	}
	formats.insert(it, fmt);
	//printFormatList(); // DEBUG
}

// static debugging function - prints the human readable format list
void LoadSavePlugin::printFormatList()
{
	qDebug("Current format list:");
	QValueList<FileFormat>::const_iterator it(formats.constBegin());
	QValueList<FileFormat>::const_iterator itEnd(formats.constEnd());
	for ( ; it != itEnd ; ++it )
	{
		qDebug("    Format: Id: %3u, Prio: %3hu, Name: %s",
				(*it).formatId, (*it).priority, (*it).trName.local8Bit().data() );
	}
	qDebug("Done");
}

void LoadSavePlugin::unregisterFormat(unsigned int id)
{
	QValueList<FileFormat>::iterator it(findFormat(id, this));
	Q_ASSERT(it != formats.end());
	formats.remove(it);
}

void LoadSavePlugin::unregisterAll()
{
	QValueList<FileFormat>::iterator it(formats.begin());
	QValueList<FileFormat>::iterator itEnd(formats.end());
	while (it != itEnd)
	{
		if ((*it).plug == this)
			it = formats.remove(it);
		else
			++it;
	}
}

QValueList<FileFormat>::iterator
LoadSavePlugin::findFormat(unsigned int id,
						   LoadSavePlugin* plug,
						   QValueList<FileFormat>::iterator it)
{
	QValueList<FileFormat>::iterator itEnd(formats.end());
	for ( ; it != itEnd ; ++it )
	{
		if (
				((*it).formatId == id) &&
				((plug == 0) || (plug == (*it).plug))
			)
			return it;
	}
	return itEnd;
}



bool FileFormat::loadFile(const QString & fileName) const
{
	return (plug && load) ? plug->loadFile(fileName, *this) : false;
}

bool FileFormat::saveFile(const QString & fileName) const
{
	return (plug && save) ? plug->saveFile(fileName, *this) : false;
}
