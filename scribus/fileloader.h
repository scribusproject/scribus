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

	int  fileType() const { return m_fileType; }
	int  testFile();

	bool loadPage(ScribusDoc* currDoc, int PageToLoad, bool Mpage, QString renamedPageName=QString::null);
	bool loadFile(ScribusDoc* currDoc);
	bool saveFile(const QString& fileName, ScribusDoc *doc, QString *savedFile = NULL);
	bool readStyles(ScribusDoc* doc, StyleSet<ParagraphStyle> &docParagraphStyles);
	bool readCharStyles(ScribusDoc* doc, StyleSet<CharStyle> &docCharStyles);
	bool readPageCount(int *num1, int *num2, QStringList & masterPageNames);
	bool readColors(ColorList & colors);
	bool readLineStyles(QMap<QString, multiLine> *Sty);
	bool postLoad(ScribusDoc* currDoc);
 	void informReplacementFonts();
	
	const FileFormat * const formatSLA12x;
	const FileFormat * const formatSLA13x;
	const FileFormat * const formatSLA134;
	const FileFormat * const formatPS;
	const FileFormat * const formatSVG;
	const FileFormat * const formatSXD;
	const FileFormat * const formatODG;

	static const QString getLoadFilterString();
private:
	QMap<QString, QString> ReplacedFonts;
	bool newReplacement;
	void readParagraphStyle(ParagraphStyle& vg, const QDomElement& pg, SCFonts &avail, ScribusDoc *doc);
	bool findFormat(uint formatId, QList<FileFormat>::const_iterator &it);

	PrefsManager* prefsManager;

	QString m_fileName;
	int     m_fileType;
};

#endif
