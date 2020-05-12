/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCRIBUS_FILELOADER_H
#define SCRIBUS_FILELOADER_H
#include <QObject>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QHash>
#include <QList>

#include "scribusapi.h"
#include "styles/styleset.h"
#include "styles/paragraphstyle.h"
#include "styles/charstyle.h"

class QDomElement;
class QProgressBar;
class ScribusDoc;
class ScribusView;
class SCFonts;
class PrefsManager;
class FileFormat;
class multiLine;

class SCRIBUS_API FileLoader : public QObject
{
	Q_OBJECT
public:
	FileLoader(const QString & fileName);
	~FileLoader() override = default;

	int fileType() const { return m_fileType; }
	int testFile();

	bool loadPage(ScribusDoc* currDoc, int PageToLoad, bool Mpage, const QString& renamedPageName=QString());
	bool loadFile(ScribusDoc* currDoc);
	bool saveFile(const QString& fileName, ScribusDoc *doc, QString *savedFile = nullptr);
	bool readStyles(ScribusDoc* doc, StyleSet<ParagraphStyle> &docParagraphStyles);
	bool readCharStyles(ScribusDoc* doc, StyleSet<CharStyle> &docCharStyles);
	bool readPageCount(int *num1, int *num2, QStringList & masterPageNames);
	bool readColors(ColorList & colors);
	bool readLineStyles(QHash<QString, multiLine> *Sty);
	QImage readThumbnail();
	bool postLoad(ScribusDoc* currDoc);
 	void informReplacementFonts();

	const FileFormat * const formatSLA12x;
	const FileFormat * const formatSLA13x;
	const FileFormat * const formatSLA134;
	const FileFormat * const formatSLA150;

	static const QString getLoadFilterString();

private:
	QMap<QString, QString> m_ReplacedFonts;
	bool m_newReplacement;
	void readParagraphStyle(ParagraphStyle& vg, const QDomElement& pg, SCFonts &avail, ScribusDoc *doc);
	bool findFormat(uint formatId, QList<FileFormat>::const_iterator &it);
	PrefsManager& m_prefsManager;

	QString m_fileName;
	int m_fileType;
};

#endif
