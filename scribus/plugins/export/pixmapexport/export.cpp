/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "export.h"
#include "dialog.h"

#include <QCursor>
#include <QDir>
#include <QMessageBox>
#include <QPixmap>
#include <QString>
#include <QSharedPointer>

#include "scribus.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "scraction.h"
#include "ui/scmwmenumanager.h"
#include "util.h"
#include "commonstrings.h"
#include "scpaths.h"

int scribusexportpixmap_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* scribusexportpixmap_getPlugin()
{
	PixmapExportPlugin* plug = new PixmapExportPlugin();
	Q_CHECK_PTR(plug);
	return plug;
}

void scribusexportpixmap_freePlugin(ScPlugin* plugin)
{
	PixmapExportPlugin* plug = qobject_cast<PixmapExportPlugin*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}

PixmapExportPlugin::PixmapExportPlugin()
{
	// Set action info in languageChange, so we only have to do
	// it in one place. This includes registering file formats.
	languageChange();
}

PixmapExportPlugin::~PixmapExportPlugin()
{
};

void PixmapExportPlugin::languageChange()
{
	// Note that we leave the unused members unset. They'll be initialised
	// with their default ctors during construction.
	// Action name
	m_actionInfo.name = "ExportAsImage";
	// Action text for menu, including accel
	m_actionInfo.text = tr("Save as &Image...");
	// Keyboard shortcut
	m_actionInfo.keySequence = "CTRL+SHIFT+E";
	// Menu
	m_actionInfo.menu = "FileExport";
	m_actionInfo.enabledOnStartup = false;
	m_actionInfo.needsNumObjects = -1;
}

QString PixmapExportPlugin::fullTrName() const
{
	return tr("Export As Image");
}

const ScActionPlugin::AboutData* PixmapExportPlugin::getAboutData() const
{
	AboutData* about = new AboutData;
	Q_CHECK_PTR(about);
	about->authors = QString::fromUtf8("Petr Van\xc4\x9bk <petr@scribus.info>");
	about->shortDescription = tr("Export As Image");
	about->description = tr("Exports selected pages as bitmap images.");
	// about->version
	// about->releaseDate
	// about->copyright
	about->license = "GPL";
	return about;
}

void PixmapExportPlugin::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

bool PixmapExportPlugin::run(ScribusDoc* doc, const QString& target)
{
	Q_ASSERT(target.isEmpty());
	Q_ASSERT(!doc->masterPageMode());
	QSharedPointer<ExportBitmap> ex( new ExportBitmap() );
	QSharedPointer<ExportForm>  dia( new ExportForm(nullptr, doc, ex->pageDPI, ex->quality, ex->bitmapType) );

	// interval widgets handling
	QString tmp;
	dia->rangeVal->setText(tmp.setNum(doc->currentPageNumber()+1));
	dia->prefixLineEdit->setText(doc->documentFileName());
	// main "loop"
	if (dia->exec() != QDialog::Accepted)
		return true;

	std::vector<int> pageNs;
	ex->pageDPI = dia->DPIBox->value();
	ex->enlargement = dia->enlargementBox->value();
	ex->quality     = dia->qualityBox->value();
	ex->exportDir   = QDir::fromNativeSeparators(dia->outputDirectory->text());
	ex->bitmapType  = dia->bitmapType->currentText();
	ex->filenamePrefix = dia->prefixLineEdit->text();

	// check availability of the destination
	QFileInfo fi(ex->exportDir);
	if (!fi.isDir())
	{
		ScMessageBox::warning(doc->scMW(), tr("Save as Image"),
			                    tr("The target location %1 must be an existing directory").arg(ex->exportDir));
		return false;
	}
	if (!fi.isWritable())
	{
		ScMessageBox::warning(doc->scMW(), tr("Save as Image"),
			                    tr("The target location %1 must be writable").arg(ex->exportDir));
		return false;
	}

	QApplication::changeOverrideCursor(QCursor(Qt::WaitCursor));
	doc->scMW()->mainWindowProgressBar->reset();
	bool res;
	if (dia->onePageRadio->isChecked())
		res = ex->exportCurrent(doc, !dia->noBackground->isChecked());
	else
	{
		if (dia->allPagesRadio->isChecked())
			parsePagesString("*", &pageNs, doc->DocPages.count());
		else
			parsePagesString(dia->rangeVal->text(), &pageNs, doc->DocPages.count());
		res = ex->exportInterval(doc, pageNs, !dia->noBackground->isChecked());
	}
	doc->scMW()->mainWindowProgressBar->reset();
	QApplication::changeOverrideCursor(Qt::ArrowCursor);
//		QApplication::restoreOverrideCursor();
	if (res)
		doc->scMW()->setStatusBarInfoText( tr("Export successful"));

	return true;
}


ExportBitmap::ExportBitmap()
{
	pageDPI = 72;
	quality = -1;
	enlargement = 100.0;
	exportDir = QDir::currentPath();
	bitmapType = QString("png");
	overwrite = false;
}

QString ExportBitmap::getFileName(ScribusDoc* doc, uint pageNr)
{
	return QDir::cleanPath(QDir::toNativeSeparators(exportDir + "/" + getFileNameByPage(doc, pageNr, bitmapType.toLower(), filenamePrefix)));
}

ExportBitmap::~ExportBitmap()
{
}

bool ExportBitmap::exportPage(ScribusDoc* doc, uint pageNr, bool background, bool single = true)
{
	uint over   = 0;
	bool saved = false, doFileSave = true;
	QString fileName(getFileName(doc, pageNr));

	if (!doc->Pages->at(pageNr))
		return false;
	ScPage* page = doc->Pages->at(pageNr);

	/* a little magic here - I need to compute the "maxGr" value...
	* We need to know the right size of the page for landscape,
	* portrait and user defined sizes.
	*/
	double pixmapSize = (page->height() > page->width()) ? page->height() : page->width();
	PageToPixmapFlags flags;
	if (background)
		flags |= Pixmap_DrawBackground;
	QImage im(doc->view()->PageToPixmap(pageNr, qRound(pixmapSize * enlargement * (pageDPI / 72.0) / 100.0), flags));
	if (im.isNull())
	{
		ScMessageBox::warning(doc->scMW(), tr("Save as Image"), tr("Insufficient memory for this image size."));
		doc->scMW()->setStatusBarInfoText( tr("Insufficient memory for this image size."));
		return false;
	}
	int dpm = qRound(100.0 / 2.54 * pageDPI);
	im.setDotsPerMeterY(dpm);
	im.setDotsPerMeterX(dpm);
	if (QFile::exists(fileName) && !overwrite)
	{
		doFileSave = false;
		QString fn = QDir::toNativeSeparators(fileName);
//		QApplication::restoreOverrideCursor();
		QApplication::changeOverrideCursor(Qt::ArrowCursor);
		over = ScMessageBox::question(doc->scMW(), tr("File exists. Overwrite?"),
				fn +"\n"+ tr("exists already. Overwrite?"),
				// hack for multiple overwriting (petr) 
				(single) ? QMessageBox::Yes | QMessageBox::No : QMessageBox::Yes | QMessageBox::No | QMessageBox::YesToAll,
				QMessageBox::NoButton,	// GUI default
				QMessageBox::YesToAll);	// batch default
		QApplication::changeOverrideCursor(QCursor(Qt::WaitCursor));
		if (over == QMessageBox::Yes || over == QMessageBox::YesToAll)
			doFileSave = true;
		if (over == QMessageBox::YesToAll)
			overwrite = true;
	}
	if (doFileSave)
		saved = im.save(fileName, bitmapType.toLocal8Bit().constData(), quality);
	if (!saved && doFileSave)
	{
		ScMessageBox::warning(doc->scMW(), tr("Save as Image"), tr("Error writing the output file(s)."));
		doc->scMW()->setStatusBarInfoText( tr("Error writing the output file(s)."));
	}
	return saved;
}

bool ExportBitmap::exportCurrent(ScribusDoc* doc,  bool background)
{
	return exportPage(doc, doc->currentPageNumber(), background, true);
}

bool ExportBitmap::exportInterval(ScribusDoc* doc, std::vector<int> &pageNs, bool background)
{
	doc->scMW()->mainWindowProgressBar->setMaximum(pageNs.size());
	for (uint a = 0; a < pageNs.size(); ++a)
	{
		doc->scMW()->mainWindowProgressBar->setValue(a);
		if (!exportPage(doc, pageNs[a]-1, background, false))
			return false;
	}
	return true;
}
