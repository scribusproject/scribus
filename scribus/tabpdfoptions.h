/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
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
class PDFExportDialog;

class SCRIBUS_API TabPDFOptions : public QTabWidget
{
	Q_OBJECT

public:
	TabPDFOptions(  QWidget* parent, PDFOptions & Optionen,
	                const SCFonts &AllFonts,
	                const ProfilesL & PDFXProfiles,
	                const QMap<QString,int> & DocFonts,
	                const QValueList<PDFPresentationData> & Eff,
	                int unitIndex, double PageH, double PageB,
	                ScribusView *vie = 0 );
	~TabPDFOptions() {};
	void restoreDefaults();

	void unitChange(int newUnitIndex);

	// GUI member pointers
	// Remember to initialize these in the initializer list of the ctor when
	// you add new ones. Add them in the same order here and in the initalizer
	// list. To help out, the members are sorted alphabetically by name.
	QCheckBox* AddSec;
	QCheckBox* Article;
	MSpinBox* BleedBottom;
	MSpinBox* BleedLeft;
	MSpinBox* BleedRight;
	MSpinBox* BleedTop;
	QCheckBox* CheckBM;
	QCheckBox* CheckBox1;
	QComboBox* CMethod;
	QCheckBox* ClipMarg;
	QComboBox* ComboBind;
	QCheckBox* Compression;
	QRadioButton* continuousPages;
	QCheckBox* CopySec;
	QComboBox* CQuality;
	QRadioButton* doublePageLeft;
	QRadioButton* doublePageRight;
	QCheckBox* DSColor;
	QCheckBox* EmbedProfs;
	QCheckBox* EmbedProfs2;
	QCheckBox* Encry;
	QComboBox* ImageP;
	QComboBox* IntendI;
	QComboBox* IntendS;
	QToolButton* MirrorH;
	QToolButton* MirrorV;
	QCheckBox* ModifySec;
	QCheckBox* NoEmbedded;
	QComboBox* OutCombo;
	QListBox* Pages;
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
	// PDFExportDialog should really privately inherit from us, but it can't
	// since it needs to be a dialog not a tab widget. Allow it access to
	// protected and private members instead. If we ever clean up the direct
	// widget access in this class, it might be able to privately inheirit and
	// be added to a very thin dialog class instead.
	friend class PDFExportDialog;
	int PgSel;
	QValueList<PDFPresentationData> EffVal;
	QString SelLPIcolor;
	QValueList<QString> FontsToEmbed;
	QValueList<QString> FontsToSubset;

	// PDFExportDialog needs access to these GUI members
	// but they don't need to be exposed to the rest of Scribus.
	QComboBox* actionCombo;
	QRadioButton* AllPages;
	QCheckBox* CheckBox10;
	QComboBox* EDirection;
	QComboBox* EDirection_2;
	QComboBox* EDirection_2_2;
	QSpinBox* EffectTime;
	QComboBox* EffectType;
	QCheckBox* fitWindow;
	QCheckBox* hideMenuBar;
	QCheckBox* hideToolBar;
	QLineEdit* InfoString;
	QSpinBox* LPIangle;
	QSpinBox* LPIfreq;
	QComboBox* LPIfunc;
	QLineEdit* PageNr;
	QSpinBox* PageTime;

private:
	// Private GUI member pointers, should not be used outside the class its
	// self.  Please ry to keep members here genuinely private, ie don't use
	// them even from friends. If you need to access it from a friend like
	// PDFExportDialog please move it to the protected section.

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

	// Non-GUI protected members
	QString unit;
	int precision;
	double unitRatio;
	ScribusView * const view;
	const SCFonts & AllFonts;
	PDFOptions & Opts;
	double pageH;
	double pageB;
	bool cms;

};
#endif
