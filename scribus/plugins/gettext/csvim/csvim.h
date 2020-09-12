/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef CSVIM_H
#define CSVIM_H

#include "pluginapi.h"
#include "csvdia.h"
#include <QByteArray>
#include <QFile>
#include <QFileInfo>
#include <QString>
#include <QStringList>
#include <QTextCodec>

class gtParagraphStyle;
class gtWriter;

extern "C" PLUGIN_API void GetText(const QString& filename, const QString& encoding, bool textOnly, gtWriter *writer);

extern "C" PLUGIN_API QString FileFormatName();

extern "C" PLUGIN_API QStringList FileExtensions();

class CsvIm 
{
public:
	CsvIm(const QString& fname, const QString& enc, gtWriter *w, const QString& fdelim = ",",
          const QString& vdelim = "\"", bool hasheader = false, bool usevdelim = true);
	~CsvIm();

	void setFieldDelimiter(const QString& fdelim);
	void setValueDelimiter(const QString& vdelim);
	void write();

private:
	QString fieldDelimiter;
	QString valueDelimiter;
	bool hasHeader { false };
	bool useVDelim { true };
	QString filename;
	QString encoding;
	gtWriter *writer {nullptr};

	QString header;
	QString data;
	int rowNumber {0};
	int colIndex {0};
	int colCount {0};
	gtParagraphStyle *pstyleData {nullptr};
	gtParagraphStyle *pstyleHeader {nullptr};

	void loadFile();
	void parseLine(const QString& line, bool isHeader);
	QString toUnicode(const QByteArray& rawText);
	void setupPStyles();
	void setupTabulators();
};

#endif // CSVIM_H
