/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "csvim.h"

QString FileFormatName()
{
    return QObject::tr("Comma Separated Value Files");
}

QStringList FileExtensions()
{
    return QStringList("csv");
}

void GetText(QString filename, QString encoding, bool textOnly, gtWriter *writer)
{
	CsvDialog* csvdia = new CsvDialog();
	if (csvdia->exec())
	{
		CsvIm *cim = new CsvIm(filename, encoding, writer, csvdia->getFDelim(), csvdia->getVDelim(),
                               csvdia->hasHeader(), csvdia->useVDelim());
		cim->write();
		delete cim;
	}
	delete csvdia;
}

/******* Class CsvIm **************************************************************/

CsvIm::CsvIm(const QString& fname, const QString& enc, gtWriter *w, 
             const QString& fdelim, const QString& vdelim, bool header, bool usevdelim)
{
	fieldDelimiter = fdelim;
	valueDelimiter = vdelim;
	hasHeader = header;
	useVDelim = usevdelim;
	filename = fname;
	encoding = enc;
	writer = w;
	header = "";
	data = "";
	rowNumber = 0;
	colIndex = 0;
	colCount = 0;
	setupPStyles();
	loadFile();
	setupTabulators();
}

void CsvIm::setupPStyles()
{
	pstyleData = new gtParagraphStyle(*(writer->getDefaultStyle()));
	pstyleData->setName(writer->getFrameName() + "-" + QObject::tr("CSV_data"));
	if (hasHeader)
	{
		pstyleHeader = new gtParagraphStyle(*pstyleData);
		pstyleHeader->setName(writer->getFrameName() + "-" + QObject::tr("CSV_header"));
		pstyleHeader->setSpaceBelow(7.0);
		int size = pstyleData->getFont()->getSize();
		size += 10;
		pstyleHeader->getFont()->setSize(size);
		pstyleHeader->getFont()->setWeight(BOLD);
	}
	else
		pstyleHeader = NULL;
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
	QFile f(filename);
	QFileInfo fi(f);
	if (!fi.exists())
		return;
	uint posi;
	QByteArray bb(f.size());
	if (f.open(IO_ReadOnly))
	{
		f.readBlock(bb.data(), f.size());
		f.close();
		for (posi = 0; posi < bb.size(); ++posi)
			text += bb[posi];
	}
	text = toUnicode(text);
	QStringList lines = QStringList::split("\n", text);
	uint i;
	if (hasHeader)
	{
		colIndex = 0;
		parseLine(lines[0], true);
		header += "\n";
		colCount = colIndex;
		i = 1;
		++rowNumber;
	}
	else
		i = 0;
	for (uint i2 = i; i2 < lines.size(); ++i2)
	{
		colIndex = 0;
		parseLine(lines[i2], false);
		data += "\n";
		++rowNumber;
		if (colCount < colIndex)
			colCount = colIndex;
	}
}

void CsvIm::parseLine(const QString& line, bool isHeader)
{
	if ((line.find(valueDelimiter) < 0) || (!useVDelim))
	{
		QStringList l = QStringList::split(fieldDelimiter, line);
		for (uint i = 0; i < l.size(); ++i)
		{
			++colIndex;
			QString tmp = l[i].stripWhiteSpace();
			if (isHeader)
				header += "\t" + tmp;
			else
				data += "\t" + tmp;
		}
		return; // line done
	}
	
	int vdIndexStart = line.find(valueDelimiter);
	int vdIndexEnd   = line.find(valueDelimiter, vdIndexStart + 1);
	if (vdIndexEnd < 0)
	{
		if (isHeader)
			header += "\t" + line;
		else
			data += "\t" + line;
		return; // error in line, no closing valuedelimiter could be found	
	}
	
	int fdIndex = line.find(fieldDelimiter, vdIndexEnd + 1);
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

QString CsvIm::toUnicode(const QString& text)
{
	QTextCodec *codec;
	if (encoding == "")
		codec = QTextCodec::codecForLocale();
	else
		codec = QTextCodec::codecForName(encoding);
	QString dec = codec->toUnicode(text);
	return dec;
}

CsvIm::~CsvIm()
{
	delete pstyleData;
	delete pstyleHeader;
}
