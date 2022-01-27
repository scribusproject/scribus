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
#include <QTextList>
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

void GetText2(const QString& filename, const QString& /*encoding*/, bool textOnly, bool prefix, bool append, PageItem *textItem)
{
	MarkDownIm* markdownim = new MarkDownIm(filename, textItem, textOnly, prefix, append);
	delete markdownim;
}

MarkDownIm::MarkDownIm(const QString& fileName, PageItem *textItem, bool textOnly, bool prefix, bool append)
          : m_Doc(textItem->doc()),
	        m_item(textItem),
	        m_prefixName(prefix),
	        m_append(append),
	        m_textOnly(textOnly)
{
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
		int start { 0 };
		int length { 0 };
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

	int listStyleCount = 1;
	int listCounter = 1;
	int blockCount = m_importedText.blockCount();
	QString listStyleName;
	for (int i = 0; i < blockCount; i++)
	{
		QTextBlock curblk = m_importedText.findBlockByNumber(i);
		if (!curblk.isValid())
			continue;
		QTextBlockFormat tbf = curblk.blockFormat();
		styleApplicationList.clear();
		currentParagraphStyle = defaultParagraphStyle;
		if (!m_textOnly)
		{
			//Headings, find and then create styles
			int headingLevel = curblk.blockFormat().headingLevel();
			if (headingLevel > 0)
			{
				QString headingStyleName = QString("Heading %1").arg(headingLevel);
				if (m_prefixName)
					headingStyleName = m_item->itemName() + "_" + headingStyleName;
				if (!newParaStyleSet.contains(headingStyleName))
				{
					ParagraphStyle newParaStyle;
					newParaStyle.setParent(CommonStrings::DefaultParagraphStyle);
					newParaStyle.setLineSpacingMode(ParagraphStyle::AutomaticLineSpacing);
					newParaStyle.setContext(m_item->itemText.defaultStyle().context());
					newParaStyle.charStyle().setFontSize(20 * (7 - headingLevel) + newParaStyle.charStyle().fontSize());
					newParaStyle.setName(headingStyleName);
					const auto* newParaStylePtr = newParaStyleSet.create(newParaStyle);
					currentParagraphStyle.setContext(newParaStylePtr->context());
					currentParagraphStyle = *newParaStylePtr;
				}
				else
				{
					const auto& headingStyle = newParaStyleSet.get(headingStyleName);
					currentParagraphStyle.setContext(headingStyle.context());
					currentParagraphStyle = headingStyle;
				}
			}

			//List items
			if (curblk.textList() != nullptr)
			{
				QTextListFormat tlf = curblk.textList()->format();
				if (listCounter == 1)
				{
					listStyleName = QString("List %1").arg(listStyleCount);
					if (tlf.style() == QTextListFormat::ListDecimal)
						listStyleName.prepend("Numbered ");
					if (m_prefixName)
						listStyleName = m_item->itemName() + "_" + listStyleName;
					if (!newParaStyleSet.contains(listStyleName))
					{
						listStyleCount++;
						ParagraphStyle newParaStyle;
						newParaStyle.setParent(CommonStrings::DefaultParagraphStyle);
						newParaStyle.setLineSpacingMode(ParagraphStyle::AutomaticLineSpacing);
						newParaStyle.setContext(m_item->itemText.defaultStyle().context());
						switch (tlf.style())
						{
							case QTextListFormat::ListDisc:
								newParaStyle.setBulletStr(QChar(0x2022));
								newParaStyle.setHasBullet(true);
								break;
							case QTextListFormat::ListCircle:
								newParaStyle.setBulletStr(QChar(0x2022));
								newParaStyle.setHasBullet(true);
								break;
							case QTextListFormat::ListSquare:
								newParaStyle.setBulletStr(QChar(0x25A0));
								newParaStyle.setHasBullet(true);
								break;
							case QTextListFormat::ListDecimal:
								newParaStyle.setHasNum(true);
								break;
							case QTextListFormat::ListLowerAlpha:
							case QTextListFormat::ListUpperAlpha:
							case QTextListFormat::ListLowerRoman:
							case QTextListFormat::ListUpperRoman:
							case QTextListFormat::ListStyleUndefined:
								break;
						}
						newParaStyle.setName(listStyleName);
						const auto* newParaStylePtr = newParaStyleSet.create(newParaStyle);
						currentParagraphStyle.setContext(newParaStylePtr->context());
						currentParagraphStyle = *newParaStylePtr;
					}
				}
				else
				{
					const auto& listStyle = newParaStyleSet.get(listStyleName);
					currentParagraphStyle.setContext(listStyle.context());
					currentParagraphStyle = listStyle;
				}
				if (listCounter++ == curblk.textList()->count())
					listCounter = 1;
			}
			//Process all fragments of the text block to find different formats within. Create a structure
			//of where and what style was applied
			//Strikethrough
			//Bold
			//Italic
			for (QTextBlock::iterator it = curblk.begin(); !(it.atEnd()); ++it)
			{
				QTextFragment currentFragment = it.fragment();
				if (!currentFragment.isValid())
					continue;

				if (!currentFragment.charFormat().isValid())
					continue;

				QTextCharFormat cf = currentFragment.charFormat();
				bool charFormatFound = false;
				CharStyle cs;
				cs.setParent(CommonStrings::DefaultCharacterStyle);
				cs.setContext(m_item->itemText.defaultStyle().context());
				QString styleName;
				if (m_prefixName)
					styleName = m_item->itemName() + "_";

				//Strikethrough
				if (cf.fontStrikeOut())
				{
					StyleFlag s(cs.effects());
					charFormatFound = true;
					styleName += "Strikethrough";
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
						styleName += "Bold";
					if (isItalic)
						styleName += "Italic";
					cs.setName(styleName);
					QString family(m_item->itemText.charStyle(m_item->itemText.length()).font().family());
					if (!family.isEmpty())
					{
						QStringList slist = PrefsManager::instance().appPrefs.fontPrefs.AvailFonts.fontMap[family];
						if (isBold && isItalic && slist.contains("Bold Italic"))
							cs.setFont(PrefsManager::instance().appPrefs.fontPrefs.AvailFonts[family + " Bold Italic"]);
						if (isBold && !isItalic && slist.contains("Bold"))
							cs.setFont(PrefsManager::instance().appPrefs.fontPrefs.AvailFonts[family + " Bold"]);
						if (!isBold && isItalic && slist.contains("Italic"))
							cs.setFont(PrefsManager::instance().appPrefs.fontPrefs.AvailFonts[family + " Italic"]);
//						if (!isBold && !isItalic && slist.contains("Regular"))
//							cs.setFont(PrefsManager::instance().appPrefs.fontPrefs.AvailFonts[family + " Regular"]);
					}
				}
						
				//If we found a character formatted then record the generated character style
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
		//Insert our text and a paragraph separator
		m_item->itemText.insertChars(m_item->itemText.length(), curblk.text());
		m_item->itemText.insertChars(m_item->itemText.length(), SpecialChars::PARSEP);
		//If we are not just inserting the text, apply a paragraph style and the character styles based on the positions we recorded earlier
		if (!m_textOnly)
		{
			ParagraphStyle paraStyle;
			paraStyle.setParent(currentParagraphStyle.name());
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


