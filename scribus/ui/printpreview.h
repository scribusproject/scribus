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
namespace Ui { class PrintPreviewBase; }

#include "ui/scrspinbox.h"
#include "scribusapi.h"
#include "scribusstructs.h"

class PageSelector;
class PrintPreviewCreator;
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
	PrintPreview(QWidget* parent, ScribusDoc *docu, const QString& printer, PrintLanguage engine);
	~PrintPreview();

	bool isCMYKPreviewEnabled() const;
	bool isAntialiasingEnabled() const;
	bool isTransparencyEnabled() const;
	bool isInkChannelVisible(const QString& ink) { return (m_inkVisibilities.contains(ink) ? m_inkVisibilities[ink]->isChecked() : false); }
	bool isInkCoverageEnabled() const;
	bool useGhostscriptPreview() const;

	double inkCoverageThreshold() const;

	static bool usesGhostscript(const QString& printerName, PrintLanguage engine);
	/*!
	\author Franz Schmid
	\brief Creates the Preview of the Actual Page
	\param pageIndex int page number
	\param res int resolution
	\retval pixmap QPixmap print preview
	*/
	QPixmap createPreview(int pageIndex, int res);

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
	void toggleAllFromHeader();

signals:
	void doPrint();

protected:
	ScribusDoc *m_doc { nullptr };
	bool m_haveTiffSep { false };
	bool m_useGhostscript { true };
	QMap<QString, QCheckBox*> m_inkVisibilities;

	PrintPreviewCreator* m_previewCreator { nullptr };

	int m_currentPage { -1 };
	int m_scaleMode { 1 };
	int m_gsVersion { 0 };
	int  m_colorOutputMode { 0 };
	bool m_colorMode { false };
	bool m_useAntialiasing { false };
	bool m_showTransparency { false };
	bool m_useGCR { true };
	bool m_mirrorH { false };
	bool m_mirrorV { false };
	bool m_clipToMargins { false };
	bool m_convertSpots { true };

	/*! \brief Percentage value of the scaling widget */
	double m_scaleFactor { 1.0 };

	Ui::PrintPreviewBase *m_ui { nullptr };
	QLabel* m_previewLabel { nullptr };

	PrefsManager& m_prefsManager;

	void setValues();
	void getUserSelection(int);

	bool printOptionsHaveChanged(int pageIndex) const;

	//! \brief repaint sample on the dialog change
	void resizeEvent(QResizeEvent * event) override;

protected slots:
	void onPrintLanguageChange(int);
	void onInkTableCellDoubleClicked(int row);

	/*!
	\author Petr Vanek
	\date 09/03/2005
	\brief Recompute scaling factor of the preview image
	\param value spinbox value from signal
	*/
	void scaleBox_valueChanged(int value);
};

#endif
