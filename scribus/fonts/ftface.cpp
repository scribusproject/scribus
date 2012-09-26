/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "fonts/ftface.h"

#include FT_OUTLINE_H
#include FT_GLYPH_H

#include <QObject>
#include <QFile>

#include "scfonts.h"
#include "util.h"
#include "fonts/scfontmetrics.h"

// static:
FT_Library FtFace::library = NULL;

/*****
   ScFace lifecycle:  unchecked -> loaded -> glyphs checked
                               |         \-> broken glyphs
							   \-> broken
   usable() == ! broken
   embeddable() == glyphs_checked
   
   canRender(unicode) -> CharMap cache? -> loadChar/Glyph -> !broken
   Glyphs:  width    status
            -1000    unkown
            -2000    broken
            >= 0     ok, outline valid
   CharMap:  unicode -> glyph index
             uint[256][256]
   unicode ignores: < 32, ...
   unicode emulate: spaces, hyphen, ligatures?, diacritics?
 *****/

FtFace::FtFace(QString fam, QString sty, QString vari, QString scname, 
			   QString psname, QString path, int face) 
: ScFaceData(), m_face(NULL)
{
	family = fam;
	style = sty;
	variant = vari;
	scName = scname;
	psName = psname;
	fontFile = path;
	faceIndex = face;
	if (!library) {
		if (FT_Init_FreeType( &library ))
			sDebug(QObject::tr("Freetype2 library not available"));
	}
}


FtFace::~FtFace() {
	unload();
}


FT_Face FtFace::ftFace() const {
	if (!m_face) {
		if (FT_New_Face( library, QFile::encodeName(fontFile), faceIndex, & m_face )) {
			status = ScFace::BROKEN;
			m_face = NULL;
			sDebug(QObject::tr("Font %1(%2) is broken").arg(fontFile).arg(faceIndex));
		}
		else {
			load();
		}
	}
	return m_face;
}

void FtFace::load() const
{
	ScFaceData::load();

	if (!m_face) {
		if (FT_New_Face( library, QFile::encodeName(fontFile), faceIndex, & m_face )) {
			status = ScFace::BROKEN;
			m_face = NULL;
			sDebug(QObject::tr("Font %1(%2) is broken").arg(fontFile).arg(faceIndex));
			return;
		}
	}
	
	const_cast<FtFace*>(this)->isStroked = false;
	m_encoding = 0;
	
	m_uniEM = static_cast<qreal>(m_face->units_per_EM);

	m_descent = m_face->descender / m_uniEM;
	m_ascent = m_face->ascender / m_uniEM;
	m_height = m_face->height / m_uniEM;

/* Temporary fix for the broken "Dutch Initials" font */
	if ((m_ascent == 0) && (m_descent == 0))
	{
		m_ascent = (m_face->bbox.yMax - m_face->bbox.yMin) / m_uniEM;
		m_height = m_ascent;
	}

	m_xHeight = m_height;
	m_capHeight = m_height;
	m_maxAdvanceWidth = m_face->max_advance_width / m_uniEM;
	m_underlinePos = m_face->underline_position / m_uniEM;
	m_strikeoutPos = m_ascent / 3;
	m_strokeWidth = m_face->underline_thickness / m_uniEM;
	const_cast<FtFace*>(this)->isFixedPitch = m_face->face_flags & 4;
	m_pdfAscent    = QString::number(m_face->ascender / m_uniEM * 1000);
	m_pdfCapHeight = QString::number(m_face->height / m_uniEM * 1000);
	m_pdfDescender = QString::number(m_face->descender / m_uniEM * 1000);
	m_pdfFontBBox  = QString::number(m_face->bbox.xMin / m_uniEM * 1000) + " " + 
	            QString::number(m_face->bbox.yMin / m_uniEM * 1000) + " " +
	            QString::number(m_face->bbox.xMax / m_uniEM * 1000) + " " + 
	            QString::number(m_face->bbox.yMax / m_uniEM * 1000);
	m_italicAngle  = "0";

//FIXME:	FT_Set_Charmap(m_face, m_face->charmaps[m_encoding]);
	setBestEncoding(m_face);
	
	FT_UInt gindex = 0;
	FT_ULong charcode = FT_Get_First_Char( m_face, &gindex );
	int goodGlyph = 0;
	int invalidGlyph = 0;
	bool error;
	
	while ( gindex != 0 )
	{
		error = FT_Load_Glyph( m_face, gindex, FT_LOAD_NO_SCALE | FT_LOAD_NO_BITMAP );
		if (error)
		{
			++invalidGlyph;
			sDebug(QObject::tr("Font %1 has broken glyph %2 (charcode %3)").arg(fontFile).arg(gindex).arg(charcode));
			charcode = FT_Get_Next_Char( m_face, charcode, &gindex );
			continue;
		}
		
		if (gindex > maxGlyph)
			const_cast<FtFace*>(this)->maxGlyph = gindex;
		
		++goodGlyph;
		if (m_face->glyph->format == FT_GLYPH_FORMAT_PLOTTER)
			const_cast<FtFace*>(this)->isStroked = true;
		charcode = FT_Get_Next_Char( m_face, charcode, &gindex );
	}
	if (invalidGlyph > 0) {
		status = ScFace::BROKENGLYPHS;
	}
}


void FtFace::unload() const
{
	if (m_face) {
		FT_Done_Face( m_face );
		m_face = NULL;
	}
	// clear caches
	ScFaceData::unload();
}


uint FtFace::char2CMap(QChar ch) const
{
	// FIXME use cMap cache
	FT_Face face = ftFace();
	uint gl = FT_Get_Char_Index(face, ch.unicode());
	return gl;
}


void FtFace::loadGlyph(uint gl) const
{
	if (m_glyphWidth.contains(gl))
		return;
	
	ScFace::GlyphData GRec;
	FT_Face face = ftFace();
	if (FT_Load_Glyph( face, gl, FT_LOAD_NO_SCALE | FT_LOAD_NO_BITMAP ))
	{
		sDebug(QObject::tr("Font %1 has broken glyph %2").arg(fontFile).arg(gl));
		m_glyphWidth[gl] = 1;
	}
	else {
		qreal ww = qreal(face->glyph->metrics.horiAdvance) / m_uniEM;
		qreal w  = (face->glyph->metrics.width + qAbs(qreal(face->glyph->metrics.horiBearingX))) / m_uniEM;
		GRec.bbox_width = qMax(w, ww);
		qreal height = qreal(face->glyph->metrics.height) / m_uniEM;
		GRec.bbox_ascent = qreal(face->glyph->metrics.horiBearingY) / m_uniEM;
		GRec.bbox_descent = height - GRec.bbox_ascent;
//		qDebug() << QString("glyphmetrics %1: EM %2 bearing (%3,%4) size (%5,%6) advance %7 bbox (%8,%9)")
//			   .arg(gl).arg(m_uniEM).arg(face->glyph->metrics.horiBearingX).arg(face->glyph->metrics.horiBearingY)
//			   .arg(face->glyph->metrics.width).arg(face->glyph->metrics.height).arg(face->glyph->metrics.horiAdvance)
//			   .arg(w).arg(height);

		qreal x, y;
		bool error = false;
		error = FT_Set_Char_Size( face, 0, 10, 72, 72 );
		if (error)
			m_glyphWidth[gl] = 1;
		FPointArray outlines = traceGlyph(face, gl, 10, &x, &y, &error);
		if (!error)
		{
			m_glyphWidth[gl] = ww;
			GRec.Outlines = outlines;
			GRec.x = x;
			GRec.y = y;
			GRec.broken = false;
		}
		else {
			m_glyphWidth[gl] = 1;
		}
	}
	m_glyphOutline[gl] = GRec;
	if (GRec.broken && status < ScFace::BROKENGLYPHS)
		status = ScFace::BROKENGLYPHS;
}


qreal FtFace::glyphKerning(uint gl, uint gl2, qreal size) const
{
	FT_Vector  delta;
	FT_Face    face = ftFace();
	qreal result = 0;
	/****
		Ok, this looks like a regression between Freetype 2.1.9 -> 2.1.10.
		Ignoring the value of FT_HAS_KERNING for now -- AV
		****/
	if (true || FT_HAS_KERNING(face) )
	{
		FT_Error error = FT_Get_Kerning(face, gl, gl2, FT_KERNING_UNSCALED, &delta);
		if (error) {
			sDebug(QString("Error %2 when accessing kerning pair for font %1").arg(scName).arg(error));
		}
		else {
			result = delta.x / m_uniEM * size;
		}
	}
	else {
		sDebug(QString("Font %1 has no kerning pairs (according to Freetype)").arg(scName));
	}
	return result;
}

/*
GlyphMetrics FtFace::glyphBBox (uint gl, qreal sz) const
{
	FT_Face    face = ftFace();
	GlyphMetrics result;
	FT_Error error = FT_Load_Glyph( face, gl, FT_LOAD_NO_SCALE | FT_LOAD_NO_BITMAP );
	if (!error) {
		qreal w  = (face->glyph->metrics.width + QABS((qreal)face->glyph->metrics.horiBearingX)) / m_uniEM * sz;
		result.width = qMax(w, face->glyph->metrics.horiAdvance / m_uniEM * sz);
		qreal height = face->glyph->metrics.height / m_uniEM * sz;
		result.ascent = face->glyph->metrics.horiBearingY / m_uniEM * sz;
		result.descent = height - result.ascent;
	}
	else {
		result.width = result.ascent = sz;
		result.descent = 0;
	}
	return result;
}
*/


/// copied from Freetype's FT_Stream_ReadAt()
FT_Error ftIOFunc( FT_Stream stream, unsigned long pos, unsigned char* buffer, unsigned long count)
{
    FT_Error  error = FT_Err_Ok;
    FT_ULong  read_bytes;
	
    if ( pos >= stream->size )
    {
		qDebug( "ftIOFunc: invalid i/o; pos = 0x%lx, size = 0x%lx\n", pos, stream->size );
		
		return FT_Err_Invalid_Stream_Operation;
    }
	
    if ( stream->read )
		read_bytes = stream->read( stream, pos, buffer, count );
    else
    {
		read_bytes = stream->size - pos;
		if ( read_bytes > count )
			read_bytes = count;
		
		memcpy( buffer, stream->base + pos, read_bytes );
    }
	
    stream->pos = pos + read_bytes;
	
    if ( read_bytes < count )
    {
		qDebug( "ftIOFunc: invalid read; expected %lu bytes, got %lu\n", count, read_bytes );
		
		error = FT_Err_Invalid_Stream_Operation;
    }
	
    return error;
}


bool FtFace::glyphNames(QMap<uint, std::pair<QChar, QString> >& GList) const
{
	return GlyNames(ftFace(), GList);
}


void FtFace::RawData(QByteArray & bb) const
{
	FT_Stream fts = ftFace()->stream;
	bb.resize(fts->size);
	bool error = ftIOFunc(fts, 0L, reinterpret_cast<FT_Byte *>(bb.data()), fts->size);
	if (error) 
	{
		sDebug(QObject::tr("Font %1 is broken (read stream), no embedding").arg(fontFile));
		bb.resize(0);
		status = qMax(status, ScFace::BROKENGLYPHS);
	}
/*	
//	 if (showFontInformation)
	 {
		 QFile f(fontFile);
		 qDebug(QObject::tr("RawData for Font %1(%2): size=%3 filesize=%4").arg(fontFile).arg(faceIndex).arg(bb.size()).arg(f.size()));
	 }
*/	 
}


