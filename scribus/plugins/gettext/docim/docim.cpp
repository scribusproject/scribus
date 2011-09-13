/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "docim.h"
#include "gtwriter.h"
#include "scpaths.h"
#include "scribusstructs.h"
#include <QObject>
#include <QByteArray>
#include <QMessageBox>
#include <QProcess>
#include <QString>
#include <QStringList>
#include <QTextCodec>

#if defined(_WIN32) && !defined(usleep)
#include <windows.h>
#define usleep(t) Sleep((t > 1000) ? (t / 1000) : 1)
#endif

bool hasAntiword()
{
	static bool searched = false, found = false;
	if (searched) // searched already in this run
		return found;

	QProcess *test = new QProcess();
	QString exename("antiword");
#if defined(_WIN32)
	exename = ScPaths::instance().libDir() + "tools/antiword/antiword.exe";
#endif
	test->start(exename, QStringList());
	if (test->waitForStarted())
	{
		found = true;
		test->terminate();
		usleep(5000);
		test->kill();	
	}
	delete test;
	searched = true;
	return found;
}

QString FileFormatName()
{
	if (hasAntiword())
    	return QObject::tr("Word Documents");
	else
		return QString::null;
}

QStringList FileExtensions()
{
	if (hasAntiword())
    	return QStringList("doc");
	else
		return QStringList();
}

void GetText(QString filename, QString encoding, bool textOnly, gtWriter *writer)
{
	if (!hasAntiword())
		return;

	DocIm *dim = new DocIm(filename, encoding, textOnly, writer);
	while (dim->isRunning())
	{
		usleep(5000);
	}
	delete dim;
}

DocIm::DocIm(const QString& fname, const QString& enc, bool textO, gtWriter *w) : QObject(), textBuffer(this), errorBuffer(this)
{
	filename = fname;
	encoding = enc;
	writer = w;
	textOnly = textO;
	failed = false;

	textBuffer.open(QIODevice::WriteOnly);
	errorBuffer.open(QIODevice::WriteOnly);

	proc = new QProcess();
	QString exename("antiword");
#if defined(Q_OS_WIN32)
	exename = ScPaths::instance().libDir() + "tools/antiword/antiword.exe";
	QString homeDir =  QDir::toNativeSeparators(ScPaths::instance().libDir() + "tools");
	proc->setWorkingDirectory( ScPaths::instance().libDir() + "tools/antiword/" );
	proc->setEnvironment( QStringList() << QString("HOME=%1").arg(homeDir));
#endif

	QStringList args;
	args << "-t" << "-w 0";
#if defined(Q_OS_WIN32)
	// #10258 : use UTF-8 whenever possible
	if (QFile::exists(ScPaths::instance().libDir() + "tools/antiword/UTF-8.txt"))
		args << "-m" << "UTF-8.txt";
#endif
	args  << QDir::toNativeSeparators(filename);

	//connect(proc, SIGNAL(readyReadStdout()), this, SLOT(slotReadOutput()));
	//connect(proc, SIGNAL(readyReadStderr()), this, SLOT(slotReadErr()));
	proc->start(exename, args);
	if (!proc->waitForStarted())
	{
		failed = true;
		return;
	}
	while (proc->waitForReadyRead())
	{
		usleep(5000);
	}

	while(!proc->atEnd() || proc->state()==QProcess::Running)
	{
		proc->setReadChannel(QProcess::StandardOutput);
		if ( proc->canReadLine() )
		{
			QByteArray bo = proc->readAllStandardOutput();
			if (bo.size() > 0)
				textBuffer.write(bo);
		}
		else
		{
			proc->setReadChannel(QProcess::StandardError);
			if ( proc->canReadLine() )
			{
				QByteArray be = proc->readAllStandardError();
				if (be.size() > 0)
					errorBuffer.write(be);
			}
			else
			{
				usleep(5000);
			}
		}
	}

	errorBuffer.close();
	textBuffer.close();

	if (proc->exitStatus() != QProcess::NormalExit)
	{
		failed = true;
		return;
	}

	write();
}

bool DocIm::isRunning()
{
	return proc->state() == QProcess::Running;
}

void DocIm::write()
{
	QTextCodec *codec = 0;

#if defined(Q_OS_WIN32)
	// #10258 : use UTF-8 whenever possible
	if (QFile::exists(ScPaths::instance().libDir() + "tools/antiword/UTF-8.txt"))
		codec = QTextCodec::codecForName("UTF-8");
#endif

	if (encoding.isEmpty() && !codec)
		codec = QTextCodec::codecForLocale();
	else if (!codec)
		codec = QTextCodec::codecForName(encoding.toLocal8Bit());

	if (failed)
	{
		QString error = codec->toUnicode( errorBuffer.data() ); 
		QMessageBox::information(0, tr("Importing failed"),
		                         tr("Importing Word document failed \n%1").arg(error),
		                         QMessageBox::Ok);
		return;
	}

	QString text = codec->toUnicode( textBuffer.data() );
	writer->appendUnstyled(text);
}

DocIm::~DocIm()
{
	delete proc;
}


