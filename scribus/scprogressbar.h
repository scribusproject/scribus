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
#ifndef SCPROGRESSBAR_H
#define SCPROGRESSBAR_H

#include <QProgressBar>

/*!
 * @brief A Progress Bar that allows a number (X of Y) style or standard % indicator
 */
class ScProgressBar : public QProgressBar
{
	Q_OBJECT
	
	public:
		ScProgressBar( bool showNumbers, QWidget *parent=0);
		ScProgressBar( bool showNumbers, int totalSteps, QWidget* parent=0);
		virtual bool setIndicator( QString & indicator, int progress, int totalSteps );

		~ScProgressBar();
		
	protected:
		bool m_useNumeric;

};

#endif
