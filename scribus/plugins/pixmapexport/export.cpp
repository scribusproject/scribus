#include "export.h"
#include "dialog.h"
#include "export.moc"
#include <qpixmap.h>
#include <qstring.h>
#include <qdir.h>
#include <qcursor.h>


QString Name()
{
	return QObject::tr("Save as Image...");
}


int Type()
{
	return 3;
}


void Run(QWidget *d, ScribusApp *plug)
{
	bool res;
	ExportBitmap *ex = new ExportBitmap(plug);
	ExportForm *dia = new ExportForm(d, ex->pageSize, ex->quality, ex->bitmapType);

	// interval widgets handling
	dia->ToBox->setMinValue(1);
	dia->ToBox->setMaxValue(plug->doc->PageC);
	dia->ToBox->setValue(plug->doc->PageC);
	dia->FromBox->setMinValue(1);
	dia->FromBox->setMaxValue(plug->doc->PageC);
	dia->FromBox->setValue(plug->doc->ActPage->PageNr+1);
	// main "loop"
	if (dia->exec()==QDialog::Accepted)
	{
		QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
		ex->pageSize = dia->SizeBox->value();
		ex->quality = dia->QualityBox->value();
		ex->exportDir = dia->OutputDirectory->text();
		ex->bitmapType = dia->bitmapType;
		plug->FProg->reset();
		switch (dia->ButtonGroup1->id(dia->ButtonGroup1->selected()))
		{
			case 0: res = ex->exportActual();
					break;
			case 1: res = ex->exportAll();
					break;
			case 2: res = ex->exportInterval(dia->FromBox->value(), dia->ToBox->value());
					break;
		} // switch
		plug->FProg->reset();
		QApplication::restoreOverrideCursor();
		if (!res)
		{
			QMessageBox::warning(plug, QObject::tr("Save as Image"),
				QObject::tr("Error writting the output file(s)."));
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
	pageSize = static_cast<int>(carrier->doc->PageH);
	quality = 100;
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

	if (!carrier->view->Pages.at(pageNr))
		return FALSE;

	QPixmap pixmap = carrier->view->PageToPixmap(pageNr, pageSize);
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
			return pixmap.save(fileName, bitmapType);
		if (over == 2)
			overwrite = TRUE;
	}

	return pixmap.save(fileName, bitmapType);
}


bool ExportBitmap::exportActual()
{
	return exportPage(carrier->doc->ActPage->PageNr, TRUE);
}


bool ExportBitmap::exportAll()
{
	return exportInterval(0, carrier->view->Pages.count());
}


bool ExportBitmap::exportInterval(uint from, uint to)
{
	bool res;

	carrier->FProg->setTotalSteps(to - from+1);
	for (uint pageNo = from-1; pageNo < to; pageNo++)
	{
		carrier->FProg->setProgress(pageNo - from+1);
		res = exportPage(pageNo, FALSE);
		if (!res)
			return FALSE;
	}
	return TRUE;
}

