/***************************************************************************
 *   Riku Leino, riku.leino@gmail.com                                          *
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
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
	bool isValid();
};

#endif // NFTTEMPLATE_H
