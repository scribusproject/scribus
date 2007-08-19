/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QToolTip>
#include <QTextCodec>
#include "gtfiledialog.h"


gtFileDialog::gtFileDialog(const QString& filters, const QStringList& importers, const QString& wdir)
	: QDialog()
{
	setupUi(this);

	fileWidget->setDir(wdir);
	fileWidget->setMode(QFileDialog::ExistingFile);
	fileWidget->setFilter(filters);

	importerCombo->setToolTip( tr("Choose the importer to use"));
	importerCombo->insertItem( tr("Automatic"));
	importerCombo->insertStringList(importers);

	textOnlyCheckBox->setToolTip( tr("Import text without any formatting"));

	QString tmp_txc[] = {"ISO 8859-1", "ISO 8859-2", "ISO 8859-3", "ISO 8859-4", "ISO 8859-5", "ISO 8859-6",
					   "ISO 8859-7", "ISO 8859-8", "ISO 8859-9", "ISO 8859-10", "ISO 8859-13", "ISO 8859-14",
					   "ISO 8859-15", "UTF-8", "UTF-16", "KOI8-R", "KOI8-U", "CP1250", "CP1251", "CP1252", "CP1253",
					   "CP1254", "CP1255", "CP1256", "CP1257", "Apple Roman"};
	size_t array = sizeof(tmp_txc) / sizeof(*tmp_txc);
	for (uint a = 0; a < array; ++a)
		encodingCombo->insertItem(tmp_txc[a]);
	QString localEn = QTextCodec::codecForLocale()->name();
	if (localEn == "ISO-10646-UCS-2")
		localEn = "UTF-16";
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

	connect(fileWidget, SIGNAL(accepted()), this, SLOT(accept()));
}

QString gtFileDialog::selectedFile()
{
	return fileWidget->selectedFile();
}

gtFileDialog::~gtFileDialog()
{
}
