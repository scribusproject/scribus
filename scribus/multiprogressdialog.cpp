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

#include "multiprogressdialog.h"

MultiProgressDialog::MultiProgressDialog(QWidget* parent, Qt::WFlags f)
: QDialog(parent, f)
{
	setupUi(this);
	connect(buttonCancel, SIGNAL(clicked()), this, SLOT(emitCancel()));
}

MultiProgressDialog::MultiProgressDialog(const QString& titleText, const QString & cancelButtonText, QWidget* parent, Qt::WFlags f)
: QDialog(parent, f)
{
	setupUi(this);
	setWindowTitle(titleText);
	buttonCancel->setText(cancelButtonText);
	connect(buttonCancel, SIGNAL(clicked()), this, SLOT(emitCancel()));
}

MultiProgressDialog::~MultiProgressDialog()
{
}

void MultiProgressDialog::emitCancel()
{
	emit canceled();
}

void MultiProgressDialog::removeExtraProgressBars()
{
	progressBars.clear();
	progressLabels.clear();
}

bool MultiProgressDialog::addExtraProgressBars(const QStringList &barsList, const QStringList &barsTexts, const QList<bool>& barsNumerical)
{
	uint barCount=barsList.count();
	if (barCount==0)
		return false;
	int gridLayoutRow=gridLayout->rowCount();
	for (uint i=0; i<barCount; ++i)
	{
		QString barName(barsList[i]);
		if(progressBarTitles.contains(barName))
			continue;
		progressBarTitles.append(barName);
		progressBars.insert(barName, new QProgressBar(this));
		if (barsNumerical[i])
			progressBars[barName]->setFormat(tr("%v of %m"));
		progressBars[barName]->setMinimumWidth(150);
		progressLabels.insert(barName, new QLabel(barsTexts[i], this));
		gridLayout->addWidget(progressLabels[barName], gridLayoutRow, 0);
		gridLayout->addWidget(progressBars[barName], gridLayoutRow, 1);
		++gridLayoutRow;
	}
	return true;
}

bool MultiProgressDialog::setLabel(const QString &barName, const QString & newLabel)
{
	if (progressLabels.contains(barName))
	{
		progressLabels[barName]->setText(newLabel);
		return true;
	}
	return false;
}

bool MultiProgressDialog::setTotalSteps(const QString &barName, int totalSteps)
{
	if (progressBars.contains(barName))
	{
		progressBars[barName]->setMaximum(totalSteps);
		return true;
	}
	return false;
}

bool MultiProgressDialog::setProgress(const QString &barName, int progress)
{
	if (progressBars.contains(barName))
	{
		progressBars[barName]->setValue(progress);
		return true;
	}
	return false;
}

bool MultiProgressDialog::setProgress(const QString &barName, int progress, int totalSteps)
{
	if (progressBars.contains(barName))
	{
		progressBars[barName]->setMaximum(totalSteps);
		progressBars[barName]->setValue(progress);
		return true;
	}
	return false;
}

bool MultiProgressDialog::setupBar(const QString &barName, const QString & barText, int progress, int totalSteps)
{
	if (progressLabels.contains(barName))
		progressLabels[barName]->setText(barText);
	else
		return false;
	if (progressBars.contains(barName))
	{
		progressBars[barName]->setMaximum(totalSteps);
		progressBars[barName]->setValue(progress);
		return true;
	}
	return false;
}


void MultiProgressDialog::setCancelButtonText(const QString & cancelButtonText)
{
	buttonCancel->setText(cancelButtonText);
}

void MultiProgressDialog::setOverallTotalSteps(int totalSteps)
{
	overallProgressBar->setMaximum(totalSteps);
}

void MultiProgressDialog::setOverallProgress(int progress)
{
	overallProgressBar->setValue(progress);
}

void MultiProgressDialog::setOverallProgress(int progress, int totalSteps)
{
	overallProgressBar->setMaximum(totalSteps);
	overallProgressBar->setValue(progress);
}
