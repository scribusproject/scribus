#ifndef PDF_OPTS_H
#define PDF_OPTS_H

#include <qdialog.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qtoolbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qmap.h>
#include <qvaluelist.h>
#include <qgroupbox.h>
#include <qcheckbox.h>

#include "scribusapi.h"
#include "scribusstructs.h"
#include "tabpdfoptions.h"

/**
 * @file pdfopts.h
 * @brief PDF export GUI code
 */

class PDFOptions;

/**
 * @brief PDF export dialog
 *
 * Most of the guts of the dialog actually come from TabPDFOptions, which
 * is also used by the preferences dialog.
 */
class SCRIBUS_API PDF_Opts : public QDialog
{
	Q_OBJECT

public:
	PDF_Opts( QWidget* parent, QString docFileName, QMap<QString,int> DocFonts, ScribusView *currView, PDFOptions *pdfOptions,
				QValueList<PDFPresentationData> Eff, ProfilesL *PDFXProfiles, SCFonts &AllFonts, double unitRatio, ProfilesL *printerProfiles);
	~PDF_Opts() {};

	void updateDocOptions();
	const QString cmsDescriptor();
	const int colorSpaceComponents();
	const QString getPagesString();

protected slots:
	void DoExport();
	void ChangeFile();
	void fileNameChanged();
	void enableSave();
	void disableSave();

protected:
	QVBoxLayout* PDFOptsLayout;
	QGridLayout* NameLayout;
	QHBoxLayout* Layout7;
	QGroupBox* Name;
	QCheckBox* multiFile;
	TabPDFOptions* Options;
	QToolButton* FileC;
	QPushButton* OK;
	QPushButton* Cancel;
	QLineEdit* fileNameLineEdit;
	QValueList<PDFPresentationData> EffVal;
	
	PDFOptions *Opts;
	double docUnitRatio;
	QString cmsDescriptorName;
	int components;
	ProfilesL *appPrinterProfiles;
};

#endif // PDF_OPTS_H
