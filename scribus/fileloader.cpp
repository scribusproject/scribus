#include "fileloader.h"
#include <qfile.h>
#include <qfileinfo.h>
#include "scribus.h"
#include "scribusXml.h"
#if (_MSC_VER >= 1200)
 	#include "win-config.h"
#else
	#include "config.h"
#endif
#ifdef HAVE_LIBZ
	#include <zlib.h>
#endif
extern bool loadText(QString nam, QString *Buffer);

/*!
 \fn FileLoader::FileLoader(QString fileName)
 \author Franz Schmid
 \date
 \brief Constructor, sets the variable "FileName" to the input parameter fileName
 \param QString fileName
 \retval None
 */
FileLoader::FileLoader(QString fileName)
{
	FileName = fileName;
	FileType = -1;
}

/*!
 \fn int FileLoader::TestFile()
 \author Franz Schmid
 \date
 \brief Tests if the file "FileName" exists and determines the type of the file.
 \param None
 \retval int -1 if the file doesn't exist or any other error has occured, 0 for the old Format, 1 for the new Format, 2 for EPS and PS files, 3 for SVG files and 4 for PDF files
 */
int FileLoader::TestFile()
{
	QFileInfo fi = QFileInfo(FileName);
	int ret = -1;
	if (!fi.exists())
		ret = -1;
	QString ext = fi.extension(true).lower();
	if ((ext == "sla.gz") || (ext == "sla") || (ext == "scd.gz") || (ext == "scd"))
		ret = CheckScribus();
	if ((ext == "ps") || (ext == "eps"))
		ret = 2;
	if ((ext == "svg") || (ext == "svgz"))
		ret = 3;
/*	if (ext == "pdf")
		ret = 4; */
	FileType = ret;
	return ret;
}

/*!
 \fn int FileLoader::CheckScribus()
 \author Franz Schmid
 \date
 \brief Checks which Scribus fileformat the File in FileName has.
 \param None
 \retval 0 for old Scribus files, 1 for Scribus files created by Scribus 1.3 and above
 */
int FileLoader::CheckScribus()
{
	int ret = -1;
	QString fText = ReadDatei(FileName);
	if (fText == "")
		return ret;
	if ((fText.startsWith("<SCRIBUS")) && (fText.contains("<PAGE ", TRUE) != 0))
		return 0;
	if ((fText.startsWith("<SCRIBUSUTF8NEW")) && (fText.contains("<PAGE ", TRUE) != 0))
		return 1;
	return ret;
}

/*!
 \fn QString FileLoader::ReadDatei(QString fileName)
 \author Franz Schmid
 \date
 \brief Auxiliary function loads the file "fileName" into a QString and does conversion from UTF8 if required
 \param QString fileName
 \retval QString Contents of the file, converted from UTF8
 */
QString FileLoader::ReadDatei(QString fileName)
{
/**
  * added to support gz docs
  * 2.7.2002 C.Toepp
  * <c.toepp@gmx.de>
  */
	QString f = "";
#ifdef HAVE_LIBZ
	if(fileName.right(2) == "gz")
	{
		gzFile gzDoc;
		char buff[4097];
		int i;
		gzDoc = gzopen(fileName.latin1(),"rb");
		if(gzDoc == NULL)
			return "";
		while((i = gzread(gzDoc,&buff,4096)) > 0)
		{
			buff[i] = '\0';
			f.append(buff);
		}
		gzclose(gzDoc);
	}
	else
		loadText(fileName, &f);
#else
	loadText(fileName, &f);
#endif
	QString ff = "";
	if (f.startsWith("<SCRIBUSUTF8"))
		ff = QString::fromUtf8(f);
	else if (f.startsWith("<SCRIBUS"))
		ff = f;
	return ff;
/** end changes */
}

/*!
 \fn bool FileLoader::LoadFile(ScribusApp* app)
 \author Franz Schmid
 \date
 \brief Loads the file "FileName" as a Scribus document
 \param ScribusApp* app, pointer to the main application class
 \retval bool true when loading is succsessful, false otherwise
 */
bool FileLoader::LoadFile(ScribusApp* app)
{
	bool ret = false;
	switch (FileType)
	{
		case 0:
			{
				ScriXmlDoc *ss = new ScriXmlDoc();
				QObject::connect(ss, SIGNAL(NewPage(int)), app, SLOT(slotNewPage(int)));
				ret = ss->ReadDoc(FileName, app->Prefs.AvailFonts, app->doc, app->view, app->FProg);
				QObject::disconnect(ss, SIGNAL(NewPage(int)), app, SLOT(slotNewPage(int)));
				delete ss;
			}
			break;
		case 2:
			app->DLLinput = FileName;
			app->CallDLL( 6 );
			ret = true;
			break;
		case 3:
			app->DLLinput = FileName;
			app->CallDLL( 10 );
			ret = true;
			break;
		default:
			ret = false;
			break;
	}
	return ret;
}
