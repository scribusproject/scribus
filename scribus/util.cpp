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
extern cmsHTRANSFORM stdTrans;
extern cmsHTRANSFORM stdProof;
extern cmsHTRANSFORM stdTransImg;
extern cmsHTRANSFORM stdProofImg;
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
void WordAndPara(PageItem* b, int *w, int *p, int *c, int *wN, int *pN, int *cN);
void CopyPageItem(struct CopyPasteBuffer *Buffer, PageItem *b);
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
QString ImageToTxt(QImage *im);
QString ImageToCMYK(QImage *im);
QString ImageToGray(QImage *im);
QString ImageToCMYK_PS(QImage *im, int pl, bool pre);
QString ImageToCMYK_PDF(QImage *im, bool pre);
void Convert2JPG(QString fn, QImage *image, int Quality, bool isCMYK, bool isGray);
QString MaskToTxt(QImage *im, bool PDF = true);
QString MaskToTxt14(QImage *im);
void Level2Layer(ScribusDoc *currentDoc, struct Layer *ll, int Level);
void BezierPoints(QPointArray *ar, QPoint n1, QPoint n2, QPoint n3, QPoint n4);
double xy2Deg(double x, double y);
QPointArray FlattenPath(FPointArray ina, QValueList<uint> &Segs);
QPointArray RegularPolygon(double w, double h, uint c, bool star, double factor, double rota);
FPointArray RegularPolygonF(double w, double h, uint c, bool star, double factor, double rota);
QPixmap loadIcon(QString nam);
uint getDouble(QString in, bool raw);
bool loadText(QString nam, QString *Buffer);
double Cwidth(ScribusDoc *currentDoc, QString name, QString ch, int Siz, QString ch2 = " ");
double RealCWidth(ScribusDoc *currentDoc, QString name, QString ch, int Siz);
double QStodouble(QString in);
int QStoInt(QString in);
QString GetAttr(QDomElement *el, QString at, QString def="0");
#ifdef HAVE_CMS
QImage ProofPict(QImage *Im, QString Prof, int Rend, cmsHPROFILE emPr=0);
#else
QImage ProofPict(QImage *Im, QString Prof, int Rend);
#endif
QImage ProofImage(QImage *Im);
QImage LoadPicture(QString fn, QString Prof, int rend, bool useEmbedded, bool useProf, int requestType, int gsRes, bool *realCMYK = 0, ImageInfoRecord *info = 0);
QString getAlpha(QString fn, bool PDF, bool pdf14);
int System(const QStringList & args);
int callGS(const QStringList & args_in);
int copyFile(QString source, QString target);
int moveFile(QString source, QString target);

typedef struct my_error_mgr
{
	struct jpeg_error_mgr pub;            /* "public" fields */
	jmp_buf setjmp_buffer;  /* for return to caller */
}
*my_error_ptr;

/*
 * Here's the routine that will replace the standard error_exit method:
 */

static void my_error_exit (j_common_ptr cinfo)
{
	my_error_ptr myerr = (my_error_ptr) cinfo->err;
	(*cinfo->err->output_message) (cinfo);
	longjmp (myerr->setjmp_buffer, 1);
}

void Convert2JPG(QString fn, QImage *image, int Quality, bool isCMYK, bool isGray)
{
	struct jpeg_compress_struct cinfo;
	struct my_error_mgr         jerr;
	FILE     *outfile;
	JSAMPROW row_pointer[1];
	row_pointer[0] = 0;
	cinfo.err = jpeg_std_error (&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
	outfile = NULL;
	if (setjmp (jerr.setjmp_buffer))
	{
		jpeg_destroy_compress (&cinfo);
		if (outfile)
			fclose (outfile);
		return;
	}
	jpeg_create_compress (&cinfo);
	if ((outfile = fopen (fn, "wb")) == NULL)
		return;
	jpeg_stdio_dest (&cinfo, outfile);
	cinfo.image_width  = image->width();
	cinfo.image_height = image->height();
	if (isCMYK)
	{
		cinfo.in_color_space = JCS_CMYK;
		cinfo.input_components = 4;
	}
	else
	{
		if (isGray)
		{
			cinfo.in_color_space = JCS_GRAYSCALE;
			cinfo.input_components = 1;
		}
		else
		{
			cinfo.in_color_space = JCS_RGB;
			cinfo.input_components = 3;
		}
	}
	jpeg_set_defaults (&cinfo);
	int qual[] = { 95, 85, 75, 50, 25 };  // These are the JPEG Quality settings 100 means best, 0 .. don't discuss
	jpeg_set_quality (&cinfo, qual[Quality], true);
	jpeg_start_compress (&cinfo, TRUE);
	row_pointer[0] = new uchar[cinfo.image_width*cinfo.input_components];
	int w = cinfo.image_width;
	while (cinfo.next_scanline < cinfo.image_height)
	{
		uchar *row = row_pointer[0];
		if (isCMYK)
		{
			QRgb* rgba = (QRgb*)image->scanLine(cinfo.next_scanline);
			for (int i=0; i<w; ++i)
			{
				*row++ = qAlpha(*rgba);
				*row++ = qRed(*rgba);
				*row++ = qGreen(*rgba);
				*row++ = qBlue(*rgba);
				++rgba;
			}
		}
		else
		{
			if (isGray)
			{
				QRgb* rgba = (QRgb*)image->scanLine(cinfo.next_scanline);
				for (int i=0; i<w; ++i)
				{
					*row++ = qRed(*rgba);
					++rgba;
				}
			}
			else
			{
				QRgb* rgb = (QRgb*)image->scanLine(cinfo.next_scanline);
				for (int i=0; i<w; i++)
				{
					*row++ = qRed(*rgb);
					*row++ = qGreen(*rgb);
					*row++ = qBlue(*rgb);
					++rgb;
				}
			}
		}
		jpeg_write_scanlines (&cinfo, row_pointer, 1);
	}
	jpeg_finish_compress (&cinfo);
	fclose (outfile);
	jpeg_destroy_compress (&cinfo);
	delete [] row_pointer[0];
}

enum PSDColorMode
{
    CM_BITMAP = 0,
    CM_GRAYSCALE = 1,
    CM_INDEXED = 2,
    CM_RGB = 3,
    CM_CMYK = 4,
    CM_MULTICHANNEL = 7,
    CM_DUOTONE = 8,
    CM_LABCOLOR = 9
};

struct PSDHeader
{
	uint signature;
	ushort version;
	uchar reserved[6];
	ushort channel_count;
	uint height;
	uint width;
	ushort depth;
	ushort color_mode;
};

static QDataStream & operator>> ( QDataStream & s, PSDHeader & header )
{
	s >> header.signature;
	s >> header.version;
	for( int i = 0; i < 6; i++ )
	{
		s >> header.reserved[i];
	}
	s >> header.channel_count;
	s >> header.height;
	s >> header.width;
	s >> header.depth;
	s >> header.color_mode;
	return s;
}

// Check that the header is a valid PSD.
static bool IsValid( const PSDHeader & header )
{
	if( header.signature != 0x38425053 )
		return false;
	return true;
}

// Check that the header is supported.
static bool IsSupported( const PSDHeader & header )
{
	if ( header.version != 1 )
		return false;
	if ( header.channel_count > 16 )
		return false;
	if ( header.depth != 8 )
		return false;
	if (( header.color_mode == CM_RGB ) || (header.color_mode == CM_CMYK))
		return true;
	return false;
}

static unsigned char INT_MULT ( unsigned char a, unsigned char b )
{
	int c = a * b + 0x80;
	return (unsigned char)(( ( c >> 8 ) + c ) >> 8);
}

/*!
 * Convert a color in RGB space to HSV space (Hue, Saturation, Value).
 * \param red the red component (modified in place).
 * \param green the green component (modified in place).
 * \param blue the blue component (modified in place).
 */
static void RGBTOHSV ( uchar& red, uchar& green, uchar& blue )
{
	int r, g, b;
	double h, s, v;
	int min, max;
	h = 0.;
	r = red;
	g = green;
	b = blue;
	if ( r > g )
	{
		max = QMAX( r, b );
		min = QMIN( g, b );
	}
	else
	{
		max = QMAX( g, b );
		min = QMIN( r, b );
	}
	v = max;
	if ( max != 0 )
		s = ( ( max - min ) * 255 ) / (double)max;
	else
		s = 0;
	if ( s == 0 )
		h = 0;
	else
	{
		int delta = max - min;
		if ( r == max )
			h = ( g - b ) / (double)delta;
		else if ( g == max )
			h = 2 + ( b - r ) / (double)delta;
		else if ( b == max )
			h = 4 + ( r - g ) / (double)delta;
		h *= 42.5;
		if ( h < 0 )
			h += 255;
		if ( h > 255 )
			h -= 255;
	}
	red   = (uchar)h;
	green = (uchar)s;
	blue  = (uchar)v;
}

/*!
 * Convert a color in HSV space to RGB space.
 * \param hue the hue component (modified in place).
 * \param saturation the saturation component (modified in place).
 * \param value the value component (modified in place).
 */
static void HSVTORGB ( uchar& hue, uchar& saturation, uchar& value )
{
	if ( saturation == 0 )
	{
		hue        = value;
		saturation = value;
		value      = value;
	}
	else
	{
		double h = hue * 6. / 255.;
		double s = saturation / 255.;
		double v = value / 255.;

		double f = h - (int)h;
		double p = v * ( 1. - s );
		double q = v * ( 1. - ( s * f ) );
		double t = v * ( 1. - ( s * ( 1. - f ) ) );
		// Worth a note here that gcc 2.96 will generate different results
		// depending on optimization mode on i386.
		switch ((int)h)
		{
		case 0:
			hue        = (uchar)( v * 255 );
			saturation = (uchar)( t * 255 );
			value      = (uchar)( p * 255 );
			break;
		case 1:
			hue        = (uchar)( q * 255 );
			saturation = (uchar)( v * 255 );
			value      = (uchar)( p * 255 );
			break;
		case 2:
			hue        = (uchar)( p * 255 );
			saturation = (uchar)( v * 255 );
			value      = (uchar)( t * 255 );
			break;
		case 3:
			hue        = (uchar)( p * 255 );
			saturation = (uchar)( q * 255 );
			value      = (uchar)( v * 255 );
			break;
		case 4:
			hue        = (uchar)( t * 255 );
			saturation = (uchar)( p * 255 );
			value      = (uchar)( v * 255 );
			break;
		case 5:
			hue        = (uchar)( v * 255 );
			saturation = (uchar)( p * 255 );
			value      = (uchar)( q * 255 );
		}
	}
}

/*!
 * Convert a color in RGB space to HLS space (Hue, Lightness, Saturation).
 * \param red the red component (modified in place).
 * \param green the green component (modified in place).
 * \param blue the blue component (modified in place).
 */
static void RGBTOHLS ( uchar& red, uchar& green, uchar& blue )
{
	int r = red;
	int g = green;
	int b = blue;
	int min, max;
	if ( r > g )
	{
		max = QMAX( r, b );
		min = QMIN( g, b );
	}
	else
	{
		max = QMAX( g, b );
		min = QMIN( r, b );
	}
	double h;
	double l = ( max + min ) / 2.;
	double s;
	if ( max == min )
	{
		s = 0.;
		h = 0.;
	}
	else
	{
		int delta = max - min;
		if ( l < 128 )
			s = 255 * (double)delta / (double)( max + min );
		else
			s = 255 * (double)delta / (double)( 511 - max - min );
		if ( r == max )
			h = ( g - b ) / (double)delta;
		else if ( g == max )
			h = 2 + ( b - r ) / (double)delta;
		else
			h = 4 + ( r - g ) / (double)delta;
		h *= 42.5;
		if ( h < 0 )
			h += 255;
		else if ( h > 255 )
			h -= 255;
	}
	red   = (uchar)h;
	green = (uchar)l;
	blue  = (uchar)s;
}

/*!
 * Implement the HLS "double hex-cone".
 * \param n1 lightness fraction (?)
 * \param n2 saturation fraction (?)
 * \param hue hue "angle".
 * \return HLS value.
 */
static int HLSVALUE ( double n1, double n2, double hue )
{
	double value;
	if ( hue > 255 )
		hue -= 255;
	else if ( hue < 0 )
		hue += 255;
	if ( hue < 42.5 )
		value = n1 + ( n2 - n1 ) * ( hue / 42.5 );
	else if ( hue < 127.5 )
		value = n2;
	else if ( hue < 170 )
		value = n1 + ( n2 - n1 ) * ( ( 170 - hue ) / 42.5 );
	else
		value = n1;
	return (int)( value * 255 );
}

/*!
 * Convert a color in HLS space to RGB space.
 * \param hue the hue component (modified in place).
 * \param lightness the lightness component (modified in place).
 * \param saturation the saturation component (modified in place).
 */
static void HLSTORGB ( uchar& hue, uchar& lightness, uchar& saturation )
{
	double h = hue;
	double l = lightness;
	double s = saturation;
	if ( s == 0 )
	{
		hue        = (uchar)l;
		lightness  = (uchar)l;
		saturation = (uchar)l;
	}
	else
	{
		double m1, m2;
		if ( l < 128 )
			m2 = ( l * ( 255 + s ) ) / 65025.;
		else
			m2 = ( l + s - ( l * s ) / 255. ) / 255.;
		m1 = ( l / 127.5 ) - m2;
		hue        = HLSVALUE( m1, m2, h + 85 );
		lightness  = HLSVALUE( m1, m2, h );
		saturation = HLSVALUE( m1, m2, h - 85 );
	}
}

static bool loadLayerChannels( QDataStream & s, const PSDHeader & header, QImage & img, QValueList<PSDLayer> &layerInfo, uint layer, bool* firstLayer, ImageInfoRecord *info )
{
	// Find out if the data is compressed.
	// Known values:
	//   0: no compression
	//   1: RLE compressed
	QImage tmpImg;
	if( !tmpImg.create( header.width, header.height, 32 ))
		return false;
	tmpImg.setAlphaBuffer( true );
	tmpImg.fill(qRgba(0, 0, 0, 0));
	uint base = s.device()->at();
	uchar cbyte;
	ushort compression;
	s >> compression;
	if( compression > 1 )
		return false;
	uint channel_num = layerInfo[layer].channelLen.count();
	channel_num = QMIN(channel_num, 39);
	uint components[40];
	for(uint channel = 0; channel < channel_num; channel++)
	{
		switch(layerInfo[layer].channelType[channel])
		{
		case 0:
			components[channel] = 2;
			break;
		case 1:
			components[channel] = 1;
			break;
		case 2:
			components[channel] = 0;
			break;
		case 3:
			components[channel] = 3;
			break;
		case -1:
		case -2:
			components[channel] = 3;
			break;
		}
	}
	if( compression )
	{
		// Skip row lengths.
		s.device()->at( s.device()->at() + layerInfo[layer].height * 2 );
		// Read RLE data
		bool first = true;
		for(uint channel = 0; channel < channel_num; channel++)
		{
			if ((layerInfo[layer].channelType[channel] < 0) && (header.color_mode == CM_CMYK))
			{
				first = false;
				continue;
			}
			if (!first)
			{
				s.device()->at(layerInfo[layer].channelLen[channel-1]+base);
				base += layerInfo[layer].channelLen[channel-1];
				s >> compression;
				s.device()->at( s.device()->at() + layerInfo[layer].height * 2 );
			}
			first = false;
			uint pixel_count = layerInfo[layer].width;
			for (int hh = 0; hh < layerInfo[layer].height; hh++)
			{
				uint count = 0;
				uchar *ptr = tmpImg.scanLine(QMIN(QMAX(layerInfo[layer].ypos, 0)+hh, static_cast<int>(tmpImg.height()-1)));
				uchar *ptr2 = ptr+tmpImg.width() * 4;
				ptr += QMAX(layerInfo[layer].xpos, 0) * 4;
				ptr += components[channel];
				while( count < pixel_count )
				{
					uchar c;
					s >> c;
					uint len = c;
					if( len < 128 )
					{
						// Copy next len+1 bytes literally.
						len++;
						count += len;
						while( len != 0 )
						{
							s >> cbyte;
							if (ptr < ptr2)
							{
								if (header.color_mode == CM_CMYK)
									cbyte = 255 - cbyte;
								*ptr = cbyte;
							}
							ptr += 4;
							len--;
						}
					}
					else if( len > 128 )
					{
						// Next -len+1 bytes in the dest are replicated from next source byte.
						// (Interpret len as a negative 8-bit int.)
						len ^= 0xFF;
						len += 2;
						count += len;
						uchar val;
						s >> val;
						if (header.color_mode == CM_CMYK)
							val = 255 - val;
						while( len != 0 )
						{
							if (ptr < ptr2)
								*ptr = val;
							ptr += 4;
							len--;
						}
					}
					else if( len == 128 )
					{
						// No-op.
					}
				}
			}
		}
	}
	else
	{
		// We're at the raw image data.  It's each channel in order (Red, Green, Blue, Alpha, ...)
		// where each channel consists of an 8-bit value for each pixel in the image.
		// Read the data by channel.
		int count = 0;
		for(uint channel = 0; channel < channel_num; channel++)
		{
			count += layerInfo[layer].channelLen[channel];
		}
		uchar * ptr = tmpImg.bits();
		if (channel_num < 4)
			tmpImg.fill(qRgba(0, 0, 0, 255));
		while( count >= 0 )
		{
			for(uint channel = 0; channel < channel_num; channel++)
			{
				s >> cbyte;
				count--;
				if (header.color_mode == CM_CMYK)
					cbyte = 255 - cbyte;
				if (channel < 4)
					ptr[components[channel]] = cbyte;
			}
			ptr += 4;
		}
	}
	QImage tmpImg2 = tmpImg.copy();
	if (header.color_mode == CM_CMYK)
	{
		for (int i = 0; i < tmpImg.height(); i++)
		{
			unsigned int *ptr = (unsigned int *) tmpImg.scanLine(i);
			unsigned int *ptr2 = (unsigned int *) tmpImg2.scanLine(i);
			unsigned char r, g, b;
			for (int j = 0; j < tmpImg.width(); j++)
			{
				unsigned char *p = (unsigned char *) ptr;
				unsigned char *p2 = (unsigned char *) ptr2;
				r = 255 - QMIN(255, p[0] + p[3]);
				g = 255 - QMIN(255, p[1] + p[3]);
				b = 255 - QMIN(255, p[2] + p[3]);
				p2[0] = r;
				p2[1] = g;
				p2[2] = b;
				p2[3] = 255;
				ptr++;
				ptr2++;
			}
		}
	}
	QImage imt;
	double sx = tmpImg.width() / 40.0;
	double sy = tmpImg.height() / 40.0;
	imt = sy < sx ?  tmpImg2.smoothScale(qRound(tmpImg.width() / sx), qRound(tmpImg.height() / sx)) :
							tmpImg2.smoothScale(qRound(tmpImg.width() / sy), qRound(tmpImg.height() / sy));
	layerInfo[layer].thumb = imt.copy();
	bool visible = !(layerInfo[layer].flags & 2);
	if ((info->isRequest) && (info->RequestProps.contains(layer)))
		visible = info->RequestProps[layer].visible;
	if (visible)
	{
		if (*firstLayer)
		{
			img = tmpImg.copy();
			*firstLayer = false;
		}
		else
		{
			for (int i = 0; i < layerInfo[layer].height; i++)
			{
				unsigned int *dst = (unsigned int *)img.scanLine(QMIN(QMAX(layerInfo[layer].ypos, 0)+i, img.height()-1));
				unsigned int *src = (unsigned int *)tmpImg.scanLine(QMIN(QMAX(layerInfo[layer].ypos, 0)+i, tmpImg.height()-1));
				dst += QMAX(layerInfo[layer].xpos, 0);
				src += QMAX(layerInfo[layer].xpos, 0);
				unsigned int adj = 0;
				if (layerInfo[layer].xpos < 0)
					adj = abs(layerInfo[layer].xpos);
				unsigned char r, g, b, a, src_r, src_g, src_b, src_a;
				for (unsigned int j = 0; j < layerInfo[layer].width-adj; j++)
				{
					unsigned char *d = (unsigned char *) dst;
					unsigned char *s = (unsigned char *) src;
					src_r = s[0];
					src_g = s[1];
					src_b = s[2];
					src_a = s[3];
					QString layBlend = layerInfo[layer].blend;
					if ((info->isRequest) && (info->RequestProps.contains(layer)))
						layBlend = info->RequestProps[layer].blend;
					if (layBlend == "mul ")
					{
						src_r = INT_MULT(src_r, d[0]);
						src_g = INT_MULT(src_g, d[1]);
						src_b = INT_MULT(src_b, d[2]);
						if (header.color_mode == CM_CMYK)
							src_a = INT_MULT(src_a, d[3]);
						else
							src_a = QMIN(src_a, d[3]);
					}
					else if (layBlend == "scrn")
					{
						src_r = 255 - INT_MULT(255 - d[0], 255 - src_r);
						src_g = 255 - INT_MULT(255 - d[1], 255 - src_g);
						src_b = 255 - INT_MULT(255 - d[2], 255 - src_b);
						if (header.color_mode == CM_CMYK)
							src_a = 255 - INT_MULT(255 - d[3], 255 - src_a);
						else
							src_a = QMIN(src_a, d[3]);
					}
					else if (layBlend == "over")
					{
						src_r = INT_MULT(d[0], d[0] + INT_MULT(2 * src_r, 255 - d[0]));
						src_g = INT_MULT(d[1], d[1] + INT_MULT(2 * src_g, 255 - d[1]));
						src_b = INT_MULT(d[2], d[2] + INT_MULT(2 * src_b, 255 - d[2]));
						if (header.color_mode == CM_CMYK)
							src_a = INT_MULT(d[3], d[3] + INT_MULT(2 * src_a, 255 - d[3]));
						else
							src_a = QMIN(src_a, d[3]);
					}
					else if (layBlend == "diff")
					{
						src_r = d[0] > src_r ? d[0] - src_r : src_r - d[0];
						src_g = d[1] > src_g ? d[1] - src_g : src_g - d[1];
						src_b = d[2] > src_b ? d[2] - src_b : src_b - d[2];
						if (header.color_mode == CM_CMYK)
							src_a = d[3] > src_a ? d[3] - src_a : src_a - d[3];
						else
							src_a = QMIN(src_a, d[3]);
					}
					else if (layBlend == "dark")
					{
						src_r = d[0]  < src_r ? d[0]  : src_r;
						src_g = d[1] < src_g ? d[1] : src_g;
						src_b = d[2] < src_b ? d[2] : src_b;
						if (header.color_mode == CM_CMYK)
							src_a = d[3] < src_a ? d[3] : src_a;
						else
							src_a = QMIN( src_a, d[3] );
					}
					else if (layBlend == "lite")
					{
						src_r = d[0] < src_r ? src_r : d[0];
						src_g = d[1] < src_g ? src_g : d[1];
						src_b = d[2] < src_b ? src_b : d[2];
						if (header.color_mode == CM_CMYK)
							src_a = d[3] < src_a ? src_a : d[3];
						else
							src_a = QMIN( src_a, d[3] );
					}
					else if (layBlend == "hue ")
					{
						if (header.color_mode != CM_CMYK)
						{
							uchar new_r = d[0];
							uchar new_g = d[1];
							uchar new_b = d[2];
							RGBTOHSV(src_r, src_g, src_b);
							RGBTOHSV(new_r, new_g, new_b);
							new_r = src_r;
							HSVTORGB(new_r, new_g, new_b);
							src_r = new_r;
							src_g = new_g;
							src_b = new_b;
							src_a = QMIN( src_a, d[3] );
						}
					}
					else if (layBlend == "sat ")
					{
						if (header.color_mode != CM_CMYK)
						{
							uchar new_r = d[0];
							uchar new_g = d[1];
							uchar new_b = d[2];
							RGBTOHSV(src_r, src_g, src_b);
							RGBTOHSV(new_r, new_g, new_b);
							new_g = src_g;
							HSVTORGB(new_r, new_g, new_b);
							src_r = new_r;
							src_g = new_g;
							src_b = new_b;
							src_a = QMIN(src_a, d[3]);
						}
					}
					else if (layBlend == "lum ")
					{
						if (header.color_mode != CM_CMYK)
						{
							uchar new_r = d[0];
							uchar new_g = d[1];
							uchar new_b = d[2];
							RGBTOHSV(src_r, src_g, src_b);
							RGBTOHSV(new_r, new_g, new_b);
							new_b = src_b;
							HSVTORGB(new_r, new_g, new_b);
							src_r = new_r;
							src_g = new_g;
							src_b = new_b;
							src_a = QMIN(src_a, d[3]);
						}
					}
					else if (layBlend == "colr")
					{
						if (header.color_mode != CM_CMYK)
						{
							uchar new_r = d[0];
							uchar new_g = d[1];
							uchar new_b = d[2];
							RGBTOHLS(src_r, src_g, src_b);
							RGBTOHLS(new_r, new_g, new_b);
							new_r = src_r;
							new_b = src_b;
							HLSTORGB(new_r, new_g, new_b);
							src_r = new_r;
							src_g = new_g;
							src_b = new_b;
							src_a = QMIN(src_a, d[3]);
						}
					}
					int layOpa = layerInfo[layer].opacity;
					if ((info->isRequest) && (info->RequestProps.contains(layer)))
						layOpa = info->RequestProps[layer].opacity;
					r = (d[0] * (255 - layOpa) + src_r * layOpa) / 255;
					g = (d[1] * (255 - layOpa) + src_g * layOpa) / 255;
					b = (d[2] * (255 - layOpa) + src_b * layOpa) / 255;
					a = (d[3] * (255 - layOpa) + src_a * layOpa) / 255;
					if (header.color_mode == CM_CMYK)
					{
						d[0] = r;
						d[1] = g;
						d[2] = b;
						d[3] = a;
					}
					else
					{
						if (s[3] > 0)
						{
							d[0] = r;
							d[1] = g;
							d[2] = b;
						}
					}
					dst++;
					src++;
				}
			}
		}
	}
	return true;
}

static bool loadLayer( QDataStream & s, const PSDHeader & header, QImage & img )
{
	// Find out if the data is compressed.
	// Known values:
	//   0: no compression
	//   1: RLE compressed
	ushort compression;
	uchar cbyte;
	s >> compression;
	if( compression > 1 )
	{
		// Unknown compression type.
		return false;
	}
	uint channel_num = header.channel_count;
	// Clear the image.
	/*	if( channel_num < 4 )
		{
			img.fill(qRgba(0, 0, 0, 0xFF));
		}
		else
		{ */
	// Enable alpha.
	//		img.setAlphaBuffer( true );
	// Ignore the other channels.
	channel_num = 4;
	//	}
	const uint pixel_count = header.height * header.width;
	static const uint components[4] = {2, 1, 0, 3}
	                                  ; // @@ Is this endian dependant?
	if( compression )
	{
		// Skip row lengths.
		ushort w;
		for(uint i = 0; i < header.height * header.channel_count; i++)
		{
			s >> w;
		}
		// Read RLE data.
		for(uint channel = 0; channel < channel_num; channel++)
		{
			uchar * ptr = img.bits() + components[channel];
			uint count = 0;
			while( count < pixel_count )
			{
				uchar c;
				s >> c;
				uint len = c;
				if( len < 128 )
				{
					// Copy next len+1 bytes literally.
					len++;
					count += len;
					while( len != 0 )
					{
						if (header.color_mode == CM_CMYK)
						{
							s >> cbyte;
							*ptr = 255 - cbyte;
						}
						else
							s >> *ptr;
						ptr += 4;
						len--;
					}
				}
				else if( len > 128 )
				{
					// Next -len+1 bytes in the dest are replicated from next source byte.
					// (Interpret len as a negative 8-bit int.)
					len ^= 0xFF;
					len += 2;
					count += len;
					uchar val;
					s >> val;
					if (header.color_mode == CM_CMYK)
						val = 255 - val;
					while( len != 0 )
					{
						*ptr = val;
						ptr += 4;
						len--;
					}
				}
				else if( len == 128 )
				{
					// No-op.
				}
			}
		}
	}
	else
	{
		// We're at the raw image data.  It's each channel in order (Red, Green, Blue, Alpha, ...)
		// where each channel consists of an 8-bit value for each pixel in the image.
		// Read the data by channel.
		for(uint channel = 0; channel < channel_num; channel++)
		{
			uchar * ptr = img.bits() + components[channel];
			// Read the data.
			uint count = pixel_count;
			while( count != 0 )
			{
				if (header.color_mode == CM_CMYK)
				{
					s >> cbyte;
					*ptr = 255 - cbyte;
				}
				else
					s >> *ptr;
				ptr += 4;
				count--;
			}
		}
	}
	return true;
}

static QString getLayerString(QDataStream & s)
{
	uchar len, tmp;
	uint adj;
	QString ret = "";
	s >> len;
	if (len == 0)
	{
		s >> tmp;
		s >> tmp;
		s >> tmp;
		return ret;
	}
	for( int i = 0; i < len; i++ )
	{
		s >> tmp;
		ret += QChar(tmp);
	}
	adj = 0;
	if (((ret.length()+1) % 4) != 0)
		adj = 4 - ((ret.length()+1) % 4);
	s.device()->at( s.device()->at() + adj );
	return ret;
}

static QString getPascalString(QDataStream & s)
{
	uchar len, tmp;
	uint adj;
	QString ret = "";
	s >> len;
	if (len == 0)
	{
		s >> tmp;
		return ret;
	}
	for( int i = 0; i < len; i++ )
	{
		s >> tmp;
		ret += QChar(tmp);
	}
	adj = (ret.length()+1) % 2;
	s.device()->at( s.device()->at() + adj );
	return ret;
}

static void parseRessourceData( QDataStream & s, const PSDHeader & header, uint size, ImageInfoRecord *info )
{
	uint signature, resSize, offset, resBase, vRes, hRes, adj;
	ushort resID, hResUnit, vResUnit, dummyW;
	QString resName;
	uchar filler;
	offset = 0;
	bool first = false;
	bool pathOpen = false;
	FPoint firstPoint, firstControl;
	FPointArray clip2;
	while (offset < size)
	{
		s >> signature;
		offset += 4;
		s >> resID;
		offset += 2;
		adj = s.device()->at();
		resName = getPascalString(s);
		offset += s.device()->at() - adj;
		s >> resSize;
		resBase = s.device()->at();
		if ( (resID >= 0x07d0) && (resID <= 0x0bb6) )
		{
			QString db1, db2;
			short type;
			uint data1, data2, data3, data4, data5, data6;
			double frac1, frac2, frac3, frac4, frac5, frac6;
			ushort man1, man2, man3, man4, man5, man6;
			uint offset2;
			offset2 = 0;
			first = false;
			pathOpen = false;
			clip2.resize(0);
			while (offset2 < resSize)
			{
				s >> type;
				s >> data1;
				frac1 = (data1 & 0x00FFFFFF) / 16777215.0;
				man1 = (data1 & 0xFF000000) >> 24;
				s >> data2;
				frac2 = (data2 & 0x00FFFFFF) / 16777215.0;
				man2 = (data2 & 0xFF000000) >> 24;
				s >> data3;
				frac3 = (data3 & 0x00FFFFFF) / 16777215.0;
				man3 = (data3 & 0xFF000000) >> 24;
				s >> data4;
				frac4 = (data4 & 0x00FFFFFF) / 16777215.0;
				man4 = (data4 & 0xFF000000) >> 24;
				s >> data5;
				frac5 = (data5 & 0x00FFFFFF) / 16777215.0;
				man5 = (data5 & 0xFF000000) >> 24;
				s >> data6;
				frac6 = (data6 & 0x00FFFFFF) / 16777215.0;
				man6 = (data6 & 0xFF000000) >> 24;
				switch (type)
				{
					case 0:
					case 3:
						if (pathOpen)
						{
							clip2.addPoint(firstPoint);
							clip2.addPoint(firstControl);
							clip2.setMarker();
						}
						pathOpen = false;
						first = true;
						break;
					case 1:
						if (first)
						{
							firstControl = FPoint(frac2 * header.width, frac1 * header.height);
							firstPoint = FPoint(frac4 * header.width, frac3 * header.height);
							clip2.addPoint(FPoint(frac4 * header.width, frac3 * header.height));
							clip2.addPoint(FPoint(frac6 * header.width, frac5 * header.height));
						}
						else
						{
							clip2.addPoint(FPoint(frac4 * header.width, frac3 * header.height));
							clip2.addPoint(FPoint(frac2 * header.width, frac1 * header.height));
							clip2.addPoint(FPoint(frac4 * header.width, frac3 * header.height));
							clip2.addPoint(FPoint(frac6 * header.width, frac5 * header.height));
						}
						pathOpen = true;
						first = false;
						break;
					case 6:
						first = true;
						break;
					default:
						break;
				}
				offset2 += 26;
			}
			clip2.addPoint(firstPoint);
			clip2.addPoint(firstControl);
			info->PDSpathData.insert(resName, clip2.copy());
		}
		 else
		 {
			switch (resID)
			{
				case 0x0bb7:
					adj = s.device()->at();
					info->clipPath = getPascalString(s);
					offset += s.device()->at() - adj;
					break;
				case 0x03ed:
					s >> hRes;
					s >> hResUnit;
					s >> dummyW;
					s >> vRes;
					s >> vResUnit;
					s >> dummyW;
					info->xres = qRound(hRes / 65536.0);
					info->yres = qRound(vRes / 65536.0);
					break;
				default:
					break;
			}
		}
		s.device()->at( resBase + resSize );
		offset += resSize;
		if (resSize & 1)
		{
			s >> filler;
			offset += 1;
		}
	}
}

static bool parseLayer( QDataStream & s, const PSDHeader & header, QImage & img, ImageInfoRecord *info )
{
	uint addRes, layerinfo, channelLen, signature, extradata, layermasksize, layerRange, dummy;
	int top, left, bottom, right;
	short numLayers, numChannels;
	short channelType;
	uchar blendKey[4];
	uchar opacity, clipping, flags, filler;
	QString layerName, blend;
	struct PSDLayer lay;
	s >> layerinfo;
	s >> numLayers;
	if (numLayers < 0)
		numLayers = -numLayers;
	if (numLayers != 0)
	{
		for (int layer = 0; layer < numLayers; layer++)
		{
			s >> top;
			lay.ypos = top;
			s >> left;
			lay.xpos = left;
			s >> bottom;
			lay.height = bottom - top;
			s >> right;
			lay.width = right - left;
			s >> numChannels;
			lay.channelType.clear();
			lay.channelLen.clear();
			for (int channels = 0; channels < numChannels; channels++)
			{
				s >> channelType;
				s >> channelLen;
				lay.channelType.append(channelType);
				lay.channelLen.append(channelLen);
			}
			s >> signature;
			blend = "";
			for( int i = 0; i < 4; i++ )
			{
				s >> blendKey[i];
				blend += QChar(blendKey[i]);
			}
			lay.blend = blend;
			s >> opacity;
			lay.opacity = opacity;
			s >> clipping;
			lay.clipping = clipping;
			s >> flags;
			lay.flags = flags;
			s >> filler;
			s >> extradata;
			s >> layermasksize;
			if (layermasksize != 0)
			{
				s >> dummy;
				s >> dummy;
				s >> dummy;
				s >> dummy;
				s >> dummy;
			}
			s >> layerRange;
			s.device()->at( s.device()->at() + layerRange );
			lay.layerName = getLayerString(s);
			info->layerInfo.append(lay);
			s >> signature;
			if( signature == 0x3842494D )
			{
				while (signature == 0x3842494D )
				{
					s >> signature;
					s >> addRes;
					s.device()->at( s.device()->at() + addRes );
					s >> signature;
				}
				s.device()->at( s.device()->at() - 4 );
			}
			else
			{
				s.device()->at( s.device()->at() - 2 );
				s >> signature;
				if( signature == 0x3842494D )
				{
					while (signature == 0x3842494D )
					{
						s >> signature;
						s >> addRes;
						s.device()->at( s.device()->at() + addRes );
						s >> signature;
					}
					s.device()->at( s.device()->at() - 4 );
				}
				else
					s.device()->at( s.device()->at() - 6 );
			}
		}
		bool firstLayer = true;
		for (int layer = 0; layer < numLayers; layer++)
		{
			loadLayerChannels( s, header, img, info->layerInfo, layer, &firstLayer, info );
		}
	}
	else
	{
		s >> numLayers;
		loadLayer( s, header, img );
	}
	return true;
}

// Load the PSD image.
static bool LoadPSD( QDataStream & s, const PSDHeader & header, QImage & img, ImageInfoRecord *info)
{
	// Create dst image.
	if( !img.create( header.width, header.height, 32 ))
		return false;
	img.setAlphaBuffer( true );
	uint tmp;
	uint ressourceDataLen;
	uint startRessource;
	uint layerDataLen;
	uint startLayers;
	// Skip mode data. FIX: this is incorrect, it's the Colormap Data for indexed Images
	s >> tmp;
	s.device()->at( s.device()->at() + tmp );
	s >> ressourceDataLen;
	startRessource = s.device()->at();
	if (ressourceDataLen != 0)
		parseRessourceData(s, header, ressourceDataLen, info);
	s.device()->at( startRessource + ressourceDataLen );
	// Skip the reserved data. FIX: Also incorrect, this is the actual Layer Data for Images with Layers
	s >> layerDataLen;
	startLayers = s.device()->at();
	if (layerDataLen != 0)
		return parseLayer( s, header, img, info);
	else
	{
		// Decoding simple psd file, no layers
		s.device()->at( s.device()->at() + layerDataLen );
		loadLayer( s, header, img );
	}
	return true;
}

QString getAlpha(QString fn, bool PDF, bool pdf14)
{
	QImage img;
	QString retS = "";
	bool miniswhite = false;
	bool bilevel = false;
	float xres, yres;
	short resolutionunit = 0;
	ImageInfoRecord imgInfo;
	QFileInfo fi = QFileInfo(fn);
	if (!fi.exists())
		return retS;
	QString ext = fi.extension(false).lower();
	QString tmpFile = QDir::convertSeparators(QDir::homeDirPath()+"/.scribus/sc.png");
	if ((ext == "pdf") || (ext == "eps") || (ext == "ps") || (ext == "jpg") || (ext == "jpeg"))
		return retS;
	if (ext == "tif")
	{
#ifdef HAVE_TIFF
		TIFF* tif = TIFFOpen(fn, "r");
		if(tif)
		{
			unsigned width, height, size;
			TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
			TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
			TIFFGetField(tif, TIFFTAG_XRESOLUTION, &xres);
			TIFFGetField(tif, TIFFTAG_YRESOLUTION, &yres);
			TIFFGetField(tif, TIFFTAG_RESOLUTIONUNIT , &resolutionunit);
			size = width * height;
			uint16 photometric, bitspersample, samplesperpixel, fillorder;
			TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &photometric);
			TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bitspersample);
			TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel);
			TIFFGetField(tif, TIFFTAG_FILLORDER, &fillorder);
			uint32 *bits = 0;
			img.create(width,height,32);
			img.setAlphaBuffer(true);
			if (TIFFIsTiled(tif))
			{
				uint32 columns, rows;
				uint32 *tile_buf;
				uint32 xt, yt;
				TIFFGetField(tif, TIFFTAG_TILEWIDTH,  &columns);
				TIFFGetField(tif, TIFFTAG_TILELENGTH, &rows);
				tile_buf = (uint32*) _TIFFmalloc(columns*rows*sizeof(uint32));
				if (tile_buf == NULL)
				{
					TIFFClose(tif);
					return retS;
				}
				uint32 tileW = columns, tileH = rows;
				for (yt = 0; yt < (uint32)img.height(); yt += rows)
				{
					if (yt > (uint)img.height())
						break;
					if (img.height()-yt < rows)
						tileH = img.height()-yt;
					tileW = columns;
					register uint32 xi, yi;
					for (xt = 0; xt < (uint)img.width(); xt += columns)
					{
						switch (photometric)
						{
						case PHOTOMETRIC_SEPARATED:
							TIFFReadTile(tif, tile_buf, xt, yt, 0, 0);
							for (yi = 0; yi < tileH; yi++)
								_TIFFmemcpy(img.scanLine(yt+(tileH-1-yi))+xt, tile_buf+tileW*yi, tileW*4);
							break;
						default:
							TIFFReadRGBATile(tif, xt, yt, tile_buf);
							for(yi = 0; yi < tileH; yi++)
							{
								QRgb *row= (QRgb*) ( img.scanLine(yt+yi) );
								for(xi = 0; xi < tileW; xi++)
								{
									const uint32 pix = *(tile_buf + ((tileH-1-yi)*tileW)+xi);
									row[xi + xt] = qRgba(TIFFGetB(pix), TIFFGetG(pix), TIFFGetR(pix) , TIFFGetA(pix));
								} // for xi
							} // for yi
							break;
						} // else per pixel method
					} // for x by tiles
				} // for y by tiles
				_TIFFfree(tile_buf);
			}
			else
			{
				tsize_t bytesperrow = TIFFScanlineSize(tif);
				switch (photometric)
				{
				case PHOTOMETRIC_MINISWHITE:
					miniswhite = true;
				case PHOTOMETRIC_MINISBLACK:
					if (bitspersample == 1)
						bilevel = true;
					if (samplesperpixel != 1)
						break;
					bits = (uint32 *) _TIFFmalloc(bytesperrow);
					if (bits)
					{
						QImage img2;
						QImage::Endian bitorder = QImage::IgnoreEndian;
						if (bilevel)
						{
							if (fillorder == FILLORDER_MSB2LSB)
								bitorder = QImage::BigEndian;
							else
								bitorder = QImage::LittleEndian;
						}
						img2.create(width, height, bitspersample, 0, bitorder);
						for (unsigned int y = 0; y < height; y++)
						{
							if (TIFFReadScanline(tif, bits, y, 0))
								memcpy(img2.scanLine(y), bits, bytesperrow);
						}
						_TIFFfree(bits);
						img2 = img2.convertDepth(8);
						if (!miniswhite)
							img2.invertPixels();
						for (unsigned int y = 0; y < height; y++)
						{
							unsigned char *ptr = (unsigned char *) img.scanLine(y);
							unsigned char *ptr2 = (unsigned char *) img2.scanLine(y);
							for (unsigned int x = 0; x < width; x++)
							{
								if (bilevel)
									ptr[3] = (unsigned char) -img2.pixelIndex(x, y);
								else
									ptr[3] = ptr2[x];
								ptr+=4;
							}
						}
					}
					break;
				case PHOTOMETRIC_SEPARATED:
					bits = (uint32 *) _TIFFmalloc(bytesperrow);
					if (bits)
					{
						for (unsigned int y = 0; y < height; y++)
						{
							if (TIFFReadScanline(tif, bits, y, 0))
							{
								memcpy(img.scanLine(y), bits, width * 4);
							}
						}
						_TIFFfree(bits);
					}
					break;
				default:
					bits = (uint32 *) _TIFFmalloc(size * sizeof(uint32));
					if(bits)
					{
						if (TIFFReadRGBAImage(tif, width, height, bits, 0))
						{
							for(unsigned int y = 0; y < height; y++)
								memcpy(img.scanLine(height - 1 - y), bits + y * width, width * 4);
						}
						_TIFFfree(bits);
					}
				}
				TIFFClose(tif);
			}
		}
#else
		qDebug("TIFF Support not available");
#endif // HAVE_TIFF

	}
	else if (ext == "psd")
	{
		QFile f(fn);
		if (f.open(IO_ReadOnly))
		{
			xres = 72.0;
			yres = 72.0;
			QDataStream s( &f );
			s.setByteOrder( QDataStream::BigEndian );
			PSDHeader header;
			s >> header;
			// Check image file format.
			if( s.atEnd() || !IsValid( header ) )
				return retS;
			// Check if it's a supported format.
			if( !IsSupported( header ) )
				return retS;
			FPointArray clip;
			if( !LoadPSD(s, header, img, &imgInfo) )
				return retS;
			img = img.convertDepth(32);
			img.setAlphaBuffer(true);
			f.close();
		}
		else
			return retS;
	}
	else
	{
		if (img.load(fn))
		{
			img = img.convertDepth(32);
			img.setAlphaBuffer(true);
		}
		else
			return retS;
	}
	if (img.isNull())
		return retS;
	int hm = img.height();
	int wm = img.width();
	int w2;
	if (pdf14)
	{
		for( int yi=0; yi < hm; ++yi )
		{
			QRgb * s = (QRgb*)(img.scanLine( yi ));
			for( int xi=0; xi < wm; ++xi )
			{
				QRgb r=*s++;
				unsigned char u=qAlpha(r);
				retS += u;
			}
		}
	}
	else
	{
		QImage iMask = img.createAlphaMask();
		hm = iMask.height();
		wm = iMask.width();
		w2 = wm / 8;
		if ((wm % 8) != 0)
			w2++;
		for( int yi=0; yi < hm; ++yi )
		{
			uchar * s = iMask.scanLine( yi );
			for( int xi=0; xi < w2; ++xi )
			{
				unsigned char u = *(s+xi);
				retS += PDF ? ~u : u;
			}
		}
	}
	return retS;
}

QImage LoadPicture(QString fn, QString Prof, int rend, bool useEmbedded, bool useProf, int requestType, int gsRes, bool *realCMYK, ImageInfoRecord *info)
{
	// requestType - 0: CMYK, 1: RGB, 2: RGB Proof
	// gsRes - is the resolution that ghostscript will render at
	QImage img;
	bool isCMYK = false;
	if (realCMYK != 0)
		*realCMYK = false;
	bool miniswhite = false;
	bool bilevel = false;
	float xres, yres;
	short resolutionunit = 0;
#ifdef HAVE_CMS
	cmsHTRANSFORM xform = 0;
	cmsHPROFILE inputProf = 0;
	cmsHPROFILE tiffProf = 0;
	int cmsFlags = 0;
#endif
	QFileInfo fi = QFileInfo(fn);
	if (!fi.exists())
		return img;
	QString ext = fi.extension(false).lower();
	QString tmp, dummy, cmd1, cmd2, BBox, tmp2;
	QChar tc;
	double x, y, b, h;
	bool found = false;
	int ret = -1;
	QString tmpFile = QDir::convertSeparators(QDir::homeDirPath()+"/.scribus/sc.png");
	if (ext == "pdf")
	{
		QStringList args;
		xres = gsRes;
		yres = gsRes;
		args.append("-r"+QString::number(gsRes));
		args.append("-sOutputFile="+tmpFile);
		args.append("-dFirstPage=1");
		args.append("-dLastPage=1");
		args.append(fn);
		ret = callGS(args);
		if (ret == 0)
		{
			QImage image;
			image.load(tmpFile);
			unlink(tmpFile);
			image = image.convertDepth(32);
			image.setAlphaBuffer(true);
			int wi = image.width();
			int hi = image.height();
			if (ScApp->HavePngAlpha != 0)
			{
				for( int yi=0; yi < hi; ++yi )
				{
					QRgb *s = (QRgb*)(image.scanLine( yi ));
					for(int xi=0; xi < wi; ++xi )
					{
						if((*s) == 0xffffffff)
							(*s) &= 0x00ffffff;
						s++;
					}
				}
			}
			img = image.convertDepth(32);
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
				if (tmp.startsWith("%%BoundingBox:"))
				{
					found = true;
					BBox = tmp.remove("%%BoundingBox:");
				}
				if (!found)
				{
					if (tmp.startsWith("%%BoundingBox"))
					{
						found = true;
						BBox = tmp.remove("%%BoundingBox");
					}
				}
				if (tmp.startsWith("%%EndComments"))
					break;
			}
		}
		f.close();
		if (found)
		{
			QTextStream ts2(&BBox, IO_ReadOnly);
			ts2 >> x >> y >> b >> h;
			x = x * gsRes / 72.0;
			y = y * gsRes / 72.0;
			b = b * gsRes / 72.0;
			h = h * gsRes / 72.0;
			QStringList args;
			xres = gsRes;
			yres = gsRes;
			args.append("-r"+QString::number(gsRes));
			args.append("-sOutputFile="+tmpFile);
			args.append("-g"+tmp.setNum(qRound(b))+"x"+tmp2.setNum(qRound(h)));
			args.append(fn);
			ret = callGS(args);
			if (ret == 0)
			{
				QImage image;
				image.load(tmpFile);
				image = image.convertDepth(32);
				image.setAlphaBuffer(true);
				if (ScApp->HavePngAlpha != 0)
				{
					int wi = image.width();
					int hi = image.height();
					for( int yi=0; yi < hi; ++yi )
					{
						QRgb *s = (QRgb*)(image.scanLine( yi ));
						for(int xi=0; xi < wi; ++xi )
						{
							if((*s) == 0xffffffff)
								(*s) &= 0x00ffffff;
							s++;
						}
					}
				}
				img = image.copy(static_cast<int>(x), 0, static_cast<int>(b-x), static_cast<int>(h-y));
				unlink(tmpFile);
			}
		}
	}
#ifdef HAVE_TIFF
	else if (ext == "tif")
	{
		QImage img2;
		TIFF* tif = TIFFOpen(fn, "r");
		if(tif)
		{
			unsigned width, height, size;
			TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
			TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
			TIFFGetField(tif, TIFFTAG_XRESOLUTION, &xres);
			TIFFGetField(tif, TIFFTAG_YRESOLUTION, &yres);
			TIFFGetField(tif, TIFFTAG_RESOLUTIONUNIT , &resolutionunit);
			size = width * height;
			uint16 photometric, bitspersample, samplesperpixel, fillorder;
			TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &photometric);
			TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bitspersample);
			TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel);
			TIFFGetField(tif, TIFFTAG_FILLORDER, &fillorder);
			if (!img.create(width,height,32))
			{
				TIFFClose(tif);
				return img;
			}
			img.setAlphaBuffer(true);
			if (TIFFIsTiled(tif))
			{
				uint32 columns, rows;
				uint32 *tile_buf;
				uint32 xt, yt;
				TIFFGetField(tif, TIFFTAG_TILEWIDTH,  &columns);
				TIFFGetField(tif, TIFFTAG_TILELENGTH, &rows);
				tile_buf = (uint32*) _TIFFmalloc(columns*rows*sizeof(uint32));
				if (tile_buf == NULL)
				{
					TIFFClose(tif);
					return img;
				}
				uint32 tileW = columns, tileH = rows;
				for (yt = 0; yt < (uint32)img.height(); yt += rows)
				{
					if (yt > (uint)img.height())
						break;
					if (img.height()-yt < rows)
						tileH = img.height()-yt;
					tileW = columns;
					register uint32 xi, yi;
					for (xt = 0; xt < (uint)img.width(); xt += columns)
					{
						switch (photometric)
						{
						case PHOTOMETRIC_SEPARATED:
							TIFFReadTile(tif, tile_buf, xt, yt, 0, 0);
							for (yi = 0; yi < tileH; yi++)
								_TIFFmemcpy(img.scanLine(yt+(tileH-1-yi))+xt, tile_buf+tileW*yi, tileW*4);
							isCMYK = true;
							if (realCMYK != 0)
								*realCMYK = true;
							break;
						default:
							TIFFReadRGBATile(tif, xt, yt, tile_buf);
							for(yi = 0; yi < tileH; yi++)
							{
								QRgb *row= (QRgb*) ( img.scanLine(yt+yi) );
								for(xi = 0; xi < tileW; xi++)
								{
									const uint32 pix = *(tile_buf + ((tileH-1-yi)*tileW)+xi);
									row[xi + xt] = qRgba(TIFFGetB(pix), TIFFGetG(pix), TIFFGetR(pix) , TIFFGetA(pix));
								} // for xi
							} // for yi
							isCMYK = false;
							if (realCMYK != 0)
								*realCMYK = false;
							break;
						} // else per pixel method
					} // for x by tiles
				} // for y by tiles
				_TIFFfree(tile_buf);
			}
			else
			{
				tsize_t bytesperrow = TIFFScanlineSize(tif);
				uint32 *bits = 0;
				switch (photometric)
				{
				case PHOTOMETRIC_MINISWHITE:
					miniswhite = true;
				case PHOTOMETRIC_MINISBLACK:
					if (bitspersample == 1)
						bilevel = true;
					if (samplesperpixel != 1)
						break;
					bits = (uint32 *) _TIFFmalloc(bytesperrow);
					if (bits)
					{
						QImage::Endian bitorder = QImage::IgnoreEndian;
						if (bilevel)
						{
							if (fillorder == FILLORDER_MSB2LSB)
								bitorder = QImage::BigEndian;
							else
								bitorder = QImage::LittleEndian;
						}
						if (!img2.create(width, height, bitspersample, 0, bitorder))
						{
							_TIFFfree(bits);
							TIFFClose(tif);
							return img;
						}
						for (unsigned int y = 0; y < height; y++)
						{
							if (TIFFReadScanline(tif, bits, y, 0))
								memcpy(img2.scanLine(y), bits, bytesperrow);
						}
						_TIFFfree(bits);
						img2 = img2.convertDepth(8);
						if (!miniswhite)
							img2.invertPixels();
						for (unsigned int y = 0; y < height; y++)
						{
							unsigned char *ptr = (unsigned char *) img.scanLine(y);
							unsigned char *ptr2 = (unsigned char *) img2.scanLine(y);
							for (unsigned int x = 0; x < width; x++)
							{
								if (bilevel)
									ptr[3] = (unsigned char) -img2.pixelIndex(x, y);
								else
									ptr[3] = ptr2[x];
								ptr+=4;
							}
						}
					}
					isCMYK = true;
					if (realCMYK != 0)
						*realCMYK = true;
					break;
				case PHOTOMETRIC_SEPARATED:
					bits = (uint32 *) _TIFFmalloc(bytesperrow);
					if (bits)
					{
						for (unsigned int y = 0; y < height; y++)
						{
							if (TIFFReadScanline(tif, bits, y, 0))
							{
								memcpy(img.scanLine(y), bits, width * 4);
							}
						}
						_TIFFfree(bits);
					}
					isCMYK = true;
					if (realCMYK != 0)
						*realCMYK = true;
					break;
				default:
					bits = (uint32 *) _TIFFmalloc(size * sizeof(uint32));
					if(bits)
					{
						if (TIFFReadRGBAImage(tif, width, height, bits, 0))
						{
							for(unsigned int y = 0; y < height; y++)
								memcpy(img.scanLine(height - 1 - y), bits + y * width, width * 4);
						}
						_TIFFfree(bits);
					}
				}
			}
#ifdef HAVE_CMS
			DWORD EmbedLen = 0;
			LPBYTE EmbedBuffer;
			if (TIFFGetField(tif, TIFFTAG_ICCPROFILE, &EmbedLen, &EmbedBuffer) && useEmbedded && CMSuse && useProf)
				tiffProf = cmsOpenProfileFromMem(EmbedBuffer, EmbedLen);
#endif // HAVE_CMS
			img = img.swapRGB();
			uint32 ClipLen = 0;
			uint8 ClipBuffer;
			QString db;
			if (TIFFGetField(tif, TIFFTAG_CLIPPATH, &ClipLen, &ClipBuffer))
				qDebug(db.setNum(ClipLen));
			TIFFClose(tif);
			if (resolutionunit == RESUNIT_INCH)
			{
				img.setDotsPerMeterX ((int) (xres / 0.0254));
				img.setDotsPerMeterY ((int) (yres / 0.0254));
			}
			else if (resolutionunit == RESUNIT_CENTIMETER)
			{
				img.setDotsPerMeterX ((int) (xres / 100.0));
				img.setDotsPerMeterY ((int) (yres / 100.0));
			}
		}
	}
#endif // HAVE_TIFF
	else if (ext == "psd")
	{
		QFile f(fn);
		if (f.open(IO_ReadOnly))
		{
			ImageInfoRecord imgInfo;
			imgInfo.xres = 72;
			imgInfo.yres = 72;
			QDataStream s( &f );
			s.setByteOrder( QDataStream::BigEndian );
			PSDHeader header;
			s >> header;
			// Check image file format.
			if( s.atEnd() || !IsValid( header ) )
				return img;
			// Check if it's a supported format.
			if( !IsSupported( header ) )
				return img;
			FPointArray clip;
			if (info != 0)
			{
				if( !LoadPSD(s, header, img, info) )
					return img;
			}
			else
			{
				if( !LoadPSD(s, header, img, &imgInfo) )
					return img;
			}
			if (header.color_mode == CM_CMYK)
				isCMYK = true;
			else
				isCMYK = false;
			if (realCMYK != 0)
				*realCMYK = isCMYK;
			if (info != 0)
			{
				info->colorspace = isCMYK;
				info->valid = true;
				img.setDotsPerMeterX ((int) (info->xres / 0.0254));
				img.setDotsPerMeterY ((int) (info->yres / 0.0254));
				xres = info->xres;
				yres = info->yres;
			}
			else
			{
				img.setDotsPerMeterX ((int) (imgInfo.xres / 0.0254));
				img.setDotsPerMeterY ((int) (imgInfo.yres / 0.0254));
				xres = imgInfo.xres;
				yres = imgInfo.yres;
			}
			f.close();
		}
		else
			return img;
	}
	else if ((ext == "jpg") || (ext == "jpeg"))
	{
		struct jpeg_decompress_struct cinfo;
		struct my_error_mgr         jerr;
		FILE     *infile;
		cinfo.err = jpeg_std_error (&jerr.pub);
		jerr.pub.error_exit = my_error_exit;
		infile = NULL;
		if (setjmp (jerr.setjmp_buffer))
		{
			jpeg_destroy_decompress (&cinfo);
			if (infile)
				fclose (infile);
			return img;
		}
		jpeg_create_decompress (&cinfo);
		if ((infile = fopen (fn, "rb")) == NULL)
			return img;
		jpeg_stdio_src(&cinfo, infile);
		jpeg_read_header(&cinfo, TRUE);
		jpeg_start_decompress(&cinfo);
		if ( cinfo.output_components == 3 || cinfo.output_components == 4)
			img.create( cinfo.output_width, cinfo.output_height, 32 );
		else if ( cinfo.output_components == 1 )
		{
			img.create( cinfo.output_width, cinfo.output_height, 8, 256 );
			for (int i=0; i<256; i++)
				img.setColor(i, qRgb(i,i,i));
		}
		if (!img.isNull())
		{
			uchar** lines = img.jumpTable();
			while (cinfo.output_scanline < cinfo.output_height)
				(void) jpeg_read_scanlines(&cinfo, lines + cinfo.output_scanline, cinfo.output_height);
			if ( cinfo.output_components == 3 )
			{
				for (uint j=0; j<cinfo.output_height; j++)
				{
					uchar *in = img.scanLine(j) + cinfo.output_width * 3;
					QRgb *out = (QRgb*)img.scanLine(j);
					for (uint i=cinfo.output_width; i--; )
					{
						in -= 3;
						out[i] = qRgb(in[0], in[1], in[2]);
					}
				}
			}
			if ( cinfo.output_components == 4 )
			{
				for (int i = 0; i < img.height(); i++)
				{
					unsigned int *ptr = (unsigned int *) img.scanLine(i);
					unsigned char c, m, y ,k;
					if ((cinfo.saw_Adobe_marker) && (cinfo.Adobe_transform == 2))
					{
						for (int j = 0; j < img.width(); j++)
						{
							unsigned char *p = (unsigned char *) ptr;
							c = p[0];
							m = p[1];
							y =  p[2];
							k =  p[3];
							p[0] = 255 - y;
							p[1] = 255 - m;
							p[2] = 255 - c;
							p[3] = 255 - k;
							ptr++;
						}
					}
					else
					{
						for (int j = 0; j < img.width(); j++)
						{
							unsigned char *p = (unsigned char *) ptr;
							c = p[0];
							m = p[1];
							y =  p[2];
							k =  p[3];
							p[0] = y;
							p[1] = m;
							p[2] = c;
							p[3] = k;
							ptr++;
						}
					}
				}
				isCMYK = true;
				if (realCMYK != 0)
					*realCMYK = true;
			}
			else
				isCMYK = false;
			img = img.convertDepth(32);
			img.setAlphaBuffer(true);
			if ( cinfo.density_unit == 1 )
			{
				xres = cinfo.X_density;
				yres = cinfo.Y_density;
				img.setDotsPerMeterX( int(100. * cinfo.X_density / 2.54) );
				img.setDotsPerMeterY( int(100. * cinfo.Y_density / 2.54) );
			}
			else if ( cinfo.density_unit == 2 )
			{
				xres = cinfo.X_density * 2.54;
				yres = cinfo.Y_density * 2.54;
				img.setDotsPerMeterX( int(100. * cinfo.X_density) );
				img.setDotsPerMeterY( int(100. * cinfo.Y_density) );
			}
		}
		(void) jpeg_finish_decompress(&cinfo);
		fclose (infile);
		jpeg_destroy_decompress (&cinfo);
	}
	else
	{
		if (img.load(fn))
		{
			xres = img.dotsPerMeterX() * 0.0254;
			yres = img.dotsPerMeterY() * 0.0254;
			img = img.convertDepth(32);
			img.setAlphaBuffer(true);
		}
	}
	if (img.isNull())
		return img;
#ifdef HAVE_CMS
	if (CMSuse && useProf)
	{
		if (tiffProf)
			inputProf = tiffProf;
		else
		{
			if (isCMYK)
				inputProf = CMSprinterProf;
			else
				inputProf = cmsOpenProfileFromFile(InputProfiles[Prof], "r");
		}
	}
	if (CMSuse && useProf && inputProf)
	{
		DWORD inputProfFormat = TYPE_RGBA_8;
		switch (static_cast<int>(cmsGetColorSpace(inputProf)))
		{
		case icSigRgbData:
			inputProfFormat = TYPE_RGBA_8;
			break;
		case icSigCmykData:
			inputProfFormat = TYPE_CMYK_8;
			break;
		}
		if (SoftProofing)
		{
			cmsFlags |= cmsFLAGS_SOFTPROOFING;
			if (Gamut)
				cmsFlags |= cmsFLAGS_GAMUTCHECK;
		}
#ifdef cmsFLAGS_BLACKPOINTCOMPENSATION
		if (BlackPoint)
			cmsFlags |= cmsFLAGS_BLACKPOINTCOMPENSATION;
#endif
		switch (requestType)
		{
		case 0: // CMYK
			if (!isCMYK)
				xform = cmsCreateTransform(inputProf, inputProfFormat,
				                           CMSprinterProf, TYPE_CMYK_8, IntentPrinter, 0);
			break;
		case 1: // RGB
			if (isCMYK)
				xform = cmsCreateTransform(inputProf, inputProfFormat,
				                           CMSoutputProf, TYPE_RGBA_8, rend, 0);
			break;
		case 2: // RGB Proof
			xform = cmsCreateProofingTransform(inputProf, inputProfFormat,
			                                   CMSoutputProf, TYPE_RGBA_8, CMSprinterProf,
			                                   IntentPrinter, rend, cmsFlags);
			break;
		}
		if (xform)
		{
			for (int i = 0; i < img.height(); i++)
			{
				LPBYTE ptr = img.scanLine(i);
				cmsDoTransform(xform, ptr, ptr, img.width());
				// if transforming from CMYK to RGB, flatten the alpha channel
				// which will still contain the black channel
				if (isCMYK && requestType != 0 && !bilevel)
				{
					unsigned int *p = (unsigned int *) ptr;
					for (int j = 0; j < img.width(); j++, p++)
						*p |= 0xff000000;
				}

			}
			cmsDeleteTransform (xform);
		}
		if (inputProf && inputProf != CMSprinterProf)
			cmsCloseProfile(inputProf);
	}
	else
#endif // HAVE_CMS

	{
		switch (requestType)
		{
		case 0:
			if (!isCMYK)
			{
				for (int i = 0; i < img.height(); i++)
				{
					unsigned int *ptr = (unsigned int *) img.scanLine(i);
					unsigned char c, m, y ,k;
					for (int j = 0; j < img.width(); j++)
					{
						unsigned char *p = (unsigned char *) ptr;
						c = 255 - p[0];
						m = 255 - p[1];
						y = 255 - p[2];
						k = QMIN(QMIN(c, m), y);
						p[0] = c - k;
						p[1] = m - k;
						p[2] = y - k;
						p[3] = k;
						ptr++;
					}
				}
			}
			break;
		case 1:
		case 2:
			if (isCMYK)
			{
				for (int i = 0; i < img.height(); i++)
				{
					unsigned int *ptr = (unsigned int *) img.scanLine(i);
					unsigned char r, g, b;
					for (int j = 0; j < img.width(); j++)
					{
						unsigned char *p = (unsigned char *) ptr;
						r = 255 - QMIN(255, p[0] + p[3]);
						g = 255 - QMIN(255, p[1] + p[3]);
						b = 255 - QMIN(255, p[2] + p[3]);
						p[0] = r;
						p[1] = g;
						p[2] = b;
						p[3] = 255;
						ptr++;
					}
				}
			}
			break;
		}
	}
	if ((requestType == 0 || isCMYK) && !bilevel)
		img.setAlphaBuffer(false);
	img.setDotsPerMeterX (QMAX(2834, (int) (xres / 0.0254)));
	img.setDotsPerMeterY (QMAX(2834, (int) (yres / 0.0254)));
	return img;
}

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
	args.append(fn);
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
	QString pfad = ICONDIR;
	pfad += nam;
	QPixmap pm;
	pm.load(pfad);
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

double Cwidth(ScribusDoc *currentDoc, QString name, QString ch, int Size, QString ch2)
{
	double width;
	FT_Vector  delta;
	uint c1 = ch.at(0).unicode();
	uint c2 = ch2.at(0).unicode();
	Foi* fo = (*currentDoc->AllFonts)[name];
	if (fo->CharWidth.contains(c1))
	{
		width = fo->CharWidth[c1]*(Size / 10.0);
		if (fo->HasKern)
		{
			uint cl = FT_Get_Char_Index(currentDoc->FFonts[name], c1);
			uint cr = FT_Get_Char_Index(currentDoc->FFonts[name], c2);
			FT_Get_Kerning(currentDoc->FFonts[name], cl, cr, ft_kerning_unscaled, &delta);
			width += delta.x / fo->uniEM * (Size / 10.0);
		}
		return width;
	}
	else
		return static_cast<double>(Size / 10.0);
}

double RealCWidth(ScribusDoc *currentDoc, QString name, QString ch, int Size)
{
	double w, ww;
	uint c1 = ch.at(0).unicode();
	Foi* fo = (*currentDoc->AllFonts)[name];
	if (fo->CharWidth.contains(c1))
	{
		uint cl = FT_Get_Char_Index(currentDoc->FFonts[name], c1);
		FT_Load_Glyph( currentDoc->FFonts[name], cl, FT_LOAD_NO_SCALE | FT_LOAD_NO_BITMAP );
		w = (currentDoc->FFonts[name]->glyph->metrics.width + fabs((double)currentDoc->FFonts[name]->glyph->metrics.horiBearingX)) / fo->uniEM * (Size / 10.0);
		ww = currentDoc->FFonts[name]->glyph->metrics.horiAdvance / fo->uniEM * (Size / 10.0);
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
	FPointArray pts = FPointArray();
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
			QRgb r=*s;
			int c = 255 - qRed(r);
			int m = 255 - qGreen(r);
			int y = 255 - qBlue(r);
			int k = QMIN(QMIN(c, m), y);
			*s = qRgba(m - k, y - k, k, c - k);
			ImgStr += c - k;
			ImgStr += m - k;
			ImgStr += y - k;
			ImgStr += k;
			s++;
		}
	}
	return ImgStr;
}

QString ImageToGray(QImage *im)
{
	int h = im->height();
	int w = im->width();
	QString ImgStr = "";
	for( int yi=0; yi < h; ++yi )
	{
		QRgb * s = (QRgb*)(im->scanLine( yi ));
		for( int xi=0; xi < w; ++xi )
		{
			QRgb r=*s;
			int k = QMIN(qRound(0.3 * qRed(r) + 0.59 * qGreen(r) + 0.11 * qBlue(r)), 255);
			*s = qRgba(k, 0, 0, 0);
			ImgStr += k;
			s++;
		}
	}
	return ImgStr;
}

QString ImageToCMYK_PDF(QImage *im, bool pre)
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
				QRgb r=*s;
				int c = qRed(r);
				int m = qGreen(r);
				int y = qBlue(r);
				int k = qAlpha(r);
				*s = qRgba(m, y, k, c);
				ImgStr += c;
				ImgStr += m;
				ImgStr += y;
				ImgStr += k;
				s++;
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
				QRgb r=*s;
				int c = 255 - qRed(r);
				int m = 255 - qGreen(r);
				int y = 255 - qBlue(r);
				int k = QMIN(QMIN(c, m), y);
				*s = qRgba(m, y, k, c);
				ImgStr += c - k;
				ImgStr += m - k;
				ImgStr += y - k;
				ImgStr += k;
				s++;
			}
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
			ImgStr += PDF ? ~u : u;
		}
	}
	return ImgStr;
}

QString MaskToTxt14(QImage *im)
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
			unsigned char u=qAlpha(r);
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
	if (countUniCode > 255)
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
	composite->addPoint(FPoint(tox, toy));
	composite->addPoint(FPoint(tox, toy));
	firstP = FPoint(tox, toy);
	return 0;
}

int traceLineto( FT_Vector *to, FPointArray *composite )
{
	double tox = ( to->x / 64.0 );
	double toy = ( to->y / 64.0 );
	if (composite->size() > 4)
	{
		FPoint b1 = composite->point(composite->size()-4);
		FPoint b2 = composite->point(composite->size()-3);
		FPoint b3 = composite->point(composite->size()-2);
		FPoint b4 = composite->point(composite->size()-1);
		FPoint n1 = FPoint(tox, toy);
		FPoint n2 = FPoint(tox, toy);
		FPoint n3 = FPoint(tox, toy);
		FPoint n4 = FPoint(tox, toy);
		if ((b1 == n1) && (b2 == n2) && (b3 == n3) && (b4 == n4))
			return 0;
	}
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
	if (composite->size() > 4)
	{
		FPoint b1 = composite->point(composite->size()-4);
		FPoint b2 = composite->point(composite->size()-3);
		FPoint b3 = composite->point(composite->size()-2);
		FPoint b4 = composite->point(composite->size()-1);
		FPoint n1 = FPoint(x2, y2);
		FPoint n2 = FPoint(x1, y1);
		FPoint n3 = FPoint(x2, y2);
		FPoint n4 = FPoint(x2, y2);
		if ((b1 == n1) && (b2 == n2) && (b3 == n3) && (b4 == n4))
			return 0;
	}
	composite->addPoint(FPoint(x2, y2));
	composite->addPoint(FPoint(x1, y1));
	composite->addPoint(FPoint(x2, y2));
	composite->addPoint(FPoint(x2, y2));
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
	if (composite->size() > 4)
	{
		FPoint b1 = composite->point(composite->size()-4);
		FPoint b2 = composite->point(composite->size()-3);
		FPoint b3 = composite->point(composite->size()-2);
		FPoint b4 = composite->point(composite->size()-1);
		FPoint n1 = FPoint(x3, y3);
		FPoint n2 = FPoint(x2, y2);
		FPoint n3 = FPoint(x3, y3);
		FPoint n4 = FPoint(x3, y3);
		if ((b1 == n1) && (b2 == n2) && (b3 == n3) && (b4 == n4))
			return 0;
	}
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

FPointArray traceChar(FT_Face face, uint chr, int chs, double *x, double *y, bool *err)
{
	bool error = false;
	FT_UInt glyphIndex;
	FPointArray pts, pts2;
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
	rp = FPoint(mx, my);
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
	rp = FPoint(mx, my);
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

void CopyPageItem(struct CopyPasteBuffer *Buffer, PageItem *b)
{
	uint a;
	Buffer->PType = b->itemType();
	Buffer->Xpos = b->Xpos;
	Buffer->Ypos = b->Ypos;
	Buffer->Width = b->Width;
	Buffer->Height = b->Height;
	Buffer->RadRect = b->RadRect;
	Buffer->FrameType = b->FrameType;
	Buffer->ClipEdited = b->ClipEdited;
	Buffer->Pwidth = b->Pwidth;
	Buffer->Pcolor = b->fillColor();
	Buffer->Pcolor2 = b->lineColor();
	Buffer->Shade = b->fillShade();
	Buffer->Shade2 = b->lineShade();
	Buffer->GrColor = "";
	Buffer->GrColor2 = "";
	Buffer->GrShade = 100;
	Buffer->GrShade2 = 100;
	Buffer->fill_gradient = b->fill_gradient;
	Buffer->GrType = b->GrType;
	Buffer->GrStartX = b->GrStartX;
	Buffer->GrStartY = b->GrStartY;
	Buffer->GrEndX = b->GrEndX;
	Buffer->GrEndY = b->GrEndY;
	Buffer->TxtStroke = b->TxtStroke;
	Buffer->TxtFill = b->TxtFill;
	Buffer->ShTxtStroke = b->ShTxtStroke;
	Buffer->ShTxtFill = b->ShTxtFill;
	Buffer->TxtScale = b->TxtScale;
	Buffer->TxTStyle = b->TxTStyle;
	Buffer->Rot = b->Rot;
	Buffer->PLineArt = b->PLineArt;
	Buffer->PLineEnd = b->PLineEnd;
	Buffer->PLineJoin = b->PLineJoin;
	Buffer->LineSp = b->LineSp;
	Buffer->LocalScX = b->LocalScX;
	Buffer->LocalScY = b->LocalScY;
	Buffer->LocalX = b->LocalX;
	Buffer->LocalY = b->LocalY;
	Buffer->PicArt = b->PicArt;
	Buffer->flippedH = b->imageFlippedH();
	Buffer->flippedV = b->imageFlippedV();
	Buffer->BBoxX = b->BBoxX;
	Buffer->BBoxH = b->BBoxH;
	Buffer->isPrintable = b->isPrintable;
	Buffer->isBookmark = b->isBookmark;
	Buffer->BMnr = b->BMnr;
	Buffer->isAnnotation = b->isAnnotation;
	Buffer->AnType = b->AnType;
	Buffer->AnAction = b->AnAction;
	Buffer->An_E_act = b->An_E_act;
	Buffer->An_X_act = b->An_X_act;
	Buffer->An_D_act = b->An_D_act;
	Buffer->An_Fo_act = b->An_Fo_act;
	Buffer->An_Bl_act = b->An_Bl_act;
	Buffer->An_K_act = b->An_K_act;
	Buffer->An_F_act = b->An_F_act;
	Buffer->An_V_act = b->An_V_act;
	Buffer->An_C_act = b->An_C_act;
	Buffer->An_Extern = b->An_Extern;
	Buffer->AnZiel = b->AnZiel;
	Buffer->AnName = b->itemName();
	Buffer->AnActType = b->AnActType;
	Buffer->AnToolTip = b->AnToolTip;
	Buffer->AnBwid = b->AnBwid;
	Buffer->AnBsty = b->AnBsty;
	Buffer->AnFeed = b->AnFeed;
	Buffer->AnFlag = b->AnFlag;
	Buffer->AnFont = b->AnFont;
	Buffer->AnRollOver = b->AnRollOver;
	Buffer->AnDown = b->AnDown;
	Buffer->AnFormat = b->AnFormat;
	Buffer->AnVis = b->AnVis;
	Buffer->AnMaxChar = b->AnMaxChar;
	Buffer->AnChkStil = b->AnChkStil;
	Buffer->AnIsChk = b->AnIsChk;
	Buffer->AnAAact = b->AnAAact;
	Buffer->AnBColor = b->AnBColor;
	Buffer->AnHTML = b->AnHTML;
	Buffer->AnUseIcons = b->AnUseIcons;
	Buffer->AnIPlace = b->AnIPlace;
	Buffer->AnScaleW = b->AnScaleW;
	Buffer->Extra = b->Extra;
	Buffer->TExtra = b->TExtra;
	Buffer->BExtra = b->BExtra;
	Buffer->RExtra = b->RExtra;
	Buffer->Pfile = b->Pfile;
	Buffer->Pfile2 = b->Pfile2;
	Buffer->Pfile3 = b->Pfile3;
	QString Text = "";
	if (b->itemText.count() != 0)
	{
		for (a=0; a<b->itemText.count(); ++a)
		{
			if( (b->itemText.at(a)->ch == "\n") || (b->itemText.at(a)->ch == "\r"))
				Text += QString(QChar(5))+"\t";
			else if(b->itemText.at(a)->ch == "\t")
				Text += QString(QChar(4))+"\t";
			else
				Text += b->itemText.at(a)->ch+"\t";
			Text += b->itemText.at(a)->cfont+"\t";
			Text += QString::number(b->itemText.at(a)->csize / 10.0)+"\t";
			Text += b->itemText.at(a)->ccolor+"\t";
			Text += QString::number(b->itemText.at(a)->cextra)+"\t";
			Text += QString::number(b->itemText.at(a)->cshade)+'\t';
			Text += QString::number(b->itemText.at(a)->cstyle)+'\t';
			Text += QString::number(b->itemText.at(a)->cab)+'\t';
			Text += b->itemText.at(a)->cstroke+"\t";
			Text += QString::number(b->itemText.at(a)->cshade2)+'\t';
			Text += QString::number(b->itemText.at(a)->cscale)+'\n';
		}
	}
	Buffer->itemText = Text;
	Buffer->Clip = b->Clip.copy();
	Buffer->PoLine = b->PoLine.copy();
	Buffer->ContourLine = b->ContourLine.copy();
	Buffer->UseContour = b->textFlowUsesContourLine();
	Buffer->TabValues = b->TabValues;
	Buffer->DashValues = b->DashValues;
	Buffer->DashOffset = b->DashOffset;
	Buffer->PoShow = b->PoShow;
	Buffer->BaseOffs = b->BaseOffs;
	Buffer->Textflow = b->textFlowsAroundFrame();
	Buffer->Textflow2 = b->textFlowUsesBoundingBox();
	Buffer->textAlignment = b->textAlignment;
	Buffer->IFont = b->IFont;
	Buffer->ISize = b->ISize;
	Buffer->ExtraV = b->ExtraV;
	Buffer->Groups = b->Groups;
	Buffer->IProfile = b->IProfile;
	Buffer->IRender = b->IRender;
	Buffer->UseEmbedded = b->UseEmbedded;
	Buffer->EmProfile = b->EmProfile;
	Buffer->LayerNr = b->LayerNr;
	Buffer->ScaleType = b->ScaleType;
	Buffer->AspectRatio = b->AspectRatio;
	Buffer->Locked = b->locked();
	Buffer->LockRes = b->sizeLocked();
	Buffer->Transparency = b->fillTransparency();
	Buffer->TranspStroke = b->lineTransparency();
	Buffer->Reverse = b->Reverse;
	Buffer->InvPict = b->InvPict;
	Buffer->NamedLStyle = b->NamedLStyle;
	Buffer->Language = b->Language;
	Buffer->Cols = b->Cols;
	Buffer->ColGap = b->ColGap;
	Buffer->isTableItem = b->isTableItem;
	Buffer->TopLine = b->TopLine;
	Buffer->LeftLine = b->LeftLine;
	Buffer->RightLine = b->RightLine;
	Buffer->BottomLine = b->BottomLine;
	if (b->isTableItem)
	{
		if (b->TopLink != 0)
			Buffer->TopLinkID = b->TopLink->ItemNr;
		else
			Buffer->TopLinkID = -1;
		if (b->LeftLink != 0)
			Buffer->LeftLinkID = b->LeftLink->ItemNr;
		else
			Buffer->LeftLinkID = -1;
		if (b->RightLink != 0)
			Buffer->RightLinkID = b->RightLink->ItemNr;
		else
			Buffer->RightLinkID = -1;
		if (b->BottomLink != 0)
			Buffer->BottomLinkID = b->BottomLink->ItemNr;
		else
			Buffer->BottomLinkID = -1;
	}
	Buffer->startArrowIndex = b->startArrowIndex;
	Buffer->endArrowIndex = b->endArrowIndex;
}

void WordAndPara(PageItem* b, int *w, int *p, int *c, int *wN, int *pN, int *cN)
{
	QChar Dat = QChar(32);
	int para = 0;
	int ww = 0;
	int cc = 0;
	int paraN = 0;
	int wwN = 0;
	int ccN = 0;
	bool first = true;
	PageItem *nb = b;
	PageItem *nbl = b;
	while (nb != 0)
	{
		if (nb->BackBox != 0)
			nb = nb->BackBox;
		else
			break;
	}
	while (nb != 0)
	{
		for (uint a = 0; a < nb->itemText.count(); ++a)
		{
			QChar b = nb->itemText.at(a)->ch[0];
			if (b == QChar(13))
			{
				if (a >= nb->MaxChars)
					paraN++;
				else
					para++;
			}
			if ((!b.isLetterOrNumber()) && (Dat.isLetterOrNumber()) && (!first))
			{
				if (a >= nb->MaxChars)
					wwN++;
				else
					ww++;
			}
			if (a >= nb->MaxChars)
				ccN++;
			else
				cc++;
			Dat = b;
			first = false;
		}
		nbl = nb;
		nb = nb->NextBox;
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
		PageItem *ite = currentDoc->MasterItems.at(azz);
		if (ite->itemType() == PageItem::PathText)
			ite->DrawObj(painter, rd);
	}
	for (uint azz=0; azz<currentDoc->Items.count(); ++azz)
	{
		PageItem *ite = currentDoc->Items.at(azz);
		if ((ite->itemType() == PageItem::TextFrame) || (ite->itemType() == PageItem::PathText))
			ite->DrawObj(painter, rd);
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
