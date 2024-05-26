/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
For general Scribus copyright and licensing information please refer
to the COPYING file provided with the program.
*/

#ifndef SECTIONCONTAINERPLUGIN_H
#define SECTIONCONTAINERPLUGIN_H

#include <QtUiPlugin/QDesignerCustomWidgetInterface>

QT_BEGIN_NAMESPACE
class QIcon;
class QWidget;
QT_END_NAMESPACE

class SectionContainerPlugin: public QObject, public QDesignerCustomWidgetInterface
{
	Q_OBJECT
//    Q_PLUGIN_METADATA(IID "net.scribus.uiwidgets")
	Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
	explicit SectionContainerPlugin(QObject *parent = nullptr);

	QString name() const override;
	QString group() const override;
	QString toolTip() const override;
	QString whatsThis() const override;
	QString includeFile() const override;
	QIcon icon() const override;
	bool isContainer() const override;
	QWidget *createWidget(QWidget *parent) override;
	bool isInitialized() const override;
	void initialize(QDesignerFormEditorInterface *formEditor) override;
	QString domXml() const override;

private:
	bool initialized = false;
};

#endif // SECTIONCONTAINERPLUGIN_H
