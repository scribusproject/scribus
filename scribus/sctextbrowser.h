/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCTEXTBROWSER_H
#define SCTEXTBROWSER_H

#include <QTextBrowser>

#include "scribusapi.h"

class SCRIBUS_API ScTextBrowser : public QTextBrowser
{
	Q_OBJECT
	
	public:
		ScTextBrowser( QWidget * parent = 0 );
		
	signals:
		void overLink(const QString &link);

	protected slots:
		/*! \brief Show the hover mouse pointer in the textBrowser*/
		void hoverMouse(const QString &link);

		/*! \brief Show the hover mouse pointer in the textBrowser*/
		void navigateOverride(const QUrl & link);
};

#endif
