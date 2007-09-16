/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PDBIM_H
#define PDBIM_H

#include "pluginapi.h"

class gtWriter;
class gtParagraphStyle;
class QString;
class QStringList;

extern "C" PLUGIN_API void GetText(QString filename, QString encoding, bool textOnly, gtWriter *writer);

extern "C" PLUGIN_API QString FileFormatName();

extern "C" PLUGIN_API QStringList FileExtensions();




/*! \brief Abiword's internal data types */
typedef unsigned int UT_uint32;
typedef unsigned short UT_uint16;

/*! \brief Define integral type Byte, Word, and DWord to match those on the
Pilot being 8, 16, and 32 bits, respectively. Max  8-bit unsigned */
typedef unsigned char Byte;
typedef UT_uint16 Word;
typedef UT_uint32 DWord;

/*! \brief Pilots have a fixed 4K record size */
#define RECORD_SIZE_MAX	4096
#define BUFFER_SIZE 4096
#define COUNT_BITS 3
#define DISP_BITS 11
#define	DOC_CREATOR "REAd"
#define	DOC_TYPE "TEXt"
/*! \brief 31 chars + 1 null terminator */
#define	dmDBNameLength 32

/*! \brief PDB document header
http://www.pyrite.org/doc_format.html
version  2 bytes  0x0002 if data is compressed, 0x0001 if uncompressed
spare  2 bytes  purpose unknown (set to 0 on creation)
length  4 bytes  total length of text before compression
records  2 bytes  number of text records
record_size  2 bytes  maximum size of each record (usually 4096; see below)
position  4 bytes  currently viewed position in the document
sizes  2*records bytes  record size array
78 bytes total
*/
typedef struct
{
	char	name[ dmDBNameLength ];
	Word	attributes;
	Word	version;
	DWord	create_time;
	DWord	modify_time;
	DWord	backup_time;
	DWord	modificationNumber;
	DWord	appInfoID;
	DWord	sortInfoID;
	char	type[4];
	char	creator[4];
	DWord	id_seed;
	DWord	nextRecordList;
	Word	numRecords;
} pdb_header;

/*! \brief Some compilers pad structures out to DWord boundaries so using 
sizeof() doesn't give the intended result.
*/
#define PDB_HEADER_SIZE 78
#define	PDB_RECORD_HEADER_SIZE 8

/*! \brief PDB Document record.
16 bytes total. */
typedef struct {
	Word    version; /* 1 = plain text, 2 = compressed */
	Word    reserved1;
	DWord   doc_size; /* in bytes, when uncompressed */
	Word    numRecords; /* text rec's only; = pdb_header.numRecords-1 */
	Word    rec_size; /* usually RECORD_SIZE_MAX */
	DWord   reserved2;
} doc_record0;

/*! \brief Binary buffer */
typedef struct {
	Byte buf[BUFFER_SIZE];
	UT_uint32   len;
	UT_uint32   position;
} buffer;

#define GET_Word(f,n)   { fread( &n, 2, 1, f ); n = swap_Word ( n ); }
#define GET_DWord(f,n)  { fread( &n, 4, 1, f ); n = swap_DWord( n ); }

/*! \brief An import filter for Palm Documents (PDB files).
PDB documents are simple non-formatted texts in binary forms used
mainly for e-books distribution.
It's taken from Abiword's PDB import/export plugin http://www.abisource.com
I've simplified the importer guts to fit special Scribus needs.
\warning This plugin uses a very special low-level IO and memory operations.
It uses big/little endian handling for bit related operations.
Welcome in the "macro and gtk hell" ;)
\note User should specify file encoding correctly.
\author Copyright (C) 2001 AbiSource, Inc.
\author Petr Vanek <petr@scribus.info>
*/
class PdbIm
{
public:
	/*! \brief Parse and decode the PDB file
	\param fname a file name of the document
	\param enc user selected text encoding. See encoding attr.
	\param w a reference to the gtWriter instance */
	PdbIm(const QString& fname, const QString& enc, gtWriter *w);
	~PdbIm(){};
	/*! \brief Write data into Scribus text frame.
	User should specify encoding of the imported text - it's recoded here. */
	void write();
private:
	//! \brief Binary buffer for extraction tasks
	buffer *m_buf;
	//! \brief Store the extracted text here
	QString data;
	//! \brief Name of the codec/encoding to recode
	QString encoding;
	//! \brief Imp plugin handler
	gtWriter *writer;
	//! \brief A "bit order" flag. True on little endian systems.
	bool m_littlendian;
	//! \brief A "document uses that strange compress algorithm" flag.
	bool bCompressed;
	
	/*! \brief Parse the PDB file.
	\param fname a filename to open */
	void loadFile(QString fname);
	/*! \brief Learn which endian to use.
	It fills the m_littlendian flag */
	void selectSwap();
	/*! \brief Recompute the binary value for given endian
	\param r a binary Word
	\retval Word a correct endian Word */
	Word swap_Word(Word r);
	/*! \brief Recompute the binary value for given endian
	\param r a binary DWord
	\retval DWord a correct endian Word */
	DWord swap_DWord(DWord r);
	/*! \brief PDB strange decompress algorithm implementation.
	\param m_buf a reference to the current buffer */
	void uncompress(buffer *m_buf);
};

#endif
