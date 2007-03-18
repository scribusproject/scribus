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
#include <Q3HBoxLayout>
#include <Q3GridLayout>
#include <Q3VBoxLayout>

#include "scribusapi.h"
#include "bookmwin.h"
#include "scrspinbox.h"

class PDFOptions;
class PDFExportDialog;
class ScribusDoc;

class SCRIBUS_API TabPDFOptions : public QTabWidget
{
	Q_OBJECT

public:
	TabPDFOptions(  QWidget* parent, PDFOptions & Optionen,
	                const SCFonts &AllFonts,
	                const ProfilesL & PDFXProfiles,
	                const QMap<QString, int> & DocFonts,
	                const Q3ValueList<PDFPresentationData> & Eff,
	                int unitIndex, double PageH, double PageB,
	                ScribusDoc *mdoc = 0, bool exporting = false);
	~TabPDFOptions() {};
	void restoreDefaults(PDFOptions & Optionen,
						 const SCFonts &AllFonts,
						 const ProfilesL & PDFXProfiles,
						 const QMap<QString, int> & DocFonts,
						 const Q3ValueList<PDFPresentationData> & Eff,
						 int unitIndex, double PageH, double PageB,
						 ScribusDoc *mdoc, bool exporting);

	void unitChange(QString unit, int docUnitIndex, int decimals, double invUnitConversion);

	// GUI member pointers
	// Remember to initialize these in the initializer list of the ctor when
	// you add new ones. Add them in the same order here and in the initalizer
	// list. To help out, the members are sorted alphabetically by name.
	QCheckBox* AddSec;
	QCheckBox* Article;
	ScrSpinBox* BleedBottom;
	ScrSpinBox* BleedLeft;
	ScrSpinBox* BleedRight;
	ScrSpinBox* BleedTop;
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
	Q3ListBox* Pages;
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
	QCheckBox* overprintMode;
	QRadioButton* useThumbnails;
	QSpinBox* ValC;
	// End GUI member pointers

signals:
	void noInfo();
	void hasInfo();

public slots:
	void doDocBleeds();
	void ToggleEncr();
	void EnablePDFX(int a);
	void DoDownsample();
	void RemoveEmbed();
	void PutToEmbed();
	void RemoveOutline();
	void PutToOutline();
	void SelAFont(Q3ListBoxItem*);
	void SelEFont(Q3ListBoxItem*);
	void SelSFont(Q3ListBoxItem*);
	void EmbedAll();
	void OutlineAll();
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

protected slots:
	void createPageNumberRange();

protected:
	// PDFExportDialog should really privately inherit from us, but it can't
	// since it needs to be a dialog not a tab widget. Allow it access to
	// protected and private members instead. If we ever clean up the direct
	// widget access in this class, it might be able to privately inheirit and
	// be added to a very thin dialog class instead.
	friend class PDFExportDialog;
	int PgSel;
	Q3ValueList<PDFPresentationData> EffVal;
	QString SelLPIcolor;
	Q3ValueList<QString> FontsToEmbed;
	Q3ValueList<QString> FontsToOutline;

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
	Q3ListBox* AvailFlist;
	Q3GroupBox* BleedGroup;
	Q3GridLayout* BleedGroupLayout;
	Q3GroupBox* MarkGroup;
	Q3GridLayout* MarkGroupLayout;
	QCheckBox* cropMarks;
	QCheckBox* bleedMarks;
	QCheckBox* registrationMarks;
	QCheckBox* colorMarks;
	QCheckBox* docInfoMarks;
	ScrSpinBox* markOffset;
	Q3GroupBox* CBox;
	Q3GridLayout* CBoxLayout;
	Q3GroupBox* ColorGroup;
	Q3HBoxLayout* ColorGroupLayout;
	QLabel* ColorText1;
	Q3GroupBox* Effects;
	Q3GridLayout* EffectsLayout;
	QPushButton* EmbedFonts;
	Q3ListBox* EmbedList;
	QPushButton* EonAllPg;
	QPushButton* FromEmbed;
	QPushButton* FromOutline;
	Q3GroupBox* GroupBox1;
	Q3GridLayout* GroupBox1Layout;
	Q3GridLayout* GroupBox2Layout;
	Q3GroupBox* GroupBox9;
	Q3GridLayout* GroupBox9Layout;
	Q3GroupBox* groupDisplay;
	Q3VBoxLayout* groupDisplayLayout;
	Q3GroupBox* GroupFont;
	Q3HBoxLayout* GroupFontLayout;
	Q3GroupBox* groupJava;
	Q3HBoxLayout* groupJavaLayout;
	Q3ButtonGroup* groupNavigation;
	Q3VBoxLayout* groupNavigationLayout;
	Q3GroupBox* GroupPass;
	Q3GridLayout* GroupPassLayout;
	Q3GroupBox* GroupSecSet;
	Q3VBoxLayout* GroupSecSetLayout;
	Q3GridLayout* Layout11;
	Q3HBoxLayout *pageNumberSelectorLayout;
	Q3GridLayout* Layout11a;
	Q3HBoxLayout* Layout13;
	Q3HBoxLayout* Layout2;
	Q3VBoxLayout* Layout3;
	Q3VBoxLayout* Layout4_2;
	Q3VBoxLayout* Layout5_2;
	Q3HBoxLayout* Layout5_2a;
	Q3VBoxLayout* Layout6;
	Q3HBoxLayout* LayoutSpecial;
	QComboBox* LPIcolor;
	Q3GroupBox* LPIgroup;
	Q3GridLayout* LPIgroupLayout;
	QRadioButton* OnlySome;
	Q3ButtonGroup* pageLayout;
	Q3VBoxLayout* pageLayoutLayout;
	QCheckBox* PagePrev;
	QLabel* PDFX1;
	QLabel* PDFX2;
	QCheckBox* PrintSec2;
	Q3GroupBox* ProfsGroup;
	Q3GridLayout* ProfsGroupLayout;
	QLabel* ProfsTxt1;
	QLabel* ProfsTxt2;
	QLabel* ProfsTxt3;
	QLabel* ProfsTxt4;
	QLabel* BleedTxt1;
	QLabel* BleedTxt2;
	QLabel* BleedTxt3;
	QLabel* BleedTxt4;
	QLabel* MarkTxt1;
	QCheckBox *docBleeds;
	Q3ButtonGroup* RangeGroup;
	Q3VBoxLayout* RangeGroupLayout;
	Q3HBoxLayout* RotationLayout;
	QPushButton* OutlineFonts;
	Q3ListBox* OutlineList;
	QWidget* tabColor;
	Q3VBoxLayout* tabColorLayout;
	QWidget* tabFonts;
	QWidget* tabGeneral;
	Q3VBoxLayout* tabLayout;
	Q3VBoxLayout* tabLayout_3;
	Q3GridLayout* tabLayout_5;
	Q3GridLayout* tabOptionsGridLayout;
	QWidget* tabPDFX;
	Q3VBoxLayout* tabPDFXLayout;
	QWidget* tabPresentation;
	QWidget* tabSecurity;
	Q3VBoxLayout* tabSecurityLayout;
	QWidget* tabSpecial;
	Q3VBoxLayout* tabSpecialLayout;
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
	QPushButton* pageNrButton;
	QPushButton* ToEmbed;
	QPushButton* ToOutline;
	QRadioButton* useViewDefault;
	Q3GroupBox* X3Group;
	Q3GridLayout* X3GroupLayout;
	// end protected GUI member pointers

	// Non-GUI protected members
	QString unit;
	int precision;
	double unitRatio;
	bool pdfExport;
	ScribusDoc* const doc;
	const SCFonts & AllFonts;
	PDFOptions & Opts;
	double pageH;
	double pageB;
	bool cms;
	QMap<QString, QString> AnnotationFonts;

};
#endif
