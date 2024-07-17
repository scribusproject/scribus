#include "propertiespalette_fill.h"

#include "iconmanager.h"
#include "scribus.h"
#include "scribusapp.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "selection.h"
#include "undomanager.h"
#include "ui/colorpicker/colorpicker.h"

PropertiesPalette_Fill::PropertiesPalette_Fill(QWidget *parent) :
	QWidget(parent)
{
	setupUi(this);

	setSizePolicy( QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum));

	buttonFillColor->setContext(Context::Fill);
	buttonFillColor->setMenuContextType(ColorButton::Floating);
	buttonFillMask->setContext(Context::FillMask);
	buttonFillMask->setMenuContextType(ColorButton::Floating);

	undoManager = UndoManager::instance();

	iconSetChange();
	languageChange();

	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));
	connect(ScQApp, SIGNAL(labelVisibilityChanged(bool)), this, SLOT(toggleLabelVisibility(bool)));

	connect(evenOdd, SIGNAL(clicked()), this, SLOT(handleFillRule()));
	connect(nonZero, SIGNAL(clicked()), this, SLOT(handleFillRule()));
	connect(buttonFillColor, SIGNAL(changed()), this, SLOT(handleFillColor()));
	connect(buttonFillColor, SIGNAL(gradientVectorChanged()), this, SLOT(handleFillColorVector()));
	connect(buttonFillColor, SIGNAL(gradientMeshChanged()), this, SLOT(handleFillColorMesh()));
	connect(buttonFillMask, SIGNAL(changed()), this, SLOT(handleFillMask()));
	connect(buttonFillMask, SIGNAL(gradientVectorChanged()), this, SLOT(handleFillMaskVector()));
	connect(comboBlendmode, SIGNAL(currentIndexChanged(int)), this, SLOT(handleFillBlendmode(int)));

}

void PropertiesPalette_Fill::updateColorList()
{
	if (!m_haveDoc || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
}

void PropertiesPalette_Fill::setMainWindow(ScribusMainWindow* mw)
{
	m_ScMW = mw;

	connect(m_ScMW, SIGNAL(UpdateRequest(int)), this  , SLOT(handleUpdateRequest(int)));
}

void PropertiesPalette_Fill::setDoc(ScribusDoc *d)
{
	if ((d == (ScribusDoc*) m_doc) || (m_ScMW && m_ScMW->scriptIsRunning()))
		return;

	if (m_doc)
	{
		disconnect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
		disconnect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
	}

	m_doc  = d;
	m_item = nullptr;

	m_haveDoc  = true;
	m_haveItem = false;
	m_unitRatio   = m_doc->unitRatio();

	buttonFillColor->setDoc(m_doc);
	buttonFillMask->setDoc(m_doc);

	connect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
	connect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
}

void PropertiesPalette_Fill::unsetDoc()
{
	if (m_doc)
	{
		disconnect(m_doc->m_Selection, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()));
		disconnect(m_doc             , SIGNAL(docChanged())      , this, SLOT(handleSelectionChanged()));
	}

	m_haveDoc  = false;
	m_haveItem = false;
	m_doc   = nullptr;
	m_item  = nullptr;

	buttonFillColor->unsetDoc();
	buttonFillMask->unsetDoc();

	setEnabled(false);
}

void PropertiesPalette_Fill::unsetItem()
{
	m_haveItem = false;
	m_item     = nullptr;
	handleSelectionChanged();
}

PageItem* PropertiesPalette_Fill::currentItemFromSelection()
{
	PageItem *currentItem = nullptr;

	if (m_doc && m_doc->m_Selection->count() > 0)
		currentItem = m_doc->m_Selection->itemAt(0);

	return currentItem;
}

void PropertiesPalette_Fill::setCurrentItem_Fill(PageItem *item)
{
	buttonFillColor->setType(item->gradientType());

	// Gradient
	buttonFillColor->setGradient(
				item->gradient(),
				item->fill_gradient,
				item->getGradientExtend(),
				item->gradientCol1(),
				item->gradientCol2(),
				item->gradientCol3(),
				item->gradientCol4(),
				item->gradientShade1(),
				item->gradientShade2(),
				item->gradientShade3(),
				item->gradientShade4(),
				item->gradientTransp1(),
				item->gradientTransp2(),
				item->gradientTransp3(),
				item->gradientTransp4()
				);

	// Gradient Vector
	buttonFillColor->setGradientVector(
				item->gradientStartX(),
				item->gradientStartY(),
				item->gradientEndX(),
				item->gradientEndY(),
				item->gradientFocalX(),
				item->gradientFocalY(),
				item->gradientScale(),
				item->gradientSkew(),
				item->gradientControl1().x(),
				item->gradientControl1().y(),
				item->gradientControl2().x(),
				item->gradientControl2().y(),
				item->gradientControl3().x(),
				item->gradientControl3().y(),
				item->gradientControl4().x(),
				item->gradientControl4().y(),
				item->gradientControl5().x(),
				item->gradientControl5().y()
				);	

	// Gradient Mesh
	if (m_ScMW->view->editStrokeGradient == GradientEdit::Mesh_Point/* || (m_ScMW->view->editStrokeGradient == GradientEdit::Mesh_Color)*/)
	{
		if ((item->selectedMeshPointX > -1) && (item->selectedMeshPointY > -1))
		{
			MeshPoint mp = item->meshGradientArray[item->selectedMeshPointX][item->selectedMeshPointY];

			buttonFillColor->setGradientMesh(
						mp.colorName, mp.shade, mp.transparency
						);
		}

		WidgetManager::instance().colorPicker()->enableMeshColor((item->selectedMeshPointX > -1) && (item->selectedMeshPointY > -1));
	}
	else if (m_ScMW->view->editStrokeGradient == GradientEdit::PatchMesh_Point)
	{
		if ((item->selectedMeshPointX > -1) && (item->selectedMeshPointY > 0))
		{
			int index = qBound(0, item->selectedMeshPointX, item->meshGradientPatches.count()-1);

			meshGradientPatch patch = item->meshGradientPatches[index];
			MeshPoint mp;
			switch (item->selectedMeshPointY)
			{
				case 1:
					mp = patch.TL;
					break;
				case 2:
					mp = patch.TR;
					break;
				case 3:
					mp = patch.BR;
					break;
				case 4:
					mp = patch.BL;
					break;
			}

			buttonFillColor->setGradientMesh(
						mp.colorName, mp.shade, mp.transparency
						);
		}

		WidgetManager::instance().colorPicker()->enableButtonRemovePatch((item->selectedMeshPointX > -1) && (item->meshGradientPatches.count() > 1));
		WidgetManager::instance().colorPicker()->enableMeshColor((item->selectedMeshPointX > -1) && (item->selectedMeshPointY > 0));

	}

	// Hatch
	buttonFillColor->setHatch(
				item->hatchType,
				item->hatchDistance,
				item->hatchAngle,
				item->hatchUseBackground,
				item->hatchBackground,
				item->hatchForeground
				);
	// Pattern
	bool mirrorX, mirrorY;
	item->patternFlip(mirrorX, mirrorY);
	buttonFillColor->setPattern(item->pattern(),
								item->patternTransform().offsetX,
								item->patternTransform().offsetY,
								item->patternTransform().scaleX,
								item->patternTransform().scaleY,
								item->patternTransform().skewX,
								item->patternTransform().skewY,
								item->patternTransform().rotation,
								0.0, mirrorX, mirrorY, false
								);
	// Color
	buttonFillColor->setColor(item->fillColor(), item->fillShade());
	// General
	buttonFillColor->setGeneral(item->overprint());

	buttonFillColor->updateFloatingContext();
	buttonFillColor->updatePreview();

}

void PropertiesPalette_Fill::setCurrentItem_FillMask(PageItem *item)
{
	buttonFillMask->setType(item->maskType());
	// Gradient
	buttonFillMask->setGradient(
				item->gradientMask(),
				item->mask_gradient
				);

	// Gradient Vector
	buttonFillMask->setGradientVector(
				item->gradientMaskStartX(),
				item->gradientMaskStartY(),
				item->gradientMaskEndX(),
				item->gradientMaskEndY(),
				item->gradientMaskFocalX(),
				item->gradientMaskFocalY(),
				item->gradientMaskScale(),
				item->gradientMaskSkew()
				);

	// Pattern
	bool mirrorX, mirrorY;
	item->maskFlip(mirrorX, mirrorY);
	buttonFillMask->setPattern(item->patternMask(),
							   item->maskTransform().offsetX,
							   item->maskTransform().offsetY,
							   item->maskTransform().scaleX,
							   item->maskTransform().scaleY,
							   item->maskTransform().skewX,
							   item->maskTransform().skewY,
							   item->maskTransform().rotation,
							   0.0, mirrorX, mirrorY, false
							   );
	// Color
	buttonFillMask->setColor(
				"",
				100,
				item->fillTransparency()
				);

	buttonFillMask->updateFloatingContext();
	buttonFillMask->updatePreview();

}

void PropertiesPalette_Fill::setCurrentItem(PageItem *item)
{
	if (!m_ScMW || m_ScMW->scriptIsRunning())
		return;
	//CB We shouldn't really need to process this if our item is the same one
	//maybe we do if the item has been changed by scripter.. but that should probably
	//set some status if so.
	//FIXME: This won't work until when a canvas deselect happens, m_item must be nullptr.
	//if (m_item == i)
	//	return;

	if (item && !m_doc)
		setDoc(item->doc());

	m_haveItem = false;
	m_item = item;

	if (!m_item)
		return;

	int itemType = m_item ? (int) m_item->itemType() : -1;

	switch (itemType)
	{
	case -1:

		break;
	case PageItem::LatexFrame:
	case PageItem::OSGFrame:
	case PageItem::Group:
	case PageItem::Symbol:
	case PageItem::Table:
	case PageItem::ItemType1:
	case PageItem::ItemType3:
	case PageItem::Polygon:
	case PageItem::RegularPolygon:
	case PageItem::Arc:
	case PageItem::PolyLine:
	case PageItem::Spiral:
		nonZero->setEnabled(true);
		evenOdd->setEnabled(true);
		nonZero->setChecked(!m_item->fillRule);
		evenOdd->setChecked(m_item->fillRule);

		if (m_item->isGroup())
			labelFillColor->setEnabled(false);
		else
			labelFillColor->setEnabled(true);

		break;
	case PageItem::ImageFrame:
	case PageItem::TextFrame:
	case PageItem::PathText:
		nonZero->setChecked(false);
		nonZero->setEnabled(false);
		evenOdd->setChecked(false);
		evenOdd->setEnabled(false);
		labelFillColor->setEnabled(true);
		break;
	}

	setCurrentItem_Fill(m_item);
	setCurrentItem_FillMask(m_item);

	QSignalBlocker sigFillBlendMode(comboBlendmode);
	comboBlendmode->setCurrentIndex(m_item->fillBlendmode());

	m_haveItem = true;
}

void PropertiesPalette_Fill::handleFillRule()
{
	if (!m_haveDoc || !m_haveItem || !m_ScMW || m_ScMW->scriptIsRunning())
		return;
	m_item->fillRule = evenOdd->isChecked();
	m_item->update();
}

void PropertiesPalette_Fill::handleFillColor()
{
	if (!m_doc ||!m_item || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	blockUpdates(true);

	switch(buttonFillColor->mode())
	{
	default:
	case Mode::Solid:
		m_doc->itemSelection_SetItemBrush(buttonFillColor->colorData().Name);
		m_doc->itemSelection_SetItemBrushShade(static_cast<int>(buttonFillColor->colorData().Shade));
		break;
	case Mode::Gradient:
	{
		if (buttonFillColor->gradientData().Name.isEmpty())
		{
			m_item->setGradient("");
			m_doc->itemSelection_SetFillGradient(buttonFillColor->gradientData().Gradient);
		}
		else
		{
			m_item->setGradient(buttonFillColor->gradientData().Name);
		}

		UndoTransaction trans;
		if (UndoManager::undoEnabled())
			trans = undoManager->beginTransaction(Um::Selection, Um::IFill, Um::GradVal, QString(), Um::IFill);
		m_item->set4ColorShade(
					static_cast<int>(buttonFillColor->gradientData().Color1Shade),
					static_cast<int>(buttonFillColor->gradientData().Color2Shade),
					static_cast<int>(buttonFillColor->gradientData().Color3Shade),
					static_cast<int>(buttonFillColor->gradientData().Color4Shade)
					);
		m_item->set4ColorTransparency(
					buttonFillColor->gradientData().Color1Alpha,
					buttonFillColor->gradientData().Color2Alpha,
					buttonFillColor->gradientData().Color3Alpha,
					buttonFillColor->gradientData().Color4Alpha
					);
		m_item->set4ColorColors(
					buttonFillColor->gradientData().Color1Name,
					buttonFillColor->gradientData().Color2Name,
					buttonFillColor->gradientData().Color3Name,
					buttonFillColor->gradientData().Color4Name
					);
		if (trans)
			trans.commit();

		m_item->setGradientExtend(buttonFillColor->gradientData().repeatMethod);
		m_item->update();
		m_doc->regionsChanged()->update(QRect());
	}
		break;
	case Mode::Hatch:
		m_doc->itemSelection_SetItemHatchFill(
					buttonFillColor->hatchData().Type,
					buttonFillColor->hatchData().Distance,
					buttonFillColor->hatchData().Angle,
					buttonFillColor->hatchData().HasBackgroundColor,
					buttonFillColor->hatchData().ColorBackground,
					buttonFillColor->hatchData().ColorForeground
					);
		break;
	case Mode::Pattern:
		m_doc->itemSelection_SetItemPatternFill(buttonFillColor->patternData().Name);
		m_doc->itemSelection_SetItemPatternProps(
					buttonFillColor->patternData().ScaleX,
					buttonFillColor->patternData().ScaleY,
					buttonFillColor->patternData().OffsetX,
					buttonFillColor->patternData().OffsetY,
					buttonFillColor->patternData().Angle,
					buttonFillColor->patternData().SkewX,
					buttonFillColor->patternData().SkewY,
					buttonFillColor->patternData().MirrorX,
					buttonFillColor->patternData().MirrorY
					);
		break;
	}

	m_doc->itemSelection_SetItemGradFill(buttonFillColor->type());
	m_doc->itemSelection_SetOverprint(buttonFillColor->generalData().overprint);

	blockUpdates(false);

}

void PropertiesPalette_Fill::handleFillColorVector()
{
	if (!m_doc ||!m_item || !m_ScMW || m_ScMW->scriptIsRunning() || buttonFillColor->mode() != Mode::Gradient)
		return;

	CPGradientVectorData gvd = buttonFillColor->gradientVectorData();

	QRectF upRect;
	UndoTransaction trans;
	if (UndoManager::undoEnabled())
		trans = undoManager->beginTransaction(Um::Selection, Um::ILine, Um::GradPos + "p", "", Um::ILine);

	switch (buttonFillColor->type())
	{
	case Gradient_4Colors:
		m_item->setGradientControl1(FPoint(gvd.C1X, gvd.C1Y));
		m_item->setGradientControl2(FPoint(gvd.C2X, gvd.C2Y));
		m_item->setGradientControl3(FPoint(gvd.C3X, gvd.C3Y));
		m_item->setGradientControl4(FPoint(gvd.C4X, gvd.C4Y));
		m_item->update();
		upRect = QRectF(QPointF(-m_item->width(), -m_item->height()), QPointF(m_item->width() * 2, m_item->height() * 2)).normalized();
		break;
	case Gradient_Diamond:
		m_item->setGradientControl1(FPoint(gvd.C1X, gvd.C1Y));
		m_item->setGradientControl2(FPoint(gvd.C2X, gvd.C2Y));
		m_item->setGradientControl3(FPoint(gvd.C3X, gvd.C3Y));
		m_item->setGradientControl4(FPoint(gvd.C4X, gvd.C4Y));
		m_item->setGradientControl5(FPoint(gvd.C5X, gvd.C5Y));
		m_item->update();
		upRect = QRectF(QPointF(-m_item->width(), -m_item->height()), QPointF(m_item->width() * 2, m_item->height() * 2)).normalized();
		break;
	default:
		if (m_item->gradientType() == Gradient_Conical && UndoManager::undoEnabled())
		{
			SimpleState *ss= new SimpleState("Refresh");
			ss->set("UNDO_UPDATE_CONICAL");
			undoManager->action(m_item,ss);
		}

		m_item->setGradientStart(gvd.StartX, gvd.StartY);
		m_item->setGradientEnd(gvd.EndX, gvd.EndY);
		m_item->setGradientFocal(gvd.FocalX, gvd.FocalY);
		m_item->setGradientScale(gvd.Scale);
		m_item->setGradientSkew(gvd.Skew);

		if (m_item->gradientType() == Gradient_Conical && UndoManager::undoEnabled())
		{
			m_item->createConicalMesh();
			SimpleState *ss= new SimpleState("Refresh");
			ss->set("REDO_UPDATE_CONICAL");
			undoManager->action(m_item,ss);
		}

		m_item->update();
		upRect = QRectF(m_item->gradientStart(), m_item->gradientEnd());
		double radEnd = distance(m_item->gradientEndX() - m_item->gradientStartX(), m_item->gradientEndY() - m_item->gradientStartY());
		double rotEnd = xy2Deg(m_item->gradientEndX() - m_item->gradientStartX(), m_item->gradientEndY() - m_item->gradientStartY());
		QTransform m;
		m.translate(m_item->gradientStartX(), m_item->gradientStartY());
		m.rotate(rotEnd);
		m.rotate(-90);
		m.rotate(m_item->gradientSkew());
		m.translate(radEnd * m_item->gradientScale(), 0);
		QPointF shP = m.map(QPointF(0,0));
		upRect |= QRectF(shP, m_item->gradientEnd()).normalized();
		upRect |= QRectF(shP, m_item->gradientStart()).normalized();
		upRect |= QRectF(shP, QPointF(0, 0)).normalized();
		upRect |= QRectF(shP, QPointF(m_item->width(), m_item->height())).normalized();
		break;
	}

	if (trans)
		trans.commit();

	upRect.translate(m_item->xPos(), m_item->yPos());
	m_doc->regionsChanged()->update(upRect.adjusted(-10.0, -10.0, 10.0, 10.0));
	m_doc->changed();
	m_doc->changedPagePreview();

}

void PropertiesPalette_Fill::handleFillColorMesh()
{
	if (!m_doc ||!m_item || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	CPColorData mcd = buttonFillColor->gradientMeshData();

	// Mesh Color
	if (m_ScMW->view->editStrokeGradient == GradientEdit::Mesh_Point)
	{
		if ((m_item->selectedMeshPointX > -1) && (m_item->selectedMeshPointY > -1))
				m_item->setMeshPointColor(m_item->selectedMeshPointX, m_item->selectedMeshPointY,
									  mcd.Name,
									  mcd.Shade,
									  mcd.Opacity
									  );
	}

	// Mesh Patch Color
	if (m_ScMW->view->editStrokeGradient == GradientEdit::PatchMesh_Point)
	{
		if ((m_item->selectedMeshPointX > -1) && (m_item->selectedMeshPointY > -1))
				m_item->setMeshPointColor(m_item->selectedMeshPointX, m_item->selectedMeshPointY,
										  mcd.Name,
										  mcd.Shade,
										  mcd.Opacity,
										  true
									  );
	}

	m_item->update();
	m_doc->regionsChanged()->update(QRect());

}

void PropertiesPalette_Fill::handleFillMask()
{
	if (!m_doc || !m_item || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	blockUpdates(true);
	m_doc->itemSelection_SetItemGradMask(buttonFillMask->type());

	switch(buttonFillMask->mode())
	{
	default:
	case Mode::Solid:
		m_doc->itemSelection_SetItemFillTransparency(buttonFillMask->colorData().Opacity);
		break;
	case Mode::Gradient:

		if (buttonFillMask->gradientData().Name.isEmpty())
		{
			m_item->setGradientMask("");
			m_doc->itemSelection_SetMaskGradient(buttonFillMask->gradient());
		}
		else
		{
			m_item->setGradientMask(buttonFillMask->gradientName());
		}

		break;
	case Mode::Hatch:
		// Do nothing
		break;
	case Mode::Pattern:
		m_doc->itemSelection_SetItemPatternMask(buttonFillMask->patternData().Name);
		m_doc->itemSelection_SetItemPatternMaskProps(
					buttonFillMask->patternData().ScaleX,
					buttonFillMask->patternData().ScaleY,
					buttonFillMask->patternData().OffsetX,
					buttonFillMask->patternData().OffsetY,
					buttonFillMask->patternData().Angle,
					buttonFillMask->patternData().SkewX,
					buttonFillMask->patternData().SkewY,
					buttonFillMask->patternData().MirrorX,
					buttonFillMask->patternData().MirrorY
					);
		break;
	}
	blockUpdates(false);
}

void PropertiesPalette_Fill::handleFillMaskVector()
{
	if (!m_doc ||!m_item || !m_ScMW || m_ScMW->scriptIsRunning() || buttonFillMask->mode() != Mode::Gradient)
		return;

	CPGradientVectorData gvd = buttonFillMask->gradientVectorData();

	QRectF upRect;
	UndoTransaction trans;
	if (UndoManager::undoEnabled())
		trans = undoManager->beginTransaction(Um::Selection, Um::ILine, Um::GradPos + "o", "", Um::ILine);

	m_item->setGradientMaskStart(gvd.StartX, gvd.StartY);
	m_item->setGradientMaskEnd(gvd.EndX, gvd.EndY);
	m_item->setGradientMaskFocal(gvd.FocalX, gvd.FocalY);
	m_item->setGradientMaskScale(gvd.Scale);
	m_item->setGradientMaskSkew(gvd.Skew);

//	if ((m_item->GrMask == GradMask_Linear) || (m_item->GrMask == GradMask_LinearLumAlpha))
//	{
//		m_item->setGradientMaskFocal(m_item->GrMaskStartX, m_item->GrMaskStartY);
//	}
	m_item->update();
	if (trans)
	{
		trans.commit();
	}
	upRect = QRectF(m_item->gradientMaskStart(), m_item->gradientMaskEnd());
	double radEnd = distance(m_item->GrMaskEndX - m_item->GrMaskStartX, m_item->GrMaskEndY - m_item->GrMaskStartY);
	double rotEnd = xy2Deg(m_item->GrMaskEndX - m_item->GrMaskStartX, m_item->GrMaskEndY - m_item->GrMaskStartY);
	QTransform m;
	m.translate(m_item->GrMaskStartX, m_item->GrMaskStartY);
	m.rotate(rotEnd);
	m.rotate(-90);
	m.rotate(m_item->GrMaskSkew);
	m.translate(radEnd * m_item->GrMaskScale, 0);
	QPointF shP = m.map(QPointF(0,0));
	upRect |= QRectF(shP, m_item->gradientMaskEnd()).normalized();
	upRect |= QRectF(shP, m_item->gradientMaskStart()).normalized();
	upRect |= QRectF(shP, QPointF(0, 0)).normalized();
	upRect |= QRectF(shP, QPointF(m_item->width(), m_item->height())).normalized();
	upRect.translate(m_item->xPos(), m_item->yPos());
	m_doc->regionsChanged()->update(upRect.adjusted(-10.0, -10.0, 10.0, 10.0));
	m_doc->changed();
	m_doc->changedPagePreview();
}

void PropertiesPalette_Fill::handleFillBlendmode(int mode)
{
	if (!m_doc || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	m_doc->itemSelection_SetItemFillBlend(comboBlendmode->currentIndex());
}

void PropertiesPalette_Fill::handleSelectionChanged()
{
	if (!m_haveDoc || !m_ScMW || m_ScMW->scriptIsRunning())
		return;

	if (updatesBlocked())
		return;

	PageItem* currItem = currentItemFromSelection();
	if (m_doc->m_Selection->count() > 1)
	{
		setEnabled(true);
	}
	else
	{
		int itemType = currItem ? (int) currItem->itemType() : -1;

		m_haveItem = (itemType != -1);
		switch (itemType)
		{
		case -1:
		case PageItem::OSGFrame:
		case PageItem::Line:
		case PageItem::Symbol:
			setEnabled(false);
			break;
		case PageItem::Arc:
		case PageItem::ItemType1:
		case PageItem::ItemType3:
		case PageItem::Polygon:
		case PageItem::RegularPolygon:
		case PageItem::PolyLine:
		case PageItem::Spiral:
		case PageItem::PathText:
		case PageItem::Table:
		case PageItem::TextFrame:
		case PageItem::ImageFrame:
		case PageItem::LatexFrame:
			setEnabled(true);
			break;
		}
	}
	setCurrentItem(currItem);
	updateGeometry();
}

void PropertiesPalette_Fill::handleUpdateRequest(int updateFlags)
{
	if (updateFlags & reqColorsUpdate)
		updateColorList();
}

void PropertiesPalette_Fill::iconSetChange()
{
	IconManager& im = IconManager::instance();

	evenOdd->setIcon(im.loadIcon("fill-rule-even-odd.png"));
	nonZero->setIcon(im.loadIcon("fill-rule-nonzero.png"));

	if (labelFillMask->labelVisibility())
		buttonFillMask->setIcon(QIcon());
	else
		buttonFillMask->setIcon(im.loadIcon("mask", 8));
}

void PropertiesPalette_Fill::languageChange()
{
	retranslateUi(this);

	comboBlendmode->setToolTip( tr("Blendmode of fill"));
}

void PropertiesPalette_Fill::unitChange()
{
	m_unitRatio = m_doc->unitRatio();
}

void PropertiesPalette_Fill::toggleLabelVisibility(bool v)
{
	labelFillColor->setLabelVisibility(v);
	labelFillMask->setLabelVisibility(v);
	labelFillRule->setLabelVisibility(v);
	labelBlendmode->setLabelVisibility(v);

	iconSetChange();
}


