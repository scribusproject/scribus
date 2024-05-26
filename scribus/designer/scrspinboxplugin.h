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


#ifndef SCRSPINBOXPLUGIN_H
#define SCRSPINBOXPLUGIN_H

#include <QDesignerContainerExtension>
#include <QDesignerCustomWidgetInterface>

class QDesignerFormEditorInterface;


/*! \brief A ScrSpinBox plugin for Qt designer
\author Petr Vanek <petr@scribus.info>
*/
class ScrSpinBoxPlugin : public QObject, public QDesignerCustomWidgetInterface
{
	Q_OBJECT
	Q_INTERFACES(QDesignerCustomWidgetInterface)
public:
	ScrSpinBoxPlugin(QObject* = 0);
	bool isContainer() const;
	bool isInitialized() const;
	QIcon icon() const;
	QString codeTemplate() const;
	QString domXml() const;
	QString group() const;
	QString includeFile() const;
	QString name() const;
	QString toolTip() const;
	QString whatsThis() const;
	QWidget* createWidget(QWidget*);
	void initialize(QDesignerFormEditorInterface*);
private:
	bool initialized;
};

#endif

