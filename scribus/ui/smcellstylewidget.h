/*
Copyright (C) 2011 Elvis Stansvik <elvstone@gmail.com>

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SMCELLSTYLEWIDGET_H
#define SMCELLSTYLEWIDGET_H

#include <QWidget>

#include "ui_smcellstylewidget.h"
#include "styles/cellstyle.h"

/**
 * Widget for editing cell style attributes.
 *
 * NOTE: Many attributes unsupported.
 */
class SMCellStyleWidget : public QWidget, public Ui::SMCellStyleWidget
{
	Q_OBJECT
public:
	/// Constructor.
	SMCellStyleWidget(QWidget *parent = 0);
	/// Destructor.
	~SMCellStyleWidget();
	virtual void changeEvent(QEvent *e);

	/**
	 * Shows attributes for a single cell style.
	 *
	 * @param cellStyle cell style for which attributes should be shown.
	 * @param cellStyles list of all cell styles.
	 * @param defaultLanguage default language.
	 * @param unitIndex index of currently used unit.
	 */
	void show(CellStyle *cellStyle, QList<CellStyle> &cellStyles, const QString &defaultLanguage, int unitIndex);

	/**
	 * Shows attributes for multiple cell styles.
	 *
	 * TODO: Implement actual support for multiple styles.
	 *
	 * @param cellStyles list of cell styles for which attributes should be shown.
	 * @param cellStylesAll list of all cell styles.
	 * @param defaultLanguage default language.
	 * @param unitIndex index of currently used unit.
	 */
	void show(QList<CellStyle*> &cellStyles, QList<CellStyle> &cellStylesAll, const QString &defaultLanguage, int unitIndex);

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
	void showColors(const QList<CellStyle*> &cellStyles);
};

#endif // SMCELLSTYLEWIDGET_H
