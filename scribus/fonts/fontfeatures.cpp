/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "fontfeatures.h"

quint64 FontFeatures::NoFeatures               = 0;
quint64 FontFeatures::CommonLigatures          = 1;
quint64 FontFeatures::ContextualLigatures      = ((quint64) 1 << 1);
quint64 FontFeatures::DiscretionaryLigatures   = ((quint64) 1 << 2);
quint64 FontFeatures::HistoricalLigatures      = ((quint64) 1 << 3);
quint64 FontFeatures::SmallCaps                = ((quint64) 1 << 4);
quint64 FontFeatures::SmallCapsFromCaps        = ((quint64) 1 << 5);
quint64 FontFeatures::PetiteCaps               = ((quint64) 1 << 6);
quint64 FontFeatures::PetiteCapsFromCaps       = ((quint64) 1 << 7);
quint64 FontFeatures::UnicaseCaps              = ((quint64) 1 << 8);
quint64 FontFeatures::TiltingCaps              = ((quint64) 1 << 9);
quint64 FontFeatures::LiningNumerals           = ((quint64) 1 << 10);
quint64 FontFeatures::OldStyleNumerals         = ((quint64) 1 << 11);
quint64 FontFeatures::ProportionalNumeralWidth = ((quint64) 1 << 12);
quint64 FontFeatures::TabularNumeralWidth      = ((quint64) 1 << 13);
quint64 FontFeatures::DiagonalFractions        = ((quint64) 1 << 14);
quint64 FontFeatures::StackedFractions         = ((quint64) 1 << 15);
quint64 FontFeatures::Subscript                = ((quint64) 1 << 16);
quint64 FontFeatures::Superscript              = ((quint64) 1 << 17);
quint64 FontFeatures::Ordinals                 = ((quint64) 1 << 18);
quint64 FontFeatures::SlashedZero              = ((quint64) 1 << 19);
quint64 FontFeatures::StyleSet01               = ((quint64) 1 << 20);
quint64 FontFeatures::StyleSet02               = ((quint64) 1 << 21);
quint64 FontFeatures::StyleSet03               = ((quint64) 1 << 22);
quint64 FontFeatures::StyleSet04               = ((quint64) 1 << 23);
quint64 FontFeatures::StyleSet05               = ((quint64) 1 << 24);
quint64 FontFeatures::StyleSet06               = ((quint64) 1 << 25);
quint64 FontFeatures::StyleSet07               = ((quint64) 1 << 26);
quint64 FontFeatures::StyleSet08               = ((quint64) 1 << 27);
quint64 FontFeatures::StyleSet09               = ((quint64) 1 << 28);
quint64 FontFeatures::StyleSet10               = ((quint64) 1 << 29);
quint64 FontFeatures::StyleSet11               = ((quint64) 1 << 30);
quint64 FontFeatures::StyleSet12               = ((quint64) 1 << 31);
quint64 FontFeatures::StyleSet13               = ((quint64) 1 << 32);
quint64 FontFeatures::StyleSet14               = ((quint64) 1 << 33);
quint64 FontFeatures::StyleSet15               = ((quint64) 1 << 34);
quint64 FontFeatures::StyleSet16               = ((quint64) 1 << 35);
quint64 FontFeatures::StyleSet17               = ((quint64) 1 << 36);
quint64 FontFeatures::StyleSet18               = ((quint64) 1 << 37);
quint64 FontFeatures::StyleSet19               = ((quint64) 1 << 38);
quint64 FontFeatures::StyleSet20               = ((quint64) 1 << 39);

quint64 FontFeatures::LigaturesMask = FontFeatures::CommonLigatures + FontFeatures::ContextualLigatures +
                                      FontFeatures::DiscretionaryLigatures + FontFeatures::HistoricalLigatures;

quint64 FontFeatures::CapsMask = FontFeatures::SmallCaps + FontFeatures::SmallCapsFromCaps +
                                 FontFeatures::PetiteCaps + FontFeatures::PetiteCapsFromCaps +
                                 FontFeatures::UnicaseCaps + FontFeatures::TiltingCaps;

quint64 FontFeatures::GlyphPositionMask = FontFeatures::Subscript + FontFeatures::Superscript + FontFeatures::Ordinals;
quint64 FontFeatures::NumeralStyleMask = FontFeatures::LiningNumerals + FontFeatures::OldStyleNumerals;
quint64 FontFeatures::NumeralWidthMask = FontFeatures::ProportionalNumeralWidth + FontFeatures::TabularNumeralWidth;
quint64 FontFeatures::NumeralFractionsMask = FontFeatures::DiagonalFractions + FontFeatures::StackedFractions;

quint64 FontFeatures::StyleSetsMask = FontFeatures::StyleSet01 + FontFeatures::StyleSet02 + FontFeatures::StyleSet03 + FontFeatures::StyleSet04 + FontFeatures::StyleSet05 +
                                      FontFeatures::StyleSet06 + FontFeatures::StyleSet07 + FontFeatures::StyleSet08 + FontFeatures::StyleSet09 + FontFeatures::StyleSet10 +
	                                  FontFeatures::StyleSet11 + FontFeatures::StyleSet12 + FontFeatures::StyleSet13 + FontFeatures::StyleSet14 + FontFeatures::StyleSet15 +
	                                  FontFeatures::StyleSet16 + FontFeatures::StyleSet17 + FontFeatures::StyleSet18 + FontFeatures::StyleSet19 + FontFeatures::StyleSet20;

quint64 FontFeatures::fontFlagsFromList(QStringList features)
{
	quint64 flags = 0;

	for (int i = 0; i < features.count(); i++)
	{
		const QString& feature = features.at(i);

		if (feature == "liga")
			flags |= FontFeatures::CommonLigatures;
		else if (feature == "clig")
			flags |= FontFeatures::ContextualLigatures;
		else if (feature == "dlig")
			flags |= FontFeatures::DiscretionaryLigatures;
		else if (feature == "hlig")
			flags |= FontFeatures::HistoricalLigatures;

		else if (feature == "smcp")
			flags |= FontFeatures::SmallCaps;
		else if (feature == "c2sc")
			flags |= FontFeatures::SmallCapsFromCaps;
		else if (feature == "pcap")
			flags |= FontFeatures::PetiteCaps;
		else if (feature == "c2pc")
			flags |= FontFeatures::PetiteCapsFromCaps;
		else if (feature == "unic")
			flags |= FontFeatures::UnicaseCaps;
		else if (feature == "titl")
			flags |= FontFeatures::TiltingCaps;

		else if (feature == "lnum")
			flags |= FontFeatures::LiningNumerals;
		else if (feature == "onum")
			flags |= FontFeatures::OldStyleNumerals;

		else if (feature == "pnum")
			flags |= FontFeatures::ProportionalNumeralWidth;
		else if (feature == "tnum")
			flags |= FontFeatures::TabularNumeralWidth;

		else if (feature == "frac")
			flags |= FontFeatures::DiagonalFractions;
		else if (feature == "afrc")
			flags |= FontFeatures::StackedFractions;

		else if (feature == "subs")
			flags |= FontFeatures::Subscript;
		else if (feature == "sups")
			flags |= FontFeatures::Superscript;
		else if (feature == "ordn")
			flags |= FontFeatures::Ordinals;

		else if (feature == "zero")
			flags |= FontFeatures::SlashedZero;

		else if (feature == "ss01")
			flags |= FontFeatures::StyleSet01;
		else if (feature == "ss02")
			flags |= FontFeatures::StyleSet02;
		else if (feature == "ss03")
			flags |= FontFeatures::StyleSet03;
		else if (feature == "ss04")
			flags |= FontFeatures::StyleSet04;
		else if (feature == "ss05")
			flags |= FontFeatures::StyleSet05;
		else if (feature == "ss06")
			flags |= FontFeatures::StyleSet06;
		else if (feature == "ss07")
			flags |= FontFeatures::StyleSet07;
		else if (feature == "ss08")
			flags |= FontFeatures::StyleSet08;
		else if (feature == "ss09")
			flags |= FontFeatures::StyleSet09;
		else if (feature == "ss10")
			flags |= FontFeatures::StyleSet10;
		else if (feature == "ss11")
			flags |= FontFeatures::StyleSet11;
		else if (feature == "ss12")
			flags |= FontFeatures::StyleSet12;
		else if (feature == "ss13")
			flags |= FontFeatures::StyleSet13;
		else if (feature == "ss14")
			flags |= FontFeatures::StyleSet14;
		else if (feature == "ss15")
			flags |= FontFeatures::StyleSet15;
		else if (feature == "ss16")
			flags |= FontFeatures::StyleSet16;
		else if (feature == "ss17")
			flags |= FontFeatures::StyleSet17;
		else if (feature == "ss18")
			flags |= FontFeatures::StyleSet18;
		else if (feature == "ss19")
			flags |= FontFeatures::StyleSet19;
		else if (feature == "ss20")
			flags |= FontFeatures::StyleSet20;
	}

	return flags;
}

quint64 FontFeatures::textFlagsFromList(QStringList features)
{
	quint64 flags = 0;
	flags |= CommonLigatures;
	flags |= ContextualLigatures;

	for (int i = 0; i < features.count(); i++)
	{
		const QString& feature = features.at(i);

		if (feature == "-liga")
			flags &= ~FontFeatures::CommonLigatures;
		else if (feature == "-clig")
			flags &= ~FontFeatures::ContextualLigatures;
		else if (feature == "+dlig")
			flags |= FontFeatures::DiscretionaryLigatures;
		else if (feature == "+hlig")
			flags |= FontFeatures::HistoricalLigatures;

		else if (feature == "+smcp")
			flags |= FontFeatures::SmallCaps;
		else if (feature == "+c2sc")
			flags |= FontFeatures::SmallCapsFromCaps;
		else if (feature == "+pcap")
			flags |= FontFeatures::PetiteCaps;
		else if (feature == "+c2pc")
			flags |= FontFeatures::PetiteCapsFromCaps;
		else if (feature == "+unic")
			flags |= FontFeatures::UnicaseCaps;
		else if (feature == "+titl")
			flags |= FontFeatures::TiltingCaps;

		else if (feature == "+lnum")
			flags |= FontFeatures::LiningNumerals;
		else if (feature == "+onum")
			flags |= FontFeatures::OldStyleNumerals;

		else if (feature == "+pnum")
			flags |= FontFeatures::ProportionalNumeralWidth;
		else if (feature == "+tnum")
			flags |= FontFeatures::TabularNumeralWidth;

		else if (feature == "+frac")
			flags |= FontFeatures::DiagonalFractions;
		else if (feature == "+afrc")
			flags |= FontFeatures::StackedFractions;

		else if (feature == "+subs")
			flags |= FontFeatures::Subscript;
		else if (feature == "+sups")
			flags |= FontFeatures::Superscript;
		else if (feature == "+ordn")
			flags |= FontFeatures::Ordinals;

		else if (feature == "+zero")
			flags |= FontFeatures::SlashedZero;

		else if (feature == "+ss01")
			flags |= FontFeatures::StyleSet01;
		else if (feature == "+ss02")
			flags |= FontFeatures::StyleSet02;
		else if (feature == "+ss03")
			flags |= FontFeatures::StyleSet03;
		else if (feature == "+ss04")
			flags |= FontFeatures::StyleSet04;
		else if (feature == "+ss05")
			flags |= FontFeatures::StyleSet05;
		else if (feature == "+ss06")
			flags |= FontFeatures::StyleSet06;
		else if (feature == "+ss07")
			flags |= FontFeatures::StyleSet07;
		else if (feature == "+ss08")
			flags |= FontFeatures::StyleSet08;
		else if (feature == "+ss09")
			flags |= FontFeatures::StyleSet09;
		else if (feature == "+ss10")
			flags |= FontFeatures::StyleSet10;
		else if (feature == "+ss11")
			flags |= FontFeatures::StyleSet11;
		else if (feature == "ss12")
			flags |= FontFeatures::StyleSet12;
		else if (feature == "+ss13")
			flags |= FontFeatures::StyleSet13;
		else if (feature == "+ss14")
			flags |= FontFeatures::StyleSet14;
		else if (feature == "+ss15")
			flags |= FontFeatures::StyleSet15;
		else if (feature == "+ss16")
			flags |= FontFeatures::StyleSet16;
		else if (feature == "+ss17")
			flags |= FontFeatures::StyleSet17;
		else if (feature == "+ss18")
			flags |= FontFeatures::StyleSet18;
		else if (feature == "+ss19")
			flags |= FontFeatures::StyleSet19;
		else if (feature == "+ss20")
			flags |= FontFeatures::StyleSet20;
	}

	return flags;
}