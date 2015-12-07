//
//  cff.h
//  Scribus
//
//  Created by Andreas Vox on 04.05.15.
//
//

#ifndef Scribus__cff_h
#define Scribus__cff_h


#include "scribusapi.h"

#include <QByteArray>
#include <QDataStream>
#include <QHash>
#include <QList>
#include <QMap>
#include <QString>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace cff {
	
	typedef quint16 sid_type;
	typedef uint operator_type;
	
	enum sid_range {
		sid_min = 0,
		sid_last_std = 390,
		sid_max = 64999,
		sid_max1 = 65000
	};
	
	enum CFF_Header_Format {
		cff_major   = 0,
		cff_minor   = 1,
		cff_hdrSize = 2,
		cff_offSize = 3
	};
	
	
	enum CFF_INDEX_Format {
		cff_idx_count   = 0,
		cff_idx_offsize = 2,
		cff_idx_offsets = 3
	};
	
	
	enum cff_Real_Format {
		cff_nibble_maxDigit = 9,
		cff_nibble_Point    = 10,
		cff_nibble_PosExp   = 11,
		cff_nibble_NegExp   = 12,
		cff_nibble_Reserved = 13,
		cff_nibble_Minus    = 14,
		cff_nibble_End      = 15
	};
	
	
	enum CFF_DICT_Format {
		cff_dict_version            = 0x00,
		cff_dict_Notice             = 0x01,
		cff_dict_FullName           = 0x02,
		cff_dict_FamilyName         = 0x03,
		cff_dict_Weight             = 0x04,
		cff_dict_FontBBox           = 0x05,
		cff_dict_BlueValues         = 0x06,
		cff_dict_OtherBlues         = 0x07,
		cff_dict_FamilyBlues        = 0x08,
		cff_dict_FamilyOtherBlues   = 0x09,
		cff_dict_StdHW              = 0x0a,
		cff_dict_StdVW              = 0x0b,
		cff_dict_TwoBytes           = 12,
		cff_dict_UniqueID           = 0x0d,
		cff_dict_XUID               = 0x0e,
		cff_dict_charset            = 0x0f,
		cff_dict_Encoding           = 0x10,
		cff_dict_CharStrings        = 0x11,
		cff_dict_Private            = 0x12,
		cff_dict_Subrs              = 0x13,
		cff_dict_defaultWidthX      = 0x14,
		cff_dict_nominalWidthX      = 0x15,
		// 0x16+ reserved
		cff_dict_Card16             = 28,
		cff_dict_Card32             = 29,
		cff_dict_Real               = 30,
		cff_dict_minOperand         = 32,
		cff_dict_maxSmallCard       = 246,
		cff_dict_biasSmallCard      = -139,
		cff_dict_minPosCard         = 247,
		cff_dict_maxPosCard         = 250,
		cff_dict_biasPosCard        = 108,
		cff_dict_minNegCard         = 251,
		cff_dict_maxNegCard         = 254,
		cff_dict_biasNegCard        = -108,
		cff_dict_Copyright          = 0x0c00,
		cff_dict_isFixedPitch       = 0x0c01,
		cff_dict_ItalicAngle        = 0x0c02,
		cff_dict_UnderlinePosition  = 0x0c03,
		cff_dict_UnderlineThickness = 0x0c04,
		cff_dict_CharstringType     = 0x0c06,
		cff_dict_FontMatrix         = 0x0c07,
		cff_dict_StrokeWidth        = 0x0c08,
		cff_dict_BlueScale          = 0x0c09,
		cff_dict_BlueShift          = 0x0c0a,
		cff_dict_BlueFuzz           = 0x0c0b,
		cff_dict_StemSnapH          = 0x0c0c,
		cff_dict_StemSnapV          = 0x0c0d,
		cff_dict_ForceBold          = 0x0c0e,
		// 0c 0f -Reserved-
		// 0c 10 -Reserved-
		cff_dict_LanguageGroup      = 0x0c11,
		cff_dict_ExpansionFactor    = 0x0c12,
		cff_dict_initialRandomSeed  = 0x0c13,
		cff_dict_SyntheticBase      = 0x0c14,
		cff_dict_PostScript         = 0x0c15,
		cff_dict_BaseFontName       = 0x0c16,
		cff_dict_BaseFontBlend      = 0x0c17,
		// 0c 18 -Reserved-
		// 0c 19 -Reserved-
		// 0c 1a -Reserved-
		// 0c 1b -Reserved-
		// 0c 1c -Reserved-
		// 0c 1d -Reserved-
		cff_dict_ROS                = 0x0c1e,
		cff_dict_CIDFontVersion     = 0x0c1f,
		cff_dict_CIDFontRevision    = 0x0c20,
		cff_dict_CIDFontType        = 0x0c21,
		cff_dict_CIDCount           = 0x0c22,
		cff_dict_UIDBase            = 0x0c23,
		cff_dict_FDArray            = 0x0c24,
		cff_dict_FDSelect           = 0x0c25,
		cff_dict_FontName           = 0x0c26
	};
	
	
	enum CFF_Variant_Type {
		cff_varnt_Error    = 0,
		cff_varnt_Bool     = 1,
		cff_varnt_Card     = 2,
		cff_varnt_SID      = 3,
		cff_varnt_Real     = 4,
		cff_varnt_Array    = 5,
		cff_varnt_Delta    = 6,
		cff_varnt_Operator = 7
	};

	
	struct CFF_Number {
		long long   card;
		int         exponent;
		uchar       type;
		bool        isCardinal() const;
		double      toDouble() const;
		int         toCardinal() const;
	};
	

	struct CFF_Variant {
		uchar type;
		QList<CFF_Number> array;
		CFF_Variant()                      : type(cff_varnt_Error) {}
		CFF_Variant(CFF_Number val)        : type(val.type), array() { array.append(val); }
		CFF_Variant(QList<CFF_Number> arr) : type(cff_varnt_Array), array(arr) {}
	};
	
	
	class CFF {
	public:
		/// For creating new CFF fonts
		CFF();
		CFF(const QByteArray& cff);

		uint readCard(uint pos) const;
		QByteArray readSegment(uint pos, uint size) const;
		QMap<operator_type,CFF_Variant> getDict(const QByteArray& dict) const;
		QList<QByteArray> readIndex(uint& pos) const;
		CFF extractSubset(uint faceIndex, QList<uint> cids) const;
		
		void dump();
		QByteArray dump(const CFF_Variant& var) const;
		
		const QByteArray& data() const {
			return m_bytes;
		}
		
		QList<QByteArray> fontNames() const {
			return m_fontTopDicts.keys();
		}
		
		uint offset(uint unscaled)
		{
			return unscaled * m_offsetSize;
		}
		
		QByteArray string(sid_type sid) const {
			return sid < m_strings.length()? m_strings[sid] : "";
		}
		
		sid_type sid(const QByteArray str) const {
			return m_sids.contains(str)? m_sids[str] : sid_max1;
		}
		
		void dump(QDataStream& out) const;
		
	private:
		QByteArray m_bytes;
		uint m_offsetSize;
		
		QList<QByteArray> m_names;
		QMap<QByteArray, QMap<uint,CFF_Variant> > m_fontTopDicts;
		QList<QByteArray> m_strings;
		QHash<QByteArray,uint> m_sids;
		QList<QByteArray> m_globalSubr;
		
		sid_type createSid(const QByteArray& str);
		
		CFF_Number parseDictElement(const QByteArray& dict, uint& pos) const;
		CFF_Number parseReal(const QByteArray& dict, uint& pos) const;
		CFF_Number parseCard(const QByteArray& dict, uint& pos) const;
		QList<sid_type> readCharset(uint nGlyphs, uint& pos) const;
		QList<uint> readEncoding(uint& pos) const;
		
		/// This write the header, name index and TopDict index for exactly one font.
		/// All unknown offset are preset as 4 byte cardinal and corresponding operator positions returned
		uint writeTopDict(QByteArray name, QMap<operator_type, CFF_Variant> dict, QList<QByteArray> oldStrings, QHash<operator_type, uint>& patches);
		/// this will write another segment and return the start offset
		uint writeSegment(const QByteArray& data);
		void patch(QHash<operator_type, uint> patchPositions, uint patchOffset, operator_type op, uint offset, uint length = 0);
		
		QByteArray makeCharset(QList<sid_type>) const;
		QByteArray makeEncoding(QList<uint>) const;
		QByteArray makeIndex(QList<QByteArray> data) const;
		QByteArray makeDict(QMap<operator_type, CFF_Variant> dict, QList<QByteArray> oldStrings, QHash<operator_type, uint>& patchAddresses);
	};
	
	QByteArray extractFace(const QByteArray& cff, int faceIndex);
	QByteArray subsetFace(const QByteArray& cff, QList<uint> cids);
}


#endif /* defined(Scribus__cff_h) */
