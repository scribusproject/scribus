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

	QList<QByteArray> codecNames = QTextCodec::availableCodecs();
	QStringList codecList;
	for (int a = 0; a < codecNames.count(); ++a)
		codecList.append(codecNames[a]);
	codecList.sort();
	encodingCombo->addItems(codecList);
	QString localEn = QTextCodec::codecForLocale()->name();
	int localIndex = encodingCombo->findText(localEn);
	if (localIndex >= 0)
		encodingCombo->setCurrentIndex(localIndex);
	else
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
