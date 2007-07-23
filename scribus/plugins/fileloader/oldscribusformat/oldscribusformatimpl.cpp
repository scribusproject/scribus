/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "oldscribusformatimpl.h"
//#include "oldscribusformatimpl.moc"
#include "scribus.h"

#include <QString>
#include <QMessageBox>

// Initialize members here, if any
OldScribusFormatImpl::OldScribusFormatImpl() : QObject(0, "OldScribusFormatImpl")
{
}

// This method is connected to the "import page" entry in the UI
// For now, we just call back into Scribus
bool OldScribusFormatImpl::run(const QString & )
{
	ScMW->slotPageImport();
	return true;
}
