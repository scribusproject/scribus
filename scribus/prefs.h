#ifndef PREFS_H
#define PREFS_H

#include <qdialog.h>
#include <qbuttongroup.h>
#include <qcombobox.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qspinbox.h>
#include <qwidget.h>
#include <qlayout.h>
#include <qtoolbutton.h>
#include <qframe.h>
#include <qcheckbox.h>
#include <qwidgetstack.h>
#include <qpixmap.h>
#include <qslider.h>
#include <qlineedit.h>
#include <qfiledialog.h>

class QListView;

#include "scribusapi.h"
#include "scribusstructs.h"

#include "mspinbox.h"
#include "scribus.h"
#include "prefsdialogbase.h"
class TabTypograpy;
class HySettings;
class CMSPrefs;
class KeyManager;
class TabGuides;
class TabTools;
class TabCheckDoc;
class TabPDFOptions;
class FontPrefs;
class DocumentItemAttributes;
class TOCIndexPrefs;
class MarginWidget;
class PrefsManager;
class PageLayouts;

#include "langmgr.h"

class SCRIBUS_API Preferences : public PrefsDialogBase
{
    	Q_OBJECT

public:
	Preferences( QWidget* parent);
	~Preferences();
	void drawRuler();
	void updatePreferences();

	QString prefsPageSizeName;
	//const QString getSelectedGUILang( void );
	QString selectedGUILang;


	QWidget* tab;
	QLabel* TextLabel1_2;
	QSpinBox* SpinBox3;
	QSpinBox* Recen;
	QButtonGroup* ButtonGroup1;
	QLabel* guiLangLabel;
	QLabel* TextGstil;
	QLabel* TextGstil2;
	QSpinBox* GFsize;
	QComboBox* guiLangCombo;
	QComboBox* GUICombo;
	QComboBox* UnitCombo;
	QLabel* unitComboText;
	QGroupBox* GroupBox200;
	QLabel* PfadText;
	QLineEdit* Docs;
	QToolButton* FileC;
	QLabel* PfadText2;
	QLineEdit* ProPfad;
	QToolButton* FileC2;
	QLabel* PfadText3;
	QLineEdit* ScriptPfad;
	QToolButton* FileC3;
	QLabel* PfadText4;
	QLineEdit* DocumentTemplateDir;
	QToolButton* FileC4;

	TabGuides* tabGuides;

	QLabel* TextLabel4c;

	TabTypograpy* tabTypo;

	QWidget* tabView;
	QButtonGroup* pageBackground;
	QLabel* textLabel9;
	QCheckBox* checkLink;
	QCheckBox* checkFrame;
	QCheckBox* checkPictures;
	QCheckBox* checkControl;
	QCheckBox* checkRuler;
	QPushButton* backColor;
	QCheckBox* checkUnprintable;
	QGroupBox* groupScratch;
	MSpinBox* topScratch;
	MSpinBox* bottomScratch;
	MSpinBox* leftScratch;
	MSpinBox* rightScratch;
	QLabel* TextLabel7s;
	QLabel* TextLabel5s;
	QLabel* Linkss;
	QLabel* Rechtss;
	QGroupBox* groupGap;
	MSpinBox* gapVertical;
	MSpinBox* gapHorizontal;
	QLabel* TextLabel5sg;
	QLabel* TextLabel7sg;

	TabTools* tabTools;

	HySettings* tabHyphenator;
	FontPrefs* tabFonts;
	TabCheckDoc* tabDocChecker;
	CMSPrefs* tabColorManagement;
	TabPDFOptions* tabPDF;
	DocumentItemAttributes* tabDefaultItemAttributes;
	TOCIndexPrefs* tabDefaultTOCIndexPrefs;
	KeyManager* tabKeys;

	QLabel* textLabelP;
	QWidget* tab_5;
	QGroupBox* saving;
	QCheckBox* SaveAtQuit;
	QGroupBox* preview;
	QComboBox* PreviewSize;
	QWidget* tab_7;
	QButtonGroup* GroupSize;
	QLabel* GZText2;
	QComboBox* GZComboF;
	QComboBox* GZComboO;
	QLabel* GZText1;
	QLabel* GZText3;
	QLabel* GZText4;
	MSpinBox* pageWidth;
	MSpinBox* pageHeight;
	PageLayouts* docLayout;
	MarginWidget* GroupRand;
	QGroupBox* GroupAS;
	QCheckBox* ASon;
	QLabel* ASText;
	QSpinBox* ASTime;
	QGroupBox* urGroup;
	QLabel* urLabel;
	QSpinBox* urSpinBox;
	QGroupBox* CaliGroup;
	QLabel* CaliText;
	QLabel* CaliRuler;
	QSlider* CaliSlider;
	QLabel* CaliAnz;
 	QWidget* ExtTool;
	QGroupBox* ghostscriptGroup;
	QLabel* ghostscriptLabel;
	QLineEdit* ghostscriptLineEdit;
	QToolButton* ghostscriptChangeButton;
	QCheckBox* GSantiText;
	QCheckBox* GSantiGraph;
	QSpinBox* GSResolution;
	QLabel* GSResText;
	QGroupBox* groupImageEditor;
	QLabel* imageEditorLabel;
	QLineEdit* imageEditorLineEdit;
	QToolButton* imageEditorChangeButton;
	QWidget* Misc;
	QGroupBox* groupPrint;
	QCheckBox* ClipMarg;
	QCheckBox* DoGCR;
	QCheckBox* AskForSubs;
	QCheckBox* stylePreview;
	QCheckBox* startUpDialog;
	//! lorem ipsum
	QGroupBox* groupLI;
	QCheckBox* useStandardLI;
	QSpinBox* paragraphsLI;
	QLabel* paraLabelLI;

	double unitRatio;
	double PFactor;
	double Pagebr;
	double Pageho;
	int choosenLayout;
	int docUnitIndex;
	double DisScale;
	ScribusApp *ap;
	QColor colorPaper;


public slots:
	void changeDocs();
	void changeProfs();
	void changeScripts();
	void changeDocumentTemplates();
	void changeGhostscript();
	void changeImageEditor();
	virtual void changePaperColor();

	void setDS(int layout);
	void setPageWidth(int v);
	void setPageHeight(int v);
	void setPageSize();
	void setSize(const QString &);
	void setOrien(int ori);
	void unitChange();
	void setDisScale();
	void switchCMS(bool enable);


protected:
	
	QHBoxLayout* groupImageEditorLayout;
	QVBoxLayout* groupGhostScriptLayout;
	QHBoxLayout* ghostscriptLayout;
	
	QGridLayout* tabLayout;
	QGridLayout* ButtonGroup1Layout;
	QGridLayout* GroupBox200Layout;
	QGridLayout* tabLayout_5;
	QHBoxLayout* savingLayout;
	QHBoxLayout* previewLayout;
	QHBoxLayout* Layout3;
	QHBoxLayout* tabLayout_7;
	QVBoxLayout* Layout21;
	QVBoxLayout* GroupSizeLayout;
	QGridLayout* Layout6;
	QHBoxLayout* Layout5_2;
	QGridLayout* GroupASLayout;
	QVBoxLayout* CaliGroupLayout;
	QHBoxLayout* layout15ca;
	QVBoxLayout* ExtToolLayout;
	
	
	QHBoxLayout* GSlayout2;
	QVBoxLayout* MiscLayout;
	QVBoxLayout* groupPrintLayout;
	//! layout of the lorem ipsum group
	QVBoxLayout* groupLILayout;
	QVBoxLayout* tabViewLayout;
	QGridLayout* pageBackgroundLayout;
	QHBoxLayout* layout10;
	QHBoxLayout* groupScratchLayout;
	QGridLayout* Layout4s;
	QHBoxLayout* groupGapLayout;
	QGridLayout* layout4sg;
	QHBoxLayout* dsLayout4p;
	QVBoxLayout* dsLayout4pv;

	QStringList defaultAttributesList;

protected slots:
	virtual void setTOCIndexData(QWidget *widgetToShow);

private slots:
	void setSelectedGUILang( const QString &newLang );
	void setActionHistoryLength();

private:
	LanguageManager langMgr;
	PrefsManager* prefsManager;
};

#endif // PREFS_H
