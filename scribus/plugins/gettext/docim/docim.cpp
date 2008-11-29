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
	test->start(exename);
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

DocIm::DocIm(const QString& fname, const QString& enc, bool textO, gtWriter *w) : QObject()
{
	filename = fname;
	encoding = enc;
	writer = w;
	textOnly = textO;
	failed = false;
	QTextCodec *codec;
	if (encoding.isEmpty())
		codec = QTextCodec::codecForLocale();
	else
		codec = QTextCodec::codecForName(encoding.toLocal8Bit());
	QTextCodec::setCodecForCStrings(codec);
	text = "";
	error = "";
	proc = new QProcess();
	QString exename("antiword");
#if defined(_WIN32)
	exename = ScPaths::instance().libDir() + "tools/antiword/antiword.exe";
	proc->setWorkingDirectory( ScPaths::instance().libDir() + "tools/antiword/" ); 
#endif
	QStringList args;
	args << "-t" << "-w 0" << filename;
	//connect(proc, SIGNAL(readyReadStdout()), this, SLOT(slotReadOutput()));
	//connect(proc, SIGNAL(readyReadStderr()), this, SLOT(slotReadErr()));
#if defined(_WIN32)
	QStringList envVar;
	QString homeDir =  QDir::convertSeparators(ScPaths::instance().libDir() + "tools");
	envVar.append( QString("HOME=%1").arg(homeDir) );
	proc->setEnvironment(envVar);
#endif
	proc->start(exename, args);
	if (!proc->waitForStarted())
	{
		failed = true;
		return;
	}

	while(proc->state()==QProcess::Running)
	{
		proc->setReadChannel(QProcess::StandardOutput);
		if ( proc->canReadLine() )
		{
			QByteArray bo = proc->readAllStandardOutput();
			if (bo.size() > 0)
				text += QString(bo);
		}
		else
		{
			proc->setReadChannel(QProcess::StandardError);
			if ( proc->canReadLine() )
			{
				QByteArray be = proc->readAllStandardError();
				if (be.size() > 0)
					error += QString(be);
			}
			else
			{
				usleep(5000);
			}
		}
	}

	if (proc->exitStatus()==QProcess::NormalExit)
	{
		toUnicode();
		write();
	} 
	else
		failed = true;
}

bool DocIm::isRunning()
{
	return proc->state()==QProcess::Running;
}

void DocIm::write()
{
	if (!failed)
		writer->appendUnstyled(text);
	else
		QMessageBox::information(0, tr("Importing failed"),
		                         tr("Importing Word document failed \n%1").arg(error),
		                         QMessageBox::Ok);
}

void DocIm::toUnicode()
{
	QTextCodec *codec;
	if (encoding.isEmpty())
		codec = QTextCodec::codecForLocale();
	else
		codec = QTextCodec::codecForName(encoding.toLocal8Bit());
	QString dec = codec->toUnicode( text.toLocal8Bit() );
	text = dec;
}

DocIm::~DocIm()
{
	delete proc;
}


