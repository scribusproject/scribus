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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "gtdialogs.h"
#include "gtdialogs.moc"
#include "prefsmanager.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "sccombobox.h"
#include <qlabel.h>
#include "commonstrings.h"

extern QPixmap loadIcon(QString nam);
extern QString DocDir;

/********* Class gtFileDialog ************************************************************************/

gtFileDialog::gtFileDialog(const QString& filters, const QStringList& importers, const QString& wdir) : 
               QFileDialog(QString::null, filters, 0, 0, true)
{
// 	setIcon(loadIcon("AppIcon.png"));
	setCaption("Open");
	dir = QDir(wdir);
	setDir(dir);
	setMode(QFileDialog::ExistingFile);
	createWidgets(importers);
}

void gtFileDialog::createWidgets(const QStringList& importers)
{
	importerFrame = new QFrame(this);
	importerLayout = new QHBoxLayout(importerFrame);
	importerLayout->setSpacing(10);
	importerLayout->setMargin(0);

	importerCombo = new ScComboBox(0, importerFrame, "importerCombo");
	importerCombo->setMinimumSize(QSize(150, 0));
	QToolTip::add(importerCombo, tr("Choose the importer to use"));
	importerCombo->insertItem( tr("Automatic"));
	importerCombo->insertStringList(importers);
	importerLayout->addWidget(importerCombo);

	textOnlyCheckBox = new QCheckBox(importerFrame, "textOnlyCB");
	textOnlyCheckBox->setText( tr("Import Text Only"));
	QToolTip::add(textOnlyCheckBox, tr("Import text without any formatting"));
// 	                                   "\nNotice that not all importers provide this feature."));
	importerLayout->addWidget(textOnlyCheckBox);

	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	importerLayout->addItem(spacer);

	addWidgets(new QLabel( tr("Importer:"), this), importerFrame, 0);

	encodingFrame = new QFrame(this);
	encodingLayout = new QHBoxLayout(encodingFrame);
	encodingLayout->setSpacing(10);
	encodingLayout->setMargin(0);

	encodingCombo = new ScComboBox(true, encodingFrame, "encodingCombo");
	encodingCombo->setEditable(false);
	QString tmp_txc[] = {"ISO 8859-1", "ISO 8859-2", "ISO 8859-3", "ISO 8859-4", "ISO 8859-5", "ISO 8859-6",
					   "ISO 8859-7", "ISO 8859-8", "ISO 8859-9", "ISO 8859-10", "ISO 8859-13", "ISO 8859-14",
					   "ISO 8859-15", "utf8", "KOI8-R", "KOI8-U", "CP1250", "CP1251", "CP1252", "CP1253",
					   "CP1254", "CP1255", "CP1256", "CP1257", "Apple Roman"};
	size_t array = sizeof(tmp_txc) / sizeof(*tmp_txc);
	for (uint a = 0; a < array; ++a)
		encodingCombo->insertItem(tmp_txc[a]);
	QString localEn = QTextCodec::codecForLocale()->name();
	bool hasIt = false;
	for (int cc = 0; cc < encodingCombo->count(); ++cc)
	{
		if (encodingCombo->text(cc) == localEn)
		{
			encodingCombo->setCurrentItem(cc);
			hasIt = true;
			break;
		}
	}
	if (!hasIt)
	{
		encodingCombo->insertItem(localEn);
		encodingCombo->setCurrentItem(encodingCombo->count()-1);
	}
	encodingCombo->setMinimumSize(QSize(150, 0));
	encodingLayout->addWidget(encodingCombo);
	QSpacerItem* spacer2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	encodingLayout->addItem( spacer2 );
	addWidgets(new QLabel( tr("Encoding:"), this), encodingFrame, 0);

	HomeB = new QToolButton(this);
	HomeB->setIconSet(loadIcon("gohome.png"));
	HomeB->setAutoRaise(true);
// 	HomeB->setTextLabel( tr("Moves to your Document Directory.\nThis can be set in the Preferences."));
	connect(HomeB, SIGNAL(clicked()), this, SLOT(slotHome()));
	addToolButton(HomeB);
}

void gtFileDialog::slotHome()
{
	setDir(QDir(DocDir));
}

gtFileDialog::~gtFileDialog()
{

}

/********* Class gtImporterDialog*******************************************************************/

gtImporterDialog::gtImporterDialog(const QStringList& importers, int currentSelection)
{
	setCaption( tr("Choose the importer to use"));
	setIcon(loadIcon("AppIcon.png"));

	QBoxLayout* layout = new QVBoxLayout(this);

	QBoxLayout* llayout = new QHBoxLayout(0, 5, 5, "llayout");
	QLabel* label = new QLabel( tr("Choose the importer to use"), this, "label");
	llayout->addWidget(label);
	layout->addLayout(llayout);

	QBoxLayout* ilayout = new QHBoxLayout(0, 5, 5, "dlayout2");
	importerCombo = new ScComboBox(0, this, "importerCombo2");
	importerCombo->setMinimumSize(QSize(150, 0));
	QToolTip::add(importerCombo, tr("Choose the importer to use"));
	importerCombo->insertStringList(importers);
	if (static_cast<int>(importers.count()) > currentSelection)
		importerCombo->setCurrentItem(currentSelection);
	else
		importerCombo->setCurrentItem(0);
	ilayout->addWidget(importerCombo);
	layout->addLayout(ilayout);

	QBoxLayout* dlayout = new QHBoxLayout(0, 5, 5, "dlayout2");
	rememberCheck = new QCheckBox( tr("Remember association"), this, "rememberCheck");
	rememberCheck->setChecked(false);
	QToolTip::add(rememberCheck, "<qt>" + tr("Remember the file extension - importer association and do not ask again to select an importer for files of this type.") + "</qt>" );
	dlayout->addStretch(10);
	dlayout->addWidget(rememberCheck);
	layout->addLayout(dlayout);

	QBoxLayout* blayout = new QHBoxLayout(0, 5, 5, "blayout2");
	blayout->addStretch(10);
	okButton = new QPushButton( CommonStrings::tr_OK, this, "okButton2");
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
	fdia = NULL;
	fileName = "";
	encoding = "";
	importer = -1;
	prefs = PrefsManager::instance()->prefsFile->getContext("gtDialogs");
	pwd = QDir::currentDirPath();
}

bool gtDialogs::runFileDialog(const QString& filters, const QStringList& importers)
{
	bool accepted = false;
	PrefsContext* dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
	QString dir = dirs->get("get_text", ".");
	fdia = new gtFileDialog(filters, importers, dir);
	
	if (fdia->exec() == QDialog::Accepted)
	{
		fileName = fdia->selectedFile();
		if (!fileName.isEmpty())
			accepted = true;
		encoding = fdia->encodingCombo->currentText();
		importer = fdia->importerCombo->currentItem() - 1;
		dirs->set("get_text", fileName.left(fileName.findRev("/")));
	}
	QDir::setCurrent(pwd);
	return accepted;
}

bool gtDialogs::runImporterDialog(const QStringList& importers)
{
	int curSel = prefs->getInt("curSel", 0);
	QString extension = "";
	QString shortName = fileName.right(fileName.length() - fileName.findRev("/") - 1);
	if (shortName.find(".") == -1)
		extension = ".no_extension";
	else
		extension = fileName.right(fileName.length() - fileName.findRev("."));
	int extensionSel = prefs->getInt(extension, -1);
	QString imp = prefs->get("remember"+extension, QString("false"));
	QString res = "";
	bool shouldRemember = false;
	bool ok = false;
	if (imp != "false")
	{
		res = imp;
		if (importers.contains(res) > 0)
			ok = true;
	}
	
	if (!ok)
	{
		if ((extensionSel > -1) && (extensionSel < static_cast<int>(importers.count())))
			curSel = extensionSel;
		else
			curSel = 0;
		gtImporterDialog* idia = new gtImporterDialog(importers, curSel);
		if (idia->exec())
		{
			res = idia->getImporter();
			shouldRemember = idia->shouldRemember();
			delete idia;
			ok = true;
		}
	}

	if (ok)
	{
		QString fileExtension = "";
		for (uint i = 0; i < importers.count(); ++i)
		{
			if (importers[i] == res)
			{
				importer = i;
				prefs->set("curSel", static_cast<int>(i));
				if (fileName.find(".") != -1)
				{
					if (shortName.find(".") == -1)
						fileExtension = ".no_extension";
					else
						fileExtension = fileName.right(fileName.length() - fileName.findRev("."));
					if (!fileExtension.isEmpty())
					{
						prefs->set(fileExtension, static_cast<int>(i));
						if (shouldRemember)
							prefs->set("remember"+fileExtension, res);
					}
				}
				break;
			}
		}
	}
	return ok;
}

const QString& gtDialogs::getFileName()
{
	return fileName;
}

const QString& gtDialogs::getEncoding()
{
	return encoding;
}

int gtDialogs::getImporter()
{
	return importer;
}

bool gtDialogs::importTextOnly()
{
	bool ret = false;
	if (fdia)
		ret = fdia->textOnlyCheckBox->isChecked();
	return ret;
}

gtDialogs::~gtDialogs()
{
	delete fdia;
}
