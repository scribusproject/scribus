/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
							 -------------------
	begin                : Sun Jan  3 2016
	copyright            : (C) 2016 by Franz Schmid
	email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/
#include "importodt.h"

#include <QApplication>
#include <QByteArray>
#include <QScopedPointer>
#include <QXmlInputSource>
#include <QXmlSimpleReader>
#include "color.h"
#include "scribusdoc.h"
#include "styles/charstyle.h"
#include "styles/paragraphstyle.h"
#include "third_party/zip/scribus_zip.h"
#include "prefsmanager.h"
#include "scclocale.h"
#include "ui/missing.h"
#include "util.h"


QString FileFormatName()
{
    return QObject::tr("ODT Document");
}

QStringList FileExtensions()
{
	QStringList ret;
	ret.append("odt");
	ret.append("fodt");
	return ret;
}

void GetText2(QString filename, QString encoding, bool textOnly, bool prefix, bool append, PageItem *textItem)
{
	ODTIm* docxim = new ODTIm(filename, textItem, textOnly, prefix, append);
	delete docxim;
}

ODTIm::ODTIm(QString fileName, PageItem *textItem, bool textOnly, bool prefix, bool append)
{
	uz=NULL;
	m_Doc = textItem->doc();
	m_item = textItem;
	m_prefixName = prefix;
	m_append = append;
	QFileInfo fi = QFileInfo(fileName);
	QString ext = fi.suffix().toLower();
	if (ext == "fodt")
	{
		QByteArray f;
		loadRawText(fileName, f);
		QDomDocument designMapDom;
		QString errorMsg;
		int errorLine = 0;
		int errorColumn = 0;
		if (!designMapDom.setContent(f, &errorMsg, &errorLine, &errorColumn))
		{
			qDebug() << "Error loading File" << errorMsg << "at Line" << errorLine << "Column" << errorColumn;
			return;
		}
		if (textOnly)
			parseRawDocReferenceXML(designMapDom);
		else
			parseDocReferenceXML(designMapDom);
	}
	else
	{
		uz = new ScZipHandler();
		if (uz)
		{
			if (!uz->open(fileName))
			{
				delete uz;
				QByteArray f;
				loadRawText(fileName, f);
				QDomDocument designMapDom;
				QString errorMsg;
				int errorLine = 0;
				int errorColumn = 0;
				if (designMapDom.setContent(f, &errorMsg, &errorLine, &errorColumn))
				{
					if (textOnly)
						parseRawDocReferenceXML(designMapDom);
					else
						parseDocReferenceXML(designMapDom);
				}
				else
				{
					qDebug() << "Error loading File" << errorMsg << "at Line" << errorLine << "Column" << errorColumn;
					return;
				}
			}
			else
			{
				if (textOnly)
				{
					if (uz->contains("content.xml"))
						parseRawDocReference("content.xml");
				}
				else
				{
					if (uz->contains("styles.xml"))
					{
						if (parseStyleSheets("styles.xml"))
						{
							if (uz->contains("content.xml"))
								parseDocReference("content.xml");
						}
					}
					else
					{
						if (uz->contains("content.xml"))
							parseDocReference("content.xml");
					}
				}
				uz->close();
				delete uz;
			}
		}
	}
	textItem->itemText.trim();
	textItem->itemText.invalidateLayout();
}

ODTIm::~ODTIm()
{
}

/* Raw Text import */

bool ODTIm::parseRawDocReference(QString designMap)
{
	QByteArray f;
	QDomDocument designMapDom;
	if (!uz->read(designMap, f))
		return false;
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
		return false;
	}
	return parseRawDocReferenceXML(designMapDom);
}

bool ODTIm::parseRawDocReferenceXML(QDomDocument &designMapDom)
{
	QDomElement docElem = designMapDom.documentElement();
	for (QDomElement drawPag = docElem.firstChildElement(); !drawPag.isNull(); drawPag = drawPag.nextSiblingElement())
	{
		if (drawPag.tagName() == "office:body")
		{
			for (QDomElement sp = drawPag.firstChildElement(); !sp.isNull(); sp = sp.nextSiblingElement() )
			{
				if (sp.tagName() == "office:text")
				{
					parseRawText(sp, m_item);
				}
			}
		}
	}
	return true;
}

void ODTIm::parseRawTextSpan(QDomElement &elem, PageItem* item, ParagraphStyle &tmpStyle, CharStyle &tmpCStyle, int &posC)
{
	if (!elem.hasChildNodes())
		return;

	for (QDomNode spn = elem.firstChild(); !spn.isNull(); spn = spn.nextSibling())
	{
		QString txt = "";
		QDomElement spEl = spn.toElement();
		if (spn.nodeName() == "#text")
			txt = spn.nodeValue();
		else if (spn.nodeName() == "text:span")
			parseRawTextSpan(spEl, item, tmpStyle, tmpCStyle, posC);
		else if (spn.nodeName() == "text:s")
		{
			if (spEl.hasAttribute("text:c"))
			{
				int n = spEl.attribute("text:c").toInt();
				for (int nn = 0; nn < n; nn++)
				{
					txt += " ";
				}
			}
			else
				txt = " ";
		}
		else if (spn.nodeName() == "text:tab")
			txt = SpecialChars::TAB;
		else if (spn.nodeName() == "text:line-break")
			txt = SpecialChars::LINEBREAK;
		if (!txt.isEmpty())
		{
			txt.replace(QChar(0xAD), SpecialChars::SHYPHEN);
			txt.replace(QChar(0x2011), SpecialChars::NBHYPHEN);
			txt.replace(QChar(0xA0), SpecialChars::NBSPACE);
			insertChars(item, txt, tmpStyle, tmpCStyle, posC);
		}
	}
}

void ODTIm::parseRawTextParagraph(QDomNode &elem, PageItem* item, ParagraphStyle &newStyle, int &posC)
{
	CharStyle tmpCStyle = newStyle.charStyle();
	if (elem.hasChildNodes())
	{
		for (QDomNode spn = elem.firstChild(); !spn.isNull(); spn = spn.nextSibling())
		{
			QString txt = "";
			QDomElement spEl = spn.toElement();
			if (spn.nodeName() == "#text")
				txt = spn.nodeValue();
			else if (spn.nodeName() == "text:span")
				parseRawTextSpan(spEl, item, newStyle, tmpCStyle, posC);
			else if (spn.nodeName() == "text:s")
			{
				if (spEl.hasAttribute("text:c"))
				{
					int n = spEl.attribute("text:c").toInt();
					for (int nn = 0; nn < n; nn++)
					{
						txt += " ";
					}
				}
				else
					txt = " ";
			}
			else if (spn.nodeName() == "text:tab")
				txt = SpecialChars::TAB;
			else if (spn.nodeName() == "text:line-break")
				txt = SpecialChars::LINEBREAK;
			if (!txt.isEmpty())
			{
				txt.replace(QChar(0xAD), SpecialChars::SHYPHEN);
				txt.replace(QChar(0x2011), SpecialChars::NBHYPHEN);
				txt.replace(QChar(0xA0), SpecialChars::NBSPACE);
				insertChars(item, txt, newStyle, tmpCStyle, posC);
			}
		}
	}
	item->itemText.insertChars(posC, SpecialChars::PARSEP);
	item->itemText.applyStyle(posC, newStyle);
	posC = item->itemText.length();
}

void ODTIm::parseRawText(QDomElement &elem, PageItem* item)
{
	QString pStyleD = CommonStrings::DefaultParagraphStyle;
	ParagraphStyle newStyle;
	newStyle.setDefaultStyle(false);
	newStyle.setParent(pStyleD);
	if (!m_append)
	{
		item->itemText.clear();
		item->itemText.setDefaultStyle(newStyle);
	}
	int posC = item->itemText.length();
	for (QDomNode para = elem.firstChild(); !para.isNull(); para = para.nextSibling())
	{
		if ((para.nodeName() == "text:p") || (para.nodeName() == "text:h"))
			parseRawTextParagraph(para, item, newStyle, posC);
		else if (para.nodeName() == "text:list")
		{
			if (!para.hasChildNodes())
				continue;
			for (QDomNode spn = para.firstChild(); !spn.isNull(); spn = spn.nextSibling())
			{
				if (spn.nodeName() == "text:list-item")
				{
					if (!spn.hasChildNodes())
						continue;
					for(QDomNode spp = spn.firstChild(); !spp.isNull(); spp = spp.nextSibling())
					{
						if (spp.nodeName() == "text:p")
						{
							parseRawTextParagraph(spp, item, newStyle, posC);
						}
					}
				}
			}
		}
		else if (para.nodeName() == "text:section")
		{
			if (!para.hasChildNodes())
				continue;
			for (QDomNode spn = para.firstChild(); !spn.isNull(); spn = spn.nextSibling())
			{
				if (spn.nodeName() == "text:p")
				{
					parseRawTextParagraph(spn, item, newStyle, posC);
				}
			}
		}
	}
}

/* Styled Text import */

bool ODTIm::parseStyleSheets(QString designMap)
{
	QByteArray f;
	QDomDocument designMapDom;
	if (!uz->read(designMap, f))
		return false;
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
		return false;
	}
	return parseStyleSheetsXML(designMapDom);
}

bool ODTIm::parseStyleSheetsXML(QDomDocument &designMapDom)
{
	QDomElement docElem = designMapDom.documentElement();
	for (QDomElement sp = docElem.firstChildElement(); !sp.isNull(); sp = sp.nextSiblingElement() )
	{
		if (sp.tagName() == "office:font-face-decls")
		{
			for (QDomElement spf = sp.firstChildElement(); !spf.isNull(); spf = spf.nextSiblingElement() )
			{
				if (spf.tagName() == "style:font-face")
				{
					if (!spf.attribute("style:name").isEmpty())
					{
						QString fontFamily = spf.attribute("svg:font-family");
						if (fontFamily.startsWith(QChar('\'')))
							fontFamily = fontFamily.mid(1);
						if (fontFamily.endsWith(QChar('\'')))
							fontFamily.chop(1);
						m_fontMap.insert(spf.attribute("style:name"), fontFamily);
					}
				}
			}
		}
		else if (sp.tagName() == "office:styles")
			parseStyles(sp, "styles");
		else if (sp.tagName() == "office:automatic-styles")
			parseStyles(sp, "auto");
	}
	return true;
}

void ODTIm::parseStyles(QDomElement &sp, QString type)
{
	for (QDomElement spd = sp.firstChildElement(); !spd.isNull(); spd = spd.nextSiblingElement() )
	{
		if (spd.tagName() == "style:default-style")
		{
			DrawStyle currStyle;
			currStyle.styleOrigin = AttributeValue(type);
			for (QDomElement spe = spd.firstChildElement(); !spe.isNull(); spe = spe.nextSiblingElement() )
			{
				if (spe.tagName() == "style:paragraph-properties")
				{
					currStyle.margin_top = AttributeValue(spe.attribute("fo:margin-top", ""));
					currStyle.margin_bottom = AttributeValue(spe.attribute("fo:margin-bottom", ""));
					currStyle.margin_left = AttributeValue(spe.attribute("fo:margin-left", ""));
					currStyle.margin_right = AttributeValue(spe.attribute("fo:margin-right", ""));
					currStyle.textIndent = AttributeValue(spe.attribute("fo:text-indent", ""));
					currStyle.textAlign = AttributeValue(spe.attribute("fo:text-align", ""));
					currStyle.lineHeight = AttributeValue(spe.attribute("fo:line-height", ""));
					currStyle.parBackgroundColor = AttributeValue(spe.attribute("fo:background-color", ""));
					currStyle.breakAfter = AttributeValue(spe.attribute("fo:break-after", ""));
					currStyle.breakBefore = AttributeValue(spe.attribute("fo:break-before", ""));
					if (spe.hasChildNodes())
					{
						for (QDomElement spt = spe.firstChildElement(); !spt.isNull(); spt = spt.nextSiblingElement() )
						{
							if (spt.tagName() == "style:tab-stops")
							{
								QString tabDists = "";
								QString tabTypes = "";
								for (QDomElement spte = spt.firstChildElement(); !spte.isNull(); spte = spte.nextSiblingElement() )
								{
									if (spte.tagName() == "style:tab-stop")
									{
										if (spte.hasAttribute("style:position"))
											tabDists.append(spte.attribute("style:position") + ";");
										else
											tabDists.append("0;");
										if (spte.hasAttribute("style:type"))
											tabTypes.append(spte.attribute("style:type") + " ");
										else
											tabTypes.append("left ");
									}
								}
								currStyle.tabDists = AttributeValue(tabDists);
								currStyle.tabTypes = AttributeValue(tabTypes);
							}
						}
					}
				}
				else if (spe.tagName() == "style:text-properties")
				{
					currStyle.fontName = AttributeValue(spe.attribute("style:font-name", ""));
					if (!currStyle.fontName.valid)
						currStyle.fontName = AttributeValue(spe.attribute("fo:font-family", ""));
					currStyle.fontSize = AttributeValue(spe.attribute("fo:font-size", ""));
					currStyle.fontColor = AttributeValue(spe.attribute("fo:color", ""));
					currStyle.fontStyle = AttributeValue(spe.attribute("fo:font-style", ""));
					currStyle.fontWeight = AttributeValue(spe.attribute("fo:font-weight", ""));
					currStyle.textBackgroundColor = AttributeValue(spe.attribute("fo:background-color", ""));
					currStyle.textPos = AttributeValue(spe.attribute("style:text-position", ""));
					currStyle.textOutline = AttributeValue(spe.attribute("style:text-outline", ""));
					currStyle.textUnderline = AttributeValue(spe.attribute("style:text-underline-style", ""));
					currStyle.textUnderlineWords = AttributeValue(spe.attribute("style:text-underline-mode", ""));
					currStyle.textUnderlineColor = AttributeValue(spe.attribute("style:text-underline-color", ""));
					currStyle.textStrikeThrough = AttributeValue(spe.attribute("style:text-line-through-style", ""));
					currStyle.textShadow = AttributeValue(spe.attribute("fo:text-shadow", ""));
					currStyle.fontVariant = AttributeValue(spe.attribute("fo:font-variant", ""));
				}
			}
			if (spd.attribute("style:family") == "paragraph")
				parDefaultStyle = currStyle;
			else if (spd.attribute("style:family") == "text")
				txtDefaultStyle = currStyle;
		}
		else if (spd.tagName() == "style:style")
		{
			DrawStyle currStyle;
			currStyle.parentStyle = AttributeValue(spd.attribute("style:parent-style-name", ""));
			if (!currStyle.parentStyle.valid)
			{
				if (spd.attribute("style:family") == "paragraph")
					currStyle = parDefaultStyle;
				else if (spd.attribute("style:family") == "text")
					currStyle = txtDefaultStyle;
			}
			currStyle.styleType = AttributeValue(spd.attribute("style:family", ""));
			currStyle.styleOrigin = AttributeValue(type);
			for (QDomElement spe = spd.firstChildElement(); !spe.isNull(); spe = spe.nextSiblingElement() )
			{
				if (spe.tagName() == "style:paragraph-properties")
				{
					currStyle.margin_top = AttributeValue(spe.attribute("fo:margin-top", ""));
					currStyle.margin_bottom = AttributeValue(spe.attribute("fo:margin-bottom", ""));
					currStyle.margin_left = AttributeValue(spe.attribute("fo:margin-left", ""));
					currStyle.margin_right = AttributeValue(spe.attribute("fo:margin-right", ""));
					currStyle.textIndent = AttributeValue(spe.attribute("fo:text-indent", ""));
					currStyle.textAlign = AttributeValue(spe.attribute("fo:text-align", ""));
					currStyle.lineHeight = AttributeValue(spe.attribute("fo:line-height", ""));
					currStyle.parBackgroundColor = AttributeValue(spe.attribute("fo:background-color", ""));
					currStyle.breakAfter = AttributeValue(spe.attribute("fo:break-after", ""));
					currStyle.breakBefore = AttributeValue(spe.attribute("fo:break-before", ""));
					if (spe.hasChildNodes())
					{
						for (QDomElement spt = spe.firstChildElement(); !spt.isNull(); spt = spt.nextSiblingElement() )
						{
							if (spt.tagName() == "style:tab-stops")
							{
								QString tabDists = "";
								QString tabTypes = "";
								for (QDomElement spte = spt.firstChildElement(); !spte.isNull(); spte = spte.nextSiblingElement() )
								{
									if (spte.tagName() == "style:tab-stop")
									{
										if (spte.hasAttribute("style:position"))
											tabDists.append(spte.attribute("style:position") + ";");
										else
											tabDists.append("0;");
										if (spte.hasAttribute("style:type"))
											tabTypes.append(spte.attribute("style:type") + " ");
										else
											tabTypes.append("left ");
									}
								}
								currStyle.tabDists = AttributeValue(tabDists);
								currStyle.tabTypes = AttributeValue(tabTypes);
							}
						}
					}
				}
				else if (spe.tagName() == "style:text-properties")
				{
					currStyle.fontName = AttributeValue(spe.attribute("style:font-name", ""));
					if (!currStyle.fontName.valid)
						currStyle.fontName = AttributeValue(spe.attribute("fo:font-family", ""));
					currStyle.fontSize = AttributeValue(spe.attribute("fo:font-size", ""));
					currStyle.fontColor = AttributeValue(spe.attribute("fo:color", ""));
					currStyle.fontStyle = AttributeValue(spe.attribute("fo:font-style", ""));
					currStyle.fontWeight = AttributeValue(spe.attribute("fo:font-weight", ""));
					currStyle.textBackgroundColor = AttributeValue(spe.attribute("fo:background-color", ""));
					currStyle.textPos = AttributeValue(spe.attribute("style:text-position", ""));
					currStyle.textOutline = AttributeValue(spe.attribute("style:text-outline", ""));
					currStyle.textUnderline = AttributeValue(spe.attribute("style:text-underline-style", ""));
					currStyle.textUnderlineWords = AttributeValue(spe.attribute("style:text-underline-mode", ""));
					currStyle.textUnderlineColor = AttributeValue(spe.attribute("style:text-underline-color", ""));
					currStyle.textStrikeThrough = AttributeValue(spe.attribute("style:text-line-through-style", ""));
					currStyle.textShadow = AttributeValue(spe.attribute("fo:text-shadow", ""));
					currStyle.fontVariant = AttributeValue(spe.attribute("fo:font-variant", ""));
				}
			}
			currStyle.displayName = AttributeValue(spd.attribute("style:display-name", ""));
			m_Styles.insert(spd.attribute("style:name"), currStyle);
			if (type == "styles")
			{
				ObjStyleODT tmpOStyle;
				resolveStyle(tmpOStyle, spd.attribute("style:name"));
				if (spd.attribute("style:family") == "paragraph")
				{
					ParagraphStyle newStyle;
					newStyle.erase();
					newStyle.setDefaultStyle(false);
					newStyle.setLineSpacingMode(ParagraphStyle::AutomaticLineSpacing);
					QString styleName = spd.attribute("style:name");
					if (currStyle.displayName.valid)
						styleName = currStyle.displayName.value;
					if (m_prefixName)
						newStyle.setName(m_item->itemName() + "_" + styleName);
					else
						newStyle.setName(styleName);
					QString parentName = CommonStrings::DefaultParagraphStyle;
					if (currStyle.parentStyle.valid)
					{
						if (m_Styles.contains(currStyle.parentStyle.value))
						{
							DrawStyle pStyle = m_Styles[currStyle.parentStyle.value];
							parentName = currStyle.parentStyle.value;
							if (pStyle.displayName.valid)
								parentName = pStyle.displayName.value;
						}
					}
					if (m_prefixName)
						newStyle.setParent(m_item->itemName() + "_" + parentName);
					else
						newStyle.setParent(parentName);
					applyParagraphStyle(newStyle, tmpOStyle);
					applyCharacterStyle(newStyle.charStyle(), tmpOStyle);
					StyleSet<ParagraphStyle>tmp;
					tmp.create(newStyle);
					m_Doc->redefineStyles(tmp, false);
				}
				else if (spd.attribute("style:family") == "text")
				{
					CharStyle newStyle;
					newStyle.setDefaultStyle(false);
					QString styleName = spd.attribute("style:name");
					if (currStyle.displayName.valid)
						styleName = currStyle.displayName.value;
					if (m_prefixName)
						newStyle.setName(m_item->itemName() + "_" + styleName);
					else
						newStyle.setName(styleName);
					newStyle.setParent(CommonStrings::DefaultCharacterStyle);
					applyCharacterStyle(newStyle, tmpOStyle);
					StyleSet<CharStyle> temp;
					temp.create(newStyle);
					m_Doc->redefineCharStyles(temp, false);
				}
			}
		}
	}
}

bool ODTIm::parseDocReference(QString designMap)
{
	QByteArray f;
	QDomDocument designMapDom;
	if (!uz->read(designMap, f))
		return false;
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
		return false;
	}
	return parseDocReferenceXML(designMapDom);
}

bool ODTIm::parseDocReferenceXML(QDomDocument &designMapDom)
{
	QDomElement docElem = designMapDom.documentElement();
	for (QDomElement drawPag = docElem.firstChildElement(); !drawPag.isNull(); drawPag = drawPag.nextSiblingElement())
	{
		if (drawPag.tagName() == "office:font-face-decls")
		{
			for (QDomElement spf = drawPag.firstChildElement(); !spf.isNull(); spf = spf.nextSiblingElement() )
			{
				if (spf.tagName() == "style:font-face")
				{
					if (!spf.attribute("style:name").isEmpty())
					{
						QString fontFamily = spf.attribute("svg:font-family");
						if (fontFamily.startsWith(QChar('\'')))
							fontFamily = fontFamily.mid(1);
						if (fontFamily.endsWith(QChar('\'')))
							fontFamily.chop(1);
						m_fontMap.insert(spf.attribute("style:name"), fontFamily);
					}
				}
			}
		}
		else if (drawPag.tagName() == "office:styles")
			parseStyles(drawPag, "styles");
		else if (drawPag.tagName() == "office:automatic-styles")
			parseStyles(drawPag, "auto");
		else if (drawPag.tagName() == "office:body")
		{
			for (QDomElement sp = drawPag.firstChildElement(); !sp.isNull(); sp = sp.nextSiblingElement() )
			{
				if (sp.tagName() == "office:text")
				{
					ObjStyleODT tmpOStyle;
					resolveStyle(tmpOStyle, "standard");
					parseText(sp, m_item, tmpOStyle);
				}
			}
		}
	}
	return true;
}

void ODTIm::parseTextSpan(QDomElement &elem, PageItem* item, ParagraphStyle &tmpStyle, CharStyle &tmpCStyle, ObjStyleODT &tmpOStyle, int &posC)
{
	ObjStyleODT odtStyle = tmpOStyle;
	CharStyle cStyle = tmpCStyle;

	QString textStyleName = elem.attribute("text:style-name");
	if (textStyleName.length() > 0)
	{
		resolveStyle(odtStyle, textStyleName);
		m_textStylesStack.push(textStyleName);
	}
	
	applyCharacterStyle(cStyle, odtStyle);
	if (!elem.hasChildNodes())
		return;

	for (QDomNode spn = elem.firstChild(); !spn.isNull(); spn = spn.nextSibling())
	{
		QString txt = "";
		QDomElement spEl = spn.toElement();
		if (spn.nodeName() == "#text")
			txt = spn.nodeValue();
		else if (spn.nodeName() == "text:span")
			parseTextSpan(spEl, item, tmpStyle, cStyle, odtStyle, posC);
		else if (spn.nodeName() == "text:s")
		{
			if (spEl.hasAttribute("text:c"))
			{
				int n = spEl.attribute("text:c").toInt();
				for (int nn = 0; nn < n; nn++)
				{
					txt += " ";
				}
			}
			else
				txt = " ";
		}
		else if (spn.nodeName() == "text:tab")
			txt = SpecialChars::TAB;
		else if (spn.nodeName() == "text:line-break")
			txt = SpecialChars::LINEBREAK;
		if (!txt.isEmpty())
		{
			txt.replace(QChar(0xAD), SpecialChars::SHYPHEN);
			txt.replace(QChar(0x2011), SpecialChars::NBHYPHEN);
			txt.replace(QChar(0xA0), SpecialChars::NBSPACE);
			insertChars(item, txt, tmpStyle, cStyle, posC);
		}
	}

	if (textStyleName.length() > 0)
		m_textStylesStack.pop();
}

void ODTIm::parseTextParagraph(QDomNode &elem, PageItem* item, ParagraphStyle &newStyle, ObjStyleODT &tmpOStyle, int &posC)
{
	ParagraphStyle tmpStyle = newStyle;
	CharStyle tmpCStyle = tmpStyle.charStyle();
	ObjStyleODT pStyle = tmpOStyle;
	QString parStyleName = "";

	QString pStyleName = elem.toElement().attribute("text:style-name");
	if (pStyleName.length() > 0)
	{
		resolveStyle(pStyle, pStyleName);
		if (m_Styles.contains(pStyleName))
		{
			DrawStyle currStyle = m_Styles[pStyleName];
			if (currStyle.styleOrigin.value == "styles")
			{
				if (m_prefixName)
				{
					parStyleName = m_item->itemName() + "_" + pStyleName;
					if (currStyle.displayName.valid)
						parStyleName = m_item->itemName() + "_" + currStyle.displayName.value;
				}
				else
				{
					parStyleName = pStyleName;
					if (currStyle.displayName.valid)
						parStyleName = currStyle.displayName.value;
				}
			}
		}
		m_textStylesStack.push(pStyleName);
	}
	if ((pStyle.breakBefore == "column") && (item->itemText.length() > 0))
	{
		QString txt = SpecialChars::COLBREAK;
		insertChars(item, txt, tmpStyle, tmpCStyle, posC);
	}
	else if ((pStyle.breakBefore == "page") && (item->itemText.length() > 0))
	{
		QString txt = SpecialChars::FRAMEBREAK;
		insertChars(item, txt, tmpStyle, tmpCStyle, posC);
	}
	applyParagraphStyle(tmpStyle, pStyle);
	if (elem.hasChildNodes())
	{
		for (QDomNode spn = elem.firstChild(); !spn.isNull(); spn = spn.nextSibling())
		{
			if (!parStyleName.isEmpty())
			{
				tmpStyle.setParent(parStyleName);
				applyParagraphStyle(tmpStyle, pStyle);
				tmpCStyle = tmpStyle.charStyle();
				applyCharacterStyle(tmpCStyle, pStyle);
			}
			else
			{
				tmpStyle = newStyle;
				applyParagraphStyle(tmpStyle, pStyle);
				tmpCStyle = tmpStyle.charStyle();
				applyCharacterStyle(tmpCStyle, pStyle);
			}
			QString txt = "";
			ObjStyleODT cStyle = pStyle;
			QDomElement spEl = spn.toElement();
			if (spn.nodeName() == "#text")
				txt = spn.nodeValue();
			else if (spn.nodeName() == "text:span")
				parseTextSpan(spEl, item, tmpStyle, tmpCStyle, cStyle, posC);
			else if (spn.nodeName() == "text:s")
			{
				if (spEl.hasAttribute("text:c"))
				{
					int n = spEl.attribute("text:c").toInt();
					for (int nn = 0; nn < n; nn++)
					{
						txt += " ";
					}
				}
				else
					txt = " ";
			}
			else if (spn.nodeName() == "text:tab")
				txt = SpecialChars::TAB;
			else if (spn.nodeName() == "text:line-break")
				txt = SpecialChars::LINEBREAK;
			if (!txt.isEmpty())
			{
				txt.replace(QChar(0xAD), SpecialChars::SHYPHEN);
				txt.replace(QChar(0x2011), SpecialChars::NBHYPHEN);
				txt.replace(QChar(0xA0), SpecialChars::NBSPACE);
				insertChars(item, txt, tmpStyle, tmpCStyle, posC);
			}
		}
	}
/*	if (pStyle.lineHeight < 0.0)
		tmpStyle.setLineSpacingMode(ParagraphStyle::AutomaticLineSpacing);
	else
	{
		tmpStyle.setLineSpacingMode(ParagraphStyle::FixedLineSpacing);
		if (pStyle.absLineHeight)
			tmpStyle.setLineSpacing(tmpOStyle.lineHeight);
		else
			tmpStyle.setLineSpacing(tmpOStyle.lineHeight * tmpOStyle.fontSize);
	} */
	if (pStyle.breakAfter == "column")
	{
		QString txt = SpecialChars::COLBREAK;
		insertChars(item, txt, tmpStyle, tmpCStyle, posC);
	}
	else if (pStyle.breakAfter == "page")
	{
		QString txt = SpecialChars::FRAMEBREAK;
		insertChars(item, txt, tmpStyle, tmpCStyle, posC);
	}
	item->itemText.insertChars(posC, SpecialChars::PARSEP);
	item->itemText.applyStyle(posC, tmpStyle);
	posC = item->itemText.length();

	if (pStyleName.length() > 0)
		m_textStylesStack.pop();
}

void ODTIm::parseText(QDomElement &elem, PageItem* item, ObjStyleODT &tmpOStyle)
{
	QString pStyleD = CommonStrings::DefaultParagraphStyle;
	ParagraphStyle newStyle;
	newStyle.setDefaultStyle(false);
	newStyle.setParent(pStyleD);
	ParagraphStyle ttx = m_Doc->paragraphStyle(pStyleD);
	CharStyle nstyle = ttx.charStyle();
	newStyle.setLineSpacingMode(ParagraphStyle::AutomaticLineSpacing);
	newStyle.setLineSpacing(nstyle.fontSize() / 10.0);
	if (!m_append)
	{
		item->itemText.clear();
		item->itemText.setDefaultStyle(newStyle);
		item->setFirstLineOffset(FLOPFontAscent);
	}
	int posC = item->itemText.length();
	for (QDomNode para = elem.firstChild(); !para.isNull(); para = para.nextSibling())
	{
		if ((para.nodeName() == "text:p") || (para.nodeName() == "text:h"))
			parseTextParagraph(para, item, newStyle, tmpOStyle, posC);
		else if (para.nodeName() == "text:list")
		{
			if (!para.hasChildNodes())
				continue;
			for (QDomNode spn = para.firstChild(); !spn.isNull(); spn = spn.nextSibling())
			{
				if (spn.nodeName() == "text:list-item")
				{
					if (!spn.hasChildNodes())
						continue;
					for(QDomNode spp = spn.firstChild(); !spp.isNull(); spp = spp.nextSibling())
					{
						if (spp.nodeName() == "text:p")
						{
							parseTextParagraph(spp, item, newStyle, tmpOStyle, posC);
						}
					}
				}
			}
		}
		else if (para.nodeName() == "text:section")
		{
			if (!para.hasChildNodes())
				continue;
			for (QDomNode spn = para.firstChild(); !spn.isNull(); spn = spn.nextSibling())
			{
				if (spn.nodeName() == "text:p")
				{
					parseTextParagraph(spn, item, newStyle, tmpOStyle, posC);
				}
			}
		}
	}
}

void ODTIm::insertChars(PageItem *item, QString &txt, ParagraphStyle &tmpStyle, CharStyle &tmpCStyle, int &posC)
{
	if (txt.length() > 0)
	{
		item->itemText.insertChars(posC, txt);
		item->itemText.applyStyle(posC, tmpStyle);
		item->itemText.applyCharStyle(posC, txt.length(), tmpCStyle);
		posC = item->itemText.length();
		txt = "";
	}
}

void ODTIm::applyCharacterStyle(CharStyle &tmpCStyle, ObjStyleODT &oStyle)
{
	tmpCStyle.setFont((*m_Doc->AllFonts)[oStyle.fontName]);
	tmpCStyle.setFontSize(oStyle.fontSize * 10);
	tmpCStyle.setFillColor(oStyle.CurrColorText);
	tmpCStyle.setBackColor(oStyle.CurrColorBText);
	StyleFlag styleEffects = tmpCStyle.effects();
	if ((oStyle.textPos.startsWith("super")) || (oStyle.textPos.startsWith("sub")))
	{
		if (oStyle.textPos.startsWith("super"))
			styleEffects |= ScStyle_Superscript;
		else
			styleEffects |= ScStyle_Subscript;
	}
	if (oStyle.textOutline == "true")
	{
		styleEffects |= ScStyle_Outline;
		tmpCStyle.setOutlineWidth(30);
		tmpCStyle.setFillColor("White");
		tmpCStyle.setStrokeColor(oStyle.CurrColorText);
	}
	if (oStyle.textUnderline)
	{
		styleEffects |= ScStyle_Underline;
		tmpCStyle.setUnderlineOffset(-1);
		tmpCStyle.setUnderlineWidth(-1);
		tmpCStyle.setStrokeColor(oStyle.textUnderlineColor);
	}
	if (oStyle.textStrikeThrough)
	{
		if (oStyle.textUnderlineWords)
			styleEffects |= ScStyle_UnderlineWords;
		else
			styleEffects |= ScStyle_Strikethrough;
		tmpCStyle.setStrikethruOffset(-1);
		tmpCStyle.setStrikethruWidth(-1);
		tmpCStyle.setStrokeColor(oStyle.CurrColorText);
	}
	if (oStyle.textShadow)
	{
		styleEffects |= ScStyle_Shadowed;
		tmpCStyle.setShadowXOffset(30);
		tmpCStyle.setShadowYOffset(-30);
		tmpCStyle.setStrokeColor(oStyle.CurrColorText);
	}
	if (oStyle.textSmallCaps)
	{
		styleEffects |= ScStyle_SmallCaps;
	}
	tmpCStyle.setFeatures(styleEffects.featureList());
	if ((oStyle.fontStyle == "italic") && (oStyle.fontWeight == "bold"))
		setFontstyle(tmpCStyle, 3);
	else if ((oStyle.fontStyle == "oblique") && (oStyle.fontWeight == "bold"))
		setFontstyle(tmpCStyle, 4);
	else if (oStyle.fontStyle == "italic")
		setFontstyle(tmpCStyle, 0);
	else if (oStyle.fontStyle == "oblique")
		setFontstyle(tmpCStyle, 1);
	else if (oStyle.fontWeight == "bold")
		setFontstyle(tmpCStyle, 2);
}

void ODTIm::applyParagraphStyle(ParagraphStyle &tmpStyle, ObjStyleODT &oStyle)
{
	tmpStyle.setAlignment(oStyle.textAlign);
	tmpStyle.setLeftMargin(oStyle.margin_left);
	tmpStyle.setRightMargin(oStyle.margin_right);
	tmpStyle.setFirstIndent(oStyle.textIndent);
	tmpStyle.setGapAfter(oStyle.margin_bottom);
	tmpStyle.setGapBefore(oStyle.margin_top);
	tmpStyle.setBackgroundColor(oStyle.CurrColorBPara);
	tmpStyle.setTabValues(oStyle.tabStops);
}

void ODTIm::resolveStyle(ObjStyleODT &tmpOStyle, QString pAttrs)
{
	if (m_Styles.contains(pAttrs))
	{
		DrawStyle actStyle;
		DrawStyle currStyle = m_Styles[pAttrs];
		QStringList parents;
		while (currStyle.parentStyle.valid)
		{
			if (m_Styles.contains(currStyle.parentStyle.value))
			{
				parents.prepend(currStyle.parentStyle.value);
				currStyle = m_Styles[currStyle.parentStyle.value];
			}
			else
				break;
		}
		parents.append(pAttrs);
		if (!parents.isEmpty())
		{
			for (int p = 0; p < parents.count(); p++)
			{
				currStyle = m_Styles[parents[p]];
				if (currStyle.fontName.valid)
					actStyle.fontName = AttributeValue(currStyle.fontName.value);
				if (currStyle.fontSize.valid)
				{
					if (currStyle.fontSize.value.right(1) == "%")
					{
						double perc = parseUnit(currStyle.fontSize.value);
						if (actStyle.fontSize.valid)
						{
							double value = parseUnit(actStyle.fontSize.value) * perc;
							actStyle.fontSize = AttributeValue(QString("%1pt").arg(value));
						}
					}
					else
						actStyle.fontSize = AttributeValue(currStyle.fontSize.value);
				}
				if (currStyle.fontStyle.valid)
					actStyle.fontStyle = AttributeValue(currStyle.fontStyle.value);
				if (currStyle.fontWeight.valid)
					actStyle.fontWeight = AttributeValue(currStyle.fontWeight.value);
				if (currStyle.margin_top.valid)
					actStyle.margin_top = AttributeValue(currStyle.margin_top.value);
				if (currStyle.margin_bottom.valid)
					actStyle.margin_bottom = AttributeValue(currStyle.margin_bottom.value);
				if (currStyle.margin_left.valid)
					actStyle.margin_left = AttributeValue(currStyle.margin_left.value);
				if (currStyle.margin_right.valid)
					actStyle.margin_right = AttributeValue(currStyle.margin_right.value);
				if (currStyle.textIndent.valid)
					actStyle.textIndent = AttributeValue(currStyle.textIndent.value);
				if (currStyle.textAlign.valid)
					actStyle.textAlign = AttributeValue(currStyle.textAlign.value);
				if (currStyle.textPos.valid)
					actStyle.textPos = AttributeValue(currStyle.textPos.value);
				if (currStyle.textOutline.valid)
					actStyle.textOutline = AttributeValue(currStyle.textOutline.value);
				if (currStyle.textUnderline.valid)
					actStyle.textUnderline = AttributeValue(currStyle.textUnderline.value);
				if (currStyle.textUnderlineWords.valid)
					actStyle.textUnderlineWords = AttributeValue(currStyle.textUnderlineWords.value);
				if (currStyle.textUnderlineColor.valid)
					actStyle.textUnderlineColor = AttributeValue(currStyle.textUnderlineColor.value);
				if (currStyle.textStrikeThrough.valid)
					actStyle.textStrikeThrough = AttributeValue(currStyle.textStrikeThrough.value);
				if (currStyle.textShadow.valid)
					actStyle.textShadow = AttributeValue(currStyle.textShadow.value);
				if (currStyle.fontVariant.valid)
					actStyle.fontVariant = AttributeValue(currStyle.fontVariant.value);
				if (currStyle.lineHeight.valid)
					actStyle.lineHeight = AttributeValue(currStyle.lineHeight.value);
				if (currStyle.fontColor.valid)
					actStyle.fontColor = AttributeValue(currStyle.fontColor.value);
				if (currStyle.textBackgroundColor.valid)
					actStyle.textBackgroundColor = AttributeValue(currStyle.textBackgroundColor.value);
				if (currStyle.parBackgroundColor.valid)
					actStyle.parBackgroundColor = AttributeValue(currStyle.parBackgroundColor.value);
				if (currStyle.verticalAlignment.valid)
					actStyle.verticalAlignment = AttributeValue(currStyle.verticalAlignment.value);
				if (currStyle.tabDists.valid)
					actStyle.tabDists = AttributeValue(currStyle.tabDists.value);
				if (currStyle.tabTypes.valid)
					actStyle.tabTypes = AttributeValue(currStyle.tabTypes.value);
				if (currStyle.breakAfter.valid)
					actStyle.breakAfter = AttributeValue(currStyle.breakAfter.value);
				if (currStyle.breakBefore.valid)
					actStyle.breakBefore = AttributeValue(currStyle.breakBefore.value);
			}
		}
		if (actStyle.textBackgroundColor.valid)
		{
			if (actStyle.textBackgroundColor.value == "transparent")
				tmpOStyle.CurrColorBText = CommonStrings::None;
			else
				tmpOStyle.CurrColorBText = parseColor(actStyle.textBackgroundColor.value);
		}
		else
			tmpOStyle.CurrColorBText = CommonStrings::None;
		if (actStyle.parBackgroundColor.valid)
		{
			if (actStyle.parBackgroundColor.value == "transparent")
				tmpOStyle.CurrColorBPara = CommonStrings::None;
			else
				tmpOStyle.CurrColorBPara = parseColor(actStyle.parBackgroundColor.value);
		}
		else
			tmpOStyle.CurrColorBPara = CommonStrings::None;
		if (actStyle.fontName.valid)
		{
			if (m_fontMap.contains(actStyle.fontName.value))
				tmpOStyle.fontName = m_fontMap[actStyle.fontName.value];
			else
				tmpOStyle.fontName = actStyle.fontName.value;
			if (!PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts.contains(tmpOStyle.fontName))
			{
				tmpOStyle.fontName = constructFontName(tmpOStyle.fontName, "");
				m_fontMap[actStyle.fontName.value] = tmpOStyle.fontName;
			}
		}
		else
		{
			QString fontName;
			QStack<QString> textStyleStack = m_textStylesStack;
			while (!textStyleStack.isEmpty())
			{
				QString styleName = textStyleStack.pop();
				if (!m_Styles.contains(styleName))
					continue;
				const DrawStyle& odtStyle = m_Styles[styleName];
				if (odtStyle.fontName.valid)
				{
					if (m_fontMap.contains(odtStyle.fontName.value))
						tmpOStyle.fontName = m_fontMap[odtStyle.fontName.value];
					else
						tmpOStyle.fontName = constructFontName(odtStyle.fontName.value, "");
					if (!PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts.contains(tmpOStyle.fontName))
					{
						tmpOStyle.fontName = constructFontName(tmpOStyle.fontName, "");
						m_fontMap[odtStyle.fontName.value] = tmpOStyle.fontName;
					}
					fontName = tmpOStyle.fontName;
					break;
				}
				if (odtStyle.parentStyle.valid)
				{
					QVector<QString> parentStyles;
					DrawStyle drawStyle = odtStyle;
					while (drawStyle.parentStyle.valid)
					{
						if (!m_Styles.contains(drawStyle.parentStyle.value))
							break;
						parentStyles.prepend(drawStyle.parentStyle.value);
						drawStyle = m_Styles[drawStyle.parentStyle.value];
					}
					if (parentStyles.count() > 0)
						textStyleStack += parentStyles;
				}
			}
			if (txtDefaultStyle.fontName.valid && fontName.isEmpty())
			{
				tmpOStyle.fontName = constructFontName(txtDefaultStyle.fontName.value, "");
				m_fontMap[txtDefaultStyle.fontName.value] = tmpOStyle.fontName;
				fontName = tmpOStyle.fontName;
			}
			if (parDefaultStyle.fontName.valid && fontName.isEmpty())
			{
				tmpOStyle.fontName = constructFontName(parDefaultStyle.fontName.value, "");
				m_fontMap[parDefaultStyle.fontName.value] = tmpOStyle.fontName;
				fontName = tmpOStyle.fontName;
			}
		}
		if (actStyle.fontStyle.valid)
			tmpOStyle.fontStyle = actStyle.fontStyle.value;
		if (actStyle.fontWeight.valid)
			tmpOStyle.fontWeight = actStyle.fontWeight.value;
		if (actStyle.fontSize.valid)
			tmpOStyle.fontSize = parseUnit(actStyle.fontSize.value);
		if (actStyle.fontColor.valid)
			tmpOStyle.CurrColorText = parseColor(actStyle.fontColor.value);
		if (actStyle.margin_top.valid)
			tmpOStyle.margin_top = parseUnit(actStyle.margin_top.value);
		if (actStyle.margin_bottom.valid)
			tmpOStyle.margin_bottom = parseUnit(actStyle.margin_bottom.value);
		if (actStyle.margin_left.valid)
			tmpOStyle.margin_left = parseUnit(actStyle.margin_left.value);
		if (actStyle.margin_right.valid)
			tmpOStyle.margin_right = parseUnit(actStyle.margin_right.value);
		if (actStyle.textIndent.valid)
			tmpOStyle.textIndent = parseUnit(actStyle.textIndent.value);
		if (actStyle.textAlign.valid)
		{
			QString attValue = actStyle.textAlign.value;
			if ((attValue == "left") || (attValue == "start"))
				tmpOStyle.textAlign = ParagraphStyle::Leftaligned;
			else if (attValue == "center")
				tmpOStyle.textAlign = ParagraphStyle::Centered;
			else if ((attValue == "right") || (attValue == "end"))
				tmpOStyle.textAlign = ParagraphStyle::Rightaligned;
			else if (attValue == "justify")
				tmpOStyle.textAlign = ParagraphStyle::Justified;
		}
		if (actStyle.verticalAlignment.valid)
		{
			if (actStyle.verticalAlignment.value == "middle")
				tmpOStyle.verticalAlignment = 1;
			else if (actStyle.verticalAlignment.value == "bottom")
				tmpOStyle.verticalAlignment = 2;
		}
		if (actStyle.textPos.valid)
			tmpOStyle.textPos = actStyle.textPos.value;
		if (actStyle.textOutline.valid)
			tmpOStyle.textOutline = actStyle.textOutline.value;
		if (actStyle.textUnderline.valid)
			tmpOStyle.textUnderline = actStyle.textUnderline.value != "none";
		if (actStyle.textUnderlineWords.valid)
			tmpOStyle.textUnderlineWords = actStyle.textUnderlineWords.value != "continuous";
		if (actStyle.textUnderlineColor.valid)
		{
			if (actStyle.textUnderlineColor.value == "font-color")
				tmpOStyle.textUnderlineColor = tmpOStyle.CurrColorText;
			else
				tmpOStyle.textUnderlineColor = parseColor(actStyle.textUnderlineColor.value);
		}
		if (actStyle.textStrikeThrough.valid)
			tmpOStyle.textStrikeThrough = actStyle.textStrikeThrough.value != "none";
		if (actStyle.textShadow.valid)
			tmpOStyle.textShadow = actStyle.textShadow.value != "none";
		if (actStyle.fontVariant.valid)
			tmpOStyle.textSmallCaps = actStyle.fontVariant.value == "small-caps";
		if (actStyle.lineHeight.valid)
		{
			if (actStyle.lineHeight.value == "normal")
				tmpOStyle.lineHeight = -1.0;
			else if (actStyle.lineHeight.value.right(1) != "%")
			{
				tmpOStyle.lineHeight = parseUnit(actStyle.lineHeight.value);
				tmpOStyle.absLineHeight = true;
			}
			else
			{
				tmpOStyle.lineHeight = parseUnit(actStyle.lineHeight.value) * tmpOStyle.fontSize;
				tmpOStyle.absLineHeight = false;
			}
		}
		if ((actStyle.tabDists.valid) && (actStyle.tabTypes.valid))
		{
			QStringList dists = actStyle.tabDists.value.split(";", QString::SkipEmptyParts);
			QStringList types = actStyle.tabTypes.value.split(" ", QString::SkipEmptyParts);
			if (dists.count() == types.count())
			{
				tmpOStyle.tabStops.clear();
				ParagraphStyle::TabRecord tb;
				for (int a = 0; a < dists.count(); a++)
				{
					tb.tabPosition = parseUnit(dists[a]);
					if (types[a] == "left")
						tb.tabType = 0;
					else if (types[a] == "center")
						tb.tabType = 4;
					else if (types[a] == "right")
						tb.tabType = 1;
					else if (types[a] == "char")
						tb.tabType = 3;
					tmpOStyle.tabStops.append(tb);
				}
			}
		}
		if (actStyle.breakAfter.valid)
			tmpOStyle.breakAfter = actStyle.breakAfter.value;
		if (actStyle.breakBefore.valid)
			tmpOStyle.breakBefore = actStyle.breakBefore.value;
	}
}

double ODTIm::parseUnit(const QString &unit)
{
	QString unitval=unit;
	if (unit.isEmpty())
		return 0.0;
	if( unit.right( 2 ) == "pt" )
		unitval.replace( "pt", "" );
	else if( unit.right( 2 ) == "cm" )
		unitval.replace( "cm", "" );
	else if( unit.right( 2 ) == "mm" )
		unitval.replace( "mm" , "" );
	else if( unit.right( 2 ) == "in" )
		unitval.replace( "in", "" );
	else if( unit.right( 2 ) == "px" )
		unitval.replace( "px", "" );
	else if( unit.right( 1 ) == "%" )
		unitval.replace( "%", "" );
	double value = ScCLocale::toDoubleC(unitval);
	if( unit.right( 2 ) == "pt" )
		{}/* value = value; */ //no change
	else if( unit.right( 2 ) == "cm" )
		value = ( value / 2.54 ) * 72;
	else if( unit.right( 2 ) == "mm" )
		value = ( value / 25.4 ) * 72;
	else if( unit.right( 2 ) == "in" )
		value = value * 72;
	else if( unit.right( 2 ) == "px" )
		{}/* value = value; */ //no change
	else if( unit.right( 1 ) == "%" )
		value = value / 100.0;
	return value;
}

QColor ODTIm::parseColorN( const QString &rgbColor )
{
	int r, g, b;
	keywordToRGB( rgbColor, r, g, b );
	return QColor( r, g, b );
}

QString ODTIm::parseColor( const QString &s )
{
	QColor c;
	QString ret = CommonStrings::None;
	if ((s == "") || s.isEmpty())
		return ret;
	if( s.startsWith( "rgb(" ) )
	{
		QString parse = s.trimmed();
		QStringList colors = parse.split( ',', QString::SkipEmptyParts );
		QString r = colors[0].right( ( colors[0].length() - 4 ) );
		QString g = colors[1];
		QString b = colors[2].left( ( colors[2].length() - 1 ) );
		if( r.contains( "%" ) )
		{
			r.chop(1);
			r = QString::number( static_cast<int>( ( static_cast<double>( 255 * ScCLocale::toDoubleC(r) ) / 100.0 ) ) );
		}
		if( g.contains( "%" ) )
		{
			g.chop(1);
			g = QString::number( static_cast<int>( ( static_cast<double>( 255 * ScCLocale::toDoubleC(g) ) / 100.0 ) ) );
		}
		if( b.contains( "%" ) )
		{
			b.chop(1);
			b = QString::number( static_cast<int>( ( static_cast<double>( 255 * ScCLocale::toDoubleC(b) ) / 100.0 ) ) );
		}
		c = QColor(r.toInt(), g.toInt(), b.toInt());
	}
	else
	{
		QString rgbColor = s.trimmed();
		if( rgbColor.startsWith( "#" ) )
			c.setNamedColor( rgbColor );
		else
			c = parseColorN( rgbColor );
	}
	ScColor tmp;
	tmp.fromQColor(c);
	tmp.setSpotColor(false);
	tmp.setRegistrationColor(false);
	QString fNam = m_Doc->PageColors.tryAddColor("FromOdt"+c.name(), tmp);
	ret = fNam;
	return ret;
}

QString ODTIm::constructFontName(QString fontBaseName, QString fontStyle)
{
	QString fontName;
	SCFontsIterator it(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts);
	for ( ; it.hasNext(); it.next())
	{
		if (fontBaseName.toLower() != it.current().family().toLower())
			continue;

		// found the font family, now go for the style
		QStringList slist = PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts.fontMap[it.current().family()];
		slist.sort();
		if (slist.count() > 0)
		{
			for (int a = 0; a < slist.count(); a++)
			{
				if (fontStyle.toLower() == slist[a].toLower())
				{
					fontName = it.current().family() + " " + slist[a];
					return fontName;
				}
			}
			int reInd = slist.indexOf("Regular");
			if (reInd < 0)
				fontName = it.current().family() + " " + slist[0];
			else
				fontName = it.current().family() + " " + slist[reInd];
			return fontName;
		}
		else
		{
			fontName = it.current().family();
			return fontName;
		}
	}

	// Still no font found
	QString family = fontBaseName;
	if (!fontStyle.isEmpty())
		family += " " + fontStyle;
	if (PrefsManager::instance()->appPrefs.fontPrefs.GFontSub.contains(family))
	{
		fontName = PrefsManager::instance()->appPrefs.fontPrefs.GFontSub[family];
		return fontName;
	}

	qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
	QScopedPointer<MissingFont> dia(new MissingFont(0, family, m_Doc));
	if (dia->exec())
		fontName = dia->getReplacementFont();
	else
		fontName = m_Doc->itemToolPrefs().textFont;
	PrefsManager::instance()->appPrefs.fontPrefs.GFontSub[family] = fontName;
	qApp->changeOverrideCursor(QCursor(Qt::WaitCursor));

	return fontName;
}

void ODTIm::setFontstyle(CharStyle &tmpCStyle, int kind)
{
	int posC = m_item->itemText.length();
	m_item->itemText.insertChars(posC, "B");
	m_item->itemText.applyCharStyle(posC, 1, tmpCStyle);
	QString fam = m_item->itemText.charStyle(posC).font().family();
	m_item->itemText.removeChars(posC, 1);
	if (fam.isEmpty())
		return;
	QStringList slist = PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts.fontMap[fam];
	if (kind == 0)
	{
		if (slist.contains("Italic"))
			tmpCStyle.setFont(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts[fam + " Italic"]);
	}
	else if (kind == 1)
	{
		if (slist.contains("Oblique"))
			tmpCStyle.setFont(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts[fam + " Oblique"]);
	}
	else if (kind == 2)
	{
		if (slist.contains("Bold"))
			tmpCStyle.setFont(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts[fam + " Bold"]);
	}
	else if (kind == 3)
	{
		if (slist.contains("Bold Italic"))
			tmpCStyle.setFont(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts[fam + " Bold Italic"]);
	}
	else if (kind == 4)
	{
		if (slist.contains("Bold Oblique"))
			tmpCStyle.setFont(PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts[fam + " Bold Oblique"]);
	}
}
