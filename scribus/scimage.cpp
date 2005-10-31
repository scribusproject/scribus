#include "scimage.h"
#include "scribus.h"
#include <qtextstream.h>
#include <cassert>
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
#include "util.h"
#include "exif.h"



typedef struct my_error_mgr
{
	struct jpeg_error_mgr pub;            /* "public" fields */
	jmp_buf setjmp_buffer;  /* for return to caller */
}
*my_error_ptr;

static void my_error_exit (j_common_ptr cinfo)
{
	my_error_ptr myerr = (my_error_ptr) cinfo->err;
	(*cinfo->err->output_message) (cinfo);
	longjmp (myerr->setjmp_buffer, 1);
}

static QDataStream & operator>> ( QDataStream & s, ScImage::PSDHeader & header )
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

ScImage::ScImage(QImage image) : QImage(image)
{
	initialize();
}

ScImage::ScImage() : QImage()
{
	initialize();
}

ScImage::ScImage( int width, int height ) : QImage( width, height, 32 )
{
	initialize();
}

void ScImage::initialize()
{
	imgInfo.xres = 72;
	imgInfo.yres = 72;
	imgInfo.colorspace = 0;
	imgInfo.valid = false;
	imgInfo.isRequest = false;
	imgInfo.progressive = false;
	imgInfo.exifDataValid = false;
	imgInfo.lowResType = 1;
	imgInfo.lowResScale = 1.0;
	imgInfo.PDSpathData.clear();
	imgInfo.RequestProps.clear();
	imgInfo.clipPath = "";
	imgInfo.usedPath = "";
	imgInfo.layerInfo.clear();
	imgInfo.exifInfo.cameraName = "";
	imgInfo.exifInfo.cameraVendor = "";
	imgInfo.exifInfo.thumbnail = QImage();
}

void ScImage::applyEffect(QValueList<imageEffect> effectsList, QMap<QString,ScColor> colors, bool cmyk)
{
	if (effectsList.count() != 0)
	{
		for (uint a = 0; a < effectsList.count(); ++a)
		{
			if ((*effectsList.at(a)).effectCode == EF_INVERT)
				invert(cmyk);
			if ((*effectsList.at(a)).effectCode == EF_GRAYSCALE)
				toGrayscale(cmyk);
			if ((*effectsList.at(a)).effectCode == EF_COLORIZE)
			{
				QString tmpstr = (*effectsList.at(a)).effectParameters;
				QString col = "None";
				int shading = 100;
				QTextStream fp(&tmpstr, IO_ReadOnly);
				fp >> col;
				fp >> shading;
				colorize(colors[col], shading, cmyk);
			}
			if ((*effectsList.at(a)).effectCode == EF_BRIGHTNESS)
			{
				QString tmpstr = (*effectsList.at(a)).effectParameters;
				int brightnessValue = 0;
				QTextStream fp(&tmpstr, IO_ReadOnly);
				fp >> brightnessValue;
				brightness(brightnessValue, cmyk);
			}
			if ((*effectsList.at(a)).effectCode == EF_CONTRAST)
			{
				QString tmpstr = (*effectsList.at(a)).effectParameters;
				int contrastValue = 0;
				QTextStream fp(&tmpstr, IO_ReadOnly);
				fp >> contrastValue;
				contrast(contrastValue, cmyk);
			}
			if ((*effectsList.at(a)).effectCode == EF_SHARPEN)
			{
				QString tmpstr = (*effectsList.at(a)).effectParameters;
				double radius, sigma;
				QTextStream fp(&tmpstr, IO_ReadOnly);
				fp >> radius;
				fp >> sigma;
				sharpen(radius, sigma);
			}
			if ((*effectsList.at(a)).effectCode == EF_BLUR)
			{
				QString tmpstr = (*effectsList.at(a)).effectParameters;
				double radius, sigma;
				QTextStream fp(&tmpstr, IO_ReadOnly);
				fp >> radius;
				fp >> sigma;
				blur(radius, sigma);
			}
			if ((*effectsList.at(a)).effectCode == EF_SOLARIZE)
			{
				QString tmpstr = (*effectsList.at(a)).effectParameters;
				double sigma;
				QTextStream fp(&tmpstr, IO_ReadOnly);
				fp >> sigma;
				solarize(sigma, cmyk);
			}
		}
	}
}

void ScImage::liberateMemory(void **memory)
{
	assert(memory != (void **)NULL);
	if(*memory == (void *)NULL)
		return;
	free(*memory);
	*memory=(void *) NULL;
}

void ScImage::solarize(double factor, bool cmyk)
{
	curveTable.resize(256);
	int fk = qRound(255 / factor);
	for (int i = 0; i < 256; ++i)
	{
		curveTable[i] = QMIN(255, static_cast<int>(i / fk) * fk);
	}
	applyCurve(cmyk);
}

void ScImage::blurScanLine(double *kernel, int width, unsigned int *src, unsigned int *dest, int columns)
{
	register double *p;
	unsigned int *q;
	register int x;
	register long i;
	double red, green, blue, alpha;
	double scale = 0.0;
	if(width > columns)
	{
		for(x=0; x < columns; ++x)
		{
			scale = 0.0;
			red = blue = green = alpha = 0.0;
			p = kernel;
			q = src;
			for(i=0; i < columns; ++i)
			{
				if((i >= (x-width/2)) && (i <= (x+width/2)))
				{
					red += (*p)*(qRed(*q)*257);
					green += (*p)*(qGreen(*q)*257);
					blue += (*p)*(qBlue(*q)*257);
					alpha += (*p)*(qAlpha(*q)*257);
				}
				if(((i+width/2-x) >= 0) && ((i+width/2-x) < width))
					scale+=kernel[i+width/2-x];
				p++;
				q++;
			}
			scale = 1.0/scale;
			red = scale*(red+0.5);
			green = scale*(green+0.5);
			blue = scale*(blue+0.5);
			alpha = scale*(alpha+0.5);
			red = red < 0 ? 0 : red > 65535 ? 65535 : red;
			green = green < 0 ? 0 : green > 65535 ? 65535 : green;
			blue = blue < 0 ? 0 : blue > 65535 ? 65535 : blue;
			alpha = alpha < 0 ? 0 : alpha > 65535 ? 65535 : alpha;
			dest[x] = qRgba((unsigned char)(red/257UL),
			                (unsigned char)(green/257UL),
			                (unsigned char)(blue/257UL),
			                (unsigned char)(alpha/257UL));
		}
		return;
	}

	for(x=0; x < width/2; ++x)
	{
		scale = 0.0;
		red = blue = green = alpha = 0.0;
		p = kernel+width/2-x;
		q = src;
		for(i=width/2-x; i < width; ++i)
		{
			red += (*p)*(qRed(*q)*257);
			green += (*p)*(qGreen(*q)*257);
			blue += (*p)*(qBlue(*q)*257);
			alpha += (*p)*(qAlpha(*q)*257);
			scale += (*p);
			p++;
			q++;
		}
		scale=1.0/scale;
		red = scale*(red+0.5);
		green = scale*(green+0.5);
		blue = scale*(blue+0.5);
		alpha = scale*(alpha+0.5);
		red = red < 0 ? 0 : red > 65535 ? 65535 : red;
		green = green < 0 ? 0 : green > 65535 ? 65535 : green;
		blue = blue < 0 ? 0 : blue > 65535 ? 65535 : blue;
		alpha = alpha < 0 ? 0 : alpha > 65535 ? 65535 : alpha;
		dest[x] = qRgba((unsigned char)(red/257UL),
		                (unsigned char)(green/257UL),
		                (unsigned char)(blue/257UL),
		                (unsigned char)(alpha/257UL));
	}
	for(; x < columns-width/2; ++x)
	{
		red = blue = green = alpha = 0.0;
		p = kernel;
		q = src+(x-width/2);
		for (i=0; i < (long) width; ++i)
		{
			red += (*p)*(qRed(*q)*257);
			green += (*p)*(qGreen(*q)*257);
			blue += (*p)*(qBlue(*q)*257);
			alpha += (*p)*(qAlpha(*q)*257);
			p++;
			q++;
		}
		red = scale*(red+0.5);
		green = scale*(green+0.5);
		blue = scale*(blue+0.5);
		alpha = scale*(alpha+0.5);
		red = red < 0 ? 0 : red > 65535 ? 65535 : red;
		green = green < 0 ? 0 : green > 65535 ? 65535 : green;
		blue = blue < 0 ? 0 : blue > 65535 ? 65535 : blue;
		alpha = alpha < 0 ? 0 : alpha > 65535 ? 65535 : alpha;
		dest[x] = qRgba((unsigned char)(red/257UL),
		                (unsigned char)(green/257UL),
		                (unsigned char)(blue/257UL),
		                (unsigned char)(alpha/257UL));
	}
	for(; x < columns; ++x)
	{
		red = blue = green = alpha = 0.0;
		scale=0;
		p = kernel;
		q = src+(x-width/2);
		for(i=0; i < columns-x+width/2; ++i)
		{
			red += (*p)*(qRed(*q)*257);
			green += (*p)*(qGreen(*q)*257);
			blue += (*p)*(qBlue(*q)*257);
			alpha += (*p)*(qAlpha(*q)*257);
			scale += (*p);
			p++;
			q++;
		}
		scale=1.0/scale;
		red = scale*(red+0.5);
		green = scale*(green+0.5);
		blue = scale*(blue+0.5);
		alpha = scale*(alpha+0.5);
		red = red < 0 ? 0 : red > 65535 ? 65535 : red;
		green = green < 0 ? 0 : green > 65535 ? 65535 : green;
		blue = blue < 0 ? 0 : blue > 65535 ? 65535 : blue;
		alpha = alpha < 0 ? 0 : alpha > 65535 ? 65535 : alpha;
		dest[x] = qRgba((unsigned char)(red/257UL),
		                (unsigned char)(green/257UL),
		                (unsigned char)(blue/257UL),
		                (unsigned char)(alpha/257UL));
	}
}

int ScImage::getBlurKernel(int width, double sigma, double **kernel)
{
	double alpha, normalize;
	register long i;
	int bias;
	assert(sigma != 0.0);
	if(width == 0)
		width = 3;
	*kernel=(double *)malloc(width*sizeof(double));
	if(*kernel == (double *)NULL)
		return(0);
	memset(*kernel, 0, width*sizeof(double));
	bias = 3*width/2;
	for(i=(-bias); i <= bias; i++)
	{
		alpha=exp(-((double) i*i)/(2.0*3*3*sigma*sigma));
		(*kernel)[(i+bias)/3]+=alpha/(2.50662827463100024161235523934010416269302368164062*sigma);
	}
	normalize=0;
	for(i=0; i < width; i++)
		normalize+=(*kernel)[i];
	for(i=0; i < width; i++)
		(*kernel)[i]/=normalize;
	return(width);
}

void ScImage::blur(double radius, double sigma)
{
	double *kernel;
	QImage dest;
	int widthk;
	int x, y;
	unsigned int *scanline, *temp;
	unsigned int *p, *q;
	if(sigma == 0.0)
		return;
	kernel=(double *) NULL;
	if(radius > 0)
		widthk=getBlurKernel((int) (2*ceil(radius)+1),sigma,&kernel);
	else
	{
		double *last_kernel;
		last_kernel=(double *) NULL;
		widthk=getBlurKernel(3,sigma,&kernel);
		while ((long) (255*kernel[0]) > 0)
		{
			if(last_kernel != (double *)NULL)
			{
				liberateMemory((void **) &last_kernel);
			}
			last_kernel=kernel;
			kernel = (double *)NULL;
			widthk = getBlurKernel(widthk+2, sigma, &kernel);
		}
		if(last_kernel != (double *) NULL)
		{
			liberateMemory((void **) &kernel);
			widthk-=2;
			kernel = last_kernel;
		}
	}
	if(widthk < 3)
	{
		liberateMemory((void **) &kernel);
		return;
	}
	dest.create(width(), height(), 32);
	scanline = (unsigned int *)malloc(sizeof(unsigned int)*height());
	temp = (unsigned int *)malloc(sizeof(unsigned int)*height());
	for(y=0; y < height(); ++y)
	{
		p = (unsigned int *)scanLine(y);
		q = (unsigned int *)dest.scanLine(y);
		blurScanLine(kernel, widthk, p, q, width());
	}
	unsigned int **srcTable = (unsigned int **)jumpTable();
	unsigned int **destTable = (unsigned int **)dest.jumpTable();
	for(x=0; x < width(); ++x)
	{
		for(y=0; y < height(); ++y)
		{
			scanline[y] = srcTable[y][x];
		}
		blurScanLine(kernel, widthk, scanline, temp, height());
		for(y=0; y < height(); ++y)
		{
			destTable[y][x] = temp[y];
		}
	}
	liberateMemory((void **) &scanline);
	liberateMemory((void **) &temp);
	liberateMemory((void **) &kernel);
	for( int yi=0; yi < dest.height(); ++yi )
	{
		QRgb *s = (QRgb*)(dest.scanLine( yi ));
		QRgb *d = (QRgb*)(scanLine( yi ));
		for(int xi=0; xi < dest.width(); ++xi )
		{
			(*d) = (*s);
			s++;
			d++;
		}
	}
	return;
}

bool ScImage::convolveImage(QImage *dest, const unsigned int order, const double *kernel)
{
	long widthk;
	double red, green, blue, alpha;
	double normalize, *normal_kernel;
	register const double *k;
	register unsigned int *q;
	int x, y, mx, my, sx, sy;
	long i;
	int mcx, mcy;
	widthk = order;
	if((widthk % 2) == 0)
		return(false);
	normal_kernel = (double *)malloc(widthk*widthk*sizeof(double));
	if(!normal_kernel)
		return(false);
	dest->reset();
	dest->create(width(), height(), 32);
	normalize=0.0;
	for(i=0; i < (widthk*widthk); i++)
		normalize += kernel[i];
	if(fabs(normalize) <= 1.0e-12)
		normalize=1.0;
	normalize=1.0/normalize;
	for(i=0; i < (widthk*widthk); i++)
		normal_kernel[i] = normalize*kernel[i];
	unsigned int **jumpTablek = (unsigned int **)jumpTable();
	for(y=0; y < dest->height(); ++y)
	{
		sy = y-(widthk/2);
		q = (unsigned int *)dest->scanLine(y);
		for(x=0; x < dest->width(); ++x)
		{
			k = normal_kernel;
			red = green = blue = alpha = 0;
			sy = y-(widthk/2);
			for(mcy=0; mcy < widthk; ++mcy, ++sy)
			{
				my = sy < 0 ? 0 : sy > height()-1 ? height()-1 : sy;
				sx = x+(-widthk/2);
				for(mcx=0; mcx < widthk; ++mcx, ++sx)
				{
					mx = sx < 0 ? 0 : sx > width()-1 ? width()-1 : sx;
					red += (*k)*(qRed(jumpTablek[my][mx])*257);
					green += (*k)*(qGreen(jumpTablek[my][mx])*257);
					blue += (*k)*(qBlue(jumpTablek[my][mx])*257);
					alpha += (*k)*(qAlpha(jumpTablek[my][mx])*257);
					++k;
				}
			}
			red = red < 0 ? 0 : red > 65535 ? 65535 : red+0.5;
			green = green < 0 ? 0 : green > 65535 ? 65535 : green+0.5;
			blue = blue < 0 ? 0 : blue > 65535 ? 65535 : blue+0.5;
			alpha = alpha < 0 ? 0 : alpha > 65535 ? 65535 : alpha+0.5;
			*q++ = qRgba((unsigned char)(red/257UL),
			             (unsigned char)(green/257UL),
			             (unsigned char)(blue/257UL),
			             (unsigned char)(alpha/257UL));
		}
	}
	free(normal_kernel);
	return(true);
}

int ScImage::getOptimalKernelWidth(double radius, double sigma)
{
	double normalize, value;
	long width;
	register long u;
	assert(sigma != 0.0);
	if(radius > 0.0)
		return((int)(2.0*ceil(radius)+1.0));
	for(width=5; ;)
	{
		normalize=0.0;
		for(u=(-width/2); u <= (width/2); u++)
			normalize+=exp(-((double) u*u)/(2.0*sigma*sigma))/(2.50662827463100024161235523934010416269302368164062*sigma);
		u=width/2;
		value=exp(-((double) u*u)/(2.0*sigma*sigma))/(2.50662827463100024161235523934010416269302368164062*sigma)/normalize;
		if((long)(65535*value) <= 0)
			break;
		width+=2;
	}
	return((int)width-2);
}

void ScImage::sharpen(double radius, double sigma)
{
	double alpha, normalize, *kernel;
	int widthk;
	register long i, u, v;
	QImage dest;
	if(sigma == 0.0)
		return;
	widthk = getOptimalKernelWidth(radius, sigma);
	if(width() < widthk)
		return;
	kernel = (double *)malloc(widthk*widthk*sizeof(double));
	if(!kernel)
		return;
	i = 0;
	normalize=0.0;
	for (v=(-widthk/2); v <= (widthk/2); v++)
	{
		for (u=(-widthk/2); u <= (widthk/2); u++)
		{
			alpha=exp(-((double) u*u+v*v)/(2.0*sigma*sigma));
			kernel[i]=alpha/(2.0*3.14159265358979323846264338327950288419716939937510*sigma*sigma);
			normalize+=kernel[i];
			i++;
		}
	}
	kernel[i/2]=(-2.0)*normalize;
	convolveImage(&dest, widthk, kernel);
	liberateMemory((void **) &kernel);
	for( int yi=0; yi < dest.height(); ++yi )
	{
		QRgb *s = (QRgb*)(dest.scanLine( yi ));
		QRgb *d = (QRgb*)(scanLine( yi ));
		for(int xi=0; xi < dest.width(); ++xi )
		{
			(*d) = (*s);
			s++;
			d++;
		}
	}
	return;
}

void ScImage::contrast(int contrastValue, bool cmyk)
{
	curveTable.resize(256);
	QPoint p1(0,0 - contrastValue);
	QPoint p2(256, 256 + contrastValue);
	double mc = (p1.y() - p2.y()) / (double)(p1.x() - p2.x());
	for (int i = 0; i < 256; ++i)
	{
		curveTable[i] = QMIN(255, QMAX(0, int(i * mc) + p1.y()));
	}
	applyCurve(cmyk);
}

void ScImage::brightness(int brightnessValue, bool cmyk)
{
	curveTable.resize(256);
	QPoint p1(0,0 + brightnessValue);
	QPoint p2(256, 256 + brightnessValue);
	double mc = (p1.y() - p2.y()) / (double)(p1.x() - p2.x());
	for (int i = 0; i < 256; ++i)
	{
		curveTable[i] = QMIN(255, QMAX(0, int(i * mc) + p1.y()));
	}
	applyCurve(cmyk);
}

void ScImage::applyCurve(bool cmyk)
{
	int h = height();
	int w = width();
	for( int yi=0; yi < h; ++yi )
	{
		QRgb * s = (QRgb*)(scanLine( yi ));
		for( int xi=0; xi < w; ++xi )
		{
			QRgb r=*s;
			int c, m, y, k;
			if (cmyk)
			{
				unsigned char *p = (unsigned char *) s;
				unsigned char rc = 255 - QMIN(255, p[0] + p[3]);
				unsigned char gc = 255 - QMIN(255, p[1] + p[3]);
				unsigned char bc = 255 - QMIN(255, p[2] + p[3]);
				c = 255 - curveTable[rc];
				m = 255 - curveTable[gc];
				y = 255 - curveTable[bc];
				k = QMIN(QMIN(c, m), y);
				*s = qRgba(y - k, m - k, c - k, k );
			}
			else
			{
				c = curveTable[qRed(r)];
				m = curveTable[qGreen(r)];
				y = curveTable[qBlue(r)];
				k = qAlpha(r);
				*s = qRgba(c, m, y, k);
			}
			s++;
		}
	}
}

void ScImage::colorize(ScColor color, int shade, bool cmyk)
{
	int h = height();
	int w = width();
	int cc, cm, cy, ck;
	int hu, sa, v;
	ScColor tmp2;
	QColor tmpR;
	if (cmyk)
		color.getShadeColorCMYK(&cc, &cm, &cy, &ck, shade);
	else
	{
		ck = 0;
		color.getShadeColorRGB(&cc, &cm, &cy, shade);
	}
	for( int yi=0; yi < h; ++yi )
	{
		QRgb * s = (QRgb*)(scanLine( yi ));
		for( int xi=0; xi < w; ++xi )
		{
			QRgb r=*s;
			if (cmyk)
			{
				double k = QMIN(qRound(0.3 * qRed(r) + 0.59 * qGreen(r) + 0.11 * qBlue(r) + qAlpha(r)), 255) / 255.0;
				*s = qRgba(QMIN(qRound(cc*k), 255), QMIN(qRound(cm*k), 255), QMIN(qRound(cy*k), 255), QMIN(qRound(ck*k), 255));
			}
			else
			{
				int k = 255 - QMIN(qRound(0.3 * qRed(r) + 0.59 * qGreen(r) + 0.11 * qBlue(r)), 255);
				int cc2, cm2, cy2;
				tmpR.setRgb(cc, cm, cy);
				tmpR.hsv(&hu, &sa, &v);
				if (cc == cm && cm == cy)
					tmpR.setHsv(hu, sa, 255 - ((255 - v) * k / 255));
				else
					tmpR.setHsv(hu, sa * k / 255, v);
				tmpR.getRgb(&cc2, &cm2, &cy2);
				int a = qAlpha(r);
				*s = qRgba(cc2, cm2, cy2, a);
			}
			s++;
		}
	}
}

void ScImage::invert(bool cmyk)
{
	int h = height();
	int w = width();
	for( int yi=0; yi < h; ++yi )
	{
		QRgb * s = (QRgb*)(scanLine( yi ));
		for( int xi=0; xi < w; ++xi )
		{
			if (cmyk)
			{
				unsigned char *p = (unsigned char *) s;
				unsigned char c, m, y, k;
				c = 255 - QMIN(255, p[0] + p[3]);
				m = 255 - QMIN(255, p[1] + p[3]);
				y = 255 - QMIN(255, p[2] + p[3]);
				k = QMIN(QMIN(c, m), y);
				p[0] = c - k;
				p[1] = m - k;
				p[2] = y - k;
				p[3] = k;
			}
			else
				*s ^= 0x00ffffff;
			s++;
		}
	}
}

void ScImage::toGrayscale(bool cmyk)
{
	int h = height();
	int w = width();
	for( int yi=0; yi < h; ++yi )
	{
		QRgb * s = (QRgb*)(scanLine( yi ));
		for( int xi=0; xi < w; ++xi )
		{
			QRgb r=*s;
			int k;
			if (cmyk)
			{
				k = QMIN(qRound(0.3 * qRed(r) + 0.59 * qGreen(r) + 0.11 * qBlue(r) + qAlpha(r)), 255);
				*s = qRgba(0, 0, 0, k);
			}
			else
			{
				k = QMIN(qRound(0.3 * qRed(r) + 0.59 * qGreen(r) + 0.11 * qBlue(r)), 255);
				int a = qAlpha(r);
				*s = qRgba(k, k, k, a);
			}
			s++;
		}
	}
}

void ScImage::swapRGBA()
{
	for (int i = 0; i < height(); ++i)
	{
		unsigned int *ptr = (unsigned int *) scanLine(i);
		unsigned char r, b;
		for (int j = 0; j < width(); ++j)
		{
			unsigned char *p = (unsigned char *) ptr;
			r = p[0];
			b = p[2];
			p[2] = r;
			p[0] = b;
			ptr++;
		}
	}
}

void ScImage::createLowRes(double scale)
{
	int w = qRound(width() / scale);
	int h = qRound(height() / scale);
	QImage tmp = smoothScale(w, h);
	create(w, h, 32);
	for( int yi=0; yi < tmp.height(); ++yi )
	{
		QRgb *s = (QRgb*)(tmp.scanLine( yi ));
		QRgb *d = (QRgb*)(scanLine( yi ));
		for(int xi=0; xi < tmp.width(); ++xi )
		{
			(*d) = (*s);
			s++;
			d++;
		}
	}
}

void ScImage::Convert2JPG(QString fn, int Quality, bool isCMYK, bool isGray)
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
	if ((outfile = fopen (fn.local8Bit(), "wb")) == NULL)
		return;
	jpeg_stdio_dest (&cinfo, outfile);
	cinfo.image_width  = width();
	cinfo.image_height = height();
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
	jpeg_start_compress (&cinfo, true);
	row_pointer[0] = new uchar[cinfo.image_width*cinfo.input_components];
	int w = cinfo.image_width;
	while (cinfo.next_scanline < cinfo.image_height)
	{
		uchar *row = row_pointer[0];
		if (isCMYK)
		{
			QRgb* rgba = (QRgb*)scanLine(cinfo.next_scanline);
			for (int i=0; i<w; ++i)
			{
				*row++ = qRed(*rgba);
				*row++ = qGreen(*rgba);
				*row++ = qBlue(*rgba);
				*row++ = qAlpha(*rgba);
				++rgba;
			}
		}
		else
		{
			if (isGray)
			{
				QRgb* rgba = (QRgb*)scanLine(cinfo.next_scanline);
				for (int i=0; i<w; ++i)
				{
					*row++ = qRed(*rgba);
					++rgba;
				}
			}
			else
			{
				QRgb* rgb = (QRgb*)scanLine(cinfo.next_scanline);
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

QString ScImage::ImageToTxt()
{
	int i = 0;
	int h = height();
	int w = width();
	unsigned char u;
	QString ImgStr = "";
	ImgStr.reserve(3 * h * w);
	for( int yi=0; yi < h; ++yi )
	{
		QRgb * s = (QRgb*)(scanLine( yi ));
		for( int xi=0; xi < w; ++xi )
		{
			QRgb r=*s++;
			u=qRed(r);
			ImgStr[i++] = u;
			u=qGreen(r);
			ImgStr[i++] = u;
			u=qBlue(r);
			ImgStr[i++] = u;
		}
	}
	return ImgStr;
}

QString ScImage::ImageToCMYK()
{
	int i = 0;
	int h = height();
	int w = width();
	QString ImgStr = "";
	ImgStr.reserve(4 * h * w);
	for( int yi=0; yi < h; ++yi )
	{
		QRgb * s = (QRgb*)(scanLine( yi ));
		for( int xi=0; xi < w; ++xi )
		{
			QRgb r=*s;
			int c = 255 - qRed(r);
			int m = 255 - qGreen(r);
			int y = 255 - qBlue(r);
			int k = QMIN(QMIN(c, m), y);
			*s = qRgba(m - k, y - k, k, c - k);
			ImgStr[i++] = static_cast<char> (c - k);
			ImgStr[i++] = static_cast<char> (m - k);
			ImgStr[i++] = static_cast<char> (y - k);
			ImgStr[i++] = static_cast<char> (k);
			s++;
		}
	}
	return ImgStr;
}

QString ScImage::ImageToGray()
{
	int i = 0;
	int h = height();
	int w = width();
	QString ImgStr = "";
	ImgStr.reserve( h * w);
	for( int yi=0; yi < h; ++yi )
	{
		QRgb * s = (QRgb*)(scanLine( yi ));
		for( int xi=0; xi < w; ++xi )
		{
			QRgb r=*s;
			int k = QMIN(qRound(0.3 * qRed(r) + 0.59 * qGreen(r) + 0.11 * qBlue(r)), 255);
			*s = qRgba(k, 0, 0, 0);
			ImgStr[i++] = k;
			s++;
		}
	}
	return ImgStr;
}

QString ScImage::ImageToCMYK_PDF(bool pre)
{
	int i = 0;
	int h = height();
	int w = width();
	QString ImgStr = "";
	ImgStr.reserve( 4 * h * w );
	if (pre)
	{
		for( int yi=0; yi < h; ++yi )
		{
			QRgb * s = (QRgb*)(scanLine( yi ));
			for( int xi=0; xi < w; ++xi )
			{
				QRgb r=*s;
				int c = qRed(r);
				int m = qGreen(r);
				int y = qBlue(r);
				int k = qAlpha(r);
				/*				*s = qRgba(m, y, k, c); */
				ImgStr[i++] = static_cast<unsigned char> (c);
				ImgStr[i++] = static_cast<unsigned char> (m);
				ImgStr[i++] = static_cast<unsigned char> (y);
				ImgStr[i++] = static_cast<unsigned char> (k);
				s++;
			}
		}
	}
	else
	{
		for( int yi=0; yi < h; ++yi )
		{
			QRgb * s = (QRgb*)(scanLine( yi ));
			for( int xi=0; xi < w; ++xi )
			{
				QRgb r=*s;
				int c = 255 - qRed(r);
				int m = 255 - qGreen(r);
				int y = 255 - qBlue(r);
				int k = QMIN(QMIN(c, m), y);
				//				*s = qRgba(m, y, k, c);
//				*s = qRgba(c, m, y, k);
				ImgStr[i++] = static_cast<unsigned char> (c - k);
				ImgStr[i++] = static_cast<unsigned char> (m - k);
				ImgStr[i++] = static_cast<unsigned char> (y - k);
				ImgStr[i++] = static_cast<unsigned char> (k);
				s++;
			}
		}
	}
	return ImgStr;
}

QString ScImage::ImageToCMYK_PS(int pl, bool pre)
{
	int i = 0;
	int h = height();
	int w = width();
	QString ImgStr = "";
	if(pl == -1)
		ImgStr.reserve(4 * h * w);
	else
		ImgStr.reserve(h * w);
	if (pre)
	{
		for( int yi=0; yi < h; ++yi )
		{
			QRgb * s = (QRgb*)(scanLine( yi ));
			for( int xi=0; xi < w; ++xi )
			{
				QRgb r=*s++;
				int c = qRed(r);
				int m = qGreen(r);
				int y = qBlue(r);
				int k = qAlpha(r);
				if (pl == -1)
				{
					ImgStr[i++] = static_cast<unsigned char> (c);
					ImgStr[i++] = static_cast<unsigned char> (m);
					ImgStr[i++] = static_cast<unsigned char> (y);
					ImgStr[i++] = static_cast<unsigned char> (k);
				}
				else
				{
					if (pl == -2)
						ImgStr[i++] = static_cast<unsigned char> (QMIN(255, qRound(0.3 * c + 0.59 * m + 0.11 * y + k)));
					if (pl == 1)
						ImgStr[i++] = static_cast<unsigned char> (c);
					if (pl == 2)
						ImgStr[i++] = static_cast<unsigned char> (m);
					if (pl == 3)
						ImgStr[i++] = static_cast<unsigned char> (y);
					if (pl == 0)
						ImgStr[i++] = static_cast<unsigned char> (k);
				}
			}
		}
	}
	else
	{
		for( int yi=0; yi < h; ++yi )
		{
			QRgb * s = (QRgb*)(scanLine( yi ));
			for( int xi=0; xi < w; ++xi )
			{
				QRgb r=*s++;
				int c = 255 - qRed(r);
				int m = 255 - qGreen(r);
				int y = 255 - qBlue(r);
				int k = QMIN(QMIN(c, m), y);
				if (pl == -1)
				{
					ImgStr[i++] = static_cast<unsigned char> (c - k);
					ImgStr[i++] = static_cast<unsigned char> (m - k);
					ImgStr[i++] = static_cast<unsigned char> (y - k);
					ImgStr[i++] = static_cast<unsigned char> (k);
				}
				else
				{
					if (pl == -2)
						ImgStr[i++] = static_cast<unsigned char> (QMIN(255, qRound(0.3 * c + 0.59 * m + 0.11 * y + k)));
					if (pl == 1)
						ImgStr[i++] = static_cast<unsigned char> (c - k);
					if (pl == 2)
						ImgStr[i++] = static_cast<unsigned char> (m - k);
					if (pl == 3)
						ImgStr[i++] = static_cast<unsigned char> (y - k);
					if (pl == 0)
						ImgStr[i++] = static_cast<unsigned char> (k);
				}
			}
		}
	}
	return ImgStr;
}

QString ScImage::MaskToTxt(bool PDF)
{
	int i = 0;
	int h = height();
	int w = width();
	int w2;
	unsigned char u;
	w2 = w / 8;
	if ((w % 8) != 0)
		w2++;
	QString ImgStr = "";
	ImgStr.reserve(h * w2);
	for( int yi=0; yi < h; ++yi )
	{
		uchar * s = scanLine( yi );
		for( int xi=0; xi < w2; ++xi )
		{
			u = *(s+xi);
			if(PDF) u = ~u;
			ImgStr[i++] = u;
		}
	}
	return ImgStr;
}

QString ScImage::MaskToTxt14()
{
	int i = 0;
	int h = height();
	int w = width();
	unsigned char u;
	QString ImgStr = "";
	ImgStr.reserve(h * w);
	for( int yi=0; yi < h; ++yi )
	{
		QRgb * s = (QRgb*)(scanLine( yi ));
		for( int xi=0; xi < w; ++xi )
		{
			QRgb r=*s++;
			u = qAlpha(r);
			ImgStr[i++] = u;
		}
	}
	return ImgStr;
}

// Check that the header is a valid PSD.
bool ScImage::IsValid( const PSDHeader & header )
{
	if( header.signature != 0x38425053 )
		return false;
	return true;
}

// Check that the header is supported.
bool ScImage::IsSupported( const PSDHeader & header )
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

unsigned char ScImage::INT_MULT ( unsigned char a, unsigned char b )
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
void ScImage::RGBTOHSV ( uchar& red, uchar& green, uchar& blue )
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
void ScImage::HSVTORGB ( uchar& hue, uchar& saturation, uchar& value )
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
void ScImage::RGBTOHLS ( uchar& red, uchar& green, uchar& blue )
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
int ScImage::HLSVALUE ( double n1, double n2, double hue )
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
void ScImage::HLSTORGB ( uchar& hue, uchar& lightness, uchar& saturation )
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

bool ScImage::loadLayerChannels( QDataStream & s, const PSDHeader & header, QValueList<PSDLayer> &layerInfo, uint layer, bool* firstLayer)
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
					if(s.atEnd())
						return false;
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
	if ((imgInfo.isRequest) && (imgInfo.RequestProps.contains(layer)))
		visible = imgInfo.RequestProps[layer].visible;
	if (visible)
	{
		if (*firstLayer)
		{
			for( int yi=0; yi < tmpImg.height(); ++yi )
			{
				QRgb *s = (QRgb*)(tmpImg.scanLine( yi ));
				QRgb *d = (QRgb*)(scanLine( yi ));
				for(int xi=0; xi < tmpImg.width(); ++xi )
				{
					(*d) = (*s);
					s++;
					d++;
				}
			}
			*firstLayer = false;
		}
		else
		{
			for (int i = 0; i < layerInfo[layer].height; i++)
			{
				unsigned int *dst = (unsigned int *)scanLine(QMIN(QMAX(layerInfo[layer].ypos, 0)+i, height()-1));
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
					if ((imgInfo.isRequest) && (imgInfo.RequestProps.contains(layer)))
						layBlend = imgInfo.RequestProps[layer].blend;
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
					if ((imgInfo.isRequest) && (imgInfo.RequestProps.contains(layer)))
						layOpa = imgInfo.RequestProps[layer].opacity;
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

bool ScImage::loadLayer( QDataStream & s, const PSDHeader & header )
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
	if (channel_num < 4)
	{
		for (int i = 0; i < height(); i++)
		{
			QRgb * s = (QRgb*)(scanLine( i ));
			for (int j = 0; j < width(); j++)
			{
				*s++ = qRgba(0, 0, 0, 255);
			}
		}
	}
	channel_num = 4;
	const uint pixel_count = header.height * header.width;
	static const uint components[4] = {2, 1, 0, 3};
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
			uchar * ptr = bits() + components[channel];
			uint count = 0;
			while( count < pixel_count )
			{
				uchar c;
				if(s.atEnd())
					return false;
				s >> c;
				uint len = c;
				if( len < 128 )
				{
					// Copy next len+1 bytes literally.
					len++;
					count += len;
					if ( count > pixel_count )
						return false;
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
					if(s.atEnd() || count > pixel_count)
						return false;
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
			uchar * ptr = bits() + components[channel];
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

QString ScImage::getLayerString(QDataStream & s)
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

QString ScImage::getPascalString(QDataStream & s)
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

void ScImage::parseRessourceData( QDataStream & s, const PSDHeader & header, uint size )
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
	while ((offset + 6)< size)
	{
		s >> signature;
		offset += 4;
		if(((signature >> 24)&0xff) != '8' ||
		        ((signature >> 16)&0xff) != 'B' ||
		        ((signature >> 8)&0xff) != 'I' ||
		        ((signature )&0xff) != 'M' )
			break;
		s >> resID;
		offset += 2;
		adj = s.device()->at();
		resName = getPascalString(s);
		offset += s.device()->at() - adj;
		s >> resSize;
		if(offset + resSize > size)
			break;
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
				case 2:
					if (first)
					{
						firstControl = FPoint(frac2 * header.width, frac1 * header.height);
						firstPoint = FPoint(frac4 * header.width, frac3 * header.height);
						clip2.addPoint(FPoint(frac4 * header.width, frac3 * header.height));
						clip2.addPoint(FPoint(frac6 * header.width, frac5 * header.height));
					}
					else
					{
						clip2.addPoint(frac4 * header.width, frac3 * header.height);
						clip2.addPoint(frac2 * header.width, frac1 * header.height);
						clip2.addPoint(frac4 * header.width, frac3 * header.height);
						clip2.addPoint(frac6 * header.width, frac5 * header.height);
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
			imgInfo.PDSpathData.insert(resName, clip2.copy());
		}
		else
		{
			switch (resID)
			{
			case 0x0bb7:
				adj = s.device()->at();
				imgInfo.clipPath = getPascalString(s);
				offset += s.device()->at() - adj;
				break;
			case 0x03ed:
				s >> hRes;
				s >> hResUnit;
				s >> dummyW;
				s >> vRes;
				s >> vResUnit;
				s >> dummyW;
				imgInfo.xres = qRound(hRes / 65536.0);
				imgInfo.yres = qRound(vRes / 65536.0);
				break;
			case 0x040f:
				{
					icclen = resSize;
					char* buffer = (char*)malloc(resSize);
					iccbuf = buffer;
					s.readRawBytes(buffer, resSize);
				}
				break;
			case 0x0409:
			case 0x040C:
				{
					uint thdummy, thsize;
					s >> thdummy;
					s >> thdummy;
					s >> thdummy;
					s >> thdummy;
					s >> thdummy;
					s >> thsize;
					s >> thdummy;
					char* buffer = (char*)malloc(thsize);
					s.readRawBytes(buffer, thsize);
					QImage imth;
					imth.loadFromData((const uchar*)buffer, thsize, "JPEG");
					imth.convertDepth(32);
					if (resID == 0x0409)
						imgInfo.exifInfo.thumbnail = imth.swapRGB();
					else
						imgInfo.exifInfo.thumbnail = imth;
					imgInfo.exifDataValid = true;
					free(buffer);
				}
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
	if(offset<size)
		s.device()->at( size );
}

bool ScImage::parseLayer( QDataStream & s, const PSDHeader & header )
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
			imgInfo.layerInfo.append(lay);
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
			loadLayerChannels( s, header, imgInfo.layerInfo, layer, &firstLayer );
		}
	}
	else
	{
		s >> numLayers;
		loadLayer( s, header);
	}
	return true;
}

// Load the PSD image.
bool ScImage::LoadPSD( QDataStream & s, const PSDHeader & header)
{
	// Create dst image.
	if( !create( header.width, header.height, 32 ))
		return false;
	setAlphaBuffer( true );
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
		parseRessourceData(s, header, ressourceDataLen);
	if  ((!imgInfo.exifInfo.thumbnail.isNull()) && (header.reserved[0] == 't'))
		return true;
	s.device()->at( startRessource + ressourceDataLen );
	// Skip the reserved data. FIX: Also incorrect, this is the actual Layer Data for Images with Layers
	s >> layerDataLen;
	startLayers = s.device()->at();
	if (layerDataLen != 0)
		return parseLayer( s, header);
	else
	{
		// Decoding simple psd file, no layers
		s.device()->at( s.device()->at() + layerDataLen );
		loadLayer( s, header);
	}
	return true;
}

void ScImage::scaleImage(int nwidth, int nheight)
{
	QImage dst;
	dst.create(nwidth, nheight,32);
	QRgb* xelrow = 0;
	QRgb* tempxelrow = 0;
	register QRgb* xP;
	register QRgb* nxP;
	int rows, cols, rowsread, newrows, newcols;
	register int row, col, needtoreadrow;
	const uchar maxval = 255;
	double xscale, yscale;
	long sxscale, syscale;
	register long fracrowtofill, fracrowleft;
	long* as;
	long* rs;
	long* gs;
	long* bs;
	int rowswritten = 0;
	cols = width();
	rows = height();
	newcols = dst.width();
	newrows = dst.height();
	long SCALE;
	long HALFSCALE;
	if (cols > 4096)
	{
		SCALE = 4096;
		HALFSCALE = 2048;
	}
	else
	{
		int fac = 4096;
		while (cols * fac > 4096)
		{
			fac /= 2;
		}
		SCALE = fac * cols;
		HALFSCALE = fac * cols / 2;
	}
	xscale = (double) newcols / (double) cols;
	yscale = (double) newrows / (double) rows;
	sxscale = (long)(xscale * SCALE);
	syscale = (long)(yscale * SCALE);
	if ( newrows != rows )	/* shortcut Y scaling if possible */
		tempxelrow = new QRgb[cols];
	as = new long[cols];
	rs = new long[cols];
	gs = new long[cols];
	bs = new long[cols];
	rowsread = 0;
	fracrowleft = syscale;
	needtoreadrow = 1;
	for ( col = 0; col < cols; ++col )
		rs[col] = gs[col] =  as[col] = bs[col] = HALFSCALE;
	fracrowtofill = SCALE;
	for ( row = 0; row < newrows; ++row )
	{
		if ( newrows == rows )
			tempxelrow = xelrow = (QRgb*)scanLine(rowsread++);
		else
		{
			while ( fracrowleft < fracrowtofill )
			{
				if ( needtoreadrow && rowsread < rows )
					xelrow = (QRgb*)scanLine(rowsread++);
				for ( col = 0, xP = xelrow; col < cols; ++col, ++xP )
				{
					as[col] += fracrowleft * qAlpha( *xP );
					rs[col] += fracrowleft * qRed( *xP );
					gs[col] += fracrowleft * qGreen( *xP );
					bs[col] += fracrowleft * qBlue( *xP );
				}
				fracrowtofill -= fracrowleft;
				fracrowleft = syscale;
				needtoreadrow = 1;
			}
			if ( needtoreadrow && rowsread < rows )
			{
				xelrow = (QRgb*)scanLine(rowsread++);
				needtoreadrow = 0;
			}
			register long a=0;
			for ( col = 0, xP = xelrow, nxP = tempxelrow; col < cols; ++col, ++xP, ++nxP )
			{
				register long r, g, b;
				a = as[col] + fracrowtofill * qAlpha( *xP );
				r = rs[col] + fracrowtofill * qRed( *xP );
				g = gs[col] + fracrowtofill * qGreen( *xP );
				b = bs[col] + fracrowtofill * qBlue( *xP );
				r /= SCALE;
				if ( r > maxval ) r = maxval;
				g /= SCALE;
				if ( g > maxval ) g = maxval;
				b /= SCALE;
				if ( b > maxval ) b = maxval;
				a /= SCALE;
				if ( a > maxval ) a = maxval;
				*nxP = qRgba( (int)r, (int)g, (int)b , (int)a);
				rs[col] = as[col] = gs[col] = bs[col] = HALFSCALE;
			}
			fracrowleft -= fracrowtofill;
			if ( fracrowleft == 0 )
			{
				fracrowleft = syscale;
				needtoreadrow = 1;
			}
			fracrowtofill = SCALE;
		}
		if ( newcols == cols )
			memcpy(dst.scanLine(rowswritten++), tempxelrow, newcols*4);
		else
		{
			register long a, r, g, b;
			register long fraccoltofill, fraccolleft = 0;
			register int needcol;
			nxP = (QRgb*)dst.scanLine(rowswritten++);
			fraccoltofill = SCALE;
			a = r = g = b = HALFSCALE;
			needcol = 0;
			for ( col = 0, xP = tempxelrow; col < cols; ++col, ++xP )
			{
				fraccolleft = sxscale;
				while ( fraccolleft >= fraccoltofill )
				{
					if ( needcol )
					{
						++nxP;
						a = r = g = b = HALFSCALE;
					}
					a += fraccoltofill * qAlpha( *xP );
					r += fraccoltofill * qRed( *xP );
					g += fraccoltofill * qGreen( *xP );
					b += fraccoltofill * qBlue( *xP );
					r /= SCALE;
					if ( r > maxval ) r = maxval;
					g /= SCALE;
					if ( g > maxval ) g = maxval;
					b /= SCALE;
					if ( b > maxval ) b = maxval;
					a /= SCALE;
					if ( a > maxval ) a = maxval;
					*nxP = qRgba( (int)r, (int)g, (int)b, (int)a );
					fraccolleft -= fraccoltofill;
					fraccoltofill = SCALE;
					needcol = 1;
				}
				if ( fraccolleft > 0 )
				{
					if ( needcol )
					{
						++nxP;
						a = r = g = b = HALFSCALE;
						needcol = 0;
					}
					a += fraccolleft * qAlpha( *xP );
					r += fraccolleft * qRed( *xP );
					g += fraccolleft * qGreen( *xP );
					b += fraccolleft * qBlue( *xP );
					fraccoltofill -= fraccolleft;
				}
			}
			if ( fraccoltofill > 0 )
			{
				--xP;
				a += fraccolleft * qAlpha( *xP );
				r += fraccoltofill * qRed( *xP );
				g += fraccoltofill * qGreen( *xP );
				b += fraccoltofill * qBlue( *xP );
			}
			if ( ! needcol )
			{
				r /= SCALE;
				if ( r > maxval ) r = maxval;
				g /= SCALE;
				if ( g > maxval ) g = maxval;
				b /= SCALE;
				if ( b > maxval ) b = maxval;
				a /= SCALE;
				if ( a > maxval ) a = maxval;
				*nxP = qRgba( (int)r, (int)g, (int)b, (int)a );
			}
		}
	}
	if ( newrows != rows && tempxelrow )// Robust, tempxelrow might be 0 1 day
		delete [] tempxelrow;
	if ( as )				// Avoid purify complaint
		delete [] as;
	if ( rs )				// Robust, rs might be 0 one day
		delete [] rs;
	if ( gs )				// Robust, gs might be 0 one day
		delete [] gs;
	if ( bs )				// Robust, bs might be 0 one day
		delete [] bs;
	create(nwidth, nheight,32);
	for( int yi=0; yi < dst.height(); ++yi )
	{
		QRgb *s = (QRgb*)(dst.scanLine( yi ));
		QRgb *d = (QRgb*)(scanLine( yi ));
		for(int xi=0; xi < dst.width(); ++xi )
		{
			(*d) = (*s);
			s++;
			d++;
		}
	}
	return;
}

QString ScImage::getAlpha(QString fn, bool PDF, bool pdf14, int gsRes)
{
	QString retS = "";
	float xres, yres;
	short resolutionunit = 0;
	imgInfo.valid = false;
	imgInfo.clipPath = "";
	imgInfo.PDSpathData.clear();
	imgInfo.layerInfo.clear();
	QFileInfo fi = QFileInfo(fn);
	if (!fi.exists())
		return retS;
	QString tmp, BBox, tmp2;
	QString ext = fi.extension(false).lower();
	QString tmpFile = QDir::convertSeparators(QDir::homeDirPath()+"/.scribus/sc.png");
	QString picFile = QDir::convertSeparators(fn);
	if ((ext == "jpg") || (ext == "jpeg"))
		return retS;
	double x, y, b, h;
	bool found = false;
	int retg = -1;
	QChar tc;
	if (ext == "pdf")
	{
		QStringList args;
		xres = gsRes;
		yres = gsRes;
		args.append("-r"+QString::number(gsRes));
		args.append("-sOutputFile=\""+tmpFile + "\"");
		args.append("-dFirstPage=1");
		args.append("-dLastPage=1");
		args.append("\""+picFile+"\"");
		retg = callGS(args);
		if (retg == 0)
		{
			load(tmpFile);
			unlink(tmpFile);
			setAlphaBuffer(true);
			if (ScApp->HavePngAlpha != 0)
			{
				for( int yi=0; yi < height(); ++yi )
				{
					QRgb *s = (QRgb*)(scanLine( yi ));
					for(int xi=0; xi < width(); ++xi )
					{
						if((*s) == 0xffffffff)
							(*s) &= 0x00ffffff;
						s++;
					}
				}
			}
		}
	}
	else if ((ext == "eps") || (ext == "ps"))
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
			args.append("-sOutputFile=\""+tmpFile+"\"");
			args.append("-g"+tmp.setNum(qRound(b))+"x"+tmp2.setNum(qRound(h)));
			args.append("\""+picFile+"\"");
			retg = callGS(args);
			if (retg == 0)
			{
				QImage image;
				image.load(tmpFile);
				image.setAlphaBuffer(true);
				if (ScApp->HavePngAlpha != 0)
				{
					int wi = image.width();
					int hi = image.height();
					for( int yi=0; yi < hi; ++yi )
					{
						QRgb *s = (QRgb*)(image.scanLine( yi ));
						QRgb alphaFF = qRgba(255,255,255,255);
						QRgb alpha00 = qRgba(255,255,255,  0);
						for(int xi=0; xi < wi; ++xi )
						{
							if((*s) == alphaFF)
								(*s) &= alpha00;
							s++;
						}
					}
				}
				*this = static_cast<ScImage>(image.copy(static_cast<int>(x), 0, static_cast<int>(b-x), static_cast<int>(h-y)));
				setAlphaBuffer(true);
				unlink(tmpFile);
			}
		}
	}
	else if ((ext == "tif") || (ext == "tiff"))
	{
#ifdef HAVE_TIFF
		TIFF* tif = TIFFOpen(fn.local8Bit(), "r");
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
			if (photometric == PHOTOMETRIC_SEPARATED)
			{
				TIFFClose(tif);
				return retS;
			}
			else
			{
				create(width,height,32);
				setAlphaBuffer(true);
				bits = (uint32 *) _TIFFmalloc(size * sizeof(uint32));
				if(bits)
				{
					if (TIFFReadRGBAImage(tif, width, height, bits, 0))
					{
						for(unsigned int y = 0; y < height; y++)
							memcpy(scanLine(height - 1 - y), bits + y * width, width * 4);
					}
					_TIFFfree(bits);
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
			if (header.color_mode == CM_CMYK)
				return retS;
			if( !LoadPSD(s, header) )
				return retS;
			f.close();
		}
		else
			return retS;
	}
	else
	{
		if (load(fn))
		{
			*this = convertDepth(32);
 			 if (hasAlphaBuffer())
				setAlphaBuffer(true);
			else
				return retS;
		}
		else
			return retS;
	}
	if (isNull())
		return retS;
	int i = 0;
	unsigned char u;
	int hm = height();
	int wm = width();
	int w2;
	if (pdf14)
	{
		retS.reserve(hm * wm);
		for( int yi=0; yi < hm; ++yi )
		{
			QRgb * s = (QRgb*)(scanLine( yi ));
			for( int xi=0; xi < wm; ++xi )
			{
				QRgb r=*s++;
				u=qAlpha(r);
				retS[i++] = u;
			}
		}
	}
	else
	{
		QImage iMask = createAlphaMask();
		hm = iMask.height();
		wm = iMask.width();
		w2 = wm / 8;
		if ((wm % 8) != 0)
			w2++;
		retS.reserve(hm * w2);
		for( int yi=0; yi < hm; ++yi )
		{
			uchar * s = iMask.scanLine( yi );
			for( int xi=0; xi < w2; ++xi )
			{
				u = *(s+xi);
				if(PDF) u = ~u;
				retS[i++] = u;
			}
		}
	}
	return retS;
}

#define ICC_MARKER  (JPEG_APP0 + 2)	/* JPEG marker code for ICC */
#define PHOTOSHOP_MARKER  (JPEG_APP0 + 13)	/* JPEG marker code for PHOTOSHOP */
#define ICC_OVERHEAD_LEN  14		/* size of non-profile data in APP2 */
#define MAX_BYTES_IN_MARKER  65533	/* maximum data len of a JPEG marker */
#define MAX_DATA_BYTES_IN_MARKER  (MAX_BYTES_IN_MARKER - ICC_OVERHEAD_LEN)

bool ScImage::marker_is_icc (jpeg_saved_marker_ptr marker)
{
	return
	    marker->marker == ICC_MARKER &&
	    marker->data_length >= ICC_OVERHEAD_LEN &&
	    /* verify the identifying string */
	    GETJOCTET(marker->data[0]) == 0x49 &&
	    GETJOCTET(marker->data[1]) == 0x43 &&
	    GETJOCTET(marker->data[2]) == 0x43 &&
	    GETJOCTET(marker->data[3]) == 0x5F &&
	    GETJOCTET(marker->data[4]) == 0x50 &&
	    GETJOCTET(marker->data[5]) == 0x52 &&
	    GETJOCTET(marker->data[6]) == 0x4F &&
	    GETJOCTET(marker->data[7]) == 0x46 &&
	    GETJOCTET(marker->data[8]) == 0x49 &&
	    GETJOCTET(marker->data[9]) == 0x4C &&
	    GETJOCTET(marker->data[10]) == 0x45 &&
	    GETJOCTET(marker->data[11]) == 0x0;
}

bool ScImage::marker_is_photoshop (jpeg_saved_marker_ptr marker)
{
	return
	    marker->marker == PHOTOSHOP_MARKER &&
	    marker->data_length >= ICC_OVERHEAD_LEN &&
	    /* verify the identifying string */
	    GETJOCTET(marker->data[0]) == 0x50 &&
	    GETJOCTET(marker->data[1]) == 0x68 &&
	    GETJOCTET(marker->data[2]) == 0x6F &&
	    GETJOCTET(marker->data[3]) == 0x74 &&
	    GETJOCTET(marker->data[4]) == 0x6F &&
	    GETJOCTET(marker->data[5]) == 0x73 &&
	    GETJOCTET(marker->data[6]) == 0x68 &&
	    GETJOCTET(marker->data[7]) == 0x6F &&
	    GETJOCTET(marker->data[8]) == 0x70 &&
	    GETJOCTET(marker->data[9]) == 0x20 &&
	    GETJOCTET(marker->data[10]) == 0x33 &&
	    GETJOCTET(marker->data[11]) == 0x2E &&
	    GETJOCTET(marker->data[12]) == 0x30 &&
	    GETJOCTET(marker->data[13]) == 0x0;
}
/* Small modification of original read_icc_profile method from jpegicc of lcms project
 * to enable read of Photoshop marker 
 */
bool ScImage::read_jpeg_marker (UINT8 requestmarker, j_decompress_ptr cinfo, JOCTET **icc_data_ptr, unsigned int *icc_data_len)
{
	jpeg_saved_marker_ptr marker;
	int num_markers = 0;
	int seq_no;
	JOCTET *icc_data;
	unsigned int total_length;
#define MAX_SEQ_NO  255		/* sufficient since marker numbers are bytes */
	char marker_present[MAX_SEQ_NO+1];	  /* 1 if marker found */
	unsigned int data_length[MAX_SEQ_NO+1]; /* size of profile data in marker */
	unsigned int data_offset[MAX_SEQ_NO+1]; /* offset for data in marker */

	*icc_data_ptr = NULL;		/* avoid confusion if false return */
	*icc_data_len = 0;

	/* This first pass over the saved markers discovers whether there are
	 * any ICC markers and verifies the consistency of the marker numbering.
	 */

	for (seq_no = 1; seq_no <= MAX_SEQ_NO; seq_no++)
		marker_present[seq_no] = 0;
	seq_no = 0;
	for (marker = cinfo->marker_list; marker != NULL; marker = marker->next)
	{
		if (requestmarker == ICC_MARKER && marker_is_icc(marker))
		{
			if (num_markers == 0)
				num_markers = GETJOCTET(marker->data[13]);
			else if (num_markers != GETJOCTET(marker->data[13]))
				return false;		/* inconsistent num_markers fields */
			seq_no = GETJOCTET(marker->data[12]);
			if (seq_no <= 0 || seq_no > num_markers)
				return false;		/* bogus sequence number */
			if (marker_present[seq_no])
				return false;		/* duplicate sequence numbers */
			marker_present[seq_no] = 1;
			data_length[seq_no] = marker->data_length - ICC_OVERHEAD_LEN;
		}
		else if(requestmarker == PHOTOSHOP_MARKER && marker_is_photoshop(marker))
		{
			num_markers = ++seq_no;
			marker_present[seq_no] = 1;
			data_length[seq_no] = marker->data_length - ICC_OVERHEAD_LEN;
		}
	}

	if (num_markers == 0)
		return false;

	/* Check for missing markers, count total space needed,
	 * compute offset of each marker's part of the data.
	 */

	total_length = 0;
	for (seq_no = 1; seq_no <= num_markers; seq_no++)
	{
		if (marker_present[seq_no] == 0)
			return false;		/* missing sequence number */
		data_offset[seq_no] = total_length;
		total_length += data_length[seq_no];
	}

	if (total_length <= 0)
		return false;		/* found only empty markers? */

	/* Allocate space for assembled data */
	icc_data = (JOCTET *) malloc(total_length * sizeof(JOCTET));
	if (icc_data == NULL)
		return false;		/* oops, out of memory */
	seq_no=0;
	/* and fill it in */
	for (marker = cinfo->marker_list; marker != NULL; marker = marker->next)
	{
		if ( (requestmarker == ICC_MARKER && marker_is_icc(marker)) ||
		        (requestmarker == PHOTOSHOP_MARKER && marker_is_photoshop(marker)) || (requestmarker == 0xE1))
		{
			JOCTET FAR *src_ptr;
			JOCTET *dst_ptr;
			unsigned int length;
			if(requestmarker == ICC_MARKER)
				seq_no = GETJOCTET(marker->data[12]);
			else if(requestmarker == PHOTOSHOP_MARKER)
				seq_no++;
			dst_ptr = icc_data + data_offset[seq_no];
			src_ptr = marker->data + ICC_OVERHEAD_LEN;
			length = data_length[seq_no];
			while (length--)
			{
				*dst_ptr++ = *src_ptr++;
			}
		}
	}

	*icc_data_ptr = icc_data;
	*icc_data_len = total_length;

	return true;
}

void ScImage::getEmbeddedProfile(QString fn, QString *profile, int *components)
{
#ifdef HAVE_CMS
	cmsHPROFILE tiffProf = 0;
	QFileInfo fi = QFileInfo(fn);
	if (!fi.exists())
		return;
	QString ext = fi.extension(false).lower();
	iccbuf = 0;
	icclen = 0;
	if (ext == "psd")
	{
		QFile f(fn);
		if (f.open(IO_ReadOnly))
		{
			imgInfo.xres = 72;
			imgInfo.yres = 72;
			QDataStream s( &f );
			s.setByteOrder( QDataStream::BigEndian );
			PSDHeader header;
			s >> header;
			// Check image file format.
			if( s.atEnd() || !IsValid( header ) )
				return;
			// Check if it's a supported format.
			if( !IsSupported( header ) )
				return;
			if( !LoadPSD(s, header) )
				return;
			if (icclen > 0)
			{
				tiffProf = cmsOpenProfileFromMem(iccbuf, icclen);
				if (tiffProf)
				{
					if (static_cast<int>(cmsGetColorSpace(tiffProf)) == icSigRgbData)
						*components = 3;
					if (static_cast<int>(cmsGetColorSpace(tiffProf)) == icSigCmykData)
						*components = 4;
					for (uint el = 0; el < icclen; ++el)
						*profile += iccbuf[el];
				}
				cmsCloseProfile(tiffProf);
				free(iccbuf);
			}
			f.close();
		}
		else
			return;
	}
#ifdef HAVE_TIFF
	else if ((ext == "tif") || (ext == "tiff"))
	{
		TIFF* tif = TIFFOpen(fn.local8Bit(), "r");
		if(tif)
		{
			DWORD EmbedLen = 0;
			LPBYTE EmbedBuffer;
			if (TIFFGetField(tif, TIFFTAG_ICCPROFILE, &EmbedLen, &EmbedBuffer))
			{
				tiffProf = cmsOpenProfileFromMem(EmbedBuffer, EmbedLen);
				if (tiffProf)
				{
					if (static_cast<int>(cmsGetColorSpace(tiffProf)) == icSigRgbData)
						*components = 3;
					if (static_cast<int>(cmsGetColorSpace(tiffProf)) == icSigCmykData)
						*components = 4;
					for (uint el = 0; el < EmbedLen; ++el)
						*profile += EmbedBuffer[el];
				}
				cmsCloseProfile(tiffProf);
			}
			TIFFClose(tif);
		}
	}
#endif // HAVE_TIFF
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
			return;
		}
		jpeg_create_decompress (&cinfo);
		if ((infile = fopen (fn.local8Bit(), "rb")) == NULL)
			return;
		jpeg_stdio_src(&cinfo, infile);
		jpeg_save_markers(&cinfo, ICC_MARKER, 0xFFFF);
		jpeg_read_header(&cinfo, true);
		jpeg_start_decompress(&cinfo);
		unsigned int EmbedLen = 0;
		unsigned char* EmbedBuffer;
		if (read_jpeg_marker(ICC_MARKER,&cinfo, &EmbedBuffer, &EmbedLen))
		{
			tiffProf = cmsOpenProfileFromMem(EmbedBuffer, EmbedLen);
			if (tiffProf)
			{
				if (static_cast<int>(cmsGetColorSpace(tiffProf)) == icSigRgbData)
					*components = 3;
				if (static_cast<int>(cmsGetColorSpace(tiffProf)) == icSigCmykData)
					*components = 4;
				for (uint el = 0; el < EmbedLen; ++el)
					*profile += EmbedBuffer[el];
			}
			cmsCloseProfile(tiffProf);
			free(EmbedBuffer);
		}
		(void) jpeg_finish_decompress(&cinfo);
		fclose (infile);
		jpeg_destroy_decompress (&cinfo);
	}
	else
		return;
#endif // HAVE_CMS
}

bool ScImage::LoadPicture(QString fn, QString Prof, int rend, bool useEmbedded, bool useProf, int requestType, int gsRes, bool *realCMYK)
{
	// requestType - 0: CMYK, 1: RGB, 2: RGB Proof 3 : RawData, 4: Thumbnail
	// gsRes - is the resolution that ghostscript will render at
	bool isCMYK = false;
	bool ret = false;
	if (realCMYK != 0)
		*realCMYK = false;
	bool bilevel = false;
	float xres, yres;
	short resolutionunit = 0;
	int reqType = requestType;
#ifdef HAVE_CMS
	cmsHTRANSFORM xform = 0;
	cmsHPROFILE inputProf = 0;
	cmsHPROFILE tiffProf = 0;
	int cmsFlags = 0;
#endif
	QFileInfo fi = QFileInfo(fn);
	if (!fi.exists())
		return ret;
	QString ext = fi.extension(false).lower();
	QString tmp, dummy, cmd1, cmd2, BBox, tmp2;
	QChar tc;
	double x, y, b, h;
	bool found = false;
	int retg = -1;
	QString tmpFile = QDir::convertSeparators(QDir::homeDirPath()+"/.scribus/sc.png");
	QString picFile = QDir::convertSeparators(fn);
	if (ext == "pdf")
	{
		imgInfo.typ = 4;
		imgInfo.exifDataValid = false;
		if (reqType == 4)
			reqType = 1;
		QStringList args;
		xres = gsRes;
		yres = gsRes;
		args.append("-r"+QString::number(gsRes));
		args.append("-sOutputFile=\""+tmpFile + "\"");
		args.append("-dFirstPage=1");
		args.append("-dLastPage=1");
		args.append("\""+picFile+"\"");
		retg = callGS(args);
		if (retg == 0)
		{
			QImage image;
			load(tmpFile);
			unlink(tmpFile);
			setAlphaBuffer(true);
			if (ScApp->HavePngAlpha != 0)
			{
				for( int yi=0; yi < height(); ++yi )
				{
					QRgb *s = (QRgb*)(scanLine( yi ));
					for(int xi=0; xi < width(); ++xi )
					{
						if((*s) == 0xffffffff)
							(*s) &= 0x00ffffff;
						s++;
					}
				}
			}
			imgInfo.xres = qRound(gsRes);
			imgInfo.yres = qRound(gsRes);
			imgInfo.colorspace = 0;
		}
	}
	if ((ext == "eps") || (ext == "ps"))
	{
		imgInfo.typ = 3;
		imgInfo.exifDataValid = false;
		if (reqType == 4)
			reqType = 1;
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
			args.append("-sOutputFile=\""+tmpFile+"\"");
			args.append("-g"+tmp.setNum(qRound(b))+"x"+tmp2.setNum(qRound(h)));
			args.append("\""+picFile+"\"");
			retg = callGS(args);
			if (retg == 0)
			{
				QImage image;
				image.load(tmpFile);
				image.setAlphaBuffer(true);
				if (ScApp->HavePngAlpha != 0)
				{
					int wi = image.width();
					int hi = image.height();
					for( int yi=0; yi < hi; ++yi )
					{
						QRgb *s = (QRgb*)(image.scanLine( yi ));
						QRgb alphaFF = qRgba(255,255,255,255);
						QRgb alpha00 = qRgba(255,255,255,  0);
						for(int xi=0; xi < wi; ++xi )
						{
							if((*s) == alphaFF)
								(*s) &= alpha00;
							s++;
						}
					}
				}
				*this = static_cast<ScImage>(image.copy(static_cast<int>(x), 0, static_cast<int>(b-x), static_cast<int>(h-y)));
				unlink(tmpFile);
				imgInfo.xres = qRound(gsRes);
				imgInfo.yres = qRound(gsRes);
				imgInfo.colorspace = 0;
			}
		}
	}
#ifdef HAVE_TIFF
	else if ((ext == "tif") || (ext == "tiff"))
	{
		imgInfo.typ = 1;
		if (reqType == 4)
			reqType = 1;
		QImage img2;
		TIFF* tif = TIFFOpen(fn.local8Bit(), "r");
		if(tif)
		{
			unsigned widtht, heightt, size;
			char *description=0, *copyright=0, *datetime=0, *artist=0, *scannerMake=0, *scannerModel=0;
			
			TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &widtht);
			TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &heightt);
			TIFFGetField(tif, TIFFTAG_XRESOLUTION, &xres);
			TIFFGetField(tif, TIFFTAG_YRESOLUTION, &yres);
			TIFFGetField(tif, TIFFTAG_RESOLUTIONUNIT , &resolutionunit);
			size = widtht * heightt;
			uint16 photometric, bitspersample, samplesperpixel, fillorder;
			TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &photometric);
			TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bitspersample);
			TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel);
			TIFFGetField(tif, TIFFTAG_FILLORDER, &fillorder);
			
			TIFFGetField(tif, TIFFTAG_MAKE, &scannerMake);
			TIFFGetField(tif, TIFFTAG_MODEL, &scannerModel);
			TIFFGetField(tif, TIFFTAG_IMAGEDESCRIPTION, &description);
			TIFFGetField(tif, TIFFTAG_COPYRIGHT, &copyright);
			TIFFGetField(tif, TIFFTAG_DATETIME, &datetime);
			TIFFGetField(tif, TIFFTAG_ARTIST, &artist);
			imgInfo.exifInfo.cameraName = QString(scannerModel);
			imgInfo.exifInfo.cameraVendor = QString(scannerMake);
			imgInfo.exifInfo.comment = QString(description);
			imgInfo.exifInfo.userComment = QString(copyright);
			imgInfo.exifInfo.width = widtht;
			imgInfo.exifInfo.height = heightt;
			imgInfo.exifInfo.dateTime = QString(datetime);
			imgInfo.exifInfo.artist = QString(artist);
			imgInfo.exifInfo.thumbnail = QImage();
			imgInfo.exifDataValid = true;
			
			if (!create(widtht,heightt,32))
			{
				TIFFClose(tif);
				return ret;
			}
			setAlphaBuffer(true);
			uint32 *bits = 0;
			if (photometric == PHOTOMETRIC_SEPARATED)
			{
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
						return ret;
					}
					uint32 tileW = columns, tileH = rows;
					for (yt = 0; yt < (uint32)height(); yt += rows)
					{
						if (yt > (uint)height())
							break;
						if (height()-yt < rows)
							tileH = height()-yt;
						tileW = columns;
						register uint32 yi;
						for (xt = 0; xt < (uint)width(); xt += columns)
						{
							TIFFReadTile(tif, tile_buf, xt, yt, 0, 0);
							for (yi = 0; yi < tileH; yi++)
								_TIFFmemcpy(scanLine(yt+(tileH-1-yi))+xt, tile_buf+tileW*yi, tileW*4);
						}
					}
					_TIFFfree(tile_buf);
				}
				else
				{
					tsize_t bytesperrow = TIFFScanlineSize(tif);
					bits = (uint32 *) _TIFFmalloc(bytesperrow);
					if (bits)
					{
						for (unsigned int y = 0; y < heightt; y++)
						{
							if (TIFFReadScanline(tif, bits, y, 0))
							{
								memcpy(scanLine(y), bits, widtht * 4);
							}
						}
						_TIFFfree(bits);
					}
				}
				isCMYK = true;
				if (realCMYK != 0)
					*realCMYK = true;
			}
			else
			{
				bits = (uint32 *) _TIFFmalloc(size * sizeof(uint32));
				if(bits)
				{
					if (TIFFReadRGBAImage(tif, widtht, heightt, bits, 0))
					{
						for(unsigned int y = 0; y < heightt; y++)
							memcpy(scanLine(heightt - 1 - y), bits + y * widtht, widtht * 4);
					}
					_TIFFfree(bits);
					if (bitspersample == 1)
						bilevel = true;
				}
			}
			swapRGBA();
#ifdef HAVE_CMS
			DWORD EmbedLen = 0;
			LPBYTE EmbedBuffer;
			if (TIFFGetField(tif, TIFFTAG_ICCPROFILE, &EmbedLen, &EmbedBuffer) && useEmbedded && CMSuse && useProf)
			{
				const char *Descriptor;
				tiffProf = cmsOpenProfileFromMem(EmbedBuffer, EmbedLen);
				Descriptor = cmsTakeProductDesc(tiffProf);
				imgInfo.profileName = QString(Descriptor);
				imgInfo.isEmbedded = true;
			}
			else
			{
				imgInfo.isEmbedded = false;
				imgInfo.profileName = "";
			}
#endif // HAVE_CMS
			unsigned int PhotoshopLen = 0;
			unsigned char* PhotoshopBuffer;
			if (TIFFGetField(tif, TIFFTAG_PHOTOSHOP, &PhotoshopLen, &PhotoshopBuffer) )
			{
				if (PhotoshopLen != 0)
				{
					QByteArray arrayPhot(PhotoshopLen);
					arrayPhot.duplicate((const char*)PhotoshopBuffer,PhotoshopLen);
					QDataStream strPhot(arrayPhot,IO_ReadOnly);
					strPhot.setByteOrder( QDataStream::BigEndian );
					PSDHeader fakeHeader;
					fakeHeader.width = width();
					fakeHeader.height = height();
					parseRessourceData(strPhot, fakeHeader, PhotoshopLen);
					imgInfo.valid = (imgInfo.PDSpathData.size())>0?true:false;
				}
			}
			//			uint32 ClipLen = 0;
			//			uint8 ClipBuffer;
			//			QString db;
			//			if (TIFFGetField(tif, TIFFTAG_CLIPPATH, &ClipLen, &ClipBuffer))
			//				qDebug("%s", db.setNum(ClipLen).ascii());
			TIFFClose(tif);
			if (resolutionunit == RESUNIT_INCH)
			{
				setDotsPerMeterX ((int) (xres / 0.0254));
				setDotsPerMeterY ((int) (yres / 0.0254));
				imgInfo.xres = qRound(xres);
				imgInfo.yres = qRound(yres);
			}
			else if (resolutionunit == RESUNIT_CENTIMETER)
			{
				setDotsPerMeterX ((int) (xres * 100.0));
				setDotsPerMeterY ((int) (yres * 100.0));
				imgInfo.xres = qRound(xres*2.54);
				imgInfo.yres = qRound(yres*2.54);
			}
			if (isCMYK)
				imgInfo.colorspace = 1;
			else if (bilevel)
				imgInfo.colorspace = 2;
			else
				imgInfo.colorspace = 0;
			imgInfo.layerInfo.clear();
		}
	}
#endif // HAVE_TIFF
	else if (ext == "psd")
	{
		imgInfo.typ = 2;
		imgInfo.exifDataValid = false;
		QFile f(fn);
		if (f.open(IO_ReadOnly))
		{
			imgInfo.xres = 72;
			imgInfo.yres = 72;
			QDataStream s( &f );
			s.setByteOrder( QDataStream::BigEndian );
			PSDHeader header;
			s >> header;
			// Check image file format.
			if( s.atEnd() || !IsValid( header ) )
				return ret;
			// Check if it's a supported format.
			if( !IsSupported( header ) )
				return ret;
			iccbuf = 0;
			icclen = 0;
			if (reqType == 4)
				header.reserved[0] = 't';
			if( !LoadPSD(s, header) )
				return ret;
#ifdef HAVE_CMS
			if (icclen>0)
			{
				if (useEmbedded && CMSuse && useProf)
				{
					tiffProf = cmsOpenProfileFromMem(iccbuf, icclen);
					const char *Descriptor;
					Descriptor = cmsTakeProductDesc(tiffProf);
					imgInfo.profileName = QString(Descriptor);
					imgInfo.isEmbedded = true;
				}
				else
				{
					imgInfo.isEmbedded = false;
					imgInfo.profileName = "";
				}
				free(iccbuf);
			}
#endif // HAVE_CMS
			if (header.color_mode == CM_CMYK)
				isCMYK = true;
			else
				isCMYK = false;
			if (realCMYK != 0)
				*realCMYK = isCMYK;
			if (header.color_mode == CM_CMYK)
				imgInfo.colorspace = 1;
			else if (header.color_mode == CM_RGB)
				imgInfo.colorspace = 0;
			else if (header.color_mode == CM_GRAYSCALE)
				imgInfo.colorspace = 2;
			imgInfo.valid = true;
			setDotsPerMeterX ((int) (imgInfo.xres / 0.0254));
			setDotsPerMeterY ((int) (imgInfo.yres / 0.0254));
			xres = imgInfo.xres;
			yres = imgInfo.yres;
			f.close();
			if (reqType == 4)
			{
				if (!imgInfo.exifInfo.thumbnail.isNull())
				{
					create(imgInfo.exifInfo.thumbnail.width(), imgInfo.exifInfo.thumbnail.height(), 32);
					for( int yit=0; yit < imgInfo.exifInfo.thumbnail.height(); ++yit )
					{
						QRgb *s = (QRgb*)(imgInfo.exifInfo.thumbnail.scanLine( yit ));
						QRgb *d = (QRgb*)(scanLine( yit ));
						for(int xit=0; xit < imgInfo.exifInfo.thumbnail.width(); ++xit )
						{
							(*d) = (*s);
							s++;
							d++;
						}
					}
//					*this = imgInfo.exifInfo.thumbnail;
					imgInfo.exifInfo.width = header.width;
					imgInfo.exifInfo.height = header.height;
					return true;
				}
				else
					reqType = 1;
			}
			imgInfo.exifInfo.width = header.width;
			imgInfo.exifInfo.height = header.height;
		}
		else
			return ret;
	}
	else if ((ext == "jpg") || (ext == "jpeg"))
	{
		imgInfo.typ = 0;
		ExifData ExifInf;
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
			return ret;
		}
		jpeg_create_decompress (&cinfo);
		if ((infile = fopen (fn.local8Bit(), "rb")) == NULL)
			return ret;
		jpeg_stdio_src(&cinfo, infile);
		jpeg_save_markers(&cinfo, ICC_MARKER, 0xFFFF);
		jpeg_save_markers(&cinfo, PHOTOSHOP_MARKER, 0xFFFF);
		jpeg_read_header(&cinfo, true);
		jpeg_start_decompress(&cinfo);
		if (ExifInf.scan(fn))
		{
			if ((!ExifInf.isNullThumbnail()) && (reqType == 4))
				*this = ExifInf.getThumbnail();
			imgInfo.exifInfo.cameraName = ExifInf.getCameraModel();
			imgInfo.exifInfo.cameraVendor = ExifInf.getCameraMake();
			imgInfo.exifInfo.thumbnail = ExifInf.getThumbnail();
			imgInfo.exifInfo.comment = ExifInf.getComment();
			imgInfo.exifInfo.width = ExifInf.getWidth();
			imgInfo.exifInfo.height = ExifInf.getHeight();
			imgInfo.exifInfo.userComment = ExifInf.getUserComment();
			imgInfo.exifInfo.dateTime = ExifInf.getDateTime();
			imgInfo.exifDataValid = true;
			if (cinfo.density_unit == 0)
			{
				xres = 72;
				yres = 72;
			}
			else if ( cinfo.density_unit == 1 )
			{
				xres = cinfo.X_density;
				yres = cinfo.Y_density;
			}
			else if ( cinfo.density_unit == 2 )
			{
				xres = cinfo.X_density * 2.54;
				yres = cinfo.Y_density * 2.54;
			}
			imgInfo.xres = qRound(xres);
			imgInfo.yres = qRound(yres);
			if (cinfo.output_components == 4)
				imgInfo.colorspace = 1;
			else if (cinfo.output_components == 3)
				imgInfo.colorspace = 0;
			else if (cinfo.output_components == 1)
				imgInfo.colorspace = 2;
			imgInfo.progressive = jpeg_has_multiple_scans(&cinfo);
			if ((!ExifInf.isNullThumbnail()) && (reqType == 4))
			{
				jpeg_destroy_decompress(&cinfo);
				fclose(infile);
				return true;
			}
			else
			{
				if (reqType == 4)
					reqType = 1;
			}
		}
		else
			imgInfo.exifDataValid = false;
#ifdef HAVE_CMS
		unsigned int EmbedLen = 0;
		unsigned char* EmbedBuffer;
		if (read_jpeg_marker(ICC_MARKER,&cinfo, &EmbedBuffer, &EmbedLen) && useEmbedded && CMSuse && useProf)
		{
			const char *Descriptor;
			tiffProf = cmsOpenProfileFromMem(EmbedBuffer, EmbedLen);
			Descriptor = cmsTakeProductDesc(tiffProf);
			imgInfo.profileName = QString(Descriptor);
			imgInfo.isEmbedded = true;
			free(EmbedBuffer);
		}
		else
		{
			imgInfo.isEmbedded = false;
			imgInfo.profileName = "";
		}
#endif // HAVE_CMS
		unsigned int PhotoshopLen = 0;
		unsigned char * PhotoshopBuffer;
		if (cinfo.density_unit == 0)
		{
			xres = 72;
			yres = 72;
			setDotsPerMeterX(2834);
			setDotsPerMeterY(2834);
		}
		else if ( cinfo.density_unit == 1 )
		{
			xres = cinfo.X_density;
			yres = cinfo.Y_density;
			setDotsPerMeterX( int(100. * cinfo.X_density / 2.54) );
			setDotsPerMeterY( int(100. * cinfo.Y_density / 2.54) );
		}
		else if ( cinfo.density_unit == 2 )
		{
			xres = cinfo.X_density * 2.54;
			yres = cinfo.Y_density * 2.54;
			setDotsPerMeterX( int(100. * cinfo.X_density) );
			setDotsPerMeterY( int(100. * cinfo.Y_density) );
		}
		imgInfo.xres = qRound(xres);
		imgInfo.yres = qRound(yres);
		if (cinfo.output_components == 4)
		{
			isCMYK = true;
			imgInfo.colorspace = 1;
		}
		else if (cinfo.output_components == 3)
			imgInfo.colorspace = 0;
		else if (cinfo.output_components == 1)
			imgInfo.colorspace = 2;
		imgInfo.progressive = jpeg_has_multiple_scans(&cinfo);

		if (read_jpeg_marker(PHOTOSHOP_MARKER,&cinfo, &PhotoshopBuffer, &PhotoshopLen) )
		{
			if (PhotoshopLen != 0)
			{
				QByteArray arrayPhot(PhotoshopLen);
				arrayPhot.setRawData((const char*)PhotoshopBuffer,PhotoshopLen);
				QDataStream strPhot(arrayPhot,IO_ReadOnly);
				strPhot.setByteOrder( QDataStream::BigEndian );
				PSDHeader fakeHeader;
				fakeHeader.width = cinfo.output_width;
				fakeHeader.height = cinfo.output_height;
				if (cinfo.output_components == 4)
					imgInfo.colorspace = 1;
				else if (cinfo.output_components == 3)
					imgInfo.colorspace = 0;
				else if (cinfo.output_components == 1)
					imgInfo.colorspace = 2;
				imgInfo.progressive = jpeg_has_multiple_scans(&cinfo);
				parseRessourceData(strPhot, fakeHeader, PhotoshopLen);
				// Photoshop resolution is more accurate than jpeg header resolution
				xres = imgInfo.xres;
				yres = imgInfo.yres;
				setDotsPerMeterX( int(100. * imgInfo.xres / 2.54) );
				setDotsPerMeterY( int(100. * imgInfo.yres / 2.54) );
				imgInfo.valid = (imgInfo.PDSpathData.size())>0?true:false; // The only interest is vectormask
				arrayPhot.resetRawData((const char*)PhotoshopBuffer,PhotoshopLen);
				free( PhotoshopBuffer );
			}
		}
		if ( cinfo.output_components == 3 || cinfo.output_components == 4)
			create( cinfo.output_width, cinfo.output_height, 32 );
		else if ( cinfo.output_components == 1 )
		{
			create( cinfo.output_width, cinfo.output_height, 8, 256 );
			for (int i=0; i<256; i++)
				setColor(i, qRgb(i,i,i));
		}
		if (!isNull())
		{
			uchar** lines = jumpTable();
			while (cinfo.output_scanline < cinfo.output_height)
				(void) jpeg_read_scanlines(&cinfo, lines + cinfo.output_scanline, cinfo.output_height);
			if ( cinfo.output_components == 3 )
			{
				for (uint j=0; j<cinfo.output_height; j++)
				{
					uchar *in = scanLine(j) + cinfo.output_width * 3;
					QRgb *out = (QRgb*)scanLine(j);
					for (uint i=cinfo.output_width; i--; )
					{
						in -= 3;
						out[i] = qRgb(in[0], in[1], in[2]);
					}
				}
			}
			if ( cinfo.output_components == 4 )
			{
				for (int i = 0; i < height(); i++)
				{
					QRgb *ptr = (QRgb*) scanLine(i);
					unsigned char c, m, y ,k;
					if ((cinfo.jpeg_color_space == JCS_YCCK) || ((cinfo.jpeg_color_space == JCS_CMYK) && (cinfo.saw_Adobe_marker)))
					{
						for (int j = 0; j < width(); j++)
						{
							unsigned char *p = (unsigned char *) ptr;
							c = p[0];
							m = p[1];
							y =  p[2];
							k =  p[3];
							*ptr = qRgba(255 - c, 255 - m, 255 - y, 255 - k);
							ptr++;
						}
					}
					else
					{
						for (int j = 0; j < width(); j++)
						{
							unsigned char *p = (unsigned char *) ptr;
							c = p[0];
							m = p[1];
							y =  p[2];
							k =  p[3];
							*ptr = qRgba(y, m, c, k);
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
			if ( cinfo.output_components == 1 )
			{
				QImage tmpImg = convertDepth(32);
				create( cinfo.output_width, cinfo.output_height, 32 );
				for( int yi=0; yi < tmpImg.height(); ++yi )
				{
					QRgb *s = (QRgb*)(tmpImg.scanLine( yi ));
					QRgb *d = (QRgb*)(scanLine( yi ));
					for(int xi=0; xi < tmpImg.width(); ++xi )
					{
						(*d) = (*s);
						s++;
						d++;
					}
				}
			}
			setAlphaBuffer(true);
		}
		(void) jpeg_finish_decompress(&cinfo);
		fclose (infile);
		jpeg_destroy_decompress (&cinfo);
		imgInfo.layerInfo.clear();
	}
	else
	{
		if (load(fn))
		{
			imgInfo.typ = 6;
			imgInfo.exifDataValid = false;
			xres = dotsPerMeterX() * 0.0254;
			yres = dotsPerMeterY() * 0.0254;
			int resInf = imgInfo.lowResType;
			*this = convertDepth(32);
			setAlphaBuffer(true);
			imgInfo.colorspace = 0;
			imgInfo.xres = qRound(xres);
			imgInfo.yres = qRound(yres);
			imgInfo.lowResType = resInf;
		}
	}
	if (isNull())
		return  ret;
#ifdef HAVE_CMS
	if (CMSuse && useProf)
	{
		if (tiffProf)
			inputProf = tiffProf;
		else
		{
			QCString profilePath;
			if (isCMYK)
			{
				if (ScApp->InputProfilesCMYK.contains(Prof))
					imgInfo.profileName = Prof;
				else
					imgInfo.profileName = ScApp->doc->CMSSettings.DefaultImageCMYKProfile;
				profilePath = ScApp->InputProfilesCMYK[imgInfo.profileName].local8Bit();
			}
			else
			{
				if (ScApp->InputProfiles.contains(Prof))
					imgInfo.profileName = Prof;
				else
					imgInfo.profileName = ScApp->doc->CMSSettings.DefaultImageRGBProfile;
				profilePath = ScApp->InputProfiles[imgInfo.profileName].local8Bit();
			}
			inputProf = cmsOpenProfileFromFile(profilePath.data(), "r");
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
		if (BlackPoint)
			cmsFlags |= cmsFLAGS_BLACKPOINTCOMPENSATION;
		switch (reqType)
		{
		case 0: // CMYK
			if (!isCMYK)
				xform = cmsCreateTransform(inputProf, inputProfFormat, CMSprinterProf, TYPE_CMYK_8, IntentPrinter, 0);
			break;
		case 1: // RGB
			if (isCMYK)
				xform = cmsCreateTransform(inputProf, inputProfFormat, CMSoutputProf, TYPE_RGBA_8, rend, 0);
			break;
		case 2: // RGB Proof
			{
				if (inputProfFormat==TYPE_CMYK_8)
					inputProfFormat=(COLORSPACE_SH(PT_CMYK)|CHANNELS_SH(4)|BYTES_SH(1)|DOSWAP_SH(1)|SWAPFIRST_SH(1));//TYPE_YMCK_8;
				else
					inputProfFormat=TYPE_BGRA_8;
				if (SoftProofing)
					xform = cmsCreateProofingTransform(inputProf, inputProfFormat,
					                                   CMSoutputProf, TYPE_BGRA_8, CMSprinterProf,
					                                   IntentPrinter, rend, cmsFlags);
				else
					xform = cmsCreateTransform(inputProf, inputProfFormat,
					                           CMSoutputProf, TYPE_BGRA_8, rend, cmsFlags);
			}
			break;
		case 3: // no Conversion just raw Data
			xform = 0;
			break;
		}
		if (xform)
		{
			for (int i = 0; i < height(); i++)
			{
				LPBYTE ptr = scanLine(i);
				cmsDoTransform(xform, ptr, ptr, width());
				// if transforming from CMYK to RGB, flatten the alpha channel
				// which will still contain the black channel
				if (isCMYK && reqType != 0 && !bilevel)
				{
					QRgb *p = (QRgb *) ptr;
					QRgb alphaFF = qRgba(0,0,0,255);
					for (int j = 0; j < width(); j++, p++)
						*p |= alphaFF;
				}

			}
			cmsDeleteTransform (xform);
		}
		if (inputProf)
			cmsCloseProfile(inputProf);
	}
	else
#endif // HAVE_CMS

	{
		switch (reqType)
		{
		case 0:
			if (!isCMYK)
			{
				for (int i = 0; i < height(); i++)
				{
					QRgb *ptr = (QRgb *) scanLine(i);
					unsigned char cc, cm, cy ,ck;
					for (int j = 0; j < width(); j++)
					{
						cc = 255 - qRed(*ptr);
						cm = 255 - qGreen(*ptr);
						cy = 255 - qBlue(*ptr);
						ck = QMIN(QMIN(cc, cm), cy);
						*ptr++ = qRgba(cc-ck,cm-ck,cy-ck,ck);
					}
				}
			}
			break;
		case 1:
		case 2:
			if (isCMYK)
			{
				for (int i = 0; i < height(); i++)
				{
					QRgb *ptr = (QRgb *) scanLine(i);
					unsigned char cr, cg, cb, ck;
					for (int j = 0; j < width(); j++)
					{
						ck = qAlpha(*ptr);
						cr = 255 - QMIN(255, qRed(*ptr) + ck);
						cg = 255 - QMIN(255, qGreen(*ptr) + ck);
						cb = 255 - QMIN(255, qBlue(*ptr) + ck);
						*ptr++ = qRgba(cr,cg,cb,255);
					}
				}
			}
			break;
		case 3:
			break;
		}
	}
	if ((reqType == 0 || isCMYK) && !bilevel)
		setAlphaBuffer(false);
	setDotsPerMeterX (QMAX(2834, (int) (xres / 0.0254)));
	setDotsPerMeterY (QMAX(2834, (int) (yres / 0.0254)));
	imgInfo.xres = QMAX(72, qRound(xres));
	imgInfo.yres = QMAX(72, qRound(yres));
	return true;
}
