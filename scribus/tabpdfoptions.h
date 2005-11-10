#ifndef TABPDF_OPTS_H
#define TABPDF_OPTS_H

#include <qtabwidget.h>
#include <qwidget.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <q3listbox.h>
#include <qmap.h>
#include <q3valuelist.h>
#include <q3buttongroup.h>
#include <q3groupbox.h>
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
//Added by qt3to4:
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

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
					Q3ValueList<PDFPresentationData> Eff, int unitIndex,
					double PageH, double PageB, ScribusView *vie = 0 );
	~TabPDFOptions() {};
	void restoreDefaults();

	SCFonts *AllFontsP;
	QWidget* tabGeneral;
	QWidget* tabFonts;
	QWidget* tabPresentation;
	QWidget* tabSecurity;
	QWidget* tabColor;
	QWidget* tabPDFX;
	QLabel* TextLabel1;
	QSpinBox* Resolution;
	QCheckBox* Article;
	QCheckBox* useLayers;
	QCheckBox* Compression;
	QCheckBox* DSColor;
	QSpinBox* ValC;
	Q3GroupBox* CBox;
	QLabel* TextCom1;
	QLabel* TextCom2;
	QComboBox* CQuality;
	QComboBox* CMethod;
	Q3GroupBox* GroupBox1;
	QLabel* TextLabel2;
	QComboBox* PDFVersionCombo;
	QLabel* TextLabel1x;
	QComboBox* ComboBind;
	QCheckBox* CheckBox1;
	QToolButton* MirrorH;
	QToolButton* MirrorV;
	QComboBox* RotateDeg;
	QLabel* TextLabel3;
	QCheckBox* CheckBox10;
	QPushButton* EonAllPg;

	QLabel* TextLabel1e;
	QLabel* TextLabel2e;
	QLabel* TextLabel3e;
	QLabel* TextLabel4e;
	QLabel* TextLabel5e;
	QLabel* TextLabel6e;
	QComboBox* EffectType;
	QComboBox* EDirection;
	QComboBox* EDirection_2;
	QComboBox* EDirection_2_2;
	QSpinBox* PageTime;
	QSpinBox* EffectTime;
	Q3ListBox* Pages;
	Q3GroupBox* Effects;
	QCheckBox* PagePrev;
	QCheckBox* CheckBM;
	QCheckBox* EmbedFonts;
	Q3GroupBox* GroupFont;
	QLabel* TextFont1;
	Q3ListBox* AvailFlist;
	QPushButton* ToEmbed;
	QPushButton* FromEmbed;
	QLabel* TextFont1_2;
	QLabel* TextFont1_2a;
	Q3ButtonGroup* RangeGroup;
	QRadioButton* AllPages;
	QRadioButton* OnlySome;
	QLineEdit* PageNr;
	Q3ListBox* EmbedList;
	QPushButton* ToSubset;
	QPushButton* FromSubset;
	Q3ListBox* SubsetList;
	QCheckBox* SubsetFonts;
	Q3GroupBox* ColorGroup;
	Q3GroupBox* GroupBox9;
	QLabel* ColorText1;
	QComboBox* OutCombo;
	QCheckBox* EmbedProfs;
	QCheckBox* EmbedProfs2;
	Q3GroupBox* ProfsGroup;
	QComboBox* SolidPr;
	QLabel* ProfsTxt1;
	QComboBox* ImageP;
	QCheckBox* NoEmbedded;
	QLabel* ProfsTxt3;
	QLabel* ProfsTxt4;
	QLabel* ProfsTxt2;
	QComboBox* IntendS;
	QComboBox* IntendI;
	Q3GroupBox* X3Group;
	QComboBox* PrintProfC;
	QLineEdit* InfoString;
	QLabel* PDFX2;
	QLabel* PDFX1;
	Q3GroupBox* BleedGroup;
	QLabel* BleedIcon;
	MSpinBox* BleedTop;
	MSpinBox* BleedBottom;
	MSpinBox* BleedRight;
	MSpinBox* BleedLeft;
	Q3GroupBox* GroupPass;
	QLabel* TextSec2;
	QLabel* TextSec1;
	QLineEdit* PassOwner;
	QLineEdit* PassUser;
	Q3GroupBox* GroupSecSet;
	QCheckBox* PrintSec;
	QCheckBox* PrintSec2;
	QCheckBox* ModifySec;
	QCheckBox* CopySec;
	QCheckBox* AddSec;
	QCheckBox* Encry;
	Q3GroupBox* LPIgroup;
	QCheckBox* UseLPI;
	QSpinBox* LPIfreq;
	QSpinBox* LPIangle;
	QCheckBox* useSpot;
	QLabel* textLPI1;
	QLabel* textLPI2;
	QLabel* textLPI3;
	QComboBox* LPIcolor;
	QComboBox* LPIfunc;
	Q3ValueList<QString> FontsToEmbed;
	Q3ValueList<QString> FontsToSubset;
	ScribusView *view;
	Q3ValueList<PDFPresentationData> EffVal;
	int PgSel;
	QString SelLPIcolor;
	PDFOptions *Opts;
	double pageH;
	double pageB;
	bool cms;
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
	void SelAFont(Q3ListBoxItem*);
	void SelEFont(Q3ListBoxItem*);
	void SelSFont(Q3ListBoxItem*);
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
	
	QString unit;
	int precision;
};
#endif
