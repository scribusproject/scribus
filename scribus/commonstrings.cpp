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

#include "commonstrings.h"
#include "commonstrings.moc"

QString CommonStrings::_Apply      = "";
QString CommonStrings::tr_Apply    = "";
QString CommonStrings::_Cancel     = "";
QString CommonStrings::tr_Cancel   = "";
QString CommonStrings::None        = "";
QString CommonStrings::trNone      = "";
QString CommonStrings::_OK         = "";
QString CommonStrings::tr_OK       = "";
QString CommonStrings::_Save       = "";
QString CommonStrings::tr_Save     = "";
QString CommonStrings::Warning     = "";
QString CommonStrings::trWarning   = "";

QString CommonStrings::pageSet1    = "";
QString CommonStrings::pageSet2    = "";
QString CommonStrings::pageSet3    = "";
QString CommonStrings::pageSet4    = "";
QString CommonStrings::trPageSet1  = "";
QString CommonStrings::trPageSet2  = "";
QString CommonStrings::trPageSet3  = "";
QString CommonStrings::trPageSet4  = "";
QString CommonStrings::pageLocLeft          = "";
QString CommonStrings::pageLocMiddle        = "";
QString CommonStrings::pageLocMiddleLeft    = "";
QString CommonStrings::pageLocMiddleRight   = "";
QString CommonStrings::pageLocRight         = "";
QString CommonStrings::trPageLocLeft        = "";
QString CommonStrings::trPageLocMiddle      = "";
QString CommonStrings::trPageLocMiddleLeft  = "";
QString CommonStrings::trPageLocMiddleRight = "";
QString CommonStrings::trPageLocRight       = "";

CommonStrings::CommonStrings()
{
	languageChange();
}

const QString& CommonStrings::translatePageSetString(const QString &untrString)
{
	if (untrString==pageSet1)
		return trPageSet1;
	if (untrString==pageSet2)
		return trPageSet2;
	if (untrString==pageSet3)
		return trPageSet3;
	if (untrString==pageSet4)
		return trPageSet4;
	return untrString;
}

const QString& CommonStrings::translatePageSetLocString(const QString &untrString)
{
	if (untrString==pageLocLeft)
		return trPageLocLeft;
	if (untrString==pageLocMiddle)
		return trPageLocMiddle;
	if (untrString==pageLocMiddleLeft)
		return trPageLocMiddleLeft;
	if (untrString==pageLocMiddleRight)
		return trPageLocMiddleRight;
	if (untrString==pageLocRight)
		return trPageLocRight;
	return untrString;
}

const QString& CommonStrings::untranslatePageSetString(const QString &trString)
{
	if (trString==trPageSet1)
		return pageSet1;
	if (trString==trPageSet2)
		return pageSet2;
	if (trString==trPageSet3)
		return pageSet3;
	if (trString==trPageSet4)
		return pageSet4;
	return trString;
}

const QString& CommonStrings::untranslatePageSetLocString(const QString &trString)
{
	if (trString==trPageLocLeft)
		return pageLocLeft;
	if (trString==trPageLocMiddle)
		return pageLocMiddle;
	if (trString==trPageLocMiddleLeft)
		return pageLocMiddleLeft;
	if (trString==trPageLocMiddleRight)
		return pageLocMiddleRight;
	if (trString==trPageLocRight)
		return pageLocRight;
	return trString;
}

void CommonStrings::languageChange()
{
	CommonStrings::_Apply     = "&Apply";
	CommonStrings::tr_Apply   = tr( "&Apply" );
	
	CommonStrings::_Cancel    = "&Cancel";
	CommonStrings::tr_Cancel  = tr( "&Cancel" );
	
	CommonStrings::None       = "None";
	CommonStrings::trNone     = tr( "None" );
	
	CommonStrings::_OK        = "&OK";
	CommonStrings::tr_OK      = tr( "&OK" );
	
	CommonStrings::_Save      = "&Save";
	CommonStrings::tr_Save    = tr( "&Save" );
	
	CommonStrings::Warning    = "Warning";
	CommonStrings::trWarning  = tr( "Warning" );
	
	CommonStrings::pageSet1    = "Single Page";
	CommonStrings::pageSet2    = "Double Sided";
	CommonStrings::pageSet3    = "3-Fold";
	CommonStrings::pageSet4    = "4-Fold";
	CommonStrings::trPageSet1  = tr( "Single Page" );
	CommonStrings::trPageSet2  = tr( "Double Sided" );
	CommonStrings::trPageSet3  = tr( "3-Fold" );
	CommonStrings::trPageSet4  = tr( "4-Fold" );
	
	CommonStrings::pageLocLeft          = "Left Page";
	CommonStrings::pageLocMiddle        = "Middle";
	CommonStrings::pageLocMiddleLeft    = "Middle Left";
	CommonStrings::pageLocMiddleRight   = "Middle Right";
	CommonStrings::pageLocRight         = "Right Page";
	CommonStrings::trPageLocLeft        = tr( "Left Page" );
	CommonStrings::trPageLocMiddle      = tr( "Middle" );
	CommonStrings::trPageLocMiddleLeft  = tr( "Middle Left" );
	CommonStrings::trPageLocMiddleRight = tr( "Middle Right" );
	CommonStrings::trPageLocRight       = tr( "Right Page" );
}

