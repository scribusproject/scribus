/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QFile>
#include <QString>
#include <QObject>
#include <QDebug>

#include <sys/types.h>

#include "fonts/scface_ttf.h"
#include "fonts/scfontmetrics.h"
#include "util.h"
#include "scconfig.h"

KernFeature::KernFeature ( FT_Face face )
		:m_valid ( true )
{
	FontName = QString ( face->family_name ) + " " + QString ( face->style_name )   ;
// 	qDebug() <<"KF"<<FontName;
// 	QTime t;
// 	t.start();
	FT_ULong length = 0;
	if ( !FT_Load_Sfnt_Table ( face, TTAG_GPOS , 0, NULL, &length ) )
	{
// 		qDebug() <<"\t"<<"GPOS table len"<<length;
		if ( length > 32 )
		{
			GPOSTableRaw.resize ( length );
			FT_Load_Sfnt_Table ( face, TTAG_GPOS, 0, reinterpret_cast<FT_Byte*> ( GPOSTableRaw.data() ), &length );

			makeCoverage();
		}
		else
			m_valid = false;

		GPOSTableRaw.clear();
//		coverages.clear();
	}
	else
		m_valid = false;

	if ( !m_valid )
		pairs.clear();
// 	qDebug() <<"\t"<<m_valid;
// 	qDebug() <<"\t"<<t.elapsed();
}

KernFeature::KernFeature ( const KernFeature & kf )
{
	m_valid = kf.m_valid;
	if ( m_valid )
		pairs = kf.pairs;

}


KernFeature::~ KernFeature()
{
}

double KernFeature::getPairValue ( unsigned int glyph1, unsigned int glyph2 ) const
{
	if ( m_valid )
	{

		if ( pairs.contains( glyph1 )
				&& pairs[glyph1].contains(glyph2))
		{
			return pairs[glyph1][glyph2];
		}
		else
		{
			//qDebug()<<"Search in classes";
			foreach (const quint16& coverageId, coverages.keys())
			{
				// for each pairpos table, coverage lists covered _first_ (left) glyph
				if(coverages[coverageId].contains(glyph1))
				{
					foreach(const quint16& classDefOffset, classGlyphFirst[coverageId].keys())
					{
						const ClassDefTable& cdt(classGlyphFirst[coverageId][classDefOffset]);
						foreach(const quint16& classIndex, cdt.keys())
						{
							const QList<quint16>& gl(cdt[classIndex]);
							if(gl.contains(glyph1))
							{
								//qDebug()<<"Found G1"<<glyph1<<"in Class"<<classIndex<<"at pos"<<gl.indexOf(glyph1);
								// Now we got the index of the first glyph class, see if glyph2 is in one of the left glyphs classes attached to this subtable.
								foreach(const quint16& classDefOffset2, classGlyphSecond[coverageId].keys())
								{
									const ClassDefTable& cdt2(classGlyphSecond[coverageId][classDefOffset2]);
									foreach(const quint16& classIndex2, cdt2.keys())
									{
										const QList<quint16>& gl2(cdt2[classIndex2]);
										if(gl2.contains(glyph2))
										{
											//qDebug()<<"Found G2"<<glyph2<<"in Class"<<classIndex2<<"at pos"<<gl2.indexOf(glyph2);

											double v(classValue[coverageId][classIndex][classIndex2]);
											// Cache this pair into "pairs" map.
											pairs[glyph1][glyph2] = v;
											return v;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return 0.0;
}

void KernFeature::makeCoverage()
{
	if ( GPOSTableRaw.isEmpty() )
		return;

	quint16 FeatureList_Offset= toUint16 ( 6 );
	quint16 LookupList_Offset = toUint16 ( 8 );

	// Find the offsets of the kern feature tables
	quint16 FeatureCount = toUint16 ( FeatureList_Offset );;
	QList<quint16> FeatureKern_Offset;
	for ( quint16 FeatureRecord ( 0 ); FeatureRecord < FeatureCount; ++ FeatureRecord )
	{
		int rawIdx ( FeatureList_Offset + 2 + ( 6 * FeatureRecord ) );
		quint32 tag ( FT_MAKE_TAG ( GPOSTableRaw.at ( rawIdx ),
					    GPOSTableRaw.at ( rawIdx + 1 ),
					    GPOSTableRaw.at ( rawIdx + 2 ),
					    GPOSTableRaw.at ( rawIdx + 3 ) ) );
		if ( tag == TTAG_kern )
		{
			FeatureKern_Offset << ( toUint16 ( rawIdx + 4 ) + FeatureList_Offset );

		}
	}

	// Extract indices of lookups for feture kern
	QList<quint16> LookupListIndex;
	foreach ( quint16 kern, FeatureKern_Offset )
	{
		quint16 LookupCount ( toUint16 ( kern + 2 ) );
		for ( int llio ( 0 ) ; llio < LookupCount; ++llio )
		{
			quint16 Idx ( toUint16 ( kern + 4 + ( llio * 2 ) ) );
			if ( !LookupListIndex.contains ( Idx ) )
			{
				LookupListIndex <<Idx ;
			}
		}
	}


	// Extract offsets of lookup tables for feature kern
	QList<quint16> LookupTables;
	QList<quint16> PairAdjustmentSubTables;
	for ( int i ( 0 ); i < LookupListIndex.count(); ++i )
	{
		int rawIdx ( LookupList_Offset + 2 + ( LookupListIndex[i] * 2 ) );
		quint16 Lookup ( toUint16 ( rawIdx )  + LookupList_Offset );
		quint16 SubTableCount ( toUint16 ( Lookup + 4 ) );
		for ( int stIdx ( 0 ); stIdx < SubTableCount; ++ stIdx )
		{
			quint16 SubTable ( toUint16 ( Lookup + 6 + ( 2 * stIdx ) ) + Lookup );

// 			quint16 PosFormat ( toUint16 ( SubTable ) );
			quint16 Coverage_Offset ( toUint16 ( SubTable + 2 ) + SubTable );
			quint16 CoverageFormat ( toUint16 ( Coverage_Offset ) );

			if ( 1 == CoverageFormat ) // glyph indices based
			{
				quint16 GlyphCount ( toUint16 ( Coverage_Offset + 2 ) );
				quint16 GlyphID ( Coverage_Offset + 4 );
				if (GlyphCount == 0) continue;

				for ( unsigned int gl ( 0 ); gl < GlyphCount; ++gl )
				{
					coverages[SubTable] << toUint16 ( GlyphID + ( gl * 2 ) );
				}
			}
			else if ( 2 == CoverageFormat ) // Coverage Format2 => ranges based
			{
				quint16 RangeCount ( toUint16 ( Coverage_Offset + 2 ) );
				if (RangeCount == 0) continue;

// 				int gl_base ( 0 );
				for ( int r ( 0 ); r < RangeCount; ++r )
				{
					quint16 rBase ( Coverage_Offset + 4 + ( r * 6 ) );
					quint16 Start ( toUint16 ( rBase ) );
					quint16 End ( toUint16 ( rBase + 2 ) );
// 					quint16 StartCoverageIndex ( toUint16 ( rBase + 4 ) );
					// #9842 : for some font such as Gabriola Regular
					// the range maybe be specified in reverse order
					if (Start <= End)
					{
						for ( unsigned int gl ( Start ); gl <= End; ++gl )
							coverages[SubTable]  << gl;
					}
					else
					{
						for ( int gl ( Start ); gl >= (int) End; --gl )
							coverages[SubTable]  << gl;
					}
				}
			}
			else
			{
//				qDebug() <<"Unknow Coverage Format:"<<CoverageFormat;
				continue;
			}

			makePairs ( SubTable );
		}

	}


}


void KernFeature::makePairs ( quint16 subtableOffset )
{
	/*
	Lookup Type 2:
	Pair Adjustment Positioning Subtable
	*/

	quint16 PosFormat ( toUint16 ( subtableOffset ) );

	if ( PosFormat == 1 )
	{
		quint16 ValueFormat1 ( toUint16 ( subtableOffset +4 ) );
		quint16 ValueFormat2 ( toUint16 ( subtableOffset +6 ) );
		quint16 PairSetCount ( toUint16 ( subtableOffset +8 ) );
		if ( ValueFormat1 && ValueFormat2 )
		{
			for ( int psIdx ( 0 ); psIdx < PairSetCount; ++ psIdx )
			{
				unsigned int FirstGlyph ( coverages[subtableOffset][psIdx] );
				quint16 PairSetOffset ( toUint16 ( subtableOffset +10 + ( 2 * psIdx ) ) +  subtableOffset );
				quint16 PairValueCount ( toUint16 ( PairSetOffset ) );
				quint16 PairValueRecord ( PairSetOffset + 2 );
				for ( int pvIdx ( 0 );pvIdx < PairValueCount; ++pvIdx )
				{
					quint16 recordBase ( PairValueRecord + ( ( 2 + 2 + 2 ) * pvIdx ) );
					quint16 SecondGlyph ( toUint16 ( recordBase ) );
					qint16 Value1 ( toInt16 ( recordBase + 2 ) );
					pairs[FirstGlyph][SecondGlyph] = double ( Value1 );
				}

			}
		}
		else if ( ValueFormat1 && ( !ValueFormat2 ) )
		{
			for ( int psIdx ( 0 ); psIdx < PairSetCount; ++ psIdx )
			{
				unsigned int FirstGlyph ( coverages[subtableOffset][psIdx] );
				quint16 PairSetOffset ( toUint16 ( subtableOffset +10 + ( 2 * psIdx ) ) +  subtableOffset );
				quint16 PairValueCount ( toUint16 ( PairSetOffset ) );
				quint16 PairValueRecord ( PairSetOffset + 2 );
				for ( int pvIdx ( 0 );pvIdx < PairValueCount; ++pvIdx )
				{
					quint16 recordBase ( PairValueRecord + ( ( 2 + 2 ) * pvIdx ) );
					quint16 SecondGlyph ( toUint16 ( recordBase ) );
					qint16 Value1 ( toInt16 ( recordBase + 2 ) );
					pairs[FirstGlyph][SecondGlyph] = double ( Value1 );
				}
			}
		}
		else
		{
//			qDebug() <<"ValueFormat1 is null or both ValueFormat1 and ValueFormat2 are null";
		}
	}
	else if ( PosFormat == 2 ) // class kerning
	{
		quint16 ValueFormat1 ( toUint16 ( subtableOffset +4 ) );
		quint16 ValueFormat2 ( toUint16 ( subtableOffset +6 ) );
		quint16 ClassDef1 ( toUint16 ( subtableOffset +8 )  + subtableOffset );
		quint16 ClassDef2 ( toUint16 ( subtableOffset +10 ) + subtableOffset );
		quint16 Class1Count ( toUint16 ( subtableOffset +12 ) );
		quint16 Class2Count ( toUint16 ( subtableOffset +14 ) );
		quint16 Class1Record ( subtableOffset +16 );

		// first extract classses
		getClass(true, ClassDef1 , subtableOffset );
		getClass(false, ClassDef2 , subtableOffset );

		if ( ValueFormat1 && ValueFormat2 )
		{
			for ( quint16 C1 ( 0 );C1 < Class1Count; ++C1 )
			{
				quint16 Class2Record ( Class1Record + ( C1 * ( 2 * 2 * Class2Count ) ) );
				for ( quint16 C2 ( 0 );C2 < Class2Count; ++C2 )
				{
					qint16 Value1 ( toInt16 ( Class2Record + ( C2 * ( 2 * 2 ) ) ) );
					if(Value1 != 0)
					{
						classValue[subtableOffset][C1][C2] = double ( Value1 );
					}
				}
			}
		}
		else if ( ValueFormat1 && ( !ValueFormat2 ) )
		{
			for ( quint16 C1 ( 1 );C1 < Class1Count; ++C1 )
			{
				quint16 Class2Record ( Class1Record + ( C1 * ( 2 * Class2Count ) ) );
				for ( quint16 C2 ( 1 );C2 < Class2Count; ++C2 )
				{
					qint16 Value1 ( toInt16 ( Class2Record + ( C2 * 2 ) ) );
					if(Value1 != 0)
					{
						classValue[subtableOffset][C1][C2] = double ( Value1 );
					}
				}
			}
		}
		else
		{
//			qDebug() <<"ValueFormat1 is null or both ValueFormat1 and ValueFormat2 are null";
		}

	}
	else
		qDebug() <<"unknown PosFormat"<<PosFormat;
}

KernFeature::ClassDefTable KernFeature::getClass ( bool leftGlyph, quint16 classDefOffset, quint16 coverageId )
{
	if(leftGlyph)
	{
		if(classGlyphFirst.contains(coverageId) && classGlyphFirst[coverageId].contains(classDefOffset))
			return classGlyphFirst[coverageId][classDefOffset];
	}
	else
	{
		if(classGlyphSecond.contains(coverageId) && classGlyphSecond[coverageId].contains(classDefOffset))
			return classGlyphSecond[coverageId][classDefOffset];
	}

	ClassDefTable ret;

	QList<quint16> excludeList;
	quint16 ClassFormat ( toUint16 ( classDefOffset ) );
	if ( ClassFormat == 1 )
	{
		quint16 StartGlyph ( toUint16 ( classDefOffset +2 ) );
		quint16 GlyphCount ( toUint16 ( classDefOffset +4 ) );
		quint16 ClassValueArray ( classDefOffset + 6 );

		for ( quint16 CV ( 0 );CV < GlyphCount; ++CV )
		{
			excludeList<<StartGlyph + CV;
			ret[ toUint16 ( ClassValueArray + ( CV * 2 ) ) ] << StartGlyph + CV;
		}
	}
	else if ( ClassFormat == 2 )
	{
		quint16 ClassRangeCount ( toUint16 ( classDefOffset + 2 ) );
		quint16 ClassRangeRecord ( classDefOffset + 4 );
		for ( int CRR ( 0 ); CRR < ClassRangeCount; ++CRR )
		{
			quint16 Start ( toUint16 ( ClassRangeRecord + ( CRR * 6 ) ) );
			quint16 End ( toUint16 ( ClassRangeRecord + ( CRR * 6 ) + 2 ) );
			quint16 Class ( toUint16 ( ClassRangeRecord + ( CRR * 6 ) + 4 ) );

			for ( int gl ( Start ); gl <= (int) End; ++gl )
			{
				excludeList<< (quint16) gl;
				ret[Class] << gl;
			}
		}
	}
	else
		qDebug() <<"Unknown Class Table type";

	// if possible (all glyphs are "classed"), avoid to pass through this slow piece of code.
	if(excludeList.count() != coverages[coverageId].count())
	{
		foreach(const quint16& gidx, coverages[coverageId])
		{
			if(!excludeList.contains(gidx))
				ret[0] << gidx;
		}
	}
	if(leftGlyph)
		classGlyphFirst[coverageId][classDefOffset] = ret;
	else
		classGlyphSecond[coverageId][classDefOffset] = ret;

	return ret;
}

quint16 KernFeature::toUint16 ( quint16 index )
{
	if ( ( index + 2 ) > GPOSTableRaw.count() )
	{
//                qDebug() << "HORROR!" << index << GPOSTableRaw.count() << FontName ;
		// Rather no kerning at all than random kerning
// 		m_valid = false;
		return 0;
	}
	// FIXME I just do not know how it has to be done *properly*
	quint16 c1 ( GPOSTableRaw.at ( index ) );
	quint16 c2 ( GPOSTableRaw.at ( index + 1 ) );
	c1 &= 0xFF;
	c2 &= 0xFF;
	quint16 ret ( ( c1 << 8 ) | c2 );
	return ret;
}

qint16 KernFeature::toInt16 ( quint16 index )
{
	if ( ( index + 2 ) > GPOSTableRaw.count() )
	{
		return 0;
	}
	// FIXME I just do not know how it has to be done *properly*
	quint16 c1 ( GPOSTableRaw.at ( index ) );
	quint16 c2 ( GPOSTableRaw.at ( index + 1 ) );
	c1 &= 0xFF;
	c2 &= 0xFF;
	qint16 ret ( ( c1 << 8 ) | c2 );
	return ret;
}


namespace {
uint word(QByteArray const & bb, uint pos) 
{
	const unsigned char * pp = reinterpret_cast<const unsigned char*>(bb.data()) + pos;
	return pp[0] << 24 | pp[1] << 16 | pp[2] << 8 | pp[3];
}
uint word16(QByteArray const & bb, uint pos) 
{
	const unsigned char * pp = reinterpret_cast<const unsigned char*>(bb.data()) + pos;
	return pp[0] << 8 | pp[1];
}
QString tag(QByteArray const & bb, uint pos)
{
	char buf[5] = "1234";
	buf[0] = bb.data()[pos];
	buf[1] = bb.data()[pos+1];
	buf[2] = bb.data()[pos+2];
	buf[3] = bb.data()[pos+3];
	return buf;
}
bool copy(QByteArray & dst, uint to, QByteArray & src, uint from, uint len) 
{
	if (!dst.data())
		return false;
	if (!src.data())
		return false;
	if (to + len > static_cast<uint>(dst.size()))
		return false;
	if (from + len > static_cast<uint>(src.size()))
		return false;
	
	memcpy(dst.data() + to, src.data() + from, len);
	return true;
}
} //namespace

ScFace_ttf::ScFace_ttf ( QString fam, QString sty, QString alt, QString scname, QString psname, QString path, int face )
		: FtFace ( fam, sty, alt, scname, psname, path, face )
{
	formatCode = ScFace::SFNT;
	kernFeature = 0;
}

ScFace_ttf::~ ScFace_ttf()
{
	if ( kernFeature )
		delete kernFeature;
}


void ScFace_ttf::load() const
{
	if ( !kernFeature )
		kernFeature = new KernFeature ( ftFace() );
	FtFace::load();
}

void ScFace_ttf::unload() const
{
	if ( kernFeature )
		delete kernFeature;
	kernFeature = 0;
	FtFace::unload();
}

qreal ScFace_ttf::glyphKerning ( uint gl1, uint gl2, qreal sz ) const
{
	if ( kernFeature->isValid() )
		return kernFeature->getPairValue ( gl1,gl2 ) / m_uniEM * sz;
	return FtFace::glyphKerning ( gl1, gl2, sz );
}

void ScFace_ttf::RawData(QByteArray & bb) const {
	if (formatCode == ScFace::TTCF) {
		QByteArray coll;
		FtFace::RawData(coll);
		// access table for faceIndex
		if (faceIndex >= static_cast<int>(word(coll, 8)))
		{
			bb.resize(0);
			return;
		}
		static const uint OFFSET_TABLE_LEN = 12;
		static const uint   TDIR_ENTRY_LEN = 16;
		uint faceOffset = word(coll, 12 + 4 * faceIndex);
		uint nTables    = word16(coll, faceOffset + 4);
		sDebug(QObject::tr("extracting face %1 from font %2 (offset=%3, nTables=%4)").arg(faceIndex).arg(fontFile).arg(faceOffset).arg(nTables));
		uint headerLength = OFFSET_TABLE_LEN + TDIR_ENTRY_LEN * nTables;
		uint tableLengths = 0;
		// sum table lengths incl padding
		for (uint i=0; i < nTables; ++i)
		{
			tableLengths += word(coll, faceOffset + OFFSET_TABLE_LEN + TDIR_ENTRY_LEN * i + 12);
			tableLengths = (tableLengths+3) & ~3;
		}
		bb.resize(headerLength + tableLengths);
		if (! bb.data())
			return;
		// write header
		sDebug(QObject::tr("memcpy header: %1 %2 %3").arg(0).arg(faceOffset).arg(headerLength));
		if (!copy(bb, 0, coll, faceOffset, headerLength))
			return;

		uint pos = headerLength;
		for (uint i=0; i < nTables; ++i)
		{
			uint tableSize  = word(coll, faceOffset + OFFSET_TABLE_LEN + TDIR_ENTRY_LEN * i + 12);
			uint tableStart = word(coll, faceOffset + OFFSET_TABLE_LEN + TDIR_ENTRY_LEN * i + 8);
			sDebug(QObject::tr("table '%1'").arg(tag(coll, tableStart)));
			sDebug(QObject::tr("memcpy table: %1 %2 %3").arg(pos).arg(tableStart).arg(tableSize));
			if (!copy(bb, pos, coll, tableStart, tableSize)) break;
			// write new offset to table entry
			sDebug(QObject::tr("memcpy offset: %1 %2 %3").arg(OFFSET_TABLE_LEN + TDIR_ENTRY_LEN*i + 8).arg(pos).arg(4));
			memcpy(bb.data() + OFFSET_TABLE_LEN + TDIR_ENTRY_LEN * i + 8, &pos, 4);
			pos += tableSize;
			// pad
			while ((pos & 3) != 0)
				bb.data()[pos++] = '\0';
		}
	}	
	else if (formatCode == ScFace::TYPE42) {
		FtFace::RawData(bb);
	}
	else {
		FtFace::RawData(bb);
	}
}

bool ScFace_ttf::EmbedFont(QString &str) const
{
	if (formatCode == ScFace::TYPE42) {
		//easy:
		QByteArray bb;
		FtFace::RawData(bb);
		str += bb;
		return true;
	}
	QString tmp4;
	QString tmp2 = "";
	QString tmp3 = "";
	int counter = 0;
	char *buf[50];
	FT_ULong  charcode;
	FT_UInt   gindex;
	FT_Face face = ftFace();
	if (!face) {
		return false;
	}
	const FT_Stream fts = face->stream;
	if (ftIOFunc(fts, 0L, NULL, 0)) {
		return(false);
	}
	str+="%!PS-TrueTypeFont\n";
	str+="11 dict begin\n";
	str+="/FontName /" + psName + " def\n";
	str+="/Encoding /ISOLatin1Encoding where {pop ISOLatin1Encoding} {StandardEncoding} ifelse def\n";
	str+="/PaintType 0 def\n/FontMatrix [1 0 0 1 0 0] def\n";
	str+="/FontBBox ["+FontBBox+"] def\n";
	str+="/FontType 42 def\n";
	str+="/FontInfo 8 dict dup begin\n";
	str+="/FamilyName (" + psName + ") def\n";
	str+="end readonly def\n";
	unsigned char *tmp = new unsigned char[65535];
	int length;
	char linebuf[80];
	str += "/sfnts [";
	int poso=0;
	do {
		int posi=0;
		length= fts->size - fts->pos;
		if (length > 65534) {
			length = 65534;
		}
		if (!ftIOFunc(fts, 0L, tmp, length))
		{
			str+="\n<\n";
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
			str += "00\n>";
		}
		else {
			sDebug(QObject::tr("Font %1 is broken (read stream), no embedding").arg(fontFile));
			str += "\n] def\n";
			status = qMax(status,ScFace::BROKENGLYPHS);
			return false;
		}
	} while (length==65534);
	
	str += "\n] def\n";
	delete[] tmp;
	gindex = 0;
	charcode = FT_Get_First_Char(face, &gindex );
	while (gindex != 0)
	{
		FT_Get_Glyph_Name(face, gindex, buf, 50);
		tmp2 += "/"+QString(reinterpret_cast<char*>(buf))+" "+tmp3.setNum(gindex)+" def\n";
		 charcode = FT_Get_Next_Char(face, charcode, &gindex );
		counter++;
	}
	tmp4.setNum(counter);
	str += "/CharStrings " + tmp4 + " dict dup begin\n"+tmp2;
	str += "end readonly def\n";
	str += "FontName currentdict end definefont pop\n";
	return(true);
}

