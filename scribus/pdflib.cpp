#include "pdflib.h"

#include "pdflib_core.h"

PDFlib::PDFlib(ScribusDoc & docu)
    : impl( new PDFLibCore(docu) )
{
    Q_ASSERT(impl);
}

PDFlib::~PDFlib()
{
    delete static_cast<PDFLibCore*>(impl);
}

bool PDFlib::doExport(const QString& fn, const QString& nam, int Components,
			  const std::vector<int> & pageNs, const QMap<int,QPixmap> & thumbs)
{
    return static_cast<PDFLibCore*>(impl)->doExport(fn, nam, Components, pageNs, thumbs);
}

const QString& PDFlib::errorMessage(void)
{
	return static_cast<PDFLibCore*>(impl)->errorMessage();
}

bool PDFlib::exportAborted(void)
{
	return static_cast<PDFLibCore*>(impl)->exportAborted();
}
