/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QColor>
#include <QDebug>
#include <QMap>
#include <QPainter>
#include <QPixmap>
#include <QRegExp>
#include <QStringList>
#include <QTransform>

#include <ft2build.h>
#include <memory>

#include FT_FREETYPE_H
#include FT_TRUETYPE_TABLES_H
#include FT_TRUETYPE_IDS_H

#include "fpoint.h"
#include "fpointarray.h"
#include "ftface.h"
#include "scfontmetrics.h"
#include "scfonts.h"
#include "scpage.h"
#include "scpainter.h"
#include "scribusdoc.h"
#include "style.h"
#include "util_math.h"

// this code contains a set of font related functions
// that don't really fit within ScFonts.

static FPoint firstP;
static bool FirstM;
static QMap<FT_ULong, QString> adobeGlyphNames;
#if 0
static const char* table[] = {
//#include "glyphlist.txt.q"
					0};
#endif

// private functions
static int traceMoveto( FT_Vector *to, FPointArray *composite );
static int traceLineto( FT_Vector *to, FPointArray *composite );
static int traceQuadraticBezier( FT_Vector *control, FT_Vector *to, FPointArray *composite );
static int traceCubicBezier( FT_Vector *p, FT_Vector *q, FT_Vector *to, FPointArray *composite );

FT_Outline_Funcs OutlineMethods =
	{
		(FT_Outline_MoveTo_Func) traceMoveto,
		(FT_Outline_LineTo_Func) traceLineto,
		(FT_Outline_ConicTo_Func) traceQuadraticBezier,
		(FT_Outline_CubicTo_Func) traceCubicBezier,
		0,
		0
	};


const qreal FTSCALE = 64.0;


int setBestEncoding(FT_Face face)
{
	FT_ULong  charcode;
	FT_UInt   gindex;
	bool foundEncoding = false;
	int countUnicode = 0;
	int chmapUnicode = -1;
	int chmapCustom = -1;
	int retVal = 0;
	//FT_CharMap defaultEncoding = face->charmap;
//	int defaultchmap = face->charmap ? FT_Get_Charmap_Index(face->charmap) : 0;
// Since the above function is only available in FreeType 2.1.10 its replaced by
// the following line, assuming that the default charmap has the index 0
	int defaultchmap = 0;
	FT_ULong dbgInfo = 0;

	FT_Load_Sfnt_Table( face, FT_MAKE_TAG('p','o','s','t'), 0, nullptr, &dbgInfo );
	//qDebug() << "setBestEncoding for " << FT_Get_Postscript_Name(face) << " with " << face->num_glyphs << "glyphs, hasNames=" << FT_HAS_GLYPH_NAMES(face) << ", POST size=" << dbgInfo ;
	
	for (int i = 0; i < face->num_charmaps; i++)
	{
		FT_CharMap charmap = face->charmaps[i];
		//qDebug() << "Checking cmap " << i << "(" << charmap->platform_id << "," << charmap->encoding_id << "," << FT_Get_CMap_Language_ID(charmap) << ") format " << FT_Get_CMap_Format(charmap);
		if (charmap->encoding == FT_ENCODING_UNICODE)
		{
			FT_Set_Charmap(face, face->charmaps[i]);
			chmapUnicode = i;
			gindex = 0;
			charcode = FT_Get_First_Char(face, &gindex);
			while (gindex != 0)
			{
				countUnicode++;
				charcode = FT_Get_Next_Char(face, charcode, &gindex);
			}
			//qDebug() << "found Unicode enc for" << face->family_name << face->style_name  << "as map" << chmapUnicode << "with" << countUnicode << "glyphs";
		}
		if (charmap->encoding == FT_ENCODING_ADOBE_CUSTOM)
		{
			chmapCustom = i;
			foundEncoding = true;
			retVal = 1;
			//qDebug() << "found Custom enc for" << face->family_name << face->style_name;
			break;
		}
		if (charmap->encoding == FT_ENCODING_MS_SYMBOL)
		{
			//qDebug() << "found Symbol enc for" << face->family_name << face->style_name;

			chmapCustom = i;
			foundEncoding = true;
			retVal = 2;
			break;
		}
	}
	int mapToSet = defaultchmap;
	if (chmapUnicode >= 0 && countUnicode >= face->num_glyphs-1)
	{
		//qDebug() << "using Unicode enc for" << face->family_name << face->style_name;
		mapToSet = chmapUnicode;
		retVal = 0;
	}
	else if (foundEncoding)
	{
		//qDebug() << "using special enc for" << face->family_name << face->style_name;
		mapToSet = chmapCustom;
	}
	else
	{
		//qDebug() << "using default enc for" << face->family_name << face->style_name;
		mapToSet = defaultchmap;
		retVal = 0;
	}

	//Fixes #2199, missing glyphs from 1.2.1->1.2.2
	//If the currently wanted character map is not already Unicode...
	//if (FT_Get_Charmap_Index(face->charmap) != chmapUnicode)
	if (mapToSet != chmapUnicode)
	{
		//Change map so we can count the chars in it
		FT_Set_Charmap(face, face->charmaps[mapToSet]);
		//Count the characters in the current map
		gindex = 0;
		int countCurrMap = 0;
		charcode = FT_Get_First_Char(face, &gindex);
		while (gindex != 0)
		{
			countCurrMap++;
			charcode = FT_Get_Next_Char(face, charcode, &gindex);
		}
		//If the last Unicode map we found before has more characters,
		//then set it to be the current map.
		
		if (countUnicode > countCurrMap)
		{
//			qDebug() << "override with Unicode enc for" << face->family_name << face->style_name << "map" << mapToSet << "has only" << countCurrMap << "glyphs";
			mapToSet = chmapUnicode;
			retVal = 0;
		}
	}
	FT_Set_Charmap(face, face->charmaps[mapToSet]);
//	qDebug() << "set map" << mapToSet << "for" << face->family_name << face->style_name;
//	qDebug() << "glyphsForNumbers 0-9:" << FT_Get_Char_Index(face, QChar('0').unicode()) 
//		<< FT_Get_Char_Index(face, QChar('1').unicode()) << FT_Get_Char_Index(face, QChar('2').unicode()) << FT_Get_Char_Index(face, QChar('3').unicode()) 
//		<< FT_Get_Char_Index(face, QChar('4').unicode()) << FT_Get_Char_Index(face, QChar('5').unicode()) << FT_Get_Char_Index(face, QChar('6').unicode()) 
//		<< FT_Get_Char_Index(face, QChar('7').unicode()) << FT_Get_Char_Index(face, QChar('8').unicode()) << FT_Get_Char_Index(face, QChar('9').unicode());
	return retVal;
}

FPointArray traceGlyph(FT_Face face, ScFace::gid_type glyphIndex, int chs, qreal *x, qreal *y, bool *err)
{
	bool error = false;
	//AV: not threadsave, but tracechar is only used in ReadMetrics() and fontSample()
	static FPointArray pts; 
	FPointArray pts2;
	pts.resize(0);
	pts2.resize(0);
	firstP = FPoint(0,0);
	FirstM = true;
	error = FT_Set_Char_Size( face, 0, chs*6400, 72, 72 );
	if (error)
	{
		*err = error;
		return pts2;
	}

	error = FT_Load_Glyph( face, glyphIndex, FT_LOAD_NO_HINTING | FT_LOAD_NO_BITMAP );
	if (error)
	{
		*err = error;
		return pts2;
	}
	error = FT_Outline_Decompose(&face->glyph->outline, &OutlineMethods, reinterpret_cast<void*>(&pts));
	if (error)
	{
		*err = error;
		return pts2;
	}
	*x = face->glyph->metrics.horiBearingX / 6400.0;
	*y = face->glyph->metrics.horiBearingY / 6400.0;
	QTransform ma;
	ma.scale(0.01, -0.01);
	pts.map(ma);
	pts.translate(0, chs);
	pts2.putPoints(0, pts.size()-2, pts, 0);

	return pts2;
}


FPointArray traceChar(FT_Face face, ScFace::ucs4_type chr, int chs, qreal *x, qreal *y, bool *err)
{
	bool error = false;
	FT_UInt glyphIndex;
	error = FT_Set_Char_Size( face, 0, chs*64, 72, 72 );
	if (error)
	{
		*err = error;
		return FPointArray();
	}
	glyphIndex = FT_Get_Char_Index(face, chr);
	return traceGlyph(face, glyphIndex, chs, x, y, err);
}


QPixmap FontSample(const ScFace& fnt, int s, QVector<uint> ts, const QColor& back, bool force)
{
	FT_Face face;
	FT_Library library;
	qreal x, y, ymax;
	bool error;
	int  pen_x;
	FPoint gp;
	error = FT_Init_FreeType( &library );
	error = FT_New_Face( library, QFile::encodeName(fnt.fontFilePath()), fnt.faceIndex(), &face );
	int encode = setBestEncoding(face);
	qreal uniEM = static_cast<qreal>(face->units_per_EM);

	qreal m_height = qMax(face->height / uniEM, (face->bbox.yMax - face->bbox.yMin) / uniEM);

	int h = qRound(m_height * s) + 1;
	qreal a = 0;//m_descent * s + 1;
	int w = qRound((face->bbox.xMax - face->bbox.xMin) / uniEM) * s * (ts.length()+1);
	if (w < 1)
		w = s * (ts.length()+1);
	if (h < 1)
		h = s;
	QImage pm(w, h, QImage::Format_ARGB32_Premultiplied);
	pen_x = 0;
	ymax = 0.0;

	std::unique_ptr<ScPainter> p(new ScPainter(&pm, pm.width(), pm.height()));
	p->clear(back);
	p->setFillMode(ScPainter::Solid);
	p->setLineWidth(0.0);
	p->setBrush(Qt::black);

	FPointArray gly;
	ScFace::ucs4_type dv;
	dv = ts[0];
	error = false;
	gly = traceChar(face, dv, s, &x, &y, &error);
	if (((encode != 0) || (error)) && (!force))
	{
		error = false;
		FT_ULong  charcode;
		FT_UInt gindex;
		gindex = 0;
		charcode = FT_Get_First_Char(face, &gindex );
		for (int n = 0; n < ts.length(); ++n)
		{
			gly = traceChar(face, charcode, s, &x, &y, &error);
			if (error)
				break;
			if (gly.size() > 3)
			{
				gly.translate(static_cast<qreal>(pen_x) / 6400.0, a);
				gp = getMaxClipF(&gly);
				ymax = qMax(ymax, gp.y());
				p->setupPolygon(&gly);
				p->fillPath();
			}
			pen_x += face->glyph->advance.x;
			charcode = FT_Get_Next_Char(face, charcode, &gindex );
			if (gindex == 0)
				break;
		}
	}
	else
	{
		for (int n = 0; n < ts.length(); ++n)
		{
			dv = ts[n];
			error = false;
			gly = traceChar(face, dv, s, &x, &y, &error);
			if (gly.size() > 3)
			{
				gly.translate(static_cast<qreal>(pen_x) / 6400.0, a);
				gp = getMaxClipF(&gly);
				ymax = qMax(ymax, gp.y());
				p->setupPolygon(&gly);
				p->fillPath();
			}
			pen_x += face->glyph->advance.x;
		}
	}
	p->end();

	QPixmap pmr = QPixmap::fromImage(pm.copy(0, 0, qMin(qRound(gp.x()), w), qMin(qRound(ymax), h)));

	FT_Done_FreeType( library );
	return pmr;
}

static int traceMoveto( FT_Vector *to, FPointArray *composite )
{
	qreal tox = ( to->x / FTSCALE );
	qreal toy = ( to->y / FTSCALE );
	if (!FirstM)
	{
		composite->addPoint(firstP);
		composite->addPoint(firstP);
		composite->setMarker();
	}
	else
		FirstM = false;
	composite->addPoint(tox, toy);
	composite->addPoint(tox, toy);
	firstP.setXY(tox, toy);
	return 0;
}

static int traceLineto( FT_Vector *to, FPointArray *composite )
{
	qreal tox = ( to->x / FTSCALE );
	qreal toy = ( to->y / FTSCALE );
	if (!composite->hasLastQuadPoint(tox, toy, tox, toy, tox, toy, tox, toy))
		composite->addQuadPoint(tox, toy, tox, toy, tox, toy, tox, toy);
	return 0;
}

static int traceQuadraticBezier( FT_Vector *control, FT_Vector *to, FPointArray *composite )
{
	const FPoint& prev = composite->last();
	qreal x1 = (prev.x() + 2 * control->x / FTSCALE) / 3.0;
	qreal y1 = (prev.y() + 2 * control->y / FTSCALE) / 3.0;
	qreal x2 = (to->x / FTSCALE + 2 * (control->x / FTSCALE)) / 3.0;
	qreal y2 = (to->y / FTSCALE + 2 * (control->y / FTSCALE)) / 3.0;
	qreal x3 = (to->x / FTSCALE);
	qreal y3 = (to->y / FTSCALE);
	if ( !composite->hasLastQuadPoint(x3, y3, x2, y2, x3, y3, x3, y3) )
	{
		composite->setPoint(composite->size() - 1, FPoint(x1, y1));
		composite->addQuadPoint(x3, y3, x2, y2, x3, y3, x3, y3);
	}
	return 0;
}

static int traceCubicBezier( FT_Vector *p, FT_Vector *q, FT_Vector *to, FPointArray *composite )
{
	qreal x1 = ( p->x / FTSCALE );
	qreal y1 = ( p->y / FTSCALE );
	qreal x2 = ( q->x / FTSCALE );
	qreal y2 = ( q->y / FTSCALE );
	qreal x3 = ( to->x / FTSCALE );
	qreal y3 = ( to->y / FTSCALE );
	if ( !composite->hasLastQuadPoint(x3, y3, x2, y2, x3, y3, x3, y3) )
	{
		composite->setPoint(composite->size() - 1, FPoint(x1, y1));
		composite->addQuadPoint(x3, y3, x2, y2, x3, y3, x3, y3);
	}
	return 0;
}

/// if in AGL, use that name, else use "uni1234" or "u12345"
QString adobeGlyphName(FT_ULong charcode) 
{
	static const char HEX[] = "0123456789ABCDEF";
	QString result;
	if (adobeGlyphNames.contains(charcode))
		return adobeGlyphNames[charcode];
	if (charcode < 0x10000)
	{
		result = QString("uni") + HEX[charcode>>12 & 0xF] 
		                        + HEX[charcode>> 8 & 0xF] 
		                        + HEX[charcode>> 4 & 0xF] 
		                        + HEX[charcode     & 0xF];
	}
	else
	{
		result = QString("u");
		int first = 28;
		while ((charcode & (0xF << first)) == 0)
			first -= 4;
		for (int i = first; i >= 0; i -= 4)
			result += HEX[charcode >> i & 0xF];
	}
	return result;
}
