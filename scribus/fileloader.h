#ifndef _FILELOADER_
#define _FILELOADER_
#include <qstring.h>
#include <qvaluelist.h>
#include <qmap.h>
#include <qdom.h>
#include <qprogressbar.h>
#include "scribusstructs.h"
class ScribusDoc;
class ScribusView;
class ScribusApp;
class SCFonts;

class FileLoader
{
public:
	FileLoader(QString fileName, ScribusApp* app);
	~FileLoader() {};
	int TestFile();
	int CheckScribus();
	QString ReadDatei(QString fileName);
	bool LoadFile(ScribusApp* app);
	bool ReadDoc(ScribusApp* app, QString fileName, SCFonts &avail, ScribusDoc *doc, ScribusView *view, QProgressBar *dia2);
	QString AskForFont(SCFonts &avail, QString fStr, ApplicationPrefs *Prefs, ScribusDoc *doc);
	QString GetItemText(QDomElement *it, ScribusDoc *doc, ApplicationPrefs *Prefs, bool VorLFound, bool impo);
	QString FileName;
	int FileType;
	bool havePS;
	bool haveSVG;
	bool haveSXD;
	QValueList<int> LFrames;
	QMap<QString,QString> DoFonts;
	QMap<QString,QString> ReplacedFonts;
	QMap<uint,QString> DoVorl;
};

#endif
