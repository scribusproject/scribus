/***************************************************************************
                          util.cpp  -  description
                             -------------------
    begin                : Fri Sep 14 2001
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

#include <qstring.h>
#include <qpixmap.h>
#include <qbitmap.h>
#include <qpainter.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qtextstream.h>
#include <qdatastream.h>
#include <qstringlist.h>
#include <qmap.h>
#include <qdom.h>
#include <qimage.h>
#include <qdir.h>
#include <qpointarray.h>
#include <qmessagebox.h>
#include <cstdlib>
#include <cmath>
#include <unistd.h>
#include "md5.h"
#include <setjmp.h>
#include "qprocess.h"
#include "scpaths.h"

#ifdef _MSC_VER
 #if (_MSC_VER >= 1200)
  #include "win-config.h"
 #endif
#else
 #include "config.h"
#endif

extern "C"
{
#define XMD_H           // shut JPEGlib up
#if defined(Q_OS_UNIXWARE)
#  define HAVE_BOOLEAN  // libjpeg under Unixware seems to need this
#endif
#include <jpeglib.h>
#include <jerror.h>
#undef HAVE_STDLIB_H
#ifdef const
#  undef const          // remove crazy C hackery in jconfig.h
#endif
}

#include "scribus.h"
#include "libpdf/pdflib.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#include FT_GLYPH_H
#ifdef HAVE_LIBZ
	#include <zlib.h>
#endif
#ifdef HAVE_TIFF
	#include <tiffio.h>
#endif
#ifdef HAVE_CMS
	#include CMS_INC
extern cmsHPROFILE CMSoutputProf;
extern cmsHPROFILE CMSprinterProf;
extern cmsHTRANSFORM stdTransG;
extern cmsHTRANSFORM stdProofG;
extern cmsHTRANSFORM stdTransImgG;
extern cmsHTRANSFORM stdProofImgG;
extern bool BlackPoint;
extern bool SoftProofing;
extern bool Gamut;
extern bool CMSuse;
extern int IntentMonitor;
extern int IntentPrinter;
#endif
extern ProfilesL InputProfiles;
extern ScribusApp* ScApp;

using namespace std;


QColor SetColor(ScribusDoc *currentDoc, QString color, int shad);
void GetItemProps(bool newVersion, QDomElement *obj, struct CopyPasteBuffer *OB);
QStringList sortQStringList(QStringList aList);
void ReOrderText(ScribusDoc *currentDoc, ScribusView *view);
void WordAndPara(PageItem *currItem, int *w, int *p, int *c, int *wN, int *pN, int *cN);
void CopyPageItem(struct CopyPasteBuffer *Buffer, PageItem *currItem);
bool overwrite(QWidget *parent, QString filename);
int setBestEncoding(FT_Face face);
FPointArray traceChar(FT_Face face, uint chr, int chs, double *x, double *y, bool &err);
FPoint getMaxClipF(FPointArray* Clip);
FPoint getMinClipF(FPointArray* Clip);
QPixmap FontSample(QString da, int s, QString ts, QColor back, bool force = false);
QPixmap fontSamples(QString da, int s, QString ts, QColor back);
QString Path2Relative(QString Path);
QPixmap LoadPDF(QString fn, int Page, int Size, int *w, int *h);
bool GlyNames(QMap<uint, QString> *GList, QString Dat);
bool GlyIndex(QMap<uint, PDFlib::GlNamInd> *GListInd, QString Dat);
QByteArray ComputeMD5Sum(QByteArray *in);
char *toHex( uchar u );
QString String2Hex(QString *in, bool lang = true);
QString CompressStr(QString *in);
void Level2Layer(ScribusDoc *currentDoc, struct Layer *ll, int Level);
void BezierPoints(QPointArray *ar, QPoint n1, QPoint n2, QPoint n3, QPoint n4);
double xy2Deg(double x, double y);
QPointArray FlattenPath(FPointArray ina, QValueList<uint> &Segs);
QPointArray RegularPolygon(double w, double h, uint c, bool star, double factor, double rota);
FPointArray RegularPolygonF(double w, double h, uint c, bool star, double factor, double rota);
QPixmap loadIcon(QString nam);
uint getDouble(QString in, bool raw);
bool loadText(QString nam, QString *Buffer);
double Cwidth(ScribusDoc *currentDoc, Foi* name, QString ch, int Siz, QString ch2 = " ");
double RealCWidth(ScribusDoc *currentDoc, Foi* name, QString ch, int Siz);
double QStodouble(QString in);
int QStoInt(QString in);
QString GetAttr(QDomElement *el, QString at, QString def="0");
#ifdef HAVE_CMS
QImage ProofPict(QImage *Im, QString Prof, int Rend, cmsHPROFILE emPr=0);
#else
QImage ProofPict(QImage *Im, QString Prof, int Rend);
#endif
QImage ProofImage(QImage *Im);
int System(const QStringList & args);
int callGS(const QStringList & args_in);
int copyFile(QString source, QString target);
int moveFile(QString source, QString target);

#ifdef HAVE_CMS
QImage ProofPict(QImage *Im, QString Prof, int Rend, cmsHPROFILE emPr)
#else
QImage ProofPict(QImage *Im, QString Prof, int Rend)
#endif
{
	bool emp = false;
	if (Prof == "")
		return Im->copy();
#ifdef HAVE_CMS
	QImage out = Im->copy();
	if ((CMSuse) && (SoftProofing))
	{
		cmsHTRANSFORM xform;
		cmsHPROFILE inputProf;
		if (emPr != 0)
			inputProf = emPr;
		else
		{
			inputProf = cmsOpenProfileFromFile(InputProfiles[Prof], "r");
			emp = true;
		}
		int dcmsFlags = 0;
		dcmsFlags |= Gamut ? cmsFLAGS_GAMUTCHECK : cmsFLAGS_SOFTPROOFING;
		xform = cmsCreateProofingTransform(inputProf, TYPE_RGBA_8,
		                                   CMSoutputProf, TYPE_RGBA_8,
		                                   CMSprinterProf,
		                                   IntentPrinter,
		                                   Rend, dcmsFlags);
		for (int i=0; i < out.height(); ++i)
		{
			LPBYTE ptr = out.scanLine(i);
			cmsDoTransform(xform, ptr, ptr, out.width());
		}
		cmsDeleteTransform(xform);
		if (emp)
			cmsCloseProfile(inputProf);
	}
	else
	{
		if (CMSuse)
		{
			cmsHTRANSFORM xform;
			cmsHPROFILE inputProf;
			if (emPr != 0)
				inputProf = emPr;
			else
			{
				inputProf = cmsOpenProfileFromFile(InputProfiles[Prof], "r");
				emp = true;
			}
			xform = cmsCreateTransform(inputProf, TYPE_RGBA_8,
			                           CMSoutputProf, TYPE_RGBA_8,
			                           Rend,
			                           0);
			for (int i=0; i < out.height(); ++i)
			{
				LPBYTE ptr = out.scanLine(i);
				cmsDoTransform(xform, ptr, ptr, out.width());
			}
			cmsDeleteTransform(xform);
			if (emp)
				cmsCloseProfile(inputProf);
		}
	}
	return out;
#else
	return Im->copy();
#endif
}

QImage ProofImage(QImage *Image)
{
#ifdef HAVE_CMS
	QImage out = Image->copy();
	if ((CMSuse) && (SoftProofing))
	{
		for (int i=0; i < out.height(); ++i)
		{
			LPBYTE ptr = out.scanLine(i);
			cmsDoTransform(stdProofImgG, ptr, ptr, out.width());
		}
	}
	else
	{
		if (CMSuse)
		{
			for (int i=0; i < out.height(); ++i)
			{
				LPBYTE ptr = out.scanLine(i);
				cmsDoTransform(stdTransImgG, ptr, ptr, out.width());
			}
		}
	}
	return out;
#else
	return Image->copy();
#endif
}

/******************************************************************
 * Function System()
 *  
 * Create a new process via QProcess and wait until finished.
 * return the process exit code.
 *
 ******************************************************************/

int System(const QStringList & args)
{
	QProcess *proc = new QProcess(NULL);
	proc->setArguments(args);
	if ( !proc->start() )
	{
		delete proc;
		return 1;
	}
	/* start was OK */
	/* wait a little bit */
	while( proc->isRunning() )
		usleep(5000);

	int ex = proc->exitStatus();
	delete proc;
	return ex;
}

/******************************************************************
 * Function callGS()
 *   build the complete list of arguments for the call of our
 *   System() function.
 *
 *   The gs commands are all similar and consist of a few constant
 *   arguments, the variablke arguments and the end arguments which
 *   are also invariant.
 ******************************************************************/

int callGS(const QStringList & args_in)
{
	QString cmd1 = ScApp->Prefs.gs_exe;
	cmd1 += " -q -dNOPAUSE";
	if (ScApp->HavePngAlpha != 0)
		cmd1 += " -sDEVICE=png16m";
	else
		cmd1 += " -sDEVICE=pngalpha";
	if (ScApp->Prefs.gs_AntiAliasText)
		cmd1 += " -dTextAlphaBits=4";
	if (ScApp->Prefs.gs_AntiAliasGraphics)
		cmd1 += " -dGraphicsAlphaBits=4";
	QString extArgs = args_in.join(" ");
	cmd1 += " " + extArgs + " -c showpage -c quit";
	return system(cmd1);
}

int copyFile(QString source, QString target)
{
	if ((source.isNull()) || (target.isNull()))
		return -1;
	if (source == target)
		return -1;
	QFile s(source);
	QFile t(target);
	if (!s.exists())
		return -1;
	QByteArray bb(s.size());
	if (s.open(IO_ReadOnly))
	{
		s.readBlock(bb.data(), s.size());
		s.close();
		if (t.open(IO_WriteOnly))
		{
			t.writeBlock(bb.data(), bb.size());
			t.close();
		}
	}
	return 0;
}

int moveFile(QString source, QString target)
{
	if ((source.isNull()) || (target.isNull()))
		return -1;
	if (source == target)
		return -1;
	copyFile(source, target);
	unlink(source);
	return 0;
}

QPixmap LoadPDF(QString fn, int Page, int Size, int *w, int *h)
{
	QString tmp, cmd1, cmd2;
	QString tmpFile = QDir::convertSeparators(QDir::homeDirPath()+"/.scribus/sc.png");
	QPixmap pm;
	int ret = -1;
	tmp.setNum(Page);
	QStringList args;
	args.append("-r72");
	args.append("-sOutputFile="+tmpFile);
	args.append("-dFirstPage="+tmp);
	args.append("-dLastPage="+tmp);
	args.append("\""+fn+"\"");
	ret = callGS(args);
	if (ret == 0)
	{
		QImage image;
		image.load(tmpFile);
		unlink(tmpFile);
		QImage im2;
		*h = image.height();
		*w = image.width();
		double sx = image.width() / static_cast<double>(Size);
		double sy = image.height() / static_cast<double>(Size);
		double t = (sy < sx ? sx : sy);
		im2 = image.smoothScale(static_cast<int>(image.width() / t), static_cast<int>(image.height() / t));
		pm.convertFromImage(im2);
		QPainter p;
		p.begin(&pm);
		p.setBrush(Qt::NoBrush);
		p.setPen(Qt::black);
		p.drawRect(0, 0, pm.width(), pm.height());
		p.end();
		im2.detach();
	}
	return pm;
}

QString GetAttr(QDomElement *el, QString at, QString def)
{
	return el->attribute(at, def);
}

int QStoInt(QString in)
{
	bool ok = false;
	int c = in.toInt(&ok);
	return ok ? c : 0;
}

double QStodouble(QString in)
{
	bool ok = false;
	double c = in.toDouble(&ok);
	return ok ? c : 0.0;
}

QPixmap loadIcon(QString nam)
{
	QString iconFilePath = QString("%1/%2").arg(ScPaths::instance().iconDir()).arg(nam);
	QPixmap pm;
	if (!QFile::exists(iconFilePath))
		qWarning("Unable to load icon %s: File not found", iconFilePath.ascii());
	else
	{
		pm.load(iconFilePath);
		if (pm.isNull())
			qWarning("Unable to load icon %s: Got null pixmap", iconFilePath.ascii());
	}
	return pm;
}

uint getDouble(QString in, bool raw)
{
	QByteArray bb(4);
	if (raw)
	{
		bb[3] = static_cast<uchar>(QChar(in.at(0)));
		bb[2] = static_cast<uchar>(QChar(in.at(1)));
		bb[1] = static_cast<uchar>(QChar(in.at(2)));
		bb[0] = static_cast<uchar>(QChar(in.at(3)));
	}
	else
	{
		bb[0] = static_cast<uchar>(QChar(in.at(0)));
		bb[1] = static_cast<uchar>(QChar(in.at(1)));
		bb[2] = static_cast<uchar>(QChar(in.at(2)));
		bb[3] = static_cast<uchar>(QChar(in.at(3)));
	}
	uint ret;
	ret = bb[0] & 0xff;
	ret |= (bb[1] << 8) & 0xff00;
	ret |= (bb[2] << 16) & 0xff0000;
	ret |= (bb[3] << 24) & 0xff000000;
	return ret;
}

bool loadText(QString filename, QString *Buffer)
{
	QFile f(filename);
	QFileInfo fi(f);
	if (!fi.exists())
		return false;
	bool ret;
	QByteArray bb(f.size());
	if (f.open(IO_ReadOnly))
	{
		f.readBlock(bb.data(), f.size());
		f.close();
		for (uint posi = 0; posi < bb.size(); ++posi)
			*Buffer += bb[posi];
		ret = true;
	}
	else
		ret = false;
	return ret;
}

double Cwidth(ScribusDoc *currentDoc, Foi* name, QString ch, int Size, QString ch2)
{
	double width;
	FT_Vector  delta;
	FT_Face      face;
	uint c1 = ch.at(0).unicode();
	uint c2 = ch2.at(0).unicode();
	double size10=Size/10.0;
	if (name->CharWidth.contains(c1))
	{
		width = name->CharWidth[c1]*size10;
		if (name->HasKern)
		{
			face = currentDoc->FFonts[name->SCName];
			uint cl = FT_Get_Char_Index(face, c1);
			uint cr = FT_Get_Char_Index(face, c2);
			FT_Get_Kerning(face, cl, cr, ft_kerning_unscaled, &delta);
			width += delta.x / name->uniEM * size10;
		}
		return width;
	}
	else
		return size10;
}

double RealCWidth(ScribusDoc *currentDoc, Foi* name, QString ch, int Size)
{
	double w, ww;
	uint c1 = ch.at(0).unicode();
	FT_Face      face;
	if (name->CharWidth.contains(c1))
	{
		face = currentDoc->FFonts[name->SCName];
		uint cl = FT_Get_Char_Index(face, c1);
		FT_Load_Glyph(face, cl, FT_LOAD_NO_SCALE | FT_LOAD_NO_BITMAP );
		w = (face->glyph->metrics.width + fabs((double)face->glyph->metrics.horiBearingX)) / name->uniEM * (Size / 10.0);
		ww = face->glyph->metrics.horiAdvance / name->uniEM * (Size / 10.0);
		return QMAX(ww, w);
	}
	else
		return static_cast<double>(Size / 10.0);
}

QPointArray RegularPolygon(double w, double h, uint c, bool star, double factor, double rota)
{
	uint cx = star ? c * 2 : c;
	double seg = 360.0 / cx;
	double sc = rota + 180.0;
	double di = factor;
	int mx = 0;
	int my = 0;
	QPointArray pts = QPointArray();
	for (uint x = 0; x < cx; ++x)
	{
		sc = seg * x + 180.0 + rota;
		if (star)
		{
			double wf = x % 2 == 0 ? w / 2 : w / 2 * di;
			double hf = x % 2 == 0 ? h / 2 : h / 2 * di;
			mx = qRound(sin(sc / 180 * M_PI) * (wf) + (w/2));
			my = qRound(cos(sc / 180 * M_PI) * (hf) + (h/2));
		}
		else
		{
			mx = qRound(sin(sc / 180 * M_PI) * (w/2) + (w/2));
			my = qRound(cos(sc / 180 * M_PI) * (h/2) + (h/2));
		}
		pts.resize(x+1);
		pts.setPoint(x, mx, my);
	}
	return pts;
}

FPointArray RegularPolygonF(double w, double h, uint c, bool star, double factor, double rota)
{
	uint cx = star ? c * 2 : c;
	double seg = 360.0 / cx;
	double sc = rota + 180.0;
	double di = factor;
	double mx = 0;
	double my = 0;
	FPointArray pts;
	for (uint x = 0; x < cx; ++x)
	{
		sc = seg * x + 180.0 + rota;
		if (star)
		{
			double wf = x % 2 == 0 ? w / 2 : w / 2 * di;
			double hf = x % 2 == 0 ? h / 2 : h / 2 * di;
			mx = qRound(sin(sc / 180 * M_PI) * (wf) + (w/2));
			my = qRound(cos(sc / 180 * M_PI) * (hf) + (h/2));
		}
		else
		{
			mx = sin(sc / 180 * M_PI) * (w/2) + (w/2);
			my = cos(sc / 180 * M_PI) * (h/2) + (h/2);
		}
		pts.resize(x+1);
		pts.setPoint(x, mx, my);
	}
	return pts;
}

QPointArray FlattenPath(FPointArray ina, QValueList<uint> &Segs)
{
	QPointArray Bez(4);
	QPointArray outa, cli;
	Segs.clear();
	if (ina.size() > 3)
	{
		for (uint poi=0; poi<ina.size()-3; poi += 4)
		{
			if (ina.point(poi).x() > 900000)
			{
				outa.resize(outa.size()+1);
				outa.setPoint(outa.size()-1, cli.point(cli.size()-1));
				Segs.append(outa.size());
				continue;
			}
			BezierPoints(&Bez, ina.pointQ(poi), ina.pointQ(poi+1), ina.pointQ(poi+3), ina.pointQ(poi+2));
			cli = Bez.cubicBezier();
			outa.putPoints(outa.size(), cli.size()-1, cli);
		}
		outa.resize(outa.size()+1);
		outa.setPoint(outa.size()-1, cli.point(cli.size()-1));
	}
	return outa;
}

double xy2Deg(double x, double y)
{
	return (atan2(y,x)*(180.0/M_PI));
}

void BezierPoints(QPointArray *ar, QPoint n1, QPoint n2, QPoint n3, QPoint n4)
{
	ar->setPoint(0, n1);
	ar->setPoint(1, n2);
	ar->setPoint(2, n3);
	ar->setPoint(3, n4);
	return;
}

void Level2Layer(ScribusDoc *currentDoc, struct Layer *ll, int Level)
{
	for (uint la2 = 0; la2 < currentDoc->Layers.count(); ++la2)
	{
		if (currentDoc->Layers[la2].Level == Level)
		{
			ll->isViewable = currentDoc->Layers[la2].isViewable;
			ll->isPrintable = currentDoc->Layers[la2].isPrintable;
			ll->LNr = currentDoc->Layers[la2].LNr;
			ll->Name = currentDoc->Layers[la2].Name;
			break;
		}
	}
}

QString CompressStr(QString *in)
{
	QString out = "";
#ifdef HAVE_LIBZ
	QByteArray bb(in->length());
	for (uint ax = 0; ax < in->length(); ++ax)
		bb[ax] = uchar(QChar(in->at(ax)));
	uLong exlen = uint(bb.size() * 0.001 + 16) + bb.size();
	QByteArray bc(exlen);
	compress2((Byte *)bc.data(), &exlen, (Byte *)bb.data(), uLong(bb.size()), 9);
	for (uint cl = 0; cl < exlen; ++cl)
		out += bc[cl];
#else
	out = *in;
#endif
	return out;
}

char *toHex( uchar u )
{
	static char hexVal[3];
	int i = 1;
	while ( i >= 0 )
	{
		ushort hex = (u & 0x000f);
		if ( hex < 0x0a )
			hexVal[i] = '0'+hex;
		else
			hexVal[i] = 'A'+(hex-0x0a);
		u = u >> 4;
		i--;
	}
	hexVal[2] = '\0';
	return hexVal;
}

QString String2Hex(QString *in, bool lang)
{
	int i = 0;
	QString out = "";
	for( uint xi = 0; xi < in->length(); ++xi )
	{
		out += toHex(uchar(QChar(in->at(xi))));
		++i;
		if ((i>40) && (lang))
		{
			out += '\n';
			i=0;
		}
	}
	return out;
}

QByteArray ComputeMD5Sum(QByteArray *in)
{
	QByteArray MDsum(16);
	md5_buffer (in->data(), in->size(), reinterpret_cast<void*>(MDsum.data()));
	return MDsum;
}

QString Path2Relative(QString Path)
{
	QString	Ndir = "";
	QStringList Pdir = QStringList::split("/", QDir::currentDirPath());
	QFileInfo Bfi = QFileInfo(Path);
	QStringList Bdir = QStringList::split("/", Bfi.dirPath(true));
	bool end = true;
	uint dcoun = 0;
	uint dcoun2 = 0;
	while (end)
	{
		if (Pdir[dcoun] == Bdir[dcoun])
			dcoun++;
		else
			break;
		if (dcoun > Pdir.count())
			break;
	}
	dcoun2 = dcoun;
	for (uint ddx2 = dcoun; ddx2 < Pdir.count(); ddx2++)
		Ndir += "../";
	for (uint ddx = dcoun2; ddx < Bdir.count(); ddx++)
		Ndir += Bdir[ddx]+"/";
	Ndir += Bfi.fileName();
	return Ndir;
}

int setBestEncoding(FT_Face face)
{
	FT_ULong  charcode;
	FT_UInt   gindex;
	bool foundEncoding = false;
	int countUniCode = 0;
	int chmapUniCode = 0;
	int chmapCustom = 0;
	int retVal = 0;
	FT_CharMap defaultEncoding = face->charmap;
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
		}
		if (face->charmaps[u]->encoding == FT_ENCODING_ADOBE_CUSTOM)
		{
			chmapCustom = u;
			foundEncoding = true;
			retVal = 1;
			break;
		}
		else if (face->charmaps[u]->encoding == FT_ENCODING_MS_SYMBOL)
		{
			chmapCustom = u;
			foundEncoding = true;
			retVal = 2;
			break;
		}
	}
	if (countUniCode >= face->num_glyphs-1)
	{
		FT_Set_Charmap(face, face->charmaps[chmapUniCode]);
		retVal = 0;
	}
	else if (foundEncoding)
		FT_Set_Charmap(face, face->charmaps[chmapCustom]);
	else
	{
		FT_Set_Charmap(face, defaultEncoding);
		retVal = 0;
	}
	return retVal;
}

bool GlyNames(QMap<uint, QString> *GList, QString Dat)
{
	bool error;
	char *buf[50];
	FT_Library library;
	FT_Face face;
	FT_ULong  charcode;
	FT_UInt gindex;
	error = FT_Init_FreeType(&library);
	error = FT_New_Face(library, Dat, 0, &face);
	setBestEncoding(face);
	gindex = 0;
	charcode = FT_Get_First_Char(face, &gindex );
	while (gindex != 0)
	{
		FT_Get_Glyph_Name(face, gindex, buf, 50);
		GList->insert(charcode, QString(reinterpret_cast<char*>(buf)));
		charcode = FT_Get_Next_Char(face, charcode, &gindex );
	}
	FT_Done_FreeType( library );
	return true;
}

bool GlyIndex(QMap<uint, PDFlib::GlNamInd> *GListInd, QString Dat)
{
	struct PDFlib::GlNamInd gln;
	bool error;
	char *buf[50];
	FT_Library library;
	FT_Face face;
	FT_ULong  charcode;
	FT_UInt   gindex;
	uint counter1 = 32;
	uint counter2 = 0;
	error = FT_Init_FreeType(&library);
	error = FT_New_Face(library, Dat, 0, &face);
	setBestEncoding(face);
	gindex = 0;
	charcode = FT_Get_First_Char(face, &gindex );
	while (gindex != 0)
	{
		FT_Get_Glyph_Name(face, gindex, buf, 50);
		gln.Code = counter1 + counter2;
		gln.Name = "/"+QString(reinterpret_cast<char*>(buf));
		GListInd->insert(charcode, gln);
		charcode = FT_Get_Next_Char(face, charcode, &gindex );
		counter1++;
		if (counter1 > 255)
		{
			counter1 = 32;
			counter2 += 0x100;
		}
	}
	FT_Done_FreeType( library );
	return true;
}

FPoint firstP;
bool FirstM;

int traceMoveto( FT_Vector *to, FPointArray *composite )
{
	double tox = ( to->x / 64.0 );
	double toy = ( to->y / 64.0 );
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

int traceLineto( FT_Vector *to, FPointArray *composite )
{
	double tox = ( to->x / 64.0 );
	double toy = ( to->y / 64.0 );
	if ( !composite->hasLastQuadPoint(tox, toy, tox, toy, tox, toy, tox, toy))
		composite->addQuadPoint(tox, toy, tox, toy, tox, toy, tox, toy);
	return 0;
}

int traceQuadraticBezier( FT_Vector *control, FT_Vector *to, FPointArray *composite )
{
	double x1 = ( control->x / 64.0 );
	double y1 = ( control->y / 64.0 );
	double x2 = ( to->x / 64.0 );
	double y2 = ( to->y / 64.0 );
	if ( !composite->hasLastQuadPoint(x2, y2, x1, y1, x2, y2, x2, y2))
		composite->addQuadPoint(x2, y2, x1, y1, x2, y2, x2, y2);
	return 0;
}

int traceCubicBezier( FT_Vector *p, FT_Vector *q, FT_Vector *to, FPointArray *composite )
{
	double x1 = ( p->x / 64.0 );
	double y1 = ( p->y / 64.0 );
	double x2 = ( q->x / 64.0 );
	double y2 = ( q->y / 64.0 );
	double x3 = ( to->x / 64.0 );
	double y3 = ( to->y / 64.0 );
	if ( !composite->hasLastQuadPoint(x3, y3, x2, y2, x3, y3, x3, y3) )
	{
		composite->setPoint(composite->size()-1, FPoint(x1, y1));
		composite->addQuadPoint(x3, y3, x2, y2, x3, y3, x3, y3);
	}
	return 0;
}

FT_Outline_Funcs OutlineMethods =
    {
        (FT_Outline_MoveTo_Func) traceMoveto,
        (FT_Outline_LineTo_Func) traceLineto,
        (FT_Outline_ConicTo_Func) traceQuadraticBezier,
        (FT_Outline_CubicTo_Func) traceCubicBezier,
        0,
        0
    };

FPointArray traceChar(FT_Face face, uint chr, int chs, double *x, double *y, bool *err)
{
	bool error = false;
	FT_UInt glyphIndex;
	//AV: not threadsave, but tracechar is only used in ReadMetrics() and fontSample()
	static FPointArray pts; 
	FPointArray pts2;
	pts.resize(0);
	pts2.resize(0);
	firstP = FPoint(0,0);
	FirstM = true;
	error = FT_Set_Char_Size( face, 0, chs*64, 72, 72 );
	if (error)
	{
		*err = error;
		return pts2;
	}
	glyphIndex = FT_Get_Char_Index(face, chr);
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
	*x = face->glyph->metrics.horiBearingX / 64.0;
	*y = face->glyph->metrics.horiBearingY / 64.0;
	QWMatrix ma;
	ma.scale(1, -1);
	pts.map(ma);
	pts.translate(0, chs);
	pts2.putPoints(0, pts.size()-2, pts, 0);

	return pts2;
}

FPoint getMaxClipF(FPointArray* Clip)
{
	FPoint np, rp;
	double mx = 0;
	double my = 0;
	for (uint c = 0; c < Clip->size(); ++c)
	{
		np = Clip->point(c);
		if (np.x() > 900000)
			continue;
		if (np.x() > mx)
			mx = np.x();
		if (np.y() > my)
			my = np.y();
	}
	rp.setXY(mx, my);
	return rp;
}

FPoint getMinClipF(FPointArray* Clip)
{
	FPoint np, rp;
	double mx = 99999;
	double my = 99999;
	for (uint c = 0; c < Clip->size(); ++c)
	{
		np = Clip->point(c);
		if (np.x() > 900000)
			continue;
		if (np.x() < mx)
			mx = np.x();
		if (np.y() < my)
			my = np.y();
	}
	rp.setXY(mx, my);
	return rp;
}

QPixmap FontSample(QString da, int s, QString ts, QColor back, bool force)
{
	FT_Face face;
	FT_Library library;
	double x, y, ymax;
	bool error;
	int  pen_x;
	FPoint gp;
	error = FT_Init_FreeType( &library );
	error = FT_New_Face( library, da, 0, &face );
	int encode = setBestEncoding(face);
	double uniEM = static_cast<double>(face->units_per_EM);
	int h = qRound(face->height / uniEM) * s + 1;
	double a = static_cast<double>(face->descender) / uniEM * s + 1;
	int w = qRound((face->bbox.xMax - face->bbox.xMin) / uniEM) * s * (ts.length()+1);
	if (w < 1)
		w = s * (ts.length()+1);
	if (h < 1)
		h = s;
	QPixmap pm(w, h);
	pm.fill();
	pen_x = 0;
	ymax = 0.0;
	ScPainter *p = new ScPainter(&pm, pm.width(), pm.height());
	p->setFillMode(1);
	p->setLineWidth(0.0);
	p->setBrush(back);
	p->drawRect(0.0, 0.0, static_cast<double>(w), static_cast<double>(h));
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
		for (uint n = 0; n < ts.length(); ++n)
		{
			gly = traceChar(face, charcode, s, &x, &y, &error);
			if (error)
				break;
			if (gly.size() > 3)
			{
				gly.translate(static_cast<double>(pen_x) / 64.0, a);
				gp = getMaxClipF(&gly);
				ymax = QMAX(ymax, gp.y());
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
		for (uint n = 0; n < ts.length(); ++n)
		{
			dv = ts[n].unicode();
			error = false;
			gly = traceChar(face, dv, s, &x, &y, &error);
			if (gly.size() > 3)
			{
				gly.translate(static_cast<double>(pen_x) / 64.0, a);
				gp = getMaxClipF(&gly);
				ymax = QMAX(ymax, gp.y());
				p->setupPolygon(&gly);
				p->fillPath();
			}
			pen_x += face->glyph->advance.x;
		}
	}
	p->end();
	pm.resize(QMIN(qRound(gp.x()), w), QMIN(qRound(ymax), h));
	delete p;
	FT_Done_FreeType( library );
	return pm;
}

/** Same as FontSample() with \n strings support added.
09/26/2004 petr vanek
*/
QPixmap fontSamples(QString da, int s, QString ts, QColor back)
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
		sample = FontSample(da, s, *it, back);
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

/***************************************************************************
    begin                : Wed Oct 29 2003
    copyright            : (C) 2003 The Scribus Team
    email                : paul@all-the-johnsons.co.uk
 ***************************************************************************/
// check if the file exists, if it does, ask if they're sure
// return true if they're sure, else return false;

bool overwrite(QWidget *parent, QString filename)
{
	bool retval = true;
	QFileInfo fi(filename);
	if (fi.exists())
	{
		int t = QMessageBox::warning(parent, QObject::tr("File exists"),
		                             QObject::tr("A file named '%1' already exists.\nDo you want to replace it with the file you are saving?").arg(filename),
		                             QObject::tr("&Cancel"), QObject::tr("&Replace"), "", 1, 0);
		if (t == 0)
			retval = false;
	}
	return retval;
}

void CopyPageItem(struct CopyPasteBuffer *Buffer, PageItem *currItem)
{
	uint a;
	Buffer->PType = currItem->itemType();
	Buffer->Xpos = currItem->Xpos;
	Buffer->Ypos = currItem->Ypos;
	Buffer->Width = currItem->Width;
	Buffer->Height = currItem->Height;
	Buffer->RadRect = currItem->RadRect;
	Buffer->FrameType = currItem->FrameType;
	Buffer->ClipEdited = currItem->ClipEdited;
	Buffer->Pwidth = currItem->Pwidth;
	Buffer->Pcolor = currItem->fillColor();
	Buffer->Pcolor2 = currItem->lineColor();
	Buffer->Shade = currItem->fillShade();
	Buffer->Shade2 = currItem->lineShade();
	Buffer->GrColor = "";
	Buffer->GrColor2 = "";
	Buffer->GrShade = 100;
	Buffer->GrShade2 = 100;
	Buffer->fill_gradient = currItem->fill_gradient;
	Buffer->GrType = currItem->GrType;
	Buffer->GrStartX = currItem->GrStartX;
	Buffer->GrStartY = currItem->GrStartY;
	Buffer->GrEndX = currItem->GrEndX;
	Buffer->GrEndY = currItem->GrEndY;
	Buffer->TxtStroke = currItem->TxtStroke;
	Buffer->TxtFill = currItem->TxtFill;
	Buffer->ShTxtStroke = currItem->ShTxtStroke;
	Buffer->ShTxtFill = currItem->ShTxtFill;
	Buffer->TxtScale = currItem->TxtScale;
	Buffer->TxTStyle = currItem->TxTStyle;
	Buffer->Rot = currItem->Rot;
	Buffer->PLineArt = currItem->PLineArt;
	Buffer->PLineEnd = currItem->PLineEnd;
	Buffer->PLineJoin = currItem->PLineJoin;
	Buffer->LineSp = currItem->LineSp;
	Buffer->LocalScX = currItem->LocalScX;
	Buffer->LocalScY = currItem->LocalScY;
	Buffer->LocalX = currItem->LocalX;
	Buffer->LocalY = currItem->LocalY;
	Buffer->PicArt = currItem->PicArt;
	Buffer->flippedH = currItem->imageFlippedH();
	Buffer->flippedV = currItem->imageFlippedV();
	Buffer->BBoxX = currItem->BBoxX;
	Buffer->BBoxH = currItem->BBoxH;
	Buffer->isPrintable = currItem->isPrintable;
	Buffer->isBookmark = currItem->isBookmark;
	Buffer->BMnr = currItem->BMnr;
	Buffer->isAnnotation = currItem->isAnnotation;
	Buffer->AnType = currItem->AnType;
	Buffer->AnAction = currItem->AnAction;
	Buffer->An_E_act = currItem->An_E_act;
	Buffer->An_X_act = currItem->An_X_act;
	Buffer->An_D_act = currItem->An_D_act;
	Buffer->An_Fo_act = currItem->An_Fo_act;
	Buffer->An_Bl_act = currItem->An_Bl_act;
	Buffer->An_K_act = currItem->An_K_act;
	Buffer->An_F_act = currItem->An_F_act;
	Buffer->An_V_act = currItem->An_V_act;
	Buffer->An_C_act = currItem->An_C_act;
	Buffer->An_Extern = currItem->An_Extern;
	Buffer->AnZiel = currItem->AnZiel;
	Buffer->AnName = currItem->itemName();
	Buffer->AnActType = currItem->AnActType;
	Buffer->AnToolTip = currItem->AnToolTip;
	Buffer->AnBwid = currItem->AnBwid;
	Buffer->AnBsty = currItem->AnBsty;
	Buffer->AnFeed = currItem->AnFeed;
	Buffer->AnFlag = currItem->AnFlag;
	Buffer->AnFont = currItem->AnFont;
	Buffer->AnRollOver = currItem->AnRollOver;
	Buffer->AnDown = currItem->AnDown;
	Buffer->AnFormat = currItem->AnFormat;
	Buffer->AnVis = currItem->AnVis;
	Buffer->AnMaxChar = currItem->AnMaxChar;
	Buffer->AnChkStil = currItem->AnChkStil;
	Buffer->AnIsChk = currItem->AnIsChk;
	Buffer->AnAAact = currItem->AnAAact;
	Buffer->AnBColor = currItem->AnBColor;
	Buffer->AnHTML = currItem->AnHTML;
	Buffer->AnUseIcons = currItem->AnUseIcons;
	Buffer->AnIPlace = currItem->AnIPlace;
	Buffer->AnScaleW = currItem->AnScaleW;
	Buffer->Extra = currItem->Extra;
	Buffer->TExtra = currItem->TExtra;
	Buffer->BExtra = currItem->BExtra;
	Buffer->RExtra = currItem->RExtra;
	Buffer->Pfile = currItem->Pfile;
	Buffer->Pfile2 = currItem->Pfile2;
	Buffer->Pfile3 = currItem->Pfile3;
	QString Text = "";
	if (currItem->itemText.count() != 0)
	{
		for (a=0; a<currItem->itemText.count(); ++a)
		{
			if( (currItem->itemText.at(a)->ch == "\n") || (currItem->itemText.at(a)->ch == "\r"))
				Text += QString(QChar(5))+"\t";
			else if(currItem->itemText.at(a)->ch == "\t")
				Text += QString(QChar(4))+"\t";
			else
				Text += currItem->itemText.at(a)->ch+"\t";
			Text += currItem->itemText.at(a)->cfont->SCName+"\t";
			Text += QString::number(currItem->itemText.at(a)->csize / 10.0)+"\t";
			Text += currItem->itemText.at(a)->ccolor+"\t";
			Text += QString::number(currItem->itemText.at(a)->cextra)+"\t";
			Text += QString::number(currItem->itemText.at(a)->cshade)+'\t';
			Text += QString::number(currItem->itemText.at(a)->cstyle)+'\t';
			Text += QString::number(currItem->itemText.at(a)->cab)+'\t';
			Text += currItem->itemText.at(a)->cstroke+"\t";
			Text += QString::number(currItem->itemText.at(a)->cshade2)+'\t';
			Text += QString::number(currItem->itemText.at(a)->cscale)+'\n';
		}
	}
	Buffer->itemText = Text;
	Buffer->Clip = currItem->Clip.copy();
	Buffer->PoLine = currItem->PoLine.copy();
	Buffer->ContourLine = currItem->ContourLine.copy();
	Buffer->UseContour = currItem->textFlowUsesContourLine();
	Buffer->TabValues = currItem->TabValues;
	Buffer->DashValues = currItem->DashValues;
	Buffer->DashOffset = currItem->DashOffset;
	Buffer->PoShow = currItem->PoShow;
	Buffer->BaseOffs = currItem->BaseOffs;
	Buffer->Textflow = currItem->textFlowsAroundFrame();
	Buffer->Textflow2 = currItem->textFlowUsesBoundingBox();
	Buffer->textAlignment = currItem->textAlignment;
	Buffer->IFont = currItem->IFont;
	Buffer->ISize = currItem->ISize;
	Buffer->ExtraV = currItem->ExtraV;
	Buffer->Groups = currItem->Groups;
	Buffer->IProfile = currItem->IProfile;
	Buffer->IRender = currItem->IRender;
	Buffer->UseEmbedded = currItem->UseEmbedded;
	Buffer->EmProfile = currItem->EmProfile;
	Buffer->LayerNr = currItem->LayerNr;
	Buffer->ScaleType = currItem->ScaleType;
	Buffer->AspectRatio = currItem->AspectRatio;
	Buffer->Locked = currItem->locked();
	Buffer->LockRes = currItem->sizeLocked();
	Buffer->Transparency = currItem->fillTransparency();
	Buffer->TranspStroke = currItem->lineTransparency();
	Buffer->Reverse = currItem->Reverse;
	Buffer->InvPict = currItem->InvPict;
	Buffer->NamedLStyle = currItem->NamedLStyle;
	Buffer->Language = currItem->Language;
	Buffer->Cols = currItem->Cols;
	Buffer->ColGap = currItem->ColGap;
	Buffer->isTableItem = currItem->isTableItem;
	Buffer->TopLine = currItem->TopLine;
	Buffer->LeftLine = currItem->LeftLine;
	Buffer->RightLine = currItem->RightLine;
	Buffer->BottomLine = currItem->BottomLine;
	if (currItem->isTableItem)
	{
		if (currItem->TopLink != 0)
			Buffer->TopLinkID = currItem->TopLink->ItemNr;
		else
			Buffer->TopLinkID = -1;
		if (currItem->LeftLink != 0)
			Buffer->LeftLinkID = currItem->LeftLink->ItemNr;
		else
			Buffer->LeftLinkID = -1;
		if (currItem->RightLink != 0)
			Buffer->RightLinkID = currItem->RightLink->ItemNr;
		else
			Buffer->RightLinkID = -1;
		if (currItem->BottomLink != 0)
			Buffer->BottomLinkID = currItem->BottomLink->ItemNr;
		else
			Buffer->BottomLinkID = -1;
	}
	Buffer->startArrowIndex = currItem->startArrowIndex;
	Buffer->endArrowIndex = currItem->endArrowIndex;
}

void WordAndPara(PageItem* currItem, int *w, int *p, int *c, int *wN, int *pN, int *cN)
{
	QChar Dat = QChar(32);
	int para = 0;
	int ww = 0;
	int cc = 0;
	int paraN = 0;
	int wwN = 0;
	int ccN = 0;
	bool first = true;
	PageItem *nextItem = currItem;
	PageItem *nbl = currItem;
	while (nextItem != 0)
	{
		if (nextItem->BackBox != 0)
			nextItem = nextItem->BackBox;
		else
			break;
	}
	while (nextItem != 0)
	{
		for (uint a = 0; a < nextItem->itemText.count(); ++a)
		{
			QChar b = nextItem->itemText.at(a)->ch[0];
			if (b == QChar(13))
			{
				if (a >= nextItem->MaxChars)
					paraN++;
				else
					para++;
			}
			if ((!b.isLetterOrNumber()) && (Dat.isLetterOrNumber()) && (!first))
			{
				if (a >= nextItem->MaxChars)
					wwN++;
				else
					ww++;
			}
			if (a >= nextItem->MaxChars)
				ccN++;
			else
				cc++;
			Dat = b;
			first = false;
		}
		nbl = nextItem;
		nextItem = nextItem->NextBox;
	}
	if (nbl->MaxChars < nbl->itemText.count())
		paraN++;
	else
		para++;
	if (Dat.isLetterOrNumber())
	{
		if (nbl->MaxChars < nbl->itemText.count())
			wwN++;
		else
			ww++;
	}
	*w = ww;
	*p = para;
	*c = cc;
	*wN = wwN;
	*pN = paraN;
	*cN = ccN;
}

void ReOrderText(ScribusDoc *currentDoc, ScribusView *view)
{
	double savScale = view->Scale;
	view->Scale = 1.0;
	currentDoc->RePos = true;
	QPixmap pgPix(10, 10);
	QRect rd = QRect(0,0,9,9);
	ScPainter *painter = new ScPainter(&pgPix, pgPix.width(), pgPix.height());
	for (uint azz=0; azz<currentDoc->MasterItems.count(); ++azz)
	{
		PageItem *currItem = currentDoc->MasterItems.at(azz);
		if (currItem->itemType() == PageItem::PathText)
			currItem->DrawObj(painter, rd);
	}
	for (uint azz=0; azz<currentDoc->Items.count(); ++azz)
	{
		PageItem *currItem = currentDoc->Items.at(azz);
		if ((currItem->itemType() == PageItem::TextFrame) || (currItem->itemType() == PageItem::PathText))
			currItem->DrawObj(painter, rd);
	}
	currentDoc->RePos = false;
	view->Scale = savScale;
	delete painter;
}

/*! 10/06/2004 - pv
\param QString s1 first string
\param QString s2 second string
\retval bool t/f related s1>s2
 */
bool compareQStrings(QString s1, QString s2)
{
	if (QString::localeAwareCompare(s1, s2) >= 0)
		return FALSE;
	return TRUE;
}

/*! 10/06/2004 - pv
Returns a sorted list of QStrings - sorted by locale specific
rules! Uses compareQStrings() as rule. There is STL used!
TODO: Maybe we can implement one cass for various sorting...
\param QStringList aList unsorted string list
\retval QStringList sorted string list
*/
QStringList sortQStringList(QStringList aList)
{
	std::vector<QString> sortList;
	QStringList retList;
	QStringList::Iterator it;
	for (it = aList.begin(); it != aList.end(); ++it)
		sortList.push_back(*it);
	std::sort(sortList.begin(), sortList.end(), compareQStrings);
	for(uint i = 0; i < sortList.size(); i++)
		retList.append(sortList[i]);
	return retList;
}

void GetItemProps(bool newVersion, QDomElement *obj, struct CopyPasteBuffer *OB)
{
	QString tmp;
	int x, y;
	double xf, yf;
	OB->PType = static_cast<PageItem::ItemType>(QStoInt(obj->attribute("PTYPE")));
	OB->Width=QStodouble(obj->attribute("WIDTH"));
	OB->Height=QStodouble(obj->attribute("HEIGHT"));
	OB->RadRect = QStodouble(obj->attribute("RADRECT","0"));
	OB->ClipEdited = QStoInt(obj->attribute("CLIPEDIT", "0"));
	OB->FrameType = QStoInt(obj->attribute("FRTYPE", "0"));
	OB->Pwidth=QStodouble(obj->attribute("PWIDTH"));
	OB->Pcolor = obj->attribute("PCOLOR");
	if ((!newVersion) && (OB->PType == 4))
	{
		OB->TxtFill = obj->attribute("PCOLOR2");
		OB->Pcolor2 = "None";
	}
	else
	{
		OB->Pcolor2 = obj->attribute("PCOLOR2");
		OB->TxtFill = obj->attribute("TXTFILL", "Black");
	}
	OB->Shade = QStoInt(obj->attribute("SHADE"));
	OB->Shade2 = QStoInt(obj->attribute("SHADE2"));
	OB->TxtStroke=obj->attribute("TXTSTROKE", "None");
	OB->ShTxtFill=QStoInt(obj->attribute("TXTFILLSH", "100"));
	OB->ShTxtStroke=QStoInt(obj->attribute("TXTSTRSH", "100"));
	OB->TxtScale=QStoInt(obj->attribute("TXTSCALE", "100"));
	OB->TxTStyle=QStoInt(obj->attribute("TXTSTYLE", "0"));
	OB->Cols = QStoInt(obj->attribute("COLUMNS","1"));
	OB->ColGap = QStodouble(obj->attribute("COLGAP","0.0"));
	OB->GrType = QStoInt(obj->attribute("GRTYP","0"));
	OB->fill_gradient.clearStops();
	if (OB->GrType != 0)
	{
		OB->GrStartX = QStodouble(obj->attribute("GRSTARTX","0.0"));
		OB->GrStartY = QStodouble(obj->attribute("GRSTARTY","0.0"));
		OB->GrEndX = QStodouble(obj->attribute("GRENDX","0.0"));
		OB->GrEndY = QStodouble(obj->attribute("GRENDY","0.0"));
		OB->GrColor = obj->attribute("GRCOLOR","");
		if (OB->GrColor != "")
		{
			OB->GrColor2 = obj->attribute("GRCOLOR2","");
			OB->GrShade = QStoInt(obj->attribute("GRSHADE","100"));
			OB->GrShade2 = QStoInt(obj->attribute("GRSHADE2","100"));
		}
	}
	OB->Rot=QStodouble(obj->attribute("ROT"));
	OB->PLineArt=Qt::PenStyle(QStoInt(obj->attribute("PLINEART")));
	OB->PLineEnd=Qt::PenCapStyle(QStoInt(obj->attribute("PLINEEND","0")));
	OB->PLineJoin=Qt::PenJoinStyle(QStoInt(obj->attribute("PLINEJOIN","0")));
	OB->LineSp=QStodouble(obj->attribute("LINESP"));
	OB->ExtraV=QStodouble(obj->attribute("EXTRAV","0"));
	OB->LocalScX=QStodouble(obj->attribute("LOCALSCX"));
	OB->LocalScY=QStodouble(obj->attribute("LOCALSCY"));
	OB->LocalX=QStodouble(obj->attribute("LOCALX"));
	OB->LocalY=QStodouble(obj->attribute("LOCALY"));
	OB->PicArt=QStoInt(obj->attribute("PICART"));
	OB->flippedH = QStoInt(obj->attribute("FLIPPEDH"));
	OB->flippedV = QStoInt(obj->attribute("FLIPPEDV"));
	OB->BBoxX=QStodouble(obj->attribute("BBOXX"));
	OB->BBoxH=QStodouble(obj->attribute("BBOXH"));
	OB->ScaleType = QStoInt(obj->attribute("SCALETYPE","1"));
	OB->AspectRatio = QStoInt(obj->attribute("RATIO","0"));
	OB->isPrintable=QStoInt(obj->attribute("PRINTABLE"));
	OB->isAnnotation=QStoInt(obj->attribute("ANNOTATION","0"));
	OB->AnType = QStoInt(obj->attribute("ANTYPE","0"));
	OB->AnAction = obj->attribute("ANACTION","");
	OB->An_E_act = obj->attribute("ANEACT","");
	OB->An_X_act = obj->attribute("ANXACT","");
	OB->An_D_act = obj->attribute("ANDACT","");
	OB->An_Fo_act = obj->attribute("ANFOACT","");
	OB->An_Bl_act = obj->attribute("ANBLACT","");
	OB->An_K_act = obj->attribute("ANKACT","");
	OB->An_F_act = obj->attribute("ANFACT","");
	OB->An_V_act = obj->attribute("ANVACT","");
	OB->An_C_act = obj->attribute("ANCACT","");
	OB->AnActType = QStoInt(obj->attribute("ANACTYP","0"));
	OB->An_Extern = obj->attribute("ANEXTERN","");
	if ((OB->An_Extern != "") && (OB->AnActType != 8))
	{
		QFileInfo efp(OB->An_Extern);
		OB->An_Extern = efp.absFilePath();
	}
	OB->AnZiel = QStoInt(obj->attribute("ANZIEL","0"));
	OB->AnName = obj->attribute("ANNAME","");
	OB->AnToolTip = obj->attribute("ANTOOLTIP","");
	OB->AnRollOver = obj->attribute("ANROLL","");
	OB->AnDown = obj->attribute("ANDOWN","");
	OB->AnBwid = QStoInt(obj->attribute("ANBWID","1"));
	OB->AnBsty = QStoInt(obj->attribute("ANBSTY","0"));
	OB->AnFeed = QStoInt(obj->attribute("ANFEED","1"));
	OB->AnFlag = QStoInt(obj->attribute("ANFLAG","0"));
	OB->AnFont = QStoInt(obj->attribute("ANFONT","4"));
	OB->AnFormat = QStoInt(obj->attribute("ANFORMAT","0"));
	OB->AnVis = QStoInt(obj->attribute("ANVIS","0"));
	OB->AnIsChk = static_cast<bool>(QStoInt(obj->attribute("ANCHK","0")));
	OB->AnAAact = static_cast<bool>(QStoInt(obj->attribute("ANAA","0")));
	OB->AnHTML = static_cast<bool>(QStoInt(obj->attribute("ANHTML","0")));
	OB->AnUseIcons = static_cast<bool>(QStoInt(obj->attribute("ANICON","0")));
	OB->AnChkStil = QStoInt(obj->attribute("ANCHKS","0"));
	OB->AnMaxChar = QStoInt(obj->attribute("ANMC","-1"));
	OB->AnBColor = obj->attribute("ANBCOL","None");
	OB->AnIPlace = QStoInt(obj->attribute("ANPLACE","1"));
	OB->AnScaleW = QStoInt(obj->attribute("ANSCALE","0"));
	if (QStoInt(obj->attribute("TRANSPARENT","0")) == 1)
		OB->Pcolor = "None";
	OB->Textflow=QStoInt(obj->attribute("TEXTFLOW"));
	OB->Textflow2 =QStoInt(obj->attribute("TEXTFLOW2","0"));
	OB->UseContour = QStoInt(obj->attribute("TEXTFLOW3","0"));
	OB->Extra=QStodouble(obj->attribute("EXTRA"));
	OB->TExtra=QStodouble(obj->attribute("TEXTRA", "1"));
	OB->BExtra=QStodouble(obj->attribute("BEXTRA", "1"));
	OB->RExtra=QStodouble(obj->attribute("REXTRA", "1"));
	OB->PoShow = QStoInt(obj->attribute("PLTSHOW","0"));
	OB->BaseOffs = QStodouble(obj->attribute("BASEOF","0"));
	OB->ISize = qRound(QStodouble(obj->attribute("ISIZE","12")) * 10);
	OB->Pfile=obj->attribute("PFILE");
	OB->Pfile2=obj->attribute("PFILE2","");
	OB->Pfile3=obj->attribute("PFILE3","");
	OB->IProfile=obj->attribute("PRFILE","");
	OB->EmProfile=obj->attribute("EPROF","");
	OB->IRender = QStoInt(obj->attribute("IRENDER","1"));
	OB->UseEmbedded = QStoInt(obj->attribute("EMBEDDED","1"));
	OB->Locked = static_cast<bool>(QStoInt(obj->attribute("LOCK","0")));
	OB->LockRes = static_cast<bool>(QStoInt(obj->attribute("LOCKR","0")));
	OB->Reverse = static_cast<bool>(QStoInt(obj->attribute("REVERS","0")));
	OB->InvPict = static_cast<bool>(QStoInt(obj->attribute("INVERS","0")));
	OB->isTableItem = static_cast<bool>(QStoInt(obj->attribute("isTableItem","0")));
	OB->TopLine = static_cast<bool>(QStoInt(obj->attribute("TopLine","0")));
	OB->LeftLine = static_cast<bool>(QStoInt(obj->attribute("LeftLine","0")));
	OB->RightLine = static_cast<bool>(QStoInt(obj->attribute("RightLine","0")));
	OB->BottomLine = static_cast<bool>(QStoInt(obj->attribute("BottomLine","0")));
	OB->TopLinkID =  QStoInt(obj->attribute("TopLINK","-1"));
	OB->LeftLinkID =  QStoInt(obj->attribute("LeftLINK","-1"));
	OB->RightLinkID =  QStoInt(obj->attribute("RightLINK","-1"));
	OB->BottomLinkID =  QStoInt(obj->attribute("BottomLINK","-1"));
	OB->Transparency = QStodouble(obj->attribute("TransValue","0.0"));
	if (obj->hasAttribute("TransValueS"))
		OB->TranspStroke = QStodouble(obj->attribute("TransValueS","0.0"));
	else
		OB->TranspStroke = OB->Transparency;
	tmp = "";
	if (obj->hasAttribute("NUMCLIP"))
	{
		OB->Clip.resize(obj->attribute("NUMCLIP").toUInt());
		tmp = obj->attribute("CLIPCOOR");
		QTextStream fc(&tmp, IO_ReadOnly);
		for (uint c=0; c<obj->attribute("NUMCLIP").toUInt(); ++c)
		{
			fc >> x;
			fc >> y;
			OB->Clip.setPoint(c, x, y);
		}
	}
	else
		OB->Clip.resize(0);
	tmp = "";
	if (obj->hasAttribute("NUMPO"))
	{
		OB->PoLine.resize(obj->attribute("NUMPO").toUInt());
		tmp = obj->attribute("POCOOR");
		QTextStream fp(&tmp, IO_ReadOnly);
		for (uint cx=0; cx<obj->attribute("NUMPO").toUInt(); ++cx)
		{
			fp >> xf;
			fp >> yf;
			OB->PoLine.setPoint(cx, xf, yf);
		}
	}
	else
		OB->PoLine.resize(0);
	tmp = "";
	if (obj->hasAttribute("NUMCO"))
	{
		OB->ContourLine.resize(obj->attribute("NUMCO").toUInt());
		tmp = obj->attribute("COCOOR");
		QTextStream fp(&tmp, IO_ReadOnly);
		for (uint cx=0; cx<obj->attribute("NUMCO").toUInt(); ++cx)
		{
			fp >> xf;
			fp >> yf;
			OB->ContourLine.setPoint(cx, xf, yf);
		}
	}
	else
		OB->ContourLine.resize(0);
	tmp = "";
	if ((obj->hasAttribute("NUMTAB")) && (QStoInt(obj->attribute("NUMTAB","0")) != 0))
	{
		tmp = obj->attribute("TABS");
		QTextStream tgv(&tmp, IO_ReadOnly);
		OB->TabValues.clear();
		for (int cxv = 0; cxv < QStoInt(obj->attribute("NUMTAB","0")); ++cxv)
		{
			tgv >> xf;
			OB->TabValues.append(xf);
		}
		tmp = "";
	}
	else
		OB->TabValues.clear();
	if ((obj->hasAttribute("NUMDASH")) && (QStoInt(obj->attribute("NUMDASH","0")) != 0))
	{
		tmp = obj->attribute("DASHS");
		QTextStream dgv(&tmp, IO_ReadOnly);
		OB->DashValues.clear();
		for (int cxv = 0; cxv < QStoInt(obj->attribute("NUMDASH","0")); ++cxv)
		{
			dgv >> xf;
			OB->DashValues.append(xf);
		}
		tmp = "";
	}
	else
		OB->DashValues.clear();
	OB->DashOffset = QStodouble(obj->attribute("DASHOFF","0.0"));
}

QColor SetColor(ScribusDoc *currentDoc, QString color, int shad)
{
	return currentDoc->PageColors[color].getShadeColorProof(shad);
}
