/*
 For general Scribus (>=1.3.2) copyright and licensing information please refer
 to the COPYING file provided with the program. Following this notice may exist
 a copyright and/or license notice that predates the release of Scribus 1.3.2
 for which a new license (GPL+exception) is in place.
 */
/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/


//Syntax: ATTRDEF( datatype, gettername, name, defaultvalue)

ATTRDEF(ParagraphStyle::LineSpacingMode, lineSpacingMode, LineSpacingMode, ParagraphStyle::FixedLineSpacing)
ATTRDEF(double, lineSpacing, LineSpacing, 0.0)
ATTRDEF(ParagraphStyle::AlignmentType, alignment, Alignment, Leftaligned)
ATTRDEF(int, opticalMargins, OpticalMargins, 0)
ATTRDEF(int, hyphenationMode, HyphenationMode, 0)
ATTRDEF(double, minWordTracking, MinWordTracking, 1.0)
//ATTRDEF(double, maxWordTracking, MaxWordTracking, 1.0)
ATTRDEF(double, minGlyphExtension, MinGlyphExtension, 1.0)
ATTRDEF(double, maxGlyphExtension, MaxGlyphExtension, 1.0)
ATTRDEF(double, leftMargin, LeftMargin, 0.0)
ATTRDEF(double, rightMargin, RightMargin, 0.0)
ATTRDEF(double, firstIndent, FirstIndent, 0.0)
ATTRDEF(double, gapBefore, GapBefore, 0.0)
ATTRDEF(double, gapAfter, GapAfter, 0.0)
ATTRDEF(QList<TabRecord>, tabValues, TabValues, QList<TabRecord>())
ATTRDEF(bool, useBaselineGrid, UseBaselineGrid, false)
ATTRDEF(int, keepLinesStart, KeepLinesStart, 0)
ATTRDEF(int, keepLinesEnd, KeepLinesEnd, 0)
ATTRDEF(bool, keepWithNext, KeepWithNext, false)
ATTRDEF(bool, keepTogether, KeepTogether, false)
ATTRDEF(bool, hasDropCap, HasDropCap, false)
ATTRDEF(int, dropCapLines, DropCapLines, 2)
ATTRDEF(double, parEffectOffset, ParEffectOffset, 0.0)
ATTRDEF(bool, parEffectIndent, ParEffectIndent, false)
ATTRDEF(QString, peCharStyleName, PeCharStyleName,"")
ATTRDEF(bool, hasBullet, HasBullet, false)
ATTRDEF(QString, bulletStr, BulletStr, QString(QChar(0x2022)))
ATTRDEF(bool, hasNum, HasNum, false)
ATTRDEF(QString, numName, NumName, "")
ATTRDEF(int, numFormat, NumFormat, 0)
ATTRDEF(QString, numPrefix, NumPrefix, "")
ATTRDEF(QString, numSuffix, NumSuffix, ".")
ATTRDEF(int, numLevel, NumLevel, 0)
ATTRDEF(int, numStart, NumStart, 1)
ATTRDEF(int, numRestart, NumRestart, 0)
ATTRDEF(bool, numOther, NumOther, false)
ATTRDEF(bool, numHigher, NumHigher, true)

