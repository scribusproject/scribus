/***************************************************************************
                          pslib.cpp  -  description
                             -------------------
    begin                : Sat May 26 2001
    copyright            : (C) 2001 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include <qfileinfo.h>
#include <qtextstream.h>
#include <qimage.h>
#include <qcolor.h>
#include <qcstring.h>
#include <qfontinfo.h>
#include <cstdlib>
#include <qregexp.h>

#if (_MSC_VER >= 1200)
 #include "win-config.h"
#else
 #include "config.h"
#endif

#include "pslib.h"
#include "scfonts.h"
#include <cmath>

extern void Level2Layer(ScribusDoc *doc, struct Layer *ll, int Level);
extern double Cwidth(ScribusDoc *doc, QString name, QString ch, int Siz, QString ch2 = " ");
extern bool loadText(QString nam, QString *Buffer);
extern QImage LoadPict(QString fn, bool *gray = 0);
extern QString CompressStr(QString *in);
extern QString ImageToCMYK_PS(QImage *im, int pl, bool pre);
extern QString MaskToTxt(QImage *im, bool PDF = true);
extern char *toHex( uchar u );
extern QString String2Hex(QString *in, bool lang = true);
#ifdef HAVE_CMS
extern bool CMSuse;
extern QImage LoadPictCol(QString fn, QString Prof, bool UseEmbedded, bool *realCMYK);
#endif

extern "C" void* Run(bool psart, SCFonts &AllFonts, QMap<QString,QFont> DocFonts, CListe DocColors, bool pdf);

void* Run(bool psart, SCFonts &AllFonts, QMap<QString,QFont> DocFonts, CListe DocColors, bool pdf)
{
	PSLib *dia = new PSLib(psart, AllFonts, DocFonts, DocColors, pdf);
	return dia;
}

PSLib::PSLib(bool psart, SCFonts &AllFonts, QMap<QString,QFont> DocFonts, CListe DocColors, bool pdf)
{
	QString tmp, tmp2, tmp3, tmp4, CHset;
	QStringList wt;
  	QString Epfad = PREL;
	Seiten = 0;
	User = "";
	Creator = "Scribus 1.3.0cvs";
	Titel = "";
	FillColor = "0.0 0.0 0.0 0.0";
	StrokeColor = "0.0 0.0 0.0 0.0";
	Header = psart ? "%!PS-Adobe-3.0\n" : "%!PS-Adobe-3.0 EPSF-3.0\n";
	BBox = "";
	BBoxH = "";
	Art = psart;
	isPDF = pdf;
	UsedFonts.clear();
	Fonts = "";
	FontDesc = "";
	GraySc = false;
	DoSep = false;
#ifdef HAVE_LIBZ
	CompAvail = true;
#else
	CompAvail = false;
#endif
	GrayCalc =  "/setcmykcolor {exch 0.11 mul add exch 0.59 mul add exch 0.3 mul add\n";
	GrayCalc += "               dup 1 gt {pop 1} if 1 exch sub oldsetgray} bind def\n";
	GrayCalc += "/setrgbcolor {0.11 mul exch 0.59 mul add exch 0.3 mul add\n";
	GrayCalc += "              oldsetgray} bind def\n";
	Farben = "%%CMYKCustomColor: ";
	FNamen = "%%DocumentCustomColors: ";
	CListe::Iterator itf;
	int c, m, y, k;
	bool erst = true;
	for (itf = DocColors.begin(); itf != DocColors.end(); ++itf)
	{
		if ((itf.key() != "Cyan") && (itf.key() != "Magenta") && (itf.key() != "Yellow") && 
			(itf.key() != "Black"))
		{
			DocColors[itf.key()].getCMYK(&c, &m, &y, &k);
			if (!erst)
			{
				Farben += "%%+ ";
				FNamen += "%%+ ";
			}
			Farben += ToStr(static_cast<double>(c) / 255) + " " +  
					ToStr(static_cast<double>(m) / 255) + " ";
			Farben += ToStr(static_cast<double>(y) / 255) + " " +  
					ToStr(static_cast<double>(k) / 255) + " (" + itf.key() + ")\n";
			FNamen += "(" + itf.key() + ")\n";
			erst = false;
		}
	}
	QMap<QString,QFont>::Iterator it;
	int a = 0;
	for (it = DocFonts.begin(); it != DocFonts.end(); ++it)
	{
/* Subset all TTF Fonts until the bug in the TTF-Embedding Code is fixed */
		QFileInfo fd = QFileInfo(AllFonts[it.key()]->Datei);
		QString fext = fd.extension(false).lower();

		if ((fext == "ttf") || (AllFonts[it.key()]->isOTF) || (AllFonts[it.key()]->Subset))
		{
			FontDesc += "/"+AllFonts[it.key()]->RealName()+
					" "+IToStr(AllFonts[it.key()]->RealGlyphs.count()+1)+" dict def\n";
			FontDesc += AllFonts[it.key()]->RealName()+" begin\n";
			QMap<uint,FPointArray>::Iterator ig;
			for (ig = AllFonts[it.key()]->RealGlyphs.begin(); 
				ig != AllFonts[it.key()]->RealGlyphs.end(); ++ig)
			{
				FontDesc += "/G"+IToStr(ig.key())+" { newpath\n";
				FPoint np, np1, np2;
				bool nPath = true;
				if (ig.data().size() > 3)
				{
					for (uint poi = 0; poi < ig.data().size()-3; poi += 4)
					{
						if (ig.data().point(poi).x() > 900000)
						{
							FontDesc += "cl\n";
							nPath = true;
							continue;
						}
						if (nPath)
						{
							np = ig.data().point(poi);
							FontDesc += ToStr(np.x()) + " " + ToStr(-np.y()) + " m\n";
							nPath = false;
						}
						np = ig.data().point(poi+1);
						np1 = ig.data().point(poi+3);
						np2 = ig.data().point(poi+2);
						FontDesc += ToStr(np.x()) + " " + ToStr(-np.y()) + " " + 
								ToStr(np1.x()) + " " + ToStr(-np1.y()) + " " + 
								ToStr(np2.x()) + " " + ToStr(-np2.y()) + " cu\n";
					}
				}
				FontDesc += "cl\n} bind def\n";
			}
			FontDesc += "end\n";
			AllFonts[it.key()]->RealGlyphs.clear();
		}
		else
		{
			UsedFonts.insert(it.key(), "/Fo"+IToStr(a));
			Fonts += "/Fo"+IToStr(a)+" /"+AllFonts[it.key()]->RealName()+" findfont definefont pop\n";
			if (AllFonts[it.key()]->EmbedPS)
			{
				QString tmp;
				if(AllFonts[it.key()]->EmbedFont(tmp))
				{
					FontDesc += "%%BeginFont: " + AllFonts[it.key()]->RealName() + "\n";
					FontDesc += tmp + "\n%%EndFont\n";
				}
			}
			GListe gl;
			AllFonts[it.key()]->GlNames(&gl);
			GlyphsOfFont.insert(it.key(), gl);
			a++;
		}
	}
	Prolog = "%%BeginProlog\n";
	Prolog += "/Scribusdict 100 dict def\n";
	Prolog += "Scribusdict begin\n";
	Prolog += "/sp {showpage} bind def\n";
	Prolog += "/oldsetgray /setgray load def\n";
	Prolog += "/cmyk {setcmykcolor} def\n";
	Prolog += "/m {moveto} bind def\n";
	Prolog += "/l {lineto} bind def\n";
	Prolog += "/li {lineto} bind def\n";
	Prolog += "/cu {curveto} bind def\n";
	Prolog += "/cl {closepath} bind def\n";
	Prolog += "/gs {gsave} bind def\n";
	Prolog += "/gr {grestore} bind def\n";
	Prolog += "/tr {translate} bind def\n";
	Prolog += "/ro {rotate} bind def\n";
	Prolog += "/sh {show} bind def\n";
	Prolog += "/shg {setcmykcolor moveto glyphshow} def\n";
	Prolog += "/sc {scale} bind def\n";
	Prolog += "/se {selectfont} bind def\n";
	Prolog += "/sf {setfont} bind def\n";
	Prolog += "/sw {setlinewidth} bind def\n";
	Prolog += "/f  {findfont} bind def\n";
	Prolog += "/fi {fill} bind def\n";
	Prolog += "/st {stroke} bind def\n";
	Prolog += "/shgf {gs dup scale begin cvx exec fill end gr} bind def\n";
	Prolog += "/shgs {gs dup 1 exch div currentlinewidth mul sw dup scale\n";
	Prolog += "       begin cvx exec st end gr} bind def\n";
	Prolog += "/bEPS {\n";
	Prolog += "    /b4_Inc_state save def\n";
	Prolog += "    /dict_count countdictstack def\n";
	Prolog += "    /op_count count 1 sub def\n";
	Prolog += "    userdict begin\n";
	Prolog += "    /showpage { } def\n";
	Prolog += "    0 setgray 0 setlinecap\n";
	Prolog += "    1 setlinewidth 0 setlinejoin\n";
	Prolog += "    10 setmiterlimit [ ] 0 setdash newpath\n";
	Prolog += "    /languagelevel where\n";
	Prolog += "    {pop languagelevel\n";
	Prolog += "    1 ne\n";
	Prolog += "    {false setstrokeadjust false setoverprint\n";
	Prolog += "    } if } if } bind def\n";
	Prolog += "/eEPS { count op_count sub {pop} repeat\n";
	Prolog += "    countdictstack dict_count sub {end} repeat\n";
	Prolog += "    b4_Inc_state restore } bind def\n";
	Prolog += "    end\n";
	Prolog += "%%EndProlog\n";
}

void PSLib::PutSeite(QString c)
{
	QTextStream t(&Spool);
	t.writeRawBytes(c, c.length());
}

void PSLib::PutDoc(QString c)
{
	QTextStream t(&Spool);
	t.writeRawBytes(c, c.length());
}

QString PSLib::ToStr(double c)
{
	QString cc;
	return cc.setNum(c);
}

QString PSLib::IToStr(int c)
{
	QString cc;
	return cc.setNum(c);
}

void PSLib::PS_set_Info(QString art, QString was)
{
	if (art == "Author")
		User = was;
	if (art == "Creator")
		Creator = was;
	if (art == "Title")
		Titel = was;
}

bool PSLib::PS_set_file(QString fn)
{
	Spool.setName(fn);
	return Spool.open(IO_WriteOnly);
}

void PSLib::PS_begin_doc(int Ori, double x, double y, double breite, double hoehe, int numpage)
{
	PutDoc(Header);
	PutDoc("%%For: " + User + "\n");
	PutDoc("%%Title: " + Titel + "\n");
	PutDoc("%%Creator: " + Creator + "\n");
	PutDoc("%%Pages: " + IToStr(numpage) + "\n");
	BBox = "%%BoundingBox: " + IToStr(qRound(x)) + " " + IToStr(qRound(y)) + " " + IToStr(qRound(breite)) + " " + IToStr(qRound(hoehe)) + "\n";
	BBoxH = "%%HiResBoundingBox: " + ToStr(x) + " " + ToStr(y) + " " + ToStr(breite) + " " + ToStr(hoehe) + "\n";
  	if (!Art)
	{
		PutDoc(BBox);
		PutDoc(BBoxH);
	}
	PutDoc(FNamen);
	PutDoc(Farben);
	PutDoc("%%LanguageLevel: 3\n");
	PutDoc("%%EndComments\n");
	PutDoc(Prolog);
	PutDoc("%%BeginSetup\n");
	PutDoc("/pdfmark where {pop} {userdict /pdfmark /cleartomark load put} ifelse\n");
	if (FontDesc != "")
		PutDoc(FontDesc);
	PutDoc("Scribusdict begin\n");
	PutDoc(Fonts);
	if (GraySc)
		PutDoc(GrayCalc);
	PutDoc("%%EndSetup\n");
/*  if ((Art) && (Ori != 0))
  	{
		PutSeite("<< /PageSize [ "+ToStr(breite)+" "+ToStr(hoehe)+" ]\n");
		PutSeite("/Orientation 3\n");
		PutSeite(">> setpagedevice\n");
		}                    */
	Prolog = "";
	FontDesc = "";
}

QString PSLib::PSEncode(QString in)
{
	QString tmp = "";
	QString cc;
	for (uint d = 0; d < in.length(); ++d)
	{
		cc = in.at(d);
		if ((cc == "(") || (cc == ")") || (cc == "\\"))
			tmp += "\\";
		tmp += cc;
	}
	tmp = tmp.simplifyWhiteSpace().replace( QRegExp("\\s"), "" );
	return tmp;
}

void PSLib::PS_TemplateStart(QString Name, double breite, double hoehe)
{
	PutDoc("/"+PSEncode(Name)+"\n<<\n");
	PutDoc("/FormType 1\n");
	PutDoc("/BBox [0 0 "+ToStr(breite)+" "+ToStr(hoehe)+"]\n");
	PutDoc("/Matrix [1 0 0 1 0 0]\n");
	PutDoc("/PaintProc\n{\n");
}

void PSLib::PS_UseTemplate(QString Name)
{
	PutDoc(PSEncode(Name)+" execform\n");
}

void PSLib::PS_TemplateEnd()
{
	PutDoc("pop } bind\n");
	PutDoc(">> def\n");
}

void PSLib::PS_begin_page(double breite, double hoehe, struct Margs* Ma, bool Clipping)
{
	if (Clipping)
	{
		PDev = ToStr(Ma->Left) + " " + ToStr(Ma->Bottom) + " m\n";
		PDev += ToStr(breite - Ma->Right) + " " + ToStr(Ma->Bottom) + " li\n";
		PDev += ToStr(breite - Ma->Right) + " " + ToStr(hoehe - Ma->Top) + " li\n";
		PDev += ToStr(Ma->Left) + " " + ToStr(hoehe - Ma->Top) + " li cl clip newpath\n";
	}
	Seiten++;
	PutSeite("%%Page: " + IToStr(Seiten) + " " + IToStr(Seiten) + "\nsave\n");
	if (Clipping)
		PutSeite(PDev);
  	PutSeite("/DeviceCMYK setcolorspace\n");
}

void PSLib::PS_end_page()
{
	PutSeite("%%PageTrailer\nrestore\nsp\n");
}

void PSLib::PS_curve(double x1, double y1, double x2, double y2, double x3, double y3)
{
	PutSeite(ToStr(x1) + " " + ToStr(y1) + " " + ToStr(x2) + " " + ToStr(y2) + " " + ToStr(x3) + " " + 
			ToStr(y3) + " curveto\n");
}

void PSLib::PS_moveto(double x, double y)
{
	PutSeite(ToStr(x) + " " + ToStr(y) + " m\n");
}

void PSLib::PS_lineto(double x, double y)
{
	PutSeite(ToStr(x) + " " + ToStr(y) + " li\n");
}

void PSLib::PS_closepath()
{
	PutSeite("cl\n");
}

void PSLib::PS_translate(double x, double y)
{
	PutSeite(ToStr(x) + " " + ToStr(y) + " tr\n");
}

void PSLib::PS_scale(double x, double y)
{
	PutSeite(ToStr(x) + " " + ToStr(y) + " sc\n");
}

void PSLib::PS_rotate(double x)
{
	PutSeite(ToStr(x) + " ro\n");
}

void PSLib::PS_clip(bool mu)
{
	PutSeite( mu ? "eoclip newpath\n" : "clip newpath\n" );
}

void PSLib::PS_save()
{
	PutSeite("gs\n");
}

void PSLib::PS_restore()
{
	PutSeite("gr\n");
}

void PSLib::PS_setcmykcolor_fill(double c, double m, double y, double k)
{
	FillColor = ToStr(c) + " " + ToStr(m) + " " + ToStr(y) + " " + ToStr(k);
}

void PSLib::PS_setcmykcolor_dummy()
{
	PutSeite("0 0 0 0 cmyk\n");
}

void PSLib::PS_setcmykcolor_stroke(double c, double m, double y, double k)
{
	StrokeColor = ToStr(c) + " " + ToStr(m) + " " + ToStr(y) + " " + ToStr(k);
}

void PSLib::PS_setlinewidth(double w)
{
	PutSeite(ToStr(w) + " sw\n");
	LineW = w;
}

void PSLib::PS_setdash(Qt::PenStyle st, double offset, QValueList<double> dash)
{
	QString Dt = ToStr(QMAX(2*LineW, 1));
	QString Da = ToStr(QMAX(6*LineW, 1));
	if (dash.count() != 0)
	{
		PutSeite("[ ");
		QValueList<double>::iterator it;
		for ( it = dash.begin(); it != dash.end(); ++it )
		{
			PutSeite(IToStr(static_cast<int>(*it))+" ");
		}
		PutSeite("] "+IToStr(static_cast<int>(offset))+" setdash\n");
	}
	else
	{
		switch (st)
		{
			case Qt::SolidLine:
				PutSeite("[] 0 setdash\n");
				break;
			case Qt::DashLine:
				PutSeite("["+Da+" "+Dt+"] 0 setdash\n");
				break;
			case Qt::DotLine:
				PutSeite("["+Dt+"] 0 setdash\n");
				break;
			case Qt::DashDotLine:
				PutSeite("["+Da+" "+Dt+" "+Dt+" "+Dt+"] 0 setdash\n");
				break;
			case Qt::DashDotDotLine:
				PutSeite("["+Da+" "+Dt+" "+Dt+" "+Dt+" "+Dt+" "+Dt+"] 0 setdash\n");
				break;
			default:
				PutSeite("[] 0 setdash\n");
				break;
		}
	}
}
void PSLib::PS_setcapjoin(Qt::PenCapStyle ca, Qt::PenJoinStyle jo)
{
	switch (ca)
		{
		case Qt::FlatCap:
			PutSeite("0 setlinecap\n");
			break;
		case Qt::SquareCap:
			PutSeite("2 setlinecap\n");
			break;
		case Qt::RoundCap:
			PutSeite("1 setlinecap\n");
			break;
		default:
			PutSeite("0 setlinecap\n");
			break;
		}
	switch (jo)
		{
		case Qt::MiterJoin:
			PutSeite("0 setlinejoin\n");
			break;
		case Qt::BevelJoin:
			PutSeite("2 setlinejoin\n");
			break;
		case Qt::RoundJoin:
			PutSeite("1 setlinejoin\n");
			break;
		default:
			PutSeite("0 setlinejoin\n");
			break;
		}
}

void PSLib::PS_selectfont(QString f, double s)
{
	PutSeite(UsedFonts[f] + " " + ToStr(s) + " se\n");
}

void PSLib::PS_fill()
{
	PutSeite(FillColor + " cmyk eofill\n");
}

void PSLib::PS_stroke()
{
	PutSeite(StrokeColor + " cmyk st\n");
}

void PSLib::PS_fill_stroke()
{
	PS_save();
	PS_fill();
	PS_restore();
	PS_stroke();
}

void PSLib::PS_newpath()
{
	PutSeite("newpath\n");
}

void PSLib::PS_MultiRadGradient(double w, double h, double x, double y, QValueList<double> Stops, QStringList Colors)
{
	bool first = true;
	PutSeite( "clipsave\n" );
	PutSeite("eoclip\n");
	for (uint c = 0; c < Colors.count()-1; ++c)
	{
		PutSeite("<<\n");
		PutSeite("/ShadingType 3\n");
		PutSeite( DoSep ? "/ColorSpace /DeviceGray\n" : "/ColorSpace /DeviceCMYK\n" );
		PutSeite("/BBox [0 "+ToStr(h)+" "+ToStr(w)+" 0]\n");
		if (Colors.count() == 2)
			PutDoc("/Extend [true true]\n");
		else
		{
			if (first)
				PutSeite("/Extend [false true]\n");
			else
			{
				if (c == Colors.count()-2)
					PutSeite("/Extend [true false]\n");
				else
					PutSeite("/Extend [false false]\n");
			}
		}
		PutSeite("/Coords ["+ToStr(x)+" "+ToStr(y)+" "+ToStr((*Stops.at(c+1)))+" "+ToStr(x)+" "+ToStr(y)+" "+ToStr((*Stops.at(c)))+"]\n");
		PutSeite("/Function\n");
		PutSeite("<<\n");
		PutSeite("/FunctionType 2\n");
		PutSeite("/Domain [0 1]\n");
		if (DoSep)
		{
			int pla = Plate - 1 < 0 ? 3 : Plate - 1;
			QStringList cols1 = QStringList::split(" ", Colors[c+1]);
			QStringList cols2 = QStringList::split(" ", Colors[c]);
			PutSeite("/C1 ["+ToStr(1-cols1[pla].toDouble())+"]\n");
			PutSeite("/C0 ["+ToStr(1-cols2[pla].toDouble())+"]\n");
		}
		else
		{
			PutSeite("/C0 ["+Colors[c+1]+"]\n");
			PutSeite("/C1 ["+Colors[c]+"]\n");
		}
		PutSeite("/N 1\n");
		PutSeite(">>\n");
		PutSeite(">>\n");
		PutSeite("shfill\n");
		first = false;
	}
	PutSeite("cliprestore\n");
}

void PSLib::PS_MultiLinGradient(double w, double h, QValueList<double> Stops, QStringList Colors)
{
	bool first = true;
	PutSeite( "clipsave\n" );
	PutSeite("eoclip\n");
	for (uint c = 0; c < Colors.count()-1; ++c)
	{
		PutSeite("<<\n");
		PutSeite("/ShadingType 2\n");
		PutSeite( DoSep ? "/ColorSpace /DeviceGray\n" : "/ColorSpace /DeviceCMYK\n" );
		PutSeite("/BBox [0 "+ToStr(h)+" "+ToStr(w)+" 0]\n");
		if (Colors.count() == 2)
			PutDoc("/Extend [true true]\n");
		else
		{
			if (first)
				PutSeite("/Extend [true false]\n");
			else
			{
				if (c == Colors.count()-2)
					PutSeite("/Extend [false true]\n");
				else
					PutSeite("/Extend [false false]\n");
			}
		}
		first = false;
		PutSeite("/Coords ["+ToStr((*Stops.at(c*2)))+"  "+ToStr((*Stops.at(c*2+1)))+" "+ToStr((*Stops.at(c*2+2)))+" "+ToStr((*Stops.at(c*2+3)))+"]\n");
		PutSeite("/Function\n");
		PutSeite("<<\n");
		PutSeite("/FunctionType 2\n");
		PutSeite("/Domain [0 1]\n");
		if (DoSep)
		{
			int pla = Plate - 1 < 0 ? 3 : Plate - 1;
			QStringList cols1 = QStringList::split(" ", Colors[c]);
			QStringList cols2 = QStringList::split(" ", Colors[c+1]);
			PutSeite("/C1 ["+ToStr(1-cols1[pla].toDouble())+"]\n");
			PutSeite("/C0 ["+ToStr(1-cols2[pla].toDouble())+"]\n");
		}
		else
		{
			PutSeite("/C0 ["+Colors[c]+"]\n");
			PutSeite("/C1 ["+Colors[c+1]+"]\n");
		}
		PutSeite("/N 1\n");
		PutSeite(">>\n");
		PutSeite(">>\n");
		PutSeite("shfill\n");
	}
	PutSeite("cliprestore\n");
}

void PSLib::PS_show_xyG(QString font, QString ch, double x, double y)
{
	QString Name;
	uint cc = ch[0].unicode();
	Name = GlyphsOfFont[font].contains(cc) ? GlyphsOfFont[font][cc] : QString(".notdef");
	PutSeite("/"+Name+" "+ToStr(x)+" "+ToStr(y)+" "+StrokeColor+" shg\n");
}

void PSLib::PS_show(double x, double y)
{
	PS_moveto(x, y);
	PutSeite("/hyphen glyphshow\n");
}

void PSLib::PS_showSub(uint chr, QString font, double size, bool stroke)
{
	PutSeite(FillColor + " cmyk (G"+IToStr(chr)+") "+font+" "+ToStr(size / 10.0)+" ");
	PutSeite(stroke ? "shgs\n" : "shgf\n");
}

void PSLib::PS_ImageData(bool inver, QString fn, QString Name, QString Prof, bool UseEmbedded, bool UseProf)
{
	QString tmp;
	QFileInfo fi = QFileInfo(fn);
	QString ext = fi.extension(false).lower();
	if (ext == "eps")
	{
		if (loadText(fn, &tmp))
		{
			PutSeite("currentfile 1 (%ENDEPSDATA) /SubFileDecode filter /ReusableStreamDecode filter\n");
      			PutSeite("%%BeginDocument: " + fi.fileName() + "\n");
      			PutSeite(tmp+"\n");
      			PutSeite("%ENDEPSDATA\n");
      			PutSeite("%%EndDocument\n");
			PutSeite("/"+PSEncode(Name)+"Bild exch def\n");
		}
		return;
	}
  	QString ImgStr = "";
	QImage image;
#ifdef HAVE_CMS
	QImage image2;
	bool cmy = false;
	if ((CMSuse) && (UseProf))
	{
		image = LoadPict(fn);
		image = image.convertDepth(32);
		image2 = LoadPictCol(fn, Prof, UseEmbedded, &cmy);
		if (inver)
		{
			image.invertPixels();
			image2.invertPixels();
		}
		ImgStr = ImageToCMYK_PS(&image2, -1, cmy);
	}
	else
	{
		image = LoadPict(fn);
		if (inver)
			image.invertPixels();
		ImgStr = ImageToCMYK_PS(&image, -1, false);
	}
#else
	image = LoadPict(fn);
  image = image.convertDepth(32);
	if (inver)
		image.invertPixels();
	ImgStr = ImageToCMYK_PS(&image, -1, false);
#endif
	if (CompAvail)
	{
		PutSeite("currentfile /ASCIIHexDecode filter /FlateDecode filter /ReusableStreamDecode filter\n");
		ImgStr = CompressStr(&ImgStr);
	}
	else
		PutSeite("currentfile /ASCIIHexDecode filter /ReusableStreamDecode filter\n");
	ImgStr = String2Hex(&ImgStr);
	PutSeite(ImgStr);
	PutSeite("\n>\n");
	PutSeite("/"+PSEncode(Name)+"Bild exch def\n");
	ImgStr = "";
  if (image.hasAlphaBuffer())
   	{
		QImage iMask = image.createAlphaMask();
		ImgStr = MaskToTxt(&iMask, false);
		if (CompAvail)
		{
			PutSeite("currentfile /ASCIIHexDecode filter /FlateDecode filter /ReusableStreamDecode filter\n");
			ImgStr = CompressStr(&ImgStr);
		}
		else
			PutSeite("currentfile /ASCIIHexDecode filter /ReusableStreamDecode filter\n");
		ImgStr = String2Hex(&ImgStr);
		PutSeite(ImgStr);
		PutSeite("\n>\n");
		PutSeite("/"+PSEncode(Name)+"Mask exch def\n");
	}
}

void PSLib::PS_image(bool inver, double x, double y, QString fn, double scalex, double scaley, QString Prof, bool UseEmbedded, bool UseProf, QString Name)
{
	QString tmp;
	QFileInfo fi = QFileInfo(fn);
	QString ext = fi.extension(false).lower();
	if (ext == "eps")
	{
		if (loadText(fn, &tmp))
		{
			PutSeite("bEPS\n");
      			PutSeite(ToStr(scalex) + " " + ToStr(scaley) + " sc\n");
      			PutSeite(ToStr(x) + " " + ToStr(y) + " tr\n");
			if (Name != "")
			{
				PutSeite(PSEncode(Name)+"Bild cvx exec\n");
				PutSeite(PSEncode(Name)+"Bild resetfile\n");
			}
			else
			{
      				PutSeite("%%BeginDocument: " + fi.fileName() + "\n");
      				PutSeite(tmp+"\n");
      				PutSeite("%%EndDocument\n");
			}
			PutSeite("eEPS\n");
		}
	}
	else
	{
    		QString ImgStr = "";
		QImage image;
#ifdef HAVE_CMS
		QImage image2;
		bool cmy = false;
    		image = LoadPict(fn);
		image = image.convertDepth(32);
    		if ((CMSuse) && (UseProf))
			image2 = LoadPictCol(fn, Prof, UseEmbedded, &cmy);
		if (inver)
		{
			image.invertPixels();
			image2.invertPixels();
		}
#else
		image = LoadPict(fn);
  		image = image.convertDepth(32);
		if (inver)
			image.invertPixels();
#endif
		int w = image.width();
		int h = image.height();
    		PutSeite(ToStr(x*scalex) + " " + ToStr(y*scaley) + " tr\n");
    		PutSeite(ToStr(scalex*w) + " " + ToStr(scaley*h) + " sc\n");
    		PutSeite(((!DoSep) && (!GraySc)) ? "/DeviceCMYK setcolorspace\n" : "/DeviceGray setcolorspace\n");
    		if (image.hasAlphaBuffer())
    		{
			QImage iMask = image.createAlphaMask();
#ifdef HAVE_CMS
			if ((CMSuse) && (UseProf))
			{
				if (DoSep)
					ImgStr = ImageToCMYK_PS(&image2, Plate, cmy);
				else
					ImgStr = GraySc ? ImageToCMYK_PS(&image2, -2, cmy) : 
								ImageToCMYK_PS(&image2, -1, cmy);
			}
			else
			{
				if (DoSep)
					ImgStr = ImageToCMYK_PS(&image, Plate, false);
				else
					ImgStr = GraySc ? ImageToCMYK_PS(&image, -2, false) : 
								ImageToCMYK_PS(&image, -1, false);
			}
#else
			if (DoSep)
				ImgStr = ImageToCMYK_PS(&image, Plate, false);
			else
				ImgStr = GraySc ? ImageToCMYK_PS(&image, -2, false) : ImageToCMYK_PS(&image, -1, 
													false);
#endif
			if (Name == "")
			{
				if (CompAvail)
				{
					PutSeite("currentfile /ASCIIHexDecode filter /FlateDecode filter /ReusableStreamDecode filter\n");
					ImgStr = CompressStr(&ImgStr);
				}
				else
					PutSeite("currentfile /ASCIIHexDecode filter /ReusableStreamDecode filter\n");
				ImgStr = String2Hex(&ImgStr);
				PutSeite(ImgStr);
				ImgStr = "";
				PutSeite("\n>\n");
				PutSeite("/Bild exch def\n");
				ImgStr = MaskToTxt(&iMask, false);
				if (CompAvail)
				{
					PutSeite("currentfile /ASCIIHexDecode filter /FlateDecode filter /ReusableStreamDecode filter\n");
					ImgStr = CompressStr(&ImgStr);
				}
				else
					PutSeite("currentfile /ASCIIHexDecode filter /ReusableStreamDecode filter\n");
				ImgStr = String2Hex(&ImgStr);
				PutSeite(ImgStr);
				PutSeite("\n>\n");
				PutSeite("/Mask exch def\n");
			}
			PutSeite("<<\n");
			PutSeite("  /PaintType   1\n");
			PutSeite("  /PatternType 1\n");
			PutSeite("  /TilingType  3\n");
			PutSeite("  /BBox        [ 0 0 1 1 ]\n");
			PutSeite("  /XStep       2\n");
			PutSeite("  /YStep       2\n");
			PutSeite("  /PaintProc   {\n");
			PutSeite("   pop\n");
			PutSeite("   1 1 1 1 setcmykcolor\n");
			PutSeite("   <<\n");
			PutSeite("   /ImageType 1\n");
			PutSeite("   /Height    " + IToStr(h) + "\n");
			PutSeite("   /Width     " + IToStr(w) + "\n");
			PutSeite("   /ImageMatrix [" + IToStr(w) + " 0 0 " + IToStr(-h) + " 0 " + IToStr(h) 
				+"]\n");
			if (DoSep)
				PutSeite("   /Decode [1 0]\n");
			else
				PutSeite( GraySc ? "   /Decode [1 0]\n" : "   /Decode [0 1 0 1 0 1 0 1]\n" );
			PutSeite("   /BitsPerComponent 8\n");
			PutSeite("   /DataSource "+PSEncode(Name)+"Bild\n");
			PutSeite("   >>\n");
			PutSeite("   image\n");
			PutSeite("   }\n");
			PutSeite(">> matrix makepattern setpattern\n");
			PutSeite("<< /ImageType 1\n");
			PutSeite("   /Width " + IToStr(w) + "\n");
			PutSeite("   /Height " + IToStr(h) + "\n");
			PutSeite("   /BitsPerComponent 1\n");
			PutSeite("   /Decode [1 0]\n");
			PutSeite("   /ImageMatrix [" + IToStr(w) + " 0 0 " + IToStr(-h) + " 0 " + IToStr(h) + "]\n");
			PutSeite("   /DataSource "+PSEncode(Name)+"Mask\n");
			PutSeite(">>\n");
			PutSeite("imagemask\n");
			if (Name != "")
			{
				PutSeite(PSEncode(Name)+"Bild resetfile\n");
				PutSeite(PSEncode(Name)+"Mask resetfile\n");
			}
		}
		else
		{
			PutSeite("<< /ImageType 1\n");
			PutSeite("   /Width " + IToStr(w) + "\n");
			PutSeite("   /Height " + IToStr(h) + "\n");
			PutSeite("   /BitsPerComponent 8\n");
			if (DoSep)
				PutSeite("   /Decode [1 0]\n");
			else
				PutSeite( GraySc ? "   /Decode [1 0]\n" : "   /Decode [0 1 0 1 0 1 0 1]\n");
			PutSeite("   /ImageMatrix [" + IToStr(w) + " 0 0 " + IToStr(-h) + " 0 " + IToStr(h) + 
					"]\n");
			if (Name != "")
			{
				PutSeite("   /DataSource "+PSEncode(Name)+"Bild >>\n");
				PutSeite("image\n");
				PutSeite(PSEncode(Name)+"Bild resetfile\n");
			}
			else
			{
				PutSeite ( CompAvail ? "   /DataSource currentfile /ASCIIHexDecode filter /FlateDecode filter >>\n" :
							"   /DataSource currentfile /ASCIIHexDecode filter >>\n");
				PutSeite("image\n");
#ifdef HAVE_CMS
				if ((CMSuse) && (UseProf))
				{
					if (DoSep)
						ImgStr = ImageToCMYK_PS(&image2, Plate, cmy);
					else
						ImgStr = GraySc ? ImageToCMYK_PS(&image2, -2, cmy) : 
									ImageToCMYK_PS(&image2, -1, cmy);
				}
				else
				{
					if (DoSep)
						ImgStr = ImageToCMYK_PS(&image, Plate, false);
					else
						ImgStr = GraySc ? ImageToCMYK_PS(&image, -2, false) : 
									ImageToCMYK_PS(&image, -1, false);
				}
#else
				if (DoSep)
					ImgStr = ImageToCMYK_PS(&image, Plate, false);
				else
					ImgStr = GraySc ? ImageToCMYK_PS(&image, -2, false) : 		
								ImageToCMYK_PS(&image, -1, false);
#endif
				if (CompAvail)
					ImgStr = CompressStr(&ImgStr);
				ImgStr = String2Hex(&ImgStr);
				PutSeite(ImgStr);
				PutSeite("\n>\n");
			}
		}
	}
}


void PSLib::PS_plate(int nr)
{
	switch (nr)
	{
		case 0:
			PutSeite("%%PlateColor Black\n");
			PutSeite("/setcmykcolor {exch pop exch pop exch pop 1 exch sub oldsetgray} bind def\n");
			PutSeite("/setrgbcolor {pop pop pop 1 oldsetgray} bind def\n");
			break;
		case 1:
			PutSeite("%%PlateColor Cyan\n");
			PutSeite("/setcmykcolor {pop pop pop 1 exch sub oldsetgray} bind def\n");
			PutSeite("/setrgbcolor {pop pop oldsetgray} bind def\n");
			break;
		case 2:
			PutSeite("%%PlateColor Magenta\n");
			PutSeite("/setcmykcolor {pop pop exch pop 1 exch sub oldsetgray} bind def\n");
			PutSeite("/setrgbcolor {pop exch pop oldsetgray} bind def\n");
			break;
		case 3:
			PutSeite("%%PlateColor Yellow\n");
			PutSeite("/setcmykcolor {pop exch pop exch pop 1 exch sub oldsetgray} bind def\n");
			PutSeite("/setrgbcolor {exch pop exch pop oldsetgray} bind def\n");
			break;
	}
	Plate = nr;
	DoSep = true;
}

void PSLib::PS_setGray()
{
	GraySc = true;
}

void PSLib::PDF_Bookmark(QString text, uint Seite)
{
	PutSeite("[/Title ("+text+") /Page "+IToStr(Seite)+" /View [/Fit]\n");
	PutSeite("/OUT pdfmark\n");
	isPDF = true;
}

void PSLib::PDF_Annotation(QString text, double x, double y, double b, double h)
{
	PutSeite("[ /Rect [ "+ToStr(static_cast<int>(x))+" "+ToStr(static_cast<int>(y))
			+" "+ToStr(static_cast<int>(b))+" "+ToStr(static_cast<int>(h))+" ]\n");
	PutSeite("  /Contents ("+text+")\n  /Open false\n");
	PutSeite("/ANN pdfmark\n");
	isPDF = true;
}


void PSLib::PS_close()
{
	PutDoc("%%Trailer\n");
	PutDoc("end\n");
	PutDoc("%%EOF\n");
	Spool.close();
}


void PSLib::PS_insert(QString i)
{
	PutDoc(i);
}

void PSLib::CreatePS(ScribusDoc* Doc, ScribusView* view, std::vector<int> &pageNs, bool sep, QString SepNam, bool farb, bool Hm, bool Vm, bool Ic, bool gcr)
{
	uint a;
	int sepac;
	double wideR;
	QValueList<double> dum;
	double gx, gy, gw, gh;
	dum.clear();
	PS_set_Info("Author", Doc->DocAutor);
	PS_set_Info("Title", Doc->DocTitel);
	if (!farb)
		PS_setGray();
	if ((!Art) && (view->SelItem.count() != 0))
	{
		view->setGroupRect();
		view->getGroupRect(&gx, &gy, &gw, &gh);
		int pgNum = pageNs[0];
		gx -= Doc->Pages.at(pgNum)->Xoffset;
		gy -= Doc->Pages.at(pgNum)->Yoffset;
		PS_begin_doc(Doc->PageOri, gx, Doc->PageH - (gy+gh), gx + gw, Doc->PageH - gy, 1);
	}
	else
		PS_begin_doc(Doc->PageOri, 0.0, 0.0, Doc->PageB, Doc->PageH, pageNs.size());
	for (uint ap = 0; ap < Doc->MasterPages.count(); ++ap)
	{
		if (Doc->MasterItems.count() != 0)
		{
			if ((!sep) && (farb))
			{
				for (uint api = 0; api < Doc->MasterItems.count(); ++api)
				{
					PageItem *it = Doc->MasterItems.at(api);
					int x = static_cast<int>(Doc->MasterPages.at(ap)->Xoffset);
					int y = static_cast<int>(Doc->MasterPages.at(ap)->Yoffset);
					int w = static_cast<int>(Doc->MasterPages.at(ap)->Width);
					int h = static_cast<int>(Doc->MasterPages.at(ap)->Height);
					int x2 = static_cast<int>(it->BoundingX);
					int y2 = static_cast<int>(it->BoundingY);
					int w2 = static_cast<int>(it->BoundingW);
					int h2 = static_cast<int>(it->BoundingH);
					if (!QRect(x, y, w, h).intersects(QRect(x2, y2, w2, h2)))
						continue;
					if ((it->OwnPage != static_cast<int>(Doc->MasterPages.at(ap)->PageNr)) && (it->OwnPage != -1))
						continue;
					if ((it->PType == 2) && (it->PicAvail) && (it->Pfile != "") && (it->isPrintable))
						PS_ImageData(it->InvPict, it->Pfile, it->AnName, it->IProfile, it->UseEmbedded, Ic);
				}
			}
			PS_TemplateStart(Doc->MasterPages.at(ap)->PageNam, Doc->PageB, Doc->PageH);
			ProcessPage(Doc, view, Doc->MasterPages.at(ap), ap+1, sep, farb, Ic, gcr);
			PS_TemplateEnd();
		}
	}
	sepac = 0;
	uint aa = 0;
	while (aa < pageNs.size())
	{
		a = pageNs[aa]-1;
		if ((!Art) && (view->SelItem.count() != 0))
		{
			struct Margs Ma;
			Ma.Left = gx;
			Ma.Top = gy;
			Ma.Bottom = Doc->PageH - (gy + gh);
			Ma.Right = Doc->PageB - (gx + gw);
			PS_begin_page(Doc->PageB, Doc->PageH, &Ma, true);
		}
		else
			PS_begin_page(Doc->PageB, Doc->PageH, &Doc->Pages.at(a)->Margins, view->Prefs->ClipMargin);
		if (Hm)
		{
			PS_translate(Doc->PageB, 0);
			PS_scale(-1, 1);
		}
		if (Vm)
		{
			PS_translate(0, Doc->PageH);
			PS_scale(1, -1);
		}
		if (sep)
		{
			if (SepNam == "Black")
				PS_plate(0);
			if (SepNam == "Cyan")
				PS_plate(1);
			if (SepNam == "Magenta")
				PS_plate(2);
			if (SepNam == "Yellow")
				PS_plate(3);
			if (SepNam == QObject::tr("All"))
				PS_plate(sepac);
		}
		if (Doc->Pages.at(a)->MPageNam != "")
		{
			int h, s, v, k, tsz;
			QCString chxc;
			QString chx;
			struct Pti *hl;
			int Lnr = 0;
			struct Layer ll;
			ll.Drucken = false;
			ll.LNr = 0;
			Page* mPage = Doc->MasterPages.at(Doc->MasterNames[Doc->Pages.at(a)->MPageNam]);
			if (Doc->MasterItems.count() != 0)
			{
				PS_UseTemplate(Doc->Pages.at(a)->MPageNam);
				for (uint lam = 0; lam < Doc->Layers.count(); ++lam)
				{
					Level2Layer(Doc, &ll, Lnr);
					if (ll.Drucken)
					{
						for (uint am = 0; am < Doc->Pages.at(a)->FromMaster.count(); ++am)
						{
							PageItem *ite = Doc->Pages.at(a)->FromMaster.at(am);
							if ((ite->LayerNr != ll.LNr) || (!ite->isPrintable))
								continue;
							if ((ite->PType == 2) && ((sep) || (!farb)))
							{
								PS_save();
								PS_translate(ite->Xpos - mPage->Xoffset, Doc->PageH -(ite->Ypos) - mPage->Yoffset);
								if (ite->Rot != 0)
									PS_rotate(-ite->Rot);
								if (ite->Pcolor != "None")
								{
									SetFarbe(Doc, ite->Pcolor, ite->Shade, &h, &s, &v, &k, gcr);
									PS_setcmykcolor_fill(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
									SetClipPath(&ite->PoLine);
									PS_closepath();
									PS_fill();
								}
								else
									PS_setcmykcolor_dummy();
								SetClipPath(&ite->PoLine);
								PS_closepath();
								PS_clip(false);
								PS_save();
								if ((ite->flippedH % 2) != 0)
								{
									PS_translate(ite->Width, 0);
									PS_scale(-1, 1);
								}
								if ((ite->flippedV % 2) != 0)
								{
									PS_translate(0, -ite->Height);
									PS_scale(1, -1);
								}
								if ((ite->PicAvail) && (ite->Pfile != ""))
								{
									PS_translate(0, -ite->BBoxH*ite->LocalScY);
									PS_image(ite->InvPict, -ite->BBoxX+ite->LocalX, -ite->LocalY, ite->Pfile, ite->LocalScX, ite->LocalScY, ite->IProfile, ite->UseEmbedded, Ic, ite->AnName);
								}
								PS_restore();
								if (((ite->Pcolor2 != "None") || (ite->NamedLStyle != "")) && (!ite->isTableItem))
								{
									if ((ite->NamedLStyle == "") && (ite->Pwidth != 0.0))
									{
										SetFarbe(Doc, ite->Pcolor2, ite->Shade2, &h, &s, &v, &k, gcr);
										PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
										PS_setlinewidth(ite->Pwidth);
										PS_setcapjoin(ite->PLineEnd, ite->PLineJoin);
										PS_setdash(ite->PLineArt, ite->DashOffset, ite->DashValues);
										SetClipPath(&ite->PoLine);
										PS_closepath();
										PS_stroke();
									}
									else
									{
										multiLine ml = Doc->MLineStyles[ite->NamedLStyle];
										for (int it = ml.size()-1; it > -1; it--)
										{
											SetFarbe(Doc, ml[it].Color, ml[it].Shade, &h, &s, &v, &k, gcr);
											PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
											PS_setlinewidth(ml[it].Width);
											PS_setcapjoin(static_cast<Qt::PenCapStyle>(ml[it].LineEnd), static_cast<Qt::PenJoinStyle>(ml[it].LineJoin));
											PS_setdash(static_cast<Qt::PenStyle>(ml[it].Dash), 0, dum);
											SetClipPath(&ite->PoLine);
											PS_closepath();
											PS_stroke();
										}
									}
								}
								PS_restore();
							}
							if (ite->PType == 4)
							{
								double savScale = view->Scale;
								view->Scale = 1.0;
								Doc->RePos = true;
								QPixmap pgPix(10, 10);
								QRect rd = QRect(0,0,9,9);
								ScPainter *painter = new ScPainter(&pgPix, pgPix.width(), pgPix.height());
								ite->DrawObj(painter, rd);
								Doc->RePos = false;
								view->Scale = savScale;
								delete painter;
								PS_save();
								if (ite->Pcolor != "None")
								{
									SetFarbe(Doc, ite->Pcolor, ite->Shade, &h, &s, &v, &k, gcr);
									PS_setcmykcolor_fill(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
								}
								PS_translate(ite->Xpos - mPage->Xoffset, Doc->PageH - (ite->Ypos - mPage->Yoffset));
								if (ite->Rot != 0)
									PS_rotate(-ite->Rot);
								if ((ite->Pcolor != "None") || (ite->GrType != 0))
								{
									SetClipPath(&ite->PoLine);
									PS_closepath();
									PS_fill();
								}
								if ((ite->flippedH % 2) != 0)
								{
									PS_translate(ite->Width, 0);
									PS_scale(-1, 1);
								}
								if ((ite->flippedV % 2) != 0)
								{
									PS_translate(0, -ite->Height);
									PS_scale(1, -1);
								}
								for (uint d = 0; d < ite->MaxChars; ++d)
								{
									hl = ite->Ptext.at(d);
									if ((hl->ch == QChar(13)) || (hl->ch == QChar(10)) || (hl->ch == QChar(9)) || (hl->ch == QChar(28)))
										continue;
									if (hl->cstyle & 256)
										continue;
									if (hl->yp == 0)
										continue;
									tsz = hl->csize;
									chx = hl->ch;
									if (hl->ch == QChar(29))
										chx = " ";
									if (hl->ch == QChar(30))
									{
										if (Doc->MasterP)
											chx = "#";
										else
										{
											uint zae = 0;
											uint za2 = d;
											do
											{
												if (za2 == 0)
													break;
												za2--;
											}
											while (ite->Ptext.at(za2)->ch == QChar(30));
											if (ite->Ptext.at(za2)->ch != QChar(30))
												za2++;
											while (ite->Ptext.at(za2+zae)->ch == QChar(30))
											{
												zae++;
												if (za2+zae == ite->MaxChars)
													break;
											}
											QString out="%1";
											QString out2;
											out2 = out.arg(a+Doc->FirstPnum, -zae);
											chx = out2.mid(d-za2, 1);
										}
									}
									if (hl->cstyle & 64)
									{
										if (chx.upper() != chx)
										{
											tsz = hl->csize * Doc->VKapit / 100;
											chx = chx.upper();
										}
									}
									if (hl->cstyle & 1)
										tsz = hl->csize * Doc->VHochSc / 100;
									if (hl->cstyle & 2)
										tsz = hl->csize * Doc->VHochSc / 100;
									/* Subset all TTF Fonts until the bug in the TTF-Embedding Code is fixed */
									QFileInfo fd = QFileInfo((*Doc->AllFonts)[hl->cfont]->Datei);
									QString fext = fd.extension(false).lower();
									if ((fext == "ttf") || ((*Doc->AllFonts)[hl->cfont]->isOTF) || ((*Doc->AllFonts)[hl->cfont]->Subset))
									{
										uint chr = chx[0].unicode();
										if (((*Doc->AllFonts)[hl->cfont]->CharWidth.contains(chr)) && (chr != 32))
										{
											PS_save();
											if (ite->Reverse)
											{
												PS_translate(hl->xp, (hl->yp - (tsz / 10.0)) * -1);
												PS_scale(-1, 1);
												if (d < ite->MaxChars-1)
												{
													QString ctx = ite->Ptext.at(d+1)->ch;
													if (ctx == QChar(29))
														ctx = " ";
													wideR = -Cwidth(Doc, hl->cfont, chx, tsz, ctx) * (hl->cscale / 100.0);
												}
												else
													wideR = -Cwidth(Doc, hl->cfont, chx, tsz) * (hl->cscale / 100.0);
												PS_translate(wideR, 0);
											}
											else
												PS_translate(hl->xp, (hl->yp - (tsz / 10.0)) * -1);
											if (hl->cscale != 100)
												PS_scale(hl->cscale / 100.0, 1);
											if (hl->ccolor != "None")
											{
												SetFarbe(Doc, hl->ccolor, hl->cshade, &h, &s, &v, &k, gcr);
												PS_setcmykcolor_fill(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
												PS_showSub(chr, (*Doc->AllFonts)[hl->cfont]->RealName(), tsz / 10.0, false);
											}
											PS_restore();
										}
									}
									else
									{
										PS_selectfont(hl->cfont, tsz / 10.0);
										if (hl->ccolor != "None")
										{
											SetFarbe(Doc, hl->ccolor, hl->cshade, &h, &s, &v, &k, gcr);
											PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
										}
										PS_save();
										if (ite->Reverse)
										{
											int chs = hl->csize;
											ite->SetZeichAttr(hl, &chs, &chx);
											PS_translate(hl->xp, -hl->yp);
											PS_scale(-1, 1);
											if (d < ite->MaxChars-1)
											{
												QString ctx = ite->Ptext.at(d+1)->ch;
												if (ctx == QChar(29))
													ctx = " ";
												wideR = -Cwidth(Doc, hl->cfont, chx, chs, ctx) * (hl->cscale / 100.0);
												PS_translate(wideR, 0);
											}
											else
											{
												wideR = -Cwidth(Doc, hl->cfont, chx, chs) * (hl->cscale / 100.0);
												PS_translate(wideR, 0);
											}
											if (hl->cscale != 100)
												PS_scale(hl->cscale / 100.0, 1);
											PS_show_xyG(hl->cfont, chx, 0, 0);
										}
										else
										{
											PS_translate(hl->xp, -hl->yp);
											if (hl->cscale != 100)
												PS_scale(hl->cscale / 100.0, 1);
											PS_show_xyG(hl->cfont, chx, 0, 0);
										}
										PS_restore();
									}
									if ((hl->cstyle & 4) && (chx != QChar(13)))
									{
										uint chr = chx[0].unicode();
										if ((*Doc->AllFonts)[hl->cfont]->CharWidth.contains(chr))
										{
											FPointArray gly = (*Doc->AllFonts)[hl->cfont]->GlyphArray[chr].Outlines.copy();
											QWMatrix chma;
											chma.scale(tsz / 100.0, tsz / 100.0);
											gly.map(chma);
											chma = QWMatrix();
											chma.scale(hl->cscale / 100.0, 1);
											gly.map(chma);
											if (ite->Reverse)
											{
												chma = QWMatrix();
												chma.scale(-1, 1);
												chma.translate(wideR, 0);
												gly.map(chma);
											}
											if (hl->cstroke != "None")
											{
												PS_save();
												PS_setlinewidth(QMAX((*Doc->AllFonts)[hl->cfont]->strokeWidth / 2 * (tsz / 10.0), 1));
												PS_setcapjoin(Qt::FlatCap, Qt::MiterJoin);
												PS_setdash(Qt::SolidLine, 0, dum);
												PS_translate(hl->xp, (hl->yp - tsz) * -1);
												SetFarbe(Doc, hl->cstroke, hl->cshade2, &h, &s, &v, &k, gcr);
												PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
												SetClipPath(&gly);
												PS_closepath();
												PS_stroke();
												PS_restore();
											}
										}
									}
									if ((hl->cstyle & 16) && (chx != QChar(13)))
									{
										double Ulen = Cwidth(Doc, hl->cfont, chx, hl->csize) * (hl->cscale / 100.0);
										double Upos = (*Doc->AllFonts)[hl->cfont]->strikeout_pos * (tsz / 10.0);
										if (hl->ccolor != "None")
										{
											PS_setcapjoin(Qt::FlatCap, Qt::MiterJoin);
											PS_setdash(Qt::SolidLine, 0, dum);
											SetFarbe(Doc, hl->ccolor, hl->cshade, &h, &s, &v, &k, gcr);
											PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
										}
										PS_setlinewidth((*Doc->AllFonts)[hl->cfont]->strokeWidth * (tsz / 10.0));
										PS_moveto(hl->xp, -hl->yp+Upos);
										PS_lineto(hl->xp+Ulen, -hl->yp+Upos);
										PS_stroke();
									}
									if ((hl->cstyle & 8) && (chx != QChar(13)))
									{
										double Ulen = Cwidth(Doc, hl->cfont, chx, hl->csize) * (hl->cscale / 100.0);
										double Upos = (*Doc->AllFonts)[hl->cfont]->underline_pos * (tsz / 10.0);
										if (hl->ccolor != "None")
										{
											PS_setcapjoin(Qt::FlatCap, Qt::MiterJoin);
											PS_setdash(Qt::SolidLine, 0, dum);
											SetFarbe(Doc, hl->ccolor, hl->cshade, &h, &s, &v, &k, gcr);
											PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
										}
										PS_setlinewidth((*Doc->AllFonts)[hl->cfont]->strokeWidth * (tsz / 10.0));
										PS_moveto(hl->xp, -hl->yp+Upos);
										PS_lineto(hl->xp+Ulen, -hl->yp+Upos);
										PS_stroke();
									}
									if (hl->cstyle & 512)
									{
										int chs = hl->csize;
										ite->SetZeichAttr(hl, &chs, &chx);
										double wide = Cwidth(Doc, hl->cfont, chx, chs);
										chx = "-";
										uint chr = chx[0].unicode();
										if ((*Doc->AllFonts)[hl->cfont]->CharWidth.contains(chr))
										{
											FPointArray gly = (*Doc->AllFonts)[hl->cfont]->GlyphArray[chr].Outlines.copy();
											QWMatrix chma;
											chma.scale(tsz / 100.0, tsz / 100.0);
											gly.map(chma);
											chma = QWMatrix();
											chma.scale(hl->cscale / 100.0, 1);
											gly.map(chma);
											if (hl->ccolor != "None")
											{
												PS_save();
												PS_newpath();
												PS_translate(hl->xp+wide, (hl->yp - (tsz / 10.0)) * -1);
												SetFarbe(Doc, hl->ccolor, hl->cshade, &h, &s, &v, &k, gcr);
												PS_setcmykcolor_fill(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
												SetClipPath(&gly);
												PS_closepath();
												PS_fill();
												PS_restore();
											}
										}
									}
								}
								if (((ite->Pcolor2 != "None") || (ite->NamedLStyle != "")) && (!ite->isTableItem))
								{
									if ((ite->NamedLStyle == "") && (ite->Pwidth != 0.0))
									{
										SetFarbe(Doc, ite->Pcolor2, ite->Shade2, &h, &s, &v, &k, gcr);
										PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
										PS_setlinewidth(ite->Pwidth);
										PS_setcapjoin(ite->PLineEnd, ite->PLineJoin);
										PS_setdash(ite->PLineArt, ite->DashOffset, ite->DashValues);
										SetClipPath(&ite->PoLine);
										PS_closepath();
										PS_stroke();
									}
									else
									{
										multiLine ml = Doc->MLineStyles[ite->NamedLStyle];
										for (int it = ml.size()-1; it > -1; it--)
										{
											SetFarbe(Doc, ml[it].Color, ml[it].Shade, &h, &s, &v, &k, gcr);
											PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
											PS_setlinewidth(ml[it].Width);
											PS_setcapjoin(static_cast<Qt::PenCapStyle>(ml[it].LineEnd), static_cast<Qt::PenJoinStyle>(ml[it].LineJoin));
											PS_setdash(static_cast<Qt::PenStyle>(ml[it].Dash), 0, dum);
											SetClipPath(&ite->PoLine);
											PS_closepath();
											PS_stroke();
										}
									}
								}
								PS_restore();
							}
						}
					}
					for (uint am = 0; am < Doc->Pages.at(a)->FromMaster.count(); ++am)
					{
						PageItem *ite = Doc->Pages.at(a)->FromMaster.at(am);
						if ((ite->PType == 2) && ((sep) || (!farb)))
							continue;
						if (ite->isPrintable)
						{
							PS_save();
							if (ite->Pcolor2 != "None")
							{
								SetFarbe(Doc, ite->Pcolor2, ite->Shade2, &h, &s, &v, &k, gcr);
								PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
							}
							PS_setlinewidth(ite->Pwidth);
							PS_setcapjoin(ite->PLineEnd, ite->PLineJoin);
							PS_setdash(ite->PLineArt, ite->DashOffset, ite->DashValues);
							PS_translate(ite->Xpos - mPage->Xoffset, Doc->PageH - (ite->Ypos - mPage->Yoffset));
							if (ite->Rot != 0)
								PS_rotate(-ite->Rot);
							if (ite->isTableItem)
							{
								if ((ite->TopLine) || (ite->RightLine) || (ite->BottomLine) || (ite->LeftLine))
								{
									if (ite->TopLine)
									{
										PS_moveto(0, 0);
										PS_lineto(ite->Width, 0);
									}
									if (ite->RightLine)
									{
										PS_moveto(ite->Width, 0);
										PS_lineto(ite->Width, -ite->Height);
									}
									if (ite->BottomLine)
									{
										PS_moveto(0, -ite->Height);
										PS_lineto(ite->Width, -ite->Height);
									}
									if (ite->LeftLine)
									{
										PS_moveto(0, 0);
										PS_lineto(0, -ite->Height);
									}
									PS_stroke();
								}
							}
							PS_restore();
						}
					}
					Lnr++;
				}
			}
		}
		ProcessPage(Doc, view, Doc->Pages.at(a), a+1, sep, farb, Ic, gcr);
		PS_end_page();
		if (sep)
		{
			if (SepNam != QObject::tr("All"))
				aa++;
			else
			{
				if (sepac == 3)
				{
					aa++;
					sepac = 0;
				}
				else
					sepac++;
			}
		}
		else
			aa++;
	}
	PS_close();
}

void PSLib::ProcessPage(ScribusDoc* Doc, ScribusView* view, Page* a, uint PNr, bool sep, bool farb, bool ic, bool gcr)
{
	uint b, d;
	int h, s, v, k, tsz;
	double wideR;
	QValueList<double> dum;
	dum.clear();
	QCString chxc;
	QString chx, chglyph, tmp;
	PageItem *c;
	QPtrList<PageItem> PItems;
	struct Pti *hl;
	int Lnr = 0;
	struct Layer ll;
	ll.Drucken = false;
	ll.LNr = 0;
	for (uint la = 0; la < Doc->Layers.count(); ++la)
	{
		Level2Layer(Doc, &ll, Lnr);
		if (a->PageNam != "")
			PItems = Doc->MasterItems;
		else
			PItems = Doc->Items;
		if (ll.Drucken)
		{
			for (b = 0; b < PItems.count(); ++b)
			{
				c = PItems.at(b);
				if (c->LayerNr != ll.LNr)
					continue;
				if ((a->PageNam != "") && (c->PType == 4))
					continue;
				if ((a->PageNam != "") && (c->PType == 2) && ((sep) || (!farb)))
					continue;
				if ((!Art) && (view->SelItem.count() != 0) && (!c->Select))
					continue;
				int x = static_cast<int>(a->Xoffset);
				int y = static_cast<int>(a->Yoffset);
				int w = static_cast<int>(a->Width);
				int h1 = static_cast<int>(a->Height);
				int x2 = static_cast<int>(c->BoundingX);
				int y2 = static_cast<int>(c->BoundingY);
				int w2 = static_cast<int>(c->BoundingW);
				int h2 = static_cast<int>(c->BoundingH);
				if (!QRect(x, y, w, h1).intersects(QRect(x2, y2, w2, h2)))
					continue;
				if (c->ChangedMasterItem)
					continue;
				if ((a->PageNam != "") && (c->OwnPage != static_cast<int>(a->PageNr)) && (c->OwnPage != -1))
					continue;
				if (c->isPrintable)
				{
					PS_save();
					if (c->Pcolor != "None")
					{
						SetFarbe(Doc, c->Pcolor, c->Shade, &h, &s, &v, &k, gcr);
						PS_setcmykcolor_fill(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
					}
					if (c->Pcolor2 != "None")
					{
						SetFarbe(Doc, c->Pcolor2, c->Shade2, &h, &s, &v, &k, gcr);
						PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
					}
					PS_setlinewidth(c->Pwidth);
					PS_setcapjoin(c->PLineEnd, c->PLineJoin);
					PS_setdash(c->PLineArt, c->DashOffset, c->DashValues);
					PS_translate(c->Xpos - a->Xoffset, Doc->PageH - (c->Ypos - a->Yoffset));
					if (c->Rot != 0)
						PS_rotate(-c->Rot);
					switch (c->PType)
					{
					case 2:
						if ((c->Pcolor != "None") || (c->GrType != 0))
						{
							SetClipPath(&c->PoLine);
							PS_closepath();
							if ((c->GrType != 0) && (a->PageNam == ""))
								HandleGradient(Doc, c, c->Width, c->Height, gcr);
							else
								PS_fill();
							PS_newpath();
						}
						PS_save();
						SetClipPath(&c->PoLine);
						PS_closepath();
						PS_clip(true);
						if ((c->flippedH % 2) != 0)
						{
							PS_translate(c->Width, 0);
							PS_scale(-1, 1);
						}
						if ((c->flippedV % 2) != 0)
						{
							PS_translate(0, -c->Height);
							PS_scale(1, -1);
						}
						if ((c->PicAvail) && (c->Pfile != ""))
						{
							PS_translate(0, -c->BBoxH*c->LocalScY);
							if (a->PageNam != "")
								PS_image(c->InvPict, -c->BBoxX+c->LocalX, -c->LocalY, c->Pfile, c->LocalScX, c->LocalScY, c->IProfile, c->UseEmbedded, ic, c->AnName);
							else
								PS_image(c->InvPict, -c->BBoxX+c->LocalX, -c->LocalY, c->Pfile, c->LocalScX, c->LocalScY, c->IProfile, c->UseEmbedded, ic);
						}
						PS_restore();
						if (((c->Pcolor2 != "None") || (c->NamedLStyle != "")) && (!c->isTableItem))
						{
							if ((c->NamedLStyle == "") && (c->Pwidth != 0.0))
							{
								SetFarbe(Doc, c->Pcolor2, c->Shade2, &h, &s, &v, &k, gcr);
								PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
								PS_setlinewidth(c->Pwidth);
								PS_setcapjoin(c->PLineEnd, c->PLineJoin);
								PS_setdash(c->PLineArt, c->DashOffset, c->DashValues);
								SetClipPath(&c->PoLine);
								PS_closepath();
								PS_stroke();
							}
							else
							{
								multiLine ml = Doc->MLineStyles[c->NamedLStyle];
								for (int it = ml.size()-1; it > -1; it--)
								{
									SetFarbe(Doc, ml[it].Color, ml[it].Shade, &h, &s, &v, &k, gcr);
									PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
									PS_setlinewidth(ml[it].Width);
									PS_setcapjoin(static_cast<Qt::PenCapStyle>(ml[it].LineEnd), static_cast<Qt::PenJoinStyle>(ml[it].LineJoin));
									PS_setdash(static_cast<Qt::PenStyle>(ml[it].Dash), 0, dum);
									SetClipPath(&c->PoLine);
									PS_closepath();
									PS_stroke();
								}
							}
						}
						break;
					case 4:
						if (c->isBookmark)
						{
							QString bm = "";
							QString cc;
							for (d = 0; d < c->Ptext.count(); ++d)
							{
								if ((c->Ptext.at(d)->ch == QChar(13)) || (c->Ptext.at(d)->ch == QChar(10)) || (c->Ptext.at(d)->ch == QChar(28)))
									break;
								bm += "\\"+cc.setNum(QMAX(c->Ptext.at(d)->ch.at(0).unicode(), 32), 8);
							}
							PDF_Bookmark(bm, a->PageNr+1);
						}
						if (c->isAnnotation)
						{
							QString bm = "";
							QString cc;
							for (d = 0; d < c->Ptext.count(); ++d)
							{
								bm += "\\"+cc.setNum(QMAX(c->Ptext.at(d)->ch.at(0).unicode(), 32), 8);
							}
							PDF_Annotation(bm, 0, 0, c->Width, -c->Height);
							break;
						}
						if ((c->Pcolor != "None") || (c->GrType != 0))
						{
							SetClipPath(&c->PoLine);
							PS_closepath();
							if ((c->GrType != 0) && (a->PageNam == ""))
								HandleGradient(Doc, c, c->Width, c->Height, gcr);
							else
								PS_fill();
						}
						if ((c->flippedH % 2) != 0)
						{
							PS_translate(c->Width, 0);
							PS_scale(-1, 1);
						}
						if ((c->flippedV % 2) != 0)
						{
							PS_translate(0, -c->Height);
							PS_scale(1, -1);
						}
						for (d = 0; d < c->MaxChars; ++d)
						{
							hl = c->Ptext.at(d);
							if ((hl->ch == QChar(13)) || (hl->ch == QChar(10)) || (hl->ch == QChar(9)) || (hl->ch == QChar(28)))
								continue;
							if (hl->cstyle & 256)
								continue;
							if (hl->yp == 0)
								continue;
							tsz = hl->csize;
							chx = hl->ch;
							if (hl->ch == QChar(29))
								chx = " ";
							if (hl->ch == QChar(30))
							{
								if (Doc->MasterP)
									chx = "#";
								else
								{
									uint zae = 0;
									uint za2 = d;
									do
									{
										if (za2 == 0)
											break;
										za2--;
									}
									while (c->Ptext.at(za2)->ch == QChar(30));
									if (c->Ptext.at(za2)->ch != QChar(30))
										za2++;
									while (c->Ptext.at(za2+zae)->ch == QChar(30))
									{
										zae++;
										if (za2+zae == c->MaxChars)
											break;
									}
									QString out="%1";
									QString out2;
									out2 = out.arg(PNr-1+Doc->FirstPnum, -zae);
									chx = out2.mid(d-za2, 1);
								}
							}
							if (hl->cstyle & 64)
							{
								if (chx.upper() != chx)
								{
									tsz = hl->csize * Doc->VKapit / 100;
									chx = chx.upper();
								}
							}
							if (hl->cstyle & 1)
								tsz = hl->csize * Doc->VHochSc / 100;
							if (hl->cstyle & 2)
								tsz = hl->csize * Doc->VHochSc / 100;
							/* Subset all TTF Fonts until the bug in the TTF-Embedding Code is fixed */
							QFileInfo fd = QFileInfo((*Doc->AllFonts)[hl->cfont]->Datei);
							QString fext = fd.extension(false).lower();
							if ((fext == "ttf") || ((*Doc->AllFonts)[hl->cfont]->isOTF) || ((*Doc->AllFonts)[hl->cfont]->Subset))
							{
								uint chr = chx[0].unicode();
								if (((*Doc->AllFonts)[hl->cfont]->CharWidth.contains(chr)) && (chr != 32))
								{
									PS_save();
									if (c->Reverse)
									{
										PS_translate(hl->xp, (hl->yp - (tsz / 10.0)) * -1);
										PS_scale(-1, 1);
										if (d < c->MaxChars-1)
										{
											QString ctx = c->Ptext.at(d+1)->ch;
											if (ctx == QChar(29))
												ctx = " ";
											wideR = -Cwidth(Doc, hl->cfont, chx, tsz, ctx) * (hl->cscale / 100.0);
										}
										else
											wideR = -Cwidth(Doc, hl->cfont, chx, tsz) * (hl->cscale / 100.0);
										PS_translate(wideR, 0);
									}
									else
										PS_translate(hl->xp, (hl->yp - (tsz / 10.0)) * -1);
									if (hl->cscale != 100)
										PS_scale(hl->cscale / 100.0, 1);
									if (hl->ccolor != "None")
									{
										SetFarbe(Doc, hl->ccolor, hl->cshade, &h, &s, &v, &k, gcr);
										PS_setcmykcolor_fill(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
										PS_showSub(chr, (*Doc->AllFonts)[hl->cfont]->RealName(), tsz / 10.0, false);
									}
									PS_restore();
								}
							}
							else
							{
								PS_selectfont(hl->cfont, tsz / 10.0);
								if (hl->ccolor != "None")
								{
									SetFarbe(Doc, hl->ccolor, hl->cshade, &h, &s, &v, &k, gcr);
									PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
								}
								PS_save();
								if (c->Reverse)
								{
									int chs = hl->csize;
									c->SetZeichAttr(hl, &chs, &chx);
									PS_translate(hl->xp, -hl->yp);
									PS_scale(-1, 1);
									if (d < c->MaxChars-1)
									{
										QString ctx = c->Ptext.at(d+1)->ch;
										if (ctx == QChar(29))
											ctx = " ";
										wideR = -Cwidth(Doc, hl->cfont, chx, chs, ctx) * (hl->cscale / 100.0);
										PS_translate(wideR, 0);
									}
									else
									{
										wideR = -Cwidth(Doc, hl->cfont, chx, chs) * (hl->cscale / 100.0);
										PS_translate(wideR, 0);
									}
									if (hl->cscale != 100)
										PS_scale(hl->cscale / 100.0, 1);
									PS_show_xyG(hl->cfont, chx, 0, 0);
								}
								else
								{
									PS_translate(hl->xp, -hl->yp);
									if (hl->cscale != 100)
										PS_scale(hl->cscale / 100.0, 1);
									PS_show_xyG(hl->cfont, chx, 0, 0);
								}
								PS_restore();
							}
							if ((hl->cstyle & 4) && (chx != QChar(13)))
							{
								uint chr = chx[0].unicode();
								if ((*Doc->AllFonts)[hl->cfont]->CharWidth.contains(chr))
								{
									FPointArray gly = (*Doc->AllFonts)[hl->cfont]->GlyphArray[chr].Outlines.copy();
									QWMatrix chma;
									chma.scale(tsz / 100.0, tsz / 100.0);
									gly.map(chma);
									chma = QWMatrix();
									chma.scale(hl->cscale / 100.0, 1);
									gly.map(chma);
									if (c->Reverse)
									{
										chma = QWMatrix();
										chma.scale(-1, 1);
										chma.translate(wideR, 0);
										gly.map(chma);
									}
									if (hl->cstroke != "None")
									{
										PS_save();
										PS_setlinewidth(QMAX((*Doc->AllFonts)[hl->cfont]->strokeWidth / 2 * (tsz / 10.0), 1));
										PS_setcapjoin(Qt::FlatCap, Qt::MiterJoin);
										PS_setdash(Qt::SolidLine, 0, dum);
										PS_translate(hl->xp, (hl->yp - (tsz / 10.0)) * -1);
										SetFarbe(Doc, hl->cstroke, hl->cshade2, &h, &s, &v, &k, gcr);
										PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
										SetClipPath(&gly);
										PS_closepath();
										PS_stroke();
										PS_restore();
									}
								}
							}
							if ((hl->cstyle & 16) && (chx != QChar(13)))
							{
								double Ulen = Cwidth(Doc, hl->cfont, chx, hl->csize) * (hl->cscale / 100.0);
								double Upos = (*Doc->AllFonts)[hl->cfont]->strikeout_pos * (tsz / 10.0);
								if (hl->ccolor != "None")
								{
									PS_setcapjoin(Qt::FlatCap, Qt::MiterJoin);
									PS_setdash(Qt::SolidLine, 0, dum);
									SetFarbe(Doc, hl->ccolor, hl->cshade, &h, &s, &v, &k, gcr);
									PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
								}
								PS_setlinewidth((*Doc->AllFonts)[hl->cfont]->strokeWidth * (tsz / 10.0));
								PS_moveto(hl->xp, -hl->yp+Upos);
								PS_lineto(hl->xp+Ulen, -hl->yp+Upos);
								PS_stroke();
							}
							if ((hl->cstyle & 8) && (chx != QChar(13)))
							{
								double Ulen = Cwidth(Doc, hl->cfont, chx, hl->csize) * (hl->cscale / 100.0);
								double Upos = (*Doc->AllFonts)[hl->cfont]->underline_pos * (tsz / 10.0);
								if (hl->ccolor != "None")
								{
									PS_setcapjoin(Qt::FlatCap, Qt::MiterJoin);
									PS_setdash(Qt::SolidLine, 0, dum);
									SetFarbe(Doc, hl->ccolor, hl->cshade, &h, &s, &v, &k, gcr);
									PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
								}
								PS_setlinewidth((*Doc->AllFonts)[hl->cfont]->strokeWidth * (tsz / 10.0));
								PS_moveto(hl->xp, -hl->yp+Upos);
								PS_lineto(hl->xp+Ulen, -hl->yp+Upos);
								PS_stroke();
							}
							if (hl->cstyle & 512)
							{
								int chs = hl->csize;
								c->SetZeichAttr(hl, &chs, &chx);
								double wide = Cwidth(Doc, hl->cfont, chx, chs);
								chx = "-";
								uint chr = chx[0].unicode();
								if ((*Doc->AllFonts)[hl->cfont]->CharWidth.contains(chr))
								{
									FPointArray gly = (*Doc->AllFonts)[hl->cfont]->GlyphArray[chr].Outlines.copy();
									QWMatrix chma;
									chma.scale(tsz / 100.0, tsz / 100.0);
									gly.map(chma);
									chma = QWMatrix();
									chma.scale(hl->cscale / 100.0, 1);
									gly.map(chma);
									if (hl->ccolor != "None")
									{
										PS_save();
										PS_newpath();
										PS_translate(hl->xp+wide, (hl->yp - (tsz / 10.0)) * -1);
										SetFarbe(Doc, hl->ccolor, hl->cshade, &h, &s, &v, &k, gcr);
										PS_setcmykcolor_fill(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
										SetClipPath(&gly);
										PS_closepath();
										PS_fill();
										PS_restore();
									}
								}
							}
						}
						if (((c->Pcolor2 != "None") || (c->NamedLStyle != "")) && (!c->isTableItem))
						{
							SetFarbe(Doc, c->Pcolor2, c->Shade2, &h, &s, &v, &k, gcr);
							PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
							PS_setlinewidth(c->Pwidth);
							PS_setcapjoin(c->PLineEnd, c->PLineJoin);
							PS_setdash(c->PLineArt, c->DashOffset, c->DashValues);
							if ((c->NamedLStyle == "") && (c->Pwidth != 0.0))
							{
								SetClipPath(&c->PoLine);
								PS_closepath();
								PS_stroke();
							}
							else
							{
								multiLine ml = Doc->MLineStyles[c->NamedLStyle];
								for (int it = ml.size()-1; it > -1; it--)
								{
									SetFarbe(Doc, ml[it].Color, ml[it].Shade, &h, &s, &v, &k, gcr);
									PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
									PS_setlinewidth(ml[it].Width);
									PS_setcapjoin(static_cast<Qt::PenCapStyle>(ml[it].LineEnd), static_cast<Qt::PenJoinStyle>(ml[it].LineJoin));
									PS_setdash(static_cast<Qt::PenStyle>(ml[it].Dash), 0, dum);
									SetClipPath(&c->PoLine);
									PS_closepath();
									PS_stroke();
								}
							}
						}
						break;
					case 5:
						if ((c->NamedLStyle == "") && (c->Pwidth != 0.0))
						{
							PS_moveto(0, 0);
							PS_lineto(c->Width, -c->Height);
							PS_stroke();
						}
						else
						{
							multiLine ml = Doc->MLineStyles[c->NamedLStyle];
							for (int it = ml.size()-1; it > -1; it--)
							{
								SetFarbe(Doc, ml[it].Color, ml[it].Shade, &h, &s, &v, &k, gcr);
								PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
								PS_setlinewidth(ml[it].Width);
								PS_setcapjoin(static_cast<Qt::PenCapStyle>(ml[it].LineEnd), static_cast<Qt::PenJoinStyle>(ml[it].LineJoin));
								PS_setdash(static_cast<Qt::PenStyle>(ml[it].Dash), 0, dum);
								PS_moveto(0, 0);
								PS_lineto(c->Width, -c->Height);
								PS_stroke();
							}
						}
						break;
					case 1:
					case 3:
					case 6:
						if ((c->Pcolor != "None") || (c->GrType != 0))
						{
							SetClipPath(&c->PoLine);
							PS_closepath();
							if (c->GrType != 0)
								HandleGradient(Doc, c, c->Width, c->Height, gcr);
							else
								PS_fill();
						}
						if ((c->Pcolor2 != "None") || (c->NamedLStyle != ""))
						{
							if ((c->NamedLStyle == "") && (c->Pwidth != 0.0))
							{
								SetClipPath(&c->PoLine);
								PS_closepath();
								PS_stroke();
							}
							else
							{
								multiLine ml = Doc->MLineStyles[c->NamedLStyle];
								for (int it = ml.size()-1; it > -1; it--)
								{
									SetFarbe(Doc, ml[it].Color, ml[it].Shade, &h, &s, &v, &k, gcr);
									PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
									PS_setlinewidth(ml[it].Width);
									PS_setcapjoin(static_cast<Qt::PenCapStyle>(ml[it].LineEnd), static_cast<Qt::PenJoinStyle>(ml[it].LineJoin));
									PS_setdash(static_cast<Qt::PenStyle>(ml[it].Dash), 0, dum);
									SetClipPath(&c->PoLine);
									PS_closepath();
									PS_stroke();
								}
							}
						}
						break;
					case 7:
						if ((c->Pcolor != "None") || (c->GrType != 0))
						{
							SetClipPath(&c->PoLine);
							PS_closepath();
							if (c->GrType != 0)
								HandleGradient(Doc, c, c->Width, c->Height, gcr);
							else
								PS_fill();
							PS_newpath();
						}
						if ((c->NamedLStyle == "") && (c->Pwidth != 0.0))
						{
							SetClipPath(&c->PoLine, false);
							PS_stroke();
						}
						else
						{
							multiLine ml = Doc->MLineStyles[c->NamedLStyle];
							for (int it = ml.size()-1; it > -1; it--)
							{
								SetFarbe(Doc, ml[it].Color, ml[it].Shade, &h, &s, &v, &k, gcr);
								PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
								PS_setlinewidth(ml[it].Width);
								PS_setcapjoin(static_cast<Qt::PenCapStyle>(ml[it].LineEnd), static_cast<Qt::PenJoinStyle>(ml[it].LineJoin));
								PS_setdash(static_cast<Qt::PenStyle>(ml[it].Dash), 0, dum);
								SetClipPath(&c->PoLine, false);
								PS_stroke();
							}
						}
						break;
					case 8:
						if (c->PoShow)
						{
							if (c->PoLine.size() > 3)
							{
								PS_save();
								if ((c->NamedLStyle == "") && (c->Pwidth != 0.0))
								{
									SetClipPath(&c->PoLine, false);
									PS_stroke();
								}
								else
								{
									multiLine ml = Doc->MLineStyles[c->NamedLStyle];
									for (int it = ml.size()-1; it > -1; it--)
									{
										SetFarbe(Doc, ml[it].Color, ml[it].Shade, &h, &s, &v, &k, gcr);
										PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
										PS_setlinewidth(ml[it].Width);
										PS_setcapjoin(static_cast<Qt::PenCapStyle>(ml[it].LineEnd), static_cast<Qt::PenJoinStyle>(ml[it].LineJoin));
										PS_setdash(static_cast<Qt::PenStyle>(ml[it].Dash), 0, dum);
										SetClipPath(&c->PoLine, false);
										PS_stroke();
									}
								}
								PS_restore();
							}
						}
						for (d = 0; d < c->MaxChars; ++d)
						{
							hl = c->Ptext.at(d);
							if ((hl->ch == QChar(13)) || (hl->ch == QChar(30)) || (hl->ch == QChar(9)) || (hl->ch == QChar(28)))
								continue;
							tsz = hl->csize;
							chx = hl->ch;
							if (hl->ch == QChar(29))
								chx = " ";
							if (hl->cstyle & 64)
							{
								if (chx.upper() != chx)
								{
									tsz = hl->csize * Doc->VKapit / 100;
									chx = chx.upper();
								}
							}
							if (hl->cstyle & 1)
								tsz = hl->csize * Doc->VHochSc / 100;
							if (hl->cstyle & 2)
								tsz = hl->csize * Doc->VHochSc / 100;
							if (hl->ccolor != "None")
							{
								SetFarbe(Doc, hl->ccolor, hl->cshade, &h, &s, &v, &k, gcr);
								PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
							}
							/* Subset all TTF Fonts until the bug in the TTF-Embedding Code is fixed */
							QFileInfo fd = QFileInfo((*Doc->AllFonts)[hl->cfont]->Datei);
							QString fext = fd.extension(false).lower();
							if ((fext == "ttf") || ((*Doc->AllFonts)[hl->cfont]->isOTF) || ((*Doc->AllFonts)[hl->cfont]->Subset))
							{
								uint chr = chx[0].unicode();
								if (((*Doc->AllFonts)[hl->cfont]->CharWidth.contains(chr)) && (chr != 32))
								{
									PS_save();
									PS_translate(hl->PtransX, -hl->PtransY);
									PS_rotate(-hl->PRot);
									if (c->Reverse)
									{
										PS_translate(hl->xp, (hl->yp - (tsz / 10.0)) * -1);
										PS_scale(-1, 1);
										if (d < c->MaxChars-1)
										{
											QString ctx = c->Ptext.at(d+1)->ch;
											if (ctx == QChar(29))
												ctx = " ";
											wideR = -Cwidth(Doc, hl->cfont, chx, tsz, ctx) * (hl->cscale / 100.0);
										}
										else
											wideR = -Cwidth(Doc, hl->cfont, chx, tsz) * (hl->cscale / 100.0);
										PS_translate(wideR, 0);
									}
									else
										PS_translate(hl->xp, (hl->yp - (tsz / 10.0)) * -1);
									if (hl->cscale != 100)
										PS_scale(hl->cscale / 100.0, 1);
									if (hl->ccolor != "None")
									{
										SetFarbe(Doc, hl->ccolor, hl->cshade, &h, &s, &v, &k, gcr);
										PS_setcmykcolor_fill(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
										PS_showSub(chr, (*Doc->AllFonts)[hl->cfont]->RealName(), tsz / 10.0, false);
									}
									PS_restore();
								}
							}
							else
							{
								PS_selectfont(hl->cfont, tsz / 10.0);
								PS_save();
								PS_translate(hl->PtransX, -hl->PtransY);
								PS_rotate(-hl->PRot);
								PS_show_xyG(hl->cfont, chx, hl->xp, -hl->yp);
								PS_restore();
							}
						}
						break;
					}
					PS_restore();
				}
			}
		}
		for (b = 0; b < PItems.count(); ++b)
		{
			c = PItems.at(b);
			if (c->LayerNr != ll.LNr)
				continue;
			if ((a->PageNam != "") && (c->PType == 4))
				continue;
			if ((a->PageNam != "") && (c->PType == 2) && ((sep) || (!farb)))
				continue;
			int x = static_cast<int>(a->Xoffset);
			int y = static_cast<int>(a->Yoffset);
			int w = static_cast<int>(a->Width);
			int h1 = static_cast<int>(a->Height);
			int x2 = static_cast<int>(c->BoundingX);
			int y2 = static_cast<int>(c->BoundingY);
			int w2 = static_cast<int>(c->BoundingW);
			int h2 = static_cast<int>(c->BoundingH);
			if (!QRect(x, y, w, h1).intersects(QRect(x2, y2, w2, h2)))
				continue;
			if (c->ChangedMasterItem)
				continue;
			if ((a->PageNam != "") && (c->OwnPage != static_cast<int>(a->PageNr)) && (c->OwnPage != -1))
				continue;
			if (c->isPrintable)
			{
				PS_save();
				if (c->Pcolor2 != "None")
				{
					SetFarbe(Doc, c->Pcolor2, c->Shade2, &h, &s, &v, &k, gcr);
					PS_setcmykcolor_stroke(h / 255.0, s / 255.0, v / 255.0, k / 255.0);
				}
				PS_setlinewidth(c->Pwidth);
				PS_setcapjoin(c->PLineEnd, c->PLineJoin);
				PS_setdash(c->PLineArt, c->DashOffset, c->DashValues);
				PS_translate(c->Xpos - a->Xoffset, Doc->PageH - (c->Ypos - a->Yoffset));
				if (c->Rot != 0)
					PS_rotate(-c->Rot);
				if (c->isTableItem)
				{
					if ((c->TopLine) || (c->RightLine) || (c->BottomLine) || (c->LeftLine))
					{
						if (c->TopLine)
						{
							PS_moveto(0, 0);
							PS_lineto(c->Width, 0);
						}
						if (c->RightLine)
						{
							PS_moveto(c->Width, 0);
							PS_lineto(c->Width, -c->Height);
						}
						if (c->BottomLine)
						{
							PS_moveto(0, -c->Height);
							PS_lineto(c->Width, -c->Height);
						}
						if (c->LeftLine)
						{
							PS_moveto(0, 0);
							PS_lineto(0, -c->Height);
						}
						PS_stroke();
					}
				}
				PS_restore();
			}
		}
		Lnr++;
	}
}

void PSLib::HandleGradient(ScribusDoc* Doc, PageItem *c, double w, double h, bool gcr)
{
	int ch,cs,cv,ck;
	double StartX, StartY, EndX, EndY;
	QPtrVector<VColorStop> cstops = c->fill_gradient.colorStops();
	switch (c->GrType)
	{
		case 1:
			StartX = 0;
			StartY = h / 2.0;
			EndX = w;
			EndY = h / 2.0;
			break;
		case 2:
			StartX = w / 2.0;
			StartY = 0;
			EndX = w / 2.0;
			EndY = h;
			break;
		case 3:
			StartX = 0;
			StartY = 0;
			EndX = w;
			EndY = h;
			break;
		case 4:
			StartX = 0;
			StartY = h;
			EndX = w;
			EndY = 0;
			break;
		case 5:
			StartX = w / 2.0;
			StartY = h / 2.0;
			if (w >= h)
			{
				EndX = w;
				EndY = h / 2.0;
			}
			else
			{
				EndX = w / 2.0;
				EndY = h;
			}
			break;
		case 6:
		case 7:
			StartX = c->GrStartX;
			StartY = c->GrStartY;
			EndX = c->GrEndX;
			EndY = c->GrEndY;
			break;
	}
	QValueList<double> StopVec;
	QStringList Gcolors;
	QString hs,ss,vs,ks;
	if ((c->GrType == 5) || (c->GrType == 7))
	{
		StopVec.clear();
		for (uint cst = 0; cst < c->fill_gradient.Stops(); ++cst)
		{
			StopVec.prepend(sqrt(pow(EndX - StartX, 2) + pow(EndY - StartY,2))*cstops.at(cst)->rampPoint);
			SetFarbe(Doc, cstops.at(cst)->name, cstops.at(cst)->shade, &ch, &cs, &cv, &ck, gcr);
			QString GCol = hs.setNum(ch / 255.0)+" "+ss.setNum(cs / 255.0)+" "+vs.setNum(cv / 255.0)+" "+ks.setNum(ck / 255.0);
			Gcolors.prepend(GCol);
		}
		PS_MultiRadGradient(w, -h, StartX, -StartY, StopVec, Gcolors);
	}
	else
	{
		StopVec.clear();
		for (uint cst = 0; cst < c->fill_gradient.Stops(); ++cst)
		{
			QWMatrix ma;
			ma.translate(StartX, StartY);
			ma.rotate(atan2(EndY - StartY, EndX - StartX)*(180.0/3.1415927));
			double w2 = sqrt(pow(EndX - StartX, 2) + pow(EndY - StartY,2))*cstops.at(cst)->rampPoint;
			double x = fabs(ma.m11() * w2 + ma.dx());
			double y = fabs(ma.m12() * w2 + ma.dy());
			StopVec.append(x);
			StopVec.append(-y);
			SetFarbe(Doc, cstops.at(cst)->name, cstops.at(cst)->shade, &ch, &cs, &cv, &ck, gcr);
			QString GCol = hs.setNum(ch / 255.0)+" "+ss.setNum(cs / 255.0)+" "+vs.setNum(cv / 255.0)+" "+ks.setNum(ck / 255.0);
			Gcolors.append(GCol);
		}
		PS_MultiLinGradient(w, -h, StopVec, Gcolors);
	}
}

void PSLib::SetFarbe(ScribusDoc* Doc, QString farb, int shade, int *h, int *s, int *v, int *k, bool gcr)
{
	int h1, s1, v1, k1;
	h1 = *h;
	s1 = *s;
	v1 = *v;
	k1 = *k;
	CMYKColor tmp = Doc->PageColors[farb];
	if (gcr)
		tmp.applyGCR();
	tmp.getCMYK(&h1, &s1, &v1, &k1);
	*h = h1 * shade / 100;
	*s = s1 * shade / 100;
	*v = v1 * shade / 100;
	*k = k1 * shade / 100;
}

void PSLib::SetClipPath(FPointArray *c, bool poly)
{
	FPoint np, np1, np2;
	bool nPath = true;
	if (c->size() > 3)
	{
		for (uint poi=0; poi<c->size()-3; poi += 4)
		{
			if (c->point(poi).x() > 900000)
			{
				if (poly)
					PS_closepath();
				nPath = true;
				continue;
			}
			if (nPath)
			{
				np = c->point(poi);
				PS_moveto(np.x(), -np.y());
				nPath = false;
			}
			np = c->point(poi+1);
			np1 = c->point(poi+3);
			np2 = c->point(poi+2);
			PS_curve(np.x(), -np.y(), np1.x(), -np1.y(), np2.x(), -np2.y());
		}
	}
}
