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
		MultiProgressDialog(const QString& titleText, const QString & cancelButtonText, QWidget* parent=0, const char*name=0, bool modal=true, WFlags f=0);
		~MultiProgressDialog();
		
		void removeExtraProgressBars();
		bool addExtraProgressBars(const QStringList &barsList, const QStringList &barsTexts);
		bool setLabel(const QString &barName, const QString & newLabel);
		bool setTotalSteps(const QString &barName, int totalSteps);
		bool setProgress(const QString &barName, int progress);
		bool setProgress(const QString &barName, int progress, int totalSteps);
		bool setOverallTotalSteps(int totalSteps);
		bool setOverallProgress(int progress);
		bool setOverallProgress(int progress, int totalSteps);
		bool setupBar(const QString &barName, const QString &barText, int progress, int totalSteps);
		void setCancelButtonText(const QString & cancelButtonText);
		
	protected:
		QStringList progressBarTitles;
		QMap<QString, QProgressBar*> progressBars;
		QMap<QString, QLabel*> progressLabels;
};

#endif
