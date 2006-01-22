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
#include <qfileinfo.h>

nfttemplate::nfttemplate(QFile* tmplXmlFile, const QString &tmplCategory) 
{
	tmplXml = tmplXmlFile;
	templateCategory = tmplCategory;
	isWritable = tmplXml->open(IO_WriteOnly | IO_ReadOnly);
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
		tmplXml->open(IO_ReadOnly);
		QTextStream stream(tmplXml);
		QString line = stream.readLine();
		while (line != NULL)
		{
			if ((line.find(enCategory) != -1) || collect)
			{
				collect = true;
				line += "\n";
				tmp += line;
				if (line.find("name") != -1)
				{
					if (line.find(name) == -1)
					{
						collect = false;
						newTmplXml += tmp;
						tmp = "";
					}
				} 
				else if (line.find("file") != -1)
				{
					QString shortFile = file.right(file.length() - file.findRev("/") -1);
					if (line.find(shortFile) == -1)
					{
						collect = false;
						newTmplXml += tmp;
						tmp = "";
					}
				} 
				else if (line.find("</template>") != -1)
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
		tmplXml->open(IO_WriteOnly);
		QTextStream instream(tmplXml);
		instream.setEncoding(QTextStream::UnicodeUTF8);
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
