/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef STORYLOADER_H
#define STORYLOADER_H

#include <QByteArray>
#include <QHash>
#include <QList>
#include <QMap>
#include <QObject>
#include <QString>
#include <QStringList>

#include "scribusapi.h"

class QDomElement;
class QProgressBar;
class ScribusDoc;
class ScribusView;
class SCFonts;
class StoryText;
class PrefsManager;
class FileFormat;

class SCRIBUS_API StoryLoader : public QObject
{
	Q_OBJECT

public:
	StoryLoader();
	~StoryLoader() override = default;

	int testData(const QByteArray& storyData);

	bool loadStory(const QByteArray& storyData, ScribusDoc& doc, StoryText &story, PageItem* item = nullptr);
	bool saveStory(QByteArray& storyData, ScribusDoc& doc, StoryText &story, PageItem* item = nullptr);

	bool postLoad(ScribusDoc* currDoc);
 	void informReplacementFonts();

	const FileFormat * const formatSLA12x;
	const FileFormat * const formatSLA13x;
	const FileFormat * const formatSLA134;
	const FileFormat * const formatSLA150;

private:
	QMap<QString, QString> m_replacedFonts;
	PrefsManager& m_prefsManager;

	bool findFormat(uint formatId, QList<FileFormat>::const_iterator &it);
	bool findFormat(const QByteArray& data, QList<FileFormat>::const_iterator &it);
};

#endif
