#ifndef NODEPALETTE_H
#define NODEPALETTE_H

#include <qdialog.h>
#include <qbuttongroup.h>
#include <qpushbutton.h>
#include <qtoolbutton.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qcheckbox.h>
#include "mspinbox.h"
#include "scribusdoc.h"

class NodePalette : public QDialog
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
    QButtonGroup* ButtonGroup2;
    QToolButton* AsymMove;
    QToolButton* SymMove;
		QToolButton* ResNode;
		QToolButton* Res1Node;
    QButtonGroup* ButtonGroup3;
    QToolButton* PolySplit;
    QToolButton* BezierClose;
    QToolButton* PolyMirrorH;
    QToolButton* PolyMirrorV;
    QButtonGroup* ButtonGroup4;
    QToolButton* PolyShearL;
    QToolButton* PolyShearR;
    QToolButton* PolyShearU;
    QToolButton* PolyShearD;
		QCheckBox* AbsMode;
    QLabel* TextLabel1;
    MSpinBox* YSpin;
    QLabel* TextLabel2;
    MSpinBox* XSpin;
    QPushButton* PushButton1;
    void setDoc(ScribusDoc *dc);
    ScribusDoc *doc;

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
		void CloseBezier();
		void SplitPoly();
		void MirrorH();
		void MirrorV();
		void ShearR();
		void ShearL();
		void ShearU();
		void ShearD();
		void ToggleAbsMode();

public slots:
		void MoveN();
		void SetXY(double x, double y);
		void HaveNode(bool have, bool mov);
		void IsOpen();
		void PolyStatus(int typ, uint size);

protected:
    QVBoxLayout* NodePaletteLayout;
    QHBoxLayout* ButtonGroup1Layout;
    QHBoxLayout* ButtonGroup2Layout;
    QHBoxLayout* ButtonGroup3Layout;
    QHBoxLayout* ButtonGroup4Layout;
    QGridLayout* Layout2;

signals:
	void Schliessen();
};

#endif // NODEPALETTE_H
