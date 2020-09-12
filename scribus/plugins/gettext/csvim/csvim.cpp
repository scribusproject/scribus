/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "csvim.h"
#include "gtwriter.h"
#include "gtparagraphstyle.h"
#include "gtframestyle.h"
#include "scribusstructs.h"
#include "util.h"

QString FileFormatName()
{
    return QObject::tr("Comma Separated Value Files");
}

QStringList FileExtensions()
{
    return QStringList("csv");
}

void GetText(const QString& filename, const QString& encoding, bool /* textOnly */, gtWriter *writer)
{
	CsvDialog* csvdia = new CsvDialog();
	if (csvdia->exec())
	{
		CsvIm *cim = new CsvIm(filename, encoding, writer, csvdia->getFDelim(), csvdia->getVDelim(), csvdia->hasHeader(), csvdia->useVDelim());
		cim->write();
		delete cim;
	}
	delete csvdia;
}

/******* Class CsvIm **************************************************************/

CsvIm::CsvIm(const QString& fname, const QString& enc, gtWriter *w,
			 const QString& fdelim, const QString& vdelim, bool hasheader, bool usevdelim) :
	fieldDelimiter(fdelim),
	valueDelimiter(vdelim),
	hasHeader(hasheader),
	useVDelim(usevdelim),
	filename(fname),
	encoding(enc),
	writer(w)
{
	setupPStyles();
	loadFile();
	setupTabulators();
}

void CsvIm::setupPStyles()
{
	pstyleData = new gtParagraphStyle(writer->getDefaultStyle()->asGtParagraphStyle());
	pstyleData->setName(writer->getFrameName() + "-" + QObject::tr("CSV_data"));
	if (!hasHeader)
		return;
	pstyleHeader = new gtParagraphStyle(*pstyleData);
	pstyleHeader->setName(writer->getFrameName() + "-" + QObject::tr("CSV_header"));
	pstyleHeader->setSpaceBelow(7.0);
	int size = pstyleData->getFont()->getSize();
	size += 10;
	pstyleHeader->getFont()->setSize(size);
	pstyleHeader->getFont()->setWeight(BOLD);
}

void CsvIm::setFieldDelimiter(const QString& fdelim)
{
	fieldDelimiter = fdelim;
}

void CsvIm::setValueDelimiter(const QString& vdelim)
{
	valueDelimiter = vdelim;
}

void CsvIm::write()
{
	writer->append(header, pstyleHeader);
	writer->append(data, pstyleData);
}

void CsvIm::loadFile()
{
	QString text = "";
	/*
	QFile f(filename);
	QFileInfo fi(f);
	if (!fi.exists())
		return;
	QByteArray bb(f.size(), ' ');
	if (f.open(QIODevice::ReadOnly))
	{
		f.read(bb.data(), f.size());
		f.close();
		for (int posi = 0; posi < bb.size(); ++posi)
			text += QChar(bb[posi]);
	}
	text = toUnicode(text);
	*/
	QByteArray rawText;
	if (loadRawText(filename, rawText))
		text = toUnicode(rawText);

	QStringList lines = text.split("\n", QString::SkipEmptyParts);
	uint i=0;
	if (hasHeader)
	{
		colIndex = 0;
		parseLine(lines[0], true);
		header += "\n";
		colCount = colIndex;
		i = 1;
		++rowNumber;
	}
	for (int i2 = i; i2 < lines.size(); ++i2)
	{
		colIndex = 0;
		parseLine(lines[i2], false);
		data += "\n";
		++rowNumber;
		if (colCount < colIndex)
			colCount = colIndex;
	}
	if (data.startsWith("\t"))
		data.remove(0,1);
	data.replace("\n\t","\n");
}

void CsvIm::parseLine(const QString& line, bool isHeader)
{
	if ((line.indexOf(valueDelimiter) < 0) || (!useVDelim))
	{
		QStringList l = line.split(fieldDelimiter, QString::SkipEmptyParts);
		for (int i = 0; i < l.size(); ++i)
		{
			++colIndex;
			QString tmp = l[i].trimmed();
			if (isHeader)
				header += "\t" + tmp;
			else
				data += "\t" + tmp;
		}
		return; // line done
	}

	int vdIndexStart = line.indexOf(valueDelimiter);
	int vdIndexEnd   = line.indexOf(valueDelimiter, vdIndexStart + 1);
	if (vdIndexEnd < 0)
	{
		if (isHeader)
			header += "\t" + line;
		else
			data += "\t" + line;
		return; // error in line, no closing valuedelimiter could be found
	}

	int fdIndex = line.indexOf(fieldDelimiter, vdIndexEnd + 1);
	QString tmpCol = "";

	if (fdIndex < 0)
	{
		if (vdIndexEnd < 0)
		{
			if (isHeader)
				header += "\t" + line;
			else
				data += "\t" + line;
		}
		else 
		{
			tmpCol = line.mid(vdIndexStart + 1, (vdIndexEnd - 1) - vdIndexStart);
			if (isHeader)
				header += "\t" + tmpCol;
			else
				data += "\t" + tmpCol;
		}
		++colIndex;
		return; // no more field delimiters left
	}

	if (fdIndex < vdIndexStart)
	{
		tmpCol = line.mid(0, fdIndex);
		if (isHeader)
			header += "\t" + tmpCol;
		else
			data += "\t" + tmpCol;
		++colIndex;
		parseLine(line.mid(fdIndex + 1, line.length() - (fdIndex + 1)), isHeader);
	}
	else if (fdIndex > vdIndexEnd)
	{
		tmpCol = line.mid(vdIndexStart + 1, (vdIndexEnd - 1) - vdIndexStart);
		if (isHeader)
			header += "\t" + tmpCol;
		else
			data += "\t" + tmpCol;
		++colIndex;
		parseLine(line.mid(vdIndexEnd + 1, line.length() - (vdIndexEnd + 1)), isHeader);
	}
}


void CsvIm::setupTabulators()
{
	double frameWidth = writer->getFrameWidth();
	double addition = frameWidth / (colCount + 1);
	double curValue = addition / 2;
	for (int i = 0; i < colCount; ++i)
	{
		pstyleData->setTabValue(curValue);
		if (pstyleHeader)
			pstyleHeader->setTabValue(curValue, CENTER_T);
		curValue += addition;
	}
}
/*
QString CsvIm::toUnicode(const QString& text)
{
	QTextCodec *codec;
	if (encoding.isEmpty())
		codec = QTextCodec::codecForLocale();
	else
		codec = QTextCodec::codecForName(encoding.toLocal8Bit());
	QString dec = codec->toUnicode(text.toLocal8Bit());
	return dec;
}
*/
QString CsvIm::toUnicode(const QByteArray& rawText)
{
	QTextCodec *codec;
	if (encoding.isEmpty())
		codec = QTextCodec::codecForLocale();
	else
		codec = QTextCodec::codecForName(encoding.toLocal8Bit());
	QString unistr = codec->toUnicode(rawText);
	return unistr;
}

CsvIm::~CsvIm()
{
	delete pstyleData;
	delete pstyleHeader;
}
