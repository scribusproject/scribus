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

#include <QDir>
#include <QMap>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QXmlStreamReader>

#include "nfttemplate.h"

class nftrcreader
{
public:
	nftrcreader(std::vector<nfttemplate*> *tmplts, QString sourceDir);

	bool parse(QString filePath);

	void setSourceDir(QString source);
	void setSourceFile(QString sourceFile);

private:
	QString currentDir;
	QString currentFile;

	std::vector<nfttemplate*> *templates;
	QMap<QString, QString> cats;

	bool parseTemplate(QXmlStreamReader& reader, nfttemplate* nftTemplate);

	QString getCategory(QString cat);
	void setupCategories();
};

#endif
