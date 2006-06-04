/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCRIBUS_FILELOADER_H
#define SCRIBUS_FILELOADER_H
#include <qobject.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qvaluelist.h>
#include <qmap.h>

#include "scribusapi.h"
#include "scribusstructs.h"

class QDomElement;
class QProgressBar;
class ScribusDoc;
class ScribusView;
class SCFonts;
class PrefsManager;
class FileFormat;

class SCRIBUS_API FileLoader : public QObject
{
	Q_OBJECT
public:
	FileLoader(const QString & fileName);
	~FileLoader() {};
	int TestFile();
// 	int CheckScribus();
	bool LoadPage(int PageToLoad, bool Mpage, QString renamedPageName=QString::null);
	bool LoadFile();
	bool SaveFile(const QString& fileName, ScribusDoc *doc, QProgressBar *dia2);
	bool ReadStyles(const QString& fileName, ScribusDoc* doc, QValueList<ParagraphStyle> &docParagraphStyles);
	bool ReadPageCount(const QString& fileName, int *num1, int *num2, QStringList & masterPageNames);
	bool ReadColors(const QString& fileName, ColorList & colors);
	bool ReadLineStyles(const QString& fileName, QMap<QString,multiLine> *Sty);
	bool postLoad();
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
 	bool newReplacement;
 	QValueList<int> LFrames;
	QMap<QString,QString> ReplacedFonts;
	QMap<uint,QString> DoVorl;
	uint VorlC;
	QPtrList<Foi> dummyFois;

	static const QString getLoadFilterString();
private:
 	void readParagraphStyle(ParagraphStyle& vg, const QDomElement& pg, SCFonts &avail, ScribusDoc *doc);
	bool findFormat(uint formatId, QValueList<FileFormat>::const_iterator &it);
	PrefsManager* prefsManager;
	double maximumX;
	double maximumY;
};

#endif
