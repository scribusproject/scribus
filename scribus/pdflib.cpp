#include "pdflib.h"

#include "pdflib_core.h"

PDFlib::PDFlib(ScribusDoc & docu)
    : m_impl( new PDFLibCore(docu) )
{
	Q_ASSERT(m_impl);
}

PDFlib::~PDFlib()
{
	delete static_cast<PDFLibCore*>(m_impl);
}

bool PDFlib::doExport(const QString& fn, const QString& nam, int Components,
			  const std::vector<int> & pageNs, const QMap<int,QPixmap> & thumbs)
{
	return static_cast<PDFLibCore*>(m_impl)->doExport(fn, nam, Components, pageNs, thumbs);
}

const QString& PDFlib::errorMessage(void)
{
	return static_cast<PDFLibCore*>(m_impl)->errorMessage();
}

bool PDFlib::exportAborted(void)
{
	return static_cast<PDFLibCore*>(m_impl)->exportAborted();
}
