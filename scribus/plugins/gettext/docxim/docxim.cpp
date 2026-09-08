/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
							 -------------------
	begin                : Sat Nov 15 2015
	copyright            : (C) 2015 by Franz Schmid
	email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/
#include "docxim.h"

#include <QApplication>
#include <QByteArray>

#include "langmgr.h"
#include "scribusdoc.h"
#include "styles/charstyle.h"
#include "styles/paragraphstyle.h"
#include "third_party/zip/scribus_zip.h"
#include "prefsmanager.h"
#include "ui/missing.h"


QString FileFormatName()
{
    return QObject::tr("DOCX Document");
}

QStringList FileExtensions()
{
	return QStringList("docx");
}

void GetText2(const QString& filename, const QString& /*encoding*/, bool textOnly, bool prefix, bool append, PageItem *textItem)
{
	auto docxim = std::make_unique<DocXIm>(textItem, prefix, append);
	docxim->importFile(filename, textOnly);
}

DocXIm::DocXIm(PageItem *textItem, bool prefix, bool append)
{
	m_Doc = textItem->doc();
	m_item = textItem;
	m_prefixName = prefix;
	m_append = append;
}

void DocXIm::importFile(const QString& fileName, bool textOnly)
{
	m_zip = std::make_unique<ScZipHandler>();
	if (!m_zip->open(fileName))
		return;

	parseContentTypes();
	if (textOnly)
		parsePlainTextOnly(m_item);
	else
	{
		if (!themePart.isEmpty())
			parseTheme();
		parseStyles();
		parseStyledText(m_item);
	}
	m_zip->close();
	m_zip.reset();

	m_item->itemText.trim();
	m_item->itemText.invalidateLayout();
}

void DocXIm::parseContentTypes()
{
	QByteArray xmlData;
	QDomDocument designMapDom;
	if (!m_zip->read("[Content_Types].xml", xmlData))
		return;

#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
	QDomDocument::ParseOptions parseOptions = QDomDocument::ParseOption::PreserveSpacingOnlyNodes;
	QDomDocument::ParseResult parseResult = designMapDom.setContent(xmlData, parseOptions);
	if (!parseResult)
	{
		qDebug() << "Error loading File" << parseResult.errorMessage << "at Line" << parseResult.errorLine << "Column" << parseResult.errorColumn;
		return;
	}
#else
	QString errorMsg;
	int errorLine = 0;
	int errorColumn = 0;
	if (!designMapDom.setContent(xmlData, false, &errorMsg, &errorLine, &errorColumn))
	{
		qDebug() << "Error loading File" << errorMsg << "at Line" << errorLine << "Column" << errorColumn;
		return;
	}
#endif

	QDomElement docElem = designMapDom.documentElement();
	for (QDomElement drawPag = docElem.firstChildElement(); !drawPag.isNull(); drawPag = drawPag.nextSiblingElement())
	{
		if (drawPag.tagName() == "Override")
		{
			QString contentTyp = drawPag.attribute("ContentType");
			if (contentTyp == "application/vnd.openxmlformats-officedocument.theme+xml")
			{
				themePart = drawPag.attribute("PartName");
				if (themePart.startsWith("/"))
					themePart.remove(0, 1);
			}
			else if (contentTyp == "application/vnd.openxmlformats-officedocument.wordprocessingml.document.main+xml")
			{
				docPart = drawPag.attribute("PartName");
				if (docPart.startsWith("/"))
					docPart.remove(0, 1);
			}
			else if (contentTyp == "application/vnd.openxmlformats-officedocument.wordprocessingml.styles+xml")
			{
				stylePart = drawPag.attribute("PartName");
				if (stylePart.startsWith("/"))
					stylePart.remove(0, 1);
			}
		}
	}
}

void DocXIm::parseTheme()
{
	QByteArray xmlData;
	QDomDocument designMapDom;
	if (!m_zip->read(themePart, xmlData))
		return;

#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
	QDomDocument::ParseOptions parseOptions = QDomDocument::ParseOption::PreserveSpacingOnlyNodes;
	QDomDocument::ParseResult parseResult = designMapDom.setContent(xmlData, parseOptions);
	if (!parseResult)
	{
		qDebug() << "Error loading File" << parseResult.errorMessage << "at Line" << parseResult.errorLine << "Column" << parseResult.errorColumn;
		return;
	}
#else
	QString errorMsg;
	int errorLine = 0;
	int errorColumn = 0;
	if (!designMapDom.setContent(xmlData, false, &errorMsg, &errorLine, &errorColumn))
	{
		qDebug() << "Error loading File" << errorMsg << "at Line" << errorLine << "Column" << errorColumn;
		return;
	}
#endif

	QDomElement docElem = designMapDom.documentElement();
	for (QDomElement drawPag = docElem.firstChildElement(); !drawPag.isNull(); drawPag = drawPag.nextSiblingElement())
	{
		if (drawPag.tagName() != "a:themeElements")
			continue;

		for (QDomElement spf = drawPag.firstChildElement(); !spf.isNull(); spf = spf.nextSiblingElement())
		{
			if (spf.tagName() != "a:fontScheme")
				continue;

			for (QDomElement spr = spf.firstChildElement(); !spr.isNull(); spr = spr.nextSiblingElement())
			{
				if (spr.tagName() == "a:minorFont")
				{
					QDomElement sty = spr.firstChildElement("a:latin");
					if (!sty.isNull())
						themeFont1 = sty.attribute("typeface");
				}
				else if (spr.tagName() == "a:majorFont")
				{
					QDomElement sty = spr.firstChildElement("a:latin");
					if (!sty.isNull())
						themeFont2 = sty.attribute("typeface");
				}
			}
		}
	}
}

void DocXIm::parseStyles()
{
	QByteArray xmlData;
	QDomDocument designMapDom;
	if (!m_zip->read(stylePart, xmlData))
		return;

#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
	QDomDocument::ParseOptions parseOptions = QDomDocument::ParseOption::PreserveSpacingOnlyNodes;
	QDomDocument::ParseResult parseResult = designMapDom.setContent(xmlData, parseOptions);
	if (!parseResult)
	{
		qDebug() << "Error loading File" << parseResult.errorMessage << "at Line" << parseResult.errorLine << "Column" << parseResult.errorColumn;
		return;
	}
#else
	QString errorMsg;
	int errorLine = 0;
	int errorColumn = 0;
	if (!designMapDom.setContent(xmlData, false, &errorMsg, &errorLine, &errorColumn))
	{
		qDebug() << "Error loading File" << errorMsg << "at Line" << errorLine << "Column" << errorColumn;
		return;
	}
#endif

	defaultParagraphStyle.setParent(CommonStrings::DefaultParagraphStyle);
	defaultParagraphStyle.charStyle().setParent(CommonStrings::DefaultCharacterStyle);
	currentParagraphStyle.setParent(CommonStrings::DefaultParagraphStyle);
	currentParagraphStyle.charStyle().setParent(CommonStrings::DefaultCharacterStyle);
	currentParagraphStyle.setLineSpacingMode(ParagraphStyle::AutomaticLineSpacing);

	defaultCharacterStyle.setParent(CommonStrings::DefaultCharacterStyle);

	QDomElement docElem = designMapDom.documentElement();
	for (QDomElement drawPag = docElem.firstChildElement(); !drawPag.isNull(); drawPag = drawPag.nextSiblingElement())
	{
		if (drawPag.tagName() == "w:docDefaults")
		{
			for (QDomElement spf = drawPag.firstChildElement(); !spf.isNull(); spf = spf.nextSiblingElement())
			{
				if (spf.tagName() == "w:rPrDefault")
				{
					for (QDomElement spr = spf.firstChildElement(); !spr.isNull(); spr = spr.nextSiblingElement())
					{
						if (spr.tagName() == "w:rPr")
						{
							parseCharProps(spr, defaultParagraphStyle);
							parseCharProps(spr, defaultCharacterStyle);
						}
					}
				}
				else if (spf.tagName() == "w:pPrDefault")
				{
					for (QDomElement spr = spf.firstChildElement(); !spr.isNull(); spr = spr.nextSiblingElement())
					{
						if (spr.tagName() == "w:pPr")
							parseParaProps(spr, defaultParagraphStyle);
					}
				}
			}
		}
		else if (drawPag.tagName() == "w:style")
		{
			if (drawPag.attribute("w:type") == "paragraph")
			{
				QDomElement nam = drawPag.firstChildElement("w:name");
				if (!nam.isNull())
				{
					if (m_prefixName)
						paraStyleIDToNameMap.insert(drawPag.attribute("w:styleId"), m_item->itemName() + "_" + nam.attribute("w:val"));
					else
						paraStyleIDToNameMap.insert(drawPag.attribute("w:styleId"), nam.attribute("w:val"));
					ParagraphStyle newStyle;
					newStyle = defaultParagraphStyle;
					if (m_prefixName)
						newStyle.setName(m_item->itemName() + "_" + nam.attribute("w:val"));
					else
						newStyle.setName(nam.attribute("w:val"));
					for (QDomElement spf = drawPag.firstChildElement(); !spf.isNull(); spf = spf.nextSiblingElement())
					{
						if (spf.tagName() == "w:basedOn")
						{
							QString parentN = spf.attribute("w:val");
							if (paraStyleIDToNameMap.contains(parentN))
							{
								parentN = paraStyleIDToNameMap[parentN];
								if (m_Doc->paragraphStyles().contains(parentN))
									newStyle.setParent(parentN);
							}
						}
						else if (spf.tagName() == "w:rPr")
							parseCharProps(spf, newStyle);
						else if (spf.tagName() == "w:pPr")
							parseParaProps(spf, newStyle);
					}
					StyleSet<ParagraphStyle>tmp;
					tmp.create(newStyle);
					m_Doc->redefineStyles(tmp, false);
				}
			}
			else if (drawPag.attribute("w:type") == "character")
			{
				QDomElement nam = drawPag.firstChildElement("w:name");
				if (!nam.isNull())
				{
					if (m_prefixName)
						charStyleIDToNameMap.insert(drawPag.attribute("w:styleId"), m_item->itemName() + "_" + nam.attribute("w:val"));
					else
						charStyleIDToNameMap.insert(drawPag.attribute("w:styleId"), nam.attribute("w:val"));
					CharStyle newStyle;
					newStyle = defaultCharacterStyle;
					if (m_prefixName)
						newStyle.setName(m_item->itemName() + "_" + nam.attribute("w:val"));
					else
						newStyle.setName(nam.attribute("w:val"));
					for (QDomElement spf = drawPag.firstChildElement(); !spf.isNull(); spf = spf.nextSiblingElement())
					{
						if (spf.tagName() == "w:basedOn")
						{
							QString parentN = spf.attribute("w:val");
							if (charStyleIDToNameMap.contains(parentN))
							{
								parentN = charStyleIDToNameMap[parentN];
								if (m_Doc->charStyles().contains(parentN))
									newStyle.setParent(parentN);
							}
						}
						else if (spf.tagName() == "w:rPr")
							parseCharProps(spf, newStyle);
					}
					StyleSet<CharStyle>tmp;
					tmp.create(newStyle);
					m_Doc->redefineCharStyles(tmp, false);
				}
			}
		}
	}
}

void DocXIm::parseStyledText(PageItem *textItem)
{
	QByteArray xmlData;
	QDomDocument designMapDom;
	if (!m_zip->read(docPart, xmlData))
		return;

#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
	QDomDocument::ParseOptions parseOptions = QDomDocument::ParseOption::PreserveSpacingOnlyNodes;
	QDomDocument::ParseResult parseResult = designMapDom.setContent(xmlData, parseOptions);
	if (!parseResult)
	{
		qDebug() << "Error loading File" << parseResult.errorMessage << "at Line" << parseResult.errorLine << "Column" << parseResult.errorColumn;
		return;
	}
#else
	QString errorMsg;
	int errorLine = 0;
	int errorColumn = 0;
	if (!designMapDom.setContent(xmlData, false, &errorMsg, &errorLine, &errorColumn))
	{
		qDebug() << "Error loading File" << errorMsg << "at Line" << errorLine << "Column" << errorColumn;
		return;
	}
#endif

	if (!m_append)
	{
		QString pStyleD = CommonStrings::DefaultParagraphStyle;
		ParagraphStyle newStyle;
		newStyle.setDefaultStyle(false);
		newStyle.setParent(pStyleD);
		textItem->itemText.clear();
		textItem->itemText.setDefaultStyle(newStyle);
	}
	textItem->itemText.setDefaultStyle(defaultParagraphStyle);

	QDomElement docElem = designMapDom.documentElement();
	for (QDomElement drawPag = docElem.firstChildElement(); !drawPag.isNull(); drawPag = drawPag.nextSiblingElement())
	{
		if (drawPag.tagName() == "w:body")
		{
			for (QDomElement spf = drawPag.firstChildElement(); !spf.isNull(); spf = spf.nextSiblingElement())
			{
				if (spf.tagName() == "w:p")
				{
					currentParagraphStyle = defaultParagraphStyle;
					bool hasStyle = false;
					QString currStyleName;
					for (QDomElement spr = spf.firstChildElement(); !spr.isNull(); spr = spr.nextSiblingElement())
					{
						if (spr.tagName() == "w:pPr")
						{
							QDomElement sty = spr.firstChildElement("w:pStyle");
							if (!sty.isNull())
							{
								QString nam = sty.attribute("w:val");
								if (paraStyleIDToNameMap.contains(nam))
								{
									ParagraphStyle newStyle;
									newStyle.setParent(paraStyleIDToNameMap[nam]);
									currentParagraphStyle = newStyle;
									hasStyle = true;
									currStyleName = paraStyleIDToNameMap[nam];
									currentParagraphStyle.charStyle() = newStyle.charStyle();
								}
							}
							parseParaProps(spr, currentParagraphStyle);
						}
						else if (spr.tagName() == "w:r")
						{
							CharStyle savedStyle = currentParagraphStyle.charStyle();

							for (QDomElement spt = spr.firstChildElement(); !spt.isNull(); spt = spt.nextSiblingElement())
							{
								if (spt.tagName() == "w:t")
								{
									QString txt = spt.text();
									if (!txt.isEmpty())
									{
										txt.replace(QChar(10), SpecialChars::LINEBREAK);
										txt.replace(QChar(12), SpecialChars::FRAMEBREAK);
										txt.replace(QChar(30), SpecialChars::NBHYPHEN);
										txt.replace(QChar(160), SpecialChars::NBSPACE);
										int posT = textItem->itemText.length();
										textItem->itemText.insertChars(posT, txt);
										textItem->itemText.applyStyle(posT, currentParagraphStyle);
										textItem->itemText.applyCharStyle(posT, txt.length(), currentParagraphStyle.charStyle());
									}
								}
								else if (spt.tagName() == "w:tab")
								{
									int posT = textItem->itemText.length();
									textItem->itemText.insertChars(posT, SpecialChars::TAB);
									textItem->itemText.applyStyle(posT, currentParagraphStyle);
								}
								else if (spt.tagName() == "w:br")
								{
									int posT = textItem->itemText.length();
									textItem->itemText.insertChars(posT, SpecialChars::LINEBREAK);
									textItem->itemText.applyStyle(posT, currentParagraphStyle);
								}
								else if (spt.tagName() == "w:rPr")
									parseCharProps(spt, currentParagraphStyle);
							}

							currentParagraphStyle.charStyle() = savedStyle;
						}
					}
					textItem->itemText.insertChars(textItem->itemText.length(), SpecialChars::PARSEP);
					textItem->itemText.applyStyle(textItem->itemText.length(), currentParagraphStyle);
				}
			}
		}
	}
}

void DocXIm::parseParaProps(QDomElement &props, ParagraphStyle &pStyle)
{
	for (QDomElement spt = props.firstChildElement(); !spt.isNull(); spt = spt.nextSiblingElement())
	{
		if (spt.tagName() == "w:rPr")
		{
			parseCharProps(spt, pStyle);
		}
		else if (spt.tagName() == "w:jc")
		{
			QString align = spt.attribute("w:val");
			if (align == "start" || align == "left")
				pStyle.setAlignment(ParagraphStyle::LeftAligned);
			else if (align == "center")
				pStyle.setAlignment(ParagraphStyle::Centered);
			else if (align == "end")
				pStyle.setAlignment(ParagraphStyle::RightAligned);
			else if (align == "both")
				pStyle.setAlignment(ParagraphStyle::Justified);
			else if (align == "distribute")
				pStyle.setAlignment(ParagraphStyle::Extended);
		}
		else if (spt.tagName() == "w:ind")
		{
			if (spt.hasAttribute("w:firstLine"))
				pStyle.setFirstIndent(pixelsFromTwips(spt.attribute("w:firstLine", "0.0").toDouble()));
			if (spt.hasAttribute("w:hanging"))
				pStyle.setFirstIndent(pixelsFromTwips(-spt.attribute("w:hanging", "0.0").toDouble()));
			if (spt.hasAttribute("w:left"))
				pStyle.setLeftMargin(pixelsFromTwips(spt.attribute("w:left", "0.0").toDouble()));
			if (spt.hasAttribute("w:start"))
				pStyle.setLeftMargin(pixelsFromTwips(spt.attribute("w:start", "0.0").toDouble()));
			if (spt.hasAttribute("w:right"))
				pStyle.setRightMargin(pixelsFromTwips(spt.attribute("w:right", "0.0").toDouble()));
			if (spt.hasAttribute("w:end"))
				pStyle.setRightMargin(pixelsFromTwips(spt.attribute("w:end", "0.0").toDouble()));
		}
		else if (spt.tagName() == "w:spacing")
		{
			if (spt.hasAttribute("w:lineRule"))
			{
				double linsp = spt.attribute("w:line", "240").toDouble();
				pStyle.setLineSpacingMode(ParagraphStyle::FixedLineSpacing);
				if (spt.attribute("w:lineRule") == "auto")
					pStyle.setLineSpacing((pStyle.charStyle().fontSize() / 10.0) * (linsp / 240.0));
				else
					pStyle.setLineSpacing(pixelsFromTwips(linsp));
			}
			if (spt.hasAttribute("w:after"))
				pStyle.setGapAfter(pixelsFromTwips(spt.attribute("w:after", "0.0").toDouble()));
			if (spt.hasAttribute("w:before"))
				pStyle.setGapBefore(pixelsFromTwips(spt.attribute("w:before", "0.0").toDouble()));
		}
		else if (spt.tagName() == "w:shd")
		{
			if (spt.hasAttribute("w:fill"))
			{
				QString color = spt.attribute("w:fill");
				QColor colour;
				colour = QColor::fromString("#" + color);
				ScColor tmp;
				tmp.fromQColor(colour);
				tmp.setSpotColor(false);
				tmp.setRegistrationColor(false);
				QString fNam = m_Doc->PageColors.tryAddColor("FromDocX"+colour.name(), tmp);
				pStyle.setBackgroundColor(fNam);
			}
		}
	}
}

void DocXIm::parseCharProps(QDomElement& props, ParagraphStyle& pStyle)
{
	ScFace currentFont = pStyle.charStyle().font();
	if (currentFont.isNone() && !pStyle.charStyle().parent().isEmpty())
	{
		QString parentCStyleName = pStyle.charStyle().parent();
		const auto* parentCStyle = dynamic_cast<const CharStyle*>(m_Doc->charStyles().resolve(parentCStyleName));
		while (parentCStyle)
		{
			currentFont = parentCStyle->font();
			if (!currentFont.isNone())
				break;
			parentCStyleName = parentCStyle->parent();
			if (parentCStyleName.isEmpty())
				break;
			parentCStyle = dynamic_cast<const CharStyle*>(m_Doc->charStyles().resolve(parentCStyleName));
		}
	}
	if (currentFont.isNone() && !pStyle.parent().isEmpty())
	{
		QString parentStyleName = pStyle.parent();
		const auto* parentStyle = dynamic_cast<const ParagraphStyle*>(m_Doc->paragraphStyles().resolve(parentStyleName));
		while (parentStyle)
		{
			currentFont = parentStyle->charStyle().font();
			if (!currentFont.isNone())
				break;
			parentStyleName = parentStyle->parent();
			if (parentStyleName.isEmpty())
				break;
			parentStyle = dynamic_cast<const ParagraphStyle*>(m_Doc->paragraphStyles().resolve(parentStyleName));
		}
	}
	if (currentFont.isNone())
		currentFont = defaultCharacterStyle.font();
	parseCharProps(props, pStyle.charStyle(), currentFont);
}

void DocXIm::parseCharProps(QDomElement& props, CharStyle& cStyle, const ScFace& currFont)
{
	bool boldFont = false;
	bool italicFont = false;

	ScFace currentFace = cStyle.font();
	if (currentFace.isNone())
		currentFace = currFont;
	boldFont = currentFace.style().contains("Bold");
	italicFont = currentFace.style().contains("Italic");

	for (QDomElement spc = props.firstChildElement(); !spc.isNull(); spc = spc.nextSiblingElement())
	{
		if (spc.tagName() == "w:rStyle")
		{
			QString nam = spc.attribute("w:val");
			if (charStyleIDToNameMap.contains(nam))
			{
				ParagraphStyle newStyle;
				cStyle.setParent(charStyleIDToNameMap[nam]);
			}
		}
		if (spc.tagName() == "w:lang")
		{
			QString langAbbrev;
			if (spc.hasAttribute("w:bidi"))
				langAbbrev = spc.attribute("w:bidi");
			if (langAbbrev.isEmpty() && spc.hasAttribute("eastAsian"))
				langAbbrev = spc.attribute("w:eastAsian");
			if (langAbbrev.isEmpty() && spc.hasAttribute("w:val"))
				langAbbrev = spc.attribute("w:val");
			langAbbrev.replace('-', '_');

			QString fullLang = LanguageManager::instance()->getLangFromAbbrev(langAbbrev);
			if (fullLang.isEmpty())
			{
				QString langCode;
				QStringList decomposition = fullLang.split('_');
				if (!decomposition.isEmpty())
				{
					langCode = decomposition.first();
					fullLang = LanguageManager::instance()->getLangFromAbbrev(langCode);
					if (!fullLang.isEmpty())
						langAbbrev = langCode;
				}
			}
			if (!langAbbrev.isEmpty())
				cStyle.setLanguage(langAbbrev);
		}
		if (spc.tagName() == "w:b")
		{
			QString val = spc.attribute("w:val", "true");
			boldFont = (val == "true");
		}
		if (spc.tagName() == "w:i")
		{
			QString val = spc.attribute("w:val", "true");
			italicFont = (val == "true");
		}
		if (spc.tagName() == "w:u")
		{
			StyleFlag styleEffects;
			styleEffects |= ScStyle_Underline;
			cStyle.setFeatures(styleEffects.featureList());
		}
		else if (spc.tagName() == "w:sz")
			cStyle.setFontSize(spc.attribute("w:val").toDouble() / 2.0 * 10.0);
		else if (spc.tagName() == "w:rFonts")
		{
			QString font = spc.attribute("w:ascii");
			if (!font.isEmpty())
			{
				font = getFontName(font);
				cStyle.setFont(PrefsManager::instance().appPrefs.fontPrefs.AvailFonts[font]);
			}
			else
			{
				QString fonta = spc.attribute("w:asciiTheme");
				if (!fonta.isEmpty())
				{
					if (fonta == "minorHAnsi")
					{
						fonta = getFontName(themeFont1);
						cStyle.setFont(PrefsManager::instance().appPrefs.fontPrefs.AvailFonts[fonta]);
					}
					else if (fonta == "majorHAnsi")
					{
						fonta = getFontName(themeFont2);
						cStyle.setFont(PrefsManager::instance().appPrefs.fontPrefs.AvailFonts[fonta]);
					}
				}
			}
		}
		else if (spc.tagName() == "w:caps")
		{
			StyleFlag styleEffects = cStyle.effects();
			if (spc.hasAttribute("w:val"))
			{
				if (spc.attribute("w:val") == "true")
					styleEffects |= ScStyle_AllCaps;
				else
					styleEffects &= ~ScStyle_AllCaps;
			}
			else
				styleEffects |= ScStyle_AllCaps;
			cStyle.setFeatures(styleEffects.featureList());
		}
		else if (spc.tagName() == "w:smallCaps")
		{
			StyleFlag styleEffects = cStyle.effects();
			if (spc.hasAttribute("w:val"))
			{
				if (spc.attribute("w:val") == "true")
					styleEffects |= ScStyle_SmallCaps;
				else
					styleEffects &= ~ScStyle_SmallCaps;
			}
			else
				styleEffects |= ScStyle_SmallCaps;
			cStyle.setFeatures(styleEffects.featureList());
		}
		else if (spc.tagName() == "w:strike")
		{
			StyleFlag styleEffects = cStyle.effects();
			if (spc.hasAttribute("w:val"))
			{
				if (spc.attribute("w:val") == "true")
					styleEffects |= ScStyle_Strikethrough;
				else
					styleEffects &= ~ScStyle_Strikethrough;
			}
			else
				styleEffects |= ScStyle_Strikethrough;
			cStyle.setFeatures(styleEffects.featureList());
		}
		else if (spc.tagName() == "w:shadow")
		{
			StyleFlag styleEffects = cStyle.effects();
			if (spc.hasAttribute("w:val"))
			{
				if (spc.attribute("w:val") == "true")
					styleEffects |= ScStyle_Shadowed;
				else
					styleEffects &= ~ScStyle_Shadowed;
			}
			else
				styleEffects |= ScStyle_Shadowed;
			cStyle.setFeatures(styleEffects.featureList());
		}
		else if (spc.tagName() == "w:outline")
		{
			StyleFlag styleEffects = cStyle.effects();
			if (spc.hasAttribute("w:val"))
			{
				if (spc.attribute("w:val") == "true")
					styleEffects |= ScStyle_Outline;
				else
					styleEffects &= ~ScStyle_Outline;
			}
			else
				styleEffects |= ScStyle_Outline;
			cStyle.setFeatures(styleEffects.featureList());
		}
		else if (spc.tagName() == "w:color")
		{
			if (spc.hasAttribute("w:val"))
			{
				QString color = spc.attribute("w:val");
				QColor colour;
				colour = QColor::fromString("#" + color);
				ScColor tmp;
				tmp.fromQColor(colour);
				tmp.setSpotColor(false);
				tmp.setRegistrationColor(false);
				QString fNam = m_Doc->PageColors.tryAddColor("FromDocX"+colour.name(), tmp);
				cStyle.setFillColor(fNam);
			}
		}
		else if (spc.tagName() == "w:shd")
		{
			if (spc.hasAttribute("w:fill"))
			{
				QString color = spc.attribute("w:fill");
				QColor colour;
				colour = QColor::fromString("#" + color);
				ScColor tmp;
				tmp.fromQColor(colour);
				tmp.setSpotColor(false);
				tmp.setRegistrationColor(false);
				QString fNam = m_Doc->PageColors.tryAddColor("FromDocX"+colour.name(), tmp);
				cStyle.setBackColor(fNam);
			}
		}
		else if (spc.tagName() == "w:vertAlign")
		{
			StyleFlag styleEffects = cStyle.effects();
			if (spc.hasAttribute("w:val"))
			{
				if (spc.attribute("w:val") == "superscript")
					styleEffects |= ScStyle_Superscript;
				else if (spc.attribute("w:val") == "subscript")
					styleEffects |= ScStyle_Subscript;
				cStyle.setFeatures(styleEffects.featureList());
			}
		}
	}

	bool changedStyle = false;
	QString currentStyle = currentFace.style();
	if (boldFont && italicFont)
		changedStyle = (currentStyle != "Bold Italic");
	else if (boldFont)
		changedStyle = (currentStyle != "Bold");
	else if (italicFont)
		changedStyle = (currentStyle != "Italic");
	else
		changedStyle = (!currentStyle.isEmpty() && currentStyle != "Regular" && currentStyle != "Medium");

	if (changedStyle)
	{
		QString fontFamily = currentFace.family();
		if (!fontFamily.isEmpty())
		{
			QString font = getFontName(fontFamily, boldFont, italicFont);
			cStyle.setFont(PrefsManager::instance().appPrefs.fontPrefs.AvailFonts[font]);
		}
	}
}

void DocXIm::parsePlainTextOnly(PageItem *textItem)
{
	QByteArray xmlData;
	QDomDocument designMapDom;
	if (!m_zip->read(docPart, xmlData))
		return;

#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
	QDomDocument::ParseOptions parseOptions = QDomDocument::ParseOption::PreserveSpacingOnlyNodes;
	QDomDocument::ParseResult parseResult = designMapDom.setContent(xmlData, parseOptions);
	if (!parseResult)
	{
		qDebug() << "Error loading File" << parseResult.errorMessage << "at Line" << parseResult.errorLine << "Column" << parseResult.errorColumn;
		return;
	}
#else
	QString errorMsg;
	int errorLine = 0;
	int errorColumn = 0;
	if (!designMapDom.setContent(xmlData, false, &errorMsg, &errorLine, &errorColumn))
	{
		qDebug() << "Error loading File" << errorMsg << "at Line" << errorLine << "Column" << errorColumn;
		return;
	}
#endif

	if (!m_append)
	{
		QString pStyleD = CommonStrings::DefaultParagraphStyle;
		ParagraphStyle newStyle;
		newStyle.setDefaultStyle(false);
		newStyle.setParent(pStyleD);
		textItem->itemText.clear();
		textItem->itemText.setDefaultStyle(newStyle);
	}
	currentParagraphStyle.setParent(CommonStrings::DefaultParagraphStyle);
	currentParagraphStyle.charStyle().setParent(CommonStrings::DefaultCharacterStyle);
	currentParagraphStyle.setLineSpacingMode(ParagraphStyle::AutomaticLineSpacing);

	QDomElement docElem = designMapDom.documentElement();
	for (QDomElement drawPag = docElem.firstChildElement(); !drawPag.isNull(); drawPag = drawPag.nextSiblingElement())
	{
		if (drawPag.tagName() != "w:body")
			continue;

		for (QDomElement spf = drawPag.firstChildElement(); !spf.isNull(); spf = spf.nextSiblingElement())
		{
			if (spf.tagName() != "w:p")
				continue;

			for (QDomElement spr = spf.firstChildElement(); !spr.isNull(); spr = spr.nextSiblingElement())
			{
				if (spr.tagName() != "w:r")
					continue;

				for (QDomElement spt = spr.firstChildElement(); !spt.isNull(); spt = spt.nextSiblingElement())
				{
					if (spt.tagName() == "w:t")
					{
						QString txt = spt.text();
						if (!txt.isEmpty())
						{
							txt.replace(QChar(10), SpecialChars::LINEBREAK);
							txt.replace(QChar(12), SpecialChars::FRAMEBREAK);
							txt.replace(QChar(30), SpecialChars::NBHYPHEN);
							txt.replace(QChar(160), SpecialChars::NBSPACE);
							textItem->itemText.insertChars(textItem->itemText.length(), txt);
							textItem->itemText.applyStyle(textItem->itemText.length(), currentParagraphStyle);
							textItem->itemText.applyCharStyle(textItem->itemText.length(), txt.length(), currentParagraphStyle.charStyle());
						}
					}
					else if (spt.tagName() == "w:tab")
					{
						int posT = textItem->itemText.length();
						textItem->itemText.insertChars(posT, SpecialChars::TAB);
						textItem->itemText.applyStyle(posT, currentParagraphStyle);
					}
					else if (spt.tagName() == "w:br")
					{
						int posT = textItem->itemText.length();
						textItem->itemText.insertChars(posT, SpecialChars::LINEBREAK);
						textItem->itemText.applyStyle(posT, currentParagraphStyle);
					}
				}
			}

			textItem->itemText.insertChars(textItem->itemText.length(), SpecialChars::PARSEP);
			textItem->itemText.applyStyle(textItem->itemText.length(), currentParagraphStyle);
		}
	}
}

QString DocXIm::getFontName(const QString& name)
{
	QString fontName = name;
	SCFontsIterator it(PrefsManager::instance().appPrefs.fontPrefs.AvailFonts);
	for ( ; it.hasNext(); it.next())
	{
		if (it.current().family().toLower() == fontName.toLower())
		{
			if (it.currentKey().toLower() == fontName.toLower()) // exact Match
				return fontName;
			QStringList slist = PrefsManager::instance().appPrefs.fontPrefs.AvailFonts.fontMap.value(it.current().family());
			if (!slist.isEmpty())
			{
				slist.sort();
				int reInd = slist.indexOf("Regular");
				if (reInd < 0)
					fontName = it.current().family() + " " + slist[0];
				else
					fontName = it.current().family() + " " + slist[reInd];
				return fontName;
			}
		}
	}

	if (!PrefsManager::instance().appPrefs.fontPrefs.GFontSub.contains(fontName))
	{
		QApplication::changeOverrideCursor(QCursor(Qt::ArrowCursor));
		MissingFont dia(nullptr, fontName, m_Doc);
		static_cast<void>(dia.exec());
		QApplication::changeOverrideCursor(QCursor(Qt::WaitCursor));
		PrefsManager::instance().appPrefs.fontPrefs.GFontSub[fontName] = dia.getReplacementFont();
		fontName = dia.getReplacementFont();
	}
	else
		fontName = PrefsManager::instance().appPrefs.fontPrefs.GFontSub[fontName];

	return fontName;
}

QString DocXIm::getFontName(const QString& family, bool bold, bool italic)
{
	QString fontFamily = family;

	QString fontName = family;
	if (bold)
		fontName += " Bold";
	if (italic)
		fontName += " Italic";

	QString fontStyle = "Regular";
	if (bold && !italic)
		fontStyle = "Bold";
	else if (!bold && italic)
		fontStyle = "Italic";
	else if (bold && italic)
		fontStyle = "Bold Italic";

	SCFontsIterator it(PrefsManager::instance().appPrefs.fontPrefs.AvailFonts);
	for (; it.hasNext(); it.next())
	{
		if (it.current().family().toLower() != fontFamily.toLower())
			continue;
		if (it.currentKey().toLower() == fontName.toLower()) // exact Match
			return fontName;

		QStringList slist = PrefsManager::instance().appPrefs.fontPrefs.AvailFonts.fontMap.value(it.current().family());
		if (slist.isEmpty())
			continue;

		slist.sort();
		int reInd = slist.indexOf(fontStyle);
		if (reInd >= 0)
		{
			fontName = it.current().family() + " " + slist[reInd];
			return fontName;
		}
	}

	if (!PrefsManager::instance().appPrefs.fontPrefs.GFontSub.contains(fontName))
	{
		QApplication::changeOverrideCursor(QCursor(Qt::ArrowCursor));
		MissingFont dia(nullptr, fontName, m_Doc);
		static_cast<void>(dia.exec());
		QApplication::changeOverrideCursor(QCursor(Qt::WaitCursor));
		PrefsManager::instance().appPrefs.fontPrefs.GFontSub[fontName] = dia.getReplacementFont();
		fontName = dia.getReplacementFont();
	}
	else
		fontName = PrefsManager::instance().appPrefs.fontPrefs.GFontSub[fontName];

	return fontName;
}

double DocXIm::pixelsFromTwips(double twips) const
{
	return twips / 1440.0 * 72.0;
}
