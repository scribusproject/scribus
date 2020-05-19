/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PRINTPREVIEW_H
#define PRINTPREVIEW_H

#include <QCheckBox>
#include <QDialog>
#include <QMap>

class QComboBox;
class QHBoxLayout;
class QVBoxLayout;
class QGroupBox;
class QLabel;
class QPushButton;
class QScrollArea;
class QTableWidget;

#include "ui/scrspinbox.h"
#include "scribusapi.h"
#include "scribusstructs.h"

class PageSelector;
class ScribusDoc;
class ScribusView;
class ScImage;
class ScColor;
class PrefsManager;

//! \brief Print Preview dialog
class SCRIBUS_API PrintPreview : public QDialog
{
	Q_OBJECT

public:
	/*!
	\author Franz Schmid
	\brief Create the Print Preview window
	\param parent QWidget *
	\param vin ScribusView *
	\param docu ScribusDoc *
	\param printer a name of the printer
	\param engine a printer engine
	*/
	PrintPreview(QWidget* parent, ScribusDoc *docu, const QString& printer, PrintEngine engine );
	~PrintPreview();

	bool isCMYKPreviewEnabled() const { return enableCMYK->isChecked(); }
	bool isAntialiasingEnabled() const { return antiAliasing->isChecked(); }
	bool isTransparencyEnabled() const { return showTransparency->isChecked();  }
	bool isInkChannelVisible(const QString& ink) { return (flagsVisible.contains(ink) ? flagsVisible[ink]->isChecked() : false); }
	bool isInkCoverageEnabled() const { return enableInkCover->isChecked(); }
	bool usePostScriptPreview() const { return postscriptPreview; }

	double inkCoverageThreshold() const { return coverThresholdValue->value(); }

	/*!
	\author Franz Schmid
	\brief Renders the Preview to a file on Disk
	\param pageIndex int page number
	\param res int resolution
	\retval int Flag indicating error
	*/
	int RenderPreview(int pageIndex, int res);
	int RenderPreviewSep(int pageIndex, int res);
	void blendImages(QImage &target, ScImage &source, ScColor col);
	void blendImagesSumUp(QImage &target, ScImage &scsource);
	static bool usePostscriptPreview(const QString& printerName, PrintEngine engine);
	/*!
	\author Franz Schmid
	\brief Creates the Preview of the Actual Page
	\param pageIndex int page number
	\param res int resolution
	\retval pixmap QPixmap print preview
	*/
	QPixmap CreatePreview(int pageIndex, int res);

public slots:
	/*!
	\author Franz Schmid
	\brief Jump to newly selected page and create the new preview
	\param num int Page Number
	*/
	void jumpToPage(int num);
	/*!
	\author Franz Schmid
	\brief Create the new preview
	*/
	void redisplay();
	/*!
	\author Craig Bradney
	\brief When CMYK preview is toggled, (dis)enable the CMYK controls and create the new preview
	*/
	void toggleCMYK();
	/*!
	\author Craig Bradney
	\brief If CMYK preview is enabled, create a new preview with the new CMYK plate settings
	*/
	void toggleCMYK_Colour();
	void doSpotTable(int row);
	void toggleAllfromHeader();
	/*!
	\author Petr Vanek
	\date 09/03/2005
	\brief Recompute scaling factor of the preview image
	\param value spinbox value from signal
	*/
	void scaleBox_valueChanged(int value);

signals:
	void doPrint();

protected:
	ScribusDoc *doc { nullptr };
	bool havePngAlpha { false };
	bool haveTiffSep { false };
	bool postscriptPreview { true };
	QMap<QString, int> sepsToFileNum;
	QMap<QString, QCheckBox*> flagsVisible;
	QTableWidget* inkTable { nullptr };

	int m_currentPage { -1 };
	int m_scaleMode { 1 };
	int m_gsVersion;
	int m_inkMax { 1020 }; // 4 * 255
	bool m_colorMode { false };
	bool m_useAntialiasing { false };
	bool m_showTransparency { false };
	bool m_useGCR { true };
	bool m_mirrorH { false };
	bool m_mirrorV { false };
	bool m_clipToMargins { false };
	bool m_convertSpots { true };
	bool m_useGray { false };

	/*! \brief Percentage value of the scaling widget */
	double scaleFactor { 1.0 };

	PageSelector *pageSelector { nullptr };
	QCheckBox* antiAliasing { nullptr };
	QCheckBox* showTransparency { nullptr };
	QCheckBox* enableCMYK { nullptr };
	QCheckBox* enableGCR { nullptr };
	QCheckBox* mirrorHor { nullptr };
	QCheckBox* mirrorVert { nullptr };
	QCheckBox* clipMargins { nullptr };
	QCheckBox* spotColors { nullptr };
	QCheckBox* useGray { nullptr };
	QCheckBox* enableInkCover { nullptr };
	ScrSpinBox* coverThresholdValue { nullptr };
	QLabel* thresholdLabel { nullptr };
	QScrollArea* previewArea { nullptr };
	QLabel* previewLabel { nullptr };
	QGroupBox* devTitle { nullptr };
	QGroupBox* jobTitle { nullptr };
	QPushButton *closeButton { nullptr };
	QPushButton *printButton { nullptr };
	/*! scaling GUI */
	QLabel* scaleLabel { nullptr };
	QComboBox* scaleBox { nullptr };

	QVBoxLayout* PLayout { nullptr };
	QVBoxLayout* Layout1 { nullptr };
	QVBoxLayout* Layout2 { nullptr };
	QHBoxLayout* Layout5 { nullptr };
	QHBoxLayout* Layout6 { nullptr };
	QHBoxLayout* Layout7 { nullptr };
	QVBoxLayout* settingsBarLayout { nullptr };
	PrefsManager& prefsManager;

	void setValues();
	void getUserSelection(int);
	void imageLoadError(QPixmap &, int);

	//! \brief repaint sample on the dialog change
	void resizeEvent(QResizeEvent * event) override;
};

#endif
