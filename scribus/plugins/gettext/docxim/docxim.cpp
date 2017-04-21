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
#include <QXmlInputSource>
#include <QXmlSimpleReader>
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

void GetText2(QString filename, QString encoding, bool textOnly, bool prefix, bool append, PageItem *textItem)
{
	DocXIm* docxim = new DocXIm(filename, textItem, textOnly, prefix, append);
	delete docxim;
}

DocXIm::DocXIm(QString fileName, PageItem *textItem, bool textOnly, bool prefix, bool append)
{
	m_Doc = textItem->doc();
	m_item = textItem;
	m_prefixName = prefix;
	m_append = append;
	themePart = "";
	docPart = "";
	stylePart = "";
	themeFont1 = "";
	themeFont2 = "";
	uz = new ScZipHandler();
	if (!uz->open(fileName))
	{
		delete uz;
		return;
	}
	parseContentTypes();
	if (textOnly)
		parsePlainTextOnly(textItem);
	else
	{
		if (!themePart.isEmpty())
			parseTheme();
		parseStyles();
		parseStyledText(textItem);
	}
	uz->close();
	delete uz;
	textItem->itemText.trim();
	textItem->itemText.invalidateLayout();
}

void DocXIm::parseContentTypes()
{
	QByteArray f;
	QDomDocument designMapDom;
	if (!uz->read("[Content_Types].xml", f))
		return;
	QXmlInputSource xmlSource;
	xmlSource.setData(f);
	QXmlSimpleReader xmlReader;
	xmlReader.setFeature("http://xml.org/sax/features/namespace-prefixes", true);
	QString errorMsg = "";
	int errorLine = 0;
	int errorColumn = 0;
	if (!designMapDom.setContent(&xmlSource, &xmlReader, &errorMsg, &errorLine, &errorColumn))
	{
		qDebug() << "Error loading File" << errorMsg << "at Line" << errorLine << "Column" << errorColumn;
		return;
	}
	QDomElement docElem = designMapDom.documentElement();
	for(QDomElement drawPag = docElem.firstChildElement(); !drawPag.isNull(); drawPag = drawPag.nextSiblingElement())
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
	QByteArray f;
	QDomDocument designMapDom;
	if (!uz->read(themePart, f))
		return;
	QXmlInputSource xmlSource;
	xmlSource.setData(f);
	QXmlSimpleReader xmlReader;
	xmlReader.setFeature("http://xml.org/sax/features/namespace-prefixes", true);
	QString errorMsg = "";
	int errorLine = 0;
	int errorColumn = 0;
	if (!designMapDom.setContent(&xmlSource, &xmlReader, &errorMsg, &errorLine, &errorColumn))
	{
		qDebug() << "Error loading File" << errorMsg << "at Line" << errorLine << "Column" << errorColumn;
		return;
	}
	QDomElement docElem = designMapDom.documentElement();
	for(QDomElement drawPag = docElem.firstChildElement(); !drawPag.isNull(); drawPag = drawPag.nextSiblingElement())
	{
		if (drawPag.tagName() == "a:themeElements")
		{
			for(QDomElement spf = drawPag.firstChildElement(); !spf.isNull(); spf = spf.nextSiblingElement() )
			{
				if (spf.tagName() == "a:fontScheme")
				{
					for(QDomElement spr = spf.firstChildElement(); !spr.isNull(); spr = spr.nextSiblingElement() )
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
	}
}

void DocXIm::parseStyles()
{
	QByteArray f;
	QDomDocument designMapDom;
	if (!uz->read(stylePart, f))
		return;
	QXmlInputSource xmlSource;
	xmlSource.setData(f);
	QXmlSimpleReader xmlReader;
	xmlReader.setFeature("http://xml.org/sax/features/namespace-prefixes", true);
	QString errorMsg = "";
	int errorLine = 0;
	int errorColumn = 0;
	if (!designMapDom.setContent(&xmlSource, &xmlReader, &errorMsg, &errorLine, &errorColumn))
	{
		qDebug() << "Error loading File" << errorMsg << "at Line" << errorLine << "Column" << errorColumn;
		return;
	}
	defaultParagraphStyle.setParent(CommonStrings::DefaultParagraphStyle);
	defaultParagraphStyle.charStyle().setParent(CommonStrings::DefaultCharacterStyle);
	currentParagraphStyle.setParent(CommonStrings::DefaultParagraphStyle);
	currentParagraphStyle.charStyle().setParent(CommonStrings::DefaultCharacterStyle);
	currentParagraphStyle.setLineSpacingMode(ParagraphStyle::AutomaticLineSpacing);
	QDomElement docElem = designMapDom.documentElement();
	for(QDomElement drawPag = docElem.firstChildElement(); !drawPag.isNull(); drawPag = drawPag.nextSiblingElement())
	{
		if (drawPag.tagName() == "w:docDefaults")
		{
			for(QDomElement spf = drawPag.firstChildElement(); !spf.isNull(); spf = spf.nextSiblingElement() )
			{
				if (spf.tagName() == "w:rPrDefault")
				{
					for(QDomElement spr = spf.firstChildElement(); !spr.isNull(); spr = spr.nextSiblingElement() )
					{
						if (spr.tagName() == "w:rPr")
							parseCharProps(spr, defaultParagraphStyle);
					}
				}
				else if (spf.tagName() == "w:pPrDefault")
				{
					for(QDomElement spr = spf.firstChildElement(); !spr.isNull(); spr = spr.nextSiblingElement() )
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
						map_ID_to_Name.insert(drawPag.attribute("w:styleId"), m_item->itemName() + "_" + nam.attribute("w:val"));
					else
						map_ID_to_Name.insert(drawPag.attribute("w:styleId"), nam.attribute("w:val"));
					ParagraphStyle newStyle;
					newStyle = defaultParagraphStyle;
					if (m_prefixName)
						newStyle.setName(m_item->itemName() + "_" + nam.attribute("w:val"));
					else
						newStyle.setName(nam.attribute("w:val"));
					for(QDomElement spf = drawPag.firstChildElement(); !spf.isNull(); spf = spf.nextSiblingElement() )
					{
						if (spf.tagName() == "w:basedOn")
						{
							QString parentN = spf.attribute("w:val");
							if (map_ID_to_Name.contains(parentN))
							{
								parentN = map_ID_to_Name[parentN];
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
		}
	}
}

void DocXIm::parseStyledText(PageItem *textItem)
{
	QByteArray f;
	QDomDocument designMapDom;
	if (!uz->read(docPart, f))
		return;
	QXmlInputSource xmlSource;
	xmlSource.setData(f);
	QXmlSimpleReader xmlReader;
	xmlReader.setFeature("http://xml.org/sax/features/namespace-prefixes", true);
	QString errorMsg = "";
	int errorLine = 0;
	int errorColumn = 0;
	if (!designMapDom.setContent(&xmlSource, &xmlReader, &errorMsg, &errorLine, &errorColumn))
	{
		qDebug() << "Error loading File" << errorMsg << "at Line" << errorLine << "Column" << errorColumn;
		return;
	}
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
	for(QDomElement drawPag = docElem.firstChildElement(); !drawPag.isNull(); drawPag = drawPag.nextSiblingElement())
	{
		if (drawPag.tagName() == "w:body")
		{
			for(QDomElement spf = drawPag.firstChildElement(); !spf.isNull(); spf = spf.nextSiblingElement() )
			{
				if (spf.tagName() == "w:p")
				{
					currentParagraphStyle = defaultParagraphStyle;
					bool hasStyle = false;
					QString currStyleName = "";
					for(QDomElement spr = spf.firstChildElement(); !spr.isNull(); spr = spr.nextSiblingElement() )
					{
						if (spr.tagName() == "w:pPr")
						{
							QDomElement sty = spr.firstChildElement("w:pStyle");
							if (!sty.isNull())
							{
								QString nam = sty.attribute("w:val");
								if (map_ID_to_Name.contains(nam))
								{
									ParagraphStyle newStyle;
									newStyle.setParent(map_ID_to_Name[nam]);
									currentParagraphStyle = newStyle;
									hasStyle = true;
									currStyleName = map_ID_to_Name[nam];
									currentParagraphStyle.charStyle() = newStyle.charStyle();
								}
							}
							parseParaProps(spr, currentParagraphStyle);
						}
						else if (spr.tagName() == "w:r")
						{
							if (hasStyle)
								currentParagraphStyle.charStyle() = m_Doc->paragraphStyle(currStyleName).charStyle();
							else
								currentParagraphStyle.charStyle() = defaultParagraphStyle.charStyle();
							for(QDomElement spt = spr.firstChildElement(); !spt.isNull(); spt = spt.nextSiblingElement() )
							{
								if (spt.tagName() == "w:t")
								{
									QString m_txt = spt.text();
									if (m_txt.count() > 0)
									{
										m_txt.replace(QChar(10), SpecialChars::LINEBREAK);
										m_txt.replace(QChar(12), SpecialChars::FRAMEBREAK);
										m_txt.replace(QChar(30), SpecialChars::NBHYPHEN);
										m_txt.replace(QChar(160), SpecialChars::NBSPACE);
										int posT = textItem->itemText.length();
										textItem->itemText.insertChars(posT, m_txt);
										textItem->itemText.applyStyle(posT, currentParagraphStyle);
										textItem->itemText.applyCharStyle(posT, m_txt.length(), currentParagraphStyle.charStyle());
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
	for(QDomElement spt = props.firstChildElement(); !spt.isNull(); spt = spt.nextSiblingElement() )
	{
		if (spt.tagName() == "w:rPr")
		{
			parseCharProps(spt, pStyle);
		}
		else if (spt.tagName() == "w:jc")
		{
			QString align = spt.attribute("w:val");
			if (align == "start")
				pStyle.setAlignment(ParagraphStyle::Leftaligned);
			else if (align == "center")
				pStyle.setAlignment(ParagraphStyle::Centered);
			else if (align == "end")
				pStyle.setAlignment(ParagraphStyle::Rightaligned);
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
				colour.setNamedColor("#" + color);
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

void DocXIm::parseCharProps(QDomElement &props, ParagraphStyle &pStyle)
{
	for(QDomElement spc = props.firstChildElement(); !spc.isNull(); spc = spc.nextSiblingElement() )
	{
		if (spc.tagName() == "w:u")
		{
			StyleFlag styleEffects;
			styleEffects |= ScStyle_Underline;
			pStyle.charStyle().setFeatures(styleEffects.featureList());
		}
		else if (spc.tagName() == "w:sz")
			pStyle.charStyle().setFontSize(spc.attribute("w:val").toDouble() / 2.0 * 10.0);
		else if (spc.tagName() == "w:rFonts")
		{
			QString font = spc.attribute("w:ascii");
			if (!font.isEmpty())
			{
				font = getFontName(font);
				pStyle.charStyle().setFont(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts[font]);
			}
			else
			{
				QString fonta = spc.attribute("w:asciiTheme");
				if (!fonta.isEmpty())
				{
					if (fonta == "minorHAnsi")
					{
						fonta = getFontName(themeFont1);
						pStyle.charStyle().setFont(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts[fonta]);
					}
					else if (fonta == "majorHAnsi")
					{
						fonta = getFontName(themeFont2);
						pStyle.charStyle().setFont(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts[fonta]);
					}
				}
			}
		}
		else if (spc.tagName() == "w:caps")
		{
			StyleFlag styleEffects = pStyle.charStyle().effects();
			if (spc.hasAttribute("w:val"))
			{
				if (spc.attribute("w:val") == "true")
					styleEffects |= ScStyle_AllCaps;
				else
					styleEffects &= ~ScStyle_AllCaps;
			}
			else
				styleEffects |= ScStyle_AllCaps;
			pStyle.charStyle().setFeatures(styleEffects.featureList());
		}
		else if (spc.tagName() == "w:smallCaps")
		{
			StyleFlag styleEffects = pStyle.charStyle().effects();
			if (spc.hasAttribute("w:val"))
			{
				if (spc.attribute("w:val") == "true")
					styleEffects |= ScStyle_SmallCaps;
				else
					styleEffects &= ~ScStyle_SmallCaps;
			}
			else
				styleEffects |= ScStyle_SmallCaps;
			pStyle.charStyle().setFeatures(styleEffects.featureList());
		}
		else if (spc.tagName() == "w:strike")
		{
			StyleFlag styleEffects = pStyle.charStyle().effects();
			if (spc.hasAttribute("w:val"))
			{
				if (spc.attribute("w:val") == "true")
					styleEffects |= ScStyle_Strikethrough;
				else
					styleEffects &= ~ScStyle_Strikethrough;
			}
			else
				styleEffects |= ScStyle_Strikethrough;
			pStyle.charStyle().setFeatures(styleEffects.featureList());
		}
		else if (spc.tagName() == "w:shadow")
		{
			StyleFlag styleEffects = pStyle.charStyle().effects();
			if (spc.hasAttribute("w:val"))
			{
				if (spc.attribute("w:val") == "true")
					styleEffects |= ScStyle_Shadowed;
				else
					styleEffects &= ~ScStyle_Shadowed;
			}
			else
				styleEffects |= ScStyle_Shadowed;
			pStyle.charStyle().setFeatures(styleEffects.featureList());
		}
		else if (spc.tagName() == "w:outline")
		{
			StyleFlag styleEffects = pStyle.charStyle().effects();
			if (spc.hasAttribute("w:val"))
			{
				if (spc.attribute("w:val") == "true")
					styleEffects |= ScStyle_Outline;
				else
					styleEffects &= ~ScStyle_Outline;
			}
			else
				styleEffects |= ScStyle_Outline;
			pStyle.charStyle().setFeatures(styleEffects.featureList());
		}
		else if (spc.tagName() == "w:color")
		{
			if (spc.hasAttribute("w:val"))
			{
				QString color = spc.attribute("w:val");
				QColor colour;
				colour.setNamedColor("#" + color);
				ScColor tmp;
				tmp.fromQColor(colour);
				tmp.setSpotColor(false);
				tmp.setRegistrationColor(false);
				QString fNam = m_Doc->PageColors.tryAddColor("FromDocX"+colour.name(), tmp);
				pStyle.charStyle().setFillColor(fNam);
			}
		}
		else if (spc.tagName() == "w:shd")
		{
			if (spc.hasAttribute("w:fill"))
			{
				QString color = spc.attribute("w:fill");
				QColor colour;
				colour.setNamedColor("#" + color);
				ScColor tmp;
				tmp.fromQColor(colour);
				tmp.setSpotColor(false);
				tmp.setRegistrationColor(false);
				QString fNam = m_Doc->PageColors.tryAddColor("FromDocX"+colour.name(), tmp);
				pStyle.charStyle().setBackColor(fNam);
			}
		}
		else if (spc.tagName() == "w:vertAlign")
		{
			StyleFlag styleEffects = pStyle.charStyle().effects();
			if (spc.hasAttribute("w:val"))
			{
				if (spc.attribute("w:val") == "superscript")
					styleEffects |= ScStyle_Superscript;
				else if (spc.attribute("w:val") == "subscript")
					styleEffects |= ScStyle_Subscript;
				pStyle.charStyle().setFeatures(styleEffects.featureList());
			}
		}
	}
}

void DocXIm::parsePlainTextOnly(PageItem *textItem)
{
	QByteArray f;
	QDomDocument designMapDom;
	if (!uz->read(docPart, f))
		return;
	QXmlInputSource xmlSource;
	xmlSource.setData(f);
	QXmlSimpleReader xmlReader;
	xmlReader.setFeature("http://xml.org/sax/features/namespace-prefixes", true);
	QString errorMsg = "";
	int errorLine = 0;
	int errorColumn = 0;
	if (!designMapDom.setContent(&xmlSource, &xmlReader, &errorMsg, &errorLine, &errorColumn))
	{
		qDebug() << "Error loading File" << errorMsg << "at Line" << errorLine << "Column" << errorColumn;
		return;
	}
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
	for(QDomElement drawPag = docElem.firstChildElement(); !drawPag.isNull(); drawPag = drawPag.nextSiblingElement())
	{
		if (drawPag.tagName() == "w:body")
		{
			for(QDomElement spf = drawPag.firstChildElement(); !spf.isNull(); spf = spf.nextSiblingElement() )
			{
				if (spf.tagName() == "w:p")
				{
					for(QDomElement spr = spf.firstChildElement(); !spr.isNull(); spr = spr.nextSiblingElement() )
					{
						if (spr.tagName() == "w:r")
						{
							for(QDomElement spt = spr.firstChildElement(); !spt.isNull(); spt = spt.nextSiblingElement() )
							{
								if (spt.tagName() == "w:t")
								{
									QString m_txt = spt.text();
									if (m_txt.count() > 0)
									{
										m_txt.replace(QChar(10), SpecialChars::LINEBREAK);
										m_txt.replace(QChar(12), SpecialChars::FRAMEBREAK);
										m_txt.replace(QChar(30), SpecialChars::NBHYPHEN);
										m_txt.replace(QChar(160), SpecialChars::NBSPACE);
										textItem->itemText.insertChars(textItem->itemText.length(), m_txt);
										textItem->itemText.applyStyle(textItem->itemText.length(), currentParagraphStyle);
										textItem->itemText.applyCharStyle(textItem->itemText.length(), m_txt.length(), currentParagraphStyle.charStyle());
									}
								}
								else if (spt.tagName() == "w:tab")
								{
									int posT = textItem->itemText.length();
									textItem->itemText.insertChars(posT, SpecialChars::TAB);
									textItem->itemText.applyStyle(posT, currentParagraphStyle);
								}
							}
						}
					}
					textItem->itemText.insertChars(textItem->itemText.length(), SpecialChars::PARSEP);
					textItem->itemText.applyStyle(textItem->itemText.length(), currentParagraphStyle);
				}
			}
		}
	}
}

QString DocXIm::getFontName(QString name)
{
	QString fontName = name;
	SCFontsIterator it(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts);
	for ( ; it.hasNext(); it.next())
	{
		if (it.current().family().toLower() == fontName.toLower())
		{
			if (it.currentKey().toLower() == fontName.toLower()) // exact Match
			{
				return fontName;
			}
			else
			{
				QStringList slist = PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts.fontMap[it.current().family()];
				if (slist.count() > 0)
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
	}

	if (!PrefsManager::instance()->appPrefs.fontPrefs.GFontSub.contains(fontName))
	{
		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
		MissingFont dia(0, fontName, m_Doc);
		static_cast<void>(dia.exec());
		qApp->changeOverrideCursor(QCursor(Qt::WaitCursor));
		PrefsManager::instance()->appPrefs.fontPrefs.GFontSub[fontName] = dia.getReplacementFont();
		fontName = dia.getReplacementFont();
	}
	else
		fontName = PrefsManager::instance()->appPrefs.fontPrefs.GFontSub[fontName];

	return fontName;
}

double DocXIm::pixelsFromTwips(double twips)
{
	return twips / 1440.0 * 72.0;
}

DocXIm::~DocXIm()
{
}
