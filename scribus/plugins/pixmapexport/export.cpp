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

bool PixmapExportPlugin::run(QString target)
{
	Q_ASSERT(target.isEmpty());
	bool res;
	ExportBitmap *ex = new ExportBitmap();
	ExportForm *dia = new ExportForm(ScMW, ex->pageDPI, ex->quality, ex->bitmapType);

	// interval widgets handling
	QString tmp;
	dia->RangeVal->setText(tmp.setNum(ScMW->doc->currentPageNumber()+1));
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
		ScMW->mainWindowProgressBar->reset();
		if (dia->OnePageRadio->isChecked())
			res = ex->exportActual();
		else
		{
			if (dia->AllPagesRadio->isChecked())
				parsePagesString("*", &pageNs, ScMW->doc->pageCount);
			else
				parsePagesString(dia->RangeVal->text(), &pageNs, ScMW->doc->pageCount);
			res = ex->exportInterval(pageNs);
		}
		ScMW->mainWindowProgressBar->reset();
		QApplication::restoreOverrideCursor();
		if (!res)
		{
			QMessageBox::warning(ScMW, QObject::tr("Save as Image"), QObject::tr("Error writing the output file(s)."));
			ScMW->mainWindowStatusLabel->setText(QObject::tr("Error writing the output file(s)."));
		}
		else
		{
			ScMW->mainWindowStatusLabel->setText(QObject::tr("Export successful."));
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
	enlargement = 100;
	exportDir = QDir::currentDirPath();
	bitmapType = QString("PNG");
	overwrite = false;
}

QString ExportBitmap::getFileName(uint pageNr)
{
	return QDir::cleanDirPath(QDir::convertSeparators(exportDir + "/" + getFileNameByPage(pageNr, bitmapType.lower())));
}

ExportBitmap::~ExportBitmap()
{
}

bool ExportBitmap::exportPage(uint pageNr, bool single = true)
{
	uint over = 0;
	QString fileName = getFileName(pageNr);

	if (!ScMW->doc->Pages->at(pageNr))
		return false;

	/* a little magic here - I need to compute the "maxGr" value...
	* We need to know the right size of the page for landscape,
	* portrait and user defined sizes.
	*/
	double pixmapSize;
	(ScMW->doc->pageHeight > ScMW->doc->pageWidth)
			? pixmapSize = ScMW->doc->pageHeight
			: pixmapSize = ScMW->doc->pageWidth;
	QImage im = ScMW->view->PageToPixmap(pageNr, qRound(pixmapSize * enlargement * (pageDPI / 72.0) / 100.0));
	int dpm = qRound(100.0 / 2.54 * pageDPI);
	im.setDotsPerMeterY(dpm);
	im.setDotsPerMeterX(dpm);
	if (QFile::exists(fileName) && !overwrite)
	{
		QApplication::restoreOverrideCursor();
/* Changed the following Code from the original QMessageBox::question to QMessageBox::warning
	 to keep the Code compatible to Qt-3.1.x
	 f.s 12.05.2004 */
		over = ScMessageBox::warning(ScMW,
				QObject::tr("File exists. Overwrite?"),
				fileName +"\n"+ QObject::tr("exists already. Overwrite?"),
				QObject::tr("Yes"),
				QObject::tr("No"),
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

bool ExportBitmap::exportActual()
{
	return exportPage(ScMW->doc->currentPageNumber(), true);
}

bool ExportBitmap::exportInterval(std::vector<int> &pageNs)
{
	bool res;
	ScMW->mainWindowProgressBar->setTotalSteps(pageNs.size());
	for (uint a = 0; a < pageNs.size(); ++a)
	{
		ScMW->mainWindowProgressBar->setProgress(a);
		res = exportPage(pageNs[a]-1, false);
		if (!res)
			return false;
	}
	return true;
}
