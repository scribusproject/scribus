/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef ARROWCHOOSER_H
#define ARROWCHOOSER_H

#include <QComboBox>
#include <QList>

#include "scribusapi.h"
#include "scribusstructs.h"

class SCRIBUS_API ArrowChooser : public QComboBox
{
    Q_OBJECT

public:
	ArrowChooser(QWidget* pa, ArrowDirection direction = ArrowDirection::StartArrow);
	~ArrowChooser() {};

	void rebuildList(const QList<ArrowDesc> *arrowStyles);
	void setArrowDirection(ArrowDirection direction);

private:
	ArrowDirection m_arrowDirection { ArrowDirection::StartArrow };
};

#endif
