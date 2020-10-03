/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PRINTPREVIEWCREATOR_PS_H
#define PRINTPREVIEWCREATOR_PS_H

#include <QMap>
#include <QString>

#include "scribusapi.h"
#include "printpreviewcreator.h"
#include "scribusstructs.h"

class PrefsManager;
class ScribusDoc;

class SCRIBUS_API PrintPreviewCreator_PS : public SeparationPreviewCreator
{
public:
	PrintPreviewCreator_PS(ScribusDoc* doc);
	~PrintPreviewCreator_PS();

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
	bool isGhostscriptBased() const override { return true; }

	/**
	 * @brief Set options used for generating print preview
	 */
	void setPrintOptions(const PrintOptions& options) override;

protected:
	int     m_pageIndex { -1 };
	int     m_previewRes { 72 };
	QString m_tempBaseName; // Base name for temporary files

	PrefsManager& m_prefsManager;

	QMap<QString, int> m_sepsToFileNum;

	/**
	 * @Brief Generate PostScript for for specified page
	 */
	bool createPreviewFile(int pageIndex);

	/**
	 * @brief Delete generated temporary files
	 */
	void cleanupTemporaryFiles();

	/**
	 * @brief Render simple RGB preview
	 */
	int renderPreview(int pageIndex, int res);

	/**
	 * @brief Render CMYK-based separation preview
	 */
	int renderPreviewSep(int pageIndex, int res);

	/**
	 * @brief Utility function to simplify handling of preview generation errors
	 */
	void imageLoadError(QPixmap &pixmap, int page);
};

#endif
