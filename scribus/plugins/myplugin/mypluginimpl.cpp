/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "mypluginimpl.h"
#include "scribusdoc.h"

#include <QString>
#include <QMessageBox>

// Initialize members here, if any
MyPluginImpl::MyPluginImpl() : QObject(0)
{
}

bool MyPluginImpl::run(const QString & target, ScribusDoc* doc)
{
	// Do the bulk of your work here
	QMessageBox::information(
			(QWidget*)doc->scMW(),
			tr("Scribus - My Plugin"),
			tr("The plugin worked!"),
			QMessageBox::Ok|QMessageBox::Default|QMessageBox::Escape,
			QMessageBox::NoButton);
	return true;
}
