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

extern "C" PLUGIN_API void GetText(QString filename, QString encoding, bool textOnly, gtWriter *writer);

extern "C" PLUGIN_API QString FileFormatName();

extern "C" PLUGIN_API QStringList FileExtensions();

class CsvIm 
{
public:
	CsvIm(const QString& fname, const QString& enc, gtWriter *w, const QString& fdelim = ",",
		  const QString& vdelim = "\"", bool m_header = false, bool usevdelim = true);
	~CsvIm();
	void setFieldDelimiter(const QString& fdelim);
	void setValueDelimiter(const QString& vdelim);
	void write();
private:
	QString m_fieldDelimiter;
	QString m_valueDelimiter;
	bool m_hasHeader;
	bool m_useVDelim;
	QString m_filename;
	QString m_encoding;
	gtWriter *m_writer;
	QString m_header;
	QString m_data;
	int m_rowNumber;
	int m_colIndex;
	int m_colCount;
	gtParagraphStyle *m_pstyleData;
	gtParagraphStyle *m_pstyleHeader;
	void loadFile();
	void parseLine(const QString& line, bool isHeader);
	QString toUnicode(const QByteArray& rawText);
	void setupPStyles();
	void setupTabulators();
};

#endif // CSVIM_H
