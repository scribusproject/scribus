/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PRINTPREVIEWCREATOR_GDI_H
#define PRINTPREVIEWCREATOR_GDI_H

#include "printpreviewcreator.h"
#include "scribusapi.h"
#include "scribusstructs.h"

class ScribusDoc;

class SCRIBUS_API PrintPreviewCreator_GDI : public PrintPreviewCreator
{
public:
	PrintPreviewCreator_GDI(ScribusDoc* doc);
	~PrintPreviewCreator_GDI();

	/**
	 * @brief Creates the Preview of the actual Page
	 * @param pageIndex int page number
	 * @param res int resolution
	 * @retval pixmap QPixmap print preview
	 */
	QPixmap createPreview(int pageIndex) override;

	/**
	 * @brief If print preview is generated with Ghostscript
	 * @retval bool
	 */
	bool isGhostscriptBased() const override { return false; }

	/**
	 * @brief Set options used for generating print preview
	 */
	void setPrintOptions(const PrintOptions& options) override;

protected:
	int     m_pageIndex { -1 };
	QString m_tempBaseName; // Base name for temporary files

	/**
	 * @Brief Generate PNG preview
	 */
	bool createPreviewFile(int pageIndex, int res);

	/**
	 * @brief Delete generated temporary files
	 */
	void cleanupTemporaryFiles();
};

#endif