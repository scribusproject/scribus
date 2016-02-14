/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QCompleter>
#include <QDebug>
#include <QDirModel>
#include <QFileDialog>
#include <QImageWriter>
#include <QToolTip>

#include "commonstrings.h"
#include "dialog.h"
#include "iconmanager.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "scribusdoc.h"
#include "ui/createrange.h"
#include "ui/scrspinbox.h"
#include "usertaskstructs.h"
#include "util.h"

ExportForm::ExportForm(QWidget* parent, ScribusDoc* doc, int size, int quality, QString type)
	: QDialog(parent, 0), m_doc(doc), m_PageCount(doc->DocPages.count())
{
	setupUi(this);
	setModal(true);
	prefs = PrefsManager::instance()->prefsFile->getPluginContext("pixmapexport");

	QDirModel * dirModel = new QDirModel(this);
	dirModel->setFilter(QDir::AllDirs);
	outputDirectory->setCompleter(new QCompleter(dirModel, this));

	outputDirectory->setText( QDir::toNativeSeparators(prefs->get("wdir", QDir::currentPath())) );
	QList<QByteArray> imgs = QImageWriter::supportedImageFormats();
	for (int i = 0; i < imgs.count(); i++)
		bitmapType->addItem(imgs[i]);
	qualityBox->setValue(quality);
	qualityBox->setWrapping(true);

	DPIBox->setValue(size);
	enlargementBox->setValue(size);
	onePageRadio->setChecked( true );
	pageNrButton->setIcon(IconManager::instance()->loadIcon("ellipsis.png"));
	rangeVal->setEnabled(false);
	pageNrButton->setEnabled(false);
	noBackground->setChecked(false);

	languageChange();
	readConfig();
	computeSize();

	connect(outputDirectoryButton, SIGNAL(clicked()), this, SLOT(OutputDirectoryButton_pressed()));
	connect(intervalPagesRadio, SIGNAL(clicked()), this, SLOT(IntervalPagesRadio_stateChanged()));
	connect(allPagesRadio, SIGNAL(clicked()), this, SLOT(AllPagesRadio_stateChanged()));
	connect(onePageRadio, SIGNAL(clicked()), this, SLOT(OnePageRadio_stateChanged()));
	connect(enlargementBox, SIGNAL(valueChanged(int)), this, SLOT(computeSize()));
	connect(DPIBox, SIGNAL(valueChanged(int)), this, SLOT(computeSize()));
	connect(pageNrButton, SIGNAL(clicked()), this, SLOT(createPageNumberRange()));
}

void ExportForm::computeSize()
{
	double pixmapSize;
	double pw = (onePageRadio->isChecked() && m_doc->currentPage()) ? m_doc->currentPage()->width() : m_doc->pageWidth();
	double ph = (onePageRadio->isChecked() && m_doc->currentPage()) ? m_doc->currentPage()->height() : m_doc->pageHeight();
	(ph > pw) ? pixmapSize = ph : pixmapSize = pw;
	int maxGr = qRound(pixmapSize * enlargementBox->value() * (DPIBox->value() / 72.0) / 100.0);
	double sc = qMin(maxGr / pw, maxGr / ph);
	imageSizeLabel->setText(QString("%1 x %2 px").arg(qRound(pw * sc)).arg(qRound(ph * sc)));
}

void ExportForm::OutputDirectoryButton_pressed()
{
	QString lastDir = prefs->get("wdir", ".");
	QString dirName = QFileDialog::getExistingDirectory(this, tr("Choose an Export Directory"), lastDir);
	if (dirName.length()>0)
	{
		dirName = QDir::toNativeSeparators(dirName);
		outputDirectory->setText(dirName);
		prefs->set("wdir", dirName);
	}
}

void ExportForm::IntervalPagesRadio_stateChanged()
{
	rangeVal->setEnabled(true);
	pageNrButton->setEnabled(true);
	computeSize();
}

void ExportForm::AllPagesRadio_stateChanged()
{
	rangeVal->setEnabled(false);
	pageNrButton->setEnabled(false);
	computeSize();
}

void ExportForm::OnePageRadio_stateChanged()
{
	rangeVal->setEnabled(false);
	pageNrButton->setEnabled(false);
	computeSize();
}

ExportForm::~ExportForm()
{
	writeConfig();
}

void ExportForm::languageChange()
{
	intervalPagesRadio->setToolTip( tr( "Export a range of pages" ) );
	rangeVal->setToolTip( tr( "Insert a comma separated list of tokens where\na token can be * for all the pages, 1-5 for\na range of pages or a single page number" ) );
	pageNrButton->setToolTip( tr( "Create a range of pages" ) );
	allPagesRadio->setToolTip( tr( "Export all pages" ) );
	onePageRadio->setToolTip( tr( "Export only the current page" ) );
	DPIBox->setToolTip( tr( "Resolution of the Images\nUse 72 dpi for Images intended for the Screen" ) );
	enlargementBox->setToolTip( tr( "Size of the images. 100% for no changes, 200% for two times larger etc." ));
	qualityBox->setToolTip( tr( "The compression ratio of your images - 100% is no compression, 0% highest compression. If in doubt, use 'Automatic'" ) );
	qualityBox->setSpecialValueText(tr("Automatic"));
	bitmapType->setToolTip( tr( "Available export formats" ) );
	outputDirectory->setToolTip( tr( "The output directory - the place to store your images.\nName of the export file will be 'documentname-pagenumber.filetype'" ) );
	outputDirectoryButton->setToolTip( tr( "Change the output directory" ) );
	noBackground->setToolTip( tr("Don't export Page Background"));
}


void ExportForm::readConfig()
{
	DPIBox->setValue(prefs->getUInt("DPIBox", 72));
	enlargementBox->setValue(prefs->getInt("EnlargementBox", 100));
	qualityBox->setValue(prefs->getUInt("QualityBox", -1));
	int exportPageValue = prefs->getUInt("ButtonGroup1", 0);
	switch (exportPageValue)
	{
		case 0: onePageRadio->setChecked(true);
			break;
		case 1: allPagesRadio->setChecked(true);
			break;
		default: intervalPagesRadio->setChecked(true);
			break;
	}
	rangeVal->setEnabled(exportPageValue==2);
	pageNrButton->setEnabled(exportPageValue==2);
	bitmapType->setCurrentText("png");
	rangeVal->setText(prefs->get("RangeVal", ""));
}

void ExportForm::writeConfig()
{
	prefs->set("DPIBox", DPIBox->value());
	prefs->set("EnlargementBox", enlargementBox->value());
	prefs->set("QualityBox", qualityBox->value());
	int exportPageValue;
	if (onePageRadio->isChecked())
		exportPageValue = 0;
	else if (allPagesRadio->isChecked())
		exportPageValue = 1;
	else
		exportPageValue = 2;
	prefs->set("ButtonGroup1", exportPageValue);
	prefs->set("BitmapType", bitmapType->currentIndex());
	prefs->set("RangeVal", rangeVal->text());
}

void ExportForm::createPageNumberRange( )
{
	CreateRange cr(rangeVal->text(), m_PageCount, this);
	if (cr.exec())
	{
		CreateRangeData crData;
		cr.getCreateRangeData(crData);
		rangeVal->setText(crData.pageRange);
	}
}
