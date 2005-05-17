#ifndef TABTOOLS_H
#define TABTOOLS_H

#include <qvariant.h>
#include <qwidget.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qtoolbutton.h>
#include <qbuttongroup.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qradiobutton.h>
#include <qwidgetstack.h>
#include "scfonts.h"
class LinkButton;
class QSpinBox;
class FontCombo;
class PolygonWidget;
class ArrowChooser;
class MSpinBox;
class LineCombo;
class ScribusDoc;

class TabTools : public QWidget
{
	Q_OBJECT

public:
	TabTools( QWidget* parent, struct toolPrefs *prefsData, int unitIndex, ScribusDoc* doc);
	~TabTools() {};
	void restoreDefaults();

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
	QLabel* textLabel3b2t;
	QLabel* textLabel1b;
	QLabel* textLabel2b;
	QLabel* textLabel4b;
	QSpinBox* columnsText;
	QLabel* previewText;
	FontCombo* fontComboText;
	QComboBox* sizeComboText;
	QComboBox* colorComboText;
	QComboBox* colorComboStrokeText;
	QComboBox* tabFillCombo;
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
	QCheckBox* embeddedPath;
	QButtonGroup* buttonGroupRes;
	QRadioButton* checkFullRes;
	QRadioButton* checkNormalRes;
	QRadioButton* checkHalfRes;
	QWidget* subTabPolygon;
	PolygonWidget* polyWidget;
	QWidget* subTabZoom;
	QLabel* textLabel21b;
	QSpinBox* minimumZoom;
	QLabel* textLabel22b;
	QLabel* textLabel23b;
	QSpinBox* maximumZoom;
	QSpinBox* zoomStep;
	ArrowChooser* startArrow;
	ArrowChooser* endArrow;
	QLabel* startArrowText;
	QLabel* endArrowText;
	QLabel* arrowText;
	SCFonts *fon;

protected slots:
	virtual void toggleChain();
	virtual void hChange();
	virtual void vChange();
	virtual void changeImageScalingFree(int);
	virtual void changeImageScalingRatio(int);
	virtual void setSample();
	virtual void setTool();

protected:
	QHBoxLayout* tabToolsLayout;
	QVBoxLayout* buttonGroupToolsLayout;
	QGridLayout* subTabTextLayout;
	QGridLayout* subTabShapeLayout;
	QGridLayout* subTabLineLayout;
	QGridLayout* subTabImageLayout;
	QGridLayout* buttonGroup3Layout;
	QHBoxLayout* buttonGroup5Layout;
	QVBoxLayout* buttonGroupResLayout;
	QHBoxLayout* subTabPolygonLayout;
	QGridLayout* subTabZoomLayout;
	
	double unitRatio;
	QString unit;
	int precision;
};
#endif
