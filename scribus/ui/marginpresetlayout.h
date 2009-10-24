/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef MARGINPRESETLAYOUT_H
#define MARGINPRESETLAYOUT_H

#include "scribusapi.h"
#include "scribusstructs.h"

#include <QComboBox>

/*! \brief This is inherited QComboBox widget used in MarginWidget as "Preset List".
It contains functionality for margins setting in various ways.
\author Petr Vanek, <petr@yarpen.cz>
*/
class SCRIBUS_API PresetLayout: public QComboBox
{
	Q_OBJECT

public:
	/*! \brief QComboBox like constructor. Values/names are set here. Tooltip etc. too. */
	PresetLayout(QWidget *parent = 0);
	~PresetLayout(){};

	/*! \brief Compute the margins here.
	\param index selected item
	\param pageWidth width of the page. Taken from NewDoc dialog.
	\param pageHeight height of the page. Taken from NewDoc dialog.
	\param leftMargin leadin margin value. The others margins are set in various ratios related to this one.
	*/
	MarginStruct getMargins(int index, double pageWidth, double pageHeight, double leftMargin);

	/*! \brief Integerized indexes for tr() strings*/
	enum presetID
	{
		none = 0,
		gutenberg = 1,
		magazine = 2,
		fibonacci = 3,
		goldencut = 4,
		nineparts = 5
	};

	/*! \brief returns updateMargins value */
	bool needUpdate() const;

private:
	/*! \brief Flag if is needed to recompute values and disable widgets */
	bool updateMargins;
};


#endif
