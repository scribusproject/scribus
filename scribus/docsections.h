/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
*   Copyright (C) 2005 by Craig Bradney                                   *
*   cbradney@zip.com.au                                                   *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************/

#ifndef DOCSECTIONS_H
#define DOCSECTIONS_H

#include <QMap>
#include <QStringList>

class QEvent;

#include "ui_docsections.h"
#include "pagestructs.h"


class DocSections : public QWidget, Ui::DocSections
{
	Q_OBJECT

	public:
		DocSections(QWidget* parent);
		~DocSections();
		
		virtual void changeEvent(QEvent *e);
		
		virtual void setup(const DocumentSectionMap docSections, int maxPageIndex);
		virtual void updateTable();
		const DocumentSectionMap& getNewSections();
		
	protected slots:
		virtual void languageChange();
		virtual void tableItemChanged(int, int);
		virtual void addEntry();
		virtual void deleteEntry();
	
	protected:
		DocumentSectionMap localSections;
		uint m_maxpageindex;
		QStringList styles;
};

#endif
