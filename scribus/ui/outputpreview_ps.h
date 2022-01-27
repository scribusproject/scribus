/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef OUTPUTPREVIEW_PS_H
#define OUTPUTPREVIEW_PS_H

#include <QMap>

#include "pdfoptions.h"
#include "pdfversion.h"
#include "outputpreviewbase.h"
#include "scribusstructs.h"

class QCheckBox;

class PrefsManager;
class ScribusDoc;
namespace Ui { class OutputPreview_PS; }

class OutputPreview_PS : public OutputPreviewBase
{
	Q_OBJECT
	
public:
	OutputPreview_PS(QWidget* parent, ScribusDoc* doc);
	~OutputPreview_PS();
	
	/**
	 * @brief Delete temporary files generated during execution of the dialog
	 */
	void cleanupTemporaryFiles() override;
	
	/**
	 * @brief Creates the Preview of the Actual Page
	 * @param pageIndex int page number
	 * @param res int resolution
	 * @retval pixmap QPixmap print preview
	 */
	QPixmap createPreview(int pageIndex, int res) override;

	int  postscriptLevel() const;
	bool isCMYKPreviewEnabled() const ;
	bool isAntialiasingEnabled() const;
	bool isTransparencyEnabled() const;
	bool isInkChannelVisible(const QString& ink);
	bool isInkCoverageEnabled() const;
	double inkCoverageThreshold() const;

protected:
	bool m_havePngAlpha { false };
	bool m_haveTiffSep { false };
	int  m_gsVersion { 0 };

	// PS export options
	int  m_psLevel;
	int  m_colorOutputMode { 0 };
	bool m_cmykPreviewMode { false };
	bool m_mirrorH { false };
	bool m_mirrorV { false };
	bool m_clipToMargins { false };
	bool m_convertSpots { false };
	bool m_enableGCR { false };

	PrintOptions m_printOptions;

	// Preview display options
	int  m_scaleMode { 1 };
	int  m_inkMax { 1020 }; // 4 * 255
	bool m_useAntialiasing { false };
	bool m_showTransparency { false };

	Ui::OutputPreview_PS* m_optionsUi { nullptr };
	QMap<QString, int>     m_sepsToFileNum;
	QMap<QString, QCheckBox*> m_inkVisibilities;
	QString m_tempBaseName; // Base name for temporary files

	PrefsManager& m_prefsManager;

	int renderPreview(int pageIndex, int res);
	int renderPreviewSep(int pageIndex, int res);
	
	bool createPreviewFile(int pageIndex);

	bool optionsHaveChanged(int pageIndex) const;
	bool psOptionsHaveChanged(int pageIndex) const;
	void setPrintOptionsToOptions(PrintOptions& options);
	void setPrintOptionsToUi(PrintOptions& options);
	void setUiOptionsToPrintOptions(PrintOptions& options);
	void updateOptionsFromUI();

	void imageLoadError(QPixmap &pixmap);

protected slots:
	void onPSLevelChanged(int index);
	void onColorOutputModeChanged(int index);
	void onInkTableCellDoubleClicked(int row);
	void toggleAllFromHeader();
	void toggleCMYK();
	void toggleCMYK_Colour();
};

#endif
