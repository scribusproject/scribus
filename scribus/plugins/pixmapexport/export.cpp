/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "export.h"
#include "dialog.h"
#include "export.moc"
#include <qpixmap.h>
#include <qstring.h>
#include <qdir.h>
#include <qcursor.h>

#include "scmessagebox.h"
#include "scribus.h"
#include "scraction.h"
#include "menumanager.h"
#include "util.h"
#include "commonstrings.h"


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
	PixmapExportPlugin* plug = dynamic_cast<PixmapExportPlugin*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}

PixmapExportPlugin::PixmapExportPlugin() : ScActionPlugin()
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
	m_actionInfo.enabledOnStartup = true;
}

const QString PixmapExportPlugin::fullTrName() const
{
	return QObject::tr("Export As Image");
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

bool PixmapExportPlugin::run(ScribusDoc* doc, QString target)
{
	Q_ASSERT(target.isEmpty());
	Q_ASSERT(!doc->masterPageMode());
	bool res;
	ExportBitmap *ex = new ExportBitmap();
	ExportForm *dia = new ExportForm(doc->scMW(), ex->pageDPI, ex->quality, ex->bitmapType, doc->pageWidth, doc->pageHeight, doc->DocPages.count());

	// interval widgets handling
	QString tmp;
	dia->RangeVal->setText(tmp.setNum(doc->currentPageNumber()+1));
	// main "loop"
	if (dia->exec()==QDialog::Accepted)
	{
		QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
		std::vector<int> pageNs;
		ex->pageDPI = dia->DPIBox->value();
		ex->enlargement = dia->EnlargementBox->value();
		ex->quality = dia->QualityBox->value();
		ex->exportDir = dia->OutputDirectory->text();
		ex->bitmapType = dia->bitmapType;
		doc->scMW()->mainWindowProgressBar->reset();
		if (dia->OnePageRadio->isChecked())
			res = ex->exportCurrent(doc);
		else
		{
			if (dia->AllPagesRadio->isChecked())
				parsePagesString("*", &pageNs, doc->DocPages.count());
			else
				parsePagesString(dia->RangeVal->text(), &pageNs, doc->DocPages.count());
			res = ex->exportInterval(doc, pageNs);
		}
		doc->scMW()->mainWindowProgressBar->reset();
		QApplication::restoreOverrideCursor();
		if (!res)
		{
			QMessageBox::warning(doc->scMW(), QObject::tr("Save as Image"), QObject::tr("Error writing the output file(s)."));
			doc->scMW()->setStatusBarInfoText(QObject::tr("Error writing the output file(s)."));
		}
		else
		{
			doc->scMW()->setStatusBarInfoText(QObject::tr("Export successful."));
		}
	} // if accepted
	// clean the trash
	delete ex;
	delete dia;
	return true;
}


ExportBitmap::ExportBitmap()
{
	pageDPI = 72;
	quality = 100;
	enlargement = 100.0;
	exportDir = QDir::currentDirPath();
	bitmapType = QString("PNG");
	overwrite = false;
}

QString ExportBitmap::getFileName(ScribusDoc* doc, uint pageNr)
{
	return QDir::cleanDirPath(QDir::convertSeparators(exportDir + "/" + getFileNameByPage(doc, pageNr, bitmapType.lower())));
}

ExportBitmap::~ExportBitmap()
{
}

bool ExportBitmap::exportPage(ScribusDoc* doc, uint pageNr, bool single = true)
{
	uint over = 0;
	QString fileName = getFileName(doc, pageNr);

	if (!doc->Pages->at(pageNr))
		return false;

	/* a little magic here - I need to compute the "maxGr" value...
	* We need to know the right size of the page for landscape,
	* portrait and user defined sizes.
	*/
	double pixmapSize;
	pixmapSize = (doc->pageHeight > doc->pageWidth) ? doc->pageHeight : doc->pageWidth;
	QImage im = doc->view()->PageToPixmap(pageNr, qRound(pixmapSize * enlargement * (pageDPI / 72.0) / 100.0), false);
	int dpm = qRound(100.0 / 2.54 * pageDPI);
	im.setDotsPerMeterY(dpm);
	im.setDotsPerMeterX(dpm);
	if (QFile::exists(fileName) && !overwrite)
	{
		QApplication::restoreOverrideCursor();
/* Changed the following Code from the original QMessageBox::question to QMessageBox::warning
	 to keep the Code compatible to Qt-3.1.x
	 f.s 12.05.2004 */
		over = ScMessageBox::warning(doc->scMW(),
				QObject::tr("File exists. Overwrite?"),
				fileName +"\n"+ QObject::tr("exists already. Overwrite?"),
				CommonStrings::trYes,
				CommonStrings::trNo,
				// hack for multiple overwritting (petr)
				(single==true) ? QString::null : QObject::tr("Yes all"),
				0, 0);
		QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
		if (over == 0)
			return im.save(fileName, bitmapType, quality);
		if (over == 2)
			overwrite = true;
	}
	return im.save(fileName, bitmapType, quality);
}

bool ExportBitmap::exportCurrent(ScribusDoc* doc)
{
	return exportPage(doc, doc->currentPageNumber(), true);
}

bool ExportBitmap::exportInterval(ScribusDoc* doc, std::vector<int> &pageNs)
{
	bool res;
	doc->scMW()->mainWindowProgressBar->setTotalSteps(pageNs.size());
	for (uint a = 0; a < pageNs.size(); ++a)
	{
		doc->scMW()->mainWindowProgressBar->setProgress(a);
		res = exportPage(doc, pageNs[a]-1, false);
		if (!res)
			return false;
	}
	return true;
}
