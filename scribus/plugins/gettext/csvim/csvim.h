#ifndef CSVIM_H
#define CSVIM_H

#include "pluginapi.h"
#include "csvdia.h"
#include <gtwriter.h>
#include <gtparagraphstyle.h>
#include <qcstring.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qtextcodec.h>

extern "C" PLUGIN_API void GetText(QString filename, QString encoding, bool textOnly, gtWriter *writer);

extern "C" PLUGIN_API QString FileFormatName();

extern "C" PLUGIN_API QStringList FileExtensions();

class CsvIm 
{
public:
	CsvIm(const QString& fname, const QString& enc, gtWriter *w, const QString& fdelim = ",",
          const QString& vdelim = "\"", bool header = false, bool usevdelim = true);
	~CsvIm();
	void setFieldDelimiter(const QString& fdelim);
	void setValueDelimiter(const QString& vdelim);
	void write();
private:
	QString fieldDelimiter;
	QString valueDelimiter;
	bool hasHeader;
	bool useVDelim;
	QString filename;
	QString encoding;
	gtWriter *writer;
	QString header;
	QString data;
	int rowNumber;
	int colIndex;
	int colCount;
	gtParagraphStyle *pstyleData;
	gtParagraphStyle *pstyleHeader;
	void loadFile();
	void parseLine(const QString& line, bool isHeader);
	QString toUnicode(const QString& text);
	void setupPStyles();
	void setupTabulators();
};

#endif // CSVIM_H
