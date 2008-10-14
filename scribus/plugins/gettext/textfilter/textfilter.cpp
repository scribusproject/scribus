/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "textfilter.h"
#include <QByteArray>
#include <QFile>
#include <QFileInfo>
#include <QString>
#include <QStringList>
#include <QTextCodec>
#include <QRegExp> 
#include <QMap>
#include <QToolTip>
#include "prefsmanager.h"
#include "prefsfile.h"
#include <vector>
#include "gtframestyle.h"
#include "gtparagraphstyle.h"
#include "tfdia.h"

QString FileFormatName()
{
    return QObject::tr("Text Filters");
}

QStringList FileExtensions()
{
	return QStringList();
}

void GetText(QString filename, QString encoding, bool, gtWriter *writer)
{
	TextFilter* tf = new TextFilter(filename, encoding, writer);
	delete tf;
}

/*********** Class TextFilter *************************************/

TextFilter::TextFilter(const QString& fname, const QString& enc, gtWriter* w)
{
	filename = fname;
	encoding = enc;
	writer = w;
	writer->setOverridePStyleFont(false);
	prefs = PrefsManager::instance()->prefsFile->getPluginContext("TextFilter");
	tfDia* tfdia = new tfDia();
	if (tfdia->exec())
	{
		filters = &(tfdia->filters);
		loadText();
		write();
	}
	delete tfdia;
}


void TextFilter::loadText()
{
	QByteArray bb;
	text = "";
	QFile f(filename);
	QFileInfo fi(f);
	if (!fi.exists())
		return;
	// read file content
	if (f.open(QIODevice::ReadOnly))
	{
		bb = f.readAll();
		f.close();
	}
	// decode file content
	if (bb.size() > 0)
	{
		QTextCodec *codec;
		if (encoding.isEmpty())
			codec = QTextCodec::codecForLocale();
		else
			codec = QTextCodec::codecForName(encoding.toLocal8Bit());
		text = codec->toUnicode(bb);
	}
}

void TextFilter::write()
{
	QMap<QString, gtParagraphStyle*> pstyles;
	gtFrameStyle* fstyle = writer->getDefaultStyle();
	for (int i = 0; i < static_cast<int>(filters->size()); ++i)
	{
		if ((*filters)[i]->isEnabled())
		{
			int action = (*filters)[i]->getAction();
			QString regExp = (*filters)[i]->regExp();
			QString replaceWith = (*filters)[i]->replaceWith();
			bool useRegexp = (*filters)[i]->isRegExp();
			if (useRegexp)
				replace(&replaceWith);
			QString pstyle = (*filters)[i]->getPStyleName();
			QRegExp rx = QRegExp(regExp);
			rx.setMinimal(true);
			switch (action)
			{
				case REMOVE: 
					if (useRegexp)
						text = text.remove(rx);
					else
						text = text.remove(regExp);
					break;
				case REPLACE:
					if (useRegexp)
						text = text.replace(rx, replaceWith); 
					else
						text = text.replace(regExp, replaceWith);
					break;
				case APPLY: 
					pstyles[pstyle] = new gtParagraphStyle(*fstyle);
					pstyles[pstyle]->setName(pstyle);
					break;
			}
		}
	}
	if (pstyles.size() == 0)
		writer->append(text);
	else
	{
		QStringList list = text.split("\n", QString::KeepEmptyParts);
		gtParagraphStyle *useStyle = NULL;
		for (int i = 0; i < static_cast<int>(list.size()); ++i)
		{
			QString tmpText(list[i]);
			QString tmpText2(tmpText);
			tmpText2.simplified();
			int numberOfWords = tmpText2.count(" ");
			++numberOfWords;
			useStyle = NULL;
			for (int j = 0; j < static_cast<int>(filters->size()); ++j)
			{
				if ((*filters)[j]->isEnabled())
				{
					int action = (*filters)[j]->getAction();
					QString regExp = (*filters)[j]->regExp();
					QString replaceWith = (*filters)[j]->replaceWith();
					bool useRegexp = (*filters)[j]->isRegExp();
					if (useRegexp)
						replace(&replaceWith);
					QString pstyle = (*filters)[j]->getPStyleName();
					int lessThan = (*filters)[j]->getLessThan();
					int moreThan = (*filters)[j]->getMoreThan();
					int style = (*filters)[j]->getStyle();
					bool removeMatch = (*filters)[j]->removeMatch();
					QRegExp rx = QRegExp(regExp);
					if (!pstyle.isEmpty())
					{
						switch (action)
						{
							case APPLY:
								switch (style)
								{
								case ALL_PARAGRAPHS:
									useStyle = pstyles[pstyle];
									break;
								case STARTS_WITH:
									if (useRegexp)
									{
										if (tmpText2.indexOf(rx) == 0)
										{
											useStyle = pstyles[pstyle];
											if (removeMatch)
												tmpText.remove(rx);
										}
									}
									else
									{
										if (tmpText2.indexOf(regExp) == 0)
										{
											useStyle = pstyles[pstyle];
											if (removeMatch)
												tmpText.remove(regExp);
										}
									}
									break;
								case LESS_THAN:
									if ((lessThan != -1) && (numberOfWords < lessThan))
										useStyle = pstyles[pstyle];
									break;
								case MORE_THAN:
									if ((moreThan != -1) && (numberOfWords > moreThan))
										useStyle = pstyles[pstyle];
									break;
								}
								break;
						}
					}
				}
			}
			if (i == static_cast<int>(list.size()) - 1)
				writer->append(tmpText, useStyle);
			else
				writer->append(tmpText + "\n", useStyle);
		}
	}
}

void TextFilter::replace(QString* text)
{
	text->replace("\\\\", "__SLASH_HERE__");
	text->replace("\\", "\\");
	text->replace("__SLASH_HERE__", "\\\\");

	text->replace("\\\\t", "__|TABCHAR|__");
	text->replace("\\t", "\t");
	text->replace("__|TABCHAR|__", "\\t");

	text->replace("\\\\n", "__|L-C|__");
	text->replace("\\n", "\n");
	text->replace("__|L-C|__", "\\n");

	text->replace("\\\\f", "__|F-CHAR|__");
	text->replace("\\f", "\f");
	text->replace("__|F-CHAR|__", "\\f");

	text->replace("\\\\r", "__|R-CHAR|__");
	text->replace("\\r", "\r");
	text->replace("__|R-CHAR|__", "\\r");

	text->replace("\\\\v", "__|V-CHAR|__");
	text->replace("\\v", "\v");
	text->replace("__|V-CHAR|__", "\\v");

	replaceHex(text);
}

void TextFilter::replaceHex(QString* text)
{
	int index;
	int pos = 0;
	QString hexS;
	int hex;
	bool ok = false;
	do
	{
		index = text->indexOf("\\x", pos);
		if (index != -1)
		{
			if ((text->length() - index + 1) > 6)
			{
				hexS = text->mid(index + 2, 4);
				hex = hexS.toInt(&ok, 16);
				if (ok)
				{
					text->replace("\\x" + hexS, QChar(hex));
				}
			}
			else
				index = -1;
			pos += 2;
		}
	}
	while (index != -1);
}

TextFilter::~TextFilter()
{

}
