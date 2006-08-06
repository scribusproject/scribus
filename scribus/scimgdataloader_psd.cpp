/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "scconfig.h"
#include "scimgdataloader_psd.h"
#include "util.h"

#include <qfile.h>
#include <qfileinfo.h>

#ifdef HAVE_CMS
	#include CMS_INC
#endif

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

ScImgDataLoader_PSD::ScImgDataLoader_PSD(void)
{
	initSupportedFormatList();
}

void ScImgDataLoader_PSD::initSupportedFormatList(void)
{
	m_supportedFormats.clear();
	m_supportedFormats.append( "psd" );
}

void ScImgDataLoader_PSD::loadEmbeddedProfile(const QString& fn)
{
	m_embeddedProfile.resize(0);
	m_profileComponents = 0;
#ifdef HAVE_CMS
	cmsHPROFILE prof = 0;
	QFileInfo fi = QFileInfo(fn);
	if (!fi.exists())
		return;
	QString ext = fi.extension(false).lower();
	if (ext == "psd")
	{
		QFile f(fn);
		if (f.open(IO_ReadOnly))
		{
			m_imageInfoRecord.xres = 72;
			m_imageInfoRecord.yres = 72;
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
			if (m_embeddedProfile.size() > 0)
			{
				prof = cmsOpenProfileFromMem(m_embeddedProfile.data(), m_embeddedProfile.size());
				if (prof)
				{
					if (static_cast<int>(cmsGetColorSpace(prof)) == icSigRgbData)
						m_profileComponents = 3;
					if (static_cast<int>(cmsGetColorSpace(prof)) == icSigCmykData)
						m_profileComponents = 4;
				}
				cmsCloseProfile(prof);
			}
			f.close();
		}
		else
			return;
	}
#endif
}

void ScImgDataLoader_PSD::preloadAlphaChannel(const QString& fn, int res)
{
	float xres, yres;
	short resolutionunit = 0;
	initialize();
	QFileInfo fi = QFileInfo(fn);
	if (!fi.exists())
		return;

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
			return;
		// Check if it's a supported format.
		if( !IsSupported( header ) )
			return;
		if (header.color_mode == CM_CMYK)
				return;
		if( !LoadPSD(s, header) )
			m_image.reset();
		f.close();
	}
}

bool ScImgDataLoader_PSD::loadPicture(const QString& fn, int res, bool thumbnail)
{
	bool isCMYK = false;
	float xres = 72.0, yres = 72.0;
	if (!QFile::exists(fn))
		return false;

	initialize();
	m_imageInfoRecord.type = 2;
	m_imageInfoRecord.exifDataValid = false;
	QFile f(fn);
	if (f.open(IO_ReadOnly))
	{
		m_imageInfoRecord.xres = 72;
		m_imageInfoRecord.yres = 72;
		QDataStream s( &f );
		s.setByteOrder( QDataStream::BigEndian );
		PSDHeader header;
		s >> header;
		// Check image file format.
		if( s.atEnd() || !IsValid( header ) )
			return false;
		// Check if it's a supported format.
		if( !IsSupported( header ) )
			return false;
		if (thumbnail)
			header.reserved[0] = 't';
		if( !LoadPSD(s, header) )
			return false;
#ifdef HAVE_CMS
		if (m_embeddedProfile.size() > 0)
		{
			cmsHPROFILE prof = cmsOpenProfileFromMem(m_embeddedProfile.data(), m_embeddedProfile.size());
			const char *Descriptor;
			Descriptor = cmsTakeProductDesc(prof);
			m_imageInfoRecord.profileName = QString(Descriptor);
			m_imageInfoRecord.isEmbedded = true;
		}
#endif // HAVE_CMS
		if (header.color_mode == CM_CMYK)
			isCMYK = true;
		else
			isCMYK = false;
		if (header.color_mode == CM_CMYK)
			m_imageInfoRecord.colorspace = 1;
		else if (header.color_mode == CM_RGB)
			m_imageInfoRecord.colorspace = 0;
		else if (header.color_mode == CM_GRAYSCALE)
			m_imageInfoRecord.colorspace = 2;
		else if (header.color_mode == CM_DUOTONE)
			m_imageInfoRecord.colorspace = 3;
		m_imageInfoRecord.valid = true;
		m_image.setDotsPerMeterX ((int) (m_imageInfoRecord.xres / 0.0254));
		m_image.setDotsPerMeterY ((int) (m_imageInfoRecord.yres / 0.0254));
		xres = m_imageInfoRecord.xres;
		yres = m_imageInfoRecord.yres;
		f.close();
		if (thumbnail)
		{
			if (!m_imageInfoRecord.exifInfo.thumbnail.isNull())
			{
				m_image.create(m_imageInfoRecord.exifInfo.thumbnail.width(), m_imageInfoRecord.exifInfo.thumbnail.height(), 32);
				for( int yit=0; yit < m_imageInfoRecord.exifInfo.thumbnail.height(); ++yit )
				{
					QRgb *s = (QRgb*)(m_imageInfoRecord.exifInfo.thumbnail.scanLine( yit ));
					QRgb *d = (QRgb*)(m_image.scanLine( yit ));
					for(int xit=0; xit < m_imageInfoRecord.exifInfo.thumbnail.width(); ++xit )
					{
						if (isCMYK)
						{
							unsigned char cc = 255 - qRed(*s);
							unsigned char cm = 255 - qGreen(*s);
							unsigned char cy = 255 - qBlue(*s);
							unsigned char ck = QMIN(QMIN(cc, cm), cy);
							*d = qRgba(cc-ck,cm-ck,cy-ck,ck);
						}
						else
							(*d) = (*s);
						s++;
						d++;
					}
				}
				m_imageInfoRecord.exifInfo.width = header.width;
				m_imageInfoRecord.exifInfo.height = header.height;
				return true;
			}
		}
		m_imageInfoRecord.exifInfo.width = header.width;
		m_imageInfoRecord.exifInfo.height = header.height;
		m_imageInfoRecord.BBoxX = 0;
		m_imageInfoRecord.BBoxH = m_image.height();
		return true;
	}
	return false;
}

bool ScImgDataLoader_PSD::LoadPSD( QDataStream & s, const PSDHeader & header)
{
	// Create dst image.
	if( !m_image.create( header.width, header.height, 32 ))
		return false;
	m_image.setAlphaBuffer( true );
	uint tmp;
	uint cdataStart;
	uint ressourceDataLen;
	uint startRessource;
	uint layerDataLen;
	uint startLayers;

	srand(314159265);
	for (int i = 0; i < 4096; i++)
		random_table[i] = rand();
	for (int i = 0; i < 4096; i++)
	{
		int tmp;
		int swap = i + rand() % (4096 - i);
		tmp = random_table[i];
		random_table[i] = random_table[swap];
		random_table[swap] = tmp;
	}

	// Skip mode data. FIX: this is incorrect, it's the Colormap Data for indexed Images
	s >> tmp;
	cdataStart = s.device()->at();
	if (tmp != 0)
	{
		if ((header.color_mode == CM_DUOTONE))
		{
			short signature;
			short count;
			unsigned short c, m, y, k;
			uchar hc, sc, bc;
			ScColor col;
			s >> signature;
			s >> count;
			bool specialColour = false;
			for (int cda = 0; cda < count; cda++)
			{
				s >> signature;
				s >> c >> m >> y >> k;
				switch (signature)
				{
					case 0:	// RGB colour
						col.setColorRGB(c >> 8, m >> 8, y >> 8);
						colorTableSc.append(col);
//						qDebug("RGB = "+col.nameRGB());
						break;
					case 1:	// HSB colour
						hc = c >> 8;
						sc = m >> 8;
						bc = y >> 8;
						HSVTORGB(hc, sc, bc);
						col.setColorRGB(hc, sc, bc);
						colorTableSc.append(col);
//						qDebug("HSB as RGB = "+col.nameRGB());
						break;
					case 2:	// CMYK colour
						col.setColor(c >> 8, m >> 8, y >> 8, k >> 8);
						colorTableSc.append(col);
//						qDebug("CMYK = "+col.nameCMYK());
						break;
					case 3:	// Pantone
					case 4:	// Focoltone
					case 5:	// Truematch
					case 6:	// Toyo 88 colorfinder 1050
					case 10: // HKS colors
						colorTableSc.clear();
						colorTableSc.append(ScColor(0, 0, 0, 255));
						colorTableSc.append(ScColor(0, 0, 0, 0));
						specialColour = true;	// Reset the colourtable to black and white as we have no possibility to get the real colour values
						break;
					case 8: // Grayscale
						c = qRound((c / 10000.0) * 255);
						colorTableSc.append(ScColor(0, 0, 0, c));
						break;
				}
				if (specialColour)
					break;
			}
		}
		else
		{
			QValueList<uchar> colorTableR;
			QValueList<uchar> colorTableG;
			QValueList<uchar> colorTableB;
			colorTableR.clear();
			colorTableG.clear();
			colorTableB.clear();
			colorTable.clear();
			uchar r;
			for (uint cc = 0; cc < 256; cc++)
			{
				s >> r;
				colorTableR.append(r);
			}
			for (uint cc = 0; cc < 256; cc++)
			{
				s >> r;
				colorTableG.append(r);
			}
			for (uint cc = 0; cc < 256; cc++)
			{
				s >> r;
				colorTableB.append(r);
			}
			for (uint cc = 0; cc < 256; cc++)
			{
				colorTable.append(qRgb(colorTableR[cc], colorTableG[cc], colorTableB[cc]));
			}
		}
	}
	s.device()->at( cdataStart + tmp );
	s >> ressourceDataLen;
	startRessource = s.device()->at();
	if (ressourceDataLen != 0)
		parseRessourceData(s, header, ressourceDataLen);
	if  ((!m_imageInfoRecord.exifInfo.thumbnail.isNull()) && (header.reserved[0] == 't'))
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

bool ScImgDataLoader_PSD::parseLayer( QDataStream & s, const PSDHeader & header )
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
			lay.maskYpos = 0;
			lay.maskXpos = 0;
			lay.maskHeight = 0;
			lay.maskWidth = 0;
			if (layermasksize != 0)
			{
				s >> lay.maskYpos;
				s >> lay.maskXpos;
				s >> dummy;
				lay.maskHeight = dummy - lay.maskYpos;
				s >> dummy;
				lay.maskWidth = dummy - lay.maskXpos;
				s >> dummy;
			}
			s >> layerRange;
			s.device()->at( s.device()->at() + layerRange );
			lay.layerName = getLayerString(s);
			m_imageInfoRecord.layerInfo.append(lay);
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
			loadLayerChannels( s, header, m_imageInfoRecord.layerInfo, layer, &firstLayer );
		}
	}
	else
	{
		s >> numLayers;
		loadLayer( s, header);
	}
	return true;
}

bool ScImgDataLoader_PSD::loadChannel( QDataStream & s, const PSDHeader & header, QValueList<PSDLayer> &layerInfo, uint layer, int channel, int component, QImage &tmpImg)
{
	uint base = s.device()->at();
	uchar cbyte;
	ushort compression;
	s >> compression;
	if( compression > 1 )
		return false;
	if (compression == 0)
	{
		int count = layerInfo[layer].channelLen[channel]-2;
		for (int i = 0; i < tmpImg.height(); i++)
		{
			uchar *ptr =  tmpImg.scanLine(i);
			for (int j = 0; j < tmpImg.width(); j++)
			{
				s >> cbyte;
				count--;
				if (header.color_mode == CM_CMYK)
					cbyte = 255 - cbyte;
				if ((header.color_mode == CM_GRAYSCALE) && (component != 3))
				{
					ptr[0] = cbyte;
					ptr[1] = cbyte;
					ptr[2] = cbyte;
				}
				else if ((header.color_mode == CM_DUOTONE) && (component != 3))
				{
					ptr -= component;
					putDuotone(ptr, cbyte);
					ptr += component;
				}
				else if ((header.color_mode == CM_INDEXED) && (component != 3))
				{
					int ccol = colorTable[cbyte];
					ptr[2] = qRed(ccol);
					ptr[1] = qGreen(ccol);
					ptr[0] = qBlue(ccol);
				}
				else
				{
					if (channel < 4)
						ptr[component] = cbyte;
				}
				if (count == 0)
					break;
				ptr += 4;
			}
			if (count == 0)
				break;
		}
	}
	else
	{
		s.device()->at( s.device()->at() + tmpImg.height() * 2 );
		uint pixel_count = tmpImg.width();
		for (int hh = 0; hh < tmpImg.height(); hh++)
		{
			uint count = 0;
			uchar *ptr = tmpImg.scanLine(hh);
			uchar *ptr2 = ptr+tmpImg.width() * 4;
			ptr += component;
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
							if ((header.color_mode == CM_GRAYSCALE) && (component != 3))
							{
								ptr -= component;
								ptr[0] = cbyte;
								ptr[1] = cbyte;
								ptr[2] = cbyte;
								ptr += component;
							}
							else if ((header.color_mode == CM_DUOTONE) && (component != 3))
							{
								ptr -= component;
								putDuotone(ptr, cbyte);
								ptr += component;
							}
							else if ((header.color_mode == CM_INDEXED) && (component != 3))
							{
								ptr -= component;
								int ccol = colorTable[cbyte];
								ptr[2] = qRed(ccol);
								ptr[1] = qGreen(ccol);
								ptr[0] = qBlue(ccol);
								ptr += component;
							}
							else
							{
								*ptr = cbyte;
							}
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
						{
							if ((header.color_mode == CM_GRAYSCALE) && (component != 3))
							{
								ptr -= component;
								ptr[0] = val;
								ptr[1] = val;
								ptr[2] = val;
								ptr += component;
							}
							else if ((header.color_mode == CM_DUOTONE) && (component != 3))
							{
								ptr -= component;
								putDuotone(ptr, val);
								ptr += component;
							}
							else if ((header.color_mode == CM_INDEXED) && (component != 3))
							{
								ptr -= component;
								int ccol = colorTable[val];
								ptr[2] = qRed(ccol);
								ptr[1] = qGreen(ccol);
								ptr[0] = qBlue(ccol);
								ptr += component;
							}
							else
								*ptr = val;
						}
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
	s.device()->at( base+layerInfo[layer].channelLen[channel] );
	return true;
}

bool ScImgDataLoader_PSD::loadLayerChannels( QDataStream & s, const PSDHeader & header, QValueList<PSDLayer> &layerInfo, uint layer, bool* firstLayer)
{
	// Find out if the data is compressed.
	// Known values:
	//   0: no compression
	//   1: RLE compressed
	uint base = s.device()->at();
	uint base2 = base;
	uint channel_num = layerInfo[layer].channelLen.count();
	QImage tmpImg = QImage();
	QImage mask = QImage();
	if( !tmpImg.create( layerInfo[layer].width, layerInfo[layer].height, 32 ))
	{
		for(uint channel = 0; channel < channel_num; channel++)
		{
			base2 += layerInfo[layer].channelLen[channel];
		}
		s.device()->at( base2 );
		return false;
	}
	tmpImg.fill(qRgba(255, 255, 255, 0));
	tmpImg.setAlphaBuffer( true );
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
	if (channel_num < 4)
	{
		for (int i = 0; i < tmpImg.height(); i++)
		{
			QRgb * s = (QRgb*)(tmpImg.scanLine( i ));
			for (int j = 0; j < tmpImg.width(); j++)
			{
				*s++ = qRgba(255, 255, 255, 255);
			}
		}
	}
	for(uint channel = 0; channel < channel_num; channel++)
	{
		if ((layerInfo[layer].channelType[channel] < 0) && (header.color_mode == CM_CMYK))
		{
			s.device()->at( base+layerInfo[layer].channelLen[channel] );
			base = base+layerInfo[layer].channelLen[channel];
			continue;
		}
//		if ((layerInfo[layer].channelType[channel] == -2) || (layerInfo[layer].channelType[channel] == -1))
		if (layerInfo[layer].channelType[channel] == -2)
		{
			if (!mask.create( layerInfo[layer].maskWidth, layerInfo[layer].maskHeight, 32 ))
//			{
//				if (!mask.create(layerInfo[layer].width, layerInfo[layer].height, 32 ))
					break;
//			}
			mask.fill(qRgba(255, 255, 255, 0));
			mask.setAlphaBuffer( true );
			if (!loadChannel(s, header, layerInfo, layer, channel, components[channel], mask))
				break;
		}
		else
		{
			if (!loadChannel(s, header, layerInfo, layer, channel, components[channel], tmpImg))
				break;
		}
	}
	for(uint channel = 0; channel < channel_num; channel++)
	{
		base2 += layerInfo[layer].channelLen[channel];
	}
	s.device()->at( base2 );
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
	if ((m_imageInfoRecord.isRequest) && (m_imageInfoRecord.RequestProps.contains(layer)))
		visible = m_imageInfoRecord.RequestProps[layer].visible;
	if (visible)
	{
		unsigned int startSrcY, startSrcX, startDstY, startDstX;
		if (layerInfo[layer].ypos < 0)
		{
			startSrcY = abs(layerInfo[layer].ypos);
			startDstY = 0;
		}
		else
		{
			startSrcY = 0;
			startDstY = layerInfo[layer].ypos;
		}
		if (layerInfo[layer].xpos < 0)
		{
			startSrcX = abs(layerInfo[layer].xpos);
			startDstX = 0;
		}
		else
		{
			startSrcX = 0;
			startDstX = layerInfo[layer].xpos;
		}
		unsigned int startSrcYm, startSrcXm, startDstYm, startDstXm;
		if (layerInfo[layer].maskYpos < 0)
		{
			startSrcYm = abs(layerInfo[layer].maskYpos);
			startDstYm = 0;
		}
		else
		{
			startSrcYm = 0;
			startDstYm = layerInfo[layer].maskYpos;
		}
		if (layerInfo[layer].maskXpos < 0)
		{
			startSrcXm = abs(layerInfo[layer].maskXpos);
			startDstXm = 0;
		}
		else
		{
			startSrcXm = 0;
			startDstXm = layerInfo[layer].maskXpos;
		}
		QString layBlend2 = layerInfo[layer].blend;
		if ((m_imageInfoRecord.isRequest) && (m_imageInfoRecord.RequestProps.contains(layer)))
			layBlend2 = m_imageInfoRecord.RequestProps[layer].blend;
		if ((layBlend2 == "diss") && (header.color_mode != CM_CMYK))
		{
			for (int l = 0; l < tmpImg.height(); l++)
			{
				srand(random_table[ l  % 4096]);
				for (int k = 0; k < tmpImg.width(); k++)
				{
					int rand_val = rand() & 0xff;
					QRgb pixel = tmpImg.pixel(k, l);
					if (rand_val > 128)
						tmpImg.setPixel(k, l, qRgba(qRed(pixel), qGreen(pixel), qBlue(pixel), 0));
				}
			}
		}
		if (*firstLayer)
		{
			for( int yi=static_cast<int>(startSrcY); yi < QMIN(tmpImg.height(),  m_image.height()); ++yi )
			{
				QRgb *s = (QRgb*)(tmpImg.scanLine( yi ));
				QRgb *d = (QRgb*)( m_image.scanLine( QMIN(static_cast<int>(startDstY),  m_image.height()-1) ));
				d += QMIN(static_cast<int>(startDstX), m_image.width()-1);
				s += QMIN(static_cast<int>(startSrcX), tmpImg.width()-1);
/*				unsigned int *srcm;
				if (!mask.isNull())
				{
					srcm = (unsigned int *)mask.scanLine(QMIN(yi, mask.height()-1));
					srcm += QMIN(static_cast<int>(startSrcXm), mask.width()-1);
				} */
				for(int xi=static_cast<int>(startSrcX); xi < QMIN(tmpImg.width(),  m_image.width()); ++xi )
				{
					(*d) = (*s);
/*					unsigned char *sm = (unsigned char *) srcm;
					unsigned char *sc = (unsigned char *) d;
					unsigned char *ss = (unsigned char *) s;
					if (!mask.isNull())
					{
						if (header.color_mode == CM_CMYK)
						{
							sc[0] = (sc[0] * (255 - sm[3]) + ss[0] * sm[3]) / 255;
							sc[1] = (sc[1] * (255 - sm[3]) + ss[1] * sm[3]) / 255;
							sc[2] = (sc[2] * (255 - sm[3]) + ss[2] * sm[3]) / 255;
							sc[3] = (sc[3] * (255 - sm[3]) + ss[3] * sm[3]) / 255;
						}
						else
							sc[3] = sm[3];
					}
					srcm++; */
					s++;
					d++;
				}
				startDstY++;
			}
		}
		else
		{
			for (int i = static_cast<int>(startSrcY); i < layerInfo[layer].height; i++)
			{
				unsigned int *dst = (unsigned int *)m_image.scanLine(QMIN(static_cast<int>(startDstY),  m_image.height()-1));
				unsigned int *src = (unsigned int *)tmpImg.scanLine(QMIN(i, tmpImg.height()-1));
				dst += QMIN(static_cast<int>(startDstX),  m_image.width()-1);
				src += QMIN(static_cast<int>(startSrcX), tmpImg.width()-1);
				unsigned int *srcm = 0;
				if (!mask.isNull())
				{
					srcm = (unsigned int *)mask.scanLine(QMIN(i, mask.height()-1));
					srcm += QMIN(static_cast<int>(startSrcXm), mask.width()-1);
				}
				startDstY++;
				unsigned char r, g, b, a, src_r, src_g, src_b, src_a, mask_a;
				//<<#3356
				unsigned int maxDestX = m_image.width() - startDstX + startSrcX - 1;
				for (unsigned int j = startSrcX; j < QMIN(maxDestX, static_cast<unsigned int>(layerInfo[layer].width)); j++)
				//for (unsigned int j = startSrcX; j < static_cast<unsigned int>(layerInfo[layer].width); j++)
				//>>#3356
				{
					unsigned char *d = (unsigned char *) dst;
					unsigned char *s = (unsigned char *) src;
					unsigned char *sm = (unsigned char *) srcm;
					src_r = s[0];
					src_g = s[1];
					src_b = s[2];
					src_a = s[3];
					if (!mask.isNull())
						mask_a = sm[3];
					QString layBlend = layerInfo[layer].blend;
					if ((m_imageInfoRecord.isRequest) && (m_imageInfoRecord.RequestProps.contains(layer)))
						layBlend = m_imageInfoRecord.RequestProps[layer].blend;
					if (layBlend == "mul ")
					{
						if (header.color_mode == CM_CMYK)
						{
							src_r = INT_MULT(src_r, d[0]);
							src_g = INT_MULT(src_g, d[1]);
							src_b = INT_MULT(src_b, d[2]);
							src_a = INT_MULT(src_a, d[3]);
						}
						else
						{
							if (d[3] > 0)
							{
								src_r = INT_MULT(src_r, d[0]);
								src_g = INT_MULT(src_g, d[1]);
								src_b = INT_MULT(src_b, d[2]);
							}
						}
					}
					else if (layBlend == "scrn")
					{
						if (header.color_mode == CM_CMYK)
						{
							src_r = 255 - ((255-src_r) * (255-d[0]) / 128);
							src_g = 255 - ((255-src_g) * (255-d[1]) / 128);
							src_b = 255 - ((255-src_b) * (255-d[2]) / 128);
							src_a = 255 - ((255-src_a) * (255-d[3]) / 128);
/*							src_r = 255 - INT_MULT(255 - d[0], 255 - src_r);
							src_g = 255 - INT_MULT(255 - d[1], 255 - src_g);
							src_b = 255 - INT_MULT(255 - d[2], 255 - src_b);
							src_a = 255 - INT_MULT(255 - d[3], 255 - src_a); */
						}
						else
						{
							if (d[3] > 0)
							{
								src_r = 255 - ((255-src_r) * (255-d[0]) / 128);
								src_g = 255 - ((255-src_g) * (255-d[1]) / 128);
								src_b = 255 - ((255-src_b) * (255-d[2]) / 128);
/*								src_r = 255 - INT_MULT(255 - d[0], 255 - src_r);
								src_g = 255 - INT_MULT(255 - d[1], 255 - src_g);
								src_b = 255 - INT_MULT(255 - d[2], 255 - src_b); */
							}
						}
					}
					else if (layBlend == "over")
					{
						if (header.color_mode == CM_CMYK)
						{
							src_r = d[0] < 128 ? src_r * d[0] / 128 : 255 - ((255-src_r) * (255-d[0]) / 128);
							src_g = d[1] < 128 ? src_g * d[1] / 128 : 255 - ((255-src_g) * (255-d[1]) / 128);
							src_b = d[2] < 128 ? src_b * d[2] / 128 : 255 - ((255-src_b) * (255-d[2]) / 128);
							src_a = d[3] < 128 ? src_a * d[3] / 128 : 255 - ((255-src_a) * (255-d[3]) / 128);
/*							src_r = INT_MULT(d[0], d[0] + INT_MULT(2 * src_r, 255 - d[0]));
							src_g = INT_MULT(d[1], d[1] + INT_MULT(2 * src_g, 255 - d[1]));
							src_b = INT_MULT(d[2], d[2] + INT_MULT(2 * src_b, 255 - d[2]));
							src_a = INT_MULT(d[3], d[3] + INT_MULT(2 * src_a, 255 - d[3])); */
						}
						else
						{
							if (d[3] > 0)
							{
								src_r = d[0] < 128 ? src_r * d[0] / 128 : 255 - ((255-src_r) * (255-d[0]) / 128);
								src_g = d[1] < 128 ? src_g * d[1] / 128 : 255 - ((255-src_g) * (255-d[1]) / 128);
								src_b = d[2] < 128 ? src_b * d[2] / 128 : 255 - ((255-src_b) * (255-d[2]) / 128);
/*								src_r = INT_MULT(d[0], d[0] + INT_MULT(2 * src_r, 255 - d[0]));
								src_g = INT_MULT(d[1], d[1] + INT_MULT(2 * src_g, 255 - d[1]));
								src_b = INT_MULT(d[2], d[2] + INT_MULT(2 * src_b, 255 - d[2])); */
							}
						}
					}
					else if (layBlend == "diff")
					{
						if (header.color_mode == CM_CMYK)
						{
							src_r = d[0] > src_r ? d[0] - src_r : src_r - d[0];
							src_g = d[1] > src_g ? d[1] - src_g : src_g - d[1];
							src_b = d[2] > src_b ? d[2] - src_b : src_b - d[2];
							src_a = d[3] > src_a ? d[3] - src_a : src_a - d[3];
						}
						else
						{
							if (d[3] > 0)
							{
								src_r = d[0] > src_r ? d[0] - src_r : src_r - d[0];
								src_g = d[1] > src_g ? d[1] - src_g : src_g - d[1];
								src_b = d[2] > src_b ? d[2] - src_b : src_b - d[2];
							}
						}
					}
					else if (layBlend == "dark")
					{
						if (header.color_mode == CM_CMYK)
						{
							src_r = d[0]  < src_r ? d[0]  : src_r;
							src_g = d[1] < src_g ? d[1] : src_g;
							src_b = d[2] < src_b ? d[2] : src_b;
							src_a = d[3] < src_a ? d[3] : src_a;
						}
						else
						{
							if (d[3] > 0)
							{
								src_r = d[0]  < src_r ? d[0]  : src_r;
								src_g = d[1] < src_g ? d[1] : src_g;
								src_b = d[2] < src_b ? d[2] : src_b;
							}
						}
					}
					else if (layBlend == "hLit")
					{
						if (header.color_mode == CM_CMYK)
						{
							src_r = src_r < 128 ? src_r * d[0] / 128 : 255 - ((255-src_r) * (255-d[0]) / 128);
							src_g = src_g < 128 ? src_g * d[1] / 128 : 255 - ((255-src_g) * (255-d[1]) / 128);
							src_b = src_b < 128 ? src_b * d[2] / 128 : 255 - ((255-src_b) * (255-d[2]) / 128);
							src_a = src_a < 128 ? src_a * d[3] / 128 : 255 - ((255-src_a) * (255-d[3]) / 128);
						}
						else
						{
							if (d[3] > 0)
							{
								src_r = src_r < 128 ? src_r * d[0] / 128 : 255 - ((255-src_r) * (255-d[0]) / 128);
								src_g = src_g < 128 ? src_g * d[1] / 128 : 255 - ((255-src_g) * (255-d[1]) / 128);
								src_b = src_b < 128 ? src_b * d[2] / 128 : 255 - ((255-src_b) * (255-d[2]) / 128);
							}
						}
					}
					else if (layBlend == "sLit")
					{
						if (header.color_mode == CM_CMYK)
						{
							src_r = src_r * d[0] / 256 + src_r * (255 - ((255-src_r)*(255-d[0]) / 256) - src_r * d[0] / 256) / 256;
							src_g = src_g * d[1] / 256 + src_g * (255 - ((255-src_g)*(255-d[1]) / 256) - src_g * d[1] / 256) / 256;
							src_b = src_b * d[2] / 256 + src_b * (255 - ((255-src_b)*(255-d[2]) / 256) - src_b * d[2] / 256) / 256;
							src_a = src_a * d[3] / 256 + src_a * (255 - ((255-src_a)*(255-d[3]) / 256) - src_a * d[3] / 256) / 256;
						}
						else
						{
							if (d[3] > 0)
							{
								src_r = src_r * d[0] / 256 + src_r * (255 - ((255-src_r)*(255-d[0]) / 256) - src_r * d[0] / 256) / 256;
								src_g = src_g * d[1] / 256 + src_g * (255 - ((255-src_g)*(255-d[1]) / 256) - src_g * d[1] / 256) / 256;
								src_b = src_b * d[2] / 256 + src_b * (255 - ((255-src_b)*(255-d[2]) / 256) - src_b * d[2] / 256) / 256;
							}
						}
					}
					else if (layBlend == "lite")
					{
						if (header.color_mode == CM_CMYK)
						{
							src_r = d[0] < src_r ? src_r : d[0];
							src_g = d[1] < src_g ? src_g : d[1];
							src_b = d[2] < src_b ? src_b : d[2];
							src_a = d[3] < src_a ? src_a : d[3];
						}
						else
						{
							if (d[3] > 0)
							{
								src_r = d[0] < src_r ? src_r : d[0];
								src_g = d[1] < src_g ? src_g : d[1];
								src_b = d[2] < src_b ? src_b : d[2];
							}
						}
					}
					else if (layBlend == "smud")
					{
						if (header.color_mode == CM_CMYK)
						{
							src_r = d[0] + src_r - src_r * d[0] / 128;
							src_g = d[1] + src_g - src_g * d[1] / 128;
							src_b = d[2] + src_b - src_b * d[2] / 128;
							src_a = d[3] + src_a - src_a * d[3] / 128;
						}
						else
						{
							if (d[3] > 0)
							{
								src_r = d[0] + src_r - src_r * d[0] / 128;
								src_g = d[1] + src_g - src_g * d[1] / 128;
								src_b = d[2] + src_b - src_b * d[2] / 128;
							}
						}
					}
					else if (layBlend == "div ")
					{
						if (header.color_mode == CM_CMYK)
						{
							src_r = src_r == 255 ? 255 : ((d[0] * 256) / (255-src_r)) > 255 ? 255 : (d[0] * 256) / (255-src_r);
							src_g = src_g == 255 ? 255 : ((d[1] * 256) / (255-src_g)) > 255 ? 255 : (d[1] * 256) / (255-src_g);
							src_b = src_b == 255 ? 255 : ((d[2] * 256) / (255-src_b)) > 255 ? 255 : (d[2] * 256) / (255-src_b);
							src_a = src_a == 255 ? 255 : ((d[3] * 256) / (255-src_a)) > 255 ? 255 : (d[3] * 256) / (255-src_a);
						}
						else
						{
							if (d[3] > 0)
							{
								src_r = src_r == 255 ? 255 : ((d[0] * 256) / (255-src_r)) > 255 ? 255 : (d[0] * 256) / (255-src_r);
								src_g = src_g == 255 ? 255 : ((d[1] * 256) / (255-src_g)) > 255 ? 255 : (d[1] * 256) / (255-src_g);
								src_b = src_b == 255 ? 255 : ((d[2] * 256) / (255-src_b)) > 255 ? 255 : (d[2] * 256) / (255-src_b);
							}
						}
					}
					else if (layBlend == "idiv")
					{
						if (header.color_mode == CM_CMYK)
						{
							src_r = src_r == 0 ? 0 : (255 - (((255-d[0]) * 256) / src_r)) < 0 ? 0 : 255 - (((255-d[0]) * 256) / src_r);
							src_g = src_g == 0 ? 0 : (255 - (((255-d[1]) * 256) / src_g)) < 0 ? 0 : 255 - (((255-d[1]) * 256) / src_g);
							src_b = src_b == 0 ? 0 : (255 - (((255-d[2]) * 256) / src_b)) < 0 ? 0 : 255 - (((255-d[2]) * 256) / src_b);
							src_a = src_a == 0 ? 0 : (255 - (((255-d[3]) * 256) / src_a)) < 0 ? 0 : 255 - (((255-d[3]) * 256) / src_a);
						}
						else
						{
							if (d[3] > 0)
							{
								src_r = src_r == 0 ? 0 : (255 - (((255-d[0]) * 256) / src_r)) < 0 ? 0 : 255 - (((255-d[0]) * 256) / src_r);
								src_g = src_g == 0 ? 0 : (255 - (((255-d[1]) * 256) / src_g)) < 0 ? 0 : 255 - (((255-d[1]) * 256) / src_g);
								src_b = src_b == 0 ? 0 : (255 - (((255-d[2]) * 256) / src_b)) < 0 ? 0 : 255 - (((255-d[2]) * 256) / src_b);
							}
						}
					}
					else if (layBlend == "hue ")
					{
						if (header.color_mode != CM_CMYK)
						{
							if (d[3] > 0)
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
							}
						}
					}
					else if (layBlend == "sat ")
					{
						if (header.color_mode != CM_CMYK)
						{
							if (d[3] > 0)
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
							}
						}
					}
					else if (layBlend == "lum ")
					{
						if (header.color_mode != CM_CMYK)
						{
							if (d[3] > 0)
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
							}
						}
					}
					else if (layBlend == "colr")
					{
						if (header.color_mode != CM_CMYK)
						{
							if (d[3] > 0)
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
							}
						}
					}
					
					int layOpa = layerInfo[layer].opacity;
					if ((m_imageInfoRecord.isRequest) && (m_imageInfoRecord.RequestProps.contains(layer)))
						layOpa = m_imageInfoRecord.RequestProps[layer].opacity;
					if (!mask.isNull())
						layOpa = INT_MULT(mask_a, layOpa);
					if (header.color_mode != CM_CMYK)
						src_a = INT_MULT(src_a, layOpa);
					if (d[3] > 0)
					{
						r = (d[0] * (255 - layOpa) + src_r * layOpa) / 255;
						g = (d[1] * (255 - layOpa) + src_g * layOpa) / 255;
						b = (d[2] * (255 - layOpa) + src_b * layOpa) / 255;
						if (header.color_mode == CM_CMYK)
							a = (d[3] * (255 - layOpa) + src_a * layOpa) / 255;
						else
						{
							a = d[3] + INT_MULT(255 - d[3], src_a);
							r = (d[0] * (255 - src_a) + src_r * src_a) / 255;
							g = (d[1] * (255 - src_a) + src_g * src_a) / 255;
							b = (d[2] * (255 - src_a) + src_b * src_a) / 255;
						}
					}
					else
					{
						r = src_r;
						g = src_g;
						b = src_b;
						a = src_a;
					}
					if (header.color_mode == CM_CMYK)
					{
						d[0] = r;
						d[1] = g;
						d[2] = b;
						d[3] = a;
					}
					else
					{
						if (src_a > 0)
						{
							d[0] = r;
							d[1] = g;
							d[2] = b;
							d[3] = a;
						}
					}
					dst++;
					src++;
					if (!mask.isNull())
						srcm++;
				}
			}
		}
	}
	*firstLayer = false;
	return true;
}

bool ScImgDataLoader_PSD::loadLayer( QDataStream & s, const PSDHeader & header )
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
		for (int i = 0; i <  m_image.height(); i++)
		{
			QRgb * s = (QRgb*)( m_image.scanLine( i ));
			for (int j = 0; j <  m_image.width(); j++)
			{
				*s++ = qRgba(0, 0, 0, 255);
			}
		}
	}
//	channel_num = 4;
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
			uchar * ptr = m_image.bits() + components[channel];
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
						s >> cbyte;
						if (header.color_mode == CM_CMYK)
							cbyte = 255 - cbyte;
						if ((header.color_mode == CM_GRAYSCALE) && (components[channel] != 3))
						{
							ptr -= components[channel];
							ptr[0] = cbyte;
							ptr[1] = cbyte;
							ptr[2] = cbyte;
							ptr += components[channel];
						}
						else if ((header.color_mode == CM_DUOTONE) && (components[channel] != 3))
						{
							ptr -= components[channel];
							putDuotone(ptr, cbyte);
							ptr += components[channel];
						}
						else if ((header.color_mode == CM_INDEXED) && (components[channel] != 3))
						{
							ptr -= components[channel];
							int ccol = colorTable[cbyte];
							ptr[2] = qRed(ccol);
							ptr[1] = qGreen(ccol);
							ptr[0] = qBlue(ccol);
							ptr += components[channel];
						}
						else
							*ptr = cbyte;
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
						if ((header.color_mode == CM_GRAYSCALE) && (components[channel] != 3))
						{
							ptr -= components[channel];
							ptr[0] = val;
							ptr[1] = val;
							ptr[2] = val;
							ptr += components[channel];
						}
						else if ((header.color_mode == CM_DUOTONE) && (components[channel] != 3))
						{
							ptr -= components[channel];
							putDuotone(ptr, val);
							ptr += components[channel];
						}
						else if ((header.color_mode == CM_INDEXED) && (components[channel] != 3))
						{
							ptr -= components[channel];
							int ccol = colorTable[val];
							ptr[2] = qRed(ccol);
							ptr[1] = qGreen(ccol);
							ptr[0] = qBlue(ccol);
							ptr += components[channel];
						}
						else
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
			uchar * ptr =  m_image.bits() + components[channel];
			// Read the data.
			uint count = pixel_count;
			while( count != 0 )
			{
				s >> cbyte;
				if (header.color_mode == CM_CMYK)
					cbyte = 255 - cbyte;
				if ((header.color_mode == CM_GRAYSCALE) && (components[channel] != 3))
				{
					ptr -= components[channel];
					ptr[0] = cbyte;
					ptr[1] = cbyte;
					ptr[2] = cbyte;
					ptr += components[channel];
				}
				else if ((header.color_mode == CM_DUOTONE) && (components[channel] != 3))
				{
					ptr -= components[channel];
					putDuotone(ptr, cbyte);
					ptr += components[channel];
				}
				else if ((header.color_mode == CM_INDEXED) && (components[channel] != 3))
				{
					ptr -= components[channel];
					int ccol = colorTable[cbyte];
					ptr[2] = qRed(ccol);
					ptr[1] = qGreen(ccol);
					ptr[0] = qBlue(ccol);
					ptr += components[channel];
				}
				else
					*ptr = cbyte;
				ptr += 4;
				count--;
			}
		}
	}
	return true;
}

QString ScImgDataLoader_PSD::getLayerString(QDataStream & s)
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

bool ScImgDataLoader_PSD::IsValid( const PSDHeader & header )
{
	if( header.signature != 0x38425053 )
		return false;
	return true;
}

// Check that the header is supported.
bool ScImgDataLoader_PSD::IsSupported( const PSDHeader & header )
{
	if ( header.version != 1 )
		return false;
	if ( header.channel_count > 16 )
		return false;
	if ( header.depth != 8 )
		return false;
	if ((header.color_mode == CM_RGB) || (header.color_mode == CM_CMYK)
	 || (header.color_mode == CM_GRAYSCALE) || (header.color_mode == CM_INDEXED) || (header.color_mode == CM_DUOTONE))
		return true;
	return false;
}

void ScImgDataLoader_PSD::putDuotone(uchar *ptr, uchar cbyte)
{
	int c, c1, c2, c3, m, m1, m2, m3, y, y1, y2, y3, k, k1, k2, k3;
	uchar cb = 255 - cbyte;
	if (colorTableSc.count() == 1)
	{
		colorTableSc[0].getRawRGBColor(&c, &m, &y);
		ptr[2] = QMIN((c * cbyte) >> 8, 255);
		ptr[1] = QMIN((m * cbyte) >> 8, 255);
		ptr[0] = QMIN((y * cbyte) >> 8, 255);
	}
	else if (colorTableSc.count() == 2)
	{
		colorTableSc[0].getCMYK(&c, &m, &y, &k);
		c = QMIN((c * cb) >> 8, 255);
		m = QMIN((m * cb) >> 8, 255);
		y = QMIN((y * cb) >> 8, 255);
		k = QMIN((k * cb) >> 8, 255);
		colorTableSc[1].getCMYK(&c1, &m1, &y1, &k1);
		c1 = QMIN((c1 * cb) >> 8, 255);
		m1 = QMIN((m1 * cb) >> 8, 255);
		y1 = QMIN((y1 * cb) >> 8, 255);
		k1 = QMIN((k1 * cb) >> 8, 255);
		ScColor col = ScColor(QMIN(c+c1, 255), QMIN(m+m1, 255), QMIN(y+y1, 255), QMIN(k+k1, 255));
		col.getRawRGBColor(&c, &m, &y);
		ptr[2] = c;
		ptr[1] = m;
		ptr[0] = y;
	}
	else if (colorTableSc.count() == 3)
	{
		colorTableSc[0].getCMYK(&c, &m, &y, &k);
		c = QMIN((c * cb) >> 8, 255);
		m = QMIN((m * cb) >> 8, 255);
		y = QMIN((y * cb) >> 8, 255);
		k = QMIN((k * cb) >> 8, 255);
		colorTableSc[1].getCMYK(&c1, &m1, &y1, &k1);
		c1 = QMIN((c1 * cb) >> 8, 255);
		m1 = QMIN((m1 * cb) >> 8, 255);
		y1 = QMIN((y1 * cb) >> 8, 255);
		k1 = QMIN((k1 * cb) >> 8, 255);
		colorTableSc[2].getCMYK(&c2, &m2, &y2, &k2);
		c2 = QMIN((c2 * cb) >> 8, 255);
		m2 = QMIN((m2 * cb) >> 8, 255);
		y2 = QMIN((y2 * cb) >> 8, 255);
		k2 = QMIN((k2 * cb) >> 8, 255);
		ScColor col = ScColor(QMIN(c+c1+c2, 255), QMIN(m+m1+m2, 255), QMIN(y+y1+y2, 255), QMIN(k+k1+k2, 255));
		col.getRawRGBColor(&c, &m, &y);
		ptr[2] = c;
		ptr[1] = m;
		ptr[0] = y;
	}
	else if (colorTableSc.count() == 4)
	{
		colorTableSc[0].getCMYK(&c, &m, &y, &k);
		c = QMIN((c * cb) >> 8, 255);
		m = QMIN((m * cb) >> 8, 255);
		y = QMIN((y * cb) >> 8, 255);
		k = QMIN((k * cb) >> 8, 255);
		colorTableSc[1].getCMYK(&c1, &m1, &y1, &k1);
		c1 = QMIN((c1 * cb) >> 8, 255);
		m1 = QMIN((m1 * cb) >> 8, 255);
		y1 = QMIN((y1 * cb) >> 8, 255);
		k1 = QMIN((k1 * cb) >> 8, 255);
		colorTableSc[2].getCMYK(&c2, &m2, &y2, &k2);
		c2 = QMIN((c2 * cb) >> 8, 255);
		m2 = QMIN((m2 * cb) >> 8, 255);
		y2 = QMIN((y2 * cb) >> 8, 255);
		k2 = QMIN((k2 * cb) >> 8, 255);
		colorTableSc[3].getCMYK(&c3, &m3, &y3, &k3);
		c3 = QMIN((c3 * cb) >> 8, 255);
		m3 = QMIN((m3 * cb) >> 8, 255);
		y3 = QMIN((y3 * cb) >> 8, 255);
		k3 = QMIN((k3 * cb) >> 8, 255);
		ScColor col = ScColor(QMIN(c+c1+c2+c3, 255), QMIN(m+m1+m2+m3, 255), QMIN(y+y1+y2+y3, 255), QMIN(k+k1+k2+k3, 255));
		col.getRawRGBColor(&c, &m, &y);
		ptr[2] = c;
		ptr[1] = m;
		ptr[0] = y;
	}
}
