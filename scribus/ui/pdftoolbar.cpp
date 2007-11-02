/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          texttoolb.cpp  -  description
                             -------------------
    begin                : Sun Mar 10 2002
    copyright            : (C) 2002 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "pdftoolbar.h"

#include "scraction.h"
#include "scribus.h"

PDFToolBar::PDFToolBar(ScribusMainWindow* parent) : ScToolBar( tr("PDF Tools"), "PDF_Tools", parent)
{
	this->addAction(parent->scrActions["toolsPDFPushButton"]);
	this->addAction(parent->scrActions["toolsPDFTextField"]);
	this->addAction(parent->scrActions["toolsPDFCheckBox"]);
	this->addAction(parent->scrActions["toolsPDFComboBox"]);
	this->addAction(parent->scrActions["toolsPDFListBox"]);
	this->addAction(parent->scrActions["toolsPDFAnnotText"]);
	this->addAction(parent->scrActions["toolsPDFAnnotLink"]);
}
