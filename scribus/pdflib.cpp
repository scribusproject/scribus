#include "pdflib.h"

#include "pdflib_core.h"

PDFlib::PDFlib(ScribusDoc & docu)
    : m_impl( new PDFLibCore(docu) )
{
	Q_ASSERT(m_impl);
}

PDFlib::PDFlib(ScribusDoc & docu, const PDFOptions& options)
	 : m_impl( new PDFLibCore(docu, options) )
{

}

PDFlib::~PDFlib()
{
	delete static_cast<PDFLibCore*>(m_impl);
}

bool PDFlib::doExport(const QString& fn, const std::vector<int> & pageNs, const QMap<int, QImage>& thumbs)
{
	return static_cast<PDFLibCore*>(m_impl)->doExport(fn, pageNs, thumbs);
}

const QString& PDFlib::errorMessage()
{
	return static_cast<PDFLibCore*>(m_impl)->errorMessage();
}

bool PDFlib::exportAborted()
{
	return static_cast<PDFLibCore*>(m_impl)->exportAborted();
}
