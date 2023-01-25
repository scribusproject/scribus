/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <cstdlib>
#include <cmath>

#include <QApplication>
#include <QCheckBox>
#include <QCursor>
#include <QDir>
#include <QDomElement>
#include <QFile>
#include <QFileInfo>
#include <QList>
#include <QMessageBox>
#include <QProgressBar>
#include <QRegExp>
#include <QtAlgorithms>
#include <QTextCodec>
#include <QTextStream>

#include "scconfig.h"

#include "commonstrings.h"
#include "loadsaveplugin.h"
#include "pageitem.h"
#include "pluginmanager.h"
#include "prefsmanager.h"
#include "resourcecollection.h"
#include "scribuscore.h"
#include "scribusXml.h"
#include "storyloader.h"
#include "units.h"
#include "util.h"

#include "plugins/formatidlist.h"
#include "text/storytext.h"
#include "ui/fontreplacedialog.h"
#include "ui/missing.h"

/*!
 \author Franz Schmid
 \date
 \brief Constructor, sets the variable "FileName" to the input parameter fileName
 \param fileName filename to load
 \retval None
 */
StoryLoader::StoryLoader() :
	QObject(nullptr),
	formatSLA12x(LoadSavePlugin::getFormatById(FORMATID_SLA12XIMPORT)),
	formatSLA13x(LoadSavePlugin::getFormatById(FORMATID_SLA13XIMPORT)),
	formatSLA134(LoadSavePlugin::getFormatById(FORMATID_SLA134IMPORT)),
	formatSLA150(LoadSavePlugin::getFormatById(FORMATID_SLA150IMPORT)),
	m_prefsManager(PrefsManager::instance())
{
}

/*!
 \fn int StoryLoader::TestFile()
 \author Franz Schmid
 \date
 \brief Tests if the file "FileName" exists and determines the type of the file.
 \retval int -1 if the file doesn't exist or any other error has occurred, 0 for the old Format, 1 for the new Format, 2 for EPS and PS files, 3 for SVG files and 4 for PDF files
 */
int StoryLoader::testData(const QByteArray& storyData) const
{
	int ret = -1;

	bool found = false;
	QList<FileFormat> fileFormats(LoadSavePlugin::supportedFormats());
	QList<FileFormat>::const_iterator it(fileFormats.constBegin());
	QList<FileFormat>::const_iterator itEnd(fileFormats.constEnd());
	for ( ; (it != itEnd) && (!found); ++it)
	{
		if (!it->plug)
			continue;
		if (it->plug->storySupported(storyData))
		{
			ret = it->formatId;
			found = true;
			break;
		}
	}

	return ret;
}

/*!
 \fn bool StoryLoader::LoadFile(ScribusDoc* currDoc)
 \author Franz Schmid
 \date
 \brief Loads the file "FileName" as a Scribus document
 \param currDoc the current document
 \retval bool true when loading is succsessful, false otherwise
 */
bool StoryLoader::loadStory(const QByteArray& storyData, ScribusDoc& doc, StoryText &story, PageItem* item)
{
	m_replacedFonts = doc.AllFonts->getSubstitutions();

	QList<FileFormat>::const_iterator it;
	if (!findFormat(storyData, it))
		return false;

	bool ret = false;
	switch (it->formatId)
	{
		case FORMATID_SLA150IMPORT:
			{
				it->setupTargets(&doc, nullptr, doc.scMW(), doc.scMW()->mainWindowProgressBar, &(m_prefsManager.appPrefs.fontPrefs.AvailFonts));
				ret = it->loadStory(storyData, story, item);
			}
			break;
		default:
			it->setupTargets(&doc, doc.view(), doc.scMW(), doc.scMW()->mainWindowProgressBar, &(m_prefsManager.appPrefs.fontPrefs.AvailFonts));
			ret = it->loadStory(storyData, story, item);
			break;
	}
	return ret;
}

bool StoryLoader::saveStory(QByteArray& storyData, ScribusDoc& doc, StoryText &story, PageItem* item) const
{
	QList<FileFormat>::const_iterator it;
	if (!findFormat(FORMATID_SLA150EXPORT, it))
		return false;

	it->setupTargets(&doc, doc.view(), doc.scMW(), doc.scMW()->mainWindowProgressBar, &(m_prefsManager.appPrefs.fontPrefs.AvailFonts));
	
	bool ret = it->saveStory(story, item, storyData);
	return ret;
}

bool StoryLoader::postLoad(ScribusDoc* currDoc)
{
	m_replacedFonts = currDoc->AllFonts->getSubstitutions(m_replacedFonts.keys());
	if (m_replacedFonts.isEmpty())
		return true;

	if (!(m_prefsManager.appPrefs.fontPrefs.askBeforeSubstitute))
		return true;

	QApplication::changeOverrideCursor(QCursor(Qt::ArrowCursor));
	FontReplaceDialog dia(nullptr, &m_replacedFonts);
	if (!dia.exec())
		return false;

	QMap<QString, QString>::Iterator itfsu;
	for (itfsu = m_replacedFonts.begin(); itfsu != m_replacedFonts.end(); ++itfsu)
	{
		if (dia.stickyReplacements->isChecked())
			m_prefsManager.appPrefs.fontPrefs.GFontSub[itfsu.key()] = itfsu.value();
	}
	currDoc->AllFonts->setSubstitutions(m_replacedFonts, currDoc);
	ResourceCollection repl;
	repl.availableFonts = currDoc->AllFonts;
	repl.mapFonts(m_replacedFonts);
	currDoc->replaceNamedResources(repl);
	return true;
}

void StoryLoader::informReplacementFonts() const
{
	if (m_replacedFonts.count() == 0)
		return;

	QApplication::changeOverrideCursor(QCursor(Qt::ArrowCursor));
	QString mess = tr("Some fonts used by this document have been substituted:") + "\n\n";
	for (auto it = m_replacedFonts.begin(); it != m_replacedFonts.end(); ++it)
	{
		mess += it.key() + tr(" was replaced by: ") + it.value() +"\n";
	}
	ScMessageBox::warning(ScCore->primaryMainWindow(), CommonStrings::trWarning, mess);
}

bool StoryLoader::findFormat(uint formatId, QList<FileFormat>::const_iterator &it) const
{
	QList<FileFormat> fileFormats(LoadSavePlugin::supportedFormats());
	it = fileFormats.constBegin();
	QList<FileFormat>::const_iterator itEnd(fileFormats.constEnd());
	for (it = fileFormats.constBegin(); it != itEnd ; ++it)
	{
		if (formatId == it->formatId)
			return true;
	}
	return false;
}

bool StoryLoader::findFormat(const QByteArray& data, QList<FileFormat>::const_iterator &it) const
{
	QList<FileFormat> fileFormats(LoadSavePlugin::supportedFormats());
	it = fileFormats.constBegin();
	QList<FileFormat>::const_iterator itEnd(fileFormats.constEnd());
	for ( ; it != itEnd ; ++it)
	{
		if (it->plug && it->plug->storySupported(data))
			return true;
	}
	return false;
}