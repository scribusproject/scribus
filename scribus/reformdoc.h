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
class DocInfos;
class TabGuides;
class TabTools;
class TabCheckDoc;
class TabPDFOptions;
class FontPrefs;
class DocumentItemAttributes;
class TOCIndexPrefs;
class MarginWidget;

class ReformDoc : public PrefsDialogBase
{
	Q_OBJECT

public:
	ReformDoc( QWidget* parent, ScribusDoc* doc );
	~ReformDoc() {};
	QWidget* tabPage;
	MarginWidget* GroupRand;
	QGroupBox* groupBox7a;
	QGroupBox* dsGroupBox7;
	QGroupBox* groupScratch;
	QLabel* TextLabel5s;
	QLabel* TextLabel7s;
	QLabel* TextLabel1_3;
	QSpinBox* pageNumber;
	MSpinBox* widthMSpinBox;
	MSpinBox* heightMSpinBox;
	QCheckBox* sizeAllPages;
	QCheckBox* marginsForAllPages;
	MSpinBox* topScratch;
	MSpinBox* bottomScratch;
	MSpinBox* leftScratch;
	MSpinBox* rightScratch;
	QGroupBox* groupGap;
	MSpinBox* gapVertical;
	MSpinBox* gapHorizontal;
	QLabel* TextLabel5sg;
	QLabel* TextLabel7sg;
	QCheckBox* facingPages;
	QCheckBox* firstPage;
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
	QCheckBox* checkControl;
	QCheckBox* checkRuler;

	DocInfos* docInfos;
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
	QString prefsPageSizeName;

protected:
	QVBoxLayout* reformDocLayout;
	QHBoxLayout* dsGroupBox7Layout;
	QHBoxLayout* groupBox7aLayout;
	QHBoxLayout* applyLayout;
	QHBoxLayout* groupScratchLayout;
	QHBoxLayout* groupGapLayout;
	QGridLayout* layout4sg;
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
	virtual void restoreDefaults();
	virtual void unitChange();
	virtual void changePaperColor();
	virtual void setDS();
	virtual void setSize(const QString &);
	virtual void setOrien(int ori);
	virtual void setPageWidth(int v);
	virtual void setPageHeight(int v);
	virtual void switchCMS(bool enable);
	virtual void setTOCIndexData(QWidget *widgetToShow);
};

#endif // REFORMDOC_H
