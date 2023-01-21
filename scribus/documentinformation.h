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

#ifndef DOCUMENTINFORMATION_H
#define DOCUMENTINFORMATION_H

#include <QString>
#include "scribusapi.h"

class SCRIBUS_API DocumentInformation
{
	public:
		DocumentInformation();
		~DocumentInformation() = default;

		const QString& author() const;
		const QString& comments() const;
		const QString& contrib() const;
		const QString& cover() const;
		const QString& date() const;
		const QString& format() const;
		const QString& ident() const;
		const QString& keywords() const;
		const QString& langInfo() const;
		const QString& publisher() const;
		const QString& relation() const;
		const QString& rights() const;
		const QString& source() const;
		const QString& subject() const;
		const QString& title() const;
		const QString& type() const;
		
		void setAuthor(const QString &author);
		void setComments(const QString &comments);
		void setContrib(const QString &contrib);
		void setCover(const QString &cover);
		void setDate(const QString &date);
		void setFormat(const QString &format);
		void setIdent(const QString &ident);
		void setKeywords(const QString &keywords);
		void setLangInfo(const QString &langInfo);
		void setPublisher(const QString &publisher);
		void setRelation(const QString &relation);
		void setRights(const QString &rights);
		void setSource(const QString &source);
		void setSubject(const QString &subject);
		void setTitle(const QString &title);
		void setType(const QString &type);
		
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
