/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PDF_OPTS_H
#define PDF_OPTS_H

#include <QDialog>
#include <QList>
class QHBoxLayout;
class QGridLayout;
class QVBoxLayout;
class QGroupBox;
class QLineEdit;
class QToolButton;
class QCheckBox;
class QPushButton;

#include "scribusapi.h"
#include "scribusstructs.h"
#include "ui/tabpdfoptions.h"
class ScribusView;

/**
 * @file pdfopts.h
 * @brief PDF export GUI code
 */

class PDFOptions;
class ScribusDoc;

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
					 const QMap<QString, int > & DocFonts,
					 ScribusView * currView, PDFOptions & pdfOptions,
					 const QList<PDFPresentationData> & Eff,
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
	ScribusDoc* doc;
	QList<PDFPresentationData> EffVal;
	PDFOptions & Opts;
	double docUnitRatio;
	QString cmsDescriptorName;
	int components;
	const ProfilesL & appPrinterProfiles;
};

#endif // PDF_OPTS_H
