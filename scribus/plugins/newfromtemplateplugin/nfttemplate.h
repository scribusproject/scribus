/***************************************************************************
 *   Riku Leino, tsoots@welho.com                                          *
 ***************************************************************************/
#ifndef NFTTEMPLATE_H
#define NFTTEMPLATE_H

#include <qstring.h>
#include <qfile.h>
#include <qtextstream.h>

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
};

#endif // NFTTEMPLATE_H
