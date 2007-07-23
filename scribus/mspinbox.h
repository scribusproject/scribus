/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          mspinbox.h  -  description
                             -------------------
    begin                : Sat Jun 16 2001
    copyright            : (C) 2001 by Franz Schmid
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

#ifndef MSPINBOX_H
#define MSPINBOX_H

#include <QMap>
#include <QSpinBox>
#include <QString>
//Added by qt3to4:
#include <QEvent>
#include "scribusapi.h"

class FunctionParser;

/** \brief Enthanced QSpinBox
  *@author Franz Schmid
  */
class SCRIBUS_API MSpinBox : public QSpinBox  
{
	Q_OBJECT
	
public:
	/*!
	\author Franz Schmid
	\param pa Parent Widget
	\param s Number of Decimals
	*/
	MSpinBox(QWidget *pa, int s);
	/*!
	\author Franz Schmid
	\param minValue minimal bound
	\param maxValue maximal bound
	\param pa Parent Widget
	\param s Number of Decimals
	*/
	MSpinBox(double minValue, double maxValue, QWidget *pa, int s);
	/*! \brief A dummy constructor for use in Designer's custom widgets
	\warning You have to setup properties later!
	\author Petr Vanek
	\param parent a parent QWidget
	\param name optional name of the object */
	MSpinBox(QWidget *parent, const char * name = 0);

	~MSpinBox();
	/*!
	\author Franz Schmid
	\brief Returns the current Value of the SpinBox
	\retval double The Value
	 */
	double value();
	/*!
	\author Franz Schmid
	\brief Gets the Minimum Value of the Spinbox
	\retval The Value
	*/
	double minValue();
	/*!
	\author Franz Schmid
	\brief Gets the Maximim Value of the Spinbox
	\retval The Value
	*/
	double maxValue();
	virtual QString mapValueToText(int value);
	/*!
	\author Franz Schmid
	\brief Maps the Text of the Spinbox to the Value, does scUnit Conversion and Calculations
	\param ok not used
	\retval int The Value
	*/
	virtual int mapTextToValue(bool *ok);
	/*!
	\author Franz Schmid
	\brief Sets a new unit for the spinbox, doing the value conversions
	\param oldUnitRatio conversion factor for the old unit
	\param newUnitRatio conversion factor for the new unit
	\param unitIndex the unitIndex for the new unit
	*/
	void setNewUnit(double oldUnitRatio, double newUnitRatio, int unitIndex);
	void setTabAdvance(bool enable);
	int Decimals;
	int Width;
	QLineEdit *ed;
	void setDecimals( int deci );
	bool isReadOnly() const;
	void setConstants(const QMap<QString, double>&);
 	void setLineStepM(int val);

public slots:
	void textChanged();
	void stepDown();
	/*!
	\author Franz Schmid
	\brief Sets the Maximum Value of the Spinbox
	\param val new Value
	 */
	void setMaxValue(double val);
	/*!
	\author Franz Schmid
	\brief Sets the Minimum Value of the Spinbox
	\param val new Value
	*/
	void setMinValue(double val);
	/*!
	\author Franz Schmid
	\brief Sets the Value of the Spinbox
	\param val new Value
	*/
	virtual void setValue(double val);
	void setReadOnly(bool ro);
	void setValues(double min, double max, int deci, double val);
	void getValues(double *min, double *max, int *deci, double *val);

protected:
 	bool m_tabAdvance;
	bool eventFilter( QObject* ob, QEvent* ev );
	void setParameters( int s );
	void setFPConstants(FunctionParser &fp);
	bool readOnly;
	int oldLineStep;
	int currLineStep;
	bool edited;
	QMap <QString, double> functionParserConstants;
};

#endif
