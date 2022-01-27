/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PRINTPREVIEWCREATOR_H
#define PRINTPREVIEWCREATOR_H

#include <QMap>
#include <QPixmap>
#include <QStringList>

#include "scribusapi.h"
#include "scribusstructs.h"

class ScribusDoc;

class SCRIBUS_API PrintPreviewCreator
{
public:
	PrintPreviewCreator(ScribusDoc* doc);
	virtual ~PrintPreviewCreator() {};

	/**
	 * @brief Creates the Preview of the Actual Page
	 * @param pageIndex int page number
	 * @param res int resolution
	 * @retval pixmap QPixmap print preview
	 */
	virtual QPixmap createPreview(int pageIndex) = 0;
	
	/**
	 * @brief If print preview is generated with Ghostscript
	 * @retval bool
	 */
	virtual bool isGhostscriptBased() const = 0;

	/**
	 * @brief Return if antialiasing is currently enabled or not
	 */
	bool isAntialiasingEnabled() const { return m_useAntialiasing; }

	/**
	 * @brief Return if background transparency is enabled or not
	 */
	bool isTransparencyEnabled() const { return m_showTransparency; }

	/**
	 * @brief Return current print options
	 */
	const PrintOptions& printOptions() const { return m_printOptions; }

	/**
	 * @brief Enable or disable antialiasing
	 */
	virtual void setAntialisingEnabled(bool enabled);

	/**
	* @brief Set device pixel ratio
	*/
	virtual void setDevicePixelRatio(double ratio);

	/**
	 * @brief Set preview resolution (in dot per inch)
	 */
	virtual void setPreviewResolution(int res);

	/**
	 * @brief Set options used for generating print preview
	 */
	virtual void setPrintOptions(const PrintOptions& options);

	/**
	 * @brief If background should be shown as transparent
	 */
	virtual void setShowTransparency(bool showTrans);

	/**
	 * @brief If this generator supports separation preview
	 */
	virtual bool supportsSeparations() const { return false; }
	
protected:
	ScribusDoc* m_doc { nullptr };
	PrintOptions m_printOptions;

	double m_devicePixelRatio { 1.0 };

	int  m_previewResolution { 72 };
	bool m_useAntialiasing { true };
	bool m_showTransparency { false };

	bool m_renderingOptionsChanged { false };
	bool m_printOptionsChanged { true };
};

class SeparationPreviewCreator : public PrintPreviewCreator
{
public:
	SeparationPreviewCreator(ScribusDoc* doc);

	/**
	 * @brief Return if separation preview is enabled or not
	 */
	bool isSeparationPreviewEnabled() const { return m_sepPreviewEnabled; }

	/**
	 * @brief Return if a specific separation is set a visible or not
	 */
	bool isSeparationVisible(const QString& name) const { return m_separationVisibilities.value(name, false); }

	/**
	 * @brief Available separations
	 */
	QStringList separationNames() const { return m_separationVisibilities.keys(); }

	/**
	 * @brief If this generator supports separation preview
	 */
	bool supportsSeparations() const override { return true; }

	/**
	 * @brief Enable or disable separation preview
	 */
	void setSeparationPreviewEnabled(bool enabled);

	/**
	 * @brief Make a specific separation visible or not
	 */
	void setSeparationVisible(const QString& name, bool visible);

	/**
	 * @brief Enable or disable showing of ink coverage
	 */
	void setShowInkCoverage(bool show);

	/**
	 * @brief Set value for max acceptable ink coverage
	 */
	void setInkCoverageThreshold(double value);

	/**
	 * @brief Return the number of spot colors used in the preview
	 */
	int spotColorCount() const { return m_spotColorCount; }

protected:
	bool m_havePngAlpha { false };
	bool m_haveTiffSep { false };
	int  m_gsVersion { 0 };

	bool m_sepPreviewEnabled { false };
	QMap<QString, bool> m_separationVisibilities;

	bool m_showInkCoverage { false };
	int  m_inkMax { 1020 }; // 4 * 255
	double m_inkCoverageThreshold { 300.0 };
	int  m_spotColorCount { 0 };

	/**
	 * @brief Utility functions used for blending separation images
	 */
	void blendImages(QImage &target, ScImage &scSource, const ScColor& col);
	void blendImagesSumUp(QImage &target, ScImage &scSource);
};

#endif