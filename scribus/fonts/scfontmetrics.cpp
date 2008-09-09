/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QColor>
#include <QDebug>
#include <QMap>
#include <QMatrix>
#include <QPainter>
#include <QPixmap>
#include <QRegExp>
#include <QStringList>

#include "fpoint.h"
#include "fpointarray.h"
#include "page.h"
#include "scfontmetrics.h"
#include "scfonts.h"
#include "scpainter.h"
#include "scribusdoc.h"
#include "style.h"
#include "util_math.h"


// this code contains a set of font related functions
// that don't really fit within ScFonts.

static FPoint firstP;
static bool FirstM;
static QMap<FT_ULong, QString> adobeGlyphNames;
static const char* table[] = {
//#include "glyphnames.txt.q"
					NULL};

// private functions
static void readAdobeGlyphNames();
static QString adobeGlyphName(FT_ULong charcode);
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


const double FTSCALE = 64.0;


int setBestEncoding(FT_Face face)
{
	FT_ULong  charcode;
	FT_UInt   gindex;
	bool foundEncoding = false;
	int countUniCode = 0;
	int chmapUniCode = -1;
	int chmapCustom = -1;
	int retVal = 0;
	//FT_CharMap defaultEncoding = face->charmap;
//	int defaultchmap=face->charmap ? FT_Get_Charmap_Index(face->charmap) : 0;
// Since the above function is only available in FreeType 2.1.10 its replaced by
// the following line, assuming that the default charmap has the index 0
	int defaultchmap = 0;
	for(int u = 0; u < face->num_charmaps; u++)
	{
		if (face->charmaps[u]->encoding == FT_ENCODING_UNICODE )
		{
			FT_Set_Charmap(face, face->charmaps[u]);
			chmapUniCode = u;
			gindex = 0;
			charcode = FT_Get_First_Char( face, &gindex );
			while ( gindex != 0 )
			{
				countUniCode++;
				charcode = FT_Get_Next_Char( face, charcode, &gindex );
			}
//			qDebug() << "found Unicode enc for" << face->family_name << face->style_name  << "as map" << chmapUniCode << "with" << countUniCode << "glyphs";

		}
		if (face->charmaps[u]->encoding == FT_ENCODING_ADOBE_CUSTOM)
		{
			chmapCustom = u;
			foundEncoding = true;
			retVal = 1;
//			qDebug() << "found Custom enc for" << face->family_name << face->style_name;
			break;
		}
		else if (face->charmaps[u]->encoding == FT_ENCODING_MS_SYMBOL)
		{
//			qDebug() << "found Symbol enc for" << face->family_name << face->style_name;

			chmapCustom = u;
			foundEncoding = true;
			retVal = 2;
			break;
		}
	}
	int mapToSet=defaultchmap;
	if (chmapUniCode >= 0 && countUniCode >= face->num_glyphs-1)
	{
//		qDebug() << "using Unicode enc for" << face->family_name << face->style_name;

		mapToSet=chmapUniCode;
		//FT_Set_Charmap(face, face->charmaps[chmapUniCode]);
		retVal = 0;
	}
	else
	if (foundEncoding)
	{
//		qDebug() << "using special enc for" << face->family_name << face->style_name;
		mapToSet=chmapCustom;
		//FT_Set_Charmap(face, face->charmaps[chmapCustom]);
	}
	else
	{
//		qDebug() << "using default enc for" << face->family_name << face->style_name;
		mapToSet=defaultchmap;
		//FT_Set_Charmap(face, defaultEncoding);
		retVal = 0;
	}

	//Fixes #2199, missing glyphs from 1.2.1->1.2.2
	//If the currently wanted character map is not already Unicode...
	//if (FT_Get_Charmap_Index(face->charmap)!=chmapUniCode)
	if (mapToSet!=chmapUniCode)
	{
		//Change map so we can count the chars in it
		FT_Set_Charmap(face, face->charmaps[mapToSet]);
		//Count the characters in the current map
		gindex = 0;
		int countCurrMap=0;
		charcode = FT_Get_First_Char( face, &gindex );
		while ( gindex != 0 )
		{
			countCurrMap++;
			charcode = FT_Get_Next_Char( face, charcode, &gindex );
		}
		//If the last Unicode map we found before has more characters,
		//then set it to be the current map.
		
		if (countUniCode>countCurrMap)
		{
//			qDebug() << "override with Unicode enc for" << face->family_name << face->style_name << "map" << mapToSet << "has only" << countCurrMap << "glyphs";
			mapToSet=chmapUniCode;
			//FT_Set_Charmap(face, face->charmaps[chmapUniCode]);
			retVal = 0;
		}
	}
//	if (face->charmap == NULL || mapToSet!=FT_Get_Charmap_Index(face->charmap))
		FT_Set_Charmap(face, face->charmaps[mapToSet]);
//	qDebug() << "set map" << mapToSet << "for" << face->family_name << face->style_name;
//	qDebug() << "glyphsForNumbers 0-9:" << FT_Get_Char_Index(face, QChar('0').unicode()) 
//		<< FT_Get_Char_Index(face, QChar('1').unicode()) << FT_Get_Char_Index(face, QChar('2').unicode()) << FT_Get_Char_Index(face, QChar('3').unicode()) 
//		<< FT_Get_Char_Index(face, QChar('4').unicode()) << FT_Get_Char_Index(face, QChar('5').unicode()) << FT_Get_Char_Index(face, QChar('6').unicode()) 
//		<< FT_Get_Char_Index(face, QChar('7').unicode()) << FT_Get_Char_Index(face, QChar('8').unicode()) << FT_Get_Char_Index(face, QChar('9').unicode());
	return retVal;
}

FPointArray traceGlyph(FT_Face face, FT_UInt glyphIndex, int chs, double *x, double *y, bool *err)
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
	if (glyphIndex == 0)
	{
		*err = true;
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
	QMatrix ma;
	ma.scale(0.01, -0.01);
	pts.map(ma);
	pts.translate(0, chs);
	pts2.putPoints(0, pts.size()-2, pts, 0);

	return pts2;
}


FPointArray traceChar(FT_Face face, uint chr, int chs, double *x, double *y, bool *err)
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


QPixmap FontSample(const ScFace& fnt, int s, QString ts, QColor back, bool force)
{
	FT_Face face;
	FT_Library library;
	double x, y, ymax;
	bool error;
	int  pen_x;
	FPoint gp;
	error = FT_Init_FreeType( &library );
	error = FT_New_Face( library, QFile::encodeName(fnt.fontFilePath()), fnt.faceIndex(), &face );
	int encode = setBestEncoding(face);
	double uniEM = static_cast<double>(face->units_per_EM);

	double m_descent = face->descender / uniEM;
	double m_height = face->height / uniEM;
	if (m_height == 0)
		m_height = (face->bbox.yMax - face->bbox.yMin) / uniEM;

	int h = qRound(m_height * s) + 1;
	double a = m_descent * s + 1;
	int w = qRound((face->bbox.xMax - face->bbox.xMin) / uniEM) * s * (ts.length()+1);
	if (w < 1)
		w = s * (ts.length()+1);
	if (h < 1)
		h = s;
	QImage pm(w, h, QImage::Format_ARGB32);
	pen_x = 0;
	ymax = 0.0;
	ScPainter *p = new ScPainter(&pm, pm.width(), pm.height());
	p->clear(back);
	p->setFillMode(1);
	p->setLineWidth(0.0);
//	p->setBrush(back);
//	p->drawRect(0.0, 0.0, static_cast<double>(w), static_cast<double>(h));
	p->setBrush(Qt::black);
	FPointArray gly;
	uint dv;
	dv = ts[0].unicode();
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
				gly.translate(static_cast<double>(pen_x) / 6400.0, a);
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
			dv = ts[n].unicode();
			error = false;
			gly = traceChar(face, dv, s, &x, &y, &error);
			if (gly.size() > 3)
			{
				gly.translate(static_cast<double>(pen_x) / 6400.0, a);
				gp = getMaxClipF(&gly);
				ymax = qMax(ymax, gp.y());
				p->setupPolygon(&gly);
				p->fillPath();
			}
			pen_x += face->glyph->advance.x;
		}
	}
	p->end();
	QPixmap pmr;
	pmr=QPixmap::fromImage(pm.copy(0, 0, qMin(qRound(gp.x()), w), qMin(qRound(ymax), h)));
// this one below gives some funny results
//	pmr.convertFromImage(pm.scaled(qMin(qRound(gp.x()), w), qMin(qRound(ymax), h), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
//	pmr.resize(qMin(qRound(gp.x()), w), qMin(qRound(ymax), h));
	delete p;
	FT_Done_FreeType( library );
	return pmr;
}

/** Same as FontSample() with \n strings support added.
09/26/2004 petr vanek

QPixmap fontSamples(ScFace * fnt, int s, QString ts, QColor back)
{
	QStringList lines = QStringList::split("\n", ts);
	QPixmap ret(640, 480);
	QPixmap sample;
	QPainter *painter = new QPainter(&ret);
	int y = 0;
	int x = 0;
	ret.fill(back);
	for ( QStringList::Iterator it = lines.begin(); it != lines.end(); ++it )
	{
		sample = FontSample(fnt, s, *it, back);
		if (!sample.isNull())
			painter->drawPixmap(0, y, sample, 0, 0);
		y = y + sample.height();
		if (x < sample.width())
			x = sample.width();
	} // for
	delete(painter);
	QPixmap final(x, y);
	if ((x != 0) && (y != 0))
	{
		QPainter *fpainter = new QPainter(&final);
		fpainter->drawPixmap(0, 0, ret, 0, 0, x, y);
		delete(fpainter);
	}
	return final;
}
*/

bool GlyNames(FT_Face face, QMap<uint, std::pair<QChar, QString> >& GList)
{
	char buf[50];
	FT_ULong  charcode;
	FT_UInt gindex;
	setBestEncoding(face);
	gindex = 0;
	charcode = FT_Get_First_Char(face, &gindex );
	const bool hasPSNames = FT_HAS_GLYPH_NAMES(face);
	if (adobeGlyphNames.empty())
		readAdobeGlyphNames();
//	qDebug() << "reading metrics for" << face->family_name << face->style_name;
	while (gindex != 0)
	{
		bool notfound = true;
		if (hasPSNames)
			notfound = FT_Get_Glyph_Name(face, gindex, &buf, 50);

		// just in case FT gives empty string or ".notdef"
		// no valid glyphname except ".notdef" starts with '.'		
//		qDebug() << "\t" << gindex << " '" << charcode << "' --> '" << (notfound? "notfound" : buf) << "'";
		if (notfound || buf[0] == '\0' || buf[0] == '.')
			GList.insert(gindex, std::make_pair(QChar(static_cast<uint>(charcode)),adobeGlyphName(charcode)));
		else
			GList.insert(gindex, std::make_pair(QChar(static_cast<uint>(charcode)),QString(reinterpret_cast<char*>(buf))));
			
		charcode = FT_Get_Next_Char(face, charcode, &gindex );
	}
	// Let's see if we can find some more...
	int maxSlot1 = face->num_glyphs;
	if (hasPSNames)
		for (int gindex = 1; gindex < maxSlot1; ++gindex)
		{
			if (!GList.contains(gindex))
			{
				bool found = ! FT_Get_Glyph_Name(face, gindex, &buf, 50);
				if (found)
				{
					QString glyphname(reinterpret_cast<char*>(buf));
					charcode = 0;
					QMap<uint,std::pair<QChar,QString> >::Iterator gli;
					for (gli = GList.begin(); gli != GList.end(); ++gli)
					{
						if (glyphname == gli.value().second)
						{
							charcode = gli.value().first.unicode();
							break;
						}
					}
//					qDebug() << "\tmore: " << gindex << " '" << charcode << "' --> '" << buf << "'";
					GList.insert(gindex, std::make_pair(QChar(static_cast<uint>(charcode)), glyphname));
				}
			}
		}
			
	return true;
}


static int traceMoveto( FT_Vector *to, FPointArray *composite )
{
	double tox = ( to->x / FTSCALE );
	double toy = ( to->y / FTSCALE );
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
	double tox = ( to->x / FTSCALE );
	double toy = ( to->y / FTSCALE );
	if ( !composite->hasLastQuadPoint(tox, toy, tox, toy, tox, toy, tox, toy))
		composite->addQuadPoint(tox, toy, tox, toy, tox, toy, tox, toy);
	return 0;
}

static int traceQuadraticBezier( FT_Vector *control, FT_Vector *to, FPointArray *composite )
{
	double x1 = ( control->x / FTSCALE );
	double y1 = ( control->y / FTSCALE );
	double x2 = ( to->x / FTSCALE );
	double y2 = ( to->y / FTSCALE );
	if ( !composite->hasLastQuadPoint(x2, y2, x1, y1, x2, y2, x2, y2))
		composite->addQuadPoint(x2, y2, x1, y1, x2, y2, x2, y2);
	return 0;
}

static int traceCubicBezier( FT_Vector *p, FT_Vector *q, FT_Vector *to, FPointArray *composite )
{
	double x1 = ( p->x / FTSCALE );
	double y1 = ( p->y / FTSCALE );
	double x2 = ( q->x / FTSCALE );
	double y2 = ( q->y / FTSCALE );
	double x3 = ( to->x / FTSCALE );
	double y3 = ( to->y / FTSCALE );
	if ( !composite->hasLastQuadPoint(x3, y3, x2, y2, x3, y3, x3, y3) )
	{
		composite->setPoint(composite->size()-1, FPoint(x1, y1));
		composite->addQuadPoint(x3, y3, x2, y2, x3, y3, x3, y3);
	}
	return 0;
}

/// init the Adobe Glyph List
void readAdobeGlyphNames() 
{
	adobeGlyphNames.clear();
	QRegExp pattern("(\\w*);([0-9A-Fa-f]{4})");
	for (uint i=0; table[i]; ++i) {
		if (pattern.indexIn(table[i]) >= 0) {
			FT_ULong unicode = pattern.cap(2).toULong(0, 16);
			qDebug(QString("reading glyph name %1 for unicode %2(%3)").arg(pattern.cap(1)).arg(unicode).arg(pattern.cap(2)).toLocal8Bit());
			adobeGlyphNames.insert(unicode, pattern.cap(1));
		}
	}
}


/// if in AGL, use that name, else use "uni1234" or "u12345"
QString adobeGlyphName(FT_ULong charcode) 
{
	static const char HEX[] = "0123456789ABCDEF";
	QString result;
	if (adobeGlyphNames.contains(charcode))
		return adobeGlyphNames[charcode];
	else if (charcode < 0x10000) {
		result = QString("uni") + HEX[charcode>>12 & 0xF] 
		                        + HEX[charcode>> 8 & 0xF] 
		                        + HEX[charcode>> 4 & 0xF] 
		                        + HEX[charcode     & 0xF];
	}
	else  {
		result = QString("u");
		for (int i= 28; i >= 0; i-=4) {
			if (charcode & (0xF << i))
				result += HEX[charcode >> i & 0xF];
		}
	}
	return result;
}

/*
double Cwidth(ScribusDoc *, ScFace* scFace, QString ch, int Size, QString ch2)
{
	double width;
	FT_Vector  delta;
	FT_Face      face;
	uint c1 = ch.at(0).unicode();
	uint c2 = ch2.at(0).unicode();
	double size10=Size/10.0;
	if (scFace->canRender(ch[0]))
	{
		width = scFace->charWidth(ch[0])*size10;
		face = scFace->ftFace();
		/\****
			Ok, this looks like a regression between Freetype 2.1.9 -> 2.1.10.
			Ignoring the value of FT_HAS_KERNING for now -- AV
		 ****\/
		if (true || FT_HAS_KERNING(face) )
		{
			uint cl = FT_Get_Char_Index(face, c1);
			uint cr = FT_Get_Char_Index(face, c2);
			FT_Error error = FT_Get_Kerning(face, cl, cr, FT_KERNING_UNSCALED, &delta);
			if (error) {
				qDebug(QString("Error %2 when accessing kerning pair for font %1").arg(scFace->scName()).arg(error));
			}
			else {
				double uniEM = static_cast<double>(face->units_per_EM);
				width += delta.x / uniEM * size10;
			}
		}
		else {
			qDebug(QString("Font %1 has no kerning pairs (according to Freetype)").arg(scFace->scName()));
		}
		return width;
	}
	else
		return size10;
}

double RealCWidth(ScribusDoc *, ScFace* scFace, QString ch, int Size)
{
	double w, ww;
	uint c1 = ch.at(0).unicode();
	FT_Face      face;
	if (scFace->canRender(ch.at(0)))
	{
		face = scFace->ftFace();
		uint cl = FT_Get_Char_Index(face, c1);
		int error = FT_Load_Glyph(face, cl, FT_LOAD_NO_SCALE | FT_LOAD_NO_BITMAP );
		if (!error) {
			double uniEM = static_cast<double>(face->units_per_EM);
			w = (face->glyph->metrics.width + fabs((double)face->glyph->metrics.horiBearingX)) / uniEM * (Size / 10.0);
			ww = face->glyph->metrics.horiAdvance / uniEM * (Size / 10.0);
			return qMax(ww, w);
		}
		else
			sDebug(QString("internal error: missing glyph: %1 (char %2) error=%3").arg(c1).arg(ch).arg(error));

	}
	return static_cast<double>(Size / 10.0);
}

double RealCHeight(ScribusDoc *, ScFace* scFace, QString ch, int Size)
{
	double w;
	uint c1 = ch.at(0).unicode();
	FT_Face      face;
	if (scFace->canRender(ch.at(0)))
	{
		face = scFace->ftFace();
		uint cl = FT_Get_Char_Index(face, c1);
		int error = FT_Load_Glyph(face, cl, FT_LOAD_NO_SCALE | FT_LOAD_NO_BITMAP );
		if (!error) {
			double uniEM = static_cast<double>(face->units_per_EM);
			w = face->glyph->metrics.height / uniEM * (Size / 10.0);
		}
		else {
			sDebug(QString("internal error: missing glyph: %1 (char %2) error=%3").arg(c1).arg(ch).arg(error));
			w = Size / 10.0;
		}
		return w;
	}
	else
		return static_cast<double>(Size / 10.0);
}

double RealCAscent(ScribusDoc *, ScFace* scFace, QString ch, int Size)
{
	double w;
	uint c1 = ch.at(0).unicode();
	FT_Face      face;
	if (scFace->canRender(ch.at(0)))
	{
		face = scFace->ftFace();
		uint cl = FT_Get_Char_Index(face, c1);
		int error = FT_Load_Glyph(face, cl, FT_LOAD_NO_SCALE | FT_LOAD_NO_BITMAP );
		if (! error) {
			double uniEM = static_cast<double>(face->units_per_EM);
			w = face->glyph->metrics.horiBearingY / uniEM * (Size / 10.0);
		}
		else {
			sDebug(QString("internal error: missing glyph: %1 (char %2) error=%3").arg(c1).arg(ch).arg(error));
			w = Size / 10.0;
		}
		return w;
	}
	else
		return static_cast<double>(Size / 10.0);
}

double RealFHeight(ScribusDoc *, ScFace* scFace, int Size)
{
	FT_Face face = scFace->ftFace();
	double uniEM = static_cast<double>(face->units_per_EM);
	return face->height / uniEM * (Size / 10.0);
}
*/
