#include "mypluginimpl.h"
#include "scribus.h"

// Initialize members here, if any
MyPluginImpl::MyPluginImpl()
{
}

bool MyPluginImpl::run(const QString & target)
{
	// Do the bulk of your work here
	QMessageBox::information(
			tr("Scribus - My Plugin"),
			tr("The plugin worked!"),
			QMessageBox::Ok|QMessageBox::Default|QMessageBox::Escape,
			QMessageBox::NoButton);
	return true;
}
