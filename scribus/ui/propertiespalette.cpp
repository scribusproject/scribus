/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/


#include "propertiespalette.h"

#include <QCloseEvent>
#include <QComboBox>
#include <QEvent>
#include <QFocusEvent>
#include <QKeyEvent>
#include <QObject>
#include <QPoint>
#include <QSpacerItem>
#include <QStackedWidget>
#include <QTimer>
#include <QToolBox>
#include <QToolTip>
#include <QTransform>
#include <QVBoxLayout>
#include <QValidator>
#include <QWidget>

#if defined(_MSC_VER) && !defined(_USE_MATH_DEFINES)
#define _USE_MATH_DEFINES
#endif
#include <cmath>

#include "appmodehelper.h"
#include "appmodes.h"
#include "colorpicker/colorpicker.h"
#include "insertTable.h"
#include "pageitem_table.h"
#include "propertiespalette_attributes.h"
#include "propertiespalette_line.h"
#include "propertiespalette_fill.h"
#include "propertiespalette_shadow.h"
#include "propertiespalette_shape.h"
#include "propertiespalette_xyz.h"
#include "scribus.h"
#include "scribusview.h"
#include "selection.h"
#include "undomanager.h"

PropertiesPalette::PropertiesPalette(QWidget *parent) : DockPanelBase("PropertiesPalette", "panel-frame-properties", parent)
{
	undoManager = UndoManager::instance();

	setObjectName(QString::fromLocal8Bit("PropertiesPalette"));
	setSizePolicy( QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));

	// XZY
	xyzPal = new PropertiesPalette_XYZ( this );
	scXYZ = new SectionContainer("X, Y, &Z", "SectionPPXYZ", true, true);
	scXYZ->setCanSaveState(true);
	scXYZ->setWidget(xyzPal);
	scXYZ->restorePreferences();

	// Drop Shadow
	shadowPal = new PropertiesPalette_Shadow( this );
	scShadow = new SectionContainer("Drop Shadow", "SectionPPDropShadow", true, false);
	scShadow->setCanSaveState(true);
	scShadow->setWidget(shadowPal);
	scShadow->restorePreferences();

	// Shape
	shapePal = new PropertiesPalette_Shape( this );
	scShape = new SectionContainer("&Shape", "SectionPPShape", true, false);
	scShape->setCanSaveState(true);
	scShape->setWidget(shapePal);
	scShape->restorePreferences();

	// Fill
	fillPal = new PropertiesPalette_Fill(this);
	scFill = new SectionContainer("&Fill", "SectionPPFill", true, false);
	scFill->setCanSaveState(true);
	scFill->setWidget(fillPal);
	scFill->restorePreferences();

	// Line
	linePal = new PropertiesPalette_Line(this);
	scLine = new SectionContainer("&Line", "SectionPPLine", true, false);
	scLine->setCanSaveState(true);
	scLine->setWidget(linePal);
	scLine->restorePreferences();

	// Attributes
	attributesPal = new PropertiesPalette_Attributes(this);
	scAttributes = new SectionContainer("&Attributes", "SectionPPAttributes", true, false);
	scAttributes->setCanSaveState(true);
	scAttributes->setWidget(attributesPal);
	scAttributes->restorePreferences();

	// Layout stack
	QVBoxLayout * lyt = new QVBoxLayout();
	lyt->setContentsMargins(0, 0, 0, 0);
	lyt->setSpacing(0);
	lyt->addWidget(scXYZ);
	lyt->addWidget(scShape);
	lyt->addWidget(scFill);
	lyt->addWidget(scLine);
	lyt->addWidget(scShadow);
	lyt->addWidget(scAttributes);
	lyt->addStretch(1);

	QWidget * wdg = new QWidget();
	wdg->setLayout(lyt);
	setWidget(wdg);

	languageChange();
	enablePalettes(false);



	ColorPicker *colorPicker = WidgetManager::instance().colorPicker();
	if(colorPicker)
	{
		connect(colorPicker, SIGNAL(meshToShape()), this, SLOT(meshGradientToShape()));
		connect(colorPicker, SIGNAL(createNewMesh()), this, SLOT(createNewMeshGradient()));
		connect(colorPicker, SIGNAL(resetMesh()), this, SLOT(resetMeshGradient()));
		connect(colorPicker, SIGNAL(reset1Control()), this, SLOT(resetOneControlPoint()));
		connect(colorPicker, SIGNAL(resetAllControl()), this, SLOT(resetAllControlPoints()));
		connect(colorPicker, SIGNAL(snapToMGrid(bool)), this, SLOT(snapToPatchGrid(bool)));
		connect(colorPicker, SIGNAL(removePatch()), this, SLOT(handleRemovePatch()));
	}

	m_haveItem = false;
}

void PropertiesPalette::closeEvent(QCloseEvent *closeEvent)
{
	if (m_ScMW && !m_ScMW->scriptIsRunning())
	{
		if (m_haveDoc && m_haveItem /*&& colorPalette->gradEditButton->isChecked()*/)
		{
			m_ScMW->view->requestMode(modeNormal);
			m_ScMW->view->RefreshGradient(m_item);
		}
	}
	DockPanelBase::closeEvent(closeEvent);
}

void PropertiesPalette::setMainWindow(ScribusMainWindow* mw)
{
	m_ScMW = mw;

	// For some reason, the mapToGlobal() / mapFromGlobal() roundtrip
	// performed below does not give always good results, causing PP to
	// not display in some situations. Moreover the reparenting is useless
	// as PP is already created with ScribusMainWindow as parent.
	/*QPoint p1 = mapToGlobal(pos());
	QPoint p2 = m_ScMW->mapFromGlobal(p1);
	setParent(m_ScMW);
	move(p2);*/

	this->xyzPal->setMainWindow(mw);
	this->shadowPal->setMainWindow(mw);
	this->shapePal->setMainWindow(mw);
	this->linePal->setMainWindow(mw);
	this->fillPal->setMainWindow(mw);
	this->attributesPal->setMainWindow(mw);

	connect(m_ScMW->appModeHelper, SIGNAL(AppModeChanged(int,int)), this, SLOT(AppModeChanged()));
}

void PropertiesPalette::setDoc(ScribusDoc *d)
{
	if((d == (ScribusDoc*) m_doc) || (m_ScMW && m_ScMW->scriptIsRunning()))
		return;

	if (m_doc)
	{
		disconnect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
		disconnect(m_doc, SIGNAL(docChanged()), this, SLOT(handleSelectionChanged()));
	}

	m_doc = d;
	m_item = nullptr;
	setEnabled(!m_doc->drawAsPreview);

	m_unitRatio = m_doc->unitRatio();
	m_unitIndex = m_doc->unitIndex();
	m_haveDoc = true;
	m_haveItem = false;

	xyzPal->setDoc(m_doc);
	shadowPal->setDoc(m_doc);
	shapePal->setDoc(m_doc);
	linePal->setDoc(m_doc);
	fillPal->setDoc(m_doc);
	attributesPal->setDoc(m_doc);

	connect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
	connect(m_doc, SIGNAL(docChanged()), this, SLOT(handleSelectionChanged()));

	// Handle properties update when switching document
	handleSelectionChanged();
}

void PropertiesPalette::unsetDoc()
{
	if (m_doc)
	{
		disconnect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
		disconnect(m_doc, SIGNAL(docChanged()), this, SLOT(handleSelectionChanged()));
	}
	setEnabled(true);
	m_haveDoc = false;
	m_haveItem = false;
	m_doc = nullptr;
	m_item = nullptr;

	xyzPal->unsetItem();
	xyzPal->unsetDoc();
	shadowPal->unsetItem();
	shadowPal->unsetDoc();
	shapePal->unsetItem();
	shapePal->unsetDoc();
	linePal->unsetItem();
	linePal->unsetDoc();
	fillPal->unsetItem();
	fillPal->unsetDoc();
	attributesPal->unsetItem();
	attributesPal->unsetDoc();

	m_haveItem = false;
	enablePalettes(false);
}

void PropertiesPalette::unsetItem()
{
	m_haveItem = false;
	m_item = nullptr;
	shapePal->unsetItem();
	linePal->unsetItem();
	fillPal->unsetItem();
	handleSelectionChanged();
}

void PropertiesPalette::setTextFlowMode(PageItem::TextFlowMode mode)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning() || !m_haveItem)
		return;
	shapePal->showTextFlowMode(mode);
}

PageItem* PropertiesPalette::currentItemFromSelection()
{
	if (m_doc && m_doc->m_Selection->count() > 0)
		return m_doc->m_Selection->itemAt(0);
	return nullptr;
}

void PropertiesPalette::enablePalettes(bool enable)
{
	scXYZ->setBodyEnabled(enable);
	scShape->setBodyEnabled(enable);
	scFill->setBodyEnabled(enable);
	scLine->setBodyEnabled(enable);
	scShadow->setBodyEnabled(enable);
	scAttributes->setBodyEnabled(enable);
}

void PropertiesPalette::AppModeChanged()
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	if ((m_haveDoc) && (m_haveItem))
	{
		// TODO: not sure that this is still needed...
		if (m_item->isTable())
		{
			if (m_doc->appMode == modeEditTable)
				connect(m_item->asTable(), SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
			else
				disconnect(m_item->asTable(), SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
		}
	}
}

void PropertiesPalette::setCurrentItem(PageItem *item)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	//CB We shouldn't really need to process this if our item is the same one
	//maybe we do if the item has been changed by scripter.. but that should probably
	//set some status if so.
	//FIXME: This won't work until when a canvas deselect happens, m_item must be nullptr.
	//if (m_item == item)
	//	return;

	if (!item)
	{
		unsetItem();
		return;
	}

	if (!m_doc)
		setDoc(item->doc());

	m_haveItem = false;
	m_item = item;

	setTextFlowMode(m_item->textFlowMode());

//CB replaces old emits from PageItem::emitAllToGUI()
	setLocked(item->locked());

	if ((m_item->isGroup()) && (!m_item->isSingleSel))
	{
		scXYZ->setBodyEnabled(true);
		scShadow->setBodyEnabled(true);
		scShape->setBodyEnabled(true);
		scLine->setBodyEnabled(true);
		scFill->setBodyEnabled(true);
		scAttributes->setBodyEnabled(true);
	}

	m_haveItem = true;

	if (!sender() || (m_doc->appMode == modeEditTable))
	{
		xyzPal->handleSelectionChanged();
		shadowPal->handleSelectionChanged();
		shapePal->handleSelectionChanged();
		linePal->handleSelectionChanged();
		fillPal->handleSelectionChanged();
		attributesPal->handleSelectionChanged();
	}
	if (m_item->isOSGFrame())
	{
		scXYZ->setBodyEnabled(true);
		scShadow->setBodyEnabled(true);
		scShape->setBodyEnabled(true);
		scLine->setBodyEnabled(false);
		scFill->setBodyEnabled(true);
		scAttributes->setBodyEnabled(true);
	}
	if (m_item->asSymbol())
	{
		scXYZ->setBodyEnabled(true);
		scShadow->setBodyEnabled(true);
		scShape->setBodyEnabled(false);
		scLine->setBodyEnabled(false);
		scFill->setBodyEnabled(false);
		scAttributes->setBodyEnabled(true);
	}
}

void PropertiesPalette::handleSelectionChanged()
{
	if (!m_haveDoc || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	PageItem* currItem = currentItemFromSelection();
	if (m_doc->m_Selection->count() > 1)
	{
		scXYZ->setBodyEnabled(true);
		scShape->setBodyEnabled(false);
		scShadow->setBodyEnabled(true);
		scLine->setBodyEnabled(true);
		scFill->setBodyEnabled(true);
		scAttributes->setBodyEnabled(true);
	}
	else
	{
		int itemType = currItem ? (int) currItem->itemType() : -1;
		m_haveItem   = (itemType != -1);

		switch (itemType)
		{
		case -1:
			m_haveItem = false;
			scXYZ->setBodyEnabled(false);
			scShape->setBodyEnabled(false);
			scLine->setBodyEnabled(false);
			scFill->setBodyEnabled(false);
			scShadow->setBodyEnabled(false);
			scAttributes->setBodyEnabled(false);
			break;
		case PageItem::ImageFrame:
		case PageItem::LatexFrame:
			scXYZ->setBodyEnabled(true);
			scShadow->setBodyEnabled(true);
			scShape->setBodyEnabled(true);
			scLine->setBodyEnabled(true);
			scFill->setBodyEnabled(true);
			scAttributes->setBodyEnabled(true);
			break;
		case PageItem::OSGFrame:
			scXYZ->setBodyEnabled(true);
			scShadow->setBodyEnabled(true);
			scShape->setBodyEnabled(true);
			scLine->setBodyEnabled(false);
			scFill->setBodyEnabled(true);
			scAttributes->setBodyEnabled(true);
			break;
		case PageItem::Line:
			scXYZ->setBodyEnabled(true);
			scShadow->setBodyEnabled(true);
			scShape->setBodyEnabled(false);
			scLine->setBodyEnabled(true);
			scFill->setBodyEnabled(false);
			scAttributes->setBodyEnabled(true);
			break;
		case PageItem::Group:
		case PageItem::Symbol:
		case PageItem::Table:
		case PageItem::TextFrame:
		case PageItem::ItemType1:
		case PageItem::ItemType3:
		case PageItem::Polygon:
		case PageItem::RegularPolygon:
		case PageItem::Arc:
		case PageItem::PolyLine:
		case PageItem::Spiral:
		case PageItem::PathText:
			scXYZ->setBodyEnabled(true);
			scShadow->setBodyEnabled(true);
			scShape->setBodyEnabled(true);
			scLine->setBodyEnabled(true);
			scFill->setBodyEnabled(true);
			scAttributes->setBodyEnabled(true);
			break;
		}
	}

	updateGeometry();
	update();

	if (currItem)
	{
		setCurrentItem(currItem);
	}

}

void PropertiesPalette::unitChange()
{
	if (!m_haveDoc)
		return;
	bool tmp = m_haveItem;
	m_haveItem = false;
	m_unitRatio = m_doc->unitRatio();
	m_unitIndex = m_doc->unitIndex();

	xyzPal->unitChange();
	shadowPal->unitChange();
	shapePal->unitChange();
	linePal->unitChange();
	fillPal->unitChange();

	m_haveItem = tmp;
}

void PropertiesPalette::handleNewShape(int frameType)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	if ((m_haveDoc) && (m_haveItem))
	{
		if ((m_item->itemType() == PageItem::PolyLine) || (m_item->itemType() == PageItem::PathText))
			return;
		shapePal->setRoundRectEnabled(frameType == 0);
	}
}

void PropertiesPalette::updateColorList()
{
	if (!m_haveDoc || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	assert (m_doc->PageColors.document());
}

bool PropertiesPalette::userActionOn()
{
	return xyzPal->userActionOn();
}

void PropertiesPalette::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
		return;
	}
	DockPanelBase::changeEvent(e);
}

void PropertiesPalette::languageChange()
{
	setWindowTitle( tr("Properties"));

	scXYZ->setText(tr("X, Y, &Z"));
	scShadow->setText(tr("&Drop Shadow"));
	scShape->setText(tr("&Shape"));
	scLine->setText(tr("&Line"));
	scFill->setText(tr("&Fill"));
	scAttributes->setText(tr("&Attributes"));

	xyzPal->languageChange();
	shadowPal->languageChange();
	shapePal->languageChange();
	linePal->languageChange();
	fillPal->languageChange();
	attributesPal->languageChange();
}

void PropertiesPalette::setGradientEditMode(bool on)
{
	WidgetManager::instance().colorPicker()->setGradientEditPropertiesEnabled(on);
}

void PropertiesPalette::setLocked(bool isLocked)
{
	shapePal->setLocked(isLocked);
}

void PropertiesPalette::handleShapeEdit()
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	if ((m_haveDoc) && (m_haveItem))
		shapePal->setRoundRectEnabled(false);
}

/* ********************************************************************************* *
 *
 * Mesh Gradient Methods
 *
 * ********************************************************************************* */

void PropertiesPalette::endPatchAdd()
{
	WidgetManager::instance().colorPicker()->endPatchAdd();
}

void PropertiesPalette::meshGradientToShape()
{
	if (!m_doc || !m_item)
		return;

	m_item->meshToShape();
	m_item->update();
	m_doc->regionsChanged()->update(QRect());
}

void PropertiesPalette::createNewMeshGradient()
{
	if (!m_doc || !m_item)
		return;

	InsertTable* dia = new InsertTable(this, 255, 255);
	dia->setWindowTitle( tr( "Create Mesh" ) );
	if (dia->exec())
	{
		m_item->createGradientMesh(dia->Rows->value(), dia->Cols->value());
		m_item->update();
		m_doc->regionsChanged()->update(QRect());
	}
	delete dia;
}

void PropertiesPalette::resetMeshGradient()
{
	if (!m_doc || !m_item)
		return;

	m_item->resetGradientMesh();
	m_item->update();
	m_doc->regionsChanged()->update(QRect());
}

void PropertiesPalette::resetOneControlPoint()
{
	if (!m_doc || !m_item)
		return;

	int grow = m_item->selectedMeshPointX;
	int gcol = m_item->selectedMeshPointY;
	int cont = m_item->selectedMeshControlPoint;
	MeshPoint tmp;
	if (m_item->gradientType() == Gradient_PatchMesh)
	{
		if ((grow == -1) || (gcol == 0))
			return;
		ScOldNewState<MeshPoint> *ss = nullptr;
		if (UndoManager::undoEnabled())
		{
			ss = new ScOldNewState<MeshPoint>(Um::GradPos);
			ss->set("MOVE_MESH_PATCH");
			ss->set("ARRAY", false);
			ss->set("X", grow);
			ss->set("Y", gcol);
		}
		switch (gcol)
		{
			case 1:
				tmp = m_item->meshGradientPatches[grow].TL;
				if (cont == 2)
					tmp.controlBottom = tmp.gridPoint;
				if (cont == 4)
					tmp.controlRight  = tmp.gridPoint;
				if (UndoManager::undoEnabled())
					ss->setStates(m_item->meshGradientPatches[grow].TL, tmp);
				m_item->meshGradientPatches[grow].TL = tmp;
				break;
			case 2:
				tmp = m_item->meshGradientPatches[grow].TR;
				if (cont == 2)
					tmp.controlBottom = tmp.gridPoint;
				if (cont == 3)
					tmp.controlLeft   = tmp.gridPoint;
				if (UndoManager::undoEnabled())
					ss->setStates(m_item->meshGradientPatches[grow].TR, tmp);
				m_item->meshGradientPatches[grow].TR = tmp;
				break;
			case 3:
				tmp = m_item->meshGradientPatches[grow].BR;
				if (cont == 1)
					tmp.controlTop  = tmp.gridPoint;
				if (cont == 3)
					tmp.controlLeft = tmp.gridPoint;
				if (UndoManager::undoEnabled())
					ss->setStates(m_item->meshGradientPatches[grow].BR, tmp);
				m_item->meshGradientPatches[grow].BR = tmp;
				break;
			case 4:
				tmp = m_item->meshGradientPatches[grow].BL;
				if (cont == 1)
					tmp.controlTop   = tmp.gridPoint;
				if (cont == 4)
					tmp.controlRight = tmp.gridPoint;
				if (UndoManager::undoEnabled())
					ss->setStates(m_item->meshGradientPatches[grow].BL, tmp);
				m_item->meshGradientPatches[grow].BL = tmp;
				break;
		}
		if (UndoManager::undoEnabled())
			undoManager->action(m_item,ss);
	}
	else // if (m_item->gradientType() == Gradient_Mesh)
	{
		if ((grow == -1) || (gcol == -1))
			return;

		tmp = m_item->meshGradientArray[grow][gcol];
		if (cont == 1)
			tmp.controlTop    = tmp.gridPoint;
		else if (cont == 2)
			tmp.controlBottom = tmp.gridPoint;
		else if (cont == 3)
			tmp.controlLeft   = tmp.gridPoint;
		else if (cont == 4)
			tmp.controlRight  = tmp.gridPoint;

		if (UndoManager::undoEnabled())
		{
			auto *ss = new ScOldNewState<MeshPoint>(Um::GradPos);
			ss->set("MOVE_MESH_PATCH");
			ss->set("ARRAY", true);
			ss->set("X", grow);
			ss->set("Y", gcol);
			ss->setStates(m_item->meshGradientArray[grow][gcol], tmp);
			undoManager->action(m_item,ss);
		}
		m_item->meshGradientArray[grow][gcol] = tmp;
	}
	m_item->update();
	m_doc->regionsChanged()->update(QRect());
}

void PropertiesPalette::resetAllControlPoints()
{
	if (!m_doc || !m_item)
		return;

	int grow = m_item->selectedMeshPointX;
	int gcol = m_item->selectedMeshPointY;
	MeshPoint tmp;
	if (m_item->gradientType() == Gradient_PatchMesh)
	{
		if ((grow == -1) || (gcol == 0))
			return;
		ScOldNewState<MeshPoint> *ss = nullptr;
		if (UndoManager::undoEnabled())
		{
			ss = new ScOldNewState<MeshPoint>(Um::GradPos);
			ss->set("MOVE_MESH_PATCH");
			ss->set("ARRAY", false);
			ss->set("X", grow);
			ss->set("Y", gcol);
		}
		switch (gcol)
		{
			case 1:
				tmp = m_item->meshGradientPatches[grow].TL;
				tmp.controlTop    = tmp.gridPoint;
				tmp.controlLeft   = tmp.gridPoint;
				tmp.controlRight  = tmp.gridPoint;
				tmp.controlBottom = tmp.gridPoint;
				if (UndoManager::undoEnabled())
					ss->setStates(m_item->meshGradientPatches[grow].TL, tmp);
				m_item->meshGradientPatches[grow].TL = tmp;
		break;
			case 2:
				tmp = m_item->meshGradientPatches[grow].TR;
				tmp.controlTop    = tmp.gridPoint;
				tmp.controlLeft   = tmp.gridPoint;
				tmp.controlRight  = tmp.gridPoint;
				tmp.controlBottom = tmp.gridPoint;
				if (UndoManager::undoEnabled())
					ss->setStates(m_item->meshGradientPatches[grow].TR, tmp);
				m_item->meshGradientPatches[grow].TR = tmp;
		break;
			case 3:
				tmp = m_item->meshGradientPatches[grow].BR;
				tmp.controlTop    = tmp.gridPoint;
				tmp.controlLeft   = tmp.gridPoint;
				tmp.controlRight  = tmp.gridPoint;
				tmp.controlBottom = tmp.gridPoint;
				if (UndoManager::undoEnabled())
					ss->setStates(m_item->meshGradientPatches[grow].BR, tmp);
				m_item->meshGradientPatches[grow].BR = tmp;
		break;
			case 4:
				tmp = m_item->meshGradientPatches[grow].BL;
				tmp.controlTop    = tmp.gridPoint;
				tmp.controlLeft   = tmp.gridPoint;
				tmp.controlRight  = tmp.gridPoint;
				tmp.controlBottom = tmp.gridPoint;
				if (UndoManager::undoEnabled())
					ss->setStates(m_item->meshGradientPatches[grow].BL, tmp);
				m_item->meshGradientPatches[grow].BL = tmp;
		break;
		}
		if (UndoManager::undoEnabled())
			undoManager->action(m_item,ss);
	}
	else //if (m_item->gradientType() == Gradient_Mesh)
	{
		if ((grow == -1) || (gcol == -1))
			return;
		tmp = m_item->meshGradientArray[grow][gcol];
		tmp.controlTop    = tmp.gridPoint;
		tmp.controlLeft   = tmp.gridPoint;
		tmp.controlRight  = tmp.gridPoint;
		tmp.controlBottom = tmp.gridPoint;
		if (UndoManager::undoEnabled())
		{
			auto *ss = new ScOldNewState<MeshPoint>(Um::GradPos);
			ss->set("MOVE_MESH_PATCH");
			ss->set("ARRAY", true);
			ss->set("X", grow);
			ss->set("Y", gcol);
			ss->setStates(m_item->meshGradientArray[grow][gcol], tmp);
			undoManager->action(m_item,ss);
		}
		m_item->meshGradientArray[grow][gcol] = tmp;
	}
	m_item->update();
	m_doc->regionsChanged()->update(QRect());
}

void PropertiesPalette::snapToPatchGrid(bool val)
{
	if (!m_item)
		return;

	m_item->setSnapToPatchGrid(val);
}

void PropertiesPalette::handleRemovePatch()
{
	if (!m_doc || !m_item)
		return;

	if ((m_item->selectedMeshPointX > -1) && (m_item->meshGradientPatches.count() > 1))
	{
		if (UndoManager::undoEnabled())
		{
			ScItemState<meshGradientPatch> *ss = new ScItemState<meshGradientPatch>(Um::RemoveMeshPatch,"",Um::ILine);
			ss->set("REMOVE_MESH_PATCH");
			ss->setItem(m_item->meshGradientPatches.takeAt(m_item->selectedMeshPointX));
			ss->set("POS", m_item->selectedMeshPointX);
			undoManager->action(m_item,ss);
		}
		m_item->selectedMeshPointX = -1;

		WidgetManager::instance().colorPicker()->enableButtonRemovePatch( (m_item->selectedMeshPointX > -1) && (m_item->meshGradientPatches.count() > 1) );

		m_item->update();
		m_doc->regionsChanged()->update(QRect());
	}
}
