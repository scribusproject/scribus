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
#include "ui_nodeeditpalette.h"

class ScribusDoc;
class ScribusView;
class PageItem;

class SCRIBUS_API NodePalette : public ScrPaletteBase, public Ui::nodePalette
{
	Q_OBJECT

public:
	NodePalette( QWidget* parent);
	~NodePalette() {};

	void setDefaults(PageItem* currItem);
	void setDoc(ScribusDoc *dc, ScribusView *vi);
	ScribusDoc* currentDocument() const;

	bool isSymetricMove() { return SymMove->isChecked(); };

private slots:
	void AddN();
	void DelN();
	void MovePoint();
	void SetSym();
	void SetAsym();
	void SetIndependentMove();
	void editControl();
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
	void ToggleContourMode();
	void TogglePreview();
	void changePosOrigin(int index);

public slots:
	void MoveN();
	void SetXY(double x, double y);
	void HaveNode(bool have);
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

	enum PositionOrigin {
		Canvas = 0,
		Page,
		Item
	};

	void connectSignals();
	void disconnectSignals();

	void initPosOriginMenu();

	void changeEvent(QEvent *e) override;
	void closeEvent(QCloseEvent *) override;

	ScribusDoc *m_doc { nullptr };
	ScribusView *m_view { nullptr };
	
	double m_unitRatio { 1.0 };
	double m_xPos { 0.0 };
	double m_yPos { 0.0 };
	FPointArray m_itemPath;
	FPointArray m_itemContourPath;
	int m_posOrigin { Page };
	
signals:
	void DocChanged();
	void paletteClosed();
};

#endif // NODEPALETTE_H
