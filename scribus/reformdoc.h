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

#include "scribusstructs.h"
#include "prefsdialogbase.h"

class QSpinBox;
class ScribusDoc;
class ScribusApp;
class TabTypograpy;
class HySettings;
class CMSPrefs;
class MSpinBox;
class TabGuides;
class TabTools;
class TabCheckDoc;
class TabPDFOptions;
class FontPrefs;
class DocumentItemAttributes;
class TOCIndexPrefs;

class ReformDoc : public PrefsDialogBase
{
	Q_OBJECT

public:
	ReformDoc( QWidget* parent, ScribusDoc* doc );
	~ReformDoc() {};
	QWidget* tabPage;
	QGroupBox* groupBox7;
	QGroupBox* groupBox7a;
	QGroupBox* dsGroupBox7;
	QGroupBox* groupScratch;
	QLabel* TextLabel5;
	QLabel* TextLabel7;
	QLabel* TextLabel5s;
	QLabel* TextLabel7s;
	QLabel* TextLabel1_3;
	QSpinBox* pageNumber;
	MSpinBox* topR;
	MSpinBox* bottomR;
	MSpinBox* leftR;
	MSpinBox* rightR;
	MSpinBox* widthMSpinBox;
	MSpinBox* heightMSpinBox;
	MSpinBox* topScratch;
	MSpinBox* bottomScratch;
	MSpinBox* leftScratch;
	MSpinBox* rightScratch;
	QCheckBox* facingPages;
	QCheckBox* firstPage;
	QLabel* Links;
	QLabel* Rechts;
	QLabel* Linkss;
	QLabel* Rechtss;
	QLabel*	widthQLabel;
	QLabel*	heightQLabel;
	QLabel*	unitQLabel;
	QLabel*	sizeQLabel;
	QLabel*	orientationQLabel;
	QComboBox*	sizeQComboBox;
	QComboBox*	orientationQComboBox;
	QCheckBox* checkLink;
	QCheckBox* checkFrame;
	QCheckBox* checkPictures;

	TabGuides* tabGuides;
	QWidget* tabView;
	TabTypograpy* tabTypo;
	TabTools* tabTools;
	HySettings* tabHyphenator;
	FontPrefs* tabFonts;
	TabCheckDoc* tabDocChecker;
	CMSPrefs* tabColorManagement;
	TabPDFOptions* tabPDF;
	QLabel* textLabel9;
	QPushButton* backColor;
	QButtonGroup* pageBackground;
	QCheckBox* checkUnprintable;
	QGroupBox* groupAutoSave;
	QLabel* textLabel1m;
	QSpinBox* autoSaveTime;
	QComboBox* unitCombo;
	DocumentItemAttributes* tabDocItemAttributes;
	TOCIndexPrefs* tabTOCIndexPrefs;
	
	ScribusApp *ap;
	ScribusDoc* currDoc;
	int einheit;
	int decimals;
	double unitRatio;
	double pageWidth;
	double pageHeight;
	QColor colorPaper;

protected:
	QVBoxLayout* reformDocLayout;
	QHBoxLayout* dsGroupBox7Layout;
	QHBoxLayout* groupBox7Layout;
	QHBoxLayout* groupBox7aLayout;
	QHBoxLayout* groupScratchLayout;
	QGridLayout* dsLayout4;
	QGridLayout* layout4;
	QGridLayout* layout4a;
	QGridLayout* layout4s;
	QHBoxLayout* Layout3;
	QVBoxLayout* tabViewLayout;
	QVBoxLayout* buttonGroup1Layout;
	QHBoxLayout* layout7;
	QVBoxLayout* pageBackgroundLayout;
	QHBoxLayout* layout10;
	QHBoxLayout* groupAutoSaveLayout;
	
	QStringList docAttributesList;

protected slots:
	virtual void unitChange();
	virtual void changePaperColor();
	virtual void setDS();
	virtual void setTop(int v);
	virtual void setBottom(int v);
	virtual void setLeft(int v);
	virtual void setRight(int v);
	virtual void switchCMS(bool enable);
	virtual void setTOCIndexData(QWidget *widgetToShow);
};

#endif // REFORMDOC_H
