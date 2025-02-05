#include "scclipboardprocessor.h"

#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QDomDocument>
#include <QFile>
#include <QGuiApplication>
#include <QMimeData>
#include <QString>
#include <QRegularExpression>

#include <QTextStream>
#include <QXmlStreamReader>

#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>

#include "pageitem.h"
#include "prefsmanager.h"
#include "scfonts.h"
#include "scribusdoc.h"
#include "text/specialchars.h"
#include "undomanager.h"


ScClipboardProcessor::ScClipboardProcessor()
{
}

ScClipboardProcessor::ScClipboardProcessor(ScribusDoc* doc, PageItem *pageItem)
{
	m_doc = doc;
	m_pageItem = pageItem;
}

void ScClipboardProcessor::setContent(const QString &content, ContentType type)
{
	m_content = content;
	m_contentType = type;
	processed = false;
}

void ScClipboardProcessor::setDocAndPageItem(ScribusDoc *doc, PageItem *pageItem)
{
	m_doc = doc;
	m_pageItem = pageItem;
}

bool ScClipboardProcessor::process()
{
#ifdef SCCLIP_DEBUG
	QString dashes(40, '-');  // Create a string of 'count' dashes
	qDebug() << dashes;
	dumpClipboardData();
	qDebug() << dashes;
#endif
	if (m_content.isEmpty())
	{
		qDebug()<<"ScClipboardProcessor: Content is empty";
		return false;
	}
	switch (m_contentType)
	{
		case ContentType::Text:
			return processText();
			break;
		case ContentType::HTML:
			return processHTML();
			break;
		default:
			break;
	}
	return false;
}

bool ScClipboardProcessor::processText()
{
	QString text(m_content);
	text.replace("\r\n", SpecialChars::PARSEP);
	text.replace('\n', SpecialChars::PARSEP);
	m_result = text;
	processed = true;
	UndoManager* undoManager = UndoManager::instance();
	//TODO change undo to PASTE_HTML
	if (UndoManager::undoEnabled())
	{
		auto *is = new SimpleState(Um::Paste, QString(), Um::IPaste);
		is->set("PASTE_PLAINTEXT");
		is->set("START", m_pageItem->itemText.cursorPosition());
		is->set("TEXT", m_result);
		undoManager->action(m_pageItem, is);
	}
	m_pageItem->itemText.insertChars(m_result, true);

	return true;
}

bool ScClipboardProcessor::processHTML()
{
	//Something from Apple Pages (just a first example)
	if (m_content.contains("Cocoa HTML Writer"))
		return html_Cocoa_Process();

	//Something from Microsoft Word (just a first example, maybe all of Microsoft's apps)
	if (m_content.contains("schemas-microsoft-com"))
		return html_MSFT_Process();

	return processHTML_Other();
}

bool ScClipboardProcessor::html_MSFT_Process()
{
	QTextDocument qTextDoc;
	qTextDoc.setHtml(m_content);


	//temporary result for 1.7.0
	m_result = qTextDoc.toPlainText();
	m_result.replace("\r\n", SpecialChars::PARSEP);
	m_result.replace('\n', SpecialChars::PARSEP);
	processed = true;
	return true;
}

bool ScClipboardProcessor::html_Cocoa_Process()
{
	// Convert to a const xmlChar*, parse with libxml2
	const xmlChar* html_content_cstr = reinterpret_cast<const xmlChar*>(m_content.toUtf8().constData());

	// Parse the HTML content using libxml2's HTML parser
	htmlDocPtr doc = htmlReadDoc(html_content_cstr, NULL, NULL, HTML_PARSE_RECOVER | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);

	if (!doc)
	{
		qDebug() << "Failed to parse the HTML content.";
		return false;
	}

	// Get the document's root <html> tag
	xmlNodePtr root = xmlDocGetRootElement(doc);
	if (root != nullptr)
	{
		// Parse the content
		html_Cocoa_Parse(root);
	}
	// Clean up the document after parsing
	xmlFreeDoc(doc);
	xmlCleanupParser();

	processed = true;
	return true;
}

void ScClipboardProcessor::html_Cocoa_Parse(xmlNodePtr node)
{
	// Find <style> and parse styles
	for (xmlNode *cur = node->children; cur; cur = cur->next)
	{
		if (cur->type == XML_ELEMENT_NODE && xmlStrcmp(cur->name, (const xmlChar *)"head") == 0)
		{
			for (xmlNode *headChild = cur->children; headChild; headChild = headChild->next)
			{
				cocoaParseStyles(headChild, cssStyles);
			}
		}
	}

	processCSS(cssStyles);

	// Find <p> elements and extract text
	for (xmlNode *cur = node->children; cur; cur = cur->next)
	{
		if (cur->type == XML_ELEMENT_NODE && xmlStrcmp(cur->name, (const xmlChar *)"body") == 0)
		{
			cocoaParseParagraphs(cur->children, cssStyles);
		}
	}
}

// Function to extract styles from <style> tag
void ScClipboardProcessor::cocoaParseStyles(xmlNode *node, QMap<QString, QString> &styles)
{
	if (node->type == XML_ELEMENT_NODE && xmlStrcmp(node->name, (const xmlChar *)"style") == 0)
	{
		xmlChar *styleContent = xmlNodeGetContent(node);
		QString stylesText = QString::fromUtf8((const char*)styleContent);
		xmlFree(styleContent);

		// Regular expression to extract class and styles
		static QRegularExpression regex(R"(p\.(\w+)\s*\{(.*?)\})");
		QRegularExpressionMatchIterator it = regex.globalMatch(stylesText);

		while (it.hasNext())
		{
			QRegularExpressionMatch match = it.next();
			if (match.hasMatch())
			{
				QString className = match.captured(1).trimmed();
				QString style = match.captured(2).trimmed();
				styles[className] = style;
			}
		}
	}
}

// Function to extract text content (including formatted text like <b> and <i>)
QString ScClipboardProcessor::cocoaExtractText(xmlNode *node, QList<TextSegment> &segments, bool bold, bool italic)
{
	QString text;
	for (xmlNode *cur = node; cur; cur = cur->next)
	{
		if (cur->type == XML_TEXT_NODE)
		{
			QString t = QString::fromUtf8((const char *)cur->content);
			text += t;
			if (!text.isEmpty())
			{
				segments.append({t, bold, italic});
			}
		}
		else if (cur->type == XML_ELEMENT_NODE)
		{
			QString tag = QString::fromUtf8((const char *)cur->name);
			bool newBold = bold || (tag == "b");
			bool newItalic = italic || (tag == "i");
			QString innerText = cocoaExtractText(cur->children, segments, newBold, newItalic);

			if (tag == "b")
			{
				text += "<b>" + innerText + "</b>";
			}
			else if (tag == "i")
			{
				text += "<i>" + innerText + "</i>";
			}
			else
			{
				text += innerText;
			}
		}
	}
	return text;
}

// Function to parse paragraphs and their styles
void ScClipboardProcessor::cocoaParseParagraphs(xmlNode *node, QMap<QString, QString> &styles)
{
	SCFonts& availableFonts = PrefsManager::instance().appPrefs.fontPrefs.AvailFonts;
	QList<TextSegment> segments;
	for (xmlNode *cur = node; cur; cur = cur->next)
	{
		if (cur->type == XML_ELEMENT_NODE && xmlStrcmp(cur->name, (const xmlChar *)"p") == 0)
		{
			xmlChar *classAttr = xmlGetProp(cur, (const xmlChar *)"class");
			QString className = classAttr ? QString::fromUtf8((const char*)classAttr) : "None";
			xmlFree(classAttr);

			QString content = cocoaExtractText(cur->children, segments);
			// qDebug() << "-----------------------------------";
			// qDebug() << "Paragraph Class:" << className;
			// qDebug() << "Style:" << style;
			// qDebug() << "Text Content:" << content;

			ParagraphStyle currPstyle = m_doc->paragraphStyle(className);
			int pos = qMax(0, m_pageItem->itemText.cursorPosition());
			bool textContainsBoldItalic = content.contains("<b>") || content.contains("<i>");
			if (!textContainsBoldItalic)
			{
				m_pageItem->itemText.insertChars(pos, content);
				m_pageItem->itemText.applyStyle(pos, currPstyle);
				m_pageItem->itemText.applyCharStyle(pos, content.length(), currPstyle.charStyle());
			}
			else
			{
				for (const auto &segment : std::as_const(segments))
				{
					QString currFamily(currPstyle.charStyle().font().family());
					if (!segment.isBold && !segment.isItalic)
					{
						QStringList styles = availableFonts.fontMap[currFamily];
						QString style = styles[0];
						if (styles.contains("Regular"))
							style = "Regular";
						else if (styles.contains("Roman"))
							style = "Roman";
						else if (styles.contains("Medium"))
							style = "Medium";
						else if (styles.contains("Book"))
							style = "Book";
						const ScFace face = availableFonts.findFont(currFamily, style);
						if (face != ScFace::none())
						{
							currPstyle.charStyle().setFont(face);
							m_pageItem->itemText.insertChars(pos, segment.text);
							m_pageItem->itemText.applyStyle(pos, currPstyle);
							m_pageItem->itemText.applyCharStyle(pos, segment.text.length(), currPstyle.charStyle());
						}
						else
							qDebug()<<"No face found";
					}
					else
					{
						QString style;
						if(segment.isBold && !segment.isItalic)
							style = "Bold";
						if (!segment.isBold && segment.isItalic)
							style = "Italic";
						if(segment.isBold && segment.isItalic)
							style = "Bold Italic";

						const ScFace& face = availableFonts.findFont(currFamily, style);
						if (face != ScFace::none())
						{
							currPstyle.charStyle().setFont(face);
							m_pageItem->itemText.insertChars(pos, segment.text);
							m_pageItem->itemText.applyStyle(pos, currPstyle);
							m_pageItem->itemText.applyCharStyle(pos, segment.text.length(), currPstyle.charStyle());
						}
						else
							qDebug()<<"No face found";
					}

					pos = m_pageItem->itemText.cursorPosition();
				}
				segments.clear();
			}
			//Add a new line after the paragraph
			pos = qMax(0, m_pageItem->itemText.cursorPosition());
			m_pageItem->itemText.insertChars(pos, SpecialChars::PARSEP);
			m_pageItem->itemText.applyStyle(pos, currPstyle);
		}
	}
}

void ScClipboardProcessor::processCSS(const QMap<QString, QString> &styles)
{
	QMapIterator<QString, QString> i(styles);
	while (i.hasNext())
	{
		i.next();
		QString paraStyleName = i.key();

		// Parse properties
		QMap<QString, QString> propertyMap;
		QStringList propertyList = i.value().split(';', Qt::SkipEmptyParts);
		for (const QString& property : std::as_const(propertyList))
		{
			QStringList keyValue = property.split(':', Qt::SkipEmptyParts);
			if (keyValue.size() == 2)
				propertyMap.insert(keyValue[0].trimmed().toLower(), keyValue[1].trimmed());
		}
		ParagraphStyle newParaStyle;
		newParaStyle.setDefaultStyle(false);
		newParaStyle.setName(paraStyleName);
		//Clearly this regex only deals with this format of CSS:
		//p.p1 {margin: 0.0px 0.0px 12.0px 0.0px; font: 11.0px 'Helvetica Neue'; color: #1a1a1a}
		for (auto it = propertyMap.begin(); it != propertyMap.end(); ++it)
		{
			if (it.key() == "color")
			{
				ScColor newColor;
				newColor.fromQColor(QColor(it.value()));
				QString colorName = m_doc->PageColors.tryAddColor("FromCopy"+it.value(), newColor);
				newParaStyle.charStyle().setFillColor(colorName);
			}
			if (it.key() == "font")
			{
				// static QRegularExpression regex(R"(\s*([\d.]+)px\s*'([^']+)')");
				static QRegularExpression regex(R"(\s*([\d.]+)px\s*('([^']+)'|([\w]+)))");

				QRegularExpressionMatch match = regex.match(it.value());
				QString fontName;
				double fontSize;
				if (match.hasMatch())
				{
					//Font name moves between capture 3 and 4 depending on if quotes are used
					fontSize = match.captured(1).toDouble() * 10.0;
					fontName = match.captured(match.lastCapturedIndex());
				}
				else
				{
					fontSize = PrefsManager::instance().appPrefs.itemToolPrefs.textSize;
					// qDebug() << "No match found";
				}
				//Process font name
				SCFonts& availableFonts = PrefsManager::instance().appPrefs.fontPrefs.AvailFonts;
				QStringList styles = availableFonts.fontMap[fontName];
				QString style = styles[0];
				if (styles.contains("Regular"))
					style = "Regular";
				else if (styles.contains("Roman"))
					style = "Roman";
				else if (styles.contains("Medium"))
					style = "Medium";
				else if (styles.contains("Book"))
					style = "Book";
				const ScFace& face = availableFonts.findFont(fontName, style);
				QString textFont;
				if (face != ScFace::none())
					textFont = face.family() + " " + face.style();
				else
					textFont = PrefsManager::instance().appPrefs.itemToolPrefs.textFont;
				newParaStyle.charStyle().setFont((*m_doc->AllFonts)[textFont]);
				newParaStyle.charStyle().setFontSize(fontSize);
			}
			StyleSet<ParagraphStyle> tmpParagraphStyleSet;
			tmpParagraphStyleSet.create(newParaStyle);
			m_doc->redefineStyles(tmpParagraphStyleSet, false);
		}
	}
}

bool ScClipboardProcessor::processHTML_Other()
{
	QTextDocument qTextDoc;
	qTextDoc.setHtml(m_content);

	//temporary result for 1.7.0
	m_result = qTextDoc.toPlainText();
	m_result.replace("\r\n", SpecialChars::PARSEP);
	m_result.replace('\n', SpecialChars::PARSEP);
	processed = true;
	return true;
}



const QString& ScClipboardProcessor::data()
{
	if (!processed)
		qDebug()<<"ScClipboardProcessor: Data not processed";
	return m_result;
}

void ScClipboardProcessor::reset()
{
	m_content.clear();
	m_contentType = ContentType::Unknown;
	m_result.clear();
	cssStyles.clear();
	m_doc = nullptr;
	m_pageItem = nullptr;
	processed = false;
}

void ScClipboardProcessor::dumpClipboardData()
{
	qDebug()<<QApplication::clipboard()->mimeData()->formats();
	qDebug()<<QApplication::clipboard()->mimeData()->text();
	QDebug debug = qDebug();
	 debug.noquote()<<QApplication::clipboard()->mimeData()->html();
	//debug<<QApplication::clipboard()->mimeData()->html();
}
