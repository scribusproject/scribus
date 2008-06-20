/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef STYLEITEM_H
#define STYLEITEM_H

#include <QObject>
#include <QList>
#include <QPair>

class QString;
class QStringList;
class ScribusDoc;
class QTabWidget;

/** RemoveItem.first will be the style to remove and RemoveItem.second
 *  will be the one used in place of the deleted style */
typedef QPair<QString, QString> RemoveItem;

/** StyleName.first is the name of the style and StyleName.second is the
 *  name of the parent style or QString::null if there's no parent for this style */
typedef QPair<QString, QString> StyleName;

/**
 * @brief Represents a style type that can be added to the Style Manager
 * @brief palette.
 *
 * This class is ment to be used as a parent class for any style types
 * that are wanted to be configured using the Style Manager palette.
 * @author Riku Leino <riku@scribus.info>
 * @date November 2005
 */
class StyleItem : public QObject {
	Q_OBJECT
public:
	StyleItem();
	virtual ~StyleItem();

	/** 
	 * @brief return the QWidget for editing style's properties
	 *
	 * This is the widget where the attributes of style are edited. It
	 * will be placed on the main tab widget's Properties page.
	 * Whenever a selected style is edited StyleItem should emit the
	 * signal selectionDirty() if it is the first time style is edited
	 * after applying previous changes.
	 */
	virtual QTabWidget* widget() = 0;

	/** @brief name of the style (plural) (Paragraph Styles, Character Styles...) */
	virtual QString typeNamePlural() = 0;

	/** @brief name of the style (singular) (Paragraph Style, Character Style...) */
	virtual QString typeNameSingular() = 0;

	/**
	 * @brief Whenever style manager is shown attached StyleItems get the current doc
	 * @brief passed to them with this function.
	 */
	virtual void setCurrentDoc(ScribusDoc *doc) = 0;

	/**
	 * @brief existing styles in this category
	 *
	 * return the names of cached styles (even if those are changed)
	 * @param reloadFromDoc should the styles be loaded from the document or
	 * should function return currently cached and possibly changed styles
	 * @return Name of the styles and their parent as a QValueList.
	 * StyleName::first is the style name and StyleName::second it's parent's name.
	 * If the StyleItem has no parent StyleName::second should be set to QString::null.
	 */
	virtual QList<StyleName> styles(bool reloadFromDoc = true) = 0;

	/** 
	 * @brief Reload styles and remove all cached (and possibly changed ones).
	 *
	 * This function will be called whenever a user clicks on the reset button.
	 * Resetting styles means loading them from the doc replacing all cached styles
	 * and without applying changes to the previously cached styles.
	 **/
	virtual void reload() = 0;

	/**
	 * @brief Whenever this function is called StyleItem must update the main
	 * @brief widget with the corresponding data.
	 *
	 * Whenever user selects a style from the style list in the manager
	 * this slot is called from the StyleItem. StyleItem must then update
	 * the main widget with the data related to the selected item and then
	 * just wait for apply() to apply the changes user made (with the
	 * main widget). When implementing this method one should note that
	 * there may be more than a single style selected.
	 * @param styleNames styles selected for editing
	 */
	virtual void selected(const QStringList &styleNames) = 0;

	/**
	 * @brief Return the name of the style in this category applied to the
	 * @brief selected object(s) in the current document or QString::null
	 * @brief if there is no selection in the document.
	 *
	 * If there are multiple objects selected only return a style name if the same style
	 * is applied on all selected objects. If they doesn't share the same style then
	 * return QString::null.
	 */
	virtual QString fromSelection() const = 0;

	/** 
	 * @brief Apply style called styleName to the documents current selection
	 * @param styleName name of the style to be applied to the documents current selection
	 */
	virtual void toSelection(const QString &styleName) const = 0;

	/**
	 * @brief Create a new temp. style with default values and return the
	 * @brief name of the newly created style.
	 * @return name of the created style
	 */
	virtual QString newStyle() = 0;

	/**
	 * @brief Create a new temp. style which is a clone of the style called fromStyle.
	 * @param fromStyle name of the style to clone
	 * @return name of the created style
	 */
	virtual QString newStyle(const QString &fromStyle) = 0;

	/**
	 * @brief apply changes made to the currently selected style(s)
	 *
	 * When a user has edited a style (s)he can either cancel or apply
	 * changes. On cancel and ok button clicks no action is needed from
	 * the StyleItem but if a user chooses to press button apply StyleItem's
	 * function apply() is called and StyleItem must upgrade the style
	 * and apply it where ever that style is used in the document.
	 * (cancel will be disabled after this)
	 */
	virtual void apply() = 0;

	/**
	 * @brief When SM switches to or away from edit mode this function is called
	 * @param isOn true if SM is in edit mode false if SM has just closed edit mode
	 */
	virtual void editMode(bool isOn) {};

	/** @brief returns if the style is a default style */
	virtual bool isDefaultStyle(const QString &stylename) const = 0;

	/** @brief set if the style is a default style */
	virtual void setDefaultStyle(bool ids) = 0;
	
	/** @brief returns the key combination for the style's shortcut */
	virtual QString shortcut(const QString &stylename) const = 0;

	/** @brief set the key combination for the selected style's shortcut */
	virtual void setShortcut(const QString &shortcut) = 0;

	/**
	 * @brief User has requested to delete all the selected styles
	 * @param removeList list of styles to be deleted. RemoveItem::first is
	 * the style to be deleted and RemoveItem::second is the style to replace
	 * the deleted style with. If no replacement was requested RemoveItem::second
	 * has been set to QString::null.
	 */
	virtual void deleteStyles(const QList<RemoveItem> &removeList) = 0;

	/** @brief Called when the currently selected style's name has changed */
	virtual void nameChanged(const QString &newName) = 0;

	/** @brief Return a unique name for a style using the base in the name */
	virtual QString getUniqueName(const QString &base) = 0;

	/** @brief reload all the gui strings whenever this method is called */
	virtual void languageChange() = 0;

	/** @brief document unit has been changed, upgrade widgets  */
	virtual void unitChange() = 0;

	// do not implement this in derived classes
	QString typeName() { return typeNamePlural(); };

/*
	Emit this signal when selection has been edited. SM knows to highlight the
	edited styles based on this signal.
signals:
	selectionDirty();
*/

private:
	/* hide these two, StyleItem is not ment to be copied */
	StyleItem(const StyleItem&);
	void operator=(const StyleItem&);

};

#endif
