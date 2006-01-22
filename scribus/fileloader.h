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
	int CheckScribus();
	bool LoadPage(int PageToLoad, bool Mpage);
	bool LoadFile();
	bool ReadPage(const QString & fileName, SCFonts &avail, ScribusDoc *doc, int PageToLoad, bool Mpage);
	bool ReadDoc(const QString & fileName, SCFonts &avail, ScribusDoc *doc, QProgressBar *dia2);
	void GetItemText(QDomElement *it, ScribusDoc *doc, PageItem* obj, bool impo=false, bool VorLFound=false);
	PageItem* PasteItem(QDomElement *obj, ScribusDoc *doc);
	void GetStyle(QDomElement *pg, struct ParagraphStyle *vg, QValueList<ParagraphStyle> &docParagraphStyles, ScribusDoc* doc, bool fl);
	bool postLoad(bool is12doc = false);
	void informReplacementFonts();
	QString FileName;
	int FileType;
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
	static QString readSLA(const QString & fileName);
private:

	PrefsManager* prefsManager;
	double maximumX;
	double maximumY;
};

#endif
