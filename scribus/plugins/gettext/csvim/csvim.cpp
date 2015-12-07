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

void GetText(QString filename, QString encoding, bool /* textOnly */, gtWriter *writer)
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
	m_fieldDelimiter = fdelim;
	m_valueDelimiter = vdelim;
	m_hasHeader = header;
	m_useVDelim = usevdelim;
	m_filename = fname;
	m_encoding = enc;
	m_writer = w;
	header = "";
	m_data = "";
	m_rowNumber = 0;
	m_colIndex = 0;
	m_colCount = 0;
	setupPStyles();
	loadFile();
	setupTabulators();
}

void CsvIm::setupPStyles()
{
	m_pstyleData = new gtParagraphStyle(*(m_writer->getDefaultStyle()));
	m_pstyleData->setName(m_writer->getFrameName() + "-" + QObject::tr("CSV_data"));
	if (m_hasHeader)
	{
		m_pstyleHeader = new gtParagraphStyle(*m_pstyleData);
		m_pstyleHeader->setName(m_writer->getFrameName() + "-" + QObject::tr("CSV_header"));
		m_pstyleHeader->setSpaceBelow(7.0);
		int size = m_pstyleData->getFont()->getSize();
		size += 10;
		m_pstyleHeader->getFont()->setSize(size);
		m_pstyleHeader->getFont()->setWeight(BOLD);
	}
	else
		m_pstyleHeader = NULL;
}

void CsvIm::setFieldDelimiter(const QString& fdelim)
{
	m_fieldDelimiter = fdelim;
}

void CsvIm::setValueDelimiter(const QString& vdelim)
{
	m_valueDelimiter = vdelim;
}

void CsvIm::write()
{
	m_writer->append(m_header, m_pstyleHeader);
	m_writer->append(m_data, m_pstyleData);
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
	if (loadRawText(m_filename, rawText))
		text = toUnicode(rawText);

	QStringList lines = text.split("\n", QString::SkipEmptyParts);
	uint i=0;
	if (m_hasHeader)
	{
		m_colIndex = 0;
		parseLine(lines[0], true);
		m_header += "\n";
		m_colCount = m_colIndex;
		i = 1;
		++m_rowNumber;
	}
	for (int i2 = i; i2 < lines.size(); ++i2)
	{
		m_colIndex = 0;
		parseLine(lines[i2], false);
		m_data += "\n";
		++m_rowNumber;
		if (m_colCount < m_colIndex)
			m_colCount = m_colIndex;
	}
	if (m_data.startsWith("\t"))
		m_data.remove(0,1);
	m_data.replace("\n\t","\n");
}

void CsvIm::parseLine(const QString& line, bool isHeader)
{
	if ((line.indexOf(m_valueDelimiter) < 0) || (!m_useVDelim))
	{
		QStringList l = line.split(m_fieldDelimiter, QString::SkipEmptyParts);
		for (int i = 0; i < l.size(); ++i)
		{
			++m_colIndex;
			QString tmp = l[i].trimmed();
			if (isHeader)
				m_header += "\t" + tmp;
			else
				m_data += "\t" + tmp;
		}
		return; // line done
	}

	int vdIndexStart = line.indexOf(m_valueDelimiter);
	int vdIndexEnd   = line.indexOf(m_valueDelimiter, vdIndexStart + 1);
	if (vdIndexEnd < 0)
	{
		if (isHeader)
			m_header += "\t" + line;
		else
			m_data += "\t" + line;
		return; // error in line, no closing valuedelimiter could be found
	}

	int fdIndex = line.indexOf(m_fieldDelimiter, vdIndexEnd + 1);
	QString tmpCol = "";

	if (fdIndex < 0)
	{
		if (vdIndexEnd < 0)
		{
			if (isHeader)
				m_header += "\t" + line;
			else
				m_data += "\t" + line;
		}
		else 
		{
			tmpCol = line.mid(vdIndexStart + 1, (vdIndexEnd - 1) - vdIndexStart);
			if (isHeader)
				m_header += "\t" + tmpCol;
			else
				m_data += "\t" + tmpCol;
		}
		++m_colIndex;
		return; // no more field delimiters left
	}

	if (fdIndex < vdIndexStart)
	{
		tmpCol = line.mid(0, fdIndex);
		if (isHeader)
			m_header += "\t" + tmpCol;
		else
			m_data += "\t" + tmpCol;
		++m_colIndex;
		parseLine(line.mid(fdIndex + 1, line.length() - (fdIndex + 1)), isHeader);
	}
	else if (fdIndex > vdIndexEnd)
	{
		tmpCol = line.mid(vdIndexStart + 1, (vdIndexEnd - 1) - vdIndexStart);
		if (isHeader)
			m_header += "\t" + tmpCol;
		else
			m_data += "\t" + tmpCol;
		++m_colIndex;
		parseLine(line.mid(vdIndexEnd + 1, line.length() - (vdIndexEnd + 1)), isHeader);
	}
}


void CsvIm::setupTabulators()
{
	double frameWidth = m_writer->getFrameWidth();
	double addition = frameWidth / (m_colCount + 1);
	double curValue = addition / 2;
	for (int i = 0; i < m_colCount; ++i)
	{
		m_pstyleData->setTabValue(curValue);
		if (m_pstyleHeader)
			m_pstyleHeader->setTabValue(curValue, CENTER_T);
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
	if (m_encoding.isEmpty())
		codec = QTextCodec::codecForLocale();
	else
		codec = QTextCodec::codecForName(m_encoding.toLocal8Bit());
	QString unistr = codec->toUnicode(rawText);
	return unistr;
}

CsvIm::~CsvIm()
{
	delete m_pstyleData;
	delete m_pstyleHeader;
}
