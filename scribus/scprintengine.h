/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCPRINTENGINE_H
#define SCPRINTENGINE_H

#include <QObject>

class  ScribusDoc;
struct PrintOptions;

class ScPrintEngine : public QObject
{
	Q_OBJECT

protected:

	bool    m_abort;
	QString m_errorMessage;

	ScPrintEngine() { m_abort = false; }

public:
	virtual ~ScPrintEngine() {}
	virtual bool print(ScribusDoc& doc, PrintOptions& options) = 0;
	virtual const QString& errorMessage(void) { return m_errorMessage; }

public slots:
	void cancelRequested(void) { m_abort = true; }
};
#endif
