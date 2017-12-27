//
//  sfnt.h
//  Scribus
//
//  Created by Andreas Vox on 18.04.15.
//
//

#ifndef Scribus_sfnt_h
#define Scribus_sfnt_h

#include "scribusapi.h"

#include <QByteArray>
#include <QList>
#include <QMap>
#include <QString>

#include <ft2build.h>
#include FT_FREETYPE_H



namespace sfnt {
	
	uchar byte(QByteArray const & bb, uint pos);
	quint32  word(QByteArray const & bb, uint pos);
	void  putWord(QByteArray & bb, uint pos, quint32 val);
	void  appendWord(QByteArray& bb, quint32 val);
	quint16  word16(QByteArray const & bb, uint pos);
	void  putWord16(QByteArray & bb, uint pos, quint16 val);
	void  appendWord16(QByteArray& bb, quint16 val);
	bool  copy(QByteArray & dst, uint to, const QByteArray & src, uint from, uint len);
	const QByteArray tag(QByteArray const & bb, uint pos);
	const QByteArray getTable(const QByteArray& ttf, const QByteArray& ttfTag);
	

	QByteArray subsetFace(const QByteArray& ttf, QList<uint>& glyphs);
	QByteArray extractFace(const QByteArray& ttfColl, int faceIndex);

/**
 This class checks the post table of a ttf font.
 */
class SCRIBUS_API PostTable {
	public:
		PostTable() {usable=false;}
		bool usable;
		QString errorMsg;
		uint numberOfGlyphs() const;
		QString nameFor(uint glyphId) const;
		void readFrom(FT_Face face);
	private:
		QList<QString> m_names;
};

} //namespace
#endif

