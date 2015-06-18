//
//  sfnt_format.h
//  Scribus
//
//  Created by Andreas Vox on 26.04.15.
//
//

#ifndef Scribus_sfnt_format_h
#define Scribus_sfnt_format_h

namespace sfnt {
    
enum TTC_Format {
    ttc_Tag = 0,
    ttc_Version = 4,
    ttc_numFonts = 8,
    ttc_OffsetTables = 12,
    ttc_ulDsigTag_OffsetTables = 0,
    ttc_ulDsigLength_OffsetTables = 4,
    ttc_ulDsigOffset_OffsetTables = 8
};


enum TTF_Format {
    ttf_sfnt_version = 0,
    ttf_numtables = 4,
    ttf_searchRange = 6,
    ttf_entrySelector = 8,
    ttf_rangeShift = 10,
    ttf_TableRecords = 12,
    ttf_TableRecord_tag = 0,
    ttf_TableRecord_checkSum = 4,
    ttf_TableRecord_offset = 8,
    ttf_TableRecord_length = 12,
    ttf_TableRecord_Size = 16
};


enum TTF_head_Format {
    ttf_head_version = 0,
    ttf_head_fontRevision = 4,
    ttf_head_checkSumAdjustment = 8,
    ttf_head_magicNumber = 12,
    ttf_head_flags = 16,
    ttf_head_unitsPerEm = 18,
    ttf_head_created = 20,
    ttf_head_modified = 28,
    ttf_head_xMin = 36,
    ttf_head_yMin = 38,
    ttf_head_xMax = 40,
    ttf_head_yMax = 42,
    ttf_head_macStyle = 44,
    ttf_head_lowestRecPPEM = 46,
    ttf_head_fontDirectionHint = 48,
    ttf_head_indexToLocFormat = 50,
    ttf_head_glyphDataFormat = 52
};


enum TTF_cmap_Format {
    ttf_cmap_version = 0,
    ttf_cmap_numberSubtables = 2,
    ttf_cmap_encodings = 4,
    ttf_cmap_encoding_platformID = 0,
    ttf_cmap_encoding_platformSpecificID = 2,
    ttf_cmap_encoding_offset = 4,
    ttf_cmap_encoding_Size = 8,
    ttf_cmapx_format = 0,
    ttf_cmap4_length = 2,
    ttf_cmap4_language = 4,
    ttf_cmap4_segCountX2 = 6,
    ttf_cmap4_searchRange = 8,
    ttf_cmap4_entrySelector = 10,
    ttf_cmap4_rangeShift = 12,
    ttf_cmap4_EndCodes = 14,
    ttf_cmap4_reservePad_EndCodes = 0,
    ttf_cmap4_StartCodes_EndCodes = 2,
    ttf_cmap4_IdDeltas_StartCodes = 0,
    ttf_cmap4_IdRangeOffsets_IdDeltas = 0,
    ttf_cmap4_GlyphIds_IdRangeOffsets = 0,
    ttf_cmap6_firstCode = 6,
    ttf_cmap6_entryCount = 8,
    ttf_cmap6_glyphIndexArray = 10,
    ttf_cmap12_nGroups = 12,
    ttf_cmap12_Groups = 16,
    ttf_cmap12_Group_startCharCode = 0,
    ttf_cmap12_Group_endCharCode = 4,
    ttf_cmap12_Group_startGlyphCode = 8,
    ttf_cmap12_Group_Size = 12
};


enum TTF_glyf_Format {
    ttf_glyf_numberOfContours = 0,
    ttf_glyf_Max_numberOfContours = 0x7fff,

    ttf_glyf_xMin = 2,
    ttf_glyf_yMin = 4,
    ttf_glyf_xMax = 6,
    ttf_glyf_yMax = 8,
    ttf_glyf_headerSize = 10,
    ttf_glyf_Components = 10,
    ttf_glyf_Component_flags = 0,
    ttf_glyf_Component_glyphIndex = 2,
    ttf_glyf_Component_arguments = 4
};


enum TTF_glyf_ComponentFlags {
    ttf_glyf_ComponentFlag_ARG_1_AND_2_ARE_WORDS = 0x0001,
    ttf_glyf_ComponentFlag_ARGS_ARE_XYVALUES     = 0x0002,
    ttf_glyf_ComponentFlag_ROUND_XY_TO_GRID      = 0x0004,
    ttf_glyf_ComponentFlag_WE_HAVE_A_SCALE       = 0x0008,
    /* 0x0010 reserved */
    ttf_glyf_ComponentFlag_MORE_COMPONENTS       = 0x0020,
    ttf_glyf_ComponentFlag_WE_HAVE_AN_X_AND_Y_SCALE = 0x0040,
    ttf_glyf_ComponentFlag_WE_HAVE_A_TWO_BY_TWO = 0x0080,
    ttf_glyf_ComponentFlag_WE_HAVE_INSTRUCTIONS = 0x0100,
    ttf_glyf_ComponentFlag_USE_MY_METRICS = 0x0200,
    ttf_glyf_ComponentFlag_OVERLAP_COMPOUND = 0x0400,
    ttf_glyf_ComponentFlag_SCALED_COMPONENT_OFFSET = 0x0800,
    ttf_glyf_ComponentFlag_UNSCALED_COMPONENT_OFFSET = 0x1000,
};

    
enum ttf_hhea_Format {
    ttf_hhea_numOfLongHorMetrics = 34
};

    
enum ttf_maxp_Format {
    ttf_maxp_version = 0,
    ttf_maxp_numGlyphs = 4,
    ttf_maxp_maxPoints = 6,
    ttf_maxp_maxContours = 8,
    ttf_maxp_maxComponentPoints = 10,
    ttf_maxp_maxComponentContours = 12,
    ttf_maxp_maxZones = 14,
    ttf_maxp_maxTwilightPoints = 16,
    ttf_maxp_maxStorage = 18,
    ttf_maxp_maxFunctionDefs = 20,
    ttf_maxp_maxInstructionDefs = 22,
    ttf_maxp_maxStackElements = 24,
    ttf_maxp_maxSizeOfInstructions = 26,
    ttf_maxp_maxComponentElements = 28,
    ttf_maxp_maxComponentDepth = 30,
    ttf_maxp_Size = 32
};

enum TTF_os2_Format {
    ttf_os2_version = 0,
    ttf_os2_xAvgCharWidth = 2,
    ttf_os2_usWeightClass = 4,
    ttf_os2_usWidthClass = 6,
    ttf_os2_fsType = 8,
    ttf_os2_ySubscriptXSize = 10,
    ttf_os2_ySubscriptYSize = 12,
    ttf_os2_ySubscriptXOffset = 14,
    ttf_os2_ySubscriptYOffset = 16,
    ttf_os2_ySuperscriptXSize = 18,
    ttf_os2_ySuperscriptYSize = 20,
    ttf_os2_ySuperscriptXOffset = 22,
    ttf_os2_ySuperscriptYOffset = 24,
    ttf_os2_yStrikeoutSize = 26,
    ttf_os2_yStrikeoutPosition = 28,
    ttf_os2_sFamilyClass = 30,
    ttf_os2_panose = 32,
    ttf_os2_ulCharRange = 42,
    ttf_os2_ulUnicodeRange1 = 42,
    ttf_os2_ulUnicodeRange2 = 46,
    ttf_os2_ulUnicodeRange3 = 50,
    ttf_os2_ulUnicodeRange4 = 54,
    ttf_os2_achVendID = 58,
    ttf_os2_fsSelection = 62,
    ttf_os2_usFirstCharIndex = 64,
    ttf_os2_usLastCharIndex = 66,
    ttf_os2_sTypeAscender = 68,
    ttf_os2_sTypeoDescender = 70,
    ttf_os2_sTypoLineGap = 72,
    ttf_os2_usWinAscent = 74,
    ttf_os2_usWinDescent = 76,
    ttf_os2_f1_ulCodePageRange1 = 78,
    ttf_os2_f1_ulCodePageRange2 = 82,
    ttf_os2_f2_sxHeight = 86,
    ttf_os2_f2_CapHeight = 88,
    ttf_os2_f2_usDefaultChar = 90,
    ttf_os2_f2_usBreakChar = 92,
    ttf_os2_f2_usMaxContext = 94,
    ttf_os2_f5_usLowerOpticalPointSize = 96,
    ttf_os2_f5_usUpperOpticalPointSize = 98
};
    
enum TTF_post_Format {
    ttf_post_format = 0,
    /* more fields */
    ttf_post_header_length = 32
};


enum TTF_post_format {
    post_format10 = 0x00010000,
    post_format20 = 0x00020000,
    post_format25 = 0x00025000,
    post_format30 = 0x00030000,
    post_format40 = 0x00040000,
};
    
} // namespace

#endif
