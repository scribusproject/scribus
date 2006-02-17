/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "docim.h"
#include "docim.moc"
#include "gtwriter.h"
#include "scribusstructs.h"
#include <qobject.h>
#include <qcstring.h>
#include <qmessagebox.h>
#include <qprocess.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qtextcodec.h>

bool hasAntiword()
{
	static bool searched = false, found = false;
	if (searched) // searched already in this run
		return found;

	QProcess *test = new QProcess();
	test->addArgument("antiword");
	if (test->start())
	{
		found = true;
		test->tryTerminate();
#ifndef _WIN32
		usleep(5000);
#else
		Sleep(5);
#endif
		test->kill();
		delete test;
	}
	searched = true;
	return found;
}

QString FileFormatName()
{
	if (hasAntiword())
    	return QObject::tr("Word Documents");
	else
		return QString();
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
#ifndef _WIN32
		usleep(5000);
#else
		Sleep(5);
#endif
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
		codec = QTextCodec::codecForName(encoding);
	QTextCodec::setCodecForCStrings(codec);
	text = "";
	error = "";
	proc = new QProcess();
	proc->addArgument("antiword");
	proc->addArgument("-t");
	proc->addArgument("-w 0");
	proc->addArgument(filename);
	connect(proc, SIGNAL(readyReadStdout()), this, SLOT(slotReadOutput()));
	connect(proc, SIGNAL(readyReadStderr()), this, SLOT(slotReadErr()));
	connect(proc, SIGNAL(processExited()), this, SLOT(slotLaunchFinished()));
	if (!proc->start()) {
		failed = true;
	}
}

bool DocIm::isRunning()
{
	return proc->isRunning();
}

void DocIm::write()
{
	if (!failed)
		writer->append(text);
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
		codec = QTextCodec::codecForName(encoding);
	QString dec = codec->toUnicode( text );
	text = dec;
}

void DocIm::slotReadOutput()
{
	QByteArray bb = proc->readStdout();
	if (bb)
		text += QString(bb);
}

void DocIm::slotReadErr()
{
	QByteArray bb = proc->readStderr();
	if (bb)
		error += QString(bb);
}

void DocIm::slotLaunchFinished()
{
	if (proc->normalExit()) {
		toUnicode();
		write();
	} else
		failed = true; // some info to the user too
}

DocIm::~DocIm()
{
	delete proc;
}


