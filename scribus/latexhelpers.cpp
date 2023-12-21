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
#include <QFileInfo>
#include <QDir>
#include <QMessageBox>

#include "prefsmanager.h"
#include "scpaths.h"
#include "ui/scmessagebox.h"

LatexHighlighter::LatexHighlighter(QTextDocument *document)
	: QSyntaxHighlighter(document)
{
	m_rules = nullptr;
}

void LatexHighlighter::highlightBlock(const QString &text)
{
	//This is required to fix a Qt incompatibility. See error message below for details.
	static bool disable_highlighting = false;
	if (disable_highlighting)
		return;
	if (!m_rules)
		return;
	foreach (LatexHighlighterRule *rule, *m_rules)
	{
		QRegularExpressionMatch match = rule->regex.match(text);
		int n = 0;
		int index = match.capturedStart(n);
		while (index >= 0)
		{
			int length = match.capturedLength(n);
			index = match.capturedStart(n);
			if (length == 0)
			{
				qWarning() << "Highlighter pattern" << rule->regex.pattern() << "matched a zero length string. This would lead to an infinite loop. Aborting. Please fix this pattern!";
				break;
			}
			setFormat(index, length, rule->format);
			index = match.capturedStart(++n);
		}
	}
}

QString LatexConfigParser::configBase()
{
	return ScPaths::instance().shareDir() + "/editorconfig/";
}

QString LatexConfigParser::absoluteFilename(const QString& fn)
{
	if (!QFileInfo::exists(fn))
		return configBase() + fn;
	return fn;
}

//TODO: Pass this information to LatexEditor, so the second parser can be removed
bool LatexConfigParser::parseConfigFile(QString fn)
{
	fn = absoluteFilename(fn);
	m_error.clear();
	m_filename = fn;

	QFile f(fn);
	if (!f.open(QIODevice::ReadOnly))
	{
		ScMessageBox::critical(nullptr, QObject::tr("Error"), "<qt>" +
				QObject::tr("Opening the configfile %1 failed! %2").arg(
						fn, f.errorString())
				+ "</qt>");
	}

	I18nXmlStreamReader xml;
	xml.setDevice(&f);
	
	while (!xml.atEnd())
	{
		xml.readNext();
		if (xml.isWhitespace() || xml.isComment() || xml.isStartDocument() || xml.isEndDocument())
			continue;
		if (xml.isStartElement() && xml.name() == QLatin1String("editorsettings"))
		{
			m_description = xml.attributes().value("description").toString();
			m_icon = xml.attributes().value("icon").toString();
			if (m_description.isEmpty())
				m_description = fn;
			parseElements(xml);
		}
		else
		{
			formatUnexpectedElemError("root level", xml);
		}
	}
	if (xml.hasError())
		formatError(xml.errorString(), xml);
	f.close();
	return m_error.isEmpty();
}

void LatexConfigParser::parseElements(I18nXmlStreamReader& xml)
{
	while (!xml.atEnd())
	{
		xml.readNext();
		if (xml.isEndElement() && xml.name() == QLatin1String("editorsettings"))
			break;
		if (xml.isWhitespace() || xml.isComment() || xml.isEndElement())
			continue;
		if (!xml.isStartElement())
		{
			formatUnexpectedElemError("editorsettings", xml);
			continue;
		}
	
		if (xml.name() == QLatin1String("executable"))
			m_executable = xml.attributes().value("command").toString();
		else if (xml.name() == QLatin1String("imagefile"))
			m_imageExtension = xml.attributes().value("extension").toString();
		else if (xml.name() == QLatin1String("highlighter"))
			parseHighlighter(xml);
		else if (xml.name() == QLatin1String("empty-frame-text"))
			m_emptyFrameText = xml.readI18nText(true);
		else if (xml.name() == QLatin1String("preamble"))
			m_preamble = xml.readElementText();
		else if (xml.name() == QLatin1String("postamble"))
			m_postamble = xml.readElementText();
		else if (xml.name() == QLatin1String("tab"))
			parseTab(xml);
		else
		{
			QString formatMsg = "Unknown tag in <editorsettings>: " + xml.name().toString();
			formatError(formatMsg, xml);
		}
	}
}

void LatexConfigParser::formatError(const QString& message, const I18nXmlStreamReader& xml)
{
	QString new_error = QString::number(xml.lineNumber()) + ":" + 
			QString::number(xml.columnNumber()) + ":" + message;
	qWarning() << m_filename << new_error;
	m_error += new_error + "\n";
}

void LatexConfigParser::formatUnexpectedElemError(const QString& elemName, const I18nXmlStreamReader& xml)
{
	QString errorMessage = QString("Unexpected element in <%1>: %2, Token String: %3")
		.arg(elemName).arg(xml.name().toString()).arg(xml.tokenString());
	formatError(errorMessage, xml);
}

bool LatexConfigParser::StrViewToBool(const QStringView& str) const
{
	if (str == QLatin1String("1") || str == QLatin1String("true"))
		return true;
	if (str == QLatin1String("0") || str == QLatin1String("false") || str.isEmpty())
		return false;
	qWarning() << "Invalid bool string:" << str.toString();
	return false;
}

void LatexConfigParser::parseHighlighter(I18nXmlStreamReader& xml)
{
	foreach (LatexHighlighterRule *rule, highlighterRules)
		delete rule;
	highlighterRules.clear();

	while (!xml.atEnd())
	{
		xml.readNext();
		if (xml.isWhitespace() || xml.isComment())
			continue;
		if (xml.isEndElement() && xml.name() == QLatin1String("highlighter"))
			break;
		if (xml.isEndElement() && xml.name() == QLatin1String("rule"))
			continue;
		if (!xml.isStartElement() || xml.name() != QLatin1String("rule"))
		{
			formatUnexpectedElemError("highlighter", xml);
			continue;
		}
		QString regex = xml.attributes().value("regex").toString();
		bool bold = StrViewToBool(xml.attributes().value("bold"));
		bool italic = StrViewToBool(xml.attributes().value("italic"));
		bool underline = StrViewToBool(xml.attributes().value("underline"));
		bool minimal = StrViewToBool(xml.attributes().value("minimal"));
		QString colorStr = xml.attributes().value("color").toString();
		QColor color(colorStr);
		if (!color.isValid())
		{
			color.fromRgb(0, 0, 0); //Black
			if (!colorStr.isEmpty())
				qWarning() << "Invalid color:" << colorStr;
		}
		LatexHighlighterRule *newRule = new LatexHighlighterRule();
		newRule->format.setForeground(color);
		newRule->format.setFontItalic(italic);
		if (bold)
			newRule->format.setFontWeight(QFont::Bold);
		newRule->format.setFontUnderline(underline);
		newRule->regex.setPattern(regex);
		newRule->regex.setPatternOptions(QRegularExpression::InvertedGreedinessOption);
		highlighterRules.append(newRule);
	}
}


void LatexConfigParser::parseTab(I18nXmlStreamReader& xml)
{
	QString type = xml.attributes().value("type").toString();
	bool itemstab = (type == "items");
	QString title;
	QString name, text, default_value;
	
	while (!xml.atEnd())
	{
		xml.readNext();
		if (xml.isWhitespace() || xml.isComment())
			continue;
		if (xml.isEndElement() && xml.name() == QLatin1String("tab"))
			break;
		if (!xml.isStartElement())
		{
			formatUnexpectedElemError("tab", xml);
			continue;
		}
		if (xml.name() == QLatin1String("title"))
		{
			if (!title.isEmpty())
				formatError("Second <title> tag in <tab>", xml);
			title = xml.readI18nText();
		}
		else if (xml.name() == QLatin1String("item"))
		{
			if (!itemstab)
				formatError("Found <item> in a 'settings'-tab!", xml);
//			QString value = xml.attributes().value("value").toString();
//			QString img = xml.attributes().value("image").toString();
			text = xml.readI18nText();
		}
		else if (xml.name() == QLatin1String("comment") ||
			     xml.name() == QLatin1String("font") ||
				 xml.name() == QLatin1String("spinbox") || 
			     xml.name() == QLatin1String("color") ||
				 xml.name() == QLatin1String("text") ||
			     xml.name() == QLatin1String("list"))
		{
			//TODO: Store this + attributes in a list
//			QString tagname = xml.name().toString();
			name = xml.attributes().value("name").toString();
			default_value = xml.attributes().value("default").toString();
			if (xml.name() != QLatin1String("list"))
				text = xml.readI18nText();
			else
				ignoreList(xml);
			if (!name.isEmpty())
			{
				if (properties.contains(name))
					formatError("Redeclared setting with name: " + name, xml);
				else
					properties.insert(name, default_value);
			}
			//TODO: qDebug() << "For future use:" << tagname << name << text << default_value;
		}
		else
			formatUnexpectedElemError("tab", xml);
	}
	
	if (title.isEmpty())
		formatError("Tab ended here, but no title was found!", xml);
}

void LatexConfigParser::ignoreList(I18nXmlStreamReader& xml)
{
	//TODO: Quick hack to avoid real parsing
	while (!xml.atEnd())
	{
		xml.readNext();
		if (xml.isEndElement() && xml.name() == QLatin1String("list"))
			break;
	}
}

QString LatexConfigParser::executable() const
{
	QFileInfo f(m_filename);
	QString fileName = f.fileName();
	QString command = PrefsManager::instance().latexCommands()[fileName];
	if (command.isEmpty())
		return m_executable;
	return command;
}

QString I18nXmlStreamReader::readI18nText(bool unindent)
{
	QString language = PrefsManager::instance().uiLanguage();
	QString result;
	int matchquality = 0;
	bool i18n = false;
	if (!isStartElement())
		raiseError("readI18nText called without startelement!");
	
	QString startTag = name().toString();
	while (!atEnd())
	{
		readNext();
		if (isWhitespace() || isComment())
			continue;
		if (isStartElement() && name() == startTag)
		{
			raiseError("Invalid nested elements.");
			return "Error";
		}
		if (isEndElement() && name() == startTag)
		{
			if (!unindent)
				return result.trimmed();
			QStringList splitted = result.split("\n");
			int i;
			int minspaces = 0xffff;
			/* NOTE: First line contains no leading whitespace so we start at 1 */
			for (i = 1; i < splitted.size(); i++)
			{
				if (splitted[i].trimmed().isEmpty()) continue;
				int spaces;
				QString tmp = splitted[i];
				for (spaces = 0; spaces < tmp.length(); spaces++)
				{
					if (!tmp[spaces].isSpace())
						break;
				}
				if (spaces < minspaces)
					minspaces = spaces;
			}
			for (i = 1; i < splitted.size(); i++)
				splitted[i] = splitted[i].mid(minspaces);
			return splitted.join("\n").trimmed();
		}
		if (i18n)
		{
			if (isEndElement())
			{
				if (name() == QLatin1String("i18n"))
				{
					i18n = false;
				}
				else
				{
					raiseError("Invalid end element "+ name().toString());
				}
				continue;
			}
			if (!isStartElement())
			{
				raiseError("Unexpected data!");
			}
			if (name() == language)
			{
				matchquality = 2; //Perfect match
				result = readElementText();
			}
			else if (language.startsWith(name().toString()) && matchquality <= 1)
			{
				matchquality = 1; //Only beginning part matches
				result = readElementText();
			}
			else if (result.isEmpty())
			{
				matchquality = 0;
				result = readElementText();
			}
			else
			{
				readElementText(); //Ignore the text
			}
		}
		else
		{
			if (isStartElement())
			{
				if (name() == QLatin1String("i18n"))
				{
					i18n = true;
					continue;
				}
				raiseError("Tag " + name().toString() + "found, but \"i18n\" or string data expected.");
				continue;
			}
			if (isCharacters())
				result = result + text().toString();
		}
	}
	raiseError("Unexpected end of XML file");
	return result;
}

LatexConfigCache* LatexConfigCache::m_instance = nullptr;

LatexConfigCache* LatexConfigCache::instance()
{
	if (!m_instance)
		m_instance = new LatexConfigCache();
	return m_instance;
}

LatexConfigParser* LatexConfigCache::parser(const QString& filename, bool warnOnError)
{
	if (m_parsers.contains(filename))
	{
		if (warnOnError && m_error[filename])
		{
			//Recreate element as error might have been fixed.
			delete m_parsers[filename];
			createParser(filename, warnOnError);
		}
	}
	else
		createParser(filename, warnOnError);
	return m_parsers[filename];
}


void LatexConfigCache::createParser(const QString& filename, bool warnOnError)
{
	LatexConfigParser *parser = new LatexConfigParser();
	bool hasError = !parser->parseConfigFile(filename);
	m_parsers[filename] = parser;
	m_error[filename] = hasError;
	if (hasError)
	{
		ScMessageBox::critical(nullptr, QObject::tr("Error"), "<qt>" +
				QObject::tr("Parsing the configfile %1 failed! Depending on the type of the error "
						"render frames might not work correctly!\n%2").arg(
						filename, parser->error())
						+ "</qt>");
	}
}

bool LatexConfigCache::hasError(const QString& filename)
{
	if (!m_error.contains(filename))
		return true;
	return m_error[filename];
}

QStringList LatexConfigCache::defaultConfigs()
{
	QDir dir(LatexConfigParser::configBase());
	QStringList files;
	files = dir.entryList(QStringList("*.xml"));
	files.sort();
	int i;
	for (i = 0; i < files.size(); i++)
	{
		if (files[i].compare("sample.xml",Qt::CaseInsensitive)==0)
		{
			files.removeAt(i);
			i--;
		}
	}
	return files;
}

QMap<QString, QString> LatexConfigCache::defaultCommands()
{
	QMap<QString, QString> configCmds;

	const QStringList configFiles = PrefsManager::instance().latexConfigs();
	for (const QString& configFile : configFiles)
	{
		LatexConfigParser *config = LatexConfigCache::instance()->parser(configFile);
		configCmds.insert(configFile, config->executable());
	}
	return configCmds;
}
