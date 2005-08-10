#ifndef NODEPALETTE_H
#define NODEPALETTE_H

#include <qbuttongroup.h>
#include <qpushbutton.h>
#include <qtoolbutton.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qcheckbox.h>
#include <qspinbox.h>

#include "scribusapi.h"
#include "scrpalettebase.h"
#include "mspinbox.h"

class ScribusDoc;
class ScribusView;

class SCRIBUS_API NodePalette : public ScrPaletteBase
{
	Q_OBJECT

public:
	NodePalette( QWidget* parent);
	~NodePalette() {};

	QButtonGroup* ButtonGroup1;
	QToolButton* MoveNode;
	QToolButton* MoveControl;
	QToolButton* AddNode;
	QToolButton* DeleteNode;
	QToolButton* AsymMove;
	QToolButton* SymMove;
	QToolButton* ResNode;
	QToolButton* Res1Node;
	QToolButton* PolySplit;
	QToolButton* BezierClose;
	QToolButton* PolyMirrorH;
	QToolButton* PolyMirrorV;
	QToolButton* PolyShearL;
	QToolButton* PolyShearR;
	QToolButton* PolyShearU;
	QToolButton* PolyShearD;
	QToolButton* RotateCCW;
	QToolButton* RotateCW;
	QToolButton* Expand;
	QToolButton* Crop;
	QCheckBox* AbsMode;
	QCheckBox* EditCont;
	QLabel* TextLabel1;
	MSpinBox* YSpin;
	QLabel* TextLabel2;
	MSpinBox* XSpin;
	QSpinBox *RotVal;
	QSpinBox *ScaleVal;
	QPushButton* ResetCont;
	QPushButton* PushButton1;
	void setDoc(ScribusDoc *dc, ScribusView *vi);
	ScribusDoc *doc;
	ScribusView *view;

private slots:
	void closeEvent(QCloseEvent *);
	void EndEdit();
	void MoveK();
	void AddN();
	void DelN();
	void MovePoint();
	void SetSym();
	void SetAsym();
	void ResetControl();
	void Reset1Control();
	void ResetContour();
	void CloseBezier();
	void SplitPoly();
	void MirrorH();
	void MirrorV();
	void doRotCCW();
	void doRotCW();
	void doCrop();
	void doExpand();
	void ShearR();
	void ShearL();
	void ShearU();
	void ShearD();
	void ToggleAbsMode();
	void ToggleConMode();

public slots:
	void MoveN();
	void SetXY(double x, double y);
	void HaveNode(bool have, bool mov);
	void IsOpen();
	void PolyStatus(int typ, uint size);
	void languageChange();

protected:
	QVBoxLayout* NodePaletteLayout;
	QGridLayout* ButtonGroup1Layout;
	QGridLayout* Layout2;
	
signals:
	void Schliessen();
};

#endif // NODEPALETTE_H
