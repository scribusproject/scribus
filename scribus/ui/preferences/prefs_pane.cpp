/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QCheckBox>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>

#include "prefs_pane.h"

const QString& Prefs_Pane::caption() const
{
	return m_caption;
}

const QString& Prefs_Pane::icon() const
{
	return m_icon;
}

QStringList Prefs_Pane::keywords(bool reset)
{
	if (reset)
	{
		m_keywords.clear();
		m_keywordsGenerated = false;
	}

	if (m_keywordsGenerated)
		return m_keywords;

	for (const QCheckBox *checkBox : findChildren<QCheckBox *>())
		m_keywords.append(checkBox->text());
	for (const QGroupBox *groupBox : findChildren<QGroupBox *>())
		m_keywords.append(groupBox->title());
	for (const QLabel *label : findChildren<QLabel *>())
		m_keywords.append(label->text());
	for (const QPushButton *pushButton : findChildren<QPushButton *>())
		m_keywords.append(pushButton->text());

	m_keywordsGenerated = true;
	return m_keywords;
}
