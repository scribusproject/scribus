#include "mypluginimpl.h"
#include "scribus.h"

#include <qstring.h>
#include <qmessagebox.h>

// Initialize members here, if any
MyPluginImpl::MyPluginImpl() : QObject(0, "MyPluginImpl")
{
}

bool MyPluginImpl::run(const QString & target)
{
	// Do the bulk of your work here
	QMessageBox::information(
			ScMW,
			tr("Scribus - My Plugin"),
			tr("The plugin worked!"),
			QMessageBox::Ok|QMessageBox::Default|QMessageBox::Escape,
			QMessageBox::NoButton);
	return true;
}
