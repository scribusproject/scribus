/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SMFONTCOMBOH_H
#define SMFONTCOMBOH_H

#include "fontcombo.h"


class SMFontComboH  : public FontComboH
{
	Q_OBJECT
public:
	SMFontComboH(QWidget *parent);
	~SMFontComboH() {};
	
	void setCurrentFont(const QString &s);
	void setCurrentFont(const QString &s, bool isParentValue);

	void setParentFont(const QString &s);
	bool useParentFont();

private:
	bool    m_hasParent = false;
	bool    m_useParentValue = false;
	QString m_parentFont;
	QString m_useParentFont;

	void setFont(bool wantBold);

private slots:
	void currentChanged();
	void checkStyle();
};

#endif
