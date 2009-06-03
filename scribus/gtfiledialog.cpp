/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QToolTip>
#include <QTextCodec>
#include "gtfiledialog.h"

#include "prefsmanager.h"
#include "prefscontext.h"
#include "prefsfile.h"

gtFileDialog::gtFileDialog(const QString& filters, const QStringList& importers, const QString& wdir)
	: QDialog()
{
	setupUi(this);

	fileWidget->setDirectory(wdir);
	fileWidget->setFileMode(QFileDialog::ExistingFile);
	fileWidget->setFilter(filters);

	importerCombo->setToolTip( tr("Choose the importer to use"));
	importerCombo->addItem( tr("Automatic"));
	importerCombo->addItems(importers);

	textOnlyCheckBox->setToolTip( tr("Import text without any formatting"));

	QString tmp_txc[] = {"ISO 8859-1", "ISO 8859-2", "ISO 8859-3", "ISO 8859-4", "ISO 8859-5", "ISO 8859-6",
					   "ISO 8859-7", "ISO 8859-8", "ISO 8859-9", "ISO 8859-10", "ISO 8859-13", "ISO 8859-14",
					   "ISO 8859-15", "UTF-8", "UTF-16", "KOI8-R", "KOI8-U", "CP1250", "CP1251", "CP1252", "CP1253",
					   "CP1254", "CP1255", "CP1256", "CP1257", "Apple Roman"};
	size_t array = sizeof(tmp_txc) / sizeof(*tmp_txc);
	for (uint a = 0; a < array; ++a)
		encodingCombo->addItem(tmp_txc[a]);
	QString localEn = QTextCodec::codecForLocale()->name();
	if (localEn == "ISO-10646-UCS-2")
		localEn = "UTF-16";
	bool hasIt = false;
	for (int cc = 0; cc < encodingCombo->count(); ++cc)
	{
		if (encodingCombo->itemText(cc) == localEn)
		{
			encodingCombo->setCurrentIndex(cc);
			hasIt = true;
			break;
		}
	}
	if (!hasIt)
	{
		encodingCombo->addItem(localEn);
		encodingCombo->setCurrentIndex(encodingCombo->count()-1);
	}

	loadSettings();

	connect(fileWidget, SIGNAL(accepted()), this, SLOT(accept()));
	connect(fileWidget, SIGNAL(rejected()), this, SLOT(reject()));
}

QString gtFileDialog::selectedFile()
{
	return fileWidget->selectedFile();
}

void gtFileDialog::accept()
{
	saveSettings();
	QDialog::accept();
}

void gtFileDialog::loadSettings(void)
{
	PrefsContext* context = PrefsManager::instance()->prefsFile->getContext("textimport_dialog");
	if (context->contains("filter"))
	{
		QString filter = context->get("filter");
		QStringList filters = fileWidget->filters();
		if (!filter.isEmpty() && filters.contains(filter))
			fileWidget->selectNameFilter(filter);
	}
	if (context->contains("importer"))
	{
		QString importer = context->get("importer");
		int index = importerCombo->findText(importer);
		if (index >= 0)
			importerCombo->setCurrentIndex(index);
	}
	if (context->contains("encoding"))
	{
		QString encoding = context->get("encoding");
		int index = encodingCombo->findText(encoding);
		if (index >= 0)
			encodingCombo->setCurrentIndex(index);
	}
	if (context->contains("textonly"))
	{
		bool textOnly = context->getBool("textonly");
		textOnlyCheckBox->setChecked(textOnly);
	}
}

void gtFileDialog::saveSettings(void)
{
	PrefsContext* context = PrefsManager::instance()->prefsFile->getContext("textimport_dialog");
	context->set("filter"  , fileWidget->selectedFilter());
	context->set("importer", importerCombo->currentText());
	context->set("encoding", encodingCombo->currentText());
	context->set("textonly", textOnlyCheckBox->isChecked());
}

gtFileDialog::~gtFileDialog()
{
}
