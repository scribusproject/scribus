/****************************************************************************
** Form interface generated from reading ui file 'PDFOpts.ui'
**
** Created: Mon Jun 4 10:02:30 2001
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef PDF_OPTS_H
#define PDF_OPTS_H

#include <qdialog.h>
#include <qbuttongroup.h>
#include <qgroupbox.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qspinbox.h>
#include <qtabwidget.h>
#include <qtoolbutton.h>
#include <qwidget.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qmap.h>
#include <qvaluelist.h>
#include <qlistbox.h>
#include "bookmwin.h"
#include "mspinbox.h"

class PDF_Opts : public QDialog
{
	Q_OBJECT

public:
	PDF_Opts( QWidget* parent, QString Fname, QMap<QString,QFont> DocFonts, ScribusView *vie, PDFOpt *Optionen,
	          QValueList<PreSet> Eff, ProfilesL *PDFXProfiles, SCFonts &AllFonts );
	~PDF_Opts();

	QLabel* Name;
	QLabel* TextLabel1;
	QTabWidget* Options;
	QWidget* tab;
	QSpinBox* Resolution;
	QCheckBox* Article;
	QCheckBox* Compression;
	QCheckBox* DSColor;
	QSpinBox* ValC;
	QGroupBox* CBox;
	QLabel* TextCom1;
	QLabel* TextCom2;
	QComboBox* CQuality;
	QComboBox* CMethod;
	QToolButton* FileC;
	QPushButton* OK;
	QPushButton* Cancel;
	QLineEdit* Datei;
	BookMView *BView;
	QGroupBox* GroupBox1;
	QLabel* TextLabel2;
	QComboBox* ComboBox1;
	QLabel* TextLabel1x;
	QComboBox* ComboBind;
	QCheckBox* CheckBox1;
	QCheckBox* CheckBox10;
	QPushButton* EonAllPg;
	QWidget* tab_3;
	QWidget* tab_5;
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
	QListBox* Pages;
	QGroupBox* Effects;
	QCheckBox* PagePrev;
	QCheckBox* CheckBM;
	QCheckBox* EmbedFonts;
	QGroupBox* GroupFont;
	QLabel* TextFont1;
	QListBox* AvailFlist;
	QPushButton* ToEmbed;
	QPushButton* FromEmbed;
	QLabel* TextFont1_2;
	QButtonGroup* RangeGroup;
	QRadioButton* AllPages;
	QRadioButton* OnlySome;
	QLineEdit* PageNr;
	QListBox* EmbedList;
	QWidget* tabcolor;
	QGroupBox* ColorGroup;
	QGroupBox* GroupBox9;
	QLabel* ColorText1;
	QComboBox* OutCombo;
	QCheckBox* EmbedProfs;
	QCheckBox* EmbedProfs2;
	QGroupBox* ProfsGroup;
	QComboBox* SolidPr;
	QLabel* ProfsTxt1;
	QComboBox* ImageP;
	QCheckBox* NoEmbedded;
	QLabel* ProfsTxt3;
	QLabel* ProfsTxt4;
	QLabel* ProfsTxt2;
	QComboBox* IntendS;
	QComboBox* IntendI;
	QWidget* tabpdfx;
	QGroupBox* X3Group;
	QComboBox* PrintProfC;
	QLineEdit* InfoString;
	QLabel* PDFX2;
	QLabel* PDFX1;
	QGroupBox* BleedGroup;
	QLabel* BleedIcon;
	MSpinBox* BleedTop;
	MSpinBox* BleedBottom;
	MSpinBox* BleedRight;
	MSpinBox* BleedLeft;
	QWidget* tabsec;
	QGroupBox* GroupPass;
	QLabel* TextSec2;
	QLabel* TextSec1;
	QLineEdit* PassOwner;
	QLineEdit* PassUser;
	QGroupBox* GroupSecSet;
	QCheckBox* PrintSec;
	QCheckBox* PrintSec2;
	QCheckBox* ModifySec;
	QCheckBox* CopySec;
	QCheckBox* AddSec;
	QCheckBox* Encry;
	QValueList<QString> FontsToEmbed;
	ScribusView *view;
	QValueList<PreSet> EffVal;
	int PgSel;
	int Einheit;

public slots:
	void DoExport();
	void ToggleEncr();
	void BleedChanged();
	void EnablePDFX(int a);
	void DoDownsample();
	void RemoveEmbed();
	void PutToEmbed();
	void SelAFont(QListBoxItem*);
	void SelEFont(QListBoxItem*);
	void EmbedAll();
	void ChangeFile();
	void PagePr();
	void SetPgEff(int nr);
	void SetEffOpts(int nr);
	void ValidDI(int nr);
	void DoEffects();
	void EffectOnAll();
	void SelRange(bool e);
	void EnablePr(int a);
	void EnablePG();
	void EnablePGI();
	void EnablePGI2();

protected:
	QGridLayout* CBoxLayout;
	QVBoxLayout* PDFOptsLayout;
	QHBoxLayout* Layout5;
	QVBoxLayout* tabLayout;
	QVBoxLayout* Layout3;
	QHBoxLayout* Layout2;
	QVBoxLayout* tabLayout_3;
	QHBoxLayout* GroupFontLayout;
	QVBoxLayout* Layout4_2;
	QVBoxLayout* Layout5_2;
	QVBoxLayout* Layout6;
	QHBoxLayout* Layout7;
	QGridLayout* GroupBox1Layout;
	QGridLayout* GroupBox9Layout;
	QGridLayout* GroupBox2Layout;
	QGridLayout* tabLayout_5;
	QGridLayout* EffectsLayout;
	QHBoxLayout* Layout13;
	QVBoxLayout* RangeGroupLayout;
	QGridLayout* Layout11;
	QVBoxLayout* tabcolorLayout;
	QHBoxLayout* ColorGroupLayout;
	QGridLayout* ProfsGroupLayout;
	QVBoxLayout* tabpdfxLayout;
	QGridLayout* X3GroupLayout;
	QGridLayout* BleedGroupLayout;
	QVBoxLayout* tabsecLayout;
	QGridLayout* GroupPassLayout;
	QVBoxLayout* GroupSecSetLayout;
};

#endif // PDF_OPTS_H
