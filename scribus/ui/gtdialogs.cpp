/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
 *   Copyright (C) 2004 by Riku Leino                                      *
 *   tsoots@gmail.com                                                      *
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

#include "commonstrings.h"
#include "gtdialogs.h"
#include "iconmanager.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "sccombobox.h"
#include "ui/gtfiledialog.h"

#include <QFileInfo>
#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QToolTip>
#include <QVBoxLayout>

extern QString DocDir;

/********* Class gtImporterDialog*******************************************************************/

gtImporterDialog::gtImporterDialog(const QString& fileName, const QStringList& importers, int currentSelection)
{
	QFileInfo fInfo(fileName);
	QString ext = fInfo.suffix();
	if ((ext.length() > 0) && !ext.startsWith("."))
		ext.prepend(".");

	setWindowTitle( tr("Choose the importer to use"));
	setWindowIcon(IconManager::instance()->loadIcon("AppIcon.png"));

	QBoxLayout* layout = new QVBoxLayout(this);

	QBoxLayout* llayout = new QHBoxLayout;
	llayout->setMargin(5);
	llayout->setSpacing(5);

	QString labelText;
	if (ext.length() > 0)
		labelText = tr("Choose the importer to use for %1 file:").arg(ext);
	else
		labelText = tr("Choose the importer to use:");
	QLabel* label = new QLabel(labelText, this);
	llayout->addWidget(label);
	layout->addLayout(llayout);

	QBoxLayout* ilayout = new QHBoxLayout;
	ilayout->setMargin(5);
	ilayout->setSpacing(5);
	importerCombo = new ScComboBox(this);
	importerCombo->setMinimumSize(QSize(150, 0));
	importerCombo->setToolTip( tr("Choose the importer to use"));
	importerCombo->addItems(importers);
	if (static_cast<int>(importers.count()) > currentSelection)
		importerCombo->setCurrentIndex(currentSelection);
	else
		importerCombo->setCurrentIndex(0);
	ilayout->addWidget(importerCombo);
	layout->addLayout(ilayout);

	QBoxLayout* dlayout = new QHBoxLayout;
	dlayout->setMargin(5);
	dlayout->setSpacing(5);
	rememberCheck = new QCheckBox( tr("Remember association"), this);
	rememberCheck->setChecked(false);
	rememberCheck->setToolTip( "<qt>" + tr("Remember the file extension - importer association and do not ask again to select an importer for files of this type.") + "</qt>" );
	dlayout->addStretch(10);
	dlayout->addWidget(rememberCheck);
	layout->addLayout(dlayout);

	QBoxLayout* blayout = new QHBoxLayout;
	blayout->setMargin(5);
	blayout->setSpacing(5);
	blayout->addStretch(10);
	okButton = new QPushButton( CommonStrings::tr_OK, this);
	blayout->addWidget(okButton);
	layout->addLayout(blayout);

	connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
}

bool gtImporterDialog::shouldRemember()
{
	return rememberCheck->isChecked();
}

QString gtImporterDialog::getImporter()
{
	return importerCombo->currentText();
}

gtImporterDialog::~gtImporterDialog()
{

}

/********* Class gtDialogs *************************************************************************/

gtDialogs::gtDialogs()
{
	m_fdia = NULL;
	m_fileName = "";
	m_encoding = "";
	m_importer = -1;
	m_prefs = PrefsManager::instance()->prefsFile->getContext("gtDialogs");
}

bool gtDialogs::runFileDialog(const QString& filters, const QStringList& importers)
{
	bool accepted = false;
	PrefsContext* dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
	QString dir = dirs->get("get_text", ".");
	
	m_fdia = new gtFileDialog(filters, importers, dir);
	if (m_fdia->exec() == QDialog::Accepted)
	{
		m_fileName = m_fdia->selectedFile();
		if (!m_fileName.isEmpty())
			accepted = true;
		m_encoding = m_fdia->encodingCombo->currentText();
//		if (encoding == "UTF-16")
//			encoding = "ISO-10646-UCS-2";
		m_importer = m_fdia->importerCombo->currentIndex() - 1;
		dirs->set("get_text", m_fileName.left(m_fileName.lastIndexOf("/")));
	}
	return accepted;
}

bool gtDialogs::runImporterDialog(const QString& fileName, const QStringList& importers)
{
	bool ok = false;
	bool shouldRemember = false;

	QFileInfo fileInfo(fileName);
	QString fileExtension = fileInfo.suffix();
	if (fileExtension.length() > 0 && !fileExtension.startsWith("."))
		fileExtension.prepend(".");
	if (fileExtension.isEmpty())
		fileExtension = ".no_extension";
	
	QString imp = m_prefs->get("remember"+ fileExtension, QString("false"));
	QString res = "";
	if (imp != "false")
	{
		res = imp;
		if (importers.contains(res))
			ok = true;
	}
	
	if (!ok)
	{
		int curSel = m_prefs->getInt("curSel", 0);
		int extensionSel = m_prefs->getInt(fileExtension, -1);
		if ((extensionSel > -1) && (extensionSel < static_cast<int>(importers.count())))
			curSel = extensionSel;
		else
			curSel = 0;
		gtImporterDialog* idia = new gtImporterDialog(fileName, importers, curSel);
		if (idia->exec())
		{
			res = idia->getImporter();
			shouldRemember = idia->shouldRemember();
			ok = true;
		}
		delete idia;
	}

	if (ok)
	{
		QString fileExtension;
		int importerIndex = importers.indexOf(res);
		if (importerIndex >= 0)
		{
			m_importer = importerIndex;
			m_prefs->set("curSel", static_cast<int>(importerIndex));
			if (!fileExtension.isEmpty())
			{
				m_prefs->set(fileExtension, static_cast<int>(importerIndex));
				if (shouldRemember)
					m_prefs->set("remember" + fileExtension, res);
			}
		}
	}
	return ok;
}

const QString& gtDialogs::getFileName()
{
	return m_fileName;
}

const QString& gtDialogs::getEncoding()
{
	return m_encoding;
}

int gtDialogs::getImporter()
{
	return m_importer;
}

bool gtDialogs::importTextOnly()
{
	bool ret = false;
	if (m_fdia)
		ret = m_fdia->textOnlyCheckBox->isChecked();
	return ret;
}

bool gtDialogs::prefixStyles()
{
	bool ret = false;
	if (m_fdia)
		ret = m_fdia->prefixStylesCheckBox->isChecked();
	return ret;
}

gtDialogs::~gtDialogs()
{
	delete m_fdia;
}
