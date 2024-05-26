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

#ifndef SECTIONCONTAINEREXTENSION_H
#define SECTIONCONTAINEREXTENSION_H

#include <QtDesigner/QDesignerContainerExtension>

QT_BEGIN_NAMESPACE
class QExtensionManager;
QT_END_NAMESPACE
class SectionContainer;

class SectionContainerExtension: public QObject,
										 public QDesignerContainerExtension
{
	Q_OBJECT
	Q_INTERFACES(QDesignerContainerExtension)

public:
	explicit SectionContainerExtension(SectionContainer *widget, QObject *parent);

	bool canAddWidget() const override;
	void addWidget(QWidget *widget) override;
	int count() const override;
	int currentIndex() const override;
	void insertWidget(int index, QWidget *widget) override;
	bool canRemove(int index) const override;
	void remove(int index) override;
	void setCurrentIndex(int index) override;
	QWidget *widget(int index) const override;

private:
	SectionContainer *myWidget;
};

#endif // SECTIONCONTAINEREXTENSION_H
