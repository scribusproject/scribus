/***************************************************************************
 *   Riku Leino, tsoots@welho.com                                          *
 ***************************************************************************/
#ifndef NFTRCREADER_H
#define NFTRCREADER_H

#include <vector>
#include <utility>
#include <qobject.h>
#include <qdir.h>
#include <qxml.h>
#include <qstring.h>
#include <qstringlist.h>
#include "nfttemplate.h"

typedef std::pair<QString*,QString*> Pair;

class nftrcreader : public QXmlDefaultHandler
{
private:
	bool inSettings;
	bool inTemplate;
	bool inName;
	bool inFile;
	bool inTNail;
	bool inImg;
	bool inPSize;
	bool inColor;
	bool inDescr;
	bool inUsage;
	bool inAuthor;
	bool inEmail;
	QString currentDir;
	QString templateCategory;
	nfttemplate* tmpTemplate;
	std::vector<nfttemplate*> *templates;
	std::vector<Pair*> cats;
	QString getCategory(QString cat);
	void setupCategories();
public:
	bool startDocument();
	bool startElement(const QString&, const QString&, const QString &name, const QXmlAttributes &attrs);
	bool endElement(const QString&, const QString&, const QString &name);
	bool characters(const QString &ch);
	void setSourceDir(QString source);
	nftrcreader(std::vector<nfttemplate*> *tmplts,QString sourceDir);
	~nftrcreader();
};

#endif
