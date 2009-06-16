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
	author = "";
	comments = "";
	contrib = "";
	cover = "";
	date = "";
	format = "";
	ident = "";
	keywords = "";
	langInfo = "";
	publisher = "";
	relation = "";
	rights = "";
	source = "";
	title = "";
	type = "";
}

const QString DocumentInformation::getAuthor()
{
	return author;
}

const QString DocumentInformation::getComments()
{
	return comments;
}

const QString DocumentInformation::getContrib()
{
	return contrib;
}

const QString DocumentInformation::getCover()
{
	return cover;
}

const QString DocumentInformation::getDate()
{
	return date;
}

const QString DocumentInformation::getFormat()
{
	return format;
}

const QString DocumentInformation::getIdent()
{
	return ident;
}

const QString DocumentInformation::getKeywords()
{
	return keywords;
}

const QString DocumentInformation::getLangInfo()
{
	return langInfo;
}

const QString DocumentInformation::getPublisher()
{
	return publisher;
}

const QString DocumentInformation::getRelation()
{
	return relation;
}

const QString DocumentInformation::getRights()
{
	return rights;
}

const QString DocumentInformation::getSource()
{
	return source;
}

const QString DocumentInformation::getSubject()
{
	return subject;
}

const QString DocumentInformation::getTitle()
{
	return title;
}

const QString DocumentInformation::getType()
{
	return type;
}


void DocumentInformation::setAuthor(const QString &_author)
{
	author=_author;
}

void DocumentInformation::setComments(const QString &_comments)
{
	comments=_comments;
}

void DocumentInformation::setContrib(const QString &_contrib)
{
	contrib=_contrib;
}

void DocumentInformation::setCover(const QString &_cover)
{
	cover=_cover;
}

void DocumentInformation::setDate(const QString &_date)
{
	date=_date;
}

void DocumentInformation::setFormat(const QString &_format)
{
	format=_format;
}

void DocumentInformation::setIdent(const QString &_ident)
{
	ident=_ident;
}

void DocumentInformation::setKeywords(const QString &_keywords)
{
	keywords=_keywords;
}

void DocumentInformation::setLangInfo(const QString &_langInfo)
{
	langInfo=_langInfo;
}

void DocumentInformation::setPublisher(const QString &_publisher)
{
	publisher=_publisher;
}

void DocumentInformation::setRelation(const QString &_relation)
{
	relation=_relation;
}

void DocumentInformation::setRights(const QString &_rights)
{
	rights=_rights;
}

void DocumentInformation::setSource(const QString &_source)
{
	source=_source;
}

void DocumentInformation::setSubject(const QString & _subject)
{
	subject = _subject;
}

void DocumentInformation::setTitle(const QString &_title)
{
	title=_title;
}

void DocumentInformation::setType(const QString &_type)
{
	type=_type;
}
