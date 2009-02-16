/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
 *   Riku Leino, tsoots@gmail.com                                          *
 ***************************************************************************/
#ifndef NFTRCREADER_H
#define NFTRCREADER_H

#include <vector>
#include <utility>
#include <QObject>
#include <QDir>
#include <QMap>
#include <QString>
#include <QStringList>
#include <QXmlDefaultHandler>
#include "nfttemplate.h"

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
	bool inScribusVersion;
	bool inDate;
	bool inAuthor;
	bool inEmail;
	QString currentDir;
	QString currentFile;
	QString templateCategory;
	nfttemplate* tmpTemplate;
	std::vector<nfttemplate*> *templates;
	QMap<QString, QString> cats;
	QString getCategory(QString cat);
	void setupCategories();
public:
	nftrcreader(std::vector<nfttemplate*> *tmplts,QString sourceDir);
	bool startDocument();
	bool startElement(const QString&, const QString&, const QString &name, const QXmlAttributes &attrs);
	bool endElement(const QString&, const QString&, const QString &name);
	bool characters(const QString &ch);
	void setSourceDir(QString source);
	void setSourceFile(QString sourceFile);
};

#endif
