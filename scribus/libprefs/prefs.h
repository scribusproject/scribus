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
#include "linecombo.h"
#include "linkbutton.h"
#include "scribusdoc.h"
#include "scfonts.h"
#include "mspinbox.h"
#include "scribus.h"
#include "prefsdialogbase.h"
class FontCombo;
class PolygonWidget;

#include "langmgr.h"

class Preferences : public PrefsDialogBase
{
    	Q_OBJECT

public:
	Preferences( QWidget* parent, preV *prefsData);
	~Preferences();

	//const QString getSelectedGUILang( void );
	QString selectedGUILang;
	void DrawRuler();

	QWidget* tab;
	QGroupBox* Mouse;
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
	QGroupBox* GroupBox20;
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
	QButtonGroup* ButtonGroup2;

	QWidget* tabGuides;
	QGroupBox* checkGrid;
	QGroupBox* groupBox1;
	QPushButton* majorGridColor;
	QLabel* textLabel4;
	QLabel* textLabel6;
	MSpinBox* majorSpace;
	QGroupBox* groupBox2;
	QLabel* textLabel5;
	QPushButton* minorGridColor;
	QLabel* textLabel7;
	MSpinBox* minorSpace;
	QLabel* textLabel8;
	QLabel* textLabel82;
	MSpinBox* snapDistance;
	QSpinBox* grabDistance;
	QCheckBox* checkMargin;
	QCheckBox* checkGuides;
	QCheckBox* checkBaseline;
	QLabel* textLabel2;
	QPushButton* baselineColor;
	QLabel* textLabel3;
	QPushButton* guideColor;
	QLabel* textLabel1;
	QPushButton* marginColor;
	QCheckBox* checkLink;
	QCheckBox* checkFrame;
	QCheckBox* checkPictures;
	
	QLabel* TextLabel4c;
	QWidget* tab_3;
	QWidgetStack* Fram;
	QLabel* TextLabel2_2;
	QLabel* TextLabel1_3;
	FontCombo* FontComb;
	QComboBox* SizeCombo;
	QComboBox* ForegroundT;
	QLabel* TextLabelT1;
	QLabel* TextLabel1_4;

	QWidget* tabTypo;
	QGroupBox* groupBox1a;
	QLabel* textLabel1a;
	QLabel* textLabel2a;
	QSpinBox* subDisplacement;
	QSpinBox* subScaling;
	QGroupBox* groupBox2a;
	QLabel* textLabel3a;
	QLabel* textLabel4a;
	QSpinBox* superDisplacement;
	QSpinBox* superScaling;
	QGroupBox* groupBox3a;
	QLabel* textLabel5a;
	QSpinBox* capsScaling;
	QGroupBox* groupBox4a;
	QLabel* textLabel6a;
	QSpinBox* autoLine;
	QLabel* textLabel7a;
	MSpinBox* baseGrid;
	MSpinBox* baseOffset;
	QLabel* textLabel8a;

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

	QToolButton* TextButton;
	QToolButton* BildButton;
	QToolButton* RectButton;
	QToolButton* OvalButton;
	QToolButton* LineButton;
	QToolButton* ZoomButton;
	QToolButton* PolyButton;
	QFrame* ToolFrame;
	QFrame* ToolFrame2;
	QLabel* TextLabelT21;
	QLabel* TextLabelT22;
	QLabel* TextLabelT23;
	QLabel* TextLabelT24;
	QLabel* TextLabelT25;
	QLabel* TextLabelT26;
	QComboBox* Foreground;
	QLabel* TextCol;
	QLabel* TextGap;
	QSpinBox* TextColVal;
	MSpinBox* TextGapVal;
	QSpinBox* Shade2;
	QComboBox* Background;
	QSpinBox* Shade;
	LineCombo* Linestyle;
	MSpinBox* LineW;
	QFrame* ToolFrame3;
	QLabel* TextLabelT31;
	QLabel* TextLabelT32;
	QLabel* TextLabelT35;
	QLabel* TextLabelT36;
	QComboBox* Foreground2;
	QSpinBox* Shade22;
	LineCombo* Linestyle2;
	MSpinBox* LineW2;
	QFrame* ToolFrame4;
	QLabel* TextLabelT42;
	QLabel* TextLabelT43;
	QLabel* TextLabelT44;
	QSpinBox* MinMag;
	QSpinBox* MaxMag;
	QSpinBox* StepMag;
	QButtonGroup* ToolFrame5;
	QLabel* TextLabelT51;
	QLabel* TextLabelT52;
	QLabel* TextLabelT53;
	QLabel* TextLabelT54;
	QRadioButton* FreeScale;
	QRadioButton* FrameScale;
	QCheckBox* Aspect;
	QSpinBox* XScale;
	QSpinBox* YScale;
	LinkButton* Kette;
	QComboBox* BackgroundP;
	QSpinBox* ShadeP;
	QLabel* TextLabelP;
	QWidget* tab_5;
	QGroupBox* Saving;
	QCheckBox* SaveAtQuit;
	QGroupBox* Preview;
	QComboBox* PreviewSize;
	QFrame* Frame6;
	QFrame* ToolFrame6;
	PolygonWidget* polyWidget;
	QWidget* tab_7;
	QButtonGroup* GroupSize;
	QLabel* GZText2;
	QComboBox* GZComboF;
	QComboBox* GZComboO;
	QLabel* GZText1;
	QLabel* GZText3;
	QLabel* GZText4;
	MSpinBox* Breite;
	MSpinBox* Hoehe;
	QCheckBox* Doppelseiten;
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
	int Einheit;
	double DisScale;
	ScribusApp *ap;
	QColor colorMajorGrid;
	QColor colorMinorGrid;
	QColor colorBaselineGrid;
	QColor colorPaper;
	QColor colorGuides;
	QColor colorMargin;


public slots:
	virtual void changeMajorColor();
	virtual void changeMinorColor();
	virtual void changePaperColor();
	virtual void changeBaselineColor();
	virtual void changeGuideColor();
	virtual void changeMarginColor();
	void ChangeDocs();
	void ChangeProfs();
	void ChangeScripts();
	void ChangeTemplates();
	void ChangeScaling();
	void ChangeGrid();
	void setDS();
	void setBreite(int v);
	void setHoehe(int v);
	void setTop(int v);
	void setBottom(int v);
	void setLeft(int v);
	void setRight(int v);
	void setSize(int gr);
	void setOrien(int ori);
	void changePapColor();
	void SetSample();
	void SetTool();
	void ToggleKette();
	void HChange();
	void VChange();
	void UnitChange();
	void SetDisScale();

protected:
	QGridLayout* tabLayout;
	QGridLayout* ButtonGroup1Layout;
	QHBoxLayout* GroupBox20Layout;
	QGridLayout* GroupBox200Layout;
	QHBoxLayout* MouseLayout;
	QGridLayout* Layout5;
	QHBoxLayout* tabLayout_4;
	QHBoxLayout* ButtonGroup2Layout;
	QGridLayout* Layout16;
	QHBoxLayout* ToolFrameLayout;
	QHBoxLayout* ToolFrame2Layout;
	QHBoxLayout* ToolFrame3Layout;
	QHBoxLayout* ToolFrame4Layout;
	QHBoxLayout* ToolFrame5Layout;
	QGridLayout* Layout15;
	QGridLayout* Layout15a;
	QGridLayout* Layout15b;
	QGridLayout* Layout15c;
	QGridLayout* Layout15d;
	QGridLayout* tabLayout_5;
	QHBoxLayout* SavingLayout;
	QHBoxLayout* PreviewLayout;
	QHBoxLayout* Layout3;
	QHBoxLayout* PolygonPropsLayout;
	QHBoxLayout* tabLayout_7;
	QVBoxLayout* Layout21;
	QVBoxLayout* Layout21b;
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
	QGridLayout* tabTypoLayout;
	QGridLayout* groupBox1aLayout;
	QGridLayout* groupBox2aLayout;
	QGridLayout* groupBox3aLayout;
	QGridLayout* groupBox4aLayout;
	QVBoxLayout* tabGuidesLayout;
	QGridLayout* checkGridLayout;
	QGridLayout* groupBox1Layout;
	QGridLayout* groupBox2Layout;
	QGridLayout* layout11;
	QHBoxLayout* layout2;
	QHBoxLayout* layout3;
	QHBoxLayout* layout9;
	QGridLayout* layout12;
	QVBoxLayout* tabViewLayout;
	QHBoxLayout* pageBackgroundLayout;
	QHBoxLayout* groupScratchLayout;
	QGridLayout* Layout4s;
	QHBoxLayout* buttonGroup1Layout;

private slots:
	void setSelectedGUILang( const QString &newLang );

private:
	LanguageManager langMgr;	
};

#endif // PREFS_H
