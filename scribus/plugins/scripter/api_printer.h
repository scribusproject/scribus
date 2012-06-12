/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef API_PRINTER_H_
#define API_PRINTER_H_

#include <QObject>
#include <QtDebug>
#include <QApplication>

#include "scripterimpl.h"

class PrinterAPI : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString printer READ getPrinter WRITE setPrinter);
	Q_PROPERTY(QString file READ getFile WRITE setFile);
	Q_PROPERTY(QString cmd READ getCmd WRITE setCmd);
	Q_PROPERTY(QString separation READ getSeparation WRITE setSeparation);
	Q_PROPERTY(QList<QVariant> pages READ getPages WRITE setPages);
	Q_PROPERTY(bool color READ isColor WRITE setColor);
	Q_PROPERTY(bool useICC READ getUseICC WRITE setUseICC);
	Q_PROPERTY(int psLevel READ getPsLevel WRITE setPsLevel);
	Q_PROPERTY(bool mph READ getMph WRITE setMph);
	Q_PROPERTY(bool mpv READ getMpv WRITE setMpv);
	Q_PROPERTY(bool ucr READ getUcr WRITE setUcr);
public:
	PrinterAPI();
	virtual ~PrinterAPI();

public slots:
	void doPrint();
	QList<QVariant> allPrinters();

private:
	QString getPrinter();
	void setPrinter(QString name);
	
	QString getFile();
	void setFile(QString name);
	
	QString getCmd();
	void setCmd(QString name);
	
	QString getSeparation();
	void setSeparation(QString name);
	
	QList<QVariant> getPages();
	void setPages(QList<QVariant> list);
	
	bool isColor();
	void setColor(bool value);
	
	bool getUseICC();
	void setUseICC(bool value);
	
	int getPsLevel();
	void setPsLevel(int value);
	
	bool getMph();
	void setMph(bool value);

	bool getMpv();
	void setMpv(bool value);

	bool getUcr();
	void setUcr(bool value);
	
	QString printer;
	QString file;
	QString cmd;
	QList<int> pages;
	int copies;
	QString separation;
	bool color;
	bool useICC;
	int pslevel;
	bool mph;
	bool mpv;
	bool ucr;
};

#endif /*API_PRINTER_H_*/
