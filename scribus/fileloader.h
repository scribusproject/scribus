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

class SCRIBUS_API FileLoader : public QObject
{
public:
	FileLoader(const QString & fileName);
	~FileLoader() {};
	int TestFile();
	int CheckScribus();
	QString ReadDatei(QString fileName);
	bool LoadPage(int PageToLoad, bool Mpage);
	bool LoadFile();
	bool ReadPage(const QString & fileName, SCFonts &avail, ScribusDoc *doc, ScribusView *view, int PageToLoad, bool Mpage);
	bool ReadDoc(const QString & fileName, SCFonts &avail, ScribusDoc *doc, ScribusView *view, QProgressBar *dia2);
	void GetItemText(QDomElement *it, ScribusDoc *doc, PageItem* obj, bool impo=false, bool VorLFound=false);
	PageItem* PasteItem(QDomElement *obj, ScribusDoc *doc, ScribusView *view);
	void GetStyle(QDomElement *pg, struct ParagraphStyle *vg, QValueList<ParagraphStyle> &docParagraphStyles, ScribusDoc* doc, bool fl);
	QString FileName;
	int FileType;
	bool havePS;
	bool haveSVG;
	bool haveSXD;
	bool newReplacement;
	QValueList<int> LFrames;
	QMap<QString,QString> ReplacedFonts;
	QMap<uint,QString> DoVorl;
	uint VorlC;
	QPtrList<Foi> dummyFois;

	static const QString FileLoader::getLoadFilterString();
private:
	PrefsManager* prefsManager;
};

#endif
