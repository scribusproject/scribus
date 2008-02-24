/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCSLAINFOREADER_H
#define SCSLAINFOREADER_H

#include <QString>

class ScSlaInfoReader
{
protected:
	QString m_title;
	QString m_author;
	QString m_format;

	void resetFileInfos(void);

public:
	ScSlaInfoReader(void) {};

	const QString& title(void)  { return m_title; }
	const QString& author(void) { return m_author; }
	const QString& format(void) { return m_format; }

	bool readInfos(const QString& fileName);
};

#endif
