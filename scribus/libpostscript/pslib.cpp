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
#include "config.h"
#include "pslib.h"
#include "scfonts.h"
#include <cmath>

extern bool loadText(QString nam, QString *Buffer);
extern QImage LoadPict(QString fn);
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
	int IsoInUse = 0;
  QString Epfad = PREL;
	Seiten = 0;
	User = "";
	Creator = "PSlib-0.3";
	Titel = "";
	FillColor = "0.0 0.0 0.0 0.0";
	StrokeColor = "0.0 0.0 0.0 0.0";
  if (psart)
  	{
  	Header = "%!PS-Adobe-3.0\n";
  	}
  else
  	{
  	Header = "%!PS-Adobe-3.0 EPSF-3.0\n";
  	}
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
		if ((itf.key() != "Cyan") && (itf.key() != "Magenta") && (itf.key() != "Yellow") && (itf.key() != "Black"))
			{
			DocColors[itf.key()].getCMYK(&c, &m, &y, &k);
			if (!erst)
				{
				Farben += "%%+ ";
				FNamen += "%%+ ";
				}
			Farben += ToStr(float(c) / 255) + " " +  ToStr(float(m) / 255) + " ";
			Farben += ToStr(float(y) / 255) + " " +  ToStr(float(k) / 255) + " (" + itf.key() + ")\n";
			FNamen += "(" + itf.key() + ")\n";
			erst = false;
			}
		}
	QMap<QString,QFont>::Iterator it;
	int a = 0;
	for (it = DocFonts.begin(); it != DocFonts.end(); ++it)
		{
		UsedFonts.insert(it.key(), "/Fo"+IToStr(a));
		CHset = AllFonts[it.key()]->FontEnc;
		if ((CHset == "iso8859-1") || (CHset == "ascii-0") || (CHset == "iso8859-15"))
			Fonts += "/"+AllFonts[it.key()]->RealName()+" /Fo"+IToStr(a)+" cf ef\n";
		if (CHset == "iso8859-2")
			{
			Fonts += "/"+AllFonts[it.key()]->RealName()+" /Fo"+IToStr(a)+" iso2f ef\n";
			IsoInUse = 2;
			}
		if (CHset == "iso8859-3")
			{
			Fonts += "/"+AllFonts[it.key()]->RealName()+" /Fo"+IToStr(a)+" iso3f ef\n";
			IsoInUse = 3;
			}
		if (CHset == "iso8859-4")
			{
			Fonts += "/"+AllFonts[it.key()]->RealName()+" /Fo"+IToStr(a)+" iso4f ef\n";
			IsoInUse = 4;
			}
		if (CHset == "iso8859-5")
			{
			Fonts += "/"+AllFonts[it.key()]->RealName()+" /Fo"+IToStr(a)+" iso5f ef\n";
			IsoInUse = 5;
			}
		if (CHset == "iso8859-7")
			{
			Fonts += "/"+AllFonts[it.key()]->RealName()+" /Fo"+IToStr(a)+" iso7f ef\n";
			IsoInUse = 7;
			}
		if (CHset == "iso8859-9")
			{
			Fonts += "/"+AllFonts[it.key()]->RealName()+" /Fo"+IToStr(a)+" iso9f ef\n";
			IsoInUse = 9;
			}
		if (CHset == "iso8859-10")
			{
			Fonts += "/"+AllFonts[it.key()]->RealName()+" /Fo"+IToStr(a)+" iso10f ef\n";
			IsoInUse = 10;
			}
		if (CHset == "iso8859-13")
			{
			Fonts += "/"+AllFonts[it.key()]->RealName()+" /Fo"+IToStr(a)+" iso13f ef\n";
			IsoInUse = 13;
			}
		if (CHset.startsWith("koi8"))
			{
			Fonts += "/"+AllFonts[it.key()]->RealName()+" /Fo"+IToStr(a)+" koi8f ef\n";
			IsoInUse = 99;
			}
		if (CHset == "adobe-fontspecific")
			Fonts += "/Fo"+IToStr(a)+" /"+AllFonts[it.key()]->RealName()+" findfont definefont pop\n";
		if (AllFonts[it.key()]->EmbedPS)
			{
			QString tmp;
			if(AllFonts[it.key()]->EmbedFont(tmp))
				{
				GListe gl;
				AllFonts[it.key()]->GlNames(&gl);
				GlyphsOfFont.insert(it.key(), gl);
				FontDesc += "%%BeginFont: " + AllFonts[it.key()]->RealName() + "\n";
				FontDesc += tmp + "\n%%EndFont\n";
				}
			}
		a++;
		}
	Prolog = "%%BeginProlog\n";
	Prolog += "/Scribusdict 100 dict def\n";
	Prolog += "Scribusdict begin\n";
	Prolog += "/sp {showpage} bind def\n";
	Prolog += "/oldsetgray /setgray load def\n";
	Prolog += "/cmyk {setcmykcolor} def\n";
	Prolog += "/m {moveto} bind def\n";
	Prolog += "/li {lineto} bind def\n";
	Prolog += "/cl {closepath} bind def\n";
	Prolog += "/gs {gsave} bind def\n";
	Prolog += "/gr {grestore} bind def\n";
	Prolog += "/tr {translate} bind def\n";
	Prolog += "/ro {rotate} bind def\n";
	Prolog += "/sh {show} bind def\n";
	Prolog += "/ftt {currentfont /FontType get 42 eq { 1000 } { 1 } ifelse div} bind def\n";
	Prolog += "/shg {setcmykcolor moveto glyphshow} def\n";
	Prolog += "/shm {setcmykcolor moveto\n";
	Prolog += "      dup (\\55) eq\n";
	Prolog += "        {pop /hyphen glyphshow}\n";
	Prolog += "        {show}\n";
	Prolog += "      ifelse} def\n";
	Prolog += "/sc {scale} bind def\n";
	Prolog += "/se {selectfont} bind def\n";
	Prolog += "/sf {setfont} bind def\n";
	Prolog += "/sw {setlinewidth} bind def\n";
	Prolog += "/srt {2 setlinecap currentfont dup /FontMatrix get\n";
	Prolog += "      exch /FontInfo get dup /UnderlinePosition get ftt exch\n";
	Prolog += "      /UnderlineThickness get ftt 3 -1 roll dtransform\n";
	Prolog += "      setlinewidth 0 exch rmoveto stringwidth rlineto stroke} bind def\n";
	Prolog += "/srd {2 setlinecap currentfont dup /FontMatrix get\n";
	Prolog += "      exch /FontInfo get dup /UnderlinePosition get ftt exch\n";
	Prolog += "      /UnderlineThickness get ftt 3 -1 roll dtransform\n";
	Prolog += "      setlinewidth pop stringwidth rlineto stroke} bind def\n";
	Prolog += "/f  {findfont} bind def\n";
	Prolog += "/mf {makefont} bind def\n";
	Prolog += "/ss {scalefont setfont} bind def\n";
	Prolog += "/ci {newpath 0 0 1 0 360 arc} bind def\n";
	Prolog += "/fi {fill} bind def\n";
	Prolog += "/st {stroke} bind def\n";
	QString isoenc;
	if (IsoInUse != 0)
		{
		switch (IsoInUse)
			{
			case 2:
				Prolog += "/iso2f [\n";
				loadText(Epfad+"/lib/scribus/iso8859-2ps.enc", &isoenc);
				Prolog += isoenc;
				Prolog += "] def\n";
				break;
			case 3:
				Prolog += "/iso3f [\n";
				loadText(Epfad+"/lib/scribus/iso8859-3ps.enc", &isoenc);
				Prolog += isoenc;
				Prolog += "] def\n";
				break;
			case 4:
				Prolog += "/iso4f [\n";
				loadText(Epfad+"/lib/scribus/iso8859-4ps.enc", &isoenc);
				Prolog += isoenc;
				Prolog += "] def\n";
				break;
			case 5:
				Prolog += "/iso5f [\n";
				loadText(Epfad+"/lib/scribus/iso8859-5ps.enc", &isoenc);
				Prolog += isoenc;
				Prolog += "] def\n";
				break;
			case 7:
				Prolog += "/iso7f [\n";
				loadText(Epfad+"/lib/scribus/iso8859-7ps.enc", &isoenc);
				Prolog += isoenc;
				Prolog += "] def\n";
				break;
			case 9:
				Prolog += "/iso9f [\n";
				loadText(Epfad+"/lib/scribus/iso8859-9ps.enc", &isoenc);
				Prolog += isoenc;
				Prolog += "] def\n";
				break;
			case 10:
				Prolog += "/iso10f [\n";
				loadText(Epfad+"/lib/scribus/iso8859-10ps.enc", &isoenc);
				Prolog += isoenc;
				Prolog += "] def\n";
				break;
			case 13:
				Prolog += "/iso13f [\n";
				loadText(Epfad+"/lib/scribus/iso8859-13ps.enc", &isoenc);
				Prolog += isoenc;
				Prolog += "] def\n";
				break;
			case 99:
				Prolog += "/koi8f [\n";
				loadText(Epfad+"/lib/scribus/koi8ps.enc", &isoenc);
				Prolog += isoenc;
				Prolog += "] def\n";
				break;
			}
		}
	Prolog += "/cf [\n";
	Prolog += "    16#2d /hyphen\n";
	Prolog += "    16#80 /Euro\n";
	Prolog += "    16#82 /quotesinglbase\n";
	Prolog += "    16#83 /florin\n";
	Prolog += "    16#84 /quotedblbase\n";
	Prolog += "    16#85 /ellipsis\n";
	Prolog += "    16#86 /dagger\n";
	Prolog += "    16#87 /daggerdbl\n";
	Prolog += "    16#88 /circumflex\n";
	Prolog += "    16#89 /perthousand\n";
	Prolog += "    16#8a /Scaron\n";
	Prolog += "    16#8b /guilsinglleft\n";
	Prolog += "    16#8c /OE\n";
	Prolog += "    16#8e /zcaron\n";
	Prolog += "    16#91 /quoteleft\n";
	Prolog += "    16#92 /quoteright\n";
	Prolog += "    16#93 /quotedblleft\n";
	Prolog += "    16#94 /quotedblright\n";
	Prolog += "    16#95 /bullet\n";
	Prolog += "    16#96 /endash\n";
	Prolog += "    16#97 /emdash\n";
	Prolog += "    16#98 /tilde\n";
	Prolog += "    16#99 /trademark\n";
	Prolog += "    16#9a /scaron\n";
	Prolog += "    16#9b /guilsinglright\n";
	Prolog += "    16#9c /oe\n";
	Prolog += "    16#9e /zcaron\n";
	Prolog += "    16#9f /Ydieresis\n";
	Prolog += "] def\n";
	Prolog += "/ski /ISOLatin1Encoding where{pop true}{false}ifelse def\n";
	Prolog += "/reencodesmalldict 12 dict def\n";
	Prolog += "/ef {\n";
	Prolog += "    reencodesmalldict begin\n";
	Prolog += "    /newcodesandnames exch def\n";
	Prolog += "    /newfontname exch def\n";
	Prolog += "    /basefontname exch def\n";
	Prolog += "    /basefontdict basefontname findfont def\n";
	Prolog += "    /newfont basefontdict maxlength dict def\n";
	Prolog += "    basefontdict\n";
	Prolog += "    {\n";
	Prolog += "        exch dup /FID ne\n";
	Prolog += "        {\n";
	Prolog += "            dup /Encoding eq\n";
	Prolog += "            {\n";
	Prolog += "                ski\n";
	Prolog += "                {\n";
	Prolog += "                    exch pop\n";
	Prolog += "                    ISOLatin1Encoding dup length array copy\n";
	Prolog += "                }{\n";
	Prolog += "                exch dup length array copy\n";
	Prolog += "            }\n";
	Prolog += "            ifelse\n";
	Prolog += "            newfont 3 1 roll put\n";
	Prolog += "            }{\n";
	Prolog += "                exch newfont 3 1 roll put\n";
	Prolog += "            }\n";
	Prolog += "            ifelse\n";
	Prolog += "        }{\n";
	Prolog += "        pop pop\n";
	Prolog += "    }\n";
	Prolog += "    ifelse\n";
	Prolog += "    } forall\n";
	Prolog += "    newfont /FontName newfontname put\n";
	Prolog += "    newcodesandnames aload pop\n";
	Prolog += "    newcodesandnames length 2 idiv\n";
	Prolog += "    {\n";
	Prolog += "        newfont /Encoding get 3 1 roll put\n";
	Prolog += "    } repeat\n";
	Prolog += "    newfontname newfont definefont pop\n";
	Prolog += "    end\n";
	Prolog += "    } def\n";
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
PSLib::~PSLib()
{
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

QString PSLib::ToStr(float c)
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

void PSLib::PS_begin_doc(int Ori, float breite, float hoehe, int numpage)
{
	PutDoc(Header);
	PutDoc("%%For: " + User + "\n");
	PutDoc("%%Title: " + Titel + "\n");
	PutDoc("%%Creator: " + Creator + "\n");
	PutDoc("%%Pages: " + IToStr(numpage) + "\n");
	BBox = "%%BoundingBox: 0 0 " + IToStr(qRound(breite)) + " " + IToStr(qRound(hoehe)) + "\n";
	BBoxH = "%%HiResBoundingBox: 0 0 " + ToStr(breite) + " " + ToStr(hoehe) + "\n";
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
		{
		PutDoc(FontDesc);
		}
	PutDoc("Scribusdict begin\n");
	PutDoc(Fonts);
	if (GraySc)
		PutDoc(GrayCalc);
	PutDoc("%%EndSetup\n");
  if ((Art) && (Ori != 0))
  	{
		PutSeite("<< /PageSize [ "+ToStr(breite)+" "+ToStr(hoehe)+" ]\n");
//		if (Ori != 0)
		PutSeite("/Orientation 3\n");
		PutSeite(">> setpagedevice\n");
		}
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

void PSLib::PS_TemplateStart(QString Name, float breite, float hoehe)
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

void PSLib::PS_begin_page(float breite, float hoehe, struct Margs* Ma)
{
	PDev = ToStr(Ma->Left) + " " + ToStr(Ma->Bottom) + " m\n";
	PDev += ToStr(breite - Ma->Right) + " " + ToStr(Ma->Bottom) + " li\n";
	PDev += ToStr(breite - Ma->Right) + " " + ToStr(hoehe - Ma->Top) + " li\n";
	PDev += ToStr(Ma->Left) + " " + ToStr(hoehe - Ma->Top) + " li cl clip newpath\n";
	Seiten++;
	PutSeite("%%Page: " + IToStr(Seiten) + " " + IToStr(Seiten) + "\nsave\n" + PDev);
  PutSeite("/DeviceCMYK setcolorspace\n");
}

void PSLib::PS_end_page()
{
	PutSeite("%%PageTrailer\nrestore\nsp\n");
}

void PSLib::PS_curve(float x1, float y1, float x2, float y2, float x3, float y3)
{
	PutSeite(ToStr(x1) + " " + ToStr(y1) + " " + ToStr(x2) + " " + ToStr(y2) + " " + ToStr(x3) + " " + ToStr(y3) + " curveto\n");
}

void PSLib::PS_moveto(float x, float y)
{
	PutSeite(ToStr(x) + " " + ToStr(y) + " m\n");
}

void PSLib::PS_lineto(float x, float y)
{
	PutSeite(ToStr(x) + " " + ToStr(y) + " li\n");
}

void PSLib::PS_closepath()
{
	PutSeite("cl\n");
}

void PSLib::PS_translate(float x, float y)
{
	PutSeite(ToStr(x) + " " + ToStr(y) + " tr\n");
}

void PSLib::PS_scale(float x, float y)
{
	PutSeite(ToStr(x) + " " + ToStr(y) + " sc\n");
}

void PSLib::PS_rotate(float x)
{
	PutSeite(ToStr(x) + " ro\n");
}

void PSLib::PS_clip(bool mu)
{
	if (mu)
		PutSeite("eoclip newpath\n");
	else
		PutSeite("clip newpath\n");
}

void PSLib::PS_save()
{
	PutSeite("gs\n");
}

void PSLib::PS_restore()
{
	PutSeite("gr\n");
}

void PSLib::PS_setcmykcolor_fill(float c, float m, float y, float k)
{
	FillColor = ToStr(c) + " " + ToStr(m) + " " + ToStr(y) + " " + ToStr(k);
}

void PSLib::PS_setcmykcolor_dummy()
{
	PutSeite("0 0 0 0 cmyk\n");
}

void PSLib::PS_setcmykcolor_stroke(float c, float m, float y, float k)
{
	StrokeColor = ToStr(c) + " " + ToStr(m) + " " + ToStr(y) + " " + ToStr(k);
}

void PSLib::PS_setlinewidth(float w)
{
	PutSeite(ToStr(w) + " sw\n");
	LineW = w;
}

void PSLib::PS_setdash(Qt::PenStyle st, Qt::PenCapStyle ca, Qt::PenJoinStyle jo)
{
	QString Dt = ToStr(QMAX(LineW, 1));
	QString Da = ToStr(QMAX(3*LineW, 1));
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

void PSLib::PS_selectfont(QString f, int s)
{
	PutSeite(UsedFonts[f] + " " + IToStr(s) + " se\n");
}

void PSLib::PS_fill(bool mu)
{
	if (mu)
		PutSeite(FillColor + " cmyk eofill\n");
	else
		PutSeite(FillColor + " cmyk fi\n");
}

void PSLib::PS_stroke()
{
	PutSeite(StrokeColor + " cmyk st\n");
}

void PSLib::PS_fill_stroke(bool mu)
{
	PS_save();
	PS_fill(mu);
	PS_restore();
	PS_stroke();
}

void PSLib::PS_newpath()
{
	PutSeite("newpath\n");
}

void PSLib::PS_GradientCol1(float c, float m, float y, float k)
{
	GrColor1 = ToStr(c) + " " + ToStr(m) + " " + ToStr(y) + " " + ToStr(k);
}

void PSLib::PS_GradientCol2(float c, float m, float y, float k)
{
	GrColor2 = ToStr(c) + " " + ToStr(m) + " " + ToStr(y) + " " + ToStr(k);
}

void PSLib::PS_LinGradient(float w, float h, int item, int grad, bool mu)
{
	if (item == 1)
		PutSeite("-"+ToStr(w / 2.0)+" "+ToStr(-h / 2.0)+" tr\n");
	if (mu)
		PutSeite("eoclip\n");
	else
		PutSeite("clip\n");
	PutSeite("<<\n");
	PutSeite("/ShadingType 2\n");
	if (DoSep)
		PutSeite("/ColorSpace /DeviceGray\n");
	else
		PutSeite("/ColorSpace /DeviceCMYK\n");
	switch (grad)
		{
		case 1:
			PutSeite("/Coords [0 "+ToStr(h / 2.0)+" "+ToStr(w)+" "+ToStr(h / 2.0)+"]\n");
			break;
		case 2:
			PutSeite("/Coords ["+ToStr(w / 2.0)+" 0 "+ToStr(w / 2.0)+" "+ToStr(h)+"]\n");
			break;
		case 3:
			PutSeite("/Coords [0 0 "+ToStr(w)+" "+ToStr(h)+"]\n");
			break;
		case 4:
			PutSeite("/Coords ["+ToStr(w)+" 0 0 "+ToStr(h)+"]\n");
			break;
		}
	PutSeite("/BBox [0 "+ToStr(h)+" "+ToStr(w)+" 0]\n");
	if (DoSep)
		PutSeite("/Background [0]\n");
	else
		PutSeite("/Background ["+FillColor+"]\n");
	PutSeite("/Extend [false false]\n");
	PutSeite("/Function\n");
	PutSeite("<<\n");
	PutSeite("/FunctionType 2\n");
	PutSeite("/Domain [0 1]\n");
	if (DoSep)
		{
		int pla = Plate - 1;
		if (pla < 0)
			pla = 3;
		QStringList cols1 = QStringList::split(" ", GrColor2);
		QStringList cols2 = QStringList::split(" ", GrColor1);
		PutSeite("/C1 ["+ToStr(1-cols1[pla].toFloat())+"]\n");
		PutSeite("/C0 ["+ToStr(1-cols2[pla].toFloat())+"]\n");
		}
	else
		{
		PutSeite("/C0 ["+GrColor1+"]\n");
		PutSeite("/C1 ["+GrColor2+"]\n");
		}
	PutSeite("/N 1\n");
	PutSeite(">>\n");
	PutSeite(">>\n");
	PutSeite("shfill\n");
}

void PSLib::PS_RadGradient(float w, float h, int item, bool mu)
{
	float w2, h2, rad;
	w2 = w / 2.0;
	h2 = h / 2.0;
	rad = QMIN(w, fabs(h)) / 2.0;
	PutSeite("/cmtx matrix currentmatrix def\n");
	if (mu)
		PutSeite("eoclip\n");
	else
		PutSeite("clip\n");
	PutSeite("<<\n");
	PutSeite("/ShadingType 3\n");
	if (DoSep)
		PutSeite("/ColorSpace /DeviceGray\n");
	else
		PutSeite("/ColorSpace /DeviceCMYK\n");
	PutSeite("/Coords ["+ToStr(w2)+" "+ToStr(h2)+" 0 "+ToStr(w2)+" "+ToStr(h2)+" "+ToStr(rad)+"]\n");
	PutSeite("/BBox [0 "+ToStr(h)+" "+ToStr(w)+" 0]\n");
	if (DoSep)
		PutSeite("/Background [0]\n");
	else
		PutSeite("/Background ["+FillColor+"]\n");
	PutSeite("/Extend [true true]\n");
	PutSeite("/Function\n");
	PutSeite("<<\n");
	PutSeite("/FunctionType 2\n");
	PutSeite("/Domain [0 1]\n");
	if (DoSep)
		{
		int pla = Plate - 1;
		if (pla < 0)
			pla = 3;
		QStringList cols1 = QStringList::split(" ", GrColor2);
		QStringList cols2 = QStringList::split(" ", GrColor1);
		PutSeite("/C0 ["+ToStr(1-cols1[pla].toFloat())+"]\n");
		PutSeite("/C1 ["+ToStr(1-cols2[pla].toFloat())+"]\n");
		}
	else
		{
		PutSeite("/C0 ["+GrColor2+"]\n");
		PutSeite("/C1 ["+GrColor1+"]\n");
		}
	PutSeite("/N 1\n");
	PutSeite(">>\n");
	PutSeite(">>\n");
	if (item == 1)
		{
		if (w > fabs(h))
			PutSeite("-"+ToStr(w2*(w2 / rad))+" "+ToStr(fabs(h2))+" tr\n");
		else
			PutSeite("-"+ToStr(w2*(w2 / rad))+" "+ToStr(fabs(h2)*(fabs(h2) /rad))+" tr\n");
		}
	else
		{
		if (w > fabs(h))
			PutSeite("-"+ToStr(w2*(w2 / rad)-w2)+" 0 tr\n");
		else
			{
			if (w < fabs(h))
				PutSeite("0 "+ToStr(fabs(h2)*(fabs(h2) /rad)-fabs(h2))+" tr\n");
			}
		}
	PutSeite(ToStr(w2 / rad)+" "+ToStr(fabs(h2) / rad)+" scale\n");
	PutSeite("shfill\n");
	PutSeite("cmtx setmatrix\n");
}

void PSLib::PS_underline(QCString ch, float x, float y)
{
	PS_moveto(x, y);
	PutSeite("("+QString(ch)+") srt\n");
}

void PSLib::PS_strikeout(QCString ch, float x, float y)
{
	PS_moveto(x, y);
	PutSeite("("+QString(ch)+") srd\n");
}

void PSLib::PS_show_xy(QCString ch, float x, float y)
{
	PutSeite("("+QString(ch)+") "+ToStr(x)+" "+ToStr(y)+" "+StrokeColor+" shm\n");
}

void PSLib::PS_show_xyG(QString font, QString ch, float x, float y)
{
	QString Name;
	uint cc = ch[0].unicode();
	if (GlyphsOfFont[font].contains(cc))
	  Name = GlyphsOfFont[font][cc];
	else
		Name = ".notdef";
	PutSeite("/"+Name+" "+ToStr(x)+" "+ToStr(y)+" "+StrokeColor+" shg\n");
}

void PSLib::PS_show(float x, float y)
{
	PS_moveto(x, y);
	PutSeite("/hyphen glyphshow\n");
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

void PSLib::PS_image(bool inver, float x, float y, QString fn, float scalex, float scaley, QString Prof, bool UseEmbedded, bool UseProf, QString Name)
{
	QString tmp;
	QFileInfo fi = QFileInfo(fn);
	QString ext = fi.extension(false).lower();
	if (ext == "eps")
		{
		if (loadText(fn, &tmp))
			{
			if (Name != "")
				{
      	PutSeite("bEPS\n");
      	PutSeite(ToStr(scalex) + " " + ToStr(scaley) + " sc\n");
      	PutSeite(ToStr(x) + " " + ToStr(y) + " tr\n");
				PutSeite(PSEncode(Name)+"Bild cvx exec\n");
				PutSeite(PSEncode(Name)+"Bild resetfile\n");
      	PutSeite("eEPS\n");
				}
			else
				{
      	PutSeite("bEPS\n");
      	PutSeite(ToStr(scalex) + " " + ToStr(scaley) + " sc\n");
      	PutSeite(ToStr(x) + " " + ToStr(y) + " tr\n");
      	PutSeite("%%BeginDocument: " + fi.fileName() + "\n");
      	PutSeite(tmp+"\n");
      	PutSeite("%%EndDocument\n");
      	PutSeite("eEPS\n");
				}
			}
		}
	else
		{
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
			}
		else
			{
			image = LoadPict(fn);
			image = image.convertDepth(32);
			}
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
    if ((!DoSep) && (!GraySc))
			PutSeite("/DeviceCMYK setcolorspace\n");
		else
			PutSeite("/DeviceGray setcolorspace\n");
    if (image.hasAlphaBuffer())
    	{
			QImage iMask = image.createAlphaMask();
#ifdef HAVE_CMS
			if ((CMSuse) && (UseProf))
				{
				if (DoSep)
					ImgStr = ImageToCMYK_PS(&image2, Plate, cmy);
				else
					{
					if (GraySc)
						ImgStr = ImageToCMYK_PS(&image2, -2, cmy);
					else
						ImgStr = ImageToCMYK_PS(&image2, -1, cmy);
					}
				}
			else
				{
				if (DoSep)
					ImgStr = ImageToCMYK_PS(&image, Plate, false);
				else
					{
					if (GraySc)
						ImgStr = ImageToCMYK_PS(&image, -2, false);
					else
						ImgStr = ImageToCMYK_PS(&image, -1, false);
					}
				}
#else
			if (DoSep)
				ImgStr = ImageToCMYK_PS(&image, Plate, false);
			else
				{
				if (GraySc)
					ImgStr = ImageToCMYK_PS(&image, -2, false);
				else
					ImgStr = ImageToCMYK_PS(&image, -1, false);
				}
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
			PutSeite("   /ImageMatrix [" + IToStr(w) + " 0 0 " + IToStr(-h) + " 0 " + IToStr(h) +"]\n");
			if (DoSep)
				PutSeite("   /Decode [1 0]\n");
			else
				{
				if (GraySc)
					PutSeite("   /Decode [1 0]\n");
				else
					PutSeite("   /Decode [0 1 0 1 0 1 0 1]\n");
				}
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
				{
				if (GraySc)
					PutSeite("   /Decode [1 0]\n");
				else
					PutSeite("   /Decode [0 1 0 1 0 1 0 1]\n");
				}
			PutSeite("   /ImageMatrix [" + IToStr(w) + " 0 0 " + IToStr(-h) + " 0 " + IToStr(h) + "]\n");
			if (Name != "")
				{
				PutSeite("   /DataSource "+PSEncode(Name)+"Bild >>\n");
				PutSeite("image\n");
				PutSeite(PSEncode(Name)+"Bild resetfile\n");
				}
			else
				{
				if (CompAvail)
					PutSeite("   /DataSource currentfile /ASCIIHexDecode filter /FlateDecode filter >>\n");
				else
					PutSeite("   /DataSource currentfile /ASCIIHexDecode filter >>\n");
				PutSeite("image\n");
#ifdef HAVE_CMS
				if ((CMSuse) && (UseProf))
					{
					if (DoSep)
						ImgStr = ImageToCMYK_PS(&image2, Plate, cmy);
					else
						{
						if (GraySc)
							ImgStr = ImageToCMYK_PS(&image2, -2, cmy);
						else
							ImgStr = ImageToCMYK_PS(&image2, -1, cmy);
						}
					}
				else
					{
					if (DoSep)
						ImgStr = ImageToCMYK_PS(&image, Plate, false);
					else
						{
						if (GraySc)
							ImgStr = ImageToCMYK_PS(&image, -2, false);
						else
							ImgStr = ImageToCMYK_PS(&image, -1, false);
						}
					}
#else
				if (DoSep)
					ImgStr = ImageToCMYK_PS(&image, Plate, false);
				else
					{
					if (GraySc)
						ImgStr = ImageToCMYK_PS(&image, -2, false);
					else
						ImgStr = ImageToCMYK_PS(&image, -1, false);
					}
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

void PSLib::PDF_Annotation(QString text, float x, float y, float b, float h)
{
	PutSeite("[ /Rect [ "+ToStr(static_cast<int>(x))+" "+ToStr(static_cast<int>(y))+" "+ToStr(static_cast<int>(b))+" "+ToStr(static_cast<int>(h))+" ]\n");
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
