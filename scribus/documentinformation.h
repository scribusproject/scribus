/***************************************************************************
	begin                : Apr 2005
	copyright            : (C) 2005 by Craig Bradney
	email                : cbradney@zip.com.au
***************************************************************************/

/***************************************************************************
*                                                                         *
*   ScApp program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#ifndef DOCUMENTINFORMATION_H
#define DOCUMENTINFORMATION_H

#include <qstring.h>

class DocumentInformation
{
	public:
		DocumentInformation();
		~DocumentInformation();
		QString DocAutor;
		QString DocTitel;
		QString DocComments;
		QString DocKeyWords;
		QString DocPublisher;
		QString DocType;
		QString DocDate;
		QString DocFormat;
		QString DocIdent;
		QString DocSource;
		QString DocLangInfo;
		QString DocRelation;
		QString DocCover;
		QString DocRights;
		QString DocContrib;
	private:
};

#endif
