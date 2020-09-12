/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QApplication>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QProgressBar>
#include <QString>
#include <QStringList>
#include <QTextCodec>
#include <QCursor>

#include "pdbim.h"
#include "gtwriter.h"
#include "gtparagraphstyle.h"
#include "scribusstructs.h"
#include "scribuscore.h"
#include "ui/scmessagebox.h"


/*! \brief A dummy 0 filler (helper procedure)
\param p binary structure to fill with 0
\param len how much 0 to go */
static void _zero_fill(  Byte *p,  int len )
{
	while ( len-- > 0 )
		*p++ = '\0';
}

QString FileFormatName()
{
	return QObject::tr("Palm PDB Documents", "PDB Importer");
}

QStringList FileExtensions()
{
	return QStringList("pdb");
}

void GetText(const QString& filename, const QString& encoding, bool /* textOnly */, gtWriter *writer)
{
	if (filename.isNull())
		return;
	qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
	ScCore->primaryMainWindow()->mainWindowProgressBar->reset();
	PdbIm *im = new PdbIm(filename, encoding, writer);
	im->write();
	delete im;
	ScCore->primaryMainWindow()->mainWindowProgressBar->reset();
	qApp->restoreOverrideCursor();
}


PdbIm::PdbIm(const QString& fname, const QString& enc, gtWriter *w)
{
	m_buf = new buffer;
	_zero_fill(m_buf->buf, BUFFER_SIZE);
	m_buf->len = BUFFER_SIZE;
	m_buf->position = 0;
	writer = w;
	encoding = enc;
	selectSwap();
	loadFile(fname);
}

PdbIm::~PdbIm()
{
	delete m_buf;
}

void PdbIm::write()
{
	QTextCodec *codec;
	if (encoding.isEmpty())
		codec = QTextCodec::codecForLocale();
	else
		codec = QTextCodec::codecForName(encoding.toLocal8Bit());
	data = codec->toUnicode(data.toLocal8Bit());
	// Applying default style is of very limited use with 135svn style system
	/*gtParagraphStyle *pstyle = new gtParagraphStyle(writer->getDefaultStyle()->asGtParagraphStyle());
	pstyle->setName(writer->getFrameName() + "-" + QObject::tr("PDB_data", "PDB Importer"));
	writer->append(data, pstyle);
	delete pstyle;*/
	writer->appendUnstyled(data);
}

void PdbIm::loadFile(const QString& fname)
{
	FILE *m_pdfp = fopen(fname.toLocal8Bit(), "rb");
	pdb_header m_header;
	doc_record0 m_rec0;

	if (!m_pdfp)
	{
		ScMessageBox::warning(ScCore->primaryMainWindow(), QObject::tr("PDB Import", "PDB Importer"),
							 "<qt>" + QObject::tr("Could not open file %1", "PDB Importer").arg(fname) + "</qt>",
							 QMessageBox::Ok, QMessageBox::NoButton);
		return;
	}
	size_t result = fread( &m_header, PDB_HEADER_SIZE, 1, m_pdfp );
	if (result != 1)
	{
		fclose(m_pdfp);
		return;
	}
	if (strncmp(m_header.type, DOC_TYPE, sizeof(m_header.type) ) ||
		strncmp( m_header.creator, DOC_CREATOR, sizeof(m_header.creator)))
	{
		ScMessageBox::warning(ScCore->primaryMainWindow(), QObject::tr("PDB Import", "PDB Importer"),
							 "<qt>" + QObject::tr("This file is not recognized as a PDB document. Please, report this as a bug if you are sure it is one.", "PDB Importer") + "</qt>",
							 QMessageBox::Ok, QMessageBox::NoButton);
		fclose(m_pdfp);
		return;
	}

	// progressbar
	int num_records = swap_Word( m_header.numRecords ) - 1;
	ScCore->primaryMainWindow()->mainWindowProgressBar->setMaximum(num_records);
	fseek(m_pdfp, PDB_HEADER_SIZE, SEEK_SET);
	DWord offset;
	GET_DWord(m_pdfp, offset);
	fseek(m_pdfp, offset, SEEK_SET);
	result = fread(&m_rec0, sizeof(m_rec0), 1, m_pdfp);
	if (result != 1)
	{
		fclose(m_pdfp);
		return;
	}
	if (swap_Word( m_rec0.version ) == 2 )
		bCompressed = true;

	fseek( m_pdfp, 0, SEEK_END );
	DWord file_size = ftell( m_pdfp );
	for (int rec_num = 1; rec_num <= num_records; ++rec_num )
	{
		DWord next_offset;

		ScCore->primaryMainWindow()->mainWindowProgressBar->setValue(rec_num);
		fseek( m_pdfp, PDB_HEADER_SIZE + PDB_RECORD_HEADER_SIZE * rec_num, SEEK_SET);
		GET_DWord( m_pdfp, offset );
		if (rec_num < num_records)
		{
			fseek( m_pdfp, PDB_HEADER_SIZE + PDB_RECORD_HEADER_SIZE * (rec_num + 1), SEEK_SET);
			GET_DWord( m_pdfp, next_offset );
		}
		else
			next_offset = file_size;
		fseek( m_pdfp, offset, SEEK_SET );
		// be overly cautious here
		_zero_fill (m_buf->buf, BUFFER_SIZE);
		m_buf->position = fread( m_buf->buf, 1, next_offset - offset, m_pdfp );

		if ( bCompressed )
			uncompress( m_buf );

		m_buf->position = 0;
		while ( (m_buf->position) < (m_buf->len) )
		{
			if (m_buf->buf[m_buf->position] == '\0')
			{
				++m_buf->position;
				continue;
			}
			data += m_buf->buf[m_buf->position];
			++m_buf->position;
		} 
	}
	fclose(m_pdfp);
}

void PdbIm::selectSwap()
{
	union { char c[2];  Word n; }  w;
	strncpy(  w.c, "\1\2",     2 );

	if ( w.n == 0x0201 )
		m_littlendian = true;
	else
		m_littlendian = false;

}  

Word PdbIm::swap_Word( Word r )
{
	if (m_littlendian)
		return (r >> 8) | (r << 8);
	return r;
}

DWord PdbIm::swap_DWord( DWord r )
{
	if (m_littlendian)
		return ( (r >> 24) & 0x00FF ) | (r << 24) | ( (r >> 8) & 0xFF00 ) | ( (r << 8) & 0xFF0000 );
	return r;
}

void PdbIm::uncompress( buffer *m_buf )
{
	buffer *m_new_buf = new buffer;
	UT_uint16 i, j;
	Byte c;

	_zero_fill (m_new_buf->buf, BUFFER_SIZE);

	for (i = j = 0; i < m_buf->position && j < BUFFER_SIZE; )
	{
		c = m_buf->buf[ i++ ];

		if ( c >= 1 && c <= 8 )
			while ( c-- && j < BUFFER_SIZE-1)
				m_new_buf->buf[ j++ ] = m_buf->buf[ i++ ];

		else if ( c <= 0x7F )
			m_new_buf->buf[ j++ ] = c;

		else if ( c >= 0xC0 && j < BUFFER_SIZE-2)
		{
			m_new_buf->buf[ j++ ] = ' ';
			m_new_buf->buf[ j++ ] = c ^ 0x80;
		}
		else
		{
			int di, n;
			unsigned int temp_c = c;
			// c--> temp_c //tomy 2001.11.13 
			temp_c = (temp_c << 8) ;
			temp_c = temp_c + m_buf->buf[ i++ ];
			di = (temp_c & 0x3FFF) >> COUNT_BITS;
			for (n = (temp_c & ((1 << COUNT_BITS) - 1)) + 3; n-- && j < BUFFER_SIZE; ++j )
				m_new_buf->buf[ j ] = m_new_buf->buf[ j - di ];
			temp_c = 0;
		}
	}
	//UT_ASSERT(j <= BUFFER_SIZE);

	memcpy( static_cast<void *>(m_buf->buf), static_cast<void *>(m_new_buf->buf), static_cast<size_t>(j) );

	m_buf->position = j;
	delete( m_new_buf );
}
