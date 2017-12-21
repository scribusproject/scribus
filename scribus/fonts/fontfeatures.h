/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef FONTFEATURES_H
#define FONTFEATURES_H

#include <qglobal.h>
#include <QStringList>

// Cannot use an enum here (yet) as C++98 doesn't guarantee an enum
// can reach 64bit in size
class FontFeatures
{
public:
	static quint64 NoFeatures;
	static quint64 CommonLigatures;
	static quint64 ContextualLigatures;
	static quint64 DiscretionaryLigatures;
	static quint64 HistoricalLigatures;
	static quint64 SmallCaps;
	static quint64 SmallCapsFromCaps;
	static quint64 PetiteCaps;
	static quint64 PetiteCapsFromCaps;
	static quint64 UnicaseCaps;
	static quint64 TiltingCaps;
	static quint64 LiningNumerals;
	static quint64 OldStyleNumerals;
	static quint64 ProportionalNumeralWidth;
	static quint64 TabularNumeralWidth;
	static quint64 DiagonalFractions;
	static quint64 StackedFractions;
	static quint64 Subscript;
	static quint64 Superscript;
	static quint64 Ordinals;
	static quint64 SlashedZero;
	static quint64 StyleSet01;
	static quint64 StyleSet02;
	static quint64 StyleSet03;
	static quint64 StyleSet04;
	static quint64 StyleSet05;
	static quint64 StyleSet06;
	static quint64 StyleSet07;
	static quint64 StyleSet08;
	static quint64 StyleSet09;
	static quint64 StyleSet10;
	static quint64 StyleSet11;
	static quint64 StyleSet12;
	static quint64 StyleSet13;
	static quint64 StyleSet14;
	static quint64 StyleSet15;
	static quint64 StyleSet16;
	static quint64 StyleSet17;
	static quint64 StyleSet18;
	static quint64 StyleSet19;
	static quint64 StyleSet20;

	static quint64 LigaturesMask;
	static quint64 CapsMask;
	static quint64 GlyphPositionMask;
	static quint64 NumeralStyleMask;
	static quint64 NumeralWidthMask;
	static quint64 NumeralFractionsMask;
	static quint64 StyleSetsMask;

	static quint64 fontFlagsFromList(QStringList features);
	static quint64 textFlagsFromList(QStringList features);
};

#endif
