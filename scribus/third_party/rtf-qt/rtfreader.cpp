/*
    Copyright (C)  2008, 2010  Brad Hards <bradh@frogmouth.net>

    This library is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 2.1 of the License, or
    (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "rtfreader.h"
#include "controlword.h"
#include "AbstractRtfOutput.h"

#include "AuthorPcdataDestination.h"
#include "CategoryPcdataDestination.h"
#include "ColorTableDestination.h"
#include "CommentPcdataDestination.h"
#include "CompanyPcdataDestination.h"
#include "DocumentCommentPcdataDestination.h"
#include "DocumentDestination.h"
#include "FontTableDestination.h"
#include "GeneratorPcdataDestination.h"
#include "HLinkBasePcdataDestination.h"
#include "IgnoredDestination.h"
#include "InfoDestination.h"
#include "InfoCreatedTimeDestination.h"
#include "InfoPrintedTimeDestination.h"
#include "InfoRevisedTimeDestination.h"
#include "KeywordsPcdataDestination.h"
#include "ManagerPcdataDestination.h"
#include "OperatorPcdataDestination.h"
#include "PictDestination.h"
#include "SubjectPcdataDestination.h"
#include "StyleSheetDestination.h"
#include "TitlePcdataDestination.h"
#include "UserPropsDestination.h"

#include <QStack>
#include <QTextCursor>
#include <QUrl>

namespace RtfReader
{
	Reader::Reader(QObject *parent): QObject (parent),  m_inputDevice(0)
	{
	}

	Reader::~Reader()
	{
	}

	bool Reader::open(const QString &filename)
	{
		m_inputDevice = new QFile(filename, this);
		bool result = m_inputDevice->open(QIODevice::ReadOnly);
		return result;
	}

	void Reader::close()
	{
		if (!m_inputDevice)
			return;
		m_inputDevice->close();
		delete m_inputDevice;
		m_inputDevice = 0;
	}

	QString Reader::fileName() const
	{
		if (m_inputDevice && m_inputDevice->exists())
			return m_inputDevice->fileName();
		else
			return QString();
	}

	bool Reader::parseTo(AbstractRtfOutput *output)
	{
		if ((!m_inputDevice ) || (!m_inputDevice->isOpen()))
			return false;
		m_output = output;
		parseFile();
		return true;
	}

	bool Reader::parseFromDeviceTo(QIODevice * dev, AbstractRtfOutput *output)
	{
		m_output = output;
		m_tokenizer = new Tokenizer(dev);
		if (parseFileHeader())
			parseDocument();
		delete m_tokenizer;
		return true;
	}

	void Reader::parseFile()
	{
		m_tokenizer = new Tokenizer(m_inputDevice);
		if (parseFileHeader())
			parseDocument();
		delete m_tokenizer;
	}

	bool Reader::parseFileHeader()
	{
		bool result = true;
		Token token = m_tokenizer->fetchToken();
		if (token.type != OpenGroup )
			result = false;
		token = m_tokenizer->fetchToken();
		if (token.type != Control)
			result = false;
		if (!headerFormatIsKnown(token.name, token.parameter.toInt()))
			result = false;
		return result;
	}

	bool Reader::headerFormatIsKnown(const QString &tokenName, int tokenValue)
	{
		if (tokenName != QString("rtf"))
			return false;
		return true;
	}

	Destination* Reader::makeDestination(const QString &destinationName)
	{
		if (destinationName == "colortbl")
			return new ColorTableDestination(this, m_output, destinationName);
		else if (destinationName == "creatim")
			return new InfoCreatedTimeDestination(this, m_output, destinationName);
		else if (destinationName == "printim")
			return new InfoPrintedTimeDestination(this, m_output, destinationName);
		else if (destinationName == "revtim")
			return new InfoRevisedTimeDestination(this, m_output, destinationName);
		else if (destinationName == "author")
			return new AuthorPcdataDestination(this, m_output, destinationName);
		else if (destinationName == "company")
			return new CompanyPcdataDestination(this, m_output, destinationName);
		else if (destinationName == "operator")
			return new OperatorPcdataDestination(this, m_output, destinationName);
		else if (destinationName == "comment")
			return new CommentPcdataDestination(this, m_output, destinationName);
		else if (destinationName == "doccomm")
			return new DocumentCommentPcdataDestination(this, m_output, destinationName);
		else if (destinationName == "title")
			return new TitlePcdataDestination(this, m_output, destinationName);
		else if (destinationName == "subject")
			return new SubjectPcdataDestination(this, m_output, destinationName);
		else if (destinationName == "manager")
			return new ManagerPcdataDestination(this, m_output, destinationName);
		else if (destinationName == "category")
			return new CategoryPcdataDestination(this, m_output, destinationName);
		else if (destinationName == "keywords")
			return new KeywordsPcdataDestination(this, m_output, destinationName);
		else if (destinationName == "hlinkbase")
			return new HLinkBasePcdataDestination(this, m_output, destinationName);
		else if (destinationName == "generator")
			return new GeneratorPcdataDestination(this, m_output, destinationName);
		else if (destinationName == "pict")
			return new PictDestination(this, m_output, destinationName);
		else if (destinationName == "fonttbl")
			return new FontTableDestination(this, m_output, destinationName);
		else if (destinationName == "stylesheet")
			return new StyleSheetDestination(this, m_output, destinationName);
		else if (destinationName == "rtf")
			return new DocumentDestination(this, m_output, destinationName);
		else if (destinationName == "info")
			return new InfoDestination(this, m_output, destinationName);
		else if (destinationName == "userprops")
			return new UserPropsDestination(this, m_output, destinationName);
		else if (destinationName == "ignorable")
			return new IgnoredDestination(this, m_output, destinationName);
		return new Destination(this, m_output, destinationName);
	}

	void Reader::changeDestination(const QString &destinationName)
	{
		if (m_destinationStack.top()->name() == "ignorable")
			return;
		Destination *dest = makeDestination(destinationName);
		m_destinationStack.push(dest);
		m_stateStack.top().didChangeDestination = true;
		QStringList destStackElementNames;
		for (int i = 0; i < m_destinationStack.size(); ++i)
		{
			destStackElementNames << m_destinationStack.at(i)->name();
		}
	}

	void Reader::parseDocument()
	{
		class RtfGroupState state;
		// Push an end-of-file marker onto the stack
		state.endOfFile = true;
		m_stateStack.push(state);
		// Set up the outer part of the destination stack
		Destination *dest = makeDestination("rtf");
		m_destinationStack.push(dest);
		m_stateStack.top().didChangeDestination = true;
		m_debugIndent = QString('\t');
		// Parse RTF document
		bool atEndOfFile = false;
		m_nextSymbolMightBeDestination = false;
		RtfReader::ControlWord controlWord("");
		while (!atEndOfFile)
		{
			Token token = m_tokenizer->fetchToken();
			switch (token.type)
			{
				case OpenGroup:
				{
				// Store the current state on the stack
					RtfGroupState state;
					m_stateStack.push(state);
					m_nextSymbolMightBeDestination = true;
					m_output->startGroup();
				// qDebug() << m_debugIndent << "opengroup";
					m_debugIndent.append("\t");
					break;
				}
				case CloseGroup:
				{
					QStringList destStackElementNames;
					for (int i = 0; i < m_destinationStack.size(); ++i)
					{
						destStackElementNames << m_destinationStack.at(i)->name();
					}
					// qDebug() << m_debugIndent << "closegroup ( destinationStack:" << destStackElementNames << ")";
					m_debugIndent.remove(0, 1);
					state = m_stateStack.pop();
					if (state.endOfFile)
						atEndOfFile = true;
					else
						m_output->endGroup();
					if (state.didChangeDestination )
					{
						m_destinationStack.top()->aboutToEndDestination();
						m_destinationStack.pop();
					}
					destStackElementNames.clear();
					for (int i = 0; i < m_destinationStack.size(); ++i)
					{
						destStackElementNames << m_destinationStack.at(i)->name();
					}
					// qDebug() << m_debugIndent << "destinationStack after CloseGroup: (" << destStackElementNames << ")";
					m_nextSymbolMightBeDestination = true;
					break;
				}
				case Control:
					controlWord = ControlWord(token.name);
					if (token.name ==  "nonshppict")
					{
						m_nextSymbolMightBeDestination = true;
						m_nextSymbolIsIgnorable = true;
					}
					if (m_nextSymbolMightBeDestination && controlWord.isSupportedDestination())
					{
						m_nextSymbolMightBeDestination = false;
						m_nextSymbolIsIgnorable = false;
						changeDestination(token.name);
					}
					else if (m_nextSymbolMightBeDestination && m_nextSymbolIsIgnorable)
					{
					// This is a control word we don't understand
						m_nextSymbolMightBeDestination = false;
						m_nextSymbolIsIgnorable = false;
						changeDestination("ignorable");
					}
					else
					{
						m_nextSymbolMightBeDestination = false;
						if (token.name ==  "*")
						{
							m_nextSymbolMightBeDestination = true;
							m_nextSymbolIsIgnorable = true;
						}
					//	qDebug() << "Token" << token.name << token.parameter.toInt() << "Dest:" << m_destinationStack.top()->name();
						m_destinationStack.top()->handleControlWord(token.name, token.hasParameter, token.parameter.toInt());
					}
					break;
				case Plain:
					m_destinationStack.top()->handlePlainText(token.name);
					break;
				case Binary:
					qDebug() << "binary data:" << token.name;
				break;
				default:
					qDebug() << "Unexpected token Type";
			}
			if (m_tokenizer->atEnd())
				break;
		}
	}
}

