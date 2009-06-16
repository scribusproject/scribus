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
		const QString getAuthor();
		const QString getComments();
		const QString getContrib();
		const QString getCover();
		const QString getDate();
		const QString getFormat();
		const QString getIdent();
		const QString getKeywords();
		const QString getLangInfo();
		const QString getPublisher();
		const QString getRelation();
		const QString getRights();
		const QString getSource();
		const QString getSubject();
		const QString getTitle();
		const QString getType();
		
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
		
	private:
		QString author;
		QString comments;
		QString contrib;
		QString cover;
		QString date;
		QString format;
		QString ident;
		QString keywords;
		QString langInfo;
		QString publisher;
		QString relation;
		QString rights;
		QString source;
		QString subject;
		QString title;
		QString type;
};

#endif
