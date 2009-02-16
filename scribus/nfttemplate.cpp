/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
 *   Riku Leino, tsoots@gmail.com                                          *
 ***************************************************************************/
#include "nfttemplate.h"
#include <QFileInfo>

nfttemplate::nfttemplate(QFile* tmplXmlFile, const QString &tmplCategory) 
{
	tmplXml = tmplXmlFile;
	templateCategory = tmplCategory;
	isWritable = tmplXml->open(QIODevice::WriteOnly | QIODevice::ReadOnly);
	tmplXml->close();
	isDeleted = false;
}

void nfttemplate::remove()
{
	if (tmplXml->exists())
	{
		QString newTmplXml = "";
		QString tmp;
		bool collect = false;
		tmplXml->open(QIODevice::ReadOnly);
		QTextStream stream(tmplXml);
		stream.setCodec("UTF-8");
		QString line = stream.readLine();
		while (!line.isNull())
		{
			if ((line.indexOf(enCategory) != -1) || collect)
			{
				collect = true;
				line += "\n";
				tmp += line;
				if (line.indexOf("name") != -1)
				{
					if (line.indexOf(name) == -1)
					{
						collect = false;
						newTmplXml += tmp;
						tmp = "";
					}
				} 
				else if (line.indexOf("file") != -1)
				{
					QString shortFile = file.right(file.length() - file.lastIndexOf("/") -1);
					if (line.indexOf(shortFile) == -1)
					{
						collect = false;
						newTmplXml += tmp;
						tmp = "";
					}
				} 
				else if (line.indexOf("</template>") != -1)
				{
					collect = false;
					tmp = "";
				}
			}
			else
			{
				line += "\n";
				newTmplXml += line;
			}
			line = stream.readLine();
		}
		tmplXml->close();
		tmplXml->open(QIODevice::WriteOnly);
		QTextStream instream(tmplXml);
		instream.setCodec("UTF-8");
		instream << newTmplXml;
		tmplXml->close();
	}
}

bool nfttemplate::canWrite()
{
	return isWritable;
}

bool nfttemplate::isValid()
{
	bool ret = true;
	
	QFileInfo *fi = new QFileInfo(file);
	if (!fi->exists())
		ret = false;
	delete fi;
	
	fi = new QFileInfo(tnail);
	if (!fi->exists())
		ret = false;
	delete fi;
	
	fi = new QFileInfo(img);
	if (!fi->exists())
		ret = false;
	delete fi;
	
	return ret;	
}

nfttemplate::~nfttemplate() 
{
	if (isDeleted)
		remove();
}
