#ifndef _FILELOADER_
#define _FILELOADER_
#include <qstring.h>
#include <qvaluelist.h>
#include <qmap.h>
#include <qdom.h>
#include <qprogressbar.h>
#include <scribus.h>

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
	QString AskForFont(SCFonts &avail, QString fStr, preV *Prefs, ScribusDoc *doc);
	QString GetItemText(QDomElement *it, ScribusDoc *doc, preV *Prefs, bool VorLFound, bool impo);
	QString FileName;
	int FileType;
	bool havePS;
	bool haveSVG;
	bool haveSXD;
	QValueList<int> LFrames;
	QMap<QString,QString> DoFonts;
	QMap<uint,QString> DoVorl;
};

#endif
