/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef NODEPALETTE_H
#define NODEPALETTE_H

#include <QToolTip>
#include <QCheckBox>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QToolButton>
#include <QGroupBox>
#include <QRadioButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCloseEvent>

class QEvent;

#include "scribusapi.h"
#include "fpointarray.h"
#include "scrpalettebase.h"
#include "scrspinbox.h"

class ScribusDoc;
class ScribusView;
class PageItem;

class SCRIBUS_API NodePalette : public ScrPaletteBase
{
	Q_OBJECT

public:
	NodePalette( QWidget* parent);
	~NodePalette() {};

	void setDefaults(PageItem* currItem);

	QToolButton* MoveNode { nullptr };
	QToolButton* MoveControl { nullptr };
	QToolButton* AddNode { nullptr };
	QToolButton* DeleteNode { nullptr };
	QToolButton* AsymMove { nullptr };
	QToolButton* SymMove { nullptr };
	QToolButton* ResNode { nullptr };
	QToolButton* Res1Node { nullptr };
	QToolButton* PolySplit { nullptr };
	QToolButton* BezierClose { nullptr };
	QToolButton* PolyMirrorH { nullptr };
	QToolButton* PolyMirrorV { nullptr };
	QToolButton* PolyShearL { nullptr };
	QToolButton* PolyShearR { nullptr };
	QToolButton* PolyShearU { nullptr };
	QToolButton* PolyShearD { nullptr };
	QToolButton* RotateCCW { nullptr };
	QToolButton* RotateCW { nullptr };
	QToolButton* Expand { nullptr };
	QToolButton* Shrink { nullptr };
	QToolButton* Enlarge { nullptr };
	QToolButton* Reduce { nullptr };
	QCheckBox* PreviewMode { nullptr };
	QGroupBox* AbsMode { nullptr };
	QRadioButton* absToCanvas { nullptr };
	QRadioButton* absToPage { nullptr };
	QCheckBox* EditCont { nullptr };
	QLabel* TextLabel1 { nullptr };
	ScrSpinBox* YSpin { nullptr };
	QLabel* TextLabel2 { nullptr };
	ScrSpinBox* XSpin { nullptr };
	QSpinBox *RotVal { nullptr };
	QSpinBox *scalePercentage { nullptr };
	ScrSpinBox *scaleDistance { nullptr };
	QPushButton* ResetCont { nullptr };
	QPushButton* ResetContClip { nullptr };
	QPushButton* ResetShape2Clip { nullptr };
	QPushButton* editEditButton { nullptr };
	QPushButton* cancelEditButton { nullptr };
	QPushButton* resetDefaultButton { nullptr };

	void setDoc(ScribusDoc *dc, ScribusView *vi);
	ScribusDoc* currentDocument() const;

private slots:
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
	void ResetShapeToImageClip();
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
	void TogglePreview();

public slots:
	void MoveN();
	void SetXY(double x, double y);
	void HaveNode(bool have, bool mov);
	void IsOpen();
	void PolyStatus(int typ, uint size);
	void iconSetChange();
	void languageChange();
	void unitChange();
	void EndEdit(); // allow remote close
	void CancelEdit();
	void ResetToEditDefaults();

protected slots:
	void reject() override;

protected:
	void connectSignals();
	void disconnectSignals();

	void changeEvent(QEvent *e) override;
	void closeEvent(QCloseEvent *) override;

	QVBoxLayout *vboxLayout { nullptr };
	QVBoxLayout *vboxLayout1 { nullptr };
	QHBoxLayout *hboxLayout { nullptr };
	QGridLayout *gridLayout { nullptr };
	QGridLayout *gridLayout2 { nullptr };
	QGridLayout *gridLayout3 { nullptr };

	ScribusDoc *m_doc { nullptr };
	ScribusView *m_view { nullptr };
	
	double m_unitRatio { 1.0 };
	double m_xPos { 0.0 };
	double m_yPos { 0.0 };
	FPointArray m_itemPath;
	FPointArray m_itemContourPath;
	
signals:
	void DocChanged();
	void paletteClosed();
};

#endif // NODEPALETTE_H
