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
	rules = 0;
}

void LatexHighlighter::highlightBlock(const QString &text)
{
	if (!rules) return;
	foreach (LatexHighlighterRule *rule, *rules) {
		int index = text.indexOf(rule->regex);
		while (index >= 0) {
			int length;
			if (rule->regex.numCaptures() == 0) {
				length = rule->regex.matchedLength();
			} else {
				length = rule->regex.cap(1).length();
				index = rule->regex.pos(1);
			}
			if (length == 0) {
				qWarning() << "Highlighter pattern" << rule->regex.pattern() << "matched a zero length string. This would lead to an infinite loop. Aborting. Please fix this pattern!";
				break;
			}
			setFormat(index, length, rule->format);
			index = text.indexOf(rule->regex, index + length);
		}
	}
}

QString LatexConfigParser::configBase()
{
	return ScPaths::instance().shareDir() + "/editorconfig/";
}

QString LatexConfigParser::absoluteFilename(QString fn)
{
	QFileInfo fi(fn);
	if (!fi.exists()) {
		return configBase() + fn;
	} else {
		return fn;
	}
}

//TODO: Pass this information to LatexEditor, so the second parser can be removed
bool LatexConfigParser::parseConfigFile(QString fn)
{
	fn = absoluteFilename(fn);
	m_error = "";
	m_filename = fn;
	QFile f(fn);
	if (!f.open(QIODevice::ReadOnly)) {
		QMessageBox::critical(0, QObject::tr("Error"), "<qt>" + 
				QObject::tr("Opening the configfile %1 failed! %2").arg(
						fn, f.errorString())
				+ "</qt>", 1, 0, 0);
	}
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
			parseHighlighter();
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

bool LatexConfigParser::StrRefToBool(const QStringRef &str) const
{
	if (str == "1" || str == "true") return true;
	if (str == "0" || str == "false" || str.isEmpty()) return false;
	qWarning() << "Invalid bool string:" << str.toString();
	return false;
}

void LatexConfigParser::parseHighlighter()
{
	foreach (LatexHighlighterRule *rule, highlighterRules) {
		delete rule;
	}
	highlighterRules.clear();
	while (!xml.atEnd()) {
		xml.readNext();
		if (xml.isWhitespace() || xml.isComment()) continue;
		if (xml.isEndElement() && xml.name() == "highlighter") break;
		if (xml.isEndElement() && xml.name() == "rule") continue;
		if (!xml.isStartElement() || xml.name() != "rule") {
			formatError("Unexpected element in <highlighter>: "+
				xml.name().toString()+", Token String: "+
				xml.tokenString());
			continue;
		}
		QString regex = xml.attributes().value("regex").toString();
		bool bold = StrRefToBool(xml.attributes().value("bold"));
		bool italic = StrRefToBool(xml.attributes().value("italic"));
		bool underline = StrRefToBool(xml.attributes().value("underline"));
		bool minimal = StrRefToBool(xml.attributes().value("minimal"));
		QString colorStr = xml.attributes().value("color").toString();
		QColor color(colorStr);
		if (!color.isValid()) {
			color.fromRgb(0, 0, 0); //Black
			if (!colorStr.isEmpty())
				qWarning() << "Invalid color:" << colorStr;
		}
		LatexHighlighterRule *newRule = new LatexHighlighterRule();
		newRule->format.setForeground(color);
		newRule->format.setFontItalic(italic);
		if (bold) {
			newRule->format.setFontWeight(QFont::Bold);
		}
		newRule->format.setFontUnderline(underline);
		newRule->regex.setPattern(regex);
		newRule->regex.setMinimal(minimal);
		highlighterRules.append(newRule);
	}
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
	QDir dir(LatexConfigParser::configBase());
	QStringList files;
	files = dir.entryList(QStringList("*.xml"));
	files.sort();
	int i;
	for (i = 0; i < files.size(); i++) {
		if (files[i].compare("sample.xml",Qt::CaseInsensitive)==0) {
			files.removeAt(i);
			i--;
		}
	}
	return files;
}
