#ifndef _FILELOADER_
#define _FILELOADER_
#include <qstring.h>
class ScribusApp;

class FileLoader
{
public:
	FileLoader(QString fileName, ScribusApp* app);
	~FileLoader() {};
	int TestFile();
	int CheckScribus();
	QString ReadDatei(QString fileName);
	bool LoadFile(ScribusApp* app);
	QString FileName;
	int FileType;
	bool havePS;
	bool haveSVG;
};

#endif
