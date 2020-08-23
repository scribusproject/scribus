/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
							 -------------------
	begin                : Wed Aug 19 2020
	copyright            : (C) 2020 by Craig Bradney
	email                : cbradney@scribus.info
 ***************************************************************************/
#include "markdownim.h"

#include <QApplication>
#include <QByteArray>
#include <QTextBlock>
#include <QTextBlockFormat>
#include <QTextCharFormat>
#include <QTextDocument>
#include <QTextFrameFormat>
#include <QTextListFormat>

#include "prefsmanager.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "styles/charstyle.h"
#include "styles/paragraphstyle.h"

QString FileFormatName()
{
	return QObject::tr("MarkDown Document");
}

QStringList FileExtensions()
{
	return QStringList("md");
}

void GetText2(const QString& filename, const QString& encoding, bool textOnly, bool prefix, bool append, PageItem *textItem)
{
	MarkDownIm* markdownim = new MarkDownIm(filename, textItem, textOnly, prefix, append);
	delete markdownim;
}

MarkDownIm::MarkDownIm(const QString& fileName, PageItem *textItem, bool textOnly, bool prefix, bool append)
{
	m_Doc = textItem->doc();
	m_item = textItem;
	m_textOnly = textOnly;
	m_prefixName = prefix;
	m_append = append;

	QFile f(fileName);
	if (!f.open(QIODevice::ReadOnly))
		return;

	QByteArray data = f.readAll();
	f.close();

	m_importedText.setMarkdown(data, QTextDocument::MarkdownDialectGitHub);
	if (!append)
	{
		QString defaultParagraphStyle = CommonStrings::DefaultParagraphStyle;
		ParagraphStyle newStyle;
		newStyle.setDefaultStyle(false);
		newStyle.setParent(defaultParagraphStyle);
		m_item->itemText.clear();
		m_item->itemText.setDefaultStyle(newStyle);
	}
	parseMarkDown();
	m_item->itemText.trim();
	m_item->itemText.invalidateLayout();
}

void MarkDownIm::parseMarkDown()
{
	struct styleStruct
	{
		int start;
		int length;
		QString styleName;
	};
	QList<styleStruct> styleApplicationList;

	StyleSet<ParagraphStyle> newParaStyleSet;
	StyleSet<CharStyle> newCharStyleSet;
	defaultParagraphStyle.setParent(CommonStrings::DefaultParagraphStyle);
	defaultParagraphStyle.charStyle().setParent(CommonStrings::DefaultCharacterStyle);
	defaultParagraphStyle.setLineSpacingMode(ParagraphStyle::AutomaticLineSpacing);
	m_item->itemText.clear();
	m_item->itemText.setDefaultStyle(defaultParagraphStyle);

	int blockCount = m_importedText.blockCount();
	for (int i = 0; i < blockCount; i++)
	{
		QTextBlock curblk = m_importedText.findBlockByNumber(i);
		if (!curblk.isValid())
			continue;
		styleApplicationList.clear();
		currentParagraphStyle = defaultParagraphStyle;
		if (!m_textOnly)
		{
			//Check for and create Heading styles
			int headingLevel = curblk.blockFormat().headingLevel();
			if (headingLevel > 0)
			{
				QString headingName=QString("Heading %1").arg(headingLevel);
				if (!newParaStyleSet.contains(headingName))
				{
					ParagraphStyle newParaStyle;
					newParaStyle.setParent(CommonStrings::DefaultParagraphStyle);
					newParaStyle.setLineSpacingMode(ParagraphStyle::AutomaticLineSpacing);
					newParaStyle.setContext(m_item->itemText.defaultStyle().context());
					newParaStyle.charStyle().setFontSize(20 * (7-headingLevel) + newParaStyle.charStyle().fontSize());
					QString styleName;
					if (m_prefixName)
						styleName=m_item->itemName() + "_";
					styleName+=headingName;
					newParaStyle.setName(styleName);
					newParaStyleSet.create(newParaStyle);
					currentParagraphStyle = newParaStyle;
				}
				else
					currentParagraphStyle = newParaStyleSet.get(headingName);
			}

			for (QTextBlock::iterator it = curblk.begin(); !(it.atEnd()); ++it)
			{
				QTextFragment currentFragment = it.fragment();
				if (currentFragment.isValid())
				{
					if (currentFragment.charFormat().isValid())
					{
						QTextCharFormat cf = currentFragment.charFormat();
						bool charFormatFound = false;
						CharStyle cs;
						cs.setParent(CommonStrings::DefaultCharacterStyle);
						cs.setContext(m_item->itemText.defaultStyle().context());
						QString styleName;
						if (m_prefixName)
							styleName=m_item->itemName() + "_";
						StyleFlag s(cs.effects());
						if (cf.fontStrikeOut())
						{
							charFormatFound = true;
							styleName+="Strikethrough";
							cs.setName(styleName);
							s |= ScStyle_Strikethrough;
							cs.setFeatures(s.featureList());
						}
						//Bold and Italic
						bool isBold = cf.fontWeight() == QFont::Bold;
						bool isItalic = cf.fontItalic();
						if ((isBold && headingLevel == 0) || isItalic)
						{
							charFormatFound = true;
							if (isBold)
								styleName+="Bold";
							if (isItalic)
								styleName+="Italic";
							cs.setName(styleName);
							QString family(m_item->itemText.charStyle(m_item->itemText.length()).font().family());
							if (!family.isEmpty())
							{
								QStringList slist = PrefsManager::instance().appPrefs.fontPrefs.AvailFonts.fontMap[family];
								if (isBold)
								{
									if (isItalic)
									{
										if (slist.contains("Bold Italic"))
											cs.setFont(PrefsManager::instance().appPrefs.fontPrefs.AvailFonts[family + " Bold Italic"]);
									}
									else if (slist.contains("Bold"))
										cs.setFont(PrefsManager::instance().appPrefs.fontPrefs.AvailFonts[family + " Bold"]);
								}
								else
								{
									if (isItalic)
									{
										if (slist.contains("Italic"))
											cs.setFont(PrefsManager::instance().appPrefs.fontPrefs.AvailFonts[family + " Italic"]);
									}
									else if (slist.contains("Regular"))
										cs.setFont(PrefsManager::instance().appPrefs.fontPrefs.AvailFonts[family + " Regular"]);
								}
							}
						}
						if (charFormatFound)
						{
							newCharStyleSet.create(cs);
							styleStruct ss;
							ss.start = currentFragment.position();
							ss.length = currentFragment.length();
							ss.styleName = cs.name();
							styleApplicationList.append(ss);
						}
					}
				}
			}
		}

		ParagraphStyle paraStyle;
		paraStyle.setParent(currentParagraphStyle.name());
		m_item->itemText.insertChars(m_item->itemText.length(), curblk.text());
		m_item->itemText.insertChars(m_item->itemText.length(), SpecialChars::PARSEP);
		if (!m_textOnly)
		{
			m_item->itemText.applyStyle(m_item->itemText.length() - 1, paraStyle);
			for (int i = 0; i < styleApplicationList.size(); ++i)
			{
				CharStyle charStyle;
				charStyle.setParent(styleApplicationList.at(i).styleName);
				m_item->itemText.applyCharStyle(styleApplicationList.at(i).start, styleApplicationList.at(i).length, charStyle);
			}
		}
	}
	if (!m_textOnly)
	{
		m_Doc->redefineStyles(newParaStyleSet, false);
		m_Doc->redefineCharStyles(newCharStyleSet, false);
	}
}


