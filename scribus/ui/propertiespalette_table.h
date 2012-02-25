/*
Copyright (C) 2011 Elvis Stansvik <elvstone@gmail.com>

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PROPERTIESPALETTE_TABLE_H
#define PROPERTIESPALETTE_TABLE_H

#include <QColor>
#include <QString>
#include <QWidget>

#include "scribusapi.h"
#include "scguardedptr.h"
#include "tableborder.h"

#include "ui_propertiespalette_tablebase.h"

class PageItem;
class ScribusMainWindow;
class ScribusDoc;

/**
 * Widget for table properties in the Properties Palette.
 */
class SCRIBUS_API PropertiesPalette_Table : public QWidget, Ui::PropertiesPalette_TableBase
{
	Q_OBJECT

public:
	explicit PropertiesPalette_Table(QWidget *parent = 0);
	~PropertiesPalette_Table() {}

	/// Update the list of colors.
	void updateColorList();

public slots:
	/// Set the main window to @a mainWindow.
	void setMainWindow(ScribusMainWindow* mainWindow);
	/// Set the current document to @a doc.
	void setDocument(ScribusDoc* doc);
	/// Unset the current document.
	void unsetDocument();
	/// Set the current item to @a item.
	void setItem(PageItem* item);
	/// Unset the current item.
	void unsetItem();

	/// Handles item selection changes.
	void handleSelectionChanged();
	/// Handles cell selection changes.
	void handleCellSelectionChanged();
	/// Handles update requests from the main window.
	void handleUpdateRequest(int updateFlags);
	/// Handles language changes.
	void languageChange();
	/// Handles unit changes.
	void unitChange();
	/// Updates the fill controls.
	void updateFillControls();
	void updateStyleControls();

private slots:
	void setTableStyle(const QString& name);
	void setCellStyle(const QString& name);
	/// Handles selection changes in the side selector.
	void on_sideSelector_selectionChanged();
	/// Handles selection changes in the list of border lines.
	void on_borderLineList_currentRowChanged(int row);
	/// Handles adding of a new border line.
	void on_addBorderLineButton_clicked();
	/// Handles removing of a border line.
	void on_removeBorderLineButton_clicked();
	/// Handles border line shade changes.
	void on_borderLineShade_valueChanged(int shade);
	/// Handles border line width changes.
	void on_borderLineWidth_valueChanged(double width);
	/// Handles border line color changes.
	void on_borderLineColor_activated(const QString& colorName);
	/// Handles border line style changes.
	void on_borderLineStyle_activated(int style);

	/// Handles table fill color changes.
	void on_fillColor_activated(const QString& colorName);
	/// Handles table fill shade changes.
	void on_fillShade_valueChanged(int shade);
	void on_buttonClearTableStyle_clicked();
	void on_buttonClearCellStyle_clicked();

private:
	/// This enum represents three states. Used for three-state logic.
	enum State
	{
		Unset,
		Set,
		TriState
	};

private:
	void displayTableStyle(const QString& name);
	void displayCellStyle(const QString& name);
	/// Updates the list of border lines from the current border.
	void updateBorderLineList();
	/// Updates the current item in the list of border lines.
	void updateBorderLineListItem();
	/// Updates the selected table with the current border.
	void updateBorders();
	/// Returns the color with name @a colorName and shade @a shade as a QColor.
	QColor getColor(const QString& colorName, int shade) const;

private:
	/// The current main window.
	ScribusMainWindow* m_mainWindow;
	/// The current document.
	ScGuardedPtr<ScribusDoc> m_doc;
	/// The currently edited item.
	PageItem* m_item;
	/// The previous edited selected items.
	PageItem* m_previousItem;

	/// The currently edited border.
	TableBorder m_currentBorder;
};

#endif // PROPERTIESPALETTE_TABLE_H
