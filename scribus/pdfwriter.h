//
//  pdfwriter.h
//  Scribus
//
//  Created by Andreas Vox on 25.12.14.
//
//

#ifndef Scribus_pdfwriter_h
#define Scribus_pdfwriter_h

#include <QByteArray>
#include <QDataStream>
#include <QFile>
#include <QDateTime>
#include <QList>
#include <QRect>
#include <QString>

#include "pdfoptions.h"
#include "pdfstructs.h"
#include "scstreamfilter.h"

namespace Pdf
{
	
	
	bool isWhiteSpace(char c);
	bool isDelimiter(char c);
	bool isRegular(char c);
	
	/**
	 convert unicode to 8-Bit PdfDocEncoding, cf. PDF32000-2008, 8.9.2 and Annex D
	 */
	uchar toPdfDocEncoding(QChar c);

	/**
	 convert unicode to 8-Bit PdfDocEncoding, cf. PDF32000-2008, 8.9.2 and Annex D
	 */
	QByteArray toPdfDocEncoding(QString s);

	/**
	 convert 8-Bit PdfDocEncoding to unicode, cf. PDF32000-2008, 8.9.2 and Annex D
	 */
	int fromPDFDocEncoding(int code);
	
	/**
	 convert unicode to UTF16BE with byte order marker, cf. PDF32000-2008, 8.9.2
	 */
	QByteArray toUTF16(QString s);
	
	/**
	 convert to 7-Bit ASCII, undefined for characters outside 0-127
	 */
	QByteArray toAscii(QString s);

	/**
	 Cf. PDF32000-2008, 7.3.2
	 */
	QByteArray toPdf(bool v);
	
	/**
	 Cf. PDF32000-2008, 7.3.3
	 */
	QByteArray toPdf(int v);
	
	/**
	 Cf. PDF32000-2008, 7.3.3
	 */
	QByteArray toPdf(uint v);
	
	/**
	 Cf. PDF32000-2008, 7.3.3
	 */
	QByteArray toPdf(qlonglong v);
	
	/**
	 Cf. PDF32000-2008, 7.3.3
	 */
	QByteArray toPdf(qulonglong v);
	
	/**
	 Cf. PDF32000-2008, 7.3.3
	 */
	QByteArray toPdf(double v);
	
	/**
	 Cf. PDF32000-2008, 7.3.3
	 */
	QByteArray toObjRef(PdfId id);
	
	/**
	 convert to a PDF literal string in parentheses. Escape (, ) \ as necessary.
	 Cf. PDF32000-2008, 7.3.4.2
	 */
	QByteArray toLiteralString(QByteArray s);
	QByteArray toLiteralString(QString s);
	
	/**
	 convert to PDF hex string in <> brackets. Cf. PDF32000-2008, 7.3.4.3
	 */
	QByteArray toHexString(QByteArray s);
	
	/**
	 convert to a one-byte PDF hex string <hh>. Cf. PDF32000-2008, 7.3.4.3
	 */
	QByteArray toHexString8(quint8 b);
	
	/**
	 convert to a two-byte PDF hex string <hh>. Cf. PDF32000-2008, 7.3.4.3
	 */
	QByteArray toHexString16(quint16 b);
	
	/**
	 convert to a four-byte PDF hex string <hh>. Cf. PDF32000-2008, 7.3.4.3
	 */
	QByteArray toHexString32(quint32 b);
	
	/**
	 convert to a PDF 8-Bit name with preceding /.
	 Escapes delimiters and chars > 127 with # as necessaray.
	 Cf. PDF32000-2008, 7.3.5
	 */
	QByteArray toName(QByteArray s);
	QByteArray toName(QString s);

	/**
	 converts to a PDF date string, cf. PDF32000-2008, 8.9.4
	 */
	QByteArray toDateString(QDateTime dt);
	
	/**
	 converts to a PDF rectangle, cf. PDF32000-2008, 8.9.5
	 */
	QByteArray toRectangleArray(QRect r);
	QByteArray toRectangleArray(QRectF r);
	
	
	
	
/**
 * This class handles output to PDF files. 
 * It implements the "Syntax" section of the PDF specification:
 * - lexical conventions
 * - file structure
 * - indirect objects and XRef table
 * - streams and encryption
 * - page tree
 * - article threads
 * - optional content groups
 * - resource dictionaries
 * - graphic states
 * - named destinations
 * - form and javascript definitions
 */
class Writer
{
public:
	Writer();
	
	// file handling
	bool open (const QString& filename);
	QDataStream& getOutStream() { return m_outStream; }
	bool close(bool aborted);
	qint64 bytesWritten() { return m_Spool.pos(); }
	
	// encryption
	void setFileId(const QByteArray& id);
	void setEncryption(bool keylen16, const QByteArray& ownerKey, const QByteArray& userKey, int permissions);
	QByteArray encryptBytes(const QByteArray& in, PdfId objNum);
	
	QByteArray ComputeRC4Key(PdfId ObjNum);
private:
	void CalcOwnerKey(const QByteArray & Owner, const QByteArray & User);
	void CalcUserKey(const QByteArray & User, int Permission);
	QByteArray FitKey(const QByteArray & pass);
public:
	
	// writing
	void writeHeader(PDFOptions::PDFVersion vers);
	void writeXrefAndTrailer();
	void write(const QByteArray& bytes);
	void write(const Pdf::ResourceDictionary& dict);
	void write(const PdfFont font);

	// objects
	PdfId objectCounter() const { return m_ObjCounter; }
	PdfId reserveObjects(unsigned int n);
	
	PdfId newObject() { return reserveObjects(1); }

	void startObj(PdfId id);
	
	PdfId startObj() {
		PdfId res = newObject(); startObj(res); return res;
	}
	
	void endObj(PdfId id);
	void endObjectWithStream(bool encrypted, PdfId id, const QByteArray& streamContent);
	ScStreamFilter* openStreamFilter(bool encrypted, PdfId objId);
	
	
	// private:
	PdfId CatalogObj; // 1
	PdfId InfoObj; // 2
	PdfId PagesObj; // 3
	PdfId AcroFormObj;
	PdfId DestsObj; 
	PdfId OutlinesObj;
	PdfId NamesObj;
	PdfId ThreadsObj;
	PdfId OCPropertiesObj; // (9)
	PdfId OutputIntentObj; // (9 or 10)
	PdfId EncryptObj; 
	PdfId MetaDataObj;
	PdfId ResourcesObj;
	PdfId OpenActionObj;
	
private:
	PdfId m_ObjCounter;
	PdfId m_CurrentObj;
	
	QFile m_Spool;
	QDataStream m_outStream;
	
	QList<qint64> m_XRef;
	
	QByteArray m_KeyGen;
	QByteArray m_OwnerKey;
	QByteArray m_UserKey;
	QByteArray m_FileID;
	QByteArray m_EncryKey;
	int m_KeyLen;

};

}
#endif
