/*
 For general Scribus (>=1.3.2) copyright and licensing information please refer
 to the COPYING file provided with the program. Following this notice may exist
 a copyright and/or license notice that predates the release of Scribus 1.3.2
 for which a new license (GPL+exception) is in place.
 */
/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/



#ifndef SCMIMEDATA_H
#define SCMIMEDATA_H

#include <QByteArray>
#include <QMimeData>
#include <QString>
#include <QStringList>
#include <QVariant>

#include "scribusapi.h"

class SCRIBUS_API ScMimeData
{
public:
	static const QString ScribusElemMimeType;
	static const QString ScribusFragmentMimeType;
	static const QString ScribusTextMimeType;

	static bool clipboardHasScribusData(void);
	static bool clipboardHasScribusElem(void);
	static bool clipboardHasScribusFragment(void);
	static bool clipboardHasScribusText(void);
	static bool clipboardHasPlainText(void);

	static QString    clipboardScribusElem(void);
	static QByteArray clipboardScribusFragment(void);
	static QByteArray clipboardScribusText(void);
};

class SCRIBUS_API ScFragmentMimeData : public QMimeData
{
public:
	ScFragmentMimeData(void) : QMimeData() {};

	void setScribusFragment(const QByteArray& fragment) { setData(ScMimeData::ScribusFragmentMimeType, fragment); }
	QByteArray scribusFragment(void) { return data(ScMimeData::ScribusFragmentMimeType); }
};

class SCRIBUS_API ScElemMimeData : public QMimeData
{
protected:
	QString     m_scribusElemData;
	QStringList m_formats;

	virtual QVariant retrieveData ( const QString & mimeType, QVariant::Type type ) const;

public:
	ScElemMimeData(void);

	virtual QStringList formats() const { return m_formats; }
	virtual bool hasFormat ( const QString & mimeType ) const;

	void  setScribusElem(const QString& elem) { m_scribusElemData = elem; }
	const QString& scribusElem(void) const    { return m_scribusElemData; }
};

class SCRIBUS_API ScTextMimeData : public QMimeData
{
public:
	ScTextMimeData(void) : QMimeData() {};

	void setScribusText(const QByteArray& scText) { setData(ScMimeData::ScribusTextMimeType, scText); }
	QByteArray scribusText(void) { return data(ScMimeData::ScribusFragmentMimeType); }
};

#endif
