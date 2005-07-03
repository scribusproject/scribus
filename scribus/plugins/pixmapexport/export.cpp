/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "export.h"
#include "dialog.h"
#include "export.moc"
#include <qpixmap.h>
#include <qstring.h>
#include <qdir.h>
#include <qcursor.h>


QString Name()
{
	return QObject::tr("Save as &Image...");
}


int Type()
{
	return 3;
}

int ID()
{
	return 4;
}


void Run(QWidget *d, ScribusApp *plug)
{
	bool res;
	ExportBitmap *ex = new ExportBitmap(plug);
	ExportForm *dia = new ExportForm(d, ex->pageDPI, ex->quality, ex->bitmapType);

	// interval widgets handling
	QString tmp;
	dia->RangeVal->setText(tmp.setNum(plug->doc->ActPage->PageNr+1));
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
		plug->FProg->reset();
		if (dia->OnePageRadio->isChecked())
			res = ex->exportActual();
		else
		{
			if (dia->AllPagesRadio->isChecked())
				plug->parsePagesString("*", &pageNs, plug->doc->PageC);
			else
				plug->parsePagesString(dia->RangeVal->text(), &pageNs, plug->doc->PageC);
			res = ex->exportInterval(pageNs);
		}
		plug->FProg->reset();
		QApplication::restoreOverrideCursor();
		if (!res)
		{
			QMessageBox::warning(plug, QObject::tr("Save as Image"), QObject::tr("Error writting the output file(s)."));
			plug->FMess->setText(QObject::tr("Error writing the output file(s)."));
		}
		else
		{
			plug->FMess->setText(QObject::tr("Export successful."));
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
	return QDir::cleanDirPath(QDir::convertSeparators(exportDir + "/" + name + "-"+ number + "." + bitmapType.lower()));
}

ExportBitmap::~ExportBitmap()
{
}

bool ExportBitmap::exportPage(uint pageNr, bool single = TRUE)
{
	uint over = 0;
	QString fileName = getFileName(pageNr);

	if (!carrier->view->Pages.at(pageNr))
		return FALSE;

	/* a little magic here - I need to compute the "maxGr" value...
	 * We need to know the right size of the page for landscape,
	 * portrait and user defined sizes.
	 */
	double pixmapSize;
	(carrier->doc->PageH > carrier->doc->PageB)
			? pixmapSize = carrier->doc->PageH
			: pixmapSize = carrier->doc->PageB;
	QPixmap pixmap = carrier->view->PageToPixmap(pageNr, qRound(pixmapSize * enlargement * (pageDPI / 72.0) / 100.0));
	QImage im = pixmap.convertToImage();
	int dpm = qRound((100.0 / 2.54) * pageDPI);
	im.setDotsPerMeterY(dpm);
	im.setDotsPerMeterX(dpm);
	if (QFile::exists(fileName) && !overwrite)
	{
		QApplication::restoreOverrideCursor();
		/* Changed the following Code from the original QMessageBox::question to
		 * QMessageBox::warning to keep the Code compatible to Qt-3.1.x
	     * f.s 12.05.2004 */
		over = QMessageBox::warning(carrier,
				QObject::tr("File exists. Overwrite?"),
				fileName +"\n"+ QObject::tr("exists already. Overwrite?"),
				QObject::tr("No"),
				QObject::tr("Yes"),
				// hack for multiple overwritting (petr)
				(single==TRUE) ? QString::null : QObject::tr("Yes all"),
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
	return exportPage(carrier->doc->ActPage->PageNr, TRUE);
}

bool ExportBitmap::exportInterval(std::vector<int> &pageNs)
{
	bool res;
	carrier->FProg->setTotalSteps(pageNs.size());
	for (uint a = 0; a < pageNs.size(); ++a)
	{
		carrier->FProg->setProgress(a);
		res = exportPage(pageNs[a]-1, FALSE);
		if (!res)
			return FALSE;
	}
	return TRUE;
}

