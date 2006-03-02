/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
*   Copyright (C) 2005 by Craig Bradney                                   *
*   cbradney@zip.com.au                                                   *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************/

#ifndef MULTIPROGRESSDIALOG_H
#define MULTIPROGRESSDIALOG_H

#include "multiprogressdialogbase.h"

#include <qprogressbar.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qstringlist.h>
#include <qstring.h>
#include <qdialog.h>
#include <qmap.h>

class MultiProgressDialog : public MultiProgressDialogBase
{
	Q_OBJECT

	public:
		MultiProgressDialog(QWidget* parent=0, const char*name=0, bool modal=false, WFlags f=0);
		/**
		 * Create a multi progress bar dialog for long operations with multiple steps. The dialog includes
		 * one standard progress bar, typically for the overall progress and others may be added easily.
		 * @param titleText Title of the dialog
		 * @param cancelButtonText Text of the cancel button.. Cancel, Close, @sa CommontStrings::
		 * @param parent Parent widget for the dialog, commonly ScMW
		 * @param name
		 * @param modal 
		 * @param f 
		 * @return 
		 */
		MultiProgressDialog(const QString& titleText, const QString & cancelButtonText, QWidget* parent=0, const char*name=0, bool modal=true, WFlags f=0);
		~MultiProgressDialog();
		
		/**
		 * Remove all progress bars other than the main one
		 */
		void removeExtraProgressBars();
		/**
		 * Add a list of progress bars, where barsList contains a list of references and barTexts
		 * contains the labels for the bars.
		 * Eg:
		 * "MYBAR1" -> "My Bar 1:"
		 * "MYBAR2" -> "My Bar 2:"
		 * @param barsList 
		 * @param barsTexts 
		 * @return Success
		 */
		bool addExtraProgressBars(const QStringList &barsList, const QStringList &barsTexts);
		/**
		 * Set a new label for a user defined progress bar
		 * @param barName Progress bar name
		 * @param newLabel New label
		 * @return Success
		 */
		bool setLabel(const QString &barName, const QString & newLabel);
		/**
		 * Set the total steps for a user defined progress bar
		 * @param barName 
		 * @param totalSteps 
		 * @return Success
		 */
		bool setTotalSteps(const QString &barName, int totalSteps);
		/**
		 * Set the progress for a user defined progress bar
		 * @param barName 
		 * @param progress 
		 * @return 
		 */
		bool setProgress(const QString &barName, int progress);
		/**
		 * Set the prgress and total steps for a user defined progress bar
		 * @param barName 
		 * @param progress 
		 * @param totalSteps 
		 * @return 
		 */
		bool setProgress(const QString &barName, int progress, int totalSteps);
		/**
		 * Set the overall total steps for the dialog
		 * @param totalSteps 
		 */
		void setOverallTotalSteps(int totalSteps);
		/**
		 * Set the overall progress for the dialog
		 * @param progress 
		 */
		void setOverallProgress(int progress);
		/**
		 * Set the overall progress and total steps for the dialog
		 * @param progress 
		 * @param totalSteps 
		 */
		void setOverallProgress(int progress, int totalSteps);
		/**
		 * Create a new progress bar in one step
		 * @param barName 
		 * @param barText 
		 * @param progress 
		 * @param totalSteps 
		 * @return 
		 */
		bool setupBar(const QString &barName, const QString &barText, int progress, int totalSteps);
		/**
		 * Set the cancel button text
		 * @param cancelButtonText 
		 */
		void setCancelButtonText(const QString & cancelButtonText);
		
	protected:
		QStringList progressBarTitles;
		QMap<QString, QProgressBar*> progressBars;
		QMap<QString, QLabel*> progressLabels;
};

#endif
