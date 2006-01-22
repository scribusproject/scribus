/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
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
class SCRIBUS_API PDFExportDialog : public QDialog
{
	Q_OBJECT

public:
	PDFExportDialog( QWidget* parent, const QString & docFileName,
					 const QMap<QString,int> & DocFonts,
					 ScribusView * currView, PDFOptions & pdfOptions,
					 const QValueList<PDFPresentationData> & Eff,
					 const ProfilesL & PDFXProfiles, const SCFonts & AllFonts,
					 double unitRatio, const ProfilesL & printerProfiles);
	~PDFExportDialog() {};

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
	// Widgets
	QVBoxLayout* PDFExportLayout;
	QGridLayout* NameLayout;
	QHBoxLayout* Layout7;
	QGroupBox* Name;
	QCheckBox* multiFile;
	QToolButton* FileC;
	QPushButton* OK;
	QPushButton* Cancel;
	QLineEdit* fileNameLineEdit;
	TabPDFOptions* Options;

	// Other members
	QValueList<PDFPresentationData> EffVal;
	PDFOptions & Opts;
	double docUnitRatio;
	QString cmsDescriptorName;
	int components;
	const ProfilesL & appPrinterProfiles;
};

#endif // PDF_OPTS_H
