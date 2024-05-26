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

#ifndef SECTIONCONTAINEREXTENSIONFACTORY_H
#define SECTIONCONTAINEREXTENSIONFACTORY_H

#include <QtDesigner/QExtensionFactory>

QT_BEGIN_NAMESPACE
class QExtensionManager;
QT_END_NAMESPACE

class SectionContainerExtensionFactory: public QExtensionFactory
{
	Q_OBJECT

public:
	explicit SectionContainerExtensionFactory(QExtensionManager *parent = nullptr);

protected:
	QObject *createExtension(QObject *object, const QString &iid, QObject *parent) const override;
};

#endif // SECTIONCONTAINEREXTENSIONFACTORY_H
