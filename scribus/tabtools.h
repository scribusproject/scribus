#ifndef TABTOOLS_H
#define TABTOOLS_H

#include <qvariant.h>
#include <qwidget.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qtoolbutton.h>
#include <q3buttongroup.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qradiobutton.h>
#include <q3widgetstack.h>
//Added by qt3to4:
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

#include "scribusapi.h"
class LinkButton;
class QSpinBox;
class FontCombo;
class PolygonWidget;
class ArrowChooser;
class MSpinBox;
class LineCombo;
class ScribusDoc;

class SCRIBUS_API TabTools : public QWidget
{
	Q_OBJECT

public:
	TabTools( QWidget* parent, struct toolPrefs *prefsData, int unitIndex, ScribusDoc* doc);
	~TabTools() {};
	void restoreDefaults();

	Q3ButtonGroup* buttonGroupTools;
	QToolButton* toolShape;
	QToolButton* toolPoly;
	QToolButton* toolImage;
	QToolButton* toolText;
	QToolButton* toolLine;
	QToolButton* toolZoom;
	Q3WidgetStack* subStackTools;
	QWidget* subTabText;
	QLabel* textLabel3b;
	QLabel* textLabel3b2;
	QLabel* textLabel3b2t;
	QLabel* textLabel3b2t2;
	QLabel* textLabel3bT;
	QLabel* textLabel3bTL;
	QLabel* textLabel1b;
	QLabel* textLabel2b;
	QLabel* textLabel4b;
	QSpinBox* columnsText;
	QLabel* previewText;
	FontCombo* fontComboText;
	QComboBox* sizeComboText;
	QComboBox* colorComboText;
	QComboBox* colorComboStrokeText;
	QComboBox* colorComboTextBackground;
	QComboBox* colorComboTextLine;
	QSpinBox* shadingText;
	QSpinBox* shadingTextStroke;
	QSpinBox* shadingTextBack;
	QSpinBox* shadingTextLine;
	QLabel* textLabelTS;
	QLabel* textLabelTSS;
	QLabel* textLabelTSB;
	QLabel* textLabelTSL;
	QComboBox* tabFillCombo;
	QLabel* textLabel5b;
	MSpinBox* gapText;
	MSpinBox* gapTab;
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
	Q3ButtonGroup* buttonGroup3;
	QLabel* textLabel17b;
	QLabel* textLabel18b;
	QSpinBox* scalingHorizontal;
	QSpinBox* scalingVertical;
	LinkButton* chainButton;
	Q3ButtonGroup* buttonGroup5;
	QCheckBox* checkRatioImage;
	QCheckBox* embeddedPath;
	Q3ButtonGroup* buttonGroupRes;
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

protected slots:
	virtual void toggleChain();
	virtual void hChange();
	virtual void vChange();
	virtual void changeImageScalingFree(int);
	virtual void changeImageScalingRatio(int);
	virtual void setSample();
	virtual void setTool();
	virtual void setFillChar(int act);
	virtual void setCustomFillChar(const QString &txt);

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
