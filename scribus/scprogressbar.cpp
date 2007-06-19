/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          scrpogressbar.cpp  -  description
                             -------------------
    begin                : Sat Jan 19 2002
    copyright            : (C) 2006 by Craig Bradney
    email                : cbradney@zip.com.au
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
 
#include "scprogressbar.h"

ScProgressBar::ScProgressBar(bool showNumbers, QWidget *parent) :
	QProgressBar(parent),
	m_useNumeric(showNumbers)
{
}

ScProgressBar::ScProgressBar(bool showNumbers, int totalSteps, QWidget* parent) :
	QProgressBar(parent),
	m_useNumeric(showNumbers)
{
	setMaximum(totalSteps);
}


ScProgressBar::~ScProgressBar()
{
}

bool ScProgressBar::setIndicator(QString & indicator, int progress, int totalSteps)
{
	if ( !totalSteps )
		return FALSE;
	if ( progress < 0 ) {
		indicator = QString::fromLatin1("");
		return TRUE;
	} else {
		if (!m_useNumeric)
			return QProgressBar::setIndicator(indicator, progress, totalSteps);
		else
		{
			indicator = tr("%1 of %2").arg(progress).arg(totalSteps);
			return true;
		}
	}
}

