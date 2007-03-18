/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef NODEPALETTE_H
#define NODEPALETTE_H

#include <q3buttongroup.h>
#include <qpushbutton.h>
#include <qtoolbutton.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qcheckbox.h>
#include <qspinbox.h>
//Added by qt3to4:
#include <Q3GridLayout>
#include <Q3VBoxLayout>
#include <QCloseEvent>

#include "scribusapi.h"
#include "scrpalettebase.h"
#include "scrspinbox.h"

class ScribusDoc;
class ScribusView;

class SCRIBUS_API NodePalette : public ScrPaletteBase
{
	Q_OBJECT

public:
	NodePalette( QWidget* parent);
	~NodePalette() {};

	Q3ButtonGroup* ButtonGroup1;
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
	QToolButton* Shrink;
	QToolButton* Enlarge;
	QToolButton* Reduce;
	QCheckBox* AbsMode;
	QCheckBox* EditCont;
	QLabel* TextLabel1;
	ScrSpinBox* YSpin;
	QLabel* TextLabel2;
	ScrSpinBox* XSpin;
	QSpinBox *RotVal;
	QSpinBox *scalePercentage;
	ScrSpinBox *scaleDistance;
	QPushButton* ResetCont;
	QPushButton* ResetContClip;
	QPushButton* editEditButton;
	void setDoc(ScribusDoc *dc, ScribusView *vi);
	ScribusDoc* currentDocument() const;
	ScribusDoc *doc;
	ScribusView *view;

private slots:
	void closeEvent(QCloseEvent *);
	void MoveK();
	void AddN();
	void DelN();
	void MovePoint();
	void SetSym();
	void SetAsym();
	void ResetControl();
	void Reset1Control();
	void ResetContour();
	void ResetContourToImageClip();
	void CloseBezier();
	void SplitPoly();
	void MirrorH();
	void MirrorV();
	void doRotCCW();
	void doRotCW();
	void doShrink();
	void doExpand();
	void doReduce();
	void doEnlarge();
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
	void unitChange();
	void EndEdit(); // allow remote close

protected:
	void connectSignals();
	void disconnectSignals();
	Q3VBoxLayout* NodePaletteLayout;
	Q3GridLayout* ButtonGroup1Layout;
	Q3GridLayout* Layout2;
	
	double unitRatio;
	
signals:
	void Schliessen();
	void DocChanged();
};

#endif // NODEPALETTE_H
