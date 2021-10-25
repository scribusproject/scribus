/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef NUMFORMATCOMBO_H
#define NUMFORMATCOMBO_H

#include <QComboBox>
#include <QEvent>

#include "scribusapi.h"
#include "numeration.h"

/*!
    \class NumFormatCombo numformatcombo.h
    \brief The PdfVersionCombo widget is a combo box for displaying PDF versions
*/
class SCRIBUS_API NumFormatCombo : public QComboBox 
{
	Q_OBJECT

public:
	NumFormatCombo(QWidget* parent = nullptr);
	NumFormatCombo(QWidget* parent, bool showNone);

	NumFormat currentFormat() const;
	void setCurrentFormat(NumFormat format);
	
protected:
	bool m_showNoneFormat { false };
	
	void changeEvent(QEvent *e) override;
	void languageChange(void);
	void setupCombo();
};

#endif
