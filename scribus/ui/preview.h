/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PRVIEW_H
#define PRVIEW_H

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
class SCRIBUS_API PPreview : public QDialog
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
	PPreview(QWidget* parent, ScribusView *vin, ScribusDoc *docu, const QString& printer, PrintEngine engine );
	~PPreview() {};
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

	PageSelector *PGSel { nullptr };
	QCheckBox* AntiAlias { nullptr };
	QCheckBox* AliasTr { nullptr };
	QCheckBox* EnableCMYK { nullptr };
	QCheckBox* EnableGCR { nullptr };
	QCheckBox* MirrorHor { nullptr };
	QCheckBox* MirrorVert { nullptr };
	QCheckBox* ClipMarg { nullptr };
	QCheckBox* spotColors { nullptr };
	QCheckBox* useGray { nullptr };
	QCheckBox* EnableInkCover { nullptr };
	ScrSpinBox* CoverThresholdValue { nullptr };
	QLabel* ThresLabel { nullptr };
	QScrollArea* Anzeige { nullptr };
	QLabel* Anz { nullptr };
	QGroupBox* devTitle { nullptr };
	QGroupBox* jobTitle { nullptr };
	QPushButton *closeButton { nullptr };
	QPushButton *printButton { nullptr };
	/*! scaling GUI */
	QLabel* scaleLabel { nullptr };
	QComboBox* scaleBox { nullptr };

	ScribusView *view { nullptr };
	ScribusDoc *doc { nullptr };
	bool HavePngAlpha { false };
	bool HaveTiffSep { false };
	bool postscriptPreview { true };
	QMap<QString, int> sepsToFileNum;
	QMap<QString, QCheckBox*> flagsVisible;
	QTableWidget* Table { nullptr };

public slots:
	/*!
	\author Franz Schmid
	\brief Jump to newly selected page and create the new preview
	\param num int Page Number
	*/
	void ToSeite(int num);
	/*!
	\author Franz Schmid
	\brief Create the new preview
	*/
	void redisplay();
	/*!
	\author Craig Bradney
	\brief When CMYK preview is toggled, (dis)enable the CMYK controls and create the new preview
	*/
	void ToggleCMYK();
	/*!
	\author Craig Bradney
	\brief If CMYK preview is enabled, create a new preview with the new CMYK plate settings
	*/
	void ToggleCMYK_Colour();
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
	void resizeEvent(QResizeEvent * event);

};

#endif
