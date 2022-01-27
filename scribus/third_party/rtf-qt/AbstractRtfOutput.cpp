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

#include "AbstractRtfOutput.h"

/**
   Namespace for the public interface for the RtfReader classes
*/
namespace RtfReader
{
	AbstractRtfOutput::AbstractRtfOutput()
	{}

	AbstractRtfOutput::~AbstractRtfOutput()
	{}

	QString AbstractRtfOutput::author() const
	{
	    return m_author;
	}
	
	void AbstractRtfOutput::setAuthor( const QString &author )
	{
	    m_author = author;
	}

	QString AbstractRtfOutput::companyName() const
	{
	    return m_companyName;
	}

	void AbstractRtfOutput::setCompanyName( const QString &companyName )
	{
	    m_companyName = companyName;
	}

	QString AbstractRtfOutput::operatorName() const
	{
	    return m_operatorName;
	}

	void AbstractRtfOutput::setOperatorName( const QString &operatorName )
	{
	    m_operatorName = operatorName;
	}

	QString AbstractRtfOutput::title() const
	{
	    return m_title;
	}

	void AbstractRtfOutput::setTitle( const QString &title )
	{
	    m_title = title;
	}

	QString AbstractRtfOutput::subject() const
	{
	    return m_subject;
	}

	void AbstractRtfOutput::setSubject( const QString &subject )
	{
	    m_subject = subject;
	}

	QString AbstractRtfOutput::comment() const
	{
	    return m_comment;
	}

	void AbstractRtfOutput::setComment( const QString &comment )
	{
	    m_comment = comment;
	}

	QString AbstractRtfOutput::documentComment() const
	{
	    return m_documentComment;
	}

	void AbstractRtfOutput::setDocumentComment( const QString &documentComment )
	{
	    m_documentComment = documentComment;
	}

	QString AbstractRtfOutput::managerName() const
	{
	    return m_managerName;
	}

	void AbstractRtfOutput::setManagerName( const QString &managerName )
	{
	    m_managerName = managerName;
	}

	QString AbstractRtfOutput::category() const
	{
	    return m_category;
	}

	void AbstractRtfOutput::setCategory( const QString &category )
	{
	    m_category = category;
	}

	QString AbstractRtfOutput::keywords() const
	{
	    return m_keywords;
	}

	void AbstractRtfOutput::setKeywords( const QString &keywords )
	{
	    m_keywords = keywords;
	}

	QString AbstractRtfOutput::hLinkBase() const
	{
	    return m_hlinkbase;
	}

	void AbstractRtfOutput::setHLinkBase( const QString &hLinkBase )
	{
	    m_hlinkbase = hLinkBase;
	}

	QString AbstractRtfOutput::generatorInformation() const
	{
	    return m_generatorInformation;
	}

	void AbstractRtfOutput::setGeneratorInformation( const QString &generatorInformation )
	{
	    m_generatorInformation = generatorInformation;
	}

	QDateTime AbstractRtfOutput::created() const
	{
	    return m_created;
	}
	
	void AbstractRtfOutput::setCreatedDateTime( const QDateTime &dateTime )
	{
	    m_created = dateTime;
	}

	QDateTime AbstractRtfOutput::revised() const
	{
	    return m_revised;
	}

	void AbstractRtfOutput::setRevisedDateTime( const QDateTime &dateTime )
	{
	    m_revised = dateTime;
	}

	QDateTime AbstractRtfOutput::printed() const
	{
	    return m_printed;
	}

	void AbstractRtfOutput::setPrintedDateTime( const QDateTime &dateTime )
	{
	    m_printed = dateTime;
	}

	int AbstractRtfOutput::totalEditingTime() const
	{
	    return m_totalEditingTime;
	}

	void AbstractRtfOutput::setTotalEditingTime( const int totalEditingTime )
	{
	    m_totalEditingTime = totalEditingTime;
	}

	int AbstractRtfOutput::numberOfPages() const
	{
	    return m_numberOfPages;
	}

	void AbstractRtfOutput::setNumberOfPages( const int numberOfPages )
	{
	    m_numberOfPages = numberOfPages;
	}

	int AbstractRtfOutput::numberOfWords() const
	{
	    return m_numberOfWords;
	}

	void AbstractRtfOutput::setNumberOfWords( const int numberOfWords )
	{
	    m_numberOfWords = numberOfWords;
	}

	int AbstractRtfOutput::numberOfCharacters() const
	{
	    return m_numberOfCharacters;
	}

	void AbstractRtfOutput::setNumberOfCharacters( const int numberOfCharacters )
	{
	    m_numberOfCharacters = numberOfCharacters;
	}

	int AbstractRtfOutput::numberOfCharactersWithoutSpaces() const
	{
	    return m_numberOfCharactersWithoutSpaces;
	}

	void AbstractRtfOutput::setNumberOfCharactersWithoutSpaces( const int numberOfCharactersWithoutSpaces )
	{
	    m_numberOfCharactersWithoutSpaces = numberOfCharactersWithoutSpaces;
	}

	int AbstractRtfOutput::versionNumber() const
	{
	    return m_versionNumber;
	}

	void AbstractRtfOutput::setVersionNumber( const int versionNumber )
	{
	    m_versionNumber = versionNumber;
	}

	int AbstractRtfOutput::internalVersionNumber() const
	{
	    return m_internalVersionNumber;
	}

	void AbstractRtfOutput::setInternalVersionNumber( const int internalVersionNumber )
	{
	    m_internalVersionNumber = internalVersionNumber;
	}

	QVariant AbstractRtfOutput::userProp( const QString &propertyName ) const
	{
	    return m_userProps.value( propertyName );
	}

	QList<QString> AbstractRtfOutput::userPropNames() const
	{
	    return m_userProps.keys();
	}

	void AbstractRtfOutput::addUserProp( const QString &propertyName, const QVariant &propertyValue )
	{
	    m_userProps.insert( propertyName, propertyValue );
	}
	
	void AbstractRtfOutput::startGroup()
	{
	}
	
	void AbstractRtfOutput::endGroup()
	{
	}
}
