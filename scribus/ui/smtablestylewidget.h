/*
Copyright (C) 2011 Elvis Stansvik <elvstone@gmail.com>

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SMTABLESTYLEWIDGET_H
#define SMTABLESTYLEWIDGET_H

#include <QWidget>

#include "ui_smtablestylewidget.h"
#include "styles/tablestyle.h"

/**
 * Widget for editing table style attributes.
 *
 * NOTE: Many attributes unsupported.
 */
class SMTableStyleWidget : public QWidget, public Ui::SMTableStyleWidget
{
	Q_OBJECT
public:
	/// Constructor.
	SMTableStyleWidget(QWidget *parent = 0);
	/// Destructor.
	~SMTableStyleWidget();

	/**
	 * Shows attributes for a single table style.
	 *
	 * @param tableStyle table style for which attributes should be shown.
	 * @param tableStyles list of all table styles.
	 * @param defaultLanguage default language.
	 * @param unitIndex index of currently used unit.
	 */
	void show(TableStyle *tableStyle, QList<TableStyle> &tableStyles, const QString &defaultLanguage, int unitIndex);

	/**
	 * Shows attributes for multiple table styles.
	 *
	 * TODO: Implement actual support for multiple styles.
	 *
	 * @param tableStyles list of table styles for which attributes should be shown.
	 * @param tableStylesAll list of all table styles.
	 * @param defaultLanguage default language.
	 * @param unitIndex index of currently used unit.
	 */
	void show(QList<TableStyle*> &tableStyles, QList<TableStyle> &tableStylesAll, const QString &defaultLanguage, int unitIndex);

	/**
	 * This function is called when the language is changed.
	 */
	void languageChange();

	/**
	 * Populates the fill color combo with the given color list.
	 *
	 * @param colors list of colors to populate the combo with.
	 */
	void fillFillColorCombo(ColorList &colors);
};

#endif // SMTABLESTYLEWIDGET_H
