/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
	begin                : Apr 2005
	copyright            : (C) 2005 by Craig Bradney
	email                : cbradney@zip.com.au
***************************************************************************/

/***************************************************************************
*                                                                         *
*   Scribus program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#include "documentinformation.h"

DocumentInformation::DocumentInformation()
{
	/*m_author = "";
	m_comments = "";
	m_contrib = "";
	m_cover = "";
	m_date = "";
	m_format = "";
	m_ident = "";
	m_keywords = "";
	m_langInfo = "";
	m_publisher = "";
	m_relation = "";
	m_rights = "";
	m_source = "";
	m_title = "";
	m_type = "";*/
}

const QString& DocumentInformation::author() const
{
	return m_author;
}

const QString& DocumentInformation::comments() const
{
	return m_comments;
}

const QString& DocumentInformation::contrib() const
{
	return m_contrib;
}

const QString& DocumentInformation::cover() const
{
	return m_cover;
}

const QString& DocumentInformation::date() const
{
	return m_date;
}

const QString& DocumentInformation::format() const
{
	return m_format;
}

const QString& DocumentInformation::ident() const
{
	return m_ident;
}

const QString& DocumentInformation::keywords() const
{
	return m_keywords;
}

const QString& DocumentInformation::langInfo() const
{
	return m_langInfo;
}

const QString& DocumentInformation::publisher() const
{
	return m_publisher;
}

const QString& DocumentInformation::relation() const
{
	return m_relation;
}

const QString& DocumentInformation::rights() const
{
	return m_rights;
}

const QString& DocumentInformation::source() const
{
	return m_source;
}

const QString& DocumentInformation::subject() const
{
	return m_subject;
}

const QString& DocumentInformation::title() const
{
	return m_title;
}

const QString& DocumentInformation::type() const
{
	return m_type;
}

void DocumentInformation::setAuthor(const QString &author)
{
	m_author = author;
}

void DocumentInformation::setComments(const QString &comments)
{
	m_comments = comments;
}

void DocumentInformation::setContrib(const QString &contrib)
{
	m_contrib = contrib;
}

void DocumentInformation::setCover(const QString &cover)
{
	m_cover = cover;
}

void DocumentInformation::setDate(const QString &date)
{
	m_date = date;
}

void DocumentInformation::setFormat(const QString &format)
{
	m_format = format;
}

void DocumentInformation::setIdent(const QString &ident)
{
	m_ident = ident;
}

void DocumentInformation::setKeywords(const QString &keywords)
{
	m_keywords = keywords;
}

void DocumentInformation::setLangInfo(const QString &langInfo)
{
	m_langInfo = langInfo;
}

void DocumentInformation::setPublisher(const QString &publisher)
{
	m_publisher = publisher;
}

void DocumentInformation::setRelation(const QString &relation)
{
	m_relation = relation;
}

void DocumentInformation::setRights(const QString &rights)
{
	m_rights = rights;
}

void DocumentInformation::setSource(const QString &source)
{
	m_source = source;
}

void DocumentInformation::setSubject(const QString &subject)
{
	m_subject = subject;
}

void DocumentInformation::setTitle(const QString &title)
{
	m_title = title;
}

void DocumentInformation::setType(const QString &type)
{
	m_type = type;
}

