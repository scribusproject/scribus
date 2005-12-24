#include "oldscribusformatimpl.h"
#include "scribus.h"

#include <qstring.h>
#include <qmessagebox.h>

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
