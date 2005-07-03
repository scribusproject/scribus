/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
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

	QWidget* tabGeneral;
	QWidget* tabFonts;
	QWidget* tabPresentation;
	QWidget* tabSecurity;
	QWidget* tabColor;
	QWidget* tabPDFX;

	QLabel* Name;
	QLabel* TextLabel1;
	QTabWidget* Options;
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
	QGroupBox* GroupBox1;
	QLabel* TextLabel2;
	QComboBox* ComboBox1;
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
	QLabel* TextFont1_2a;
	QButtonGroup* RangeGroup;
	QRadioButton* AllPages;
	QRadioButton* OnlySome;
	QLineEdit* PageNr;
	QListBox* EmbedList;
	QPushButton* ToSubset;
	QPushButton* FromSubset;
	QListBox* SubsetList;
	QCheckBox* SubsetFonts;
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
	QGroupBox* LPIgroup;
	QCheckBox* UseLPI;
	QSpinBox* LPIfreq;
	QSpinBox* LPIangle;
	QLabel* textLPI1;
	QLabel* textLPI2;
	QLabel* textLPI3;
	QComboBox* LPIcolor;
	QComboBox* LPIfunc;
	QValueList<QString> FontsToEmbed;
	QValueList<QString> FontsToSubset;
	ScribusView *view;
	QValueList<PreSet> EffVal;
	int PgSel;
	int Einheit;
	QString SelLPIcolor;
	PDFOpt *Opts;

public slots:
	void DoExport();
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
	void ChangeFile();
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
	void EnablePDFX3save();

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
	QHBoxLayout* Layout5_2a;
	QVBoxLayout* Layout6;
	QHBoxLayout* Layout7;
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
};

#endif // PDF_OPTS_H
