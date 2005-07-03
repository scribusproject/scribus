/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef TXTIM_H
#define TXTIM_H

#include <scribus.h>
#include "gtwriter.h"

extern "C" void GetText(QString filename, QString encoding, bool textOnly, gtWriter *writer);

extern "C" QString FileFormatName();

extern "C" QStringList FileExtensions();

class TxtIm 
{
public:
	TxtIm(const QString& fname, const QString& enc, gtWriter *w);
	~TxtIm();
	void write();
private:
	QString filename;
	QString encoding;
	QString text;
	gtWriter *writer;
	void loadText();
	void toUnicode();
};

#endif // TXTIM_H
