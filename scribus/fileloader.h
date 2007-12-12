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
	~FileLoader() {};
	int TestFile();
// 	int CheckScribus();
	bool LoadPage(ScribusDoc* currDoc, int PageToLoad, bool Mpage, QString renamedPageName=QString::null);
	bool LoadFile(ScribusDoc* currDoc);
	bool SaveFile(const QString& fileName, ScribusDoc *doc, QString *savedFile = NULL);
	bool ReadStyles(const QString& fileName, ScribusDoc* doc, StyleSet<ParagraphStyle> &docParagraphStyles);
	bool ReadCharStyles(const QString& fileName, ScribusDoc* doc, StyleSet<CharStyle> &docCharStyles);
	bool ReadPageCount(const QString& fileName, int *num1, int *num2, QStringList & masterPageNames);
	bool ReadColors(const QString& fileName, ColorList & colors);
	bool ReadLineStyles(const QString& fileName, QMap<QString,multiLine> *Sty);
	bool postLoad(ScribusDoc* currDoc);
 	void informReplacementFonts();
	QString FileName;
	int FileType;
	const FileFormat * const formatSLA12x;
	const FileFormat * const formatSLA13x;
	const FileFormat * const formatSLA134;
	const FileFormat * const formatPS;
	const FileFormat * const formatSVG;
	const FileFormat * const formatSXD;
	const FileFormat * const formatODG;
 	QList<int> LFrames;
	QMap<uint,QString> DoVorl;
	uint VorlC;

	static const QString getLoadFilterString();
private:
	QMap<QString, QString> ReplacedFonts;
	bool newReplacement;
	void readParagraphStyle(ParagraphStyle& vg, const QDomElement& pg, SCFonts &avail, ScribusDoc *doc);
	bool findFormat(uint formatId, QList<FileFormat>::const_iterator &it);
	PrefsManager* prefsManager;
	double maximumX;
	double maximumY;
};

#endif
