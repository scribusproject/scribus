/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
 *   Riku Leino, tsoots@gmail.com                                          *
 ***************************************************************************/
#ifndef NFTTEMPLATE_H
#define NFTTEMPLATE_H

#include <QString>
#include <QFile>
#include <QTextStream>

class nfttemplate {
public:
	nfttemplate(QFile* tmplXmlFile, const QString &tmplType);
	~nfttemplate();
	QFile* tmplXml;
	QString enCategory;
	QString templateCategory;
	QString name;
	QString file;
	QString tnail;
	QString img;
	QString psize;
	QString color;
	QString descr;
	QString usage;
	QString scribusVersion;
	QString date;
	QString author;
	QString email;
	bool isWritable;
	bool isDeleted;
	void remove();
	bool canWrite();
	bool isValid();
};

#endif // NFTTEMPLATE_H
