#include "widget_manager.h"
#include <QDebug>
//#include "selection.h"

#include "ui/widgets/floatingwindow.h"
#include "ui/colorpicker/colorpicker.h"

WidgetManager* WidgetManager::m_instance = nullptr;

WidgetManager::WidgetManager(QObject *parent)
	: QObject(parent)
{

}

WidgetManager &WidgetManager::instance()
{
	static WidgetManager m_instance;
	return m_instance;
}

void WidgetManager::setup(ScribusDoc *doc)
{
	m_doc = doc;
	m_colorPicker = new ColorPicker(/*ColorPickerConfig::Default*/);
	m_colorPicker->setDoc(m_doc);
	m_colorPickerWindow = new FloatingWindow(m_colorPicker, new QWidget());
	connect(m_colorPickerWindow, &FloatingWindow::closed, m_colorPicker, &ColorPicker::parentClosed);

}


FloatingWindow *WidgetManager::colorPickerWindow()
{
	return m_colorPickerWindow;
}

ColorPicker *WidgetManager::colorPicker()
{
	return m_colorPicker;
}

void WidgetManager::unitChange()
{
	m_colorPicker->unitChange();
}
