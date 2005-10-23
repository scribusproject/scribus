#include "oldscribusformatimpl.h"
#include "scribus.h"

#include <qstring.h>
#include <qmessagebox.h>

// Initialize members here, if any
OldScribusFormatImpl::OldScribusFormatImpl() : QObject(0, "OldScribusFormatImpl")
{
}

bool OldScribusFormatImpl::run(const QString & target)
{
	// Do the bulk of your work here
	QMessageBox::information(
			ScApp,
			tr("Scribus - My Plugin"),
			tr("The plugin worked!"),
			QMessageBox::Ok|QMessageBox::Default|QMessageBox::Escape,
			QMessageBox::NoButton);
	return true;
}
