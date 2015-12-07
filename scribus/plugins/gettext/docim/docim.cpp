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
#include "ui/scmessagebox.h"
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

DocIm::DocIm(const QString& fname, const QString& enc, bool textO, gtWriter *w) : QObject(), m_textBuffer(this), m_errorBuffer(this)
{
	m_filename = fname;
	m_encoding = enc;
	m_writer = w;
	m_textOnly = textO;
	m_failed = false;

	m_textBuffer.open(QIODevice::WriteOnly);
	m_errorBuffer.open(QIODevice::WriteOnly);

	m_proc = new QProcess();
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
	args  << QDir::toNativeSeparators(m_filename);

	//connect(proc, SIGNAL(readyReadStdout()), this, SLOT(slotReadOutput()));
	//connect(proc, SIGNAL(readyReadStderr()), this, SLOT(slotReadErr()));
	m_proc->start(exename, args);
	if (!m_proc->waitForStarted())
	{
		m_failed = true;
		return;
	}
	while (m_proc->waitForReadyRead())
	{
		usleep(5000);
	}

	while(!m_proc->atEnd() || m_proc->state()==QProcess::Running)
	{
		m_proc->setReadChannel(QProcess::StandardOutput);
		if ( m_proc->canReadLine() )
		{
			QByteArray bo = m_proc->readAllStandardOutput();
			if (bo.size() > 0)
				m_textBuffer.write(bo);
		}
		else
		{
			m_proc->setReadChannel(QProcess::StandardError);
			if ( m_proc->canReadLine() )
			{
				QByteArray be = m_proc->readAllStandardError();
				if (be.size() > 0)
					m_errorBuffer.write(be);
			}
			else
			{
				usleep(5000);
			}
		}
	}

	m_errorBuffer.close();
	m_textBuffer.close();

	if (m_proc->exitStatus() != QProcess::NormalExit)
	{
		m_failed = true;
		return;
	}

	write();
}

bool DocIm::isRunning()
{
	return m_proc->state() == QProcess::Running;
}

void DocIm::write()
{
	QTextCodec *codec = 0;

#if defined(Q_OS_WIN32)
	// #10258 : use UTF-8 whenever possible
	if (QFile::exists(ScPaths::instance().libDir() + "tools/antiword/UTF-8.txt"))
		codec = QTextCodec::codecForName("UTF-8");
#endif

	if (m_encoding.isEmpty() && !codec)
		codec = QTextCodec::codecForLocale();
	else if (!codec)
		codec = QTextCodec::codecForName(m_encoding.toLocal8Bit());

	if (m_failed)
	{
		QString error = codec->toUnicode( m_errorBuffer.data() ); 
		ScMessageBox::information(0, tr("Importing failed"),
		                         tr("Importing Word document failed \n%1").arg(error),
		                         QMessageBox::Ok);
		return;
	}

	QString text = codec->toUnicode( m_textBuffer.data() );
	m_writer->appendUnstyled(text);
}

DocIm::~DocIm()
{
	delete m_proc;
}


