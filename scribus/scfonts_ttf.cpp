
#include <qapplication.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qstring.h>
#include <qptrvector.h>
#include <qmemarray.h>
#include <qregexp.h>

#include <iostream>
#include <sys/types.h>

#include "scfonts_ttf.h"
#include "scfonts_ttftypes.h"
#include "scfonts_encoding.h"
#include "config.h"
#ifdef HAVE_FREETYPE
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#include FT_GLYPH_H
#endif
// #define doPost

QString showTag(uint32_t tag)
{
	char tmp[5];
	tmp[0]=(tag>>24) & 255;
	tmp[1]=(tag>>16) & 255;
	tmp[2]=(tag>>8) & 255;
	tmp[3]=tag & 255;
	tmp[4]=0;
	return(tmp);
}

enum sc_TTFontError {sc_TTFOK,sc_TTFNoFile,sc_TTFFileError,sc_TTFNoMemory,sc_TTFBadFile};

using namespace std;

class sc_TTFont
{
	public:
		sc_TTFont(QString filename, SCFonts_Encoding &encoding);
		~sc_TTFont();
		QString	RealName();
		bool ReadMetrics(float *charwidths);
		bool EmbedFont(QString &str);
		bool Type42Metrics(QString *fb, QString *as, QString *ds, QString *it, bool *fp);
		void DumpEncoding()
		{
			for(int i=0;i<nGlyphs;++i)
			{
				if(OwnEncoding.present(i))
					std::cout << i << " -> " << OwnEncoding[i] << endl;
			}
		}
		void MapEncoding()
		{
			for(int i=0; i<256; ++i)
			{
				if(Encoding.present(i))
				{
					QString &glyph=Encoding[i];
					if(OwnEncoding.present(glyph))
					{
						std::cout << i << ": " << glyph << " -> " << OwnEncoding[glyph] << endl;
					}
				}
			}
		}
		sc_TTFontError GetError()
		{
			return(error);
		}
	private:
		bool Type42Header(QString &str);
		bool Type42Data(QString &str);
		bool Type42Trailer(QString &str);
		SCFonts_Encoding &Encoding;
		SCFonts_Encoding OwnEncoding;
		SCFonts_Encoding PostEncoding;
		sc_TTFontError error;
		uint16_t readBEUInt16();
		uint32_t readBEUInt32();

		bool readHeaders();
		bool readOffsetTable();
		bool readTableDirEntry(struct TableDirEntry *o);
		bool readNamingTable();
		bool readnGlyphs();
		bool readHeadTable();
		bool readPostTable();
		bool readLocaTable();
		bool readHheaTable();
		bool readHmtxTable();
		bool readCmapTable();

		QString readUnicode(char *str,int len);

		QFile file;
		QVector<QString> NamingTable;
		bool NamingTableRead;

		OffsetTable Offsets;
		bool OffsetsRead;

		HeadTable	Head;
		bool HeadRead;

		TableDirEntry *entries;

		PostTable	Post;
		bool PostTableRead;

		HheaTable Hhea;
		bool	HheaRead;

#ifdef doPost
		QVector<Post_GlyphName> GlyphNames;
		bool GlyphNamesRead;
#endif

		QArray<uint32_t> Loca;
		bool LocaRead;

		QArray<tt_longHorMetric> Hmtx;
		bool HmtxRead;

		bool CmapRead;

		int nGlyphs;
		bool nGlyphsRead;

		int o_maxp,o_head,o_post,o_name,o_loca,o_glyf,o_hhea,o_hmtx,o_kern,o_cmap;
};


sc_TTFont::sc_TTFont(QString filename, SCFonts_Encoding &encoding) :
	Encoding(encoding), file(filename), nGlyphs(0)
{
	NamingTableRead=OffsetsRead=HeadRead=PostTableRead=nGlyphsRead=false;
	LocaRead=HheaRead=HmtxRead=CmapRead=false;
	o_maxp=o_head=o_post=o_name=o_loca=o_glyf=o_hhea=o_hmtx=o_kern=o_cmap=-1;
	entries=0;
	error=sc_TTFOK;
	if(!(file.exists()))
	  error=sc_TTFNoFile;
	if(!(NamingTable.resize(8)))
		error=sc_TTFNoMemory;
	NamingTable.setAutoDelete(true);
#ifdef doPost
	GlyphNamesRead=false;
	GlyphNames.setAutoDelete(true);
#endif
}


sc_TTFont::~sc_TTFont()
{
	if(file.isOpen())
		file.close();
	if(entries)
		delete entries;
}


QString sc_TTFont::RealName()
{
	bool success=true;
	if(!(file.open(IO_ReadOnly)))
	{
		error=sc_TTFNoFile;
		return(0);
	}
	success&=readHeaders();
	success&=readNamingTable();
	file.close();
	if(NamingTable[6])
		return(*NamingTable[6]);
	else
		return(0);
}


bool sc_TTFont::ReadMetrics(float *charwidths)
{
	bool success=true;
	if(!(file.open(IO_ReadOnly)))
	{
		error=sc_TTFNoFile;
		return(false);
	}
	success&=readHeaders();
	success&=readHeadTable();
	success&=readnGlyphs();
	success&=readHheaTable();
	success&=readHmtxTable();
	success&=readCmapTable();
//#ifdef doPost
	success&=readPostTable();
//#endif
	file.close();

	if(success)
	{
		if(Hhea.metricDataFormat!=0)
		{
			error=sc_TTFBadFile;
			return(false);
		}

		float ndw=Hmtx[0].advanceWidth+Hmtx[0].lsb;
		ndw/=static_cast<float>(Head.unitsPerEm);

		for(int i=0; i<256; ++i)
		{
			if(Encoding.present(i))
			{
				QString &glyph=Encoding[i];
				if(OwnEncoding.present(glyph))
				{
					int gn=OwnEncoding[glyph];
					if(gn>=Hhea.numberOfHMetrics)
						gn=Hhea.numberOfHMetrics-1;
					float cw=Hmtx[gn].advanceWidth+Hmtx[gn].lsb;
					charwidths[i]=cw / static_cast<float>(Head.unitsPerEm);
				}
				else
					charwidths[i]=ndw;
			}
			else
				charwidths[i]=ndw;
		}
		return(true);
	}
	else
		return(false);
}


/***************************************************************************/


QString sc_TTFont::readUnicode(char *str,int len)
{
	QString result;
	unsigned char *t=reinterpret_cast<unsigned char *>(str);
	for (int i=0; i<len; i+=2)
	{
		int c=t[i]<<8|t[i+1];
		result+=QChar(c);
	}
	return(result);
}


u_int16_t sc_TTFont::readBEUInt16()
{
	u_int16_t result=0;
	unsigned char tmp[2];
	char *t=reinterpret_cast<char *>(tmp);
	if(file.readBlock(t,2)==2)
		result=tmp[0]<<8|tmp[1];
	else
		error=sc_TTFBadFile;
	return(result);
}


u_int32_t sc_TTFont::readBEUInt32()
{
	u_int32_t result=0;
	unsigned char tmp[4];
	char *t=reinterpret_cast<char *>(tmp);
	if(file.readBlock(t,4)==4)
		result=tmp[0]<<24|tmp[1]<<16|tmp[2]<<8|tmp[3];
	else
		error=sc_TTFBadFile;
	return(result);
}


bool sc_TTFont::readHeaders()
{
	if(OffsetsRead)
		return(true);
	if(readOffsetTable())
	{
		if(!(entries=new TableDirEntry[Offsets.numTables]))
		{
			error=sc_TTFNoMemory;
			return(false);
		}

		for(int i=0; i<Offsets.numTables; ++i)
		{
			TableDirEntry *tde=&entries[i];
			if(!(readTableDirEntry(tde)))
			{
				return(false);
			}

			switch(tde->tag)
			{
				case MAKE_ID('m','a','x','p'):
					o_maxp=i;	break;
				case MAKE_ID('h','e','a','d'):
					o_head=i;	break;
				case MAKE_ID('p','o','s','t'):
					o_post=i;	break;
				case MAKE_ID('n','a','m','e'):
					o_name=i;	break;
				case MAKE_ID('l','o','c','a'):
					o_loca=i;	break;
				case MAKE_ID('g','l','y','f'):
					o_glyf=i;	break;
				case MAKE_ID('h','h','e','a'):
					o_hhea=i;	break;
				case MAKE_ID('h','m','t','x'):
					o_hmtx=i; break;
				case MAKE_ID('k','e','r','n'):
					o_kern=i; break;
				case MAKE_ID('c','m','a','p'):
					o_cmap=i; break;
			}
		}
		if(o_maxp<0 || o_head<0 || o_post<0 || o_name<0)
		{
			error=sc_TTFBadFile;
			return(false);
		}
		return(true);
	}
	else
		return(false);
}


bool sc_TTFont::readTableDirEntry(struct TableDirEntry *o)
{
	unsigned char tmp[sizeof(struct TableDirEntry)];
	char *t=reinterpret_cast<char *>(tmp);
	if(file.readBlock(t,sizeof(struct TableDirEntry))==sizeof(struct TableDirEntry))
	{
		o->tag=tmp[0]<<24|tmp[1]<<16|tmp[2]<<8|tmp[3];
		o->checkSum=tmp[4]<<24|tmp[5]<<16|tmp[6]<<8|tmp[7];
		o->offset=tmp[8]<<24|tmp[9]<<16|tmp[10]<<8|tmp[11];
		o->length=tmp[12]<<24|tmp[13]<<16|tmp[14]<<8|tmp[16];
		return(true);
	}
	else
	{
		error=sc_TTFFileError;
		return(false);
	}
}


bool sc_TTFont::readOffsetTable()
{
	if(OffsetsRead)
		return(true);
	unsigned char tmp[sizeof(struct OffsetTable)];
	char *t=reinterpret_cast<char *>(tmp);
	if(file.readBlock(t,sizeof(struct OffsetTable))==sizeof(struct OffsetTable))
	{
		Offsets.version.mantissa=tmp[0]<<8|tmp[1];
		Offsets.version.fraction=tmp[2]<<8|tmp[3];
		Offsets.numTables=tmp[4]<<8|tmp[5];
		Offsets.searchRange=tmp[6]<<8|tmp[7];
		Offsets.entrySelector=tmp[7]<<8|tmp[8];
		Offsets.rangeShift=tmp[9]<<8|tmp[10];
		OffsetsRead=true;
		return(true);
	}
	else
	{
		error=sc_TTFFileError;
		return(false);
	}
}


bool sc_TTFont::readNamingTable()
{
	if(NamingTableRead)
		return(true);
	unsigned int format;
	unsigned int nrecords;
	size_t offset;

	file.at(entries[o_name].offset);
	format=readBEUInt16();
	if(format!=0)
	{
		error=sc_TTFBadFile;
		return(false);
	}
	nrecords=readBEUInt16();
	offset=readBEUInt16();

	NameRecord *nr=new NameRecord[nrecords];
	if(!(nr))
	{
	  error=sc_TTFNoMemory;
		return(false);
	}

	for(uint i=0; i<nrecords; ++i)
	{
		unsigned char tmp[sizeof(NameRecord)];
		char *t=reinterpret_cast<char *>(tmp);
		if(!(file.readBlock(t,sizeof(NameRecord))))
		{
			error=sc_TTFFileError;
			return(false);
		}

		nr[i].platformID=tmp[0]<<8|tmp[1];
		nr[i].encodingID=tmp[2]<<8|tmp[3];
		nr[i].languageID=tmp[4]<<8|tmp[5];
		nr[i].nameID=tmp[6]<<8|tmp[7];
		nr[i].length=tmp[8]<<8|tmp[9];
		nr[i].offset=tmp[10]<<8|tmp[11];
	}

	file.at(entries[o_name].offset+offset);

	for(uint i=0; i<nrecords; ++i)
	{
		if(nr[i].platformID==3 // Microsoft
			 && nr[i].encodingID==1 // UGL
			 && nr[i].languageID==0x0409 // US English
			 && nr[i].nameID<=7)
		{
			char *tmpstr=new char[nr[i].length];
			if(!(tmpstr))
			{
				error=sc_TTFNoMemory;
				delete nr;
				return(false);
			}
			file.at(entries[o_name].offset+offset+nr[i].offset);
			file.readBlock(reinterpret_cast<char *>(tmpstr),nr[i].length);
			NamingTable.insert(nr[i].nameID,new QString(readUnicode(tmpstr,nr[i].length)));
		}
	}
	OffsetsRead=true;
	return(true);
}


bool sc_TTFont::readnGlyphs()
{
	if(nGlyphsRead)
		return(true);
	struct {
    fxp16_16_t version;
    uint16_t nglyphs;
  } data;
	file.at(entries[o_maxp].offset);

	unsigned char tmp[sizeof(data)];
	char *t=reinterpret_cast<char *>(tmp);

	if(file.readBlock(t,sizeof(data)))
	{
		nGlyphs=tmp[4]<<8|tmp[5];
		nGlyphsRead=true;
		return(true);
	}
	else
	{
		error=sc_TTFBadFile;
		return(false);
	}
}


bool sc_TTFont::readHeadTable()
{
	if(HeadRead)
		return(true);

	file.at(entries[o_head].offset);
	unsigned char tmp[sizeof(HeadTable)];
	char *t=reinterpret_cast<char *>(tmp);
	if(file.readBlock(t,sizeof(HeadTable)))
	{
		Head.version.mantissa=tmp[0]<<8|tmp[1];
		Head.version.fraction=tmp[2]<<8|tmp[3];
		Head.fontRevision.mantissa=tmp[4]<<8|tmp[5];
		Head.fontRevision.fraction=tmp[6]<<8|tmp[7];
		Head.checkSumAdjustment=tmp[8]<<24|tmp[9]<<16|tmp[10]<<8|tmp[11];
		Head.magicNumber=tmp[12]<<24|tmp[13]<<16|tmp[14]<<8|tmp[15];
		Head.flags=tmp[16]<<8|tmp[17];
		Head.unitsPerEm=tmp[18]<<8|tmp[19];
		// skip creation and modification dates - 16 bytes in total.
		Head.xMin=tmp[36]<<8|tmp[37];
		Head.yMin=tmp[38]<<8|tmp[39];
		Head.xMax=tmp[40]<<8|tmp[41];
		Head.yMax=tmp[42]<<8|tmp[43];
		Head.macStyle=tmp[44]<<8|tmp[45];
		Head.lowestRecPPEM=tmp[46]<<8|tmp[47];
		Head.fontDirectionHint=tmp[48]<<8|tmp[49];
		Head.indexToLocFormat=tmp[50]<<8|tmp[51];
		Head.glyphDataFormat=tmp[52]<<8|tmp[53];

		if(Head.magicNumber!=0x5F0F3CF5)
    {
			error=sc_TTFBadFile;
			return(false);
		}
		HeadRead=true;
		return(true);
	}
	else
	{
		error=sc_TTFFileError;
		return(false);
	}
}


bool sc_TTFont::readPostTable()
{
	if(PostTableRead)
		return(true);
	file.at(entries[o_post].offset);
	unsigned char tmp[sizeof(PostTable)];
	char *t=reinterpret_cast<char *>(tmp);
	if(file.readBlock(t,sizeof(PostTable)))
	{
		Post.formatType.mantissa=tmp[0]<<8|tmp[1];
		Post.formatType.fraction=tmp[2]<<8|tmp[3];
		Post.italicAngle.mantissa=tmp[4]<<8|tmp[5];
		Post.italicAngle.fraction=tmp[6]<<8|tmp[7];
		Post.underlinePosition=tmp[8]<<8|tmp[9];
		Post.underlineThickness=tmp[10]<<8|tmp[11];
		Post.isFixedPitch=tmp[12]<<24|tmp[13]<<16|tmp[14]<<8|tmp[15];
		Post.minMemType42=tmp[16]<<24|tmp[17]<<16|tmp[18]<<8|tmp[19];
		Post.maxMemType42=tmp[20]<<24|tmp[21]<<16|tmp[22]<<8|tmp[23];
		Post.minMemType1=tmp[24]<<24|tmp[25]<<16|tmp[26]<<8|tmp[27];
		Post.maxMemType1=tmp[28]<<24|tmp[29]<<16|tmp[30]<<8|tmp[31];

		PostTableRead=true;
		return(true);

		if(Post.formatType.mantissa==1)
		{
//			OwnEncoding.read("TTFenc.txt");  // uses standard TTF encoding
			PostTableRead=true;
			return(true);
		}
		else if(Post.formatType.mantissa==2)	// has encoding table
		{
			SCFonts_Encoding tmpEncoding("TTFenc.txt");

			if(Post.formatType.fraction!=0)
			{
				error=sc_TTFBadFile;
				return(false);
			}
			u_int16_t nglyphsps = readBEUInt16();
			if(nglyphsps!=nGlyphs)
				std::cout << "Warning: nGlyphs should be consistent between head and post" << endl;

#ifdef doPost
			GlyphNames.resize(nglyphsps);

			int maxindex=0;
			for(int i=0;i<nglyphsps;++i)
			{
				int idx=readBEUInt16();
				GlyphNames.insert(i,new Post_GlyphName);
				if(idx>=258)
				{
					if((idx-258)>maxindex)
						maxindex=idx-258;
					GlyphNames[i]->index=idx;
					GlyphNames[i]->type=1;
				}
				else
				{
					GlyphNames[i]->index=idx;
					GlyphNames[i]->type=0;
				}
			}
			QVector<QString> NamesByIndex(maxindex+1);
			for(int i=0; i<=maxindex; ++i)
			{
				int numchars=file.getch();
				char *tmp=new char[numchars+1];
				file.readBlock(tmp,numchars);
				tmp[numchars]=0;
				NamesByIndex.insert(i,new QString(tmp));
				delete tmp;
			}
			for(int i=0; i<nglyphsps; ++i)
			{
				int idx=GlyphNames[i]->index;
				if(GlyphNames[i]->type==1)
				{
					GlyphNames[i]->name=NamesByIndex[idx-258];
					PostEncoding[i]=*(GlyphNames[i]->name);
					PostEncoding[*(GlyphNames[i]->name)]=i;
				}
				else
				{
					if(tmpEncoding.present(idx))
					{
						QString gn=tmpEncoding[idx];
						PostEncoding[i]=gn;
						PostEncoding[gn]=i;
					}
				}
			}
			QString tmp=".notdef";
			PostEncoding[0]=tmp;
			PostEncoding[tmp]=0;
			if(!(OwnEncoding.present(1)))
			{
				QString tmp=".null";
				PostEncoding[1]=tmp;
				PostEncoding[tmp]=1;
			}
#endif
		}
		PostTableRead=true;
		return(true);
	}
	else
	{
		error=sc_TTFFileError;
		return(false);
	}
}


bool sc_TTFont::readLocaTable()
{
	if(Head.indexToLocFormat>1);
	{
		error=sc_TTFBadFile;
		return(false);
	}
	if(nGlyphs)
	{
		file.at(entries[o_loca].offset);
		Loca.resize(nGlyphs);
		for(int i=0; i<nGlyphs; ++i)
		{
			if(Head.indexToLocFormat==0)
			{
				Loca[i]=readBEUInt16();
			}
			else
			{
				Loca[i]=readBEUInt32();
			}
		}
		return(true);
	}
	return(false);
}


bool sc_TTFont::readHheaTable()
{
	if(HheaRead)
		return(true);
	file.at(entries[o_hhea].offset);
	unsigned char tmp[sizeof(HheaTable)];
	char *t=reinterpret_cast<char *>(tmp);
	if(file.readBlock(t,sizeof(HheaTable)))
	{
		Hhea.version.fraction=tmp[0]<<8|tmp[1];
		Hhea.version.mantissa=tmp[2]<<8|tmp[3];
		Hhea.Ascender=tmp[4]<<8|tmp[5];
		Hhea.Descender=tmp[6]<<8|tmp[7];
		Hhea.LineGap=tmp[8]<<8|tmp[9];
		Hhea.advanceWidthMax=tmp[10]<<8|tmp[11];
		Hhea.minLeftSideBearing=tmp[12]<<8|tmp[13];
		Hhea.minRightSideBearing=tmp[14]<<8|tmp[15];
		Hhea.xMaxExtent=tmp[16]<<8|tmp[17];
		Hhea.caretSlopeRise=tmp[18]<<8|tmp[19];
		Hhea.caretSlopeRun=tmp[20]<<8|tmp[21];
		Hhea.metricDataFormat=tmp[32]<<8|tmp[33];
		Hhea.numberOfHMetrics=tmp[34]<<8|tmp[35];
		HheaRead=true;
		return(true);
	}
	else
	{
		error=sc_TTFFileError;
		return(false);
	}
}


bool sc_TTFont::readHmtxTable()  // Assumes that Head, nGlyphs, Hhea have already been read.
{
	if(HmtxRead)
		return(true);
	if(nGlyphs)
	{
		file.at(entries[o_hmtx].offset);
		if(!(Hmtx.resize(nGlyphs)))
		{
			error=sc_TTFNoMemory;
			return(false);
		}
		for(int i=0; i<Hhea.numberOfHMetrics; ++i)
		{
			Hmtx[i].advanceWidth=readBEUInt16();
			Hmtx[i].lsb=readBEUInt16();
		}
		HmtxRead=true;
		return(true);
	}
	return(false);
}


bool sc_TTFont::readCmapTable()
{
	if(CmapRead)
		return(true);

	file.at(entries[o_cmap].offset);

	CmapTable tt;
	tt.version=readBEUInt16();
	tt.numTables=readBEUInt16();
	if(tt.version != 0)
		return(false);

	long macOffset=-1;
	long windozeOffset=-1;

	CmapHeader th;
	for(int i=0;i<tt.numTables;++i)
	{
		th.platformID=readBEUInt16();
		th.encodingID=readBEUInt16();
		th.offset=readBEUInt32();
		if((th.platformID==1) && (th.encodingID==0))
			macOffset=th.offset;
		if((th.platformID==3) && (th.encodingID<2))
		{
			long fpos=file.at();

			file.at(entries[o_cmap].offset+th.offset);
			int format=readBEUInt16();
			if(format==4)
				windozeOffset=th.offset;
			file.at(fpos);
		}
	}

	if((windozeOffset<0) && (macOffset>0))
	{
		std::cout << "Using Mac encoding table" << endl;
		return(false);
#if 0		// Need to implement Mac encoding sometime...
struct CmapFormat0
{
	uint16_t format;
	uint16_t length;
	uint16_t language;
	char glyphIdArray[256];
};
#endif
	}
	else if(windozeOffset>0)
	{
		file.at(entries[o_cmap].offset+windozeOffset);
		CmapFormat4 tcm;
		tcm.format=readBEUInt16();
		tcm.length=readBEUInt16();
		tcm.language=readBEUInt16();
		tcm.segCountX2=readBEUInt16();
		tcm.searchRange=readBEUInt16();
		tcm.entrySelector=readBEUInt16();
		tcm.rangeShift=readBEUInt16();

		if(error)
			return(false);

		if(tcm.segCountX2==0)
		{
			error=sc_TTFBadFile;
			return(false);
		}

		int segCount=tcm.segCountX2/2;
		u_int16_t *data=new u_int16_t[segCount*4];

		if(!(data))
		{
			error=sc_TTFNoMemory;
			return(false);
		}

		for(int i=0;i<segCount;++i)
			data[i]=readBEUInt16();

		int junk; junk=readBEUInt16();

		for(int i=0;i<segCount;++i)
			data[i+segCount]=readBEUInt16();
		for(int i=0;i<segCount;++i)
			data[i+segCount*2]=readBEUInt16();
		for(int i=0;i<segCount;++i)
		{
			// Convert silly byte indexing from each offset's own address
			// to conventional array indices...
			int tmp=readBEUInt16();
			if(tmp)
				tmp=(tmp/2)-(segCount-i);
			else
				tmp=65535;		// 0 is now a valid value, so need to use
											// something else for null.
			data[i+segCount*3]=tmp;
		}

		tcm.endCount=&data[0];
		tcm.startCount=&data[segCount];
		tcm.idDelta=&data[segCount*2];
		tcm.idRangeOffset=&data[segCount*3];

		int idarraylength=(tcm.length-(16+8*segCount))/2;

		u_int16_t *idarray = new u_int16_t[idarraylength];
		for(int i=0;i<idarraylength;++i)
			idarray[i]=readBEUInt16();
		if(error)
			return(false);

#if 0
		for(int i=0;i<segCount;++i)
		{
			std::cout << i << endl;
			std::cout << "  " << tcm.startCount[i] << " - " << tcm.endCount[i] << endl;
			std::cout << "  " << tcm.idDelta[i] << " : " << tcm.idRangeOffset[i] << endl;
		}
#endif

		bool encoded=false;
		for(int i=0;i<256;++i)
		{
			if(Encoding.present(i))
			{
				int k=i;
				if(th.encodingID==0)
					k+=0xf000;	// *HACK!*  Supports symbol/dingbat fonts...
				for(int j=0;j<segCount;++j)
				{
					if((tcm.endCount[j]>=k)&&(tcm.startCount[j]<=k))
					{
						int result=k;
						if(tcm.idRangeOffset[j]!=65535)
						{
							result=idarray[tcm.idRangeOffset[j]+k-tcm.startCount[j]];
						}
						if(result)
						{
							result+=tcm.idDelta[j];
							result&=65535;
							QString gn=Encoding[i];
							OwnEncoding[gn]=result;
							OwnEncoding[result]=gn;
							encoded=true;
						}
					}
				}
			}
		}
		QString gn(".notdef");
		OwnEncoding[0]=gn;
		OwnEncoding[gn]=0;
		if(idarray)
			delete idarray;
		delete data;
		return(encoded);
	}
	else
	{
		return(false);
		// cout << "No suitable table found - relying on Post table" << endl;
	}
}


bool sc_TTFont::EmbedFont(QString &str)
{
	bool success=true;
	if(!(file.open(IO_ReadOnly)))
	{
		error=sc_TTFNoFile;
		return(false);
	}
	success&=readHeaders();
	success&=readHeadTable();
	success&=readnGlyphs();
	success&=readNamingTable();
	success&=readPostTable();
	success&=readHheaTable();
	success&=readHmtxTable();
	success&=readCmapTable();
	if(success)
	{
		Type42Header(str);
		Type42Data(str);
		Type42Trailer(str);
		file.close();
		return(true);
	}
	else
	{
		file.close();
		return(false);
	}
};


bool sc_TTFont::Type42Header(QString &str)
{
	QString tmp,tmp2;
  str+="%!PS-TrueTypeFont\n";
  if(Post.maxMemType42)
	{
		str+="%%%%VMUsage: ";
		long num=Post.minMemType42;
		tmp.setNum(num);
		str+=tmp + " ";
		num=Post.maxMemType42;
		tmp.setNum(num);
		str+=tmp + "\n";
	}
	str+="11 dict begin\n";
	if(NamingTable[6])
		tmp2=*NamingTable[6];
	else
	{
		QFileInfo fi(file);
		tmp2=fi.fileName();
	}
	tmp2 = tmp2.simplifyWhiteSpace();
	tmp2.replace(QRegExp(" "), "");
	str+="/FontName /" + tmp2 + " def\n";
	str+="/Encoding /ISOLatin1Encoding where {pop ISOLatin1Encoding} {StandardEncoding} ifelse def\n";
	str+="/PaintType 0 def\n/FontMatrix [1 0 0 1 0 0] def\n";
	tmp.sprintf("/FontBBox [%ld %ld %ld %ld] def\n",
		Head.xMin*1000L/Head.unitsPerEm,
		Head.yMin*1000L/Head.unitsPerEm,
		Head.xMax*1000L/Head.unitsPerEm,
		Head.yMax*1000L/Head.unitsPerEm);
	str+=tmp;
	str+="/FontType 42 def\n";
	str+="/FontInfo 8 dict dup begin\n";
	tmp.sprintf("/version (%d.%d) def\n",
		Head.fontRevision.mantissa,
		Head.fontRevision.fraction);
	str+=tmp;
	if(NamingTable[0])
		str+="/Notice (" + *NamingTable[0] + ") def\n";
  if(NamingTable[4])
		str+="/FullName (" + *NamingTable[4] + ") def\n";
  if(NamingTable[1])
		str+="/FamilyName (" + *NamingTable[1] + ") def\n";
  str+="/isFixedPitch ";
	if(Post.isFixedPitch)
		str+="true";
	else
		str+="false";
	str+=" def\n";

	long num=Post.underlinePosition*1000L/Head.unitsPerEm;
	tmp.sprintf("/UnderlinePosition %ld def\n",num);
	str+=tmp;
	num=Post.underlineThickness*1000L/Head.unitsPerEm;
  tmp.sprintf("/UnderlineThickness %ld def\n",num);
	str+=tmp;
	str+="end readonly def\n";

	return(true);
}


bool sc_TTFont::Type42Trailer(QString &str)
{
	QString tmp;
#ifdef HAVE_FREETYPE
	QString tmp2 = "";
	QString tmp3 = "";
	bool error;
	int counter = 0;
	char *buf[50];
	FT_Library library;
	FT_Face face;
	FT_ULong  charcode;
  FT_UInt   gindex;
	error = FT_Init_FreeType(&library);
	error = FT_New_Face(library, file.name(), 0, &face);
  charcode = FT_Get_First_Char(face, &gindex );
  while (gindex != 0)
		{
		FT_Get_Glyph_Name(face, gindex, buf, 50);
		tmp2 += "/"+QString((char*)buf)+" "+tmp3.setNum(gindex)+" def\n";
    charcode = FT_Get_Next_Char(face, charcode, &gindex );
		counter++;
		}
	FT_Done_FreeType( library );
	tmp.setNum(counter);
	str += "/CharStrings " + tmp + " dict dup begin\n"+tmp2;
#else
	tmp.setNum(nGlyphs+1);
	str += "/CharStrings " + tmp + " dict dup begin\n";
	QMap<int,QString>::Iterator it;
	for(it=OwnEncoding.beginByCode(); it!=OwnEncoding.endByCode(); ++it)
	{
		tmp.setNum(it.key());
		str+="/"+it.data()+" "+tmp+" def\n";
	}
	str +="/Euro 188 def\n";  // Dirty Hack for the Euro
	QMap<QString,int>::Iterator it2;
	for(it2=Encoding.beginByName(); it2!=Encoding.endByName(); ++it2)
	{
		QString gn=it2.key();
		if(!(OwnEncoding.present(gn)))
		{
			str+="/"+gn+" 0 def\n";
		}
	}
#endif
	str += "end readonly def\n";
  str += "FontName currentdict end definefont pop\n";
	return(true);
}


bool sc_TTFont::Type42Data(QString &str)
{
	char *tmp = new char[65535];
	int length;
	char linebuf[80];

	str += "/sfnts [";

	file.at(0);
	int poso=0;

	do
	{
		int posi=0;
		length=file.readBlock(tmp,65534);
		if(length)
		{
			str+="\n<";
			for (int j = 0; j < length; j++)
			{
				unsigned char u=tmp[posi];
				linebuf[poso]=((u >> 4) & 15) + '0';
				if(u>0x9f) linebuf[poso]+='a'-':';
				++poso;
				u&=15; linebuf[poso]=u + '0';
				if(u>0x9) linebuf[poso]+='a'-':';
				++posi;
				++poso;
				if (poso > 70)
				{
					linebuf[poso++]='\n';
					linebuf[poso++]=0;
					str += linebuf;
					poso = 0;
				}
			}
			linebuf[poso++]=0;
			str += linebuf;
			poso = 0;
			str += "00>";
		}
	} while (length==65534);

	str += "\n] def\n";
	delete tmp;
	return(true);
}

bool sc_TTFont::Type42Metrics(QString *fb, QString *as, QString *ds, QString *it, bool *fp)
{
		QString tmp = "";
		tmp.sprintf("%ld %ld %ld %ld",
		Head.xMin*1000L/Head.unitsPerEm,
		Head.yMin*1000L/Head.unitsPerEm,
		Head.xMax*1000L/Head.unitsPerEm,
		Head.yMax*1000L/Head.unitsPerEm);
		*fb = tmp;
		*as = tmp.setNum(static_cast<int>(Hhea.Ascender*1000L/Head.unitsPerEm));
		*ds = tmp.setNum(static_cast<int>(Hhea.Descender*1000L/Head.unitsPerEm));
		tmp.sprintf("%d.%d",Post.italicAngle.mantissa,Post.italicAngle.fraction);
		*it = tmp;
		*fp = static_cast<int>(Post.isFixedPitch);
		return true;
}

QString Foi_ttf::RealName()
{
	if(cached_RealName)
		return(cached_RealName);
	sc_TTFont ft(Datei,Encoding);
	QString result=ft.RealName();
	if(result==0)
	{
		QFile tf(Datei);
		QFileInfo fi(tf);
		result=fi.fileName();
	}
	return(result);
}

bool Foi_ttf::ReadMetrics()
{
	Ascent = "0";
	CapHeight = "0";
	Descender = "0";
	ItalicAngle = "0";
	StdVW = "1";
	FontBBox = "0 0 0 0";
	IsFixedPitch = false;	
	if(metricsread)
		return(true);
#ifdef HAVE_FREETYPE
	CharWidth.clear();
	QString tmp;
	bool			error;
	FT_Library library;
	FT_ULong  charcode;
	FT_UInt   gindex;
	FT_Face   face;
	error = FT_Init_FreeType( &library );
	error = FT_New_Face( library, Datei, 0, &face );
	uniEM = static_cast<float>(face->units_per_EM);
	HasKern = FT_HAS_KERNING(face);
	Ascent = tmp.setNum(face->ascender * 1000 / uniEM);
	Descender = tmp.setNum(face->descender * 1000 / uniEM);
	CapHeight = Ascent;
	ItalicAngle = "0";
	StdVW = "1";
	FontBBox = tmp.setNum(face->bbox.xMin * 1000 / uniEM)+" "+tmp.setNum(face->bbox.yMin * 1000 / uniEM)+" "+tmp.setNum(face->bbox.xMax * 1000 / uniEM)+" "+tmp.setNum(face->bbox.yMax * 1000 / uniEM);
	IsFixedPitch = face->face_flags & 4;
	gindex = 0;
	charcode = FT_Get_First_Char( face, &gindex );
	while ( gindex != 0 )
		{
		FT_Load_Glyph( face, gindex, FT_LOAD_NO_SCALE | FT_LOAD_NO_BITMAP );
		CharWidth.insert(charcode, face->glyph->metrics.horiAdvance / uniEM);
		charcode = FT_Get_Next_Char( face, charcode, &gindex );
		}
	FT_Done_FreeType( library );
	HasMetrics=true;
	metricsread=true;
#else
	for(int i=0;i<256;++i)
	  CharWidth[i]=0;
	sc_TTFont ft(Datei,Encoding);
	if(ft.ReadMetrics(CharWidth))
		HasMetrics=true;
	else
		HasMetrics=false;
	metricsread=true;
	if (HasMetrics)
		ft.Type42Metrics(&FontBBox, &Ascent, &Descender, &ItalicAngle, &IsFixedPitch);
	CapHeight = Ascent;
#endif
	return(HasMetrics);
}

bool Foi_ttf::EmbedFont(QString &str)
{
	sc_TTFont ft(Datei,Encoding);
	bool success=true;
	success&=ft.EmbedFont(str);
	return(success);
}

