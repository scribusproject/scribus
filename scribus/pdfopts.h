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
#include "tabpdfoptions.h"

class PDF_Opts : public QDialog
{
	Q_OBJECT

public:
	PDF_Opts( QWidget* parent, QString Fname, QMap<QString,QFont> DocFonts, ScribusView *vie, PDFOptions *Optionen,
	          QValueList<PDFPresentationData> Eff, ProfilesL *PDFXProfiles, SCFonts &AllFonts );
	~PDF_Opts() {};

	QLabel* Name;
	TabPDFOptions* Options;
	QToolButton* FileC;
	QPushButton* OK;
	QPushButton* Cancel;
	QLineEdit* Datei;
	QValueList<PDFPresentationData> EffVal;
	PDFOptions *Opts;

public slots:
	void DoExport();
	void ChangeFile();

protected:
	QVBoxLayout* PDFOptsLayout;
	QHBoxLayout* Layout5;
	QHBoxLayout* Layout7;
};

#endif // PDF_OPTS_H
