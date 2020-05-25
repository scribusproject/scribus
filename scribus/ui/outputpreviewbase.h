/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef OUTPUTPREVIEWBASE_H
#define OUTPUTPREVIEWBASE_H

#include <QDialog>
#include <QLabel>
#include <QPixmap>

class ScColor;
class ScImage;
class ScribusDoc;
namespace Ui { class OutputPreviewBase; }

class OutputPreviewBase : public QDialog
{
	Q_OBJECT

public:
	OutputPreviewBase(QWidget* parent, ScribusDoc* doc);
	virtual ~OutputPreviewBase();

	/**
	 * @brief Delete temporary files generated during execution of the dialog
	 */
	virtual void cleanupTemporaryFiles() {};
	
	/**
	 * @brief Creates the Preview of the Actual Page
	 * @param pageIndex int page number
	 * @param res int resolution
	 * @retval pixmap QPixmap print preview
	 */
	virtual QPixmap createPreview(int pageIndex, int res) = 0;

public slots:
	/**
	 * @brief Jump to newly selected page and create the new preview
	 * @param num int Page Number
	 */
	void jumpToPage(int num);

	/**
	 * @brief Create the new preview
	 */
	void redisplay();

	/**
	 * @brief Recompute scaling factor of the preview image
	 * @param value spinbox value from signal
	 */
	void onScaleBoxValueChanged(int value);

protected:
	ScribusDoc*  m_doc { nullptr };
	int m_currentPage { -1 };

	/*! \brief Percentage value of the scaling widget */
	double m_scaleFactor { 1.0 };

	Ui::OutputPreviewBase* m_uiBase { nullptr };
	QLabel* m_previewLabel { nullptr };

	void resizeEvent(QResizeEvent * event) override;

	void blendImages(QImage &target, ScImage &scSource, const ScColor& col);
	void blendImagesSumUp(QImage &target, ScImage &scSource);

signals:
	void doExport();
};

#endif
