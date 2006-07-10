/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef REFORMDOC_H
#define REFORMDOC_H

#include <qlayout.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qgroupbox.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qtoolbutton.h>
#include <qcolor.h>

#include "scribusapi.h"
#include "scribusstructs.h"
#include "prefsdialogbase.h"

class QSpinBox;
class DocSections;
class ScribusDoc;
class ScribusMainWindow;
class TabTypograpy;
class HySettings;
class CMSPrefs;
class MSpinBox;
class DocInfos;
class TabGuides;
class TabTools;
class TabCheckDoc;
class TabPDFOptions;
class FontPrefs;
class DocumentItemAttributes;
class TOCIndexPrefs;
class MarginWidget;
class PageLayouts;
class TabDisplay;
class TabDocument;


class SCRIBUS_API ReformDoc : public PrefsDialogBase
{
	Q_OBJECT

public:
	ReformDoc( QWidget* parent, ScribusDoc* doc );
	~ReformDoc() {};
	
	const int getSelectedUnit();
	const bool imageResolutionChanged();
	const bool colorManagementSettingsChanged();
	void updateDocumentSettings();	

	ScribusMainWindow *ScMW;
	ScribusDoc* currDoc;
	int docUnitIndex;
	int decimals;
	double unitRatio;
/*	double pageWidth;
	double pageHeight;*/
	int choosenLayout;
	QColor colorPaper;
	QString prefsPageSizeName;

protected:
/*	QVBoxLayout* reformDocLayout;
	QHBoxLayout* dsGroupBox7Layout;
	QHBoxLayout* groupBox7aLayout;
	QHBoxLayout* groupScratchLayout;
	QHBoxLayout* groupGapLayout;
	QGridLayout* layout4sg;
	QGridLayout* dsLayout4;
	QHBoxLayout* dsLayout4p;
	QVBoxLayout* dsLayout4pv;
	QGridLayout* layout4;
	QGridLayout* layout4a;
	QGridLayout* layout4s;
	QHBoxLayout* Layout3;
// 	QVBoxLayout* tabViewLayout;
	QVBoxLayout* buttonGroup1Layout;
	QHBoxLayout* layout7;
	QVBoxLayout* pageBackgroundLayout;
	QHBoxLayout* layout10;
	QHBoxLayout* groupAutoSaveLayout;*/
	QStringList docAttributesList;
	
	TabDocument* tabPage;

// 	PageLayouts* docLayout;
// 	MarginWidget* GroupRand;
// 	QGroupBox* groupBox7a;
// 	QGroupBox* dsGroupBox7;
// 	QGroupBox* groupScratch;
// 	QLabel* TextLabel5s;
// 	QLabel* TextLabel7s;
// 	QLabel* TextLabel1_3;
// 	QSpinBox* pageNumber;

	DocInfos* docInfos;
	DocSections* tabDocSections;
	TabGuides* tabGuides;
	TabDisplay* tabView;
	TabTypograpy* tabTypo;
	TabTools* tabTools;
	HySettings* tabHyphenator;
	FontPrefs* tabFonts;
	TabCheckDoc* tabDocChecker;
	CMSPrefs* tabColorManagement;
	TabPDFOptions* tabPDF;
	QLabel* textLabel9;

	QLabel* textLabel1m;
	DocumentItemAttributes* tabDocItemAttributes;
	TOCIndexPrefs* tabTOCIndexPrefs;
	
	bool viewToRecalcPictureRes;
	//Replace with commonstrings when translators have that one done.
	QString customText, customTextTR;

protected slots:
	virtual void restoreDefaults();
	virtual void unitChange();
	virtual void setDS(int layout);
	virtual void switchCMS(bool enable);
	virtual void showWidgetInStack(QWidget *widgetToShow);
	virtual void applyChangesButton_clicked();
};

#endif // REFORMDOC_H
