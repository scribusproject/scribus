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
#include "linkbutton.h"
#include "linecombo.h"
#include "mspinbox.h"
#include "prefsdialogbase.h"
#include "scfonts.h"
#include "scribusdoc.h"
class FontCombo;
class PolygonWidget;

class ReformDoc : public PrefsDialogBase
{
	Q_OBJECT

public:
	ReformDoc( QWidget* parent, ScribusDoc* doc, preV *prefsData );
	~ReformDoc() {};
	QWidget* tabPage;
	QGroupBox* GroupBox7;
	QGroupBox* dsGroupBox7;
	QLabel* TextLabel5;
	QLabel* TextLabel7;
	QLabel* TextLabel1_3;
	QSpinBox* pageNumber;
	MSpinBox* topR;
	MSpinBox* bottomR;
	MSpinBox* leftR;
	MSpinBox* rightR;
	MSpinBox* widthMSpinBox;
	MSpinBox* heightMSpinBox;
	QCheckBox* facingPages;
	QCheckBox* firstPage;
	QLabel* Links;
	QLabel* Rechts;
	QLabel*	widthQLabel;
	QLabel*	heightQLabel;
	QLabel*	sizeQLabel;
	QLabel*	orientationQLabel;
	QComboBox*	sizeQComboBox;
	QComboBox*	orientationQComboBox;

	QWidget* tabGuides;
	QWidget* tabView;
	QWidget* tabTypo;
	QWidget* tabTools;
	QLabel* textLabel1;
	QLabel* textLabel2;
	QLabel* textLabel3;
	QLabel* textLabel4;
	QLabel* textLabel5;
	QLabel* textLabel6;
	QLabel* textLabel7;
	QLabel* textLabel8;
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
	QLabel* textLabel1b;
	QLabel* textLabel2b;
	QLabel* textLabel4b;
	QSpinBox* columnsText;
	QLabel* previewText;
	FontCombo* fontComboText;
	QComboBox* sizeComboText;
	QComboBox* colorComboText;
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

	SCFonts *fon;
	int einheit;
	double pageWidth;
	double pageHeight;
	QColor colorMajorGrid;
	QColor colorMinorGrid;
	QColor colorBaselineGrid;
	QColor colorPaper;
	QColor colorGuides;
	QColor colorMargin;

protected:
	QVBoxLayout* ReformDocLayout;
	QHBoxLayout* dsGroupBox7Layout;
	QHBoxLayout* GroupBox7Layout;
	QGridLayout* dsLayout4;
	QGridLayout* Layout4;
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
	QGridLayout* tabTypoLayout;
	QGridLayout* groupBox1aLayout;
	QGridLayout* groupBox2aLayout;
	QGridLayout* groupBox3aLayout;
	QGridLayout* groupBox4aLayout;
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

protected slots:
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
