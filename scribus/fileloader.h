#ifndef SCRIBUS_FILELOADER_H
#define SCRIBUS_FILELOADER_H
#include <qobject.h>
#include <qstring.h>
#include <q3valuelist.h>
#include <qmap.h>
//Added by qt3to4:
#include <Q3PtrList>

#include "scribusapi.h"
#include "scribusstructs.h"

class QDomElement;
class Q3ProgressBar;
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
	bool LoadPage(int PageToLoad, bool Mpage);
	bool LoadFile();
	bool ReadPage(const QString & fileName, SCFonts &avail, ScribusDoc *doc, int PageToLoad, bool Mpage);
	bool ReadDoc(const QString & fileName, SCFonts &avail, ScribusDoc *doc, Q3ProgressBar *dia2);
	void GetItemText(QDomElement *it, ScribusDoc *doc, PageItem* obj, bool impo=false, bool VorLFound=false);
	PageItem* PasteItem(QDomElement *obj, ScribusDoc *doc);
	void GetStyle(QDomElement *pg, struct ParagraphStyle *vg, Q3ValueList<ParagraphStyle> &docParagraphStyles, ScribusDoc* doc, bool fl);
	bool postLoad(bool is12doc = false);
	void informReplacementFonts();
	QString FileName;
	int FileType;
	bool havePS;
	bool haveSVG;
	bool haveSXD;
	bool newReplacement;
	Q3ValueList<int> LFrames;
	QMap<QString,QString> ReplacedFonts;
	QMap<uint,QString> DoVorl;
	uint VorlC;
	Q3PtrList<Foi> dummyFois;

	static const QString getLoadFilterString();
	static QString readSLA(const QString & fileName);
private:
	PrefsManager* prefsManager;
	double maximumX;
	double maximumY;
};

#endif
