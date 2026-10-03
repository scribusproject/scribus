/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          toolpalette.cpp -  description
                             -------------------
    begin                : Fri Aug 28 2026
 ***************************************************************************/

#include "toolpalette.h"

#include <QAction>
#include <QApplication>
#include <QEnterEvent>
#include <QEvent>
#include <QFrame>
#include <QIcon>
#include <QMenu>
#include <QMouseEvent>
#include <QPainter>
#include <QPalette>
#include <QPointer>
#include <QSizePolicy>
#include <QStatusBar>
#include <QToolButton>
#include <QVBoxLayout>

#include <utility>

#include "autoformbuttongroup.h"
#include "modetoolbar.h"
#include "polyprops.h"
#include "scraction.h"
#include "scribus.h"
#include "scribusapp.h"
#include "scribusdoc.h"

class ToolPaletteButton : public QToolButton
{
public:
	ToolPaletteButton(ToolPalette* palette)
		: QToolButton(palette), m_palette(palette)
	{
		setAutoRaise(false);
	}

protected:
	void enterEvent(QEnterEvent *e) override
	{
		if (m_palette && defaultAction())
			m_palette->updateToolHelp(defaultAction());
		QToolButton::enterEvent(e);
	}

	void mousePressEvent(QMouseEvent *e) override
	{
		if (m_palette && defaultAction())
			m_palette->updateToolHelp(defaultAction());
		QToolButton::mousePressEvent(e);
	}

private:
	ToolPalette* m_palette { nullptr };
};

ToolPalette::ToolPalette(QWidget* parent) : DockPanelBase( tr("Tools"), "tool-select", parent)
{
	m_ScMW = qobject_cast<ScribusMainWindow*>(parent);
	if (!m_ScMW)
		return;
	// objectName/prefs context are derived from the dock title ("Tools")
	// by DockPanelBase; do not override them here as the Advanced Docking
	// System uses the objectName to save and restore the dock state.

	ValCount = 32;
	static double AutoShapes0[] = {0.0, 0.0, 0.0, 0.0, 100.0, 0.0, 100.0, 0.0, 100.0, 0.0, 100.0, 0.0,
								  100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 0.0, 100.0, 0.0, 100.0,
								  0.0, 100.0, 0.0, 100.0, 0.0, 0.0, 0.0, 0.0};
	ShapeVals = AutoShapes0;

	QWidget* content = new QWidget(this);
	content->setObjectName(QStringLiteral("toolPaletteContent"));
	content->setFixedWidth(56);
	QVBoxLayout* vbox = new QVBoxLayout(content);
	vbox->setContentsMargins(8, 6, 8, 6);
	vbox->setSpacing(2);
	vbox->setAlignment(Qt::AlignTop | Qt::AlignHCenter);

	// Persistent primary tools. Each segmented button exposes its complete
	// family and promotes the last selected member, matching Adobe toolbars.
	QToolButton* selectBtn = addToolButtonEntry("toolsSelect", vbox);
	addToolSeparator(vbox);
	addToolButtonEntry("toolsTextCursor", vbox);
	QToolButton* textBtn = addToolButtonEntry("toolsInsertTextFrame", vbox);
	QToolButton* frameBtn = addToolButtonEntry("toolsInsertImageFrame", vbox);
	addToolSeparator(vbox);
	QToolButton* shapeBtn = addToolButtonEntry("toolsInsertShape", vbox);
	QToolButton* lineBtn = addToolButtonEntry("toolsInsertLine", vbox);
	addToolSeparator(vbox);
	addToolButtonEntry("toolsRotate", vbox);
	addToolButtonEntry("toolsCopyProperties", vbox);
	QToolButton* linkBtn = addToolButtonEntry("toolsLinkTextFrame", vbox);
	addToolSeparator(vbox);
	QToolButton* inspectBtn = addToolButtonEntry("toolsEyeDropper", vbox);
	addToolButtonEntry("toolsZoom", vbox);
	addToolSeparator(vbox);
	QToolButton* pdfBtn = addToolButtonEntry("toolsPDFPushButton", vbox);
	vbox->addStretch(1);

	setWidget(content);

	// --- sub-tool flyout menus ------------------------------------------------

	// Shape: reuse the shared AutoformButtonGroup of the internal ModeToolBar so
	// that the shape sub-mode state (SubMode, ShapeVals, ValCount) is identical
	// and existing Path Draw behavior is preserved.
	if (m_ScMW->modeToolBar)
	{
		autoFormButtonGroup = m_ScMW->modeToolBar->getAutoformButtonGroup();
		if (autoFormButtonGroup)
		{
			m_ScMW->scrActions["toolsInsertShape"]->setMenu(nullptr);
			refreshToolIcon(shapeBtn);
			connect( autoFormButtonGroup, SIGNAL(FormSel(int,int,qreal*)), this, SLOT(SelShape(int,int,qreal*)) );
		}

		// Calligraphic line: reuse the shared angle/width pen settings.
		calligraphicSettingsMenu = m_ScMW->modeToolBar->getCalligraphicMenu();
		if (calligraphicSettingsMenu)
			m_ScMW->scrActions["toolsInsertCalligraphicLine"]->setMenu(nullptr);
	}

	// Polygon: side presets plus the classic properties dialog.
	insertPolygonButtonMenu = new QMenu(this);
	for (int sides : {3, 4, 5, 6, 8, 10, 12, 24, 36, 64})
	{
		QAction* act = insertPolygonButtonMenu->addAction(QString("%1").arg(sides));
		act->setData(sides);
		connect(act, &QAction::triggered, this, [this, sides]() { setPolygonSides(sides); });
	}
	insertPolygonButtonMenu->addSeparator();
	idPolygonPropertiesAction = insertPolygonButtonMenu->addAction( tr("Properties..."), this, SLOT(GetPolyProps()) );

	configureToolGroup(selectBtn, {"toolsSelect", "toolsEditContents"});
	configureToolGroup(textBtn, {"toolsInsertTextFrame", "toolsEditWithStoryEditor"});
	configureToolGroup(frameBtn, {"toolsInsertImageFrame", "toolsInsertTable", "toolsInsertRenderFrame"});
	QMenu* shapeMenu = configureToolGroup(shapeBtn,
		{"toolsInsertShape", "toolsInsertPolygon", "toolsInsertArc", "toolsInsertSpiral"});
	shapeMenu->addSeparator();
	if (autoFormButtonGroup)
		shapeMenu->addMenu(autoFormButtonGroup);
	shapeMenu->addMenu(insertPolygonButtonMenu);
	lineButtonMenu = configureToolGroup(lineBtn,
		{"toolsInsertLine", "toolsInsertBezier", "toolsInsertFreehandLine", "toolsInsertCalligraphicLine"});
	if (calligraphicSettingsMenu)
	{
		lineButtonMenu->addSeparator();
		lineButtonMenu->addMenu(calligraphicSettingsMenu);
	}
	configureToolGroup(linkBtn, {"toolsLinkTextFrame", "toolsUnlinkTextFrame"});
	configureToolGroup(inspectBtn, {"toolsEyeDropper", "toolsMeasurements"});
	configureToolGroup(pdfBtn,
		{"toolsPDFPushButton", "toolsPDFCheckBox", "toolsPDFRadioButton", "toolsPDFTextField",
		 "toolsPDFComboBox", "toolsPDFListBox", "toolsPDFAnnotText", "toolsPDFAnnotLink"});

	// The tool actions default to checked=true (see ScrAction::setToggleAction);
	// clear that here so the palette starts with a single active tool instead of
	// every button shown pressed. AppModeHelper sets the correct exclusive state
	// whenever the active tool changes.
	for (auto it = m_buttons.constBegin(); it != m_buttons.constEnd(); ++it)
		m_ScMW->scrActions[it.key()]->setChecked(false);
	m_ScMW->scrActions["toolsSelect"]->setChecked(true);

	connect(ScQApp, &ScribusQApp::iconSetChanged, this, [this]() {
		for (QToolButton* button : std::as_const(m_buttons))
			refreshToolIcon(button);
	});

	languageChange();
}

ToolPalette::~ToolPalette()
{
}

void ToolPalette::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
		languageChange();
	else
		QWidget::changeEvent(e);
}

void ToolPalette::setDoc(ScribusDoc* doc)
{
	m_doc = doc;
}

void ToolPalette::GetPolyProps()
{
	ScribusDoc* doc = m_doc ? m_doc : m_ScMW->doc;
	if (!doc)
		return;
	PolygonProps* dia = new PolygonProps(m_ScMW,
		doc->itemToolPrefs().polyCorners,
		doc->itemToolPrefs().polyFactor,
		doc->itemToolPrefs().polyUseFactor,
		doc->itemToolPrefs().polyRotation,
		doc->itemToolPrefs().polyCurvature,
		doc->itemToolPrefs().polyInnerRot,
		doc->itemToolPrefs().polyOuterCurvature);
	if (dia->exec())
	{
		dia->getValues(
			&doc->itemToolPrefs().polyCorners,
			&doc->itemToolPrefs().polyFactor,
			&doc->itemToolPrefs().polyUseFactor,
			&doc->itemToolPrefs().polyRotation,
			&doc->itemToolPrefs().polyCurvature,
			&doc->itemToolPrefs().polyInnerRot,
			&doc->itemToolPrefs().polyOuterCurvature);
		m_ScMW->scrActions["toolsInsertPolygon"]->trigger();
	}
	delete dia;
}

void ToolPalette::setPolygonSides(int sides)
{
	ScribusDoc* doc = m_doc ? m_doc : m_ScMW->doc;
	if (!doc)
		return;
	doc->itemToolPrefs().polyCorners = sides;
	m_ScMW->scrActions["toolsInsertPolygon"]->trigger();
}

void ToolPalette::SelShape(int s, int c, qreal *vals)
{
	if (autoFormButtonGroup)
		m_ScMW->scrActions["toolsInsertShape"]->setIcon(QIcon(autoFormButtonGroup->getIconPixmap(s, 24)));
	SubMode = s;
	ValCount = c;
	ShapeVals = vals;
	m_ScMW->scrActions["toolsInsertShape"]->setChecked(false);
	m_ScMW->scrActions["toolsInsertShape"]->setChecked(true);
}

void ToolPalette::languageChange()
{
	if (idPolygonPropertiesAction)
		idPolygonPropertiesAction->setText( tr("Properties...") );
	if (autoFormButtonGroup)
		autoFormButtonGroup->setTitle(tr("Shape Presets"));
	if (insertPolygonButtonMenu)
		insertPolygonButtonMenu->setTitle(tr("Polygon Sides"));
	if (calligraphicSettingsMenu)
		calligraphicSettingsMenu->setTitle(tr("Calligraphy Settings"));

	updateToolHelp(m_ScMW->scrActions["toolsSelect"]);
}

void ToolPalette::updateToolHelp(QAction* action)
{
	if (!action)
		return;
	ScrAction* scrAct = qobject_cast<ScrAction*>(action);
	QString name = scrAct ? scrAct->cleanMenuText() : action->text();
	QString sct = action->shortcut().toString(QKeySequence::NativeText);
	QString status = action->statusTip();
	if (!sct.isEmpty() && status.endsWith("(" + sct + ")"))
		status.chop(sct.length() + 2);

	QString message = name;
	if (!sct.isEmpty())
		message += " (" + sct + ")";
	if (!status.isEmpty())
		message += " — " + status;
	if (m_ScMW && m_ScMW->statusBar())
		m_ScMW->statusBar()->showMessage(message, 3000);
}

void ToolPalette::addToolSeparator(QVBoxLayout* layout)
{
	QFrame* separator = new QFrame(this);
	separator->setObjectName("toolSeparator");
	separator->setFrameShape(QFrame::NoFrame);
	separator->setFixedSize(24, 1);
	layout->addWidget(separator, 0, Qt::AlignHCenter);
}

QToolButton* ToolPalette::addToolButtonEntry(const QString &actionName, QVBoxLayout* layout)
{
	QPointer<ScrAction> action = m_ScMW->scrActions.value(actionName);
	ToolPaletteButton* btn = new ToolPaletteButton(this);
	btn->setObjectName("toolButton");
	btn->setDefaultAction(action);
	btn->setAutoRaise(true);
	btn->setIconSize(QSize(24, 24));
	btn->setToolButtonStyle(Qt::ToolButtonIconOnly);
	btn->setFixedSize(40, 40);
	btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	btn->setAccessibleName(action ? action->text().remove('&') : QString());
	refreshToolIcon(btn);
	layout->addWidget(btn, 0, Qt::AlignHCenter);
	m_buttons.insert(actionName, btn);
	connect(action, &QAction::toggled, this, [this, action](bool on) {
		if (on)
			updateToolHelp(action);
	});
	return btn;
}

QMenu* ToolPalette::configureToolGroup(QToolButton* button, const QStringList &actionNames)
{
	QMenu* menu = new QMenu(button);
	for (const QString& actionName : actionNames)
	{
		QPointer<ScrAction> action = m_ScMW->scrActions.value(actionName);
		if (!action)
			continue;

		menu->addAction(action);
		m_buttons.insert(actionName, button);
		connect(action, &QAction::toggled, this, [this, button, menu, action](bool on) {
			if (!on)
				return;
			button->setDefaultAction(action);
			button->setMenu(menu);
			button->setPopupMode(QToolButton::DelayedPopup);
			button->setAccessibleName(action->text().remove('&'));
			refreshToolIcon(button);
			updateToolHelp(action);
		});
	}
	button->setMenu(menu);
	button->setPopupMode(QToolButton::DelayedPopup);
	return menu;
}

void ToolPalette::refreshToolIcon(QToolButton* button)
{
	if (!button || !button->defaultAction())
		return;

	const QSize iconSize(24, 24);
	QPixmap normal = button->defaultAction()->icon().pixmap(iconSize, QIcon::Normal, QIcon::On);
	if (normal.isNull())
		return;

	QPixmap selected = normal;
	QPainter painter(&selected);
	painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	painter.fillRect(selected.rect(), QApplication::palette().color(QPalette::Highlight));
	painter.end();

	const QPalette applicationPalette = QApplication::palette();
	const QColor base = applicationPalette.color(QPalette::Base);
	const QColor accent = applicationPalette.color(QPalette::Highlight);
	const qreal accentWeight = base.lightness() < 128 ? 0.30 : 0.13;
	auto blend = [accentWeight](int baseChannel, int accentChannel) {
		return qRound(baseChannel * (1.0 - accentWeight) + accentChannel * accentWeight);
	};
	QColor selectedBackground(
		blend(base.red(), accent.red()),
		blend(base.green(), accent.green()),
		blend(base.blue(), accent.blue()));
	QPalette buttonPalette = button->palette();
	buttonPalette.setColor(QPalette::Button, selectedBackground);
	button->setPalette(buttonPalette);

	QIcon icon;
	icon.addPixmap(normal, QIcon::Normal, QIcon::Off);
	icon.addPixmap(normal, QIcon::Active, QIcon::Off);
	icon.addPixmap(selected, QIcon::Normal, QIcon::On);
	icon.addPixmap(selected, QIcon::Active, QIcon::On);
	button->setIcon(icon);
}
