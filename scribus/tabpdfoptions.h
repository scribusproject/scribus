#ifndef TABPDF_OPTS_H
#define TABPDF_OPTS_H

#include <qtabwidget.h>
#include <qwidget.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qlistbox.h>
#include <qmap.h>
#include <qvaluelist.h>
#include <qbuttongroup.h>
#include <qgroupbox.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qradiobutton.h>
#include <qspinbox.h>
#include <qtabwidget.h>
#include <qwidget.h>
#include <qpushbutton.h>
#include <qtoolbutton.h>
#include <qlineedit.h>
#include <qlabel.h>

#include "scribusapi.h"
#include "bookmwin.h"
#include "mspinbox.h"

class PDFOptions;

class SCRIBUS_API TabPDFOptions : public QTabWidget
{
	Q_OBJECT

public:
	TabPDFOptions(  QWidget* parent, PDFOptions *Optionen, SCFonts &AllFonts,
	                ProfilesL *PDFXProfiles, QMap<QString,int> DocFonts,
	                QValueList<PDFPresentationData> Eff, int unitIndex,
	                double PageH, double PageB, ScribusView *vie = 0 );
	~TabPDFOptions() {};
	void restoreDefaults();

	// GUI member pointers
	// Remember to initialize these in the initializer list of the ctor when
	// you add new ones. Add them in the same order here and in the initalizer
	// list. To help out, the members are sorted alphabetically by name.

	QComboBox* actionCombo;
	QLabel* actionLabel;
	QCheckBox* AddSec;
	QRadioButton* AllPages;
	QCheckBox* Article;
	QListBox* AvailFlist;
	MSpinBox* BleedBottom;
	QGroupBox* BleedGroup;
	QLabel* BleedIcon;
	MSpinBox* BleedLeft;
	MSpinBox* BleedRight;
	MSpinBox* BleedTop;
	QGroupBox* CBox;
	QCheckBox* CheckBM;
	QCheckBox* CheckBox1;
	QCheckBox* CheckBox10;
	QComboBox* CMethod;
	QGroupBox* ColorGroup;
	QLabel* ColorText1;
	QComboBox* ComboBind;
	QCheckBox* Compression;
	QRadioButton* continuousPages;
	QCheckBox* CopySec;
	QComboBox* CQuality;
	QRadioButton* doublePageLeft;
	QRadioButton* doublePageRight;
	QCheckBox* DSColor;
	QComboBox* EDirection;
	QComboBox* EDirection_2;
	QComboBox* EDirection_2_2;
	QGroupBox* Effects;
	QSpinBox* EffectTime;
	QComboBox* EffectType;
	QCheckBox* EmbedFonts;
	QListBox* EmbedList;
	QCheckBox* EmbedProfs;
	QCheckBox* EmbedProfs2;
	QCheckBox* Encry;
	QPushButton* EonAllPg;
	QCheckBox* fitWindow;
	QPushButton* FromEmbed;
	QPushButton* FromSubset;
	QGroupBox* GroupBox1;
	QGroupBox* GroupBox9;
	QGroupBox* groupDisplay;
	QGroupBox* GroupFont;
	QGroupBox* groupJava;
	QButtonGroup* groupNavigation;
	QGroupBox* GroupPass;
	QGroupBox* GroupSecSet;
	QCheckBox* hideMenuBar;
	QCheckBox* hideToolBar;
	QComboBox* ImageP;
	QLineEdit* InfoString;
	QComboBox* IntendI;
	QComboBox* IntendS;
	QSpinBox* LPIangle;
	QComboBox* LPIcolor;
	QSpinBox* LPIfreq;
	QComboBox* LPIfunc;
	QGroupBox* LPIgroup;
	QToolButton* MirrorH;
	QToolButton* MirrorV;
	QCheckBox* ModifySec;
	QCheckBox* NoEmbedded;
	QRadioButton* OnlySome;
	QComboBox* OutCombo;
	QButtonGroup* pageLayout;
	QLineEdit* PageNr;
	QCheckBox* PagePrev;
	QListBox* Pages;
	QSpinBox* PageTime;
	QLineEdit* PassOwner;
	QLineEdit* PassUser;
	QComboBox* PDFVersionCombo;
	QLabel* PDFX1;
	QLabel* PDFX2;
	QComboBox* PrintProfC;
	QCheckBox* PrintSec;
	QCheckBox* PrintSec2;
	QGroupBox* ProfsGroup;
	QLabel* ProfsTxt1;
	QLabel* ProfsTxt2;
	QLabel* ProfsTxt3;
	QLabel* ProfsTxt4;
	QButtonGroup* RangeGroup;
	QSpinBox* Resolution;
	QComboBox* RotateDeg;
	QRadioButton* singlePage;
	QComboBox* SolidPr;
	QCheckBox* SubsetFonts;
	QListBox* SubsetList;
	QWidget* tabColor;
	QWidget* tabFonts;
	QWidget* tabGeneral;
	QWidget* tabPDFX;
	QWidget* tabPresentation;
	QWidget* tabSecurity;
	QWidget* tabSpecial;
	QLabel* TextCom1;
	QLabel* TextCom2;
	QLabel* TextFont1;
	QLabel* TextFont1_2;
	QLabel* TextFont1_2a;
	QLabel* TextLabel1;
	QLabel* TextLabel1e;
	QLabel* TextLabel1x;
	QLabel* TextLabel2;
	QLabel* TextLabel2e;
	QLabel* TextLabel3;
	QLabel* TextLabel3e;
	QLabel* TextLabel4e;
	QLabel* TextLabel5e;
	QLabel* TextLabel6e;
	QLabel* textLPI1;
	QLabel* textLPI2;
	QLabel* textLPI3;
	QLabel* TextSec1;
	QLabel* TextSec2;
	QPushButton* ToEmbed;
	QPushButton* ToSubset;
	QRadioButton* useBookmarks;
	QRadioButton* useFullScreen;
	QCheckBox* useLayers;
	QRadioButton* useLayers2;
	QCheckBox* UseLPI;
	QCheckBox* useSpot;
	QRadioButton* useThumbnails;
	QRadioButton* useViewDefault;
	QSpinBox* ValC;
	QGroupBox* X3Group;
	// End GUI member pointers

	ScribusView * const view;
	SCFonts & AllFontsP;
	QValueList<PDFPresentationData> EffVal;
	int PgSel;
	QString SelLPIcolor;
	PDFOptions *Opts;
	double pageH;
	double pageB;
	bool cms;
	QValueList<QString> FontsToEmbed;
	QValueList<QString> FontsToSubset;
	double unitRatio;

signals:
	void noInfo();
	void hasInfo();

public slots:
	void ToggleEncr();
	void BleedChanged();
	void EnablePDFX(int a);
	void DoDownsample();
	void RemoveEmbed();
	void PutToEmbed();
	void RemoveSubset();
	void PutToSubset();
	void SelAFont(QListBoxItem*);
	void SelEFont(QListBoxItem*);
	void SelSFont(QListBoxItem*);
	void EmbedAll();
	void SubsetAll();
	void PagePr();
	void SetPgEff(int nr);
	void SetEffOpts(int nr);
	void ValidDI(int nr);
	void PDFMirror();
	void Rotation(int value);
	void DoEffects();
	void EffectOnAll();
	void SelRange(bool e);
	void EnablePr(int a);
	void EnablePG();
	void EnablePGI();
	void EnablePGI2();
	void EnableLPI(int a);
	void EnableLPI2();
	void SelLPIcol(int);
	void enableCMS(bool enable);
	void checkInfo();

protected:
	QGridLayout* CBoxLayout;
	QVBoxLayout* tabLayout;
	QVBoxLayout* Layout3;
	QHBoxLayout* Layout2;
	QVBoxLayout* tabLayout_3;
	QHBoxLayout* GroupFontLayout;
	QVBoxLayout* Layout4_2;
	QVBoxLayout* Layout5_2;
	QHBoxLayout* Layout5_2a;
	QVBoxLayout* Layout6;
	QGridLayout* GroupBox1Layout;
	QGridLayout* GroupBox9Layout;
	QGridLayout* GroupBox2Layout;
	QGridLayout* tabLayout_5;
	QGridLayout* tabOptionsGridLayout;
	QGridLayout* EffectsLayout;
	QHBoxLayout* Layout13;
	QVBoxLayout* RangeGroupLayout;
	QGridLayout* Layout11;
	QGridLayout* Layout11a;
	QVBoxLayout* tabColorLayout;
	QHBoxLayout* ColorGroupLayout;
	QGridLayout* ProfsGroupLayout;
	QVBoxLayout* tabPDFXLayout;
	QGridLayout* X3GroupLayout;
	QGridLayout* BleedGroupLayout;
	QVBoxLayout* tabSecurityLayout;
	QGridLayout* GroupPassLayout;
	QVBoxLayout* GroupSecSetLayout;
	QGridLayout* LPIgroupLayout;
	QVBoxLayout* tabSpecialLayout;
	QVBoxLayout* groupDisplayLayout;
	QHBoxLayout* LayoutSpecial;
	QVBoxLayout* pageLayoutLayout;
	QVBoxLayout* groupNavigationLayout;
	QHBoxLayout* groupJavaLayout;

	QString unit;
	int precision;
};
#endif
