#include "scclipboardprocessor.h"

#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QDateTime>
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

#include "commonstrings.h"
#include "pageitem.h"
#include "pageitem_table.h"
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

void ScClipboardProcessor::debugDumpClipboard()
{
	const QMimeData* mime = QApplication::clipboard()->mimeData();
	const QString dashes(60, '=');

	qDebug().noquote() << dashes;
	qDebug() << "Clipboard dump:" << QDateTime::currentDateTime().toString(Qt::ISODate);
	qDebug().noquote() << dashes;

	if (!mime)
	{
		qDebug() << "clipboard: empty (no QMimeData)";
		return;
	}

	const QStringList formats = mime->formats();
	qDebug() << "Available formats (" << formats.size() << "):";
	for (const QString& fmt : formats)
		qDebug() << "  " << fmt << "(" << mime->data(fmt).size() << "bytes )";

	qDebug().noquote() << dashes;

	for (const QString& fmt : formats)
	{
		const QByteArray data = mime->data(fmt);
		qDebug().noquote() << "--- FORMAT:" << fmt << "(" << data.size() << "bytes ) ---";

		// Decide text vs binary by trying UTF-8 and checking for high control-char density.
		const QString asText = QString::fromUtf8(data);
		bool looksBinary = false;
		const int sampleLen = qMin(256, asText.length());
		int controlCount = 0;
		for (int i = 0; i < sampleLen; ++i)
		{
			const QChar c = asText.at(i);
			if (c.unicode() < 0x20 && c != '\t' && c != '\n' && c != '\r')
				++controlCount;
		}
		if (sampleLen > 0 && controlCount * 4 > sampleLen)
			looksBinary = true;

		if (looksBinary)
		{
			qDebug().noquote() << "[binary, first 256 bytes as hex]";
			qDebug().noquote() << data.left(256).toHex(' ');
		}
		else
		{
			const int maxChars = 200000;
			if (asText.length() > maxChars)
				qDebug().noquote() << asText.left(maxChars) << "\n... [truncated," << asText.length() - maxChars << "more chars ]";
			else
				qDebug().noquote() << asText;
		}
		qDebug().noquote() << "";
	}

	qDebug().noquote() << dashes;
}

bool ScClipboardProcessor::process()
{
// #define SCCLIP_DEBUG
#ifdef SCCLIP_DEBUG
	debugDumpClipboard();
#endif

	if (!m_doc || !m_pageItem)
	{
		qDebug()<<"ScClipboardProcessor: No document or page item selected";
		return false;
	}
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

	//Something from LibreOffice
	if (m_content.contains("LibreOffice"))
		return html_LibreOffice_Process();

	// Fragment-only HTML (no <html>/<head>/<body> wrappers): if it contains a
	// <table> and we have a destination, route through the LibreOffice path
	// which handles the legacy <font>/<b>/<i>/<u> markup that Pages and
	// similar sources emit.
	if (m_tablePageItem != nullptr && m_content.contains("<table"))
		return html_LibreOffice_Process();

	return processHTML_Other();
}


bool ScClipboardProcessor::processHTML_Other()
{
	QTextDocument qTextDoc;
	qTextDoc.setHtml(m_content);

	//temporary result for 1.7.0
	m_result = qTextDoc.toPlainText();
	if (m_result.isEmpty())
	{
		qTextDoc.setPlainText(m_content);
		m_result = qTextDoc.toPlainText();
	}
	m_result.replace("\r\n", SpecialChars::PARSEP);
	m_result.replace('\n', SpecialChars::PARSEP);
	m_pageItem->itemText.insertChars(m_result, true);
	processed = true;
	return true;
}

bool ScClipboardProcessor::html_MSFT_Process()
{
	// Convert to a const xmlChar*, parse with libxml2
	QByteArray utf8Content = m_content.toUtf8();
	const xmlChar* html_content_cstr = reinterpret_cast<const xmlChar*>(utf8Content.constData());

	// Parse the HTML content using libxml2's HTML parser
	htmlDocPtr doc = htmlReadDoc(html_content_cstr, nullptr, nullptr, HTML_PARSE_RECOVER | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);
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
		html_MSFT_Parse(root);
	}
	// Clean up the document after parsing
	xmlFreeDoc(doc);

	processed = true;
	return true;
}

bool ScClipboardProcessor::html_MSFT_StyleToProcess(const QString &style)
{
	return	style.contains("color") ||
			style.contains("font-family:") ||
			style.contains("font-size") ||
			style.contains("font-style:") ||
			style.contains("font-weight:") ||
			style.contains("text-decoration:");
}

void ScClipboardProcessor::html_MSFT_Parse(xmlNode *node)
{
	if (!node)
		return;

	// Find <style> in the <head> area and parse styles
	for (xmlNode *cur = node->children; cur; cur = cur->next)
	{
		if (cur->type == XML_ELEMENT_NODE && xmlStrcmp(cur->name, (const xmlChar *)"head") == 0)
		{
			for (xmlNode *headChild = cur->children; headChild; headChild = headChild->next)
			{
				html_MSFT_ParseStyles(headChild, cssStyles);
			}
		}
	}

	html_MSFT_Process_CSS(cssStyles);

	// Two-pass approach: detect whether the body contains a table that can be
	// routed to the table-paste path. Word typically emits one table possibly
	// followed by a trailing <p>. If no destination table is set, or no <table>
	// is present, fall through to the existing paragraph path.
	bool hasTablePaste = false;
	if (m_tablePageItem != nullptr)
	{
		for (xmlNode *cur = node->children; cur && !hasTablePaste; cur = cur->next)
		{
			if (cur->type == XML_ELEMENT_NODE && xmlStrcmp(cur->name, (const xmlChar *)"body") == 0)
			{
				for (xmlNode *bodyChild = cur->children; bodyChild; bodyChild = bodyChild->next)
				{
					if (bodyChild->type == XML_ELEMENT_NODE &&
							xmlStrcmp(bodyChild->name, (const xmlChar *)"table") == 0)
					{
						hasTablePaste = true;
						break;
					}
				}
			}
		}
	}

	for (xmlNode *cur = node->children; cur; cur = cur->next)
	{
		if (cur->type == XML_ELEMENT_NODE && xmlStrcmp(cur->name, (const xmlChar *)"body") == 0)
		{
			if (hasTablePaste)
			{
				for (xmlNode *bodyChild = cur->children; bodyChild; bodyChild = bodyChild->next)
				{
					if (bodyChild->type != XML_ELEMENT_NODE)
						continue;
					if (xmlStrcmp(bodyChild->name, (const xmlChar *)"table") == 0)
					{
						ParsedTable pt;
						html_MSFT_ParseTable(bodyChild, pt);
						html_ApplyTable(pt);
					}
					// Other body children (e.g. trailing <p>) are intentionally
					// dropped for table paste; they're Word's end-of-fragment filler.
				}
			}
			else
			{
				// Existing non-table path — unchanged.
				html_MSFT_ParseParagraphs(cur->children, cssStyles);
			}
		}
	}
}

// Function to extract styles from <style>s in <head>
void ScClipboardProcessor::html_MSFT_ParseStyles(xmlNode *node, QMap<QString, QString> &styles)
{
	if (node->type == XML_ELEMENT_NODE && xmlStrcasecmp(node->name, (const xmlChar *)"style") == 0)
	{
		xmlChar *styleContent = xmlNodeGetContent(node);
		QString stylesText = QString::fromUtf8((const char*)styleContent);
		xmlFree(styleContent);

		// // Regular expression to extract class and styles
		static QRegularExpression regex("(\\.?)(\\w[\\w-]*)\\s*\\{([^}]*)\\}");
		QRegularExpressionMatchIterator it = regex.globalMatch(stylesText);
		static QRegularExpression regex2("mso-[^:]+:[^;]+;?");
		static QRegularExpression regex3("[\t\r\n]");

		while (it.hasNext())
		{
			QRegularExpressionMatch match = it.next();
			if (match.hasMatch())
			{
				// dot indicator in capture(1), name in capture(2), body in capture(3).
				// Both classes (.blah → "blah") and tag selectors (td → "td") map to bare names.
				QString selector = match.captured(2).trimmed();
				QString style = match.captured(3).trimmed();
				style.remove(regex2);
				style.remove(regex3);
				if (html_MSFT_StyleToProcess(style))
					styles[selector] = style;
			}
		}
	}
}

void ScClipboardProcessor::html_MSFT_Process_CSS(const QMap<QString, QString> &styles)
{
	QMapIterator<QString, QString> i(styles);
	while (i.hasNext())
	{
		i.next();
		QString paraStyleName = i.key();
		if (htmlHeadingTags.contains(paraStyleName))
			paraStyleName = "Heading " + paraStyleName.mid(1);

		// Skip tag-level selectors (e.g. "td", "tr") — they're not class-based
		// paragraph styles. They're used directly by the table fallback below.
		// Heuristic: class names from Word/Excel are like "MsoNormal", "xl65" —
		// they start with a letter and contain at least one digit or look like
		// CSS class identifiers. Tag selectors are short HTML tag names.
		static const QStringList tagSelectors = { "td", "tr", "th", "table", "p", "body", "div", "span", "col" };
		if (tagSelectors.contains(paraStyleName))
			continue;

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
		for (auto it = propertyMap.begin(); it != propertyMap.end(); ++it)
		{
			if (it.key() == "color")
			{
				ScColor newColor;
				newColor.fromQColor(QColor(it.value()));
				QString colorName = m_doc->PageColors.tryAddColor("FromCopy"+it.value(), newColor);
				newParaStyle.charStyle().setFillColor(colorName);
			}
			if (it.key() == "font-family")
			{
				static QRegularExpression regex(R"(\s*\"?([^\",]+)\"?\s*,?)");
				QRegularExpressionMatch match = regex.match(it.value());
				QString fontName;
				if (match.hasMatch())
					fontName = match.captured(1);
				//else
				// qDebug() << "No match found";
				//Process font name
				SCFonts& availableFonts = PrefsManager::instance().appPrefs.fontPrefs.AvailFonts;
				if (availableFonts.fontMap.contains(fontName))
				{
					QStringList styles = availableFonts.fontMap.value(fontName);
					QString style;
					if (!styles.isEmpty())
						style = styles[0];
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
				}
			}
			if (it.key() == "font-size")
			{
				static QRegularExpression regex(R"(\s*([\d.]+)pt)");
				QRegularExpressionMatch match = regex.match(it.value());
				double fontSize;
				if (match.hasMatch())
					fontSize = match.captured(1).toDouble() * 10.0;
				else
					fontSize = PrefsManager::instance().appPrefs.itemToolPrefs.textSize;
				newParaStyle.charStyle().setFontSize(fontSize);
			}
		}
		StyleSet<ParagraphStyle> tmpParagraphStyleSet;
		tmpParagraphStyleSet.create(newParaStyle);
		m_doc->redefineStyles(tmpParagraphStyleSet, false);
	}
}

// Apply CSS-style properties from a raw style string (e.g. "color:#FF0000;font-weight:bold;")
// onto the given TextSegment in-place. Unknown properties are ignored.
// Only properties relevant to character formatting are recognized.
void ScClipboardProcessor::applyMSFTCssStyleToSegment(QString styleData, TextSegment &ts)
{
	if (styleData.isEmpty())
		return;

	// Strip MSO-specific properties first so they don't interfere with the matchers below.
	static QRegularExpression regexMso("mso-[^:]+:[^;]+;?");
	styleData.remove(regexMso);

	static QRegularExpression regexColor(R"((?:^|;)\s*color\s*:\s*([^;]+))");
	static QRegularExpression regexFontsize(R"(font-size\s*:\s*([\d.]+)pt)");
	static QRegularExpression regexFamily(R"(font-family\s*:\s*((?:"[^"]+"|'[^']+'|[\w-]+(?:\s*,\s*[\w-]+)*)))");
	static QRegularExpression regexWeight(R"(font-weight\s*:\s*([^;]+))");
	static QRegularExpression regexFontStyle(R"(font-style\s*:\s*([^;]+))");
	static QRegularExpression regexDecoration(R"(text-decoration\s*:\s*([^;]+))");

	QRegularExpressionMatch m;

	m = regexColor.match(styleData);
	if (m.hasMatch())
		ts.color = m.captured(1).trimmed();

	m = regexFontsize.match(styleData);
	if (m.hasMatch())
		ts.fontsize = m.captured(1).toDouble() * 10.0;

	m = regexFamily.match(styleData);
	if (m.hasMatch())
	{
		QString fam = m.captured(1).trimmed();
		fam.remove('"');
		fam.remove('\'');
		// If it's a comma-separated list, take the first.
		const int comma = fam.indexOf(',');
		if (comma > 0)
			fam = fam.left(comma).trimmed();
		ts.family = fam;
	}

	m = regexWeight.match(styleData);
	if (m.hasMatch())
	{
		const QString w = m.captured(1).trimmed().toLower();
		if (w == "bold" || w == "bolder" || w.toInt() >= 600)
			ts.isBold = true;
	}

	m = regexFontStyle.match(styleData);
	if (m.hasMatch() && m.captured(1).trimmed().toLower() == "italic")
		ts.isItalic = true;

	m = regexDecoration.match(styleData);
	if (m.hasMatch() && m.captured(1).contains("underline", Qt::CaseInsensitive))
		ts.hasUnderline = true;
}

//Function to extract text content (including formatted text like <b> and <i>)
QString ScClipboardProcessor::html_MSFT_ExtractText(xmlNode *node, QList<TextSegment> &segments, TextSegment ts)
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
				t.replace("\r\n", " ");
				t.replace('\n', " ");
				TextSegment ts2(ts);
				ts2.text = t;
				segments.append(ts2);
			}
		}
		else if (cur->type == XML_ELEMENT_NODE)
		{
			QString tag = QString::fromUtf8((const char *)cur->name);
			// qDebug()<<"XML_ELEMENT_NODE"<<tag;

			// Skip Office paragraph markers (e.g. <o:p>) — they're paragraph-end
			// annotations, not content. The text inside them (typically &nbsp;
			// in empty cells) is filler that should not appear in the output.
			if (tag == "o:p" || tag.endsWith(":p"))
				continue;

			bool newBold = ts.isBold || (tag == "b");
			bool newItalic = ts.isItalic || (tag == "i");
			bool newUnderline = ts.hasUnderline || (tag == "u");
			QString newColor = ts.color;
			double newFontSize = ts.fontsize;
			QString newFamily = ts.family;
			if (tag == "span")
			{
				xmlChar *styleAttr = xmlGetProp(cur, (const xmlChar *)"style");
				if (styleAttr)
				{
					QString styleData = QString::fromUtf8((const char*)styleAttr);
					xmlFree(styleAttr);

					// Apply inline CSS to a new TextSegment representing this span's state.
					// Start from current ts state, then let the CSS override.
					TextSegment spanTs;
					spanTs.color = newColor;
					spanTs.fontsize = newFontSize;
					spanTs.family = newFamily;
					spanTs.isBold = newBold;
					spanTs.isItalic = newItalic;
					spanTs.hasUnderline = newUnderline;
					applyMSFTCssStyleToSegment(styleData, spanTs);

					newColor = spanTs.color;
					newFontSize = spanTs.fontsize;
					newFamily = spanTs.family;
					newBold = spanTs.isBold;
					newItalic = spanTs.isItalic;
					newUnderline = spanTs.hasUnderline;
				}
			}
			TextSegment ts{QString(), newColor, newBold, newItalic, newUnderline, newFontSize, newFamily};
			QString innerText = html_MSFT_ExtractText(cur->children, segments, ts);
			text += innerText;
		}
	}
	return text;
}



bool ScClipboardProcessor::html_LibreOffice_Process()
{
	// Convert to a const xmlChar*, parse with libxml2
	QByteArray utf8Content = m_content.toUtf8();
	const xmlChar* html_content_cstr = reinterpret_cast<const xmlChar*>(utf8Content.constData());

	// Parse the HTML content using libxml2's HTML parser
	htmlDocPtr doc = htmlReadDoc(html_content_cstr, nullptr, nullptr, HTML_PARSE_RECOVER | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);
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
		html_LibreOffice_Parse(root);
	}
	// Clean up the document after parsing
	xmlFreeDoc(doc);

	processed = true;
	return true;
}

void ScClipboardProcessor::html_LibreOffice_Parse(xmlNodePtr node)
{
	if (!node)
		return;

	// Two-pass approach: first detect whether the body contains a table that
	// we can route to the table-paste path; if so, process tables and skip
	// non-table siblings (the typical Writer case is body containing exactly
	// one table). Otherwise fall through to the existing paragraph path.
	bool hasTablePaste = false;
	if (m_tablePageItem != nullptr)
	{
		for (xmlNode *cur = node->children; cur && !hasTablePaste; cur = cur->next)
		{
			if (cur->type == XML_ELEMENT_NODE && xmlStrcmp(cur->name, (const xmlChar *)"body") == 0)
			{
				for (xmlNode *bodyChild = cur->children; bodyChild; bodyChild = bodyChild->next)
				{
					if (bodyChild->type == XML_ELEMENT_NODE && xmlStrcmp(bodyChild->name, (const xmlChar *)"table") == 0)
					{
						hasTablePaste = true;
						break;
					}
				}
			}
		}
	}
	for (xmlNode *cur = node->children; cur; cur = cur->next)
	{
		if (cur->type == XML_ELEMENT_NODE && xmlStrcmp(cur->name, (const xmlChar *)"body") == 0)
		{
			if (hasTablePaste)
			{
				// Route each top-level <table> through the table-paste path; ignore
				// other body content (paragraphs outside the table) for this paste.
				for (xmlNode *bodyChild = cur->children; bodyChild; bodyChild = bodyChild->next)
				{
					if (bodyChild->type != XML_ELEMENT_NODE)
						continue;
					if (xmlStrcmp(bodyChild->name, (const xmlChar *)"table") == 0)
					{
						ParsedTable pt;
						html_LibreOffice_ParseTable(bodyChild, pt);
						html_ApplyTable(pt);
					}
				}
			}
			else
			{
				// Original behavior: pass body children to ParseParagraphs.
				html_LibreOffice_ParseParagraphs(cur->children, cssStyles);
			}
		}
	}
}

void ScClipboardProcessor::html_LibreOffice_ProcessCSS(const QMap<QString, QString> &styles)
{
	int j = 0;
	QMapIterator<QString, QString> i(styles);
	while (i.hasNext())
	{
		i.next();
		QString paraStyleName = i.key() + QString("LO%1").arg(++j);
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
		for (auto it = propertyMap.begin(); it != propertyMap.end(); ++it)
		{
			StyleSet<ParagraphStyle> tmpParagraphStyleSet;
			tmpParagraphStyleSet.create(newParaStyle);
			m_doc->redefineStyles(tmpParagraphStyleSet, false);
		}
	}
}

void ScClipboardProcessor::html_LibreOffice_ParseStyles(xmlNode *node, QMap<QString, QString> &styles)
{
	if (node->type == XML_ELEMENT_NODE && xmlStrcasecmp(node->name, (const xmlChar *)"style") == 0)
	{
		xmlChar *styleContent = xmlNodeGetContent(node);
		QString stylesText = QString::fromUtf8((const char*)styleContent);
		xmlFree(styleContent);

		// Regular expression to extract class and styles
		static QRegularExpression regex(R"((p)\s*{(.*?)})");
		QRegularExpressionMatchIterator it = regex.globalMatch(stylesText);

		while (it.hasNext())
		{
			QRegularExpressionMatch match = it.next();
			if (match.hasMatch())
			{
				//Nothing much to come off styles from LO at this point....
				// QString className = match.captured(1).trimmed();
				// QString style = match.captured(2).trimmed();
				// qDebug()<<className<<style;
				// if (html_MSFT_StyleToProcess(style))
				// 	styles[className] = style;
			}
		}
	}
}

QString ScClipboardProcessor::html_LibreOffice_ExtractText(xmlNode *node, QList<TextSegment> &segments, TextSegment ts)
{
	QString text;
	for (xmlNode *cur = node; cur; cur = cur->next)
	{
		if (cur->type == XML_TEXT_NODE)
		{
			QString t = QString::fromUtf8((const char *)cur->content);
			text += t;
			// Collapse runs of CR/LF/TAB to a single space rather than deleting
			// leading/trailing ones. LibreOffice pretty-prints its clipboard HTML
			// and can emit an inter-run space as a lone newline in its own text
			// node; deleting that dropped the space and fused adjacent words.
			static QRegularExpression wsRun(R"([\r\n\t]+)");
			t.replace(wsRun, " ");
			// Suppress whitespace at the start of a line (LibreOffice puts a newline
			// and indentation right after <p>, and again after <br/>) so no leading
			// space is introduced. Strip the leading run rather than only dropping
			// nodes that are entirely whitespace: when the line's first text follows
			// on the same node -- <p>\n\t\t\ta</p>, or F<br/>\nf -- the collapse
			// above has just turned that indentation into a leading space. A node
			// that is entirely whitespace still ends up empty here and is skipped
			// below.
			//
			// Only at a line start: mid-line the collapsed run is the space between
			// two words and removing it fuses them, which is what this whole dance
			// is here to prevent. Nothing can fuse across a line start.
			bool atLineStart = segments.isEmpty()
							|| segments.constLast().text.endsWith(QChar(SpecialChars::LINEBREAK));
			if (atLineStart)
			{
				static QRegularExpression wsLead(R"(^\s+)");
				t.remove(wsLead);
			}
			if (!t.isEmpty())
			{
				TextSegment ts2(ts);
				ts2.text = t;
				segments.append(ts2);
			}
		}
		else if (cur->type == XML_ELEMENT_NODE)
		{
			QString tag = QString::fromUtf8((const char *)cur->name);
			// qDebug()<<"XML_ELEMENT_NODE"<<tag;
			bool newBold = ts.isBold || (tag == "b");
			bool newItalic = ts.isItalic || (tag == "i");
			bool newUnderline = ts.hasUnderline || (tag == "u");
			QString newColor = ts.color;
			double newFontSize = ts.fontsize;
			QString newFamily = ts.family;
			if (tag == "br")
			{
				// Soft line break within a paragraph. Emit as a separate segment
				// inheriting the current style context, so it stays inside the
				// paragraph rather than splitting it.
				TextSegment brTs(ts);
				brTs.text = QString(SpecialChars::LINEBREAK);
				segments.append(brTs);
				text += QChar(SpecialChars::LINEBREAK);
				continue;
			}
			if (tag == "font")
			{
				xmlChar *colorAttr = xmlGetProp(cur, (const xmlChar *)"color");
				if (colorAttr)
					newColor = QString::fromUtf8((const char*)colorAttr).trimmed();
				xmlFree(colorAttr);
				xmlChar *faceAttr = xmlGetProp(cur, (const xmlChar *)"face");
				QString fam;
				if (faceAttr)
					fam = QString::fromUtf8((const char*)faceAttr).trimmed();
				xmlFree(faceAttr);
				xmlChar *styleAttr = xmlGetProp(cur, (const xmlChar *)"style");
				QString styleData = styleAttr ? QString::fromUtf8((const char*)styleAttr) : "None";
				xmlFree(styleAttr);
				static QRegularExpression regexFontsize(R"(font-size:\s*([\d]+)pt)");
				static QRegularExpression regexFamily(R"((\"([^']+)\"|([\w-]+)))");
				QRegularExpressionMatch matchFS = regexFontsize.match(styleData);
				if (matchFS.hasMatch())
					newFontSize = matchFS.captured(1).toDouble() * 10.0;
				QRegularExpressionMatch matchFamily = regexFamily.match(fam);
				if (matchFamily.hasMatch())
					newFamily = matchFamily.captured(1);
			}

			TextSegment ts{QString(), newColor, newBold, newItalic, newUnderline, newFontSize, newFamily};
			QString innerText = html_LibreOffice_ExtractText(cur->children, segments, ts);
			text += innerText;
		}
	}
	return text;
}

void ScClipboardProcessor::html_LibreOffice_ParseTable(xmlNode *tableNode, ParsedTable &out)
{
	// Pre-pass: count columns from <col> children of <table>.
	int colCountFromCols = 0;
	for (xmlNode *cur = tableNode->children; cur; cur = cur->next)
	{
		if (cur->type == XML_ELEMENT_NODE && xmlStrcmp(cur->name, (const xmlChar *)"col") == 0)
			++colCountFromCols;
	}

	// Occupancy grid: occupied[row][col] = true when a cell has claimed that slot.
	QList<QList<bool>> occupied;

	auto ensureRow = [&](int r, int minCols)
	{
		while (occupied.size() <= r)
			occupied.append(QList<bool>());
		while (occupied[r].size() < minCols)
			occupied[r].append(false);
	};

	int rowIdx = 0;
	int observedMaxCols = 0;

	// Walk a container of <tr> rows (either <table> directly or <tbody>/<thead>/<tfoot>).
	auto walkRowContainer = [&](xmlNode *container)
	{
		for (xmlNode *rowNode = container->children; rowNode; rowNode = rowNode->next)
		{
			if (rowNode->type != XML_ELEMENT_NODE)
				continue;
			if (xmlStrcmp(rowNode->name, (const xmlChar *)"tr") != 0)
				continue;

			int col = 0;
			ensureRow(rowIdx, colCountFromCols > 0 ? colCountFromCols : 1);

			for (xmlNode *cellNode = rowNode->children; cellNode; cellNode = cellNode->next)
			{
				if (cellNode->type != XML_ELEMENT_NODE)
					continue;
				if (xmlStrcmp(cellNode->name, (const xmlChar *)"td") != 0 &&
						xmlStrcmp(cellNode->name, (const xmlChar *)"th") != 0)
					continue;

				// Advance past slots already occupied by prior rowspans or earlier cells.
				ensureRow(rowIdx, col + 1);
				while (col < occupied[rowIdx].size() && occupied[rowIdx][col])
					++col;

				// Read row/col span attributes.
				int rs = 1;
				int cs = 1;
				xmlChar *rsAttr = xmlGetProp(cellNode, (const xmlChar *)"rowspan");
				if (rsAttr)
				{
					rs = qMax(1, QString::fromUtf8((const char*)rsAttr).toInt());
					xmlFree(rsAttr);
				}
				xmlChar *csAttr = xmlGetProp(cellNode, (const xmlChar *)"colspan");
				if (csAttr)
				{
					cs = qMax(1, QString::fromUtf8((const char*)csAttr).toInt());
					xmlFree(csAttr);
				}

				// Mark the spanned rectangle occupied. Grow the grid as needed.
				for (int dr = 0; dr < rs; ++dr)
				{
					ensureRow(rowIdx + dr, col + cs);
					for (int dc = 0; dc < cs; ++dc)
						occupied[rowIdx + dr][col + dc] = true;
				}

				// Build the cell record.
				ParsedTableCell cell;
				cell.row = rowIdx;
				cell.column = col;
				cell.rowSpan = rs;
				cell.columnSpan = cs;

				for (xmlNode *pNode = cellNode->children; pNode; pNode = pNode->next)
				{
					if (pNode->type != XML_ELEMENT_NODE)
						continue;
					if (xmlStrcmp(pNode->name, (const xmlChar *)"p") != 0)
						continue;

					ParsedTableParagraph para;
					xmlChar *classAttr = xmlGetProp(pNode, (const xmlChar *)"class");
					if (classAttr)
					{
						para.className = QString::fromUtf8((const char*)classAttr);
						xmlFree(classAttr);
					}
					TextSegment seedTs;
					html_LibreOffice_ExtractText(pNode->children, para.segments, seedTs);
					cell.paragraphs.append(para);
				}

				// Fallback: spreadsheets (Calc) and some other sources don't wrap cell
				// content in <p>. Extract text directly from cellNode's children if no
				// <p>-derived paragraph was produced.
				if (cell.paragraphs.isEmpty())
				{
					ParsedTableParagraph para;
					TextSegment seedTs;
					html_LibreOffice_ExtractText(cellNode->children, para.segments, seedTs);
					if (!para.segments.isEmpty())
						cell.paragraphs.append(para);
				}

				out.cells.append(cell);
				observedMaxCols = qMax(observedMaxCols, col + cs);
				col += cs;
			}
			++rowIdx;
		}
	};

	// Walk <tbody>/<thead>/<tfoot> wrappers first.
	for (xmlNode *cur = tableNode->children; cur; cur = cur->next)
	{
		if (cur->type != XML_ELEMENT_NODE)
			continue;
		if (xmlStrcmp(cur->name, (const xmlChar *)"tbody") == 0 ||
				xmlStrcmp(cur->name, (const xmlChar *)"thead") == 0 ||
				xmlStrcmp(cur->name, (const xmlChar *)"tfoot") == 0)
		{
			walkRowContainer(cur);
		}
	}

	// Walk direct <tr> children of <table> (Writer's pattern).
	walkRowContainer(tableNode);

	out.rows = rowIdx;
	out.columns = qMax(colCountFromCols, observedMaxCols);
}

void ScClipboardProcessor::html_LibreOffice_ParseParagraphs(xmlNode *node, QMap<QString, QString> &styles)
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
			TextSegment ts;
			QString content = html_LibreOffice_ExtractText(cur->children, segments, ts);
			// qDebug() << "-----------------------------------";
			// qDebug() << "Paragraph Class:" << className;
			// qDebug() << "Text Content:" << content;


			ParagraphStyle currPstyle;
			if(m_doc->styleExists(className))
				currPstyle = m_doc->paragraphStyle(className);
			else
				currPstyle = m_pageItem->itemText.paragraphStyle();
			int pos = qMax(0, m_pageItem->itemText.cursorPosition());
			for (const auto &segment : std::as_const(segments))
			{
				QString style;
				QString currFamily(segment.family.isEmpty() ? currPstyle.charStyle().font().family() : segment.family);
				if (!segment.isBold && !segment.isItalic)
				{
					style = availableFonts.getRegularStyle(currFamily);
				}
				else
				{
					if (segment.isBold && !segment.isItalic)
						style = availableFonts.getBoldStyle(currFamily);
					else if (!segment.isBold && segment.isItalic)
						style = availableFonts.getItalicStyle(currFamily);
					else if (segment.isBold && segment.isItalic)
						style = availableFonts.getBoldItalicStyle(currFamily);
				}
				if (segment.hasUnderline)
				{
					int s = 0;
					s |= 8;
					currPstyle.charStyle().setFeatures(static_cast<StyleFlag>(s).featureList());
				}
				if (!segment.color.isEmpty())
				{
					ScColor newColor;
					newColor.fromQColor(QColor(segment.color));
					QString colorName = m_doc->PageColors.tryAddColor("FromCopy"+segment.color, newColor);
					currPstyle.charStyle().setFillColor(colorName);
				}
				if (segment.fontsize > 0.0)
					currPstyle.charStyle().setFontSize(segment.fontsize);
				const ScFace& face = availableFonts.findFont(currFamily, style);
				if (face != ScFace::none())
					currPstyle.charStyle().setFont(face);
				else
					qDebug()<<"No face found";
				m_pageItem->itemText.insertChars(pos, segment.text);
				m_pageItem->itemText.applyStyle(pos, currPstyle);
				m_pageItem->itemText.applyCharStyle(pos, segment.text.length(), currPstyle.charStyle());
				pos = m_pageItem->itemText.cursorPosition();
			}
			segments.clear();
			content.clear();
			//Add a new line after the paragraph
			pos = qMax(0, m_pageItem->itemText.cursorPosition());
			m_pageItem->itemText.insertChars(pos, SpecialChars::PARSEP);
			m_pageItem->itemText.applyStyle(pos, currPstyle);
		}
	}
}


void ScClipboardProcessor::html_MSFT_ParseParagraphs(xmlNode *node, QMap<QString, QString> &styles)
{
	SCFonts& availableFonts = PrefsManager::instance().appPrefs.fontPrefs.AvailFonts;
	QList<TextSegment> segments;
	for (xmlNode *cur = node; cur; cur = cur->next)
	{
		if (cur->type == XML_ELEMENT_NODE && xmlStrcmp(cur->name, (const xmlChar *)"span") == 0)
		{
			// qDebug() << "-----------------------------------";
			xmlChar *styleAttr = xmlGetProp(cur, (const xmlChar *)"style");
			// QString className = styleAttr ? QString::fromUtf8((const char*)styleAttr) : "None";
			// qDebug()<<"Style in para:"<<className;
			xmlFree(styleAttr);
		}
		if (cur->type == XML_ELEMENT_NODE)
		{
			QString tag = QString::fromUtf8((const char *)cur->name);
			if (tag != "p" && !htmlHeadingTags.contains(tag))
				continue;

			xmlChar *classAttr = xmlGetProp(cur, (const xmlChar *)"class");
			QString className = classAttr ? QString::fromUtf8((const char*)classAttr) : "None";
			xmlFree(classAttr);

			// For heading tags, fall back to the tag selector's style (e.g. "h1")
			// if the element has no class attribute. This lets <h1> pick up the
			// styling from the CSS `h1 { ... }` rule.
			QString styleName = className;
			if (className == "None" && htmlHeadingTags.contains(tag))
				styleName = "Heading " + tag.mid(1);

			TextSegment ts;
			QString content = html_MSFT_ExtractText(cur->children, segments, ts);
			// qDebug() << "-----------------------------------";
			// qDebug() << "Paragraph Class:" << className;
			// qDebug() << "Text Content:" << content;

			ParagraphStyle paraSeedStyle;
			if (m_doc->styleExists(styleName))
				paraSeedStyle = m_doc->paragraphStyle(styleName);
			else
				paraSeedStyle = m_pageItem->itemText.paragraphStyle();
			int pos = qMax(0, m_pageItem->itemText.cursorPosition());
			const QString defaultFillColor = paraSeedStyle.charStyle().fillColor();
			for (const auto &segment : std::as_const(segments))
			{
				ParagraphStyle currPstyle = paraSeedStyle;   // fresh per segment

				QString style;
				QString currFamily(segment.family.isEmpty() ? currPstyle.charStyle().font().family() : segment.family);
				if (!segment.isBold && !segment.isItalic)
					style = availableFonts.getRegularStyle(currFamily);
				else
				{
					if (segment.isBold && !segment.isItalic)
						style = availableFonts.getBoldStyle(currFamily);
					else if (!segment.isBold && segment.isItalic)
						style = availableFonts.getItalicStyle(currFamily);
					else if (segment.isBold && segment.isItalic)
						style = availableFonts.getBoldItalicStyle(currFamily);
				}

				// Underline: explicitly set bit 8 when on, clear when off, so it
				// doesn't carry over from a previous segment.
				int featureBits = 0;
				if (segment.hasUnderline)
					featureBits |= 8;
				currPstyle.charStyle().setFeatures(static_cast<StyleFlag>(featureBits).featureList());

				if (!segment.color.isEmpty())
				{
					ScColor newColor;
					newColor.fromQColor(QColor(segment.color));
					QString colorName = m_doc->PageColors.tryAddColor("FromCopy"+segment.color, newColor);
					currPstyle.charStyle().setFillColor(colorName);
				}
				else
				{
					// No color from the segment; explicitly set to the default so we
					// don't inherit a leftover color from a previous applyCharStyle.
					currPstyle.charStyle().setFillColor(defaultFillColor);
				}
				if (segment.fontsize > 0.0)
					currPstyle.charStyle().setFontSize(segment.fontsize);
				const ScFace& face = availableFonts.findFont(currFamily, style);
				if (face != ScFace::none())
					currPstyle.charStyle().setFont(face);
				else
					qDebug()<<"No face found";
				m_pageItem->itemText.insertChars(pos, segment.text);
				m_pageItem->itemText.applyStyle(pos, currPstyle);
				m_pageItem->itemText.applyCharStyle(pos, segment.text.length(), currPstyle.charStyle());
				pos = m_pageItem->itemText.cursorPosition();
			}
			segments.clear();
			content.clear();
			//Add a new line after the paragraph
			pos = qMax(0, m_pageItem->itemText.cursorPosition());
			m_pageItem->itemText.insertChars(pos, SpecialChars::PARSEP);
			m_pageItem->itemText.applyStyle(pos, paraSeedStyle);
		}
	}
}

void ScClipboardProcessor::html_MSFT_ParseTable(xmlNode *tableNode, ParsedTable &out)
{
	// Word does not emit <col> elements; column count is derived from
	// observed cell positions and spans.
	QList<QList<bool>> occupied;

	auto ensureRow = [&](int r, int minCols)
	{
		while (occupied.size() <= r)
			occupied.append(QList<bool>());
		while (occupied[r].size() < minCols)
			occupied[r].append(false);
	};

	int rowIdx = 0;
	int observedMaxCols = 0;

	auto walkRowContainer = [&](xmlNode *container)
	{
		for (xmlNode *rowNode = container->children; rowNode; rowNode = rowNode->next)
		{
			if (rowNode->type != XML_ELEMENT_NODE)
				continue;
			if (xmlStrcmp(rowNode->name, (const xmlChar *)"tr") != 0)
				continue;

			int col = 0;
			ensureRow(rowIdx, 1);

			for (xmlNode *cellNode = rowNode->children; cellNode; cellNode = cellNode->next)
			{
				if (cellNode->type != XML_ELEMENT_NODE)
					continue;
				if (xmlStrcmp(cellNode->name, (const xmlChar *)"td") != 0 &&
						xmlStrcmp(cellNode->name, (const xmlChar *)"th") != 0)
					continue;

				ensureRow(rowIdx, col + 1);
				while (col < occupied[rowIdx].size() && occupied[rowIdx][col])
					++col;

				int rs = 1;
				int cs = 1;
				xmlChar *rsAttr = xmlGetProp(cellNode, (const xmlChar *)"rowspan");
				if (rsAttr)
				{
					rs = qMax(1, QString::fromUtf8((const char*)rsAttr).toInt());
					xmlFree(rsAttr);
				}
				xmlChar *csAttr = xmlGetProp(cellNode, (const xmlChar *)"colspan");
				if (csAttr)
				{
					cs = qMax(1, QString::fromUtf8((const char*)csAttr).toInt());
					xmlFree(csAttr);
				}

				for (int dr = 0; dr < rs; ++dr)
				{
					ensureRow(rowIdx + dr, col + cs);
					for (int dc = 0; dc < cs; ++dc)
						occupied[rowIdx + dr][col + dc] = true;
				}

				ParsedTableCell cell;
				cell.row = rowIdx;
				cell.column = col;
				cell.rowSpan = rs;
				cell.columnSpan = cs;

				for (xmlNode *pNode = cellNode->children; pNode; pNode = pNode->next)
				{
					if (pNode->type != XML_ELEMENT_NODE)
						continue;
					if (xmlStrcmp(pNode->name, (const xmlChar *)"p") != 0)
						continue;

					ParsedTableParagraph para;
					xmlChar *classAttr = xmlGetProp(pNode, (const xmlChar *)"class");
					if (classAttr)
					{
						para.className = QString::fromUtf8((const char*)classAttr);
						xmlFree(classAttr);
					}
					TextSegment seedTs;
					html_MSFT_ExtractText(pNode->children, para.segments, seedTs);
					cell.paragraphs.append(para);
				}
				// Fallback: spreadsheets (Excel) and similar sources don't wrap cell content
				// in <p>. If no paragraphs came from <p> extraction, pull text directly from
				// the cell node, seeding from tag-level `td` CSS so default formatting applies.
				if (cell.paragraphs.isEmpty())
				{
					ParsedTableParagraph para;
					TextSegment seedTs;

					// Apply tag-level CSS for `td` (Excel emits cell defaults this way).
					if (cssStyles.contains("td"))
						applyMSFTCssStyleToSegment(cssStyles.value("td"), seedTs);

					// Apply class-level CSS from the <td>'s class attribute (Excel uses
					// .xl65, .xl66, etc. for per-cell overrides).
					xmlChar *classAttr = xmlGetProp(cellNode, (const xmlChar *)"class");
					if (classAttr)
					{
						QString className = QString::fromUtf8((const char*)classAttr);
						xmlFree(classAttr);
						if (cssStyles.contains(className))
							applyMSFTCssStyleToSegment(cssStyles.value(className), seedTs);
					}

					html_MSFT_ExtractText(cellNode->children, para.segments, seedTs);
					if (!para.segments.isEmpty())
						cell.paragraphs.append(para);
				}

				out.cells.append(cell);
				observedMaxCols = qMax(observedMaxCols, col + cs);
				col += cs;
			}
			++rowIdx;
		}
	};

	for (xmlNode *cur = tableNode->children; cur; cur = cur->next)
	{
		if (cur->type != XML_ELEMENT_NODE)
			continue;
		if (xmlStrcmp(cur->name, (const xmlChar *)"tbody") == 0 ||
				xmlStrcmp(cur->name, (const xmlChar *)"thead") == 0 ||
				xmlStrcmp(cur->name, (const xmlChar *)"tfoot") == 0)
		{
			walkRowContainer(cur);
		}
	}
	walkRowContainer(tableNode);

	out.rows = rowIdx;
	out.columns = observedMaxCols;
}

void ScClipboardProcessor::html_ApplyTable(const ParsedTable &pt)
{
	if (!m_tablePageItem || !m_doc)
		return;
	if (pt.rows <= 0 || pt.columns <= 0 || pt.cells.isEmpty())
		return;

	// Determine the paste origin in the destination table.
	const TableCell active = m_tablePageItem->activeCell();
	const int destRowOrigin = active.row();
	const int destColOrigin = active.column();

	// Expand the destination table if the pasted content would overflow.
	const int neededRows = destRowOrigin + pt.rows;
	const int neededCols = destColOrigin + pt.columns;

	if (neededRows > m_tablePageItem->rows())
		m_tablePageItem->insertRows(m_tablePageItem->rows(), neededRows - m_tablePageItem->rows());

	if (neededCols > m_tablePageItem->columns())
		m_tablePageItem->insertColumns(m_tablePageItem->columns(), neededCols - m_tablePageItem->columns());

	// Apply each parsed cell.
	for (const ParsedTableCell &src : pt.cells)
	{
		const int destRow = destRowOrigin + src.row;
		const int destCol = destColOrigin + src.column;
		const int numRows = src.rowSpan;
		const int numCols = src.columnSpan;

		// Apply merge if needed. Skip merging if it would exceed table bounds
		// (shouldn't happen after the expansion above, but defensive).
		if (numRows > 1 || numCols > 1)
		{
			if (destRow + numRows <= m_tablePageItem->rows() && destCol + numCols <= m_tablePageItem->columns())
				m_tablePageItem->mergeCells(destRow, destCol, numRows, numCols);
		}

		TableCell destCell = m_tablePageItem->cellAt(destRow, destCol);
		PageItem_TextFrame *destFrame = destCell.textFrame();
		if (!destFrame)
			continue;

		// Clear any existing content in the destination cell before applying.
		destFrame->itemText.clear();
		destFrame->itemText.setCursorPosition(0);

		html_ApplyParagraphsToFrame(destFrame, src.paragraphs);
	}

	m_tablePageItem->adjustTable();   // if such a method exists; check the header
	m_tablePageItem->updateClip();    // forces frame/clip recomputation
	m_tablePageItem->update();
}

void ScClipboardProcessor::html_ApplyParagraphsToFrame(PageItem_TextFrame *frame, const QList<ParsedTableParagraph> &paragraphs)
{
	if (!frame)
		return;

	for (int i = 0; i < paragraphs.size(); ++i)
	{
		const ParsedTableParagraph &para = paragraphs[i];

		ParagraphStyle currPstyle;
		if (m_doc->styleExists(para.className))
			currPstyle = m_doc->paragraphStyle(para.className);
		else
			currPstyle = frame->itemText.defaultStyle();

		// Detect Writer's empty-cell filler pattern: <p><br/></p> produces a
		// paragraph whose only content is LINEBREAK characters. Skip the
		// segment apply in that case — the paragraph break alone represents
		// an empty line correctly.
		bool fillerOnly = true;
		for (const TextSegment &s : para.segments)
		{
			for (const QChar &c : s.text)
			{
				if (c != QChar(SpecialChars::LINEBREAK))
				{
					fillerOnly = false;
					break;
				}
			}
			if (!fillerOnly)
				break;
		}

		if (!fillerOnly)
			html_ApplySegmentsToFrame(frame, para.segments, currPstyle);

		if (i + 1 < paragraphs.size())
		{
			int pos = qMax(0, frame->itemText.cursorPosition());
			frame->itemText.insertChars(pos, SpecialChars::PARSEP);
			frame->itemText.applyStyle(pos, currPstyle);
		}
	}
}

void ScClipboardProcessor::html_ApplySegmentsToFrame(PageItem_TextFrame *frame, const QList<TextSegment> &segments, const ParagraphStyle &seedStyle)
{
	SCFonts &availableFonts = PrefsManager::instance().appPrefs.fontPrefs.AvailFonts;

	int pos = qMax(0, frame->itemText.cursorPosition());
	for (const auto &segment : segments)
	{
		// Per-segment style starts fresh from the seed so attributes from
		// prior segments don't bleed into this one.
		ParagraphStyle currPstyle = seedStyle;

		QString currFamily(segment.family.isEmpty()
						   ? currPstyle.charStyle().font().family()
						   : segment.family);
		QString style;
		if (segment.isBold && segment.isItalic)
			style = availableFonts.getBoldItalicStyle(currFamily);
		else if (segment.isBold)
			style = availableFonts.getBoldStyle(currFamily);
		else if (segment.isItalic)
			style = availableFonts.getItalicStyle(currFamily);
		else
			style = availableFonts.getRegularStyle(currFamily);

		// Underline: set bit 8 when on, clear when off.
		int featureBits = 0;
		if (segment.hasUnderline)
			featureBits |= ScStyle_Underline;
		currPstyle.charStyle().setFeatures(static_cast<StyleFlag>(featureBits).featureList());

		if (!segment.color.isEmpty())
		{
			ScColor newColor;
			newColor.fromQColor(QColor(segment.color));
			QString colorName = m_doc->PageColors.tryAddColor("FromCopy" + segment.color, newColor);
			currPstyle.charStyle().setFillColor(colorName);
		}

		if (segment.fontsize > 0.0)
			currPstyle.charStyle().setFontSize(segment.fontsize);

		const ScFace &face = availableFonts.findFont(currFamily, style);
		if (face != ScFace::none())
			currPstyle.charStyle().setFont(face);

		frame->itemText.insertChars(pos, segment.text);
		frame->itemText.applyStyle(pos, currPstyle);
		frame->itemText.applyCharStyle(pos, segment.text.length(), currPstyle.charStyle());
		pos = frame->itemText.cursorPosition();
	}
}



bool ScClipboardProcessor::html_Cocoa_Process()
{
	// Convert to a const xmlChar*, parse with libxml2
	QByteArray utf8Content = m_content.toUtf8();
	const xmlChar* html_content_cstr = reinterpret_cast<const xmlChar*>(utf8Content.constData());

	// Parse the HTML content using libxml2's HTML parser
	htmlDocPtr doc = htmlReadDoc(html_content_cstr, nullptr, nullptr, HTML_PARSE_RECOVER | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);
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

	processed = true;
	return true;
}

void ScClipboardProcessor::html_Cocoa_Parse(xmlNodePtr node)
{
	if (!node)
		return;

	// Find <style> in the <head> area and parse styles
	for (xmlNode *cur = node->children; cur; cur = cur->next)
	{
		if (cur->type == XML_ELEMENT_NODE && xmlStrcmp(cur->name, (const xmlChar *)"head") == 0)
		{
			for (xmlNode *headChild = cur->children; headChild; headChild = headChild->next)
			{
				html_Cocoa_ParseStyles(headChild, cssStyles);
			}
		}
	}

	html_Cocoa_ProcessCSS(cssStyles);

	// Find <p> elements and extract text
	for (xmlNode *cur = node->children; cur; cur = cur->next)
	{
		if (cur->type == XML_ELEMENT_NODE && xmlStrcmp(cur->name, (const xmlChar *)"body") == 0)
		{
			html_Cocoa_ParseParagraphs(cur->children, cssStyles);
		}
	}
}

// Function to extract styles from <style>s in <head>
void ScClipboardProcessor::html_Cocoa_ParseStyles(xmlNode *node, QMap<QString, QString> &styles)
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

void ScClipboardProcessor::html_Cocoa_ProcessCSS(const QMap<QString, QString> &styles)
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
				if (availableFonts.fontMap.contains(fontName))
				{
					QStringList styles = availableFonts.fontMap.value(fontName);
					QString style;
					if (!styles.isEmpty())
						style = styles[0];
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
			}
			StyleSet<ParagraphStyle> tmpParagraphStyleSet;
			tmpParagraphStyleSet.create(newParaStyle);
			m_doc->redefineStyles(tmpParagraphStyleSet, false);
		}
	}
}

// Function to extract text content (including formatted text like <b> and <i>)
QString ScClipboardProcessor::html_Cocoa_ExtractText(xmlNode *node, QList<TextSegment> &segments, TextSegment ts)
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
				TextSegment ts2(ts);
				ts2.text = t;
				segments.append(ts2);
			}
		}
		else if (cur->type == XML_ELEMENT_NODE)
		{
			QString tag = QString::fromUtf8((const char *)cur->name);
			bool newBold = ts.isBold || (tag == "b");
			bool newItalic = ts.isItalic || (tag == "i");
			//unlikely this will work as underline is done with a span in Cocoa HTML
			bool newUnderline = ts.hasUnderline || (tag == "u");
			TextSegment ts{QString(), QString(), newBold, newItalic, newUnderline, 0.0, QString()};
			QString innerText = html_Cocoa_ExtractText(cur->children, segments, ts);

			text += innerText;
		}
	}
	return text;
}

// Function to parse paragraphs and their styles
void ScClipboardProcessor::html_Cocoa_ParseParagraphs(xmlNode *node, QMap<QString, QString> &styles)
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
			TextSegment ts;
			QString content = html_Cocoa_ExtractText(cur->children, segments, ts);
			// qDebug() << "-----------------------------------";
			// qDebug() << "Paragraph Class:" << className;
			// qDebug() << "Text Content:" << content;

			ParagraphStyle currPstyle;
			if(m_doc->styleExists(className))
				currPstyle = m_doc->paragraphStyle(className);
			else
				currPstyle = m_pageItem->itemText.paragraphStyle();
			int pos = qMax(0, m_pageItem->itemText.cursorPosition());
			for (const auto &segment : std::as_const(segments))
			{
				QString style;
				QString currFamily(currPstyle.charStyle().font().family());
				if (!segment.isBold && !segment.isItalic)
				{
					style = availableFonts.getRegularStyle(currFamily);
				}
				else
				{
					if (segment.isBold && !segment.isItalic)
						style = availableFonts.getBoldStyle(currFamily);
					else if (!segment.isBold && segment.isItalic)
						style = availableFonts.getItalicStyle(currFamily);
					else if (segment.isBold && segment.isItalic)
						style = availableFonts.getBoldItalicStyle(currFamily);
				}
				if (segment.hasUnderline)
				{
					int s = 0;
					s |= 8;
					currPstyle.charStyle().setFeatures(static_cast<StyleFlag>(s).featureList());
				}
				const ScFace& face = availableFonts.findFont(currFamily, style);
				if (face != ScFace::none())
					currPstyle.charStyle().setFont(face);
				else
					qDebug()<<"No face found";
				m_pageItem->itemText.insertChars(pos, segment.text);
				m_pageItem->itemText.applyStyle(pos, currPstyle);
				m_pageItem->itemText.applyCharStyle(pos, segment.text.length(), currPstyle.charStyle());
				pos = m_pageItem->itemText.cursorPosition();
			}
			segments.clear();
			content.clear();
			//Add a new line after the paragraph
			pos = qMax(0, m_pageItem->itemText.cursorPosition());
			m_pageItem->itemText.insertChars(pos, SpecialChars::PARSEP);
			m_pageItem->itemText.applyStyle(pos, currPstyle);
		}
	}
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

void ScClipboardProcessor::setDestTable(PageItem_Table *table)
{
	m_tablePageItem = table;
}
