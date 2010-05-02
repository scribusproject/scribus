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

#ifndef DOCUMENTINFORMATION_H
#define DOCUMENTINFORMATION_H

#include <QString>
#include "scribusapi.h"

class SCRIBUS_API DocumentInformation
{
	public:
		DocumentInformation();
		~DocumentInformation() {};
		const QString& author();
		const QString& comments();
		const QString& contrib();
		const QString& cover();
		const QString& date();
		const QString& format();
		const QString& ident();
		const QString& keywords();
		const QString& langInfo();
		const QString& publisher();
		const QString& relation();
		const QString& rights();
		const QString& source();
		const QString& subject();
		const QString& title();
		const QString& type();
		
		void setAuthor(const QString &_author);
		void setComments(const QString &_comments);
		void setContrib(const QString &_contrib);
		void setCover(const QString &_cover);
		void setDate(const QString &_date);
		void setFormat(const QString &_format);
		void setIdent(const QString &_ident);
		void setKeywords(const QString &_keywords);
		void setLangInfo(const QString &_langInfo);
		void setPublisher(const QString &_publisher);
		void setRelation(const QString &_relation);
		void setRights(const QString &_rights);
		void setSource(const QString &_source);
		void setSubject(const QString &_subject);
		void setTitle(const QString &_title);
		void setType(const QString &_type);
		
	protected:
		QString m_author;
		QString m_comments;
		QString m_contrib;
		QString m_cover;
		QString m_date;
		QString m_format;
		QString m_ident;
		QString m_keywords;
		QString m_langInfo;
		QString m_publisher;
		QString m_relation;
		QString m_rights;
		QString m_source;
		QString m_subject;
		QString m_title;
		QString m_type;
};

#endif
