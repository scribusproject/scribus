#ifndef REFORMDOC_H
#define REFORMDOC_H

#include <qlayout.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qgroupbox.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qtoolbutton.h>
#include <qcolor.h>

#include "scribusstructs.h"
#include "linkbutton.h"
#include "prefsdialogbase.h"
#include "scfonts.h"

class QSpinBox;
class ScribusDoc;
class ScribusApp;
class FontCombo;
class PolygonWidget;
class ArrowChooser;
class TabTypograpy;
class HySettings;
class CMSPrefs;
class MSpinBox;
class LineCombo;


class ReformDoc : public PrefsDialogBase
{
	Q_OBJECT

public:
	ReformDoc( QWidget* parent, ScribusDoc* doc, preV *prefsData );
	~ReformDoc() {};
	QWidget* tabPage;
	QGroupBox* groupBox7;
	QGroupBox* groupBox7a;
	QGroupBox* dsGroupBox7;
	QGroupBox* groupScratch;
	QLabel* TextLabel5;
	QLabel* TextLabel7;
	QLabel* TextLabel5s;
	QLabel* TextLabel7s;
	QLabel* TextLabel1_3;
	QSpinBox* pageNumber;
	MSpinBox* topR;
	MSpinBox* bottomR;
	MSpinBox* leftR;
	MSpinBox* rightR;
	MSpinBox* widthMSpinBox;
	MSpinBox* heightMSpinBox;
	MSpinBox* topScratch;
	MSpinBox* bottomScratch;
	MSpinBox* leftScratch;
	MSpinBox* rightScratch;
	QCheckBox* facingPages;
	QCheckBox* firstPage;
	QLabel* Links;
	QLabel* Rechts;
	QLabel* Linkss;
	QLabel* Rechtss;
	QLabel*	widthQLabel;
	QLabel*	heightQLabel;
	QLabel*	unitQLabel;
	QLabel*	sizeQLabel;
	QLabel*	orientationQLabel;
	QComboBox*	sizeQComboBox;
	QComboBox*	orientationQComboBox;

	QWidget* tabGuides;
	QWidget* tabView;
	TabTypograpy* tabTypo;
	QWidget* tabTools;
	HySettings* tabHyphenator;
	CMSPrefs* tabColorManagement;
	QLabel* textLabel1;
	QLabel* textLabel2;
	QLabel* textLabel3;
	QLabel* textLabel4;
	QLabel* textLabel5;
	QLabel* textLabel6;
	QLabel* textLabel7;
	QLabel* textLabel8;
	QLabel* textLabel82;
	QLabel* textLabel9;
	QGroupBox* checkGrid;
	QGroupBox* groupBox1;
	QGroupBox* groupBox2;
	QPushButton* majorGridColor;
	QPushButton* minorGridColor;
	QPushButton* baselineColor;
	QPushButton* guideColor;
	QPushButton* marginColor;
	QPushButton* backColor;
	MSpinBox* minorSpace;
	MSpinBox* majorSpace;
	MSpinBox* snapDistance;
	QSpinBox* grabDistance;
	QButtonGroup* buttonGroup1;
	QButtonGroup* pageBackground;
	QRadioButton* inBackground;
	QRadioButton* inForeground;
	QCheckBox* checkUnprintable;
	QCheckBox* checkMargin;
	QCheckBox* checkFrame;
	QCheckBox* checkPictures;
	QCheckBox* checkGuides;
	QCheckBox* checkBaseline;
	QCheckBox* checkLink;

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
	QGroupBox* groupAutoSave;
	QLabel* textLabel1m;
	QSpinBox* autoSaveTime;
	QComboBox* unitCombo;
	ArrowChooser* startArrow;
	ArrowChooser* endArrow;
	QLabel* startArrowText;
	QLabel* endArrowText;
	QLabel* arrowText;

	ScribusApp *ap;
	ScribusDoc* docc;
	SCFonts *fon;
	int einheit;
	int decimals;
	double Umrech;
	double pageWidth;
	double pageHeight;
	QColor colorMajorGrid;
	QColor colorMinorGrid;
	QColor colorBaselineGrid;
	QColor colorPaper;
	QColor colorGuides;
	QColor colorMargin;

protected:
	QVBoxLayout* reformDocLayout;
	QHBoxLayout* dsGroupBox7Layout;
	QHBoxLayout* groupBox7Layout;
	QHBoxLayout* groupBox7aLayout;
	QHBoxLayout* groupScratchLayout;
	QGridLayout* dsLayout4;
	QGridLayout* layout4;
	QGridLayout* layout4a;
	QGridLayout* layout4s;
	QHBoxLayout* Layout3;
	QVBoxLayout* tabGuidesLayout;
	QVBoxLayout* tabViewLayout;
	QGridLayout* checkGridLayout;
	QGridLayout* groupBox1Layout;
	QGridLayout* groupBox2Layout;
	QVBoxLayout* buttonGroup1Layout;
	QHBoxLayout* layout7;
	QGridLayout* layout11;
	QHBoxLayout* layout2;
	QHBoxLayout* layout3;
	QHBoxLayout* layout9;
	QVBoxLayout* pageBackgroundLayout;
	QHBoxLayout* layout10;
	QSpacerItem* spacer3;
	QGridLayout* layout12;
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
	QHBoxLayout* groupAutoSaveLayout;

protected slots:
	virtual void unitChange();
	virtual void toggleChain();
	virtual void hChange();
	virtual void vChange();
	virtual void changeImageScalingFree(int);
	virtual void changeImageScalingRatio(int);
	virtual void setSample();
	virtual void setTool();
	virtual void changeMajorColor();
	virtual void changeMinorColor();
	virtual void changePaperColor();
	virtual void changeBaselineColor();
	virtual void changeGuideColor();
	virtual void changeMarginColor();
	virtual void setDS();
	virtual void setTop(int v);
	virtual void setBottom(int v);
	virtual void setLeft(int v);
	virtual void setRight(int v);
};

#endif // REFORMDOC_H
