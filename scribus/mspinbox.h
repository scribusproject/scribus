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

#include <qspinbox.h>

/**
  *@author Franz Schmid
  */

class MSpinBox : public QSpinBox  
{
	Q_OBJECT
	
public: 
	MSpinBox(QWidget *pa, int s);
	~MSpinBox() {};
	double value();
	double minValue();
	double maxValue();
	QString mapValueToText(int value);
	int mapTextToValue(bool *ok);
	int Decimals;
	int Width;
	QLineEdit *ed;
	void setDecimals( int deci );

public slots:
	void setMaxValue(double val);
	void setMinValue(double val);
	void setValue(double val);

protected:
	bool eventFilter( QObject* ob, QEvent* ev );
};

#endif
