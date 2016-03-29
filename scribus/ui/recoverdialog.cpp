/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/**************************************************************************
*   Copyright (C) 2016 by Franz Schmid                                    *
*   franz.schmid@altmuehlnet.de                                           *
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
*   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.             *
***************************************************************************/

#include "recoverdialog.h"
#include "iconmanager.h"
#include <QList>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QCheckBox>
#include <QFileInfo>

RecoverDialog::RecoverDialog(QWidget* parent, QStringList files) : QDialog(parent)
{
	setupUi(this);
	setModal(true);
	setWindowIcon(IconManager::instance()->loadPixmap("AppIcon.png"));
	m_files = files;
	recoverFiles.clear();
	filesList->setColumnWidth(0, 24);
	QHeaderView *header = filesList->horizontalHeader();
	header->setStretchLastSection(true);
	header->setSectionsClickable(true );
	header->setSectionsMovable( false );
	header->setSectionResizeMode(QHeaderView::Fixed);
	header->setHighlightSections(false);
	filesList->horizontalHeaderItem(0)->setIcon(style()->standardIcon(QStyle::SP_DialogApplyButton));
	filesList->horizontalHeaderItem(0)->setToolTip("<qt>" + tr("Click here to toggle your choice") + "</qt>" );
	filesList->horizontalHeaderItem(3)->setToolTip("<qt>" + tr("Autosaved files are files are automatically saved versions of a document that was opened in a previous Scribus session.") + "<br>" + "Emergency files are files that Scribus was able to salvage after a crash." + "</qt>" );
	updateFilesTable();
	connect(buttonRecover, SIGNAL(clicked()), this, SLOT(doRecover()));
	connect(buttonRemove, SIGNAL(clicked()), this, SLOT(doRemove()));
	connect(header, SIGNAL(sectionClicked(int)), this, SLOT(toggleAllfromHeader(int)));
}

void RecoverDialog::toggleAllfromHeader(int col)
{
	if (col != 0)
		return;
	for (int a = 0; a < filesList->rowCount(); a++)
	{
		((QCheckBox*)(filesList->cellWidget(a, 0)))->toggle();
	}
	handleItemClick();
}

void RecoverDialog::handleItemClick()
{
	bool found = false;
	for (int a = 0; a < filesList->rowCount(); a++)
	{
		if (((QCheckBox*)(filesList->cellWidget(a, 0)))->isChecked())
		{
			found = true;
			break;
		}

	}
	buttonRecover->setEnabled(found);
	buttonRemove->setEnabled(found);
}

void RecoverDialog::doRecover()
{
	for (int a = 0; a < filesList->rowCount(); a++)
	{
		if (((QCheckBox*)(filesList->cellWidget(a, 0)))->isChecked())
		{
			recoverFiles.append(m_files[a]);
			QString nName = m_files[a];
			if (nName.contains("emergency"))
				nName.replace("emergency", tr("(recovered)"));
			if (nName.contains("autosave"))
				nName.replace("autosave", tr("(recovered)"));
			recoverNames.append(nName);
		}
	}
	accept();
}

void RecoverDialog::doRemove()
{
	QStringList filesToRemove;
	filesToRemove.clear();
	for (int a = 0; a < filesList->rowCount(); a++)
	{
		if (((QCheckBox*)(filesList->cellWidget(a, 0)))->isChecked())
		{
			QFile::remove(m_files[a]);
			filesToRemove.append(m_files[a]);
		}
	}
	for (int a = 0; a < filesToRemove.count(); a++)
	{
		m_files.removeAll(filesToRemove[a]);
	}
	updateFilesTable();
}

void RecoverDialog::updateFilesTable()
{
	filesList->clearContents();
	filesList->setRowCount(m_files.count());
	buttonRemove->setEnabled(false);
	buttonRecover->setEnabled(false);
	for (int a = 0; a < m_files.count(); a++)
	{
		QFileInfo fi(m_files[a]);
		QString name = fi.baseName();
		QStringList parts;
		parts.clear();
		QString fName = "";
		QString fDate = "";
		QString fType = "";
		if (name.contains("_emergency_"))
		{
			parts = name.split("_emergency_");
			fName = parts[0];
			fType = tr("Emergency");
			if (parts.count() > 1)
			{
				QStringList dateL = parts[1].split("_");
				if (dateL.count() > 4)
					fDate = dateL[0] + "." + dateL[1] + "." + dateL[2] + " " + dateL[3] + ":" + dateL[4];
			}
		}
		else if (name.contains("_autosave_"))
		{
			parts = name.split("_autosave_");
			fName = parts[0];
			fType = tr("Autosave");
			if (parts.count() > 1)
			{
				QStringList dateL = parts[1].split("_");
				if (dateL.count() > 4)
					fDate = dateL[0] + "." + dateL[1] + "." + dateL[2] + " " + dateL[3] + ":" + dateL[4];
			}
		}
		QCheckBox *cb = new QCheckBox(this);
		cb->setChecked(false);
		filesList->setCellWidget(a, 0, cb);
		connect(cb, SIGNAL(clicked()), this, SLOT(handleItemClick()));
		QTableWidgetItem *tW1 = new QTableWidgetItem(fName);
		tW1->setFlags(Qt::ItemIsEnabled);
		filesList->setItem(a, 1, tW1);
		QTableWidgetItem *tW2 = new QTableWidgetItem(fDate);
		tW2->setFlags(Qt::ItemIsEnabled);
		filesList->setItem(a, 2, tW2);
		QTableWidgetItem *tW3 = new QTableWidgetItem(fType);
		tW3->setFlags(Qt::ItemIsEnabled);
		filesList->setItem(a, 3, tW3);
	}
	filesList->setColumnWidth(0, 24);
	filesList->resizeColumnsToContents();
}
