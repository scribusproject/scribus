/***************************************************************************
 *   Riku Leino, tsoots@welho.com                                          *
 ***************************************************************************/
#ifndef NFTTEMPLATE_H
#define NFTTEMPLATE_H

#include <qstring.h>

class nfttemplate {
public:
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
	nfttemplate(const QString &tmplType);
	~nfttemplate();
};

#endif // NFTTEMPLATE_H
