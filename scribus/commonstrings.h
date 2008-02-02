/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
	begin                : Aug 2005
	copyright            : (C) 2005 by Craig Bradney
	email                : cbradney@zip.com.au
***************************************************************************/

/***************************************************************************
*                                                                         *
*   ScMW program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#ifndef COMMONSTRINGS_H
#define COMMONSTRINGS_H

#include <QObject>
#include <QString>

class QEvent;

#include "scribusapi.h"

/*! \brief A simple common strings class to reduce the string count and ease
 the translation process a little.
 - Contains untranslated and translated versions of the strings to
   help with reduction of future reliance of translated strings in
   "real" code
 - Currently stores GUI strings such as &OK and its translated
   version, but not the string without the &.
 - BE CAREFUL with context. Simple strings should be stored here
   that are obviously a verb or a noun, and if multiple contexts
   don't have different translations relating to verb/noun/gender
   in some languages. This is the reason basic words have been
   done initially.

 trNone = tr("None") was removed because it's problematic in
   East European langiages.
*/
class SCRIBUS_API CommonStrings : public QObject
{
	Q_OBJECT
public:
	CommonStrings();
	
	virtual void changeEvent(QEvent *e);

	/**
	 * \brief Return the translated Page Set string if given the "untranslated" one
	 * @param untrString "untranslated" string
	 * @return translated string
	 */
	static const QString& translatePageSetString(const QString &untrString);
	/**
	 * \brief Return the translated Page Set Location string if given the "untranslated" one
	 * @param untrString "untranslated" string
	 * @return translated string
	 */
	static const QString& translatePageSetLocString(const QString &untrString);
	/**
	 * \brief Return the untranslated Page Set string if given the translated one
	 * @param trString translated string
	 * @return "untranslated" string
	 */
	static const QString& untranslatePageSetString(const QString &trString);
	/**
	 * \brief Return the untranslated Page Set Location string if given the translated one
	 * @param trString translated string
	 * @return "untranslated" string
	 */
	static const QString& untranslatePageSetLocString(const QString &trString);
	/**
	 * \brief Return the untranslated Page Set Location string if given the translated one
	 * @param trString translated string
	 * @return "untranslated" string
	 */
	static const QString& translatePenStyleName(Qt::PenStyle ps);
	static void languageChange();
	static QString _Apply;
	static QString tr_Apply;
	static QString _Cancel;
	static QString tr_Cancel;
	static QString None;
	static QString tr_None;
	static QString tr_NoneColor;
	static QString _OK;
	static QString tr_OK;
	static QString _Save;
	static QString tr_Save;
	static QString Warning;
	static QString trWarning;
	//! \brief Translated "Yes"
	static QString trYes;
	//! \brief Translated "No"
	static QString trNo;
	//! \brief Translated "Yes" with key accelerator "&Yes"
	static QString trYesKey;
	//! \brief Translated "No" with key accelerator "&No"
	static QString trNoKey;
	
	//Item Types
	static QString itemType_TextFrame;
	static QString itemType_ImageFrame;
	static QString itemType_Line;
	static QString itemType_Polygon;
	static QString itemType_Polyline;
	static QString itemType_PathText;
	static QString itemType_LatexFrame;
	static QString itemType_Multiple;
	
	static QString itemSubType_PDF_PushButton;
	static QString itemSubType_PDF_TextField;
	static QString itemSubType_PDF_CheckBox;
	static QString itemSubType_PDF_ComboBox;
	static QString itemSubType_PDF_ListBox;
	static QString itemSubType_PDF_TextAnnotation;
	static QString itemSubType_PDF_LinkAnnotation;
	
	//Page Size
	static QString customPageSize;
	static QString trCustomPageSize;

	//Page Sets
	static QString pageSet1;
	static QString pageSet2;
	static QString pageSet3;
	static QString pageSet4;
	static QString trPageSet1;
	static QString trPageSet2;
	static QString trPageSet3;
	static QString trPageSet4;
	static QString pageLocLeft;
	static QString pageLocMiddle;
	static QString pageLocMiddleLeft;
	static QString pageLocMiddleRight;
	static QString pageLocRight;
	static QString trPageLocLeft;
	static QString trPageLocMiddle;
	static QString trPageLocMiddleLeft;
	static QString trPageLocMiddleRight;
	static QString trPageLocRight;
	
	//Master Page Default Names
	static QString masterPageNormal;
	static QString trMasterPageNormal;
	static QString masterPageNormalLeft;
	static QString trMasterPageNormalLeft;
	static QString masterPageNormalMiddle;
	static QString trMasterPageNormalMiddle;
	static QString masterPageNormalRight;
	static QString trMasterPageNormalRight;
	
	//Pen Styles
	static QString trPenStyle_SolidLine;
	static QString trPenStyle_DashedLine;
	static QString trPenStyle_DottedLine;
	static QString trPenStyle_DashDotLine;
	static QString trPenStyle_DashDotDotLine;
	
	//Days and Months
	static QString monday; 
	static QString tuesday;
	static QString wednesday;
	static QString thursday;
	static QString friday;
	static QString saturday;
	static QString sunday;

	static QString january;
	static QString february;
	static QString march;
	static QString april;
	static QString may;
	static QString june;
	static QString july;
	static QString august;
	static QString september;
	static QString october;
	static QString november;
	static QString december;
	
	//Color Related
	static QString trRGB;
	static QString trCMYK;
	static QString trGrayscale;
	static QString trDuotone;
	static QString trUnknownCS;

	//Color Blindness
	static QString trVisionNormal;
	static QString trVisionProtanopia;
	static QString trVisionDeuteranopia;
	static QString trVisionTritanopia;
	static QString trVisionFullColorBlind;
	
	//Tab Fill Custom
	static QString trCustomTabFill;
	
	//Paragraph Style Optical Margins
	static QString trOpticalMarginsNone;
	static QString trOpticalMarginsLeftProtruding;
	static QString trOpticalMarginsRightProtruding;
	static QString trOpticalMarginsLeftHangPunct;
	static QString trOpticalMarginsRightHangPunct;
	static QString trOpticalMarginsDefault;
	
	//Paragraph Style Word Tracking
	static QString trMinWordTracking;
	static QString trMaxWordTracking;
	
	//Paragraph Style Glyph Extension
	static QString trMinGlyphExtension;
	static QString trMaxGlyphExtension;

	//PostScript or other PDL strings
	static QString PostScript;
	static QString trPostScript;
	static QString PDF_1_3;
	static QString PDF_1_4;
	static QString PDF_1_5;
	static QString PDF_X3;

	static QString PostScript1;
	static QString trPostScript1;
	static QString PostScript2;
	static QString trPostScript2;
	static QString PostScript3;
	static QString trPostScript3;
	static QString WindowsGDI;
	static QString trWindowsGDI;
};

#endif
