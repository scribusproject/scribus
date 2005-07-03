/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef LANGLIST_H
#define LANGLIST_H

#include <qmap.h>
#include <qstring.h>

class LangList : public QMap<QString, QString>
{
public:
	LangList();

};

#endif

