#ifndef _FILELOADER_
#define _FILELOADER_
#include <qstring.h>
class ScribusApp;

class FileLoader
{
public:
	FileLoader(QString fileName);
	~FileLoader() {};
	int TestFile();
	int CheckScribus();
	QString ReadDatei(QString fileName);
	bool LoadFile(ScribusApp* app);
	QString FileName;
	int FileType;
};

#endif
