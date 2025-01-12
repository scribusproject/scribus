#include "scclipboardprocessor.h"

#include <QApplication>
#include <QGuiApplication>
#include <QClipboard>
#include <QMimeData>
#include <QDomDocument>
#include <QXmlStreamReader>


ScClipboardProcessor::ScClipboardProcessor()
{
}

ScClipboardProcessor::ScClipboardProcessor(const QString &content, ContentType type)
{
	m_content = content;
	m_contentType = type;
}

void ScClipboardProcessor::setContent(const QString &content, ContentType type)
{
	m_content = content;
	m_contentType = type;
	processed = false;
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
	return true;
}

bool ScClipboardProcessor::processHTML()
{
	//Something from Apple Pages (just a first example)
	if (m_content.contains("Cocoa HTML Writer"))
		return processHTML_Cocoa();
	//Something from Microsoft Word (just a first example, maybe all of Microsoft's apps)
	if (m_content.contains("schemas-microsoft-com"))
		return processHTML_MSFT();
	return processHTML_Other();
}

bool ScClipboardProcessor::processHTML_MSFT()
{
	QTextDocument qTextDoc;
	qTextDoc.setHtml(m_content);

	// qDebug()<<qTextDoc.defaultTextOption();
	// qDebug()<<qTextDoc.defaultFont().family();
	// qDebug()<<qTextDoc.defaultFont().pointSizeF();
	// qDebug()<<qTextDoc.defaultFont().style();

	/*
	// Create a QTextCursor to traverse the document
	QTextCursor cursor(&qTextDoc);

	// Traverse all the blocks in the document
	while (!cursor.atEnd())
	{
		if (!cursor.movePosition(QTextCursor::NextBlock))
			break;

		// Traverse all the characters in the current block
		QTextBlock block = cursor.block();
		// qDebug()<<"Block:"<<block.text();
		QTextCursor blockCursor(block);
		while (!blockCursor.atEnd())
		{
			if(!blockCursor.movePosition(QTextCursor::NextCharacter))
				break;
			// Get the character's formatting
			QTextCharFormat charFormat = blockCursor.charFormat();

			// Extract and print the character's style attributes
			// qDebug() << "Character: " << blockCursor.selectedText();
			// qDebug() << "Font Family: " << charFormat.fontFamilies().toString();
			// qDebug() << "Font Size: " << charFormat.fontPointSize();
			// qDebug() << "Font Weight: " << charFormat.fontWeight();
			// qDebug() << "Font Style: " << charFormat.fontItalic();
			// qDebug() << "Text Color: " << charFormat.foreground().color();
			// qDebug() << "Underline: " << charFormat.fontUnderline();
			// qDebug() << "StrikeOut: " << charFormat.fontStrikeOut();
		}
	}
	*/
	//temporary result for 1.7.0
	m_result = qTextDoc.toPlainText();
	m_result.replace("\r\n", SpecialChars::PARSEP);
	m_result.replace('\n', SpecialChars::PARSEP);
	processed = true;
	return true;
}

bool ScClipboardProcessor::processHTML_Cocoa()
{
	QTextDocument qTextDoc;
	qTextDoc.setHtml(m_content);

	//Find Style Sheets
	/*
	QXmlStreamReader xmlReader(m_content);

	QString cssContent;
	bool inStyleTag = false;

	while (!xmlReader.atEnd())
	{
		// Read the next token
		xmlReader.readNext();

		if (xmlReader.isStartElement())
		{
			// Check if we encounter the <style> tag
			if (xmlReader.name() == "style")
				inStyleTag = true;
		}
		else if (xmlReader.isEndElement())
		{
			// Check if we encounter the </style> tag
			if (xmlReader.name() == "style")
				inStyleTag = false;
		}

		// If we are inside a <style> tag, collect the content
		if (inStyleTag && xmlReader.isCharacters())
			cssContent += xmlReader.text().toString();
	}

	//Apple Pages copied contet is not XML compliant it seems.. why check for errors?
	// Check for parsing errors
	// if (xmlReader.hasError()) {
	// 	qDebug() << "Error parsing HTML:" << xmlReader.errorString();
	// }

	// Output the extracted CSS content
	if (!cssContent.isEmpty())
	{
		QDebug debug = qDebug();
		debug.noquote();
		debug << "Extracted CSS from <style> tag:";
		debug << cssContent;
	}
	else
		qDebug() << "No CSS found in <style> tag.";



	// qDebug()<<qTextDoc.defaultTextOption();
	// qDebug()<<qTextDoc.defaultFont().family();
	// qDebug()<<qTextDoc.defaultFont().pointSizeF();
	// qDebug()<<qTextDoc.defaultFont().style();



	QTextBlock block = qTextDoc.begin();
	while (block.isValid())
	{
		// Iterate over all the fragments in the block
		QTextBlock::iterator it = block.begin();

		while (it != block.end())
		{
			QTextFragment fragment = it.fragment();
			QTextCharFormat charFormat = fragment.charFormat();

			bool isBold = charFormat.fontWeight() == QFont::Bold;
			bool isItalic = charFormat.fontItalic();
			if (isBold && isItalic)
			{
				qDebug() << "Bold and Italic text detected: " << fragment.text();
			} else if (isBold) {
				qDebug() << "Bold text detected: " << fragment.text();
			} else if (isItalic) {
				qDebug() << "Italic text detected: " << fragment.text();
			} else
				qDebug() << "Normal text detected: " << fragment.text();
			++it;
		}

		// Move to the next block
		block = block.next();
	}
	*/
	//temporary result for 1.7.0
	m_result = qTextDoc.toPlainText();
	m_result.replace("\r\n", SpecialChars::PARSEP);
	m_result.replace('\n', SpecialChars::PARSEP);
	processed = true;
	return true;
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
	processed = false;
}

void ScClipboardProcessor::dumpClipboardData()
{
	qDebug()<<QApplication::clipboard()->mimeData()->text();
	qDebug()<<QApplication::clipboard()->mimeData()->html();
	qDebug()<<QApplication::clipboard()->mimeData()->formats();
}
