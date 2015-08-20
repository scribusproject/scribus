/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef BARCODEGENERATORRENDERTHREAD_H
#define BARCODEGENERATORRENDERTHREAD_H

#include <QString>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>

class BarcodeGeneratorRenderThread : public QThread
{
	Q_OBJECT

public:
	BarcodeGeneratorRenderThread(QObject *parent = 0);
	~BarcodeGeneratorRenderThread();
	void render(QString);

signals:
	void renderedImage(QString);

protected:
	void run();

private:
	QMutex mutex;
	QWaitCondition condition;
	QString psCommand;
	bool restart;
	bool abort;

};

#endif
