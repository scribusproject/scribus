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

#include "scribusstructs.h"

#include "linecombo.h"
#include "linkbutton.h"
#include "scfonts.h"
#include "mspinbox.h"
#include "scribus.h"
#include "prefsdialogbase.h"
class FontCombo;
class PolygonWidget;
class ArrowChooser;
class TabTypograpy;
class HySettings;
class CMSPrefs;
class KeyManager;
class TabGuides;

#include "langmgr.h"

class Preferences : public PrefsDialogBase
{
    	Q_OBJECT

public:
	Preferences( QWidget* parent, ApplicationPrefs *prefsData);
	~Preferences();

	//const QString getSelectedGUILang( void );
	QString selectedGUILang;
	void drawRuler();

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
	QLineEdit* TemplateDir;
	QToolButton* FileC4;

	TabGuides* tabGuides;
	
	QLabel* TextLabel4c;

	TabTypograpy* tabTypo;

	QWidget* tabView;
	QButtonGroup* pageBackground;
	QLabel* textLabel9;
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
	QButtonGroup* buttonGroup1;
	QRadioButton* inBackground;
	QRadioButton* inForeground;

	QWidget* tabTools;
	QButtonGroup* buttonGroupTools;
	QToolButton* toolShape;
	QToolButton* toolPoly;
	QToolButton* toolImage;
	QToolButton* toolText;
	QToolButton* toolLine;
	QToolButton* toolZoom;
	QWidgetStack* subStackTools;
	QWidget* subTabText;
	QLabel* textLabel3b;
	QLabel* textLabel3b2;
	QLabel* textLabel1b;
	QLabel* textLabel2b;
	QLabel* textLabel4b;
	QSpinBox* columnsText;
	QLabel* previewText;
	FontCombo* fontComboText;
	QComboBox* sizeComboText;
	QComboBox* colorComboText;
	QComboBox* colorComboStrokeText;
	QLabel* textLabel5b;
	MSpinBox* gapText;
	QWidget* subTabShape;
	QLabel* textLabel9b;
	QLabel* textLabel10b;
	MSpinBox* lineWidthShape;
	QLabel* textLabel12b;
	QLabel* textLabel11b;
	LineCombo* comboStyleShape;
	QSpinBox* shadingFillShape;
	QComboBox* comboFillShape;
	QSpinBox* shadingLineShape;
	QLabel* textLabel7b;
	QComboBox* colorComboLineShape;
	QLabel* textLabel8b;
	QWidget* subTabLine;
	MSpinBox* lineWidthLine;
	QSpinBox* shadingLine;
	QComboBox* colorComboLine;
	QLabel* textLabel13b;
	QLabel* textLabel14b;
	QLabel* textLabel15b;
	QLabel* textLabel16b;
	LineCombo* comboStyleLine;
	QWidget* subTabImage;
	QComboBox* comboFillImage;
	QLabel* textLabel19b;
	QSpinBox* shadingFillImage;
	QLabel* textLabel20b;
	QButtonGroup* buttonGroup3;
	QLabel* textLabel17b;
	QLabel* textLabel18b;
	QSpinBox* scalingHorizontal;
	QSpinBox* scalingVertical;
	LinkButton* chainButton;
	QButtonGroup* buttonGroup5;
	QCheckBox* checkRatioImage;
	QWidget* subTabPolygon;
	PolygonWidget* polyWidget;
	QWidget* subTabZoom;
	QLabel* textLabel21b;
	QSpinBox* minimumZoom;
	QLabel* textLabel22b;
	QLabel* textLabel23b;
	QSpinBox* maximumZoom;
	QSpinBox* zoomStep;
	ArrowChooser* startArrow;
	ArrowChooser* endArrow;
	QLabel* startArrowText;
	QLabel* endArrowText;
	QLabel* arrowText;

	HySettings* tabHyphenator;
	CMSPrefs* tabColorManagement;
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
	QCheckBox* facingPages;
	QCheckBox* Linkszuerst;
	QGroupBox* GroupRand;
	QLabel* GRText3;
	MSpinBox* TopR;
	MSpinBox* BottomR;
	MSpinBox* RightR;
	MSpinBox* LeftR;
	QLabel* GRText2;
	QLabel* GRText1;
	QLabel* GRText4;
	QGroupBox* GroupAS;
	QCheckBox* ASon;
	QLabel* ASText;
	QSpinBox* ASTime;
	QGroupBox* CaliGroup;
	QLabel* CaliText;
	QLabel* CaliRuler;
	QSlider* CaliSlider;
	QLabel* CaliAnz;
 	QWidget* ExtTool;
	QGroupBox* groupGS;
	QLabel* GSText;
	QLineEdit* GSName;
	QCheckBox* GSantiText;
	QCheckBox* GSantiGraph;
	QGroupBox* groupGimp;
	QLabel* GimpText;
	QLineEdit* GimpName;
	QWidget* Misc;
	QGroupBox* groupPrint;
	QCheckBox* ClipMarg;
	QCheckBox* DoGCR;
	
	SCFonts *fon; // can't use a reference, and can't copy whole structure...
	double Umrech;
	double PFactor;
	double Pagebr;
	double Pageho;
	double RandT;
	double RandB;
	double RandL;
	double RandR;
	int docUnitIndex;
	double DisScale;
	ScribusApp *ap;
	QColor colorPaper;


public slots:
	virtual void changePaperColor();
	virtual void toggleChain();
	virtual void hChange();
	virtual void vChange();
	virtual void changeImageScalingFree(int);
	virtual void changeImageScalingRatio(int);
	virtual void setSample();
	virtual void setTool();
	void changeDocs();
	void changeProfs();
	void changeScripts();
	void changeTemplates();
	void changeScaling();
	void changeGrid();
	void setDS();
	void setPageWidth(int v);
	void setPageHeight(int v);
	void setTop(int v);
	void setBottom(int v);
	void setLeft(int v);
	void setRight(int v);
	void setSize(int gr);
	void setOrien(int ori);
	void changePapColor();
	void unitChange();
	void setDisScale();

protected:
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
	QHBoxLayout* Layout8;
	QGridLayout* GroupRandLayout;
	QGridLayout* GroupASLayout;
	QVBoxLayout* CaliGroupLayout;
	QHBoxLayout* layout15ca;
	QVBoxLayout* ExtToolLayout;
	QVBoxLayout* groupGSLayout;
	QHBoxLayout* GSlayout;
	QHBoxLayout* groupGimpLayout;
	QVBoxLayout* MiscLayout;
	QVBoxLayout* groupPrintLayout;
	QVBoxLayout* tabViewLayout;
	QHBoxLayout* pageBackgroundLayout;
	QHBoxLayout* groupScratchLayout;
	QGridLayout* Layout4s;
	QHBoxLayout* buttonGroup1Layout;
	QHBoxLayout* tabToolsLayout;
	QVBoxLayout* buttonGroupToolsLayout;
	QGridLayout* subTabTextLayout;
	QGridLayout* subTabShapeLayout;
	QGridLayout* subTabLineLayout;
	QGridLayout* subTabImageLayout;
	QGridLayout* buttonGroup3Layout;
	QHBoxLayout* buttonGroup5Layout;
	QHBoxLayout* subTabPolygonLayout;
	QGridLayout* subTabZoomLayout;

private slots:
	void setSelectedGUILang( const QString &newLang );

private:
	LanguageManager langMgr;
};

#endif // PREFS_H
