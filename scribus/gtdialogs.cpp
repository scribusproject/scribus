/***************************************************************************
 *   Copyright (C) 2004 by Riku Leino                                      *
 *   tsoots@welho.com                                                      *
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

extern QPixmap loadIcon(QString nam);
extern QString DocDir;

/********* Class gtFileDialog ************************************************************************/

gtFileDialog::gtFileDialog(const QString& filters, const QStringList& importers) : 
               QFileDialog(QString::null, filters, 0, 0, true)
{
// 	setIcon(loadIcon("AppIcon.png"));
	setCaption("Open");
	dir = QDir();
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

	importerCombo = new QComboBox(0, importerFrame, "importerCombo");
	importerCombo->setMinimumSize(QSize(150, 0));
	QToolTip::add(importerCombo, tr("Choose the importer to use"));
	importerCombo->insertItem(tr("Automatic"));
	importerCombo->insertStringList(importers);
	importerLayout->addWidget(importerCombo);

	textOnlyCheckBox = new QCheckBox(importerFrame, "textOnlyCB");
	textOnlyCheckBox->setText(tr("Get text only"));
	QToolTip::add(textOnlyCheckBox, tr("Import text without any formatting"));
// 	                                   "\nNotice that not all importers provide this feature."));
	importerLayout->addWidget(textOnlyCheckBox);

	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	importerLayout->addItem(spacer);

	addWidgets(new QLabel(tr("Importer:"), this), importerFrame, 0);

	encodingFrame = new QFrame(this);
	encodingLayout = new QHBoxLayout(encodingFrame);
	encodingLayout->setSpacing(10);
	encodingLayout->setMargin(0);

	encodingCombo = new QComboBox(true, encodingFrame, "encodingCombo");
	encodingCombo->setEditable(false);
	char *tmp_txc[] = {"ISO 8859-1", "ISO 8859-2", "ISO 8859-3", "ISO 8859-4", "ISO 8859-5", "ISO 8859-6",
					   "ISO 8859-7", "ISO 8859-8", "ISO 8859-9", "ISO 8859-10", "ISO 8859-13", "ISO 8859-14",
					   "ISO 8859-15", "utf8", "KOI8-R", "KOI8-U", "CP1250", "CP1251", "CP1252", "CP1253",
					   "CP1254", "CP1255", "CP1256", "CP1257"};
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
	addWidgets(new QLabel(tr("Encoding:"), this), encodingFrame, 0);

	HomeB = new QToolButton(this);
	HomeB->setIconSet(loadIcon("gohome.png"));
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

/********* Class gtDialogs *************************************************************************/

gtDialogs::gtDialogs()
{
	fdia = NULL;
	fileName = "";
	encoding = "";
	importer = -1;
}

bool gtDialogs::runFileDialog(const QString& filters, const QStringList& importers)
{
	bool accepted = false;
	fdia = new gtFileDialog(filters, importers);
	if (fdia->exec() == QDialog::Accepted)
	{
		fileName = fdia->selectedFile();
		if (fileName != "")
			accepted = true;
		encoding = fdia->encodingCombo->currentText();
		importer = fdia->importerCombo->currentItem() - 1;
	}
	return accepted;
}

bool gtDialogs::runImporterDialog(const QStringList& importers)
{
	bool ok;
	QString res = QInputDialog::getItem(QObject::tr("Importer"), 
	                                    QObject::tr("Choose the importer to use"), 
	                                    importers, 0, false, &ok);
	if (ok)
	{
		for (uint i = 0; i < importers.count(); ++i)
		{
			if (importers[i] == res)
			{
				importer = i;
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
