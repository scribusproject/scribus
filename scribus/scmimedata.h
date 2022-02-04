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

	static bool clipboardHasScribusData();
	static bool clipboardHasScribusElem();
	static bool clipboardHasScribusFragment();
	static bool clipboardHasScribusText();
	static bool clipboardHasPlainText();
	static bool clipboardHasKnownData();
	static QString clipboardKnownDataExt();
	static QByteArray clipboardKnownDataData();

	static QString clipboardScribusElem();
	static QByteArray clipboardScribusFragment();
	static QByteArray clipboardScribusText();
};

class SCRIBUS_API ScFragmentMimeData : public QMimeData
{
	Q_OBJECT

public:
	ScFragmentMimeData() : QMimeData() {}

	void setScribusFragment(const QByteArray& fragment) { setData(ScMimeData::ScribusFragmentMimeType, fragment); }
	QByteArray scribusFragment() { return data(ScMimeData::ScribusFragmentMimeType); }
};

class SCRIBUS_API ScElemMimeData : public QMimeData
{
	Q_OBJECT

public:
	ScElemMimeData();

	QStringList formats() const override { return m_formats; }
	bool hasFormat (const QString & mimeType) const override;

	void  setScribusElem(const QString& elem) { m_scribusElemData = elem; }
	const QString& scribusElem(void) const    { return m_scribusElemData; }

protected:
	QString m_scribusElemData;
	QStringList m_formats;

	QVariant retrieveData(const QString & mimeType, QVariant::Type type) const override;
};

class SCRIBUS_API ScTextMimeData : public QMimeData
{
	Q_OBJECT

public:
	ScTextMimeData() : QMimeData() {}

	void setScribusText(const QByteArray& scText) { setData(ScMimeData::ScribusTextMimeType, scText); }
	QByteArray scribusText() { return data(ScMimeData::ScribusFragmentMimeType); }
};

#endif
