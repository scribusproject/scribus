/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/


#include "barcodegeneratorrenderthread.h"
#include "scpaths.h"
#include "scribuscore.h"
#include "util_ghostscript.h"

#include <QDir>
#include <QTextStream>
#include <QThread>


BarcodeGeneratorRenderThread::BarcodeGeneratorRenderThread(QObject *parent) : QThread(parent)
{
	restart = false;
	abort = false;
}

BarcodeGeneratorRenderThread::~BarcodeGeneratorRenderThread()
{
	mutex.lock();
	abort = true;
	condition.wakeOne();
	mutex.unlock();
	wait();
}

void BarcodeGeneratorRenderThread::render(QString psCommand)
{
	QMutexLocker locker(&mutex);

	this->psCommand = psCommand;

	if (!isRunning()) {
		start(LowPriority);
	} else {
		restart = true;
		condition.wakeOne();
	}
}

void BarcodeGeneratorRenderThread::run()
{

	QString pngFile = QDir::toNativeSeparators(ScPaths::tempFileDir() + "bcode.png");
	QString psFile = QDir::toNativeSeparators(ScPaths::tempFileDir() + "bcode.ps");
	QString fileStdErr = QDir::toNativeSeparators(ScPaths::tempFileDir() + "bcode.err");
	QString fileStdOut = QDir::toNativeSeparators(ScPaths::tempFileDir() + "bcode.out");
	int dpi=72;

	forever {
		mutex.lock();
		QString psCommand = this->psCommand;
		mutex.unlock();

		QFile f(psFile);
		f.open(QIODevice::WriteOnly);
		QTextStream ts(&f);
		ts << psCommand;
		f.close();

		QStringList gargs;
		// limit the area only for preview. EPS importer bounds the box itself.
		gargs.append("-dDEVICEWIDTHPOINTS=440");
		gargs.append("-dDEVICEHEIGHTPOINTS=150");
		gargs.append( QString("-r%1").arg(dpi) );
		gargs.append( QString("-sOutputFile=%1").arg(pngFile) );
		gargs.append( psFile );
		QFile::remove(pngFile);
		int gs = callGS(gargs, QString(), fileStdErr, fileStdOut);
		bool retval = gs==0 && QFile::exists(pngFile);   // GS returns 0 for BWIPP-handled errors

		QString errorMsg;
		if (!retval)
		{
			errorMsg="Barcode incomplete";
			if (QFile::exists(fileStdErr))  // Display BWIPP handled error message
			{
				QFile f(fileStdErr);
				if (f.open(QIODevice::ReadOnly))
				{
					QTextStream ts(&f);
					QString err = ts.readAll();
					f.close();
					QRegExp rx("[\\r\\n]+BWIPP ERROR: [^\\s]+ (.*)[\\r\\n$]+");
					rx.setMinimal(true);
					if (rx.indexIn(err) != -1)
						errorMsg=rx.cap(1).trimmed();
				}
			}
		}

		if (abort)
			return;

		if (!restart)
			emit renderedImage(errorMsg);

		// Go to sleep unless restarting
		mutex.lock();
		if (!restart)
			condition.wait(&mutex);
		restart = false;
		mutex.unlock();
	}

}

