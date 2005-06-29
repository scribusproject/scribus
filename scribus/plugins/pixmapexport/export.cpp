#include "export.h"
#include "dialog.h"
#include "export.moc"
#include <qpixmap.h>
#include <qstring.h>
#include <qdir.h>
#include <qcursor.h>

#include "scraction.h"
#include "menumanager.h"
#include "pluginmanager.h"


QString name()
{
	return QObject::tr("Save as &Image...");
}


PluginManager::PluginType type()
{
	return PluginManager::Standard;
}

int ID()
{
	return 4;
}

QString actionName()
{
	return "ExportAsImage";
}

QString actionKeySequence()
{
	return "CTRL+SHIFT+E";
}

QString actionMenu()
{
	return "FileExport";
}

QString actionMenuAfterName()
{
	return "";
}

bool actionEnabledOnStartup()
{
	return true;
}

void run(QWidget *d, ScribusApp *plug)
{
	bool res;
	ExportBitmap *ex = new ExportBitmap(plug);
	ExportForm *dia = new ExportForm(d, ex->pageDPI, ex->quality, ex->bitmapType);

	// interval widgets handling
	QString tmp;
	dia->RangeVal->setText(tmp.setNum(plug->doc->currentPage->PageNr+1));
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
		plug->mainWindowProgressBar->reset();
		if (dia->OnePageRadio->isChecked())
			res = ex->exportActual();
		else
		{
			if (dia->AllPagesRadio->isChecked())
				plug->parsePagesString("*", &pageNs, plug->doc->pageCount);
			else
				plug->parsePagesString(dia->RangeVal->text(), &pageNs, plug->doc->pageCount);
			res = ex->exportInterval(pageNs);
		}
		plug->mainWindowProgressBar->reset();
		QApplication::restoreOverrideCursor();
		if (!res)
		{
			QMessageBox::warning(plug, QObject::tr("Save as Image"), QObject::tr("Error writing the output file(s)."));
			plug->mainWindowStatusLabel->setText(QObject::tr("Error writing the output file(s)."));
		}
		else
		{
			plug->mainWindowStatusLabel->setText(QObject::tr("Export successful."));
		}
	} // if accepted
	// clean the trash
	delete ex;
	delete dia;
}


ExportBitmap::ExportBitmap(ScribusApp *plug)
{
	carrier = plug;
	pageDPI = 72;
	quality = 100;
	enlargement = 100;
	exportDir = QDir::currentDirPath();
	bitmapType = QString("PNG");
	overwrite = FALSE;
}

QString ExportBitmap::getFileName(uint pageNr)
{
	QFileInfo path(carrier->doc->DocName);
	QString name = path.baseName(); // needed tp fix the "/home/user/blah.sla"
	QString number;
	number = number.setNum(pageNr + carrier->doc->FirstPnum);
	return QDir::convertSeparators(exportDir + "/" + name + "-"+ number + "." + bitmapType.lower());
}

ExportBitmap::~ExportBitmap()
{
}

bool ExportBitmap::exportPage(uint pageNr, bool single = TRUE)
{
	uint over = 0;
	QString fileName = getFileName(pageNr);

	if (!carrier->doc->Pages.at(pageNr))
		return FALSE;

	QImage im = carrier->view->PageToPixmap(pageNr, qRound(carrier->doc->pageHeight * enlargement * (pageDPI / 72.0) / 100.0));
	int dpm = qRound(100.0 / 2.54 * pageDPI);
	im.setDotsPerMeterY(dpm);
	im.setDotsPerMeterX(dpm);
	if (QFile::exists(fileName) && !overwrite)
	{
		QApplication::restoreOverrideCursor();
/* Changed the following Code from the original QMessageBox::question to QMessageBox::warning
	 to keep the Code compatible to Qt-3.1.x
	 f.s 12.05.2004 */
		over = QMessageBox::warning(carrier,
				QObject::tr("File exists. Overwrite?"),
				fileName +"\n"+ QObject::tr("exists already. Overwrite?"),
				QObject::tr("No"),
				QObject::tr("Yes"),
				// hack for multiple overwritting (petr)
				(single==TRUE) ? 0 : QObject::tr("Yes all"),
				0, 0);
		QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
		if (over == 1)
			return im.save(fileName, bitmapType, quality);
		if (over == 2)
			overwrite = TRUE;
	}
	return im.save(fileName, bitmapType, quality);
}

bool ExportBitmap::exportActual()
{
	return exportPage(carrier->doc->currentPage->PageNr, TRUE);
}

bool ExportBitmap::exportInterval(std::vector<int> &pageNs)
{
	bool res;
	carrier->mainWindowProgressBar->setTotalSteps(pageNs.size());
	for (uint a = 0; a < pageNs.size(); ++a)
	{
		carrier->mainWindowProgressBar->setProgress(a);
		res = exportPage(pageNs[a]-1, FALSE);
		if (!res)
			return FALSE;
	}
	return TRUE;
}
