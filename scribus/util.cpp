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
#include <qstringlist.h>
#include <qmap.h>
#include <qdom.h>
#include <qimage.h>
#include <qdir.h>
#include <qpointarray.h>
#include <cstdlib>
#include <cmath>
#include "md5.h"
#include "config.h"
#include "scribusdoc.h"
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
extern cmsHTRANSFORM stdTrans;
extern cmsHTRANSFORM stdProof;
extern cmsHTRANSFORM stdTransImg;
extern cmsHTRANSFORM stdProofImg;
extern bool SoftProofing;
extern bool Gamut;
extern bool CMSuse;
extern int IntentMonitor;
extern int IntentPrinter;
#endif
extern ProfilesL InputProfiles;

FPointArray traceChar(FT_Face face, uint chr, int chs, double *x, double *y);
QString Path2Relative(QString Path);
QPixmap LoadPDF(QString fn, int Seite, int Size, int *w, int *h);
bool GlyNames(QMap<uint, QString> *GList, QString Dat);
bool GlyIndex(QMap<uint, PDFlib::GlNamInd> *GListInd, QString Dat);
QByteArray ComputeMD5Sum(QByteArray *in);
char *toHex( uchar u );
QString String2Hex(QString *in, bool lang = true);
QString CompressStr(QString *in);
QString ImageToTxt(QImage *im);
QString ImageToCMYK(QImage *im);
QString ImageToCMYK_PS(QImage *im, int pl, bool pre);
QString MaskToTxt(QImage *im, bool PDF = true);
void Level2Layer(ScribusDoc *doc, struct Layer *ll, int Level);
void BezierPoints(QPointArray *ar, QPoint n1, QPoint n2, QPoint n3, QPoint n4);
double xy2Deg(double x, double y);
QPointArray FlattenPath(FPointArray ina, QValueList<uint> &Segs);
QPointArray RegularPolygon(double w, double h, uint c, bool star, double factor, double rota);
FPointArray RegularPolygonF(double w, double h, uint c, bool star, double factor, double rota);
QPixmap loadIcon(QString nam);
bool loadText(QString nam, QString *Buffer);
double Cwidth(ScribusDoc *doc, QString name, QString ch, int Siz, QString ch2 = " ");
double QStodouble(QString in);
int QStoInt(QString in);
QString GetAttr(QDomElement *el, QString at, QString def="0");
QImage LoadPict(QString fn);
#ifdef HAVE_CMS
QImage ProofPict(QImage *Im, QString Prof, int Rend, cmsHPROFILE emPr=0);
QImage LoadPictCol(QString fn, QString Prof, bool UseEmbedded, bool *realCMYK);
#else
QImage ProofPict(QImage *Im, QString Prof, int Rend);
#endif
QImage ProofImage(QImage *Im);

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
		if (Gamut)
			dcmsFlags |= cmsFLAGS_GAMUTCHECK;
		else
			dcmsFlags |= cmsFLAGS_SOFTPROOFING;
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

QImage ProofImage(QImage *Im)
{
#ifdef HAVE_CMS
	QImage out = Im->copy();
	if ((CMSuse) && (SoftProofing))
		{
		for (int i=0; i < out.height(); ++i)
			{
			LPBYTE ptr = out.scanLine(i);
			cmsDoTransform(stdProofImg, ptr, ptr, out.width());
			}
		}
	else
		{
		if (CMSuse)
			{
			for (int i=0; i < out.height(); ++i)
				{
				LPBYTE ptr = out.scanLine(i);
				cmsDoTransform(stdTransImg, ptr, ptr, out.width());
				}
			}
		}
	return out;
#else
	return Im->copy();
#endif
}

QPixmap LoadPDF(QString fn, int Seite, int Size, int *w, int *h)
{
	QString tmp, cmd1, cmd2;
	QPixmap pm;
	int ret = -1;
	tmp.setNum(Seite);
	cmd1 = "gs -q -dNOPAUSE -sDEVICE=png16m -r72 -sOutputFile=/tmp/sc.png -dFirstPage="+tmp+" -dLastPage="+tmp+" ";
	cmd2 = " -c showpage -c quit";
	ret = system(cmd1 + fn + cmd2);
	if (ret == 0)
		{
		QImage image;
		image.load("/tmp/sc.png");
		system("rm -f /tmp/sc.png");
  	QImage im2;
		*h = image.height();
		*w = image.width();
		double sx = image.width() / static_cast<double>(Size);
		double sy = image.height() / static_cast<double>(Size);
		if (sy < sx)
			im2 = image.smoothScale(static_cast<int>(image.width() / sx), static_cast<int>(image.height() / sx));
		else
			im2 = image.smoothScale(static_cast<int>(image.width() / sy), static_cast<int>(image.height() / sy));
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

QImage LoadPict(QString fn)
{
	QString tmp, dummy, cmd1, cmd2, BBox;
	QChar tc;
	QImage Bild;
	double x, y, b, h;
	bool found = false;
	int ret = -1;
	QFileInfo fi = QFileInfo(fn);
	QString ext = fi.extension(false).lower();
	if (ext == "pdf")
		{
		cmd1 = "gs -q -dNOPAUSE -sDEVICE=png16m -r72 -sOutputFile=/tmp/sc.png -dFirstPage=1 -dLastPage=1 ";
		cmd2 = " -c showpage -c quit";
		ret = system(cmd1 + fn + cmd2);
		if (ret == 0)
			{
			QImage image;
			image.load("/tmp/sc.png");
  		Bild = image.convertDepth(32);
			system("rm -f /tmp/sc.png");
			}
		}
	if ((ext == "eps") || (ext == "ps"))
		{
		QFile f(fn);
		if (f.open(IO_ReadOnly))
			{
			QTextStream ts(&f);
			while (!ts.atEnd())
				{
				tc = ' ';
				tmp = "";
				while ((tc != '\n') && (tc != '\r'))
					{
					ts >> tc;
					if ((tc != '\n') && (tc != '\r'))
						tmp += tc;
					}
				if (tmp.startsWith("%%BoundingBox"))
					{
					found = true;
					BBox = tmp;
					}
				if (tmp.startsWith("%%EndComments"))
					break;
				}
			f.close();
			if (found)
				{
				QTextStream ts2(&BBox, IO_ReadOnly);
				ts2 >> dummy >> x >> y >> b >> h;
				cmd1 = "gs -q -dNOPAUSE -sDEVICE=png16m -r72 -sOutputFile=/tmp/sc.png -g";
				cmd2 = " -c showpage -c quit";
				ret = system(cmd1 + tmp.setNum(qRound(b)) + "x" + tmp.setNum(qRound(h)) + " " + fn + cmd2);
				if (ret == 0)
					{
					QImage im4;
					QImage image;
					image.load("/tmp/sc.png");
  				image = image.convertDepth(32);
					int wi = image.width();
					int hi = image.height();
					QBitmap bm("/tmp/sc.png");
					bm.fill(Qt::color1);
    			QPainter pp;
    			pp.begin(&bm);
    			pp.setPen(Qt::color0);
    			QString tmp2;
    			if ( image.depth() == 8 )
						{
        		for( int yi=0; yi < hi; ++yi )
							{
            	uchar * s = image.scanLine( yi );
            	for( int xi=0; xi < wi; ++xi )
								{
                if(image.color(s[xi]) == 0xffffffff)
                	pp.drawPoint(xi, yi);
            		}
        			}
    				}
					else
						{
        		for( int yi=0; yi < hi; ++yi )
							{
            	QRgb * s = (QRgb*)(image.scanLine( yi ));
              for(int xi=0; xi < wi; ++xi )
								{
                if((*s++) == 0xffffffff)
                	pp.drawPoint(xi, yi);
                }
        			}
    				}
    			pp.end();
					QPixmap pm;
					pm.convertFromImage(image);
					pm.setMask(bm);
					im4.setAlphaBuffer(true);
					im4 = pm.convertToImage();
					Bild = im4.copy(static_cast<int>(x), 0, static_cast<int>(b-x), static_cast<int>(h-y));
					system("rm -f /tmp/sc.png");
					}
				}
			}
		}
#ifdef HAVE_TIFF
	if (ext == "tif")
		{
		QImage img;
		QImage inI2;
		TIFF* tif = TIFFOpen(fn, "r");
		if(tif)
			{
			unsigned width, height,size;
			TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
			TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
			size=width*height;
			uint32 *bits=(uint32*) _TIFFmalloc(size * sizeof (uint32));
			if(bits)
				{
				if (TIFFReadRGBAImage(tif, width, height, bits, 0))
					{
        	img.create(width,height,32);
					if(TIFFGetR(0x1234567)==qRed  (0x1234567) &&
						 TIFFGetG(0x1234567)==qGreen(0x1234567) &&
						 TIFFGetB(0x1234567)==qBlue (0x1234567))
						{
						for(unsigned y=0; y<height; ++y)
							memcpy(img.scanLine(height-1-y),bits+y*width,width*4);
						}
					else
						{
						uint32 *inp=bits;
						for(unsigned y=0; y<height; ++y)
							{
							QRgb *row=(QRgb*) (img.scanLine(height-1-y));
							for(unsigned x=0; x<width; ++x)
								{
								const uint32 col=*(inp++);
								row[x]=qRgb(TIFFGetR(col), TIFFGetG(col), TIFFGetB(col) ) | (TIFFGetA(col)<<24);
								}
							}
						}
					Bild = img.copy();
					}
				_TIFFfree(bits);
				}
			TIFFClose(tif);
			}
		}
#endif
	else
		{
		Bild.load(fn);
  	Bild = Bild.convertDepth(32);
		}
	return Bild;
}

#ifdef HAVE_CMS
QImage LoadPictCol(QString fn, QString Prof, bool UseEmbedded, bool *realCMYK)
{
	QString tmp, dummy, cmd1, cmd2, BBox;
	QChar tc;
	QImage Bild;
	double x, y, b, h;
	bool found = false;
	int ret = -1;
	QFileInfo fi = QFileInfo(fn);
	QString ext = fi.extension(false).lower();
	if (ext == "pdf")
		{
		cmd1 = "gs -q -dNOPAUSE -sDEVICE=png16m -r72 -sOutputFile=/tmp/sc.png -dFirstPage=1 -dLastPage=1 ";
		cmd2 = " -c showpage -c quit";
		ret = system(cmd1 + fn + cmd2);
		if (ret == 0)
			{
			QImage image;
			image.load("/tmp/sc.png");
  		Bild = image.convertDepth(32);
			system("rm -f /tmp/sc.png");
			*realCMYK = false;
			}
		}
	if ((ext == "eps") || (ext == "ps"))
		{
		QFile f(fn);
		if (f.open(IO_ReadOnly))
			{
			QTextStream ts(&f);
			while (!ts.atEnd())
				{
				tc = ' ';
				tmp = "";
				while ((tc != '\n') && (tc != '\r'))
					{
					ts >> tc;
					if ((tc != '\n') && (tc != '\r'))
						tmp += tc;
					}
				if (tmp.startsWith("%%BoundingBox"))
					{
					found = true;
					BBox = tmp;
					}
				if (tmp.startsWith("%%EndComments"))
					break;
				}
			f.close();
			if (found)
				{
				QTextStream ts2(&BBox, IO_ReadOnly);
				ts2 >> dummy >> x >> y >> b >> h;
				cmd1 = "gs -q -dNOPAUSE -sDEVICE=png16m -r72 -sOutputFile=/tmp/sc.png -g";
				cmd2 = " -c showpage -c quit";
				ret = system(cmd1 + tmp.setNum(qRound(b)) + "x" + tmp.setNum(qRound(h)) + " " + fn + cmd2);
				if (ret == 0)
					{
					QImage im4;
					QImage image;
					image.load("/tmp/sc.png");
  				image = image.convertDepth(32);
					int wi = image.width();
					int hi = image.height();
					QBitmap bm("/tmp/sc.png");
					bm.fill(Qt::color1);
    			QPainter pp;
    			pp.begin(&bm);
    			pp.setPen(Qt::color0);
    			QString tmp2;
    			if ( image.depth() == 8 )
						{
        		for( int yi=0; yi < hi; ++yi )
							{
            	uchar * s = image.scanLine( yi );
            	for( int xi=0; xi < wi; ++xi )
								{
                if(image.color(s[xi]) == 0xffffffff)
                	pp.drawPoint(xi, yi);
            		}
        			}
    				}
					else
						{
        		for( int yi=0; yi < hi; ++yi )
							{
            	QRgb * s = (QRgb*)(image.scanLine( yi ));
              for(int xi=0; xi < wi; ++xi )
								{
                if((*s++) == 0xffffffff)
                	pp.drawPoint(xi, yi);
                }
        			}
    				}
    			pp.end();
					QPixmap pm;
					pm.convertFromImage(image);
					pm.setMask(bm);
					im4.setAlphaBuffer(true);
					im4 = pm.convertToImage();
					Bild = im4.copy(static_cast<int>(x), 0, static_cast<int>(b-x), static_cast<int>(h-y));
					system("rm -f /tmp/sc.png");
					*realCMYK = false;
					}
				}
			}
		}
#ifdef HAVE_TIFF
	if (ext == "tif")
		{
		QImage img;
		QImage inI2;
		TIFF* tif = TIFFOpen(fn, "r");
		if(tif)
			{
    	DWORD EmbedLen = 0;
    	LPBYTE EmbedBuffer;
			unsigned width, height,size;
			TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
			TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
			size=width*height;
			uint32 *bits=(uint32*) _TIFFmalloc(size * sizeof (uint32));
			if(bits)
				{
				if (TIFFReadRGBAImage(tif, width, height, bits, 0))
					{
        	img.create(width,height,32);
					if(TIFFGetR(0x1234567)==qRed  (0x1234567) &&
						 TIFFGetG(0x1234567)==qGreen(0x1234567) &&
						 TIFFGetB(0x1234567)==qBlue (0x1234567))
						{
						for(unsigned y=0; y<height; ++y)
							memcpy(img.scanLine(height-1-y),bits+y*width,width*4);
						}
					else
						{
						uint32 *inp=bits;
						for(unsigned y=0; y<height; ++y)
							{
							QRgb *row=(QRgb*) (img.scanLine(height-1-y));
							for(unsigned x=0; x<width; ++x)
								{
								const uint32 col=*(inp++);
								row[x]=qRgb(TIFFGetR(col), TIFFGetG(col), TIFFGetB(col) ) | (TIFFGetA(col)<<24);
								}
							}
						}
					Bild = img.copy();
					}
				_TIFFfree(bits);
				cmsHTRANSFORM xform;
				cmsHPROFILE inputProf;
				if((TIFFGetField(tif, TIFFTAG_ICCPROFILE, &EmbedLen, &EmbedBuffer)) && (UseEmbedded))
					{
					inputProf = cmsOpenProfileFromMem(EmbedBuffer, EmbedLen);
					if ((int) cmsGetColorSpace(inputProf) == icSigRgbData)
						{
						switch ((int) cmsGetColorSpace(CMSprinterProf))
							{
							case icSigRgbData:
								*realCMYK = false;
								xform = cmsCreateTransform(inputProf, TYPE_RGBA_8, CMSprinterProf, TYPE_RGBA_8, IntentPrinter, 0);
								break;
							case icSigCmykData:
								*realCMYK = true;
								xform = cmsCreateTransform(inputProf, TYPE_RGBA_8, CMSprinterProf, TYPE_CMYK_8, IntentPrinter, 0);
								break;
							}
						for (int i=0; i < Bild.height(); ++i)
							{
							LPBYTE ptr = Bild.scanLine(i);
							cmsDoTransform(xform, ptr, ptr, Bild.width());
							}
						}
					cmsCloseProfile(inputProf);
					}
				else
					{
					inputProf = cmsOpenProfileFromFile(InputProfiles[Prof], "r");
					switch ((int) cmsGetColorSpace(CMSprinterProf))
						{
						case icSigRgbData:
							*realCMYK = false;
							xform = cmsCreateTransform(inputProf, TYPE_RGBA_8, CMSprinterProf, TYPE_RGBA_8, IntentPrinter, 0);
							break;
						case icSigCmykData:
							*realCMYK = true;
							xform = cmsCreateTransform(inputProf, TYPE_RGBA_8, CMSprinterProf, TYPE_CMYK_8, IntentPrinter, 0);
							break;
						}
					for (int i=0; i < Bild.height(); ++i)
						{
						LPBYTE ptr = Bild.scanLine(i);
						cmsDoTransform(xform, ptr, ptr, Bild.width());
						}
					}
				cmsDeleteTransform(xform);
				}
			TIFFClose(tif);
			}
		}
#endif
	else
		{
		cmsHTRANSFORM xform;
		cmsHPROFILE inputProf;
		Bild.load(fn);
  	Bild = Bild.convertDepth(32);
		inputProf = cmsOpenProfileFromFile(InputProfiles[Prof], "r");
		switch (static_cast<int>(cmsGetColorSpace(CMSprinterProf)))
			{
			case icSigRgbData:
				*realCMYK = false;
				xform = cmsCreateTransform(inputProf, TYPE_RGBA_8, CMSprinterProf, TYPE_RGBA_8, IntentPrinter, 0);
				break;
			case icSigCmykData:
				*realCMYK = true;
				xform = cmsCreateTransform(inputProf, TYPE_RGBA_8, CMSprinterProf, TYPE_CMYK_8, IntentPrinter, 0);
				break;
			}
		for (int i=0; i < Bild.height(); ++i)
			{
			LPBYTE ptr = Bild.scanLine(i);
			cmsDoTransform(xform, ptr, ptr, Bild.width());
			}
		cmsDeleteTransform(xform);
		}
	return Bild;
}
#endif

QString GetAttr(QDomElement *el, QString at, QString def)
{
	return el->attribute(at, def);
}

int QStoInt(QString in)
{
	bool ok = false;
	int c = in.toInt(&ok);
	if (ok)
		return c;
	else
		return 0;
}

double QStodouble(QString in)
{
	bool ok = false;
	double c = in.toDouble(&ok);
	if (ok)
		return c;
	else
		return 0.0;
}

QPixmap loadIcon(QString nam)
{
  QString pfad = PREL;
  pfad += "/lib/scribus/icons/"+nam;
	QPixmap pm;
	pm.load(pfad);
	return pm;
}

bool loadText(QString nam, QString *Buffer)
{
	QFile f(nam);
	QFileInfo fi(f);
	if (!fi.exists())
		return false;
	uint posi;
	bool ret = false;
	QByteArray bb(f.size());
	if (f.open(IO_ReadOnly))
		{
		f.readBlock(bb.data(), f.size());
		f.close();
		for (posi = 0; posi < bb.size(); ++posi)
			{
			*Buffer += bb[posi];
			}
		ret = true;
		}
	else
		{
		ret = false;
		}
	return ret;
}

double Cwidth(ScribusDoc *doc, QString name, QString ch, int Siz, QString ch2)
{
	double w;
	FT_Vector  delta;
	uint c1 = ch.at(0).unicode();
	uint c2 = ch2.at(0).unicode();
	Foi* fo = (*doc->AllFonts)[name];
	if (fo->CharWidth.contains(c1))
		{
		w = fo->CharWidth[c1]*Siz;
		if (fo->HasKern)
			{
			uint cl = FT_Get_Char_Index(doc->FFonts[name], c1);
			uint cr = FT_Get_Char_Index(doc->FFonts[name], c2);
			FT_Get_Kerning(doc->FFonts[name], cl, cr, ft_kerning_unscaled, &delta);
			w += delta.x / fo->uniEM * Siz;
			}
		return w;
		}
	else
		return static_cast<double>(Siz);
}

QPointArray RegularPolygon(double w, double h, uint c, bool star, double factor, double rota)
{
	uint cx;
	if (star)
		cx = c * 2;
	else
		cx = c;
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
			if (x % 2 == 0)
				{
				mx = qRound(sin(sc / 180 * M_PI) * (w/2) + (w/2));
				my = qRound(cos(sc / 180 * M_PI) * (h/2) + (h/2));
				}
			else
				{
				mx = qRound(sin(sc / 180 * M_PI) * (w/2*di) + (w/2));
				my = qRound(cos(sc / 180 * M_PI) * (h/2*di) + (h/2));
				}
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
	uint cx;
	if (star)
		cx = c * 2;
	else
		cx = c;
	double seg = 360.0 / cx;
	double sc = rota + 180.0;
	double di = factor;
	double mx = 0;
	double my = 0;
	FPointArray pts = FPointArray();
	for (uint x = 0; x < cx; ++x)
		{
		sc = seg * x + 180.0 + rota;
		if (star)
			{
			if (x % 2 == 0)
				{
				mx = sin(sc / 180 * M_PI) * (w/2) + (w/2);
				my = cos(sc / 180 * M_PI) * (h/2) + (h/2);
				}
			else
				{
				mx = sin(sc / 180 * M_PI) * (w/2*di) + (w/2);
				my = cos(sc / 180 * M_PI) * (h/2*di) + (h/2);
				}
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

void Level2Layer(ScribusDoc *doc, struct Layer *ll, int Level)
{
	for (uint la2 = 0; la2 < doc->Layers.count(); ++la2)
		{
		if (doc->Layers[la2].Level == Level)
			{
			ll->Sichtbar = doc->Layers[la2].Sichtbar;
			ll->Drucken = doc->Layers[la2].Drucken;
			ll->LNr = doc->Layers[la2].LNr;
			break;
			}
		}
}

QString ImageToTxt(QImage *im)
{
	int h = im->height();
	int w = im->width();
	QString ImgStr = "";
	for( int yi=0; yi < h; ++yi )
		{
		QRgb * s = (QRgb*)(im->scanLine( yi ));
		for( int xi=0; xi < w; ++xi )
			{
			QRgb r=*s++;
			unsigned char u=qRed(r);
			ImgStr += u;
			u=qGreen(r);
			ImgStr += u;
			u=qBlue(r);
			ImgStr += u;
			}
		}
	return ImgStr;
}

QString ImageToCMYK(QImage *im)
{
	int h = im->height();
	int w = im->width();
	QString ImgStr = "";
	for( int yi=0; yi < h; ++yi )
		{
		QRgb * s = (QRgb*)(im->scanLine( yi ));
		for( int xi=0; xi < w; ++xi )
			{
			QRgb r=*s++;
			int c = 255 - qRed(r);
			int m = 255 - qGreen(r);
			int y = 255 - qBlue(r);
			int k = QMIN(QMIN(c, m), y);
			ImgStr += c - k;
			ImgStr += m - k;
			ImgStr += y - k;
			ImgStr += k;
			}
		}
	return ImgStr;
}

QString ImageToCMYK_PS(QImage *im, int pl, bool pre)
{
	int h = im->height();
	int w = im->width();
	QString ImgStr = "";
	if (pre)
		{
		for( int yi=0; yi < h; ++yi )
			{
			QRgb * s = (QRgb*)(im->scanLine( yi ));
			for( int xi=0; xi < w; ++xi )
				{
				QRgb r=*s++;
				int c = qRed(r);
				int m = qGreen(r);
				int y = qBlue(r);
				int k = qAlpha(r);
				if (pl == -1)
					{
					ImgStr += c;
					ImgStr += m;
					ImgStr += y;
					ImgStr += k;
					}
				else
					{
					if (pl == -2)
						ImgStr += QMIN(255, qRound(0.3 * c + 0.59 * m + 0.11 * y + k));
					if (pl == 1)
						ImgStr += c;
					if (pl == 2)
						ImgStr += m;
					if (pl == 3)
						ImgStr += y;
					if (pl == 0)
						ImgStr += k;
					}
				}
			}
		}
	else
		{
		for( int yi=0; yi < h; ++yi )
			{
			QRgb * s = (QRgb*)(im->scanLine( yi ));
			for( int xi=0; xi < w; ++xi )
				{
				QRgb r=*s++;
				int c = 255 - qRed(r);
				int m = 255 - qGreen(r);
				int y = 255 - qBlue(r);
				int k = QMIN(QMIN(c, m), y);
				if (pl == -1)
					{
					ImgStr += c - k;
					ImgStr += m - k;
					ImgStr += y - k;
					ImgStr += k;
					}
				else
					{
					if (pl == -2)
						ImgStr += QMIN(255, qRound(0.3 * c + 0.59 * m + 0.11 * y + k));
					if (pl == 1)
						ImgStr += c - k;
					if (pl == 2)
						ImgStr += m - k;
					if (pl == 3)
						ImgStr += y - k;
					if (pl == 0)
						ImgStr += k;
					}
				}
			}
		}
	return ImgStr;
}

QString MaskToTxt(QImage *im, bool PDF)
{
	int h = im->height();
	int w = im->width();
  int w2;
  w2 = w / 8;
  if ((w % 8) != 0)
  	w2++;
	QString ImgStr = "";
	for( int yi=0; yi < h; ++yi )
		{
		uchar * s = im->scanLine( yi );
		for( int xi=0; xi < w2; ++xi )
			{
			unsigned char u = *(s+xi);
			if (PDF)
				ImgStr += ~u;
			else
				ImgStr += u;
			}
		}
	return ImgStr;
}

QString CompressStr(QString *in)
{
	QString out = "";
#ifdef HAVE_LIBZ
	QByteArray bb(in->length());
	for (uint ax = 0; ax < in->length(); ++ax)
		{
		bb[ax] = uchar(QChar(in->at(ax)));
		}
	uLong exlen = uint(bb.size() * 0.001 + 16) + bb.size();
	QByteArray bc(exlen);
	compress2((Byte *)bc.data(), &exlen, (Byte *)bb.data(), uLong(bb.size()), 9);
	for (uint cl = 0; cl < exlen; ++cl)
		{
		out += bc[cl];
		}
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
	bool ende = true;
	uint dcoun = 0;
	uint dcoun2 = 0;
	while (ende)
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
		{
		Ndir += "../";
		}
	for (uint ddx = dcoun2; ddx < Bdir.count(); ddx++)
		{
		Ndir += Bdir[ddx]+"/";
		}
	Ndir += Bfi.fileName();
	return Ndir;
}

bool GlyNames(QMap<uint, QString> *GList, QString Dat)
{
	bool error;
	char *buf[50];
	FT_Library library;
	FT_Face face;
	FT_ULong  charcode;
  FT_UInt   gindex;
	error = FT_Init_FreeType(&library);
	error = FT_New_Face(library, Dat, 0, &face);
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
	composite->addPoint(FPoint(tox, toy));
	composite->addPoint(FPoint(tox, toy));
	firstP = FPoint(tox, toy);
	return 0;
}

int traceLineto( FT_Vector *to, FPointArray *composite )
{
	double tox = ( to->x / 64.0 );
	double toy = ( to->y / 64.0 );
	composite->addPoint(FPoint(tox, toy));
	composite->addPoint(FPoint(tox, toy));
	composite->addPoint(FPoint(tox, toy));
	composite->addPoint(FPoint(tox, toy));
	return 0;
}

int traceQuadraticBezier( FT_Vector *control, FT_Vector *to, FPointArray *composite )
{
	double x1 = ( control->x / 64.0 );
	double y1 = ( control->y / 64.0 );
	double x2 = ( to->x / 64.0 );
	double y2 = ( to->y / 64.0 );
	composite->addPoint(FPoint(x2, y2));
	composite->addPoint(FPoint(x1, y1));
	composite->addPoint(FPoint(x2, y2));
	composite->addPoint(FPoint(x2, y2));
	return 0;
};

int traceCubicBezier( FT_Vector *p, FT_Vector *q, FT_Vector *to, FPointArray *composite )
{
	double x1 = ( p->x / 64.0 );
	double y1 = ( p->y / 64.0 );
	double x2 = ( q->x / 64.0 );
	double y2 = ( q->y / 64.0 );
	double x3 = ( to->x / 64.0 );
	double y3 = ( to->y / 64.0 );
	composite->setPoint(composite->size()-1, FPoint(x1, y1));
	composite->addPoint(FPoint(x3, y3));
	composite->addPoint(FPoint(x2, y2));
	composite->addPoint(FPoint(x3, y3));
	composite->addPoint(FPoint(x3, y3));
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

FPointArray traceChar(FT_Face face, uint chr, int chs, double *x, double *y)
{
	FT_UInt glyphIndex;
	FPointArray pts, pts2;
	pts.resize(0);
	firstP = FPoint(0,0);
	FirstM = true;
	FT_Set_Char_Size(	face, 0, chs*64, 72, 72 );
	glyphIndex = FT_Get_Char_Index(face, chr);
	FT_Load_Glyph( face, glyphIndex, FT_LOAD_NO_HINTING | FT_LOAD_NO_BITMAP );
	FT_Outline_Decompose(&face->glyph->outline, &OutlineMethods, reinterpret_cast<void*>(&pts));
	QWMatrix ma;
	ma.scale(1, -1);
	pts.map(ma);
	pts.translate(0, chs);
	pts2.resize(0);
	pts2.putPoints(0, pts.size()-2, pts, 0);
	*x = face->glyph->metrics.horiBearingX / 64.0;
	*y = face->glyph->metrics.horiBearingY / 64.0;
	return pts2;
}
