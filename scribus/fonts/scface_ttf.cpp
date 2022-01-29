/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QFile>
#include <QString>
#include <QObject>
#include <QDebug>

#include <limits>
#include <sys/types.h>

#include "fonts/scface_ttf.h"
#include "fonts/scfontmetrics.h"
#include "util_debug.h"
#include "scconfig.h"
#include "sfnt.h"

#include FT_TRUETYPE_IDS_H


ScFace_ttf::ScFace_ttf (const QString& fam, const QString& sty, const QString& alt, const QString& scname, const QString& psname, const QString& path, int face, const QStringList& features )
		: FtFace (fam, sty, alt, scname, psname, path, face, features)
{
	formatCode = ScFace::SFNT;
}

ScFace_ttf::~ScFace_ttf() = default;

bool ScFace_ttf::isSymbolic() const
{
	FT_Face face = ftFace();
	if (!face || !face->charmap)
		return false;
	return (m_face->charmap->encoding == FT_ENCODING_MS_SYMBOL);
}

void ScFace_ttf::load() const
{
	FtFace::load();
	sfnt::PostTable checkPost;
	FT_Face face = ftFace();
	checkPost.readFrom(face);
//	if (!checkPost.usable)
//		qDebug() << "can't use post table from " << face->family_name << face->style_name << ":" << checkPost.errorMsg;
//	else
//		qDebug() << "posttable from" << face->family_name << face->style_name << "has names for" << checkPost.numberOfGlyphs() << "glyphs and" << maxGlyph << "glyphs in charmap";
	const_cast<bool&>(hasGlyphNames) = checkPost.usable() && checkPost.numberOfGlyphs() >= maxGlyph;
}

void ScFace_ttf::unload() const
{
	FtFace::unload();
}

bool ScFace_ttf::hasNames() const
{
	FT_Face face = ftFace();
	if (!face)
		return false;

	// The glyph name table embedded in Truetype fonts is not reliable.
	// For those fonts we consequently use Adobe Glyph names whenever possible.
	const bool avoidFntNames = (formatCode != ScFace::TYPE42 && typeCode == ScFace::TTF) && hasMicrosoftUnicodeCmap(face);
	if (avoidFntNames)
		return true; // We use Adobe Glyph List or 'uniXXXX' names in such case

	return FtFace::hasNames();
}

bool ScFace_ttf::glyphNames(ScFace::FaceEncoding& glyphList) const
{
	FT_ULong  charcode;
	FT_UInt gindex = 0;

	FT_Face face = ftFace();
	if (!face)
		return false;
	
	// The glyph name table embedded in Truetype fonts is not reliable.
	// For those fonts we consequently use Adobe Glyph names whenever possible.
	const bool avoidFntNames = (formatCode != ScFace::TYPE42 && typeCode == ScFace::TTF) && hasMicrosoftUnicodeCmap(face);
	if (!avoidFntNames)
		return FtFace::glyphNames(glyphList);
	
//	qDebug() << "reading metrics for" << face->family_name << face->style_name;
	FT_UInt spaceGlyphIndex = std::numeric_limits<FT_UInt>::max();

	charcode = FT_Get_First_Char(face, &gindex);
	while (gindex != 0)
	{
		// #16289 : Protect space character in case several characters are
		// mapped to same glyph as space
		if (charcode == ' ')
			spaceGlyphIndex = gindex;
		if ((gindex == spaceGlyphIndex) && (charcode != ' '))
		{
			charcode = FT_Get_Next_Char(face, charcode, &gindex);
			continue;
		}

		ScFace::GlyphEncoding glEncoding;
		glEncoding.charcode  = charcode;
		glEncoding.glyphName = adobeGlyphName(charcode);
		glEncoding.toUnicode = QString::asprintf("%04lX", charcode);
		glyphList.insert(gindex, glEncoding);
		charcode = FT_Get_Next_Char(face, charcode, &gindex);
	}

	return true;
}

void ScFace_ttf::rawData(QByteArray & bb) const
{
	if (formatCode == ScFace::TTCF)
	{
		QByteArray coll;
		FtFace::rawData(coll);
		// access table for faceIndex
		if (faceIndex >= static_cast<int>(sfnt::word(coll, 8)))
		{
			bb.resize(0);
			return;
		}
		static const uint OFFSET_TABLE_LEN = 12;
		static const uint   TDIR_ENTRY_LEN = 16;
        uint faceOffset = sfnt::word(coll, 12 + 4 * faceIndex);
		uint nTables    = sfnt::word16(coll, faceOffset + 4);
		sDebug(QObject::tr("extracting face %1 from font %2 (offset=%3, nTables=%4)").arg(faceIndex).arg(fontFile).arg(faceOffset).arg(nTables));
		uint headerLength = OFFSET_TABLE_LEN + TDIR_ENTRY_LEN * nTables;
		uint tableLengths = 0;
		// sum table lengths incl padding
		for (uint i = 0; i < nTables; ++i)
		{
			tableLengths += sfnt::word(coll, faceOffset + OFFSET_TABLE_LEN + TDIR_ENTRY_LEN * i + 12);
			tableLengths = (tableLengths+3) & ~3;
		}
		bb.resize(headerLength + tableLengths);
		if (!bb.data())
			return;
		// write header
//		sDebug(QObject::tr("memcpy header: %1 %2 %3").arg(0).arg(faceOffset).arg(headerLength));
		if (!sfnt::copy(bb, 0, coll, faceOffset, headerLength))
			return;

		uint pos = headerLength;
		for (uint i = 0; i < nTables; ++i)
		{
            uint dirEntry = faceOffset + OFFSET_TABLE_LEN + TDIR_ENTRY_LEN * i;
			uint tableSize  = sfnt::word(coll,  dirEntry + 12);
			uint tableStart = sfnt::word(coll, dirEntry + 8);
//            sDebug(QObject::tr("table '%1' %2. %3 ...").arg(QString(sfnt::tag(coll, dirEntry)))
//                   .arg(QString::number(sfnt::word16(coll,tableStart),16))
//                   .arg(QString::number(sfnt::word16(coll,tableStart+2),16)));
//			sDebug(QObject::tr("memcpy table: %1 %2 %3").arg(pos).arg(tableStart).arg(tableSize));
			if (!sfnt::copy(bb, pos, coll, tableStart, tableSize))
				break;
			// write new offset to table entry
//			sDebug(QObject::tr("memcpy offset: %1 %2 %3").arg(OFFSET_TABLE_LEN + TDIR_ENTRY_LEN*i + 8).arg(pos).arg(4));
			// buggy: not endian aware: memcpy(bb.data() + OFFSET_TABLE_LEN + TDIR_ENTRY_LEN * i + 8, &pos, 4);
            sfnt::putWord(bb, OFFSET_TABLE_LEN + TDIR_ENTRY_LEN * i + 8, pos);
			pos += tableSize;
			// pad
			while ((pos & 3) != 0)
				bb.data()[pos++] = '\0';
		}
	}
	else if (formatCode == ScFace::TYPE42)
		FtFace::rawData(bb);
	else
		FtFace::rawData(bb);
}
