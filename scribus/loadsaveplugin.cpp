/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <algorithm>

#include "loadsaveplugin.h"
#include "commonstrings.h"
#include "scribuscore.h"

#include "plugins/formatidlist.h"

#include <QList>
#include <QMessageBox>

QList<FileFormat> LoadSavePlugin::formats;

LoadSavePlugin::LoadSavePlugin() :
	m_Doc(nullptr),
	m_View(nullptr),
	m_ScMW(nullptr),
	m_mwProgressBar(nullptr),
	m_AvailableFonts(nullptr),
	undoManager(UndoManager::instance())
{
}

LoadSavePlugin::~LoadSavePlugin()
{
}

// STATIC method - return a list of all existing formats
const QList<FileFormat> & LoadSavePlugin::supportedFormats()
{
	return formats;
}

const FileFormat * LoadSavePlugin::getFormatById(const int id)
{
	QList<FileFormat>::iterator it(findFormat(id));
	if (it == formats.end())
		return nullptr;
	return &(*it);
}

FileFormat * LoadSavePlugin::getFormatByID(int id)
{
	QList<FileFormat>::iterator it(findFormat(id));
	if (it == formats.end())
		return nullptr;
	return &(*it);
}

FileFormat* LoadSavePlugin::getFormatByExt(const QString& ext)
{
	QList<FileFormat>::iterator it(findFormat(ext));
	if (it == formats.end())
		return nullptr;
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

const QStringList LoadSavePlugin::getExtensionsForColors(const int id)
{
	QList<FileFormat>::const_iterator it(findFormat(id));
	QList<FileFormat>::const_iterator itEnd(formats.constEnd());
	QStringList filterList;
	// We know the list is sorted by id, then priority, so we can just take the
	// highest priority entry for each ID, and we can start with the first entry
	// in the list.
	//First, check if we even have any plugins to load with
	if (it != itEnd)
	{
		if ((it->load) && (it->colorReading))
			filterList.append(it->fileExtensions);
		unsigned int lastID = it->formatId;
		++it;
		for ( ; it != itEnd ; ++it)
		{
			// Find the next load/save (as appropriate) plugin for the next format type
			if (((it->load) && (it->colorReading)) && (it->formatId > lastID))
			{
				// And add it to the filter list, since we know it's 
				// the highest priority because of the sort order.
				filterList.append(it->fileExtensions);
				lastID = it->formatId;
			}
		}
	}
	else
		qDebug("%s", tr("No File Loader Plugins Found").toLocal8Bit().data());
	// Avoid duplicate entries in the list
	QSet<QString> fSet = filterList.toSet();
	filterList = fSet.values();
	std::sort(filterList.begin(), filterList.end());
	return filterList;
}

const QStringList LoadSavePlugin::getExtensionsForImport(const int id)
{
	QList<FileFormat>::const_iterator it(findFormat(id));
	QList<FileFormat>::const_iterator itEnd(formats.constEnd());
	QStringList filterList;
	// We know the list is sorted by id, then priority, so we can just take the
	// highest priority entry for each ID, and we can start with the first entry
	// in the list.
	//First, check if we even have any plugins to load with
	if (it != itEnd)
	{
		if (it->load)
			filterList.append(it->fileExtensions);
		unsigned int lastID = it->formatId;
		++it;
		for ( ; it != itEnd ; ++it)
		{
			// Find the next load/save (as appropriate) plugin for the next format type
			if ((it->load) && (it->formatId > lastID))
			{
				// And add it to the filter list, since we know it's 
				// the highest priority because of the sort order.
				filterList.append(it->fileExtensions);
				lastID = it->formatId;
			}
		}
	}
	else
		qDebug("%s", tr("No File Loader Plugins Found").toLocal8Bit().data());
	// Avoid duplicate entries in the list
	QSet<QString> fSet = filterList.toSet();
	filterList = fSet.toList();
	std::sort(filterList.begin(), filterList.end());
	return filterList;
}

const QStringList LoadSavePlugin::getExtensionsForPreview(const int id)
{
	QList<FileFormat>::const_iterator it(findFormat(id));
	QList<FileFormat>::const_iterator itEnd(formats.constEnd());
	QStringList filterList;
	// We know the list is sorted by id, then priority, so we can just take the
	// highest priority entry for each ID, and we can start with the first entry
	// in the list.
	//First, check if we even have any plugins to load with
	if (it != itEnd)
	{
		if ((it->load) && (it->thumb))
			filterList.append(it->fileExtensions);
		unsigned int lastID = it->formatId;
		++it;
		for ( ; it != itEnd ; ++it)
		{
			// Find the next load/save (as appropriate) plugin for the next format type
			if (((it->load) && (it->thumb)) && (it->formatId > lastID))
			{
				// And add it to the filter list, since we know it's 
				// the highest priority because of the sort order.
				filterList.append(it->fileExtensions);
				lastID = it->formatId;
			}
		}
	}
	else
		qDebug("%s", tr("No File Loader Plugins Found").toLocal8Bit().data());
	// Avoid duplicate entries in the list
	QSet<QString> fSet = filterList.toSet();
	filterList = fSet.toList();
	std::sort(filterList.begin(), filterList.end());
	return filterList;
}

const QStringList LoadSavePlugin::getDialogFilter(bool forLoad)
{
	QList<FileFormat>::const_iterator it(formats.constBegin());
	QList<FileFormat>::const_iterator itEnd(formats.constEnd());
	QStringList filterList;
	// We know the list is sorted by id, then priority, so we can just take the
	// highest priority entry for each ID, and we can start with the first entry
	// in the list.
	//First, check if we even have any plugins to load with
	if (it == itEnd)
	{
		qDebug("%s", tr("No File Loader Plugins Found").toLocal8Bit().data());
		return filterList;
	}
	unsigned int lastID = 0;
	QStringList scribusList;
	while (it != itEnd)
	{
		// Find the next load/save (as appropriate) plugin for the next format type
		if ((forLoad ? it->load : it->save) && (it->formatId > lastID))
		{
			// And add it to the filter list, since we know it's
			// the highest priority because of the sort order.
			// #11294, sort them and keep Scribus ones at the top
			if(it->nativeScribus)
				scribusList.append(it->filter);
			else
				filterList.append(it->filter);
			lastID = it->formatId;
		}
		++it;
	}
	filterList.sort(Qt::CaseInsensitive);
	filterList.append( tr("All Files (*)"));
	return scribusList+filterList;
}

bool LoadSavePlugin::saveFile(const QString & /* fileName */,
							  const FileFormat & /* fmt */)
{
	return false;
}

bool LoadSavePlugin::loadElements(const QString &  /*data*/, const QString&  /*fileDir*/, int /*toLayer*/, double /*Xp_in*/, double /*Yp_in*/, bool /*loc*/)
{
	return false;
}

bool LoadSavePlugin::savePalette(const QString &  /*fileName*/)
{
	return false;
}

QString LoadSavePlugin::saveElements(double /*xp*/, double /*yp*/, double /*wp*/, double  /*hp*/, Selection* /*selection*/, QByteArray & /*prevData*/)
{
	return "";
}

bool LoadSavePlugin::loadPalette(const QString &  /*fileName*/)
{
	return false;
}

void LoadSavePlugin::setFileReadError()
{
	m_lastError = tr("An error occurred while opening file or file is damaged");
}

void LoadSavePlugin::setDomParsingError(const QString& msg, int line, int column)
{
	m_lastError = tr("An error occurred while parsing file at line %1, column %2 :\n%3").arg(line).arg(column).arg(msg);
}

const QString& LoadSavePlugin::lastSavedFile()
{
	return m_lastSavedFile;
}

bool LoadSavePlugin::loadFile(const QString & /* fileName */,
							  const FileFormat & /* fmt */,
							  int /* flags */,
							  int /* index */)
{
	return false;
}

bool LoadSavePlugin::checkFlags(int flags)
{
	int numFlags = 0;
	// Only one of the following flags must be set:
	// lfCreateDoc, lfUseCurrentPage, lfInsertPage
	if( flags & lfCreateDoc ) 
		numFlags++;
	if( flags & lfUseCurrentPage ) 
		numFlags++;
	if( flags & lfInsertPage ) 
		numFlags++;
	return numFlags <= 1;
}

void LoadSavePlugin::registerFormat(FileFormat & fmt)
{
	// We insert the format in a very specific location so that the formats
	// list is sorted by ascending id, then descending priority.
	// We first look for entries with equal or greater ID, then equal or
	// lesser priority, and insert before the first item that is of either:
	//     - Equal ID and lesser or equal priority; or
	//     - Greater ID
	// If we don't find one, we insert before the end iterator, ie append.
	if (fmt.formatId == 0) // only for custom plugins
	{
		uint id;
		if (formats.isEmpty())
			id = FORMATID_FIRSTUSER;
		else
		{
			QList<FileFormat>::iterator it(formats.begin());
			QList<FileFormat>::iterator itEnd(formats.end());
			id = FORMATID_FIRSTUSER - 1;
			while (it != itEnd)
			{
				id = qMax(it->formatId, id);
				++it;
			}
			id++;

//			id = qMax(static_cast<int>(formats.last().formatId), FORMATID_FIRSTUSER-1);
//			id++;
		}
		fmt.formatId = id;
		formats.insert(id, fmt);
	}
	else
	{
		QList<FileFormat>::iterator it(formats.begin());
		QList<FileFormat>::iterator itEnd(formats.end());
		while (it != itEnd)
		{
			if ( ( (it->formatId == fmt.formatId) && (it->priority <= fmt.priority) ) ||
				(it->formatId > fmt.formatId)) 
					break;
			++it;
		}
		formats.insert(it, fmt);
	}
	//qDebug("Format: Id: %3u, Prio: %3hu, Name: %s", fmt.formatId, fmt.priority, fmt.trName.toLocal8Bit().data() );
	//printFormatList(); // DEBUG
}

// static debugging function - prints the human readable format list
void LoadSavePlugin::printFormatList()
{
	qDebug("Current format list:");
	QList<FileFormat>::const_iterator it(formats.constBegin());
	QList<FileFormat>::const_iterator itEnd(formats.constEnd());
	for ( ; it != itEnd ; ++it)
	{
		qDebug("    Format: Id: %3u, Prio: %3hu, Name: %s",  it->formatId, it->priority, it->trName.toLocal8Bit().data() );
	}
	qDebug("Done");
}

void LoadSavePlugin::unregisterFormat(unsigned int id)
{
	QList<FileFormat>::iterator it(findFormat(id, this));
	Q_ASSERT(it != formats.end());
	formats.erase(it);
}

void LoadSavePlugin::unregisterAll()
{
	QList<FileFormat>::iterator it(formats.begin());
	while (it != formats.end())
	{
		if (it->plug == this)
			it = formats.erase(it);
		else
			++it;
	}
}

QList<FileFormat>::iterator
LoadSavePlugin::findFormat(unsigned int id, LoadSavePlugin* plug, QList<FileFormat>::iterator it)
{
	QList<FileFormat>::iterator itEnd(formats.end());
	for ( ; it != itEnd ; ++it)
	{
		if ((it->formatId == id) && ((plug == nullptr) || (plug == it->plug)))
			return it;
	}
	return itEnd;
}

QList<FileFormat>::iterator
LoadSavePlugin::findFormat(const QString& extension, LoadSavePlugin* plug, QList<FileFormat>::iterator it)
{
	QList<FileFormat>::iterator itEnd(formats.end());
	for ( ; it != itEnd ; ++it)
	{
		if ((it->fileExtensions.contains(extension.toLower())) && ((plug == nullptr) || (plug == it->plug)) )
			return it;
	}
	return itEnd;
}


void LoadSavePlugin::setupTargets(ScribusDoc *targetDoc, ScribusView* targetView, ScribusMainWindow* targetMW, QProgressBar* targetMWPRogressBar, SCFonts* targetAvailableFonts)
{
	m_Doc=targetDoc;
	m_View=targetView;
	m_ScMW=targetMW;
	m_mwProgressBar=targetMWPRogressBar;
	m_AvailableFonts=targetAvailableFonts;
}

void LoadSavePlugin::getReplacedFontData(bool & /*getNewReplacement*/, QMap<QString,QString> &/*getReplacedFonts*/, QList<ScFace> &/*getDummyScFaces*/)
{
}

bool LoadSavePlugin::loadPage(const QString& /*fileName*/, int /*pageNumber*/, bool /*Mpage*/, const QString& /*renamedPageName*/)
{
	return false;
}

bool LoadSavePlugin::readStyles(const QString& /*fileName*/, ScribusDoc* /*doc*/, StyleSet<ParagraphStyle> &/*docParagraphStyles*/)
{
	return false;
}

bool LoadSavePlugin::readCharStyles(const QString& /*fileName*/, ScribusDoc* /*doc*/, StyleSet<CharStyle> &/*docCharStyles*/)
{
	return false;
}

bool LoadSavePlugin::readLineStyles(const QString& /*fileName*/, QHash<QString,multiLine>* /*Sty*/)
{
	return false;
}

bool LoadSavePlugin::readColors(const QString& /*fileName*/, ColorList & /*colors*/)
{
	return false;
}

bool LoadSavePlugin::readPageCount(const QString& /*fileName*/, int* /*num1*/, int* /*num2*/, QStringList & /*masterPageNames*/)
{
	return false;
}

QImage LoadSavePlugin::readThumbnail(const QString& /*fileName*/)
{
	return QImage();
}

bool FileFormat::loadFile(const QString & fileName, int flags, int index) const
{
	if (plug && load)
	{
		plug->clearLastError();
		bool success = plug->loadFile(fileName, *this, flags, index);
		if (!success && plug->hasLastError())
		{
			if (ScCore->usingGUI())
			{
				ScMessageBox::warning(ScCore->primaryMainWindow(), CommonStrings::trWarning, plug->lastError());
			}
			else
			{
				qDebug() << plug->lastError();
			}
		}
		return success;
	}
	return false;
}

bool FileFormat::saveFile(const QString & fileName) const
{
	return (plug && save) ? plug->saveFile(fileName, *this) : false;
}

bool FileFormat::savePalette(const QString & fileName) const
{
	return (plug && save) ? plug->savePalette(fileName) : false;
}

QString FileFormat::saveElements(double xp, double yp, double wp, double hp, Selection* selection, QByteArray &prevData) const
{
	return (plug && save) ? plug->saveElements(xp, yp, wp, hp, selection, prevData) : "";
}

FileFormat::FileFormat() :
	formatId(0),
	load(false),
	save(false),
	thumb(false),
	colorReading(false),
	nativeScribus(false),
	priority(0),
	plug(nullptr)
{
}

FileFormat::FileFormat(LoadSavePlugin* plug) :
	formatId(0),
	load(false),
	save(false),
	thumb(false),
	colorReading(false),
	nativeScribus(false),
	priority(0),
	plug(plug)
{
}

bool FileFormat::loadElements(const QString& data, const QString& fileDir, int toLayer, double Xp_in, double Yp_in, bool loc) const
{
	return (plug && load) ? plug->loadElements(data, fileDir, toLayer, Xp_in, Yp_in, loc) : false;
}

bool FileFormat::loadPalette(const QString & fileName) const
{
	return (plug && load) ? plug->loadPalette(fileName) : false;
}

QString FileFormat::lastSavedFile() const
{
	if (plug)
		return plug->lastSavedFile();
	return QString();
}

void FileFormat::setupTargets(ScribusDoc *targetDoc, ScribusView* targetView, ScribusMainWindow* targetMW, QProgressBar* targetMWPRogressBar, SCFonts* targetAvailableFonts) const
{
	if (plug)
		plug->setupTargets(targetDoc, targetView, targetMW, targetMWPRogressBar, targetAvailableFonts);
}

void FileFormat::getReplacedFontData(bool & getNewReplacement, QMap<QString,QString> &getReplacedFonts, QList<ScFace> &getDummyScFaces) const
{
	if (plug)
		plug->getReplacedFontData(getNewReplacement, getReplacedFonts, getDummyScFaces);
}

bool FileFormat::loadPage(const QString & fileName, int pageNumber, bool Mpage, const QString& renamedPageName) const
{
	if (!plug || !load)
		return false;
	plug->clearLastError();
	bool success = plug->loadPage(fileName, pageNumber, Mpage, renamedPageName);
	if (!success && plug->hasLastError())
	{
		if (ScCore->usingGUI())
			ScMessageBox::warning(ScCore->primaryMainWindow(), CommonStrings::trWarning, plug->lastError());
		else
			qDebug() << plug->lastError();
	}
	return success;
}

bool FileFormat::readStyles(const QString& fileName, ScribusDoc* doc, StyleSet<ParagraphStyle> &docParagraphStyles) const
{
	return (plug && load) ? plug->readStyles(fileName, doc, docParagraphStyles) : false;
}

bool FileFormat::readCharStyles(const QString& fileName, ScribusDoc* doc, StyleSet<CharStyle> &docCharStyles) const
{
	return (plug && load) ? plug->readCharStyles(fileName, doc, docCharStyles) : false;
}

bool FileFormat::readLineStyles(const QString& fileName, QHash<QString, multiLine> *Sty) const
{
	return (plug && load) ? plug->readLineStyles(fileName, Sty) : false;
}

bool FileFormat::readColors(const QString& fileName, ColorList & colors) const
{
	return (plug && load) ? plug->readColors(fileName, colors) : false;
}

bool FileFormat::readPageCount(const QString& fileName, int *num1, int *num2, QStringList & masterPageNames) const
{
	return (plug && load) ? plug->readPageCount(fileName, num1, num2, masterPageNames) : false;
}

QImage FileFormat::readThumbnail(const QString& fileName) const
{
	return (plug && load && thumb) ? plug->readThumbnail(fileName) : QImage();
}



