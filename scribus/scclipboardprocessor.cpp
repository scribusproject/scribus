#include "scclipboardprocessor.h"


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
	QString text (QApplication::clipboard()->text(QClipboard::Clipboard));
	text.replace("\r\n", SpecialChars::PARSEP);
	text.replace('\n', SpecialChars::PARSEP);
	m_result = text;
	processed = true;
	return true;
}

bool ScClipboardProcessor::processHTML()
{
	QTextDocument qTextDoc;
	qTextDoc.setHtml(m_content);

	// qDebug()<<qTextDoc.defaultTextOption();
	// qDebug()<<qTextDoc.defaultFont().family();
	// qDebug()<<qTextDoc.defaultFont().pointSizeF();
	// qDebug()<<qTextDoc.defaultFont().style();


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
