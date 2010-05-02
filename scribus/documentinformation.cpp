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
*   ScMW program is free software; you can redistribute it and/or modify  *
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

const QString& DocumentInformation::author()
{
	return m_author;
}

const QString& DocumentInformation::comments()
{
	return m_comments;
}

const QString& DocumentInformation::contrib()
{
	return m_contrib;
}

const QString& DocumentInformation::cover()
{
	return m_cover;
}

const QString& DocumentInformation::date()
{
	return m_date;
}

const QString& DocumentInformation::format()
{
	return m_format;
}

const QString& DocumentInformation::ident()
{
	return m_ident;
}

const QString& DocumentInformation::keywords()
{
	return m_keywords;
}

const QString& DocumentInformation::langInfo()
{
	return m_langInfo;
}

const QString& DocumentInformation::publisher()
{
	return m_publisher;
}

const QString& DocumentInformation::relation()
{
	return m_relation;
}

const QString& DocumentInformation::rights()
{
	return m_rights;
}

const QString& DocumentInformation::source()
{
	return m_source;
}

const QString& DocumentInformation::subject()
{
	return m_subject;
}

const QString& DocumentInformation::title()
{
	return m_title;
}

const QString& DocumentInformation::type()
{
	return m_type;
}


void DocumentInformation::setAuthor(const QString &_author)
{
	m_author=_author;
}

void DocumentInformation::setComments(const QString &_comments)
{
	m_comments=_comments;
}

void DocumentInformation::setContrib(const QString &_contrib)
{
	m_contrib=_contrib;
}

void DocumentInformation::setCover(const QString &_cover)
{
	m_cover=_cover;
}

void DocumentInformation::setDate(const QString &_date)
{
	m_date=_date;
}

void DocumentInformation::setFormat(const QString &_format)
{
	m_format=_format;
}

void DocumentInformation::setIdent(const QString &_ident)
{
	m_ident=_ident;
}

void DocumentInformation::setKeywords(const QString &_keywords)
{
	m_keywords=_keywords;
}

void DocumentInformation::setLangInfo(const QString &_langInfo)
{
	m_langInfo=_langInfo;
}

void DocumentInformation::setPublisher(const QString &_publisher)
{
	m_publisher=_publisher;
}

void DocumentInformation::setRelation(const QString &_relation)
{
	m_relation=_relation;
}

void DocumentInformation::setRights(const QString &_rights)
{
	m_rights=_rights;
}

void DocumentInformation::setSource(const QString &_source)
{
	m_source=_source;
}

void DocumentInformation::setSubject(const QString & _subject)
{
	m_subject = _subject;
}

void DocumentInformation::setTitle(const QString &_title)
{
	m_title=_title;
}

void DocumentInformation::setType(const QString &_type)
{
	m_type=_type;
}

