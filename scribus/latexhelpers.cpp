/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
						latexhelpers.cpp  -  description
						-------------------
copyright            : Scribus Team
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/


#include "latexhelpers.h"

#include <QDebug>
#include <QFile>
#include <QMessageBox>

#include "prefsmanager.h"
#include "scpaths.h"


LatexHighlighter::LatexHighlighter(QTextDocument *document)
	: QSyntaxHighlighter(document)
{
	QTextCharFormat mathFormat;
	mathFormat.setForeground(QColor(0, 128, 0));
	mathFormat.setFontWeight(QFont::Bold);
	setFormatFor(Math, mathFormat);

	QTextCharFormat commandFormat;
	commandFormat.setForeground(QColor(255, 0, 0));
	setFormatFor(Command, commandFormat);
	
	QTextCharFormat blockNameFormat;
	blockNameFormat.setForeground(QColor(0, 0, 255));
	setFormatFor(BlockName, blockNameFormat);
	
	QTextCharFormat scribusFormat;
	scribusFormat.setForeground(QColor(192, 16, 112));
	scribusFormat.setFontWeight(QFont::Bold);
	setFormatFor(ScribusSpecial, scribusFormat);

	QTextCharFormat commentFormat;
	commentFormat.setForeground(QColor(128, 128, 128));
	commentFormat.setFontItalic(true);
	setFormatFor(Comment, commentFormat);
}

void LatexHighlighter::setFormatFor(Construct construct, const QTextCharFormat &format)
{
	m_formats[construct] = format;
	rehighlight();
}


void LatexHighlighter::highlightBlock(const QString &text)
{
	int state = previousBlockState();
	int len = text.length();
	int start = 0;
	int pos = 0;
	while (pos < len) {
		switch (state) {
			case NormalState:
			default:
				while (pos < len) {
					QChar ch = text.at(pos);
					if (ch == '%') {
						//Comment
						setFormat(pos, len - pos, m_formats[Comment]);
						pos = len; //Do not handle other formats in comments
						break;
					} else if (ch == '\\') {
						start = pos;
						pos++;
						if (pos >= len) break;
						ch = text.at(pos);
						if (ch == '[') {
							state = InMath;
							break;
						} else if ((text.mid(pos, 5) == "begin") || (text.mid(pos, 3) == "end")) {
							int saved_pos = pos;
							if (text.at(pos) == 'b') {
								setFormat(pos-1, 6, m_formats[Command]);
								pos += 5;
							} else {
								setFormat(pos-1, 4, m_formats[Command]);
								pos += 3;
							}
							
							while (pos < len && text.at(pos++).isSpace()) /*Skip over whitespace */;
							if (text.at(pos-1) == '{') {
								start = pos;
								while (pos < len && text.at(pos++) != '}') /*Do nothing */;
								pos--;
								setFormat(start, pos - start, m_formats[BlockName]);
							} else {
								pos = saved_pos;
							}
						} else {
							while (pos < len) {
								ch = text.at(pos++);
								//Allowed chars are A-Za-z@
								if (!(((ch >= 'A') && (ch <= 'Z')) ||
									((ch >= 'a') && (ch <= 'z')) || (ch == '@'))) break;
							}
							if ((pos - start) == 2) {
								//Special char
								setFormat(start, pos - start, m_formats[ScribusSpecial]);
							} else {
								setFormat(start, pos - start -1, m_formats[Command]);
							}
						}
						break;
					} else if (ch == '$') {
						if (text.mid(pos, 9) == "$scribus_") {
							//Scribus special code
							start = pos;
							pos++;
							while (pos < len && text.at(pos++) != '$') /*Do nothing */;
							setFormat(start, pos - start, m_formats[ScribusSpecial]);
						} else {
							state = InDisplayMath;
							pos++;
							break;
						}
					}
					pos++;
				}
				break;
			case InDisplayMath:
				start = pos;
				if (start > 0) start--;
				while (pos < len) {
					if (text.at(pos) == '$') {
						pos++;
						state = NormalState;
						break;
					}
					pos++;
				}
				setFormat(start, pos - start, m_formats[Math]);
				break;
			case InMath:
				start = pos;
				if (start > 0) start--;
				while (pos < len) {
					if (text.mid(pos,2) == "\\]" ) {
						pos += 2;
						state = NormalState;
						break;
					}
					pos++;
				}
				setFormat(start, pos - start, m_formats[Math]);
				break;
		}
	}
	setCurrentBlockState(state);
}

//TODO: Pass this information to LatexEditor, so the second parser can be removed
bool LatexConfigParser::parseConfigFile(QString fn)
{
	m_error = "";
	m_filename = fn;
	QFile f(fn);
	f.open(QIODevice::ReadOnly);
	xml.setDevice(&f);
	
	while (!xml.atEnd()) {
		xml.readNext();
		if (xml.isWhitespace() || xml.isComment() || xml.isStartDocument() || xml.isEndDocument()) continue;
		if (xml.isStartElement() && xml.name() == "editorsettings") {
			m_description = xml.attributes().value("description").toString();
			m_icon = xml.attributes().value("icon").toString();
			if (m_description.isEmpty()) {
				m_description = fn;
			}
			parseElements();
		} else {
			formatError("Unexpected element at root level"+xml.name().toString()+", Token String: "+
								 xml.tokenString());
		}
	}
	if (xml.hasError()) {
		formatError(xml.errorString());
	}
	f.close();
	if (!m_error.isEmpty()) {
		return false;
	}
	return true;
}

void LatexConfigParser::parseElements()
{
	while (!xml.atEnd()) {
		xml.readNext();
		if (xml.isEndElement() && xml.name() == "editorsettings") break;
		if (xml.isWhitespace() || xml.isComment() || xml.isEndElement()) continue;
		if (!xml.isStartElement()) {
			formatError("Unexpected element in <editorsettings>"+xml.name().toString()+", Token String: "+
								 xml.tokenString());
			continue;
		}
	
		if (xml.name() == "executable") {
			m_executable = xml.attributes().value("command").toString();
		} else if (xml.name() == "imagefile") {
			m_imageExtension = xml.attributes().value("extension").toString();
		} else if (xml.name() == "highlighter") {
			m_highlight_kate = xml.attributes().value("kate").toString();
			m_highlight_file = xml.attributes().value("file").toString();
		} else if (xml.name() == "empty-frame-text") {
			m_emptyFrameText = xml.readI18nText(true);
		} else if (xml.name() == "preamble") {
			m_preamble = xml.readElementText();
		} else if (xml.name() == "postamble") {
			m_postamble = xml.readElementText();
		} else if (xml.name() == "tab") {
			parseTab();
		} else {
			formatError("Unknown tag in <editorsettings>: "+xml.name().toString());
		}
	}
}

void LatexConfigParser::formatError(QString message)
{
	QString new_error = QString::number(xml.lineNumber()) + ":" + 
			QString::number(xml.columnNumber()) + ":" + message;
	qWarning() << m_filename << new_error;
	m_error += new_error + "\n";
}

void LatexConfigParser::parseTab()
{
	QString type = xml.attributes().value("type").toString();
	bool itemstab = (type == "items");
	QString title = "";
	QString name, text, default_value;
	
	while (!xml.atEnd()) {
		xml.readNext();
		if (xml.isWhitespace() || xml.isComment()) continue;
		if (xml.isEndElement() && xml.name() == "tab") break;
		if (!xml.isStartElement()) {
			formatError("Unexpected element in <tab>: "+xml.name().toString()+", Token String: "+
								 xml.tokenString());
			continue;
		}
		if (xml.name() == "title") {
			if (!title.isEmpty()) {
				formatError("Second <title> tag in <tab>");
			}
			title = xml.readI18nText();
		} else if (xml.name() == "item") {
			if (!itemstab) {
				formatError("Found <item> in a 'settings'-tab!");
			}
			QString value = xml.attributes().value("value").toString();
			QString img = xml.attributes().value("image").toString();
			text = xml.readI18nText();
		} else if (xml.name() == "comment" || xml.name() == "font" 
						 || xml.name() == "spinbox" || xml.name() == "color" 
								 || xml.name() == "text" || xml.name() == "list") {
			//TODO: Store this + attributes in a list
							 QString tagname = xml.name().toString();
							 name = xml.attributes().value("name").toString();
							 default_value = xml.attributes().value("default").toString();
							 if (xml.name() != "list") {
								 text = xml.readI18nText();
							 } else {
								 ignoreList();
							 }
							 if (!name.isEmpty()) {
								 if (properties.contains(name)) {
									 formatError("Redeclared setting with name: " + name);
								 } else {
									 properties.insert(name, default_value);
								 }
							 }
							 //TODO: qDebug() << "For future use:" << tagname << name << text << default_value;
								 } else {
									 formatError("Unexpected element in <tab>: " + xml.name().toString());
								 }
	}
	
	if (title.isEmpty()) {
		formatError("Tab ended here, but no title was found!");
	}
}

void LatexConfigParser::ignoreList()
{
	//TODO: Quick hack to avoid real parsing
	while (!xml.atEnd()) {
		xml.readNext();
		if (xml.isEndElement() && xml.name() == "list") break;
	}
}

QString LatexConfigParser::executable() const
{
	QString command = PrefsManager::instance()->latexCommands()[m_filename];
	if (command.isEmpty()) {
		return m_executable;
	} else {
		return command;
	}
}

QString I18nXmlStreamReader::readI18nText(bool unindent)
{
	QString language = PrefsManager::instance()->guiLanguage();
	QString result;
	int matchquality = 0;
	bool i18n = false;
	if (!isStartElement()) raiseError("readI18nText called without startelement!");
	
	QString startTag = name().toString();
	while (!atEnd()) {
		readNext();
		if (isWhitespace() || isComment()) continue;
		if (isStartElement() && name() == startTag) {
			raiseError("Invalid nested elements.");
			return "Error";
		}
		if (isEndElement() && name() == startTag) {
			if (!unindent) {
				return result.trimmed();
			} else {
				QStringList splitted = result.split("\n");
				int i;
				int minspaces = 0xffff;
				/* NOTE: First line contains no leading whitespace so we start at 1 */
				for (i = 1; i < splitted.size(); i++) {
					if (splitted[i].trimmed().isEmpty()) continue;
					int spaces;
					QString tmp = splitted[i];
					for (spaces = 0; spaces < tmp.length(); spaces++) {
						if (!tmp[spaces].isSpace()) break;
					}
					if (spaces < minspaces) minspaces = spaces;
				}
				for (i = 1; i < splitted.size(); i++) {
					splitted[i] = splitted[i].mid(minspaces);
				}
				return splitted.join("\n").trimmed();
			}
		}
		if (i18n) {
			if (isEndElement()) {
				if (name() == "i18n") {
					i18n = false;
				} else {
					raiseError("Invalid end element "+ name().toString());
				}
				continue;
			}
			if (!isStartElement()) {
				raiseError("Unexpected data!");
			}
			if (name() == language) {
				matchquality = 2; //Perfect match
				result = readElementText();
			} else if (language.startsWith(name().toString()) && matchquality <= 1) {
				matchquality = 1; //Only beginning part matches
				result = readElementText();
			} else if (result.isEmpty()) {
				matchquality = 0;
				result = readElementText();
			} else {
				readElementText(); //Ignore the text
			}
		} else {
			if (isStartElement()) {
				if (name() == "i18n") {
					i18n = true;
					continue;
				} else {
					raiseError("Tag " + name().toString() +
							"found, but \"i18n\" or string data expected.");
					continue;
				}
			} else if (isCharacters()) {
				result = result + text().toString();
			}
		}
	}
	raiseError("Unexpected end of XML file");
	return result;
}

LatexConfigCache* LatexConfigCache::_instance = 0;

LatexConfigCache* LatexConfigCache::instance()
{
	if (!_instance) {
		_instance = new LatexConfigCache();
	}
	return _instance;
}

LatexConfigParser* LatexConfigCache::parser(QString filename, bool warnOnError)
{
	if (parsers.contains(filename)) {
		if (warnOnError && error[filename]) {
			//Recreate element as error might have been fixed.
			delete parsers[filename];
			createParser(filename, warnOnError);
		}
	} else {
		createParser(filename, warnOnError);
	}
	return parsers[filename];
}


void LatexConfigCache::createParser(QString filename, bool warnOnError)
{
	LatexConfigParser *parser = new LatexConfigParser();
	bool hasError = !parser->parseConfigFile(filename);
	parsers[filename] = parser;
	error[filename] = hasError;
	if (hasError) {
		QMessageBox::critical(0, QObject::tr("Error"), "<qt>" + 
				QObject::tr("Parsing the configfile %1 failed! Depending on the type of the error "
						"render frames might not work correctly!\n%2").arg(
						filename, parser->error())
						+ "</qt>", 1, 0, 0);
	}
}

bool LatexConfigCache::hasError(QString filename)
{
	if (!error.contains(filename)) {
		return true;
	}
	return error[filename];
}

QStringList LatexConfigCache::defaultConfigs()
{
	QString base = ScPaths::instance().shareDir()+"/editorconfig/";
	QDir dir(base);
	QStringList files;
	files = dir.entryList(QStringList("*.xml"));
	files.sort();
	int i;
	for (i = 0; i < files.size(); i++) {
		if (files[i].compare("sample.xml",Qt::CaseInsensitive)==0) {
			files.removeAt(i);
			i--;
		} else {
			files[i] = base + files[i];
		}
	}
	return files;
}
