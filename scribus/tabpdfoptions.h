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

	void unitChange(int newUnitIndex);

	int PgSel;
	QValueList<PDFPresentationData> EffVal;
	QString SelLPIcolor;
	QValueList<QString> FontsToEmbed;
	QValueList<QString> FontsToSubset;

	// GUI member pointers
	// Remember to initialize these in the initializer list of the ctor when
	// you add new ones. Add them in the same order here and in the initalizer
	// list. To help out, the members are sorted alphabetically by name.
	QComboBox* actionCombo;
	QCheckBox* AddSec;
	QRadioButton* AllPages;
	QCheckBox* Article;
	MSpinBox* BleedBottom;
	MSpinBox* BleedLeft;
	MSpinBox* BleedRight;
	MSpinBox* BleedTop;
	QCheckBox* CheckBM;
	QCheckBox* CheckBox1;
	QCheckBox* CheckBox10;
	QComboBox* CMethod;
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
	QSpinBox* EffectTime;
	QComboBox* EffectType;
	QCheckBox* EmbedProfs;
	QCheckBox* EmbedProfs2;
	QCheckBox* Encry;
	QCheckBox* fitWindow;
	QCheckBox* hideMenuBar;
	QCheckBox* hideToolBar;
	QComboBox* ImageP;
	QLineEdit* InfoString;
	QComboBox* IntendI;
	QComboBox* IntendS;
	QSpinBox* LPIangle;
	QSpinBox* LPIfreq;
	QComboBox* LPIfunc;
	QToolButton* MirrorH;
	QToolButton* MirrorV;
	QCheckBox* ModifySec;
	QCheckBox* NoEmbedded;
	QComboBox* OutCombo;
	QLineEdit* PageNr;
	QListBox* Pages;
	QSpinBox* PageTime;
	QLineEdit* PassOwner;
	QLineEdit* PassUser;
	QComboBox* PDFVersionCombo;
	QComboBox* PrintProfC;
	QCheckBox* PrintSec;
	QSpinBox* Resolution;
	QComboBox* RotateDeg;
	QRadioButton* singlePage;
	QComboBox* SolidPr;
	QRadioButton* useBookmarks;
	QRadioButton* useFullScreen;
	QCheckBox* useLayers;
	QRadioButton* useLayers2;
	QCheckBox* UseLPI;
	QCheckBox* useSpot;
	QRadioButton* useThumbnails;
	QSpinBox* ValC;
	// End GUI member pointers

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
	// Non-GUI protected members
	QString unit;
	int precision;
	double unitRatio;
	ScribusView * const view;
	SCFonts & AllFontsP;
	PDFOptions *Opts;
	double pageH;
	double pageB;
	bool cms;

	// Protected GUI member pointers
	QLabel* actionLabel;
	QListBox* AvailFlist;
	QGroupBox* BleedGroup;
	QGridLayout* BleedGroupLayout;
	QLabel* BleedIcon;
	QGroupBox* CBox;
	QGridLayout* CBoxLayout;
	QGroupBox* ColorGroup;
	QHBoxLayout* ColorGroupLayout;
	QLabel* ColorText1;
	QGroupBox* Effects;
	QGridLayout* EffectsLayout;
	QCheckBox* EmbedFonts;
	QListBox* EmbedList;
	QPushButton* EonAllPg;
	QPushButton* FromEmbed;
	QPushButton* FromSubset;
	QGroupBox* GroupBox1;
	QGridLayout* GroupBox1Layout;
	QGridLayout* GroupBox2Layout;
	QGroupBox* GroupBox9;
	QGridLayout* GroupBox9Layout;
	QGroupBox* groupDisplay;
	QVBoxLayout* groupDisplayLayout;
	QGroupBox* GroupFont;
	QHBoxLayout* GroupFontLayout;
	QGroupBox* groupJava;
	QHBoxLayout* groupJavaLayout;
	QButtonGroup* groupNavigation;
	QVBoxLayout* groupNavigationLayout;
	QGroupBox* GroupPass;
	QGridLayout* GroupPassLayout;
	QGroupBox* GroupSecSet;
	QVBoxLayout* GroupSecSetLayout;
	QGridLayout* Layout11;
	QGridLayout* Layout11a;
	QHBoxLayout* Layout13;
	QHBoxLayout* Layout2;
	QVBoxLayout* Layout3;
	QVBoxLayout* Layout4_2;
	QVBoxLayout* Layout5_2;
	QHBoxLayout* Layout5_2a;
	QVBoxLayout* Layout6;
	QHBoxLayout* LayoutSpecial;
	QComboBox* LPIcolor;
	QGroupBox* LPIgroup;
	QGridLayout* LPIgroupLayout;
	QRadioButton* OnlySome;
	QButtonGroup* pageLayout;
	QVBoxLayout* pageLayoutLayout;
	QCheckBox* PagePrev;
	QLabel* PDFX1;
	QLabel* PDFX2;
	QCheckBox* PrintSec2;
	QGroupBox* ProfsGroup;
	QGridLayout* ProfsGroupLayout;
	QLabel* ProfsTxt1;
	QLabel* ProfsTxt2;
	QLabel* ProfsTxt3;
	QLabel* ProfsTxt4;
	QButtonGroup* RangeGroup;
	QVBoxLayout* RangeGroupLayout;
	QCheckBox* SubsetFonts;
	QListBox* SubsetList;
	QWidget* tabColor;
	QVBoxLayout* tabColorLayout;
	QWidget* tabFonts;
	QWidget* tabGeneral;
	QVBoxLayout* tabLayout;
	QVBoxLayout* tabLayout_3;
	QGridLayout* tabLayout_5;
	QGridLayout* tabOptionsGridLayout;
	QWidget* tabPDFX;
	QVBoxLayout* tabPDFXLayout;
	QWidget* tabPresentation;
	QWidget* tabSecurity;
	QVBoxLayout* tabSecurityLayout;
	QWidget* tabSpecial;
	QVBoxLayout* tabSpecialLayout;
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
	QRadioButton* useViewDefault;
	QGroupBox* X3Group;
	QGridLayout* X3GroupLayout;
	// end protected GUI member pointers

};
#endif
