/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

/***************************************************************************
    begin                : Jan 2005
    copyright            : (C) 2005 by Craig Bradney
    email                : cbradney@zip.com.au
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef SCRACTION_H
#define SCRACTION_H

#include <QAction>
#include <QPixmap>
#include <QMenu>
#include <QString>

#include "scribusapi.h"
class ScribusDoc;
/**
@author Craig Bradney
*/
class SCRIBUS_API ScrAction : public QAction
{
	Q_OBJECT

public:
	typedef enum {Normal, DataInt, DataDouble, DataQString, RecentFile, DLL, Window, RecentScript, UnicodeChar, Layer, ActionDLL, RecentPaste, ActionDLLSE } ActionType;
	
	/*!
		\author Craig Bradney
		\date Jan 2005
		\brief Constructor from QAction, sets menuType to Normal
		\param parent Parent object of this action
	*/
	ScrAction (QObject *parent) ;
		
	/*!
		\author Craig Bradney
		\date Jan 2005
		\brief Constructor from QAction, sets menuType to normal
		\param menuText Text to be in the menus for this action
		\param accel Accelerator QKeySequence
		\param parent Parent object of this action
	*/
	ScrAction(const QString &menuText, QKeySequence accel, QObject *parent);
		
	/*!
		\author Craig Bradney
		\date Jan 2005
		\brief Constructor for an action that may require a specific menu type, such as a DLL menu
		\param mType menuType, of Normal, RecentFile or DLL
		\param icon16Path path of icon for the action
		\param icon22Path path of icon for the action
		\param menuText Text to be in the menus for this action
		\param accel Accelerator QKeySequence
		\param parent Parent of this action
		\param extraInt extra int value
		\param extraDouble extra double value
		\param extraQString extra QString value
	 */
	ScrAction(ActionType aType, const QString &menuText, QKeySequence accel, QObject *parent, QVariant d = QVariant());
	ScrAction(ActionType aType, const QPixmap& icon16, const QPixmap& icon22, const QString &menuText, QKeySequence accel, QObject *parent, QVariant d = QVariant());
	ScrAction(ActionType aType, const QString& icon16Path, const QString& icon22Path, const QString &menuText, QKeySequence accel, QObject *parent, QVariant d = QVariant());
	/*!
		\author Craig Bradney
		\date Jan 2005
		\brief Constructor for a normal action. Stores iconset.
		\param icon path of icon for the action
		\param menuText Text to be in the menus for this action
		\param accel Accelerator QKeySequence
		\param parent Parent of this action
	*/
	ScrAction(const QString& icon16Path, const QString& icon22Path, const QString & menuText, QKeySequence accel, QObject *parent);
	/*!
	\author Craig Bradney
	\date Mar 2008
	\brief Constructor for a Unicode action.
	\param accel Accelerator QKeySequence
	\param parent Parent of this action
	\param extraInt extra int value
	\param extraQString extra QString value
	 */
	ScrAction(QKeySequence accel, QObject *parent, QVariant data = QVariant());
	~ScrAction();
	
	/*!
		\author Craig Bradney
		\date Jan 2005
		\brief Return the stored menuIndex as QActions cannot deal with these well
		\retval int Index of menu the action was inserted into
	*/	
	int getMenuIndex() const;
			
	/*!
		\author Craig Bradney
		\date Jan 2005
		\brief Return a copy of the menu text, without the &s, possibility to remove ellipses in future.
		\retval QString Stripped copy of the menu text
	*/
	QString cleanMenuText();

	void setToolTipFromTextAndShortcut();
	void setStatusTextAndShortcut(const QString& statusText);

	/*!
		\author Craig Bradney
		\date Jan 2005
		\brief Return true if action is a DLL
		\retval bool True if action is from a DLL
	 */
	bool isDLLAction() const;
	
	/*!
		\author Craig Bradney
		\date Jan 2005
		\brief Return DLL ID if the action is from a DLL, otherwise return -1
		\retval int DLL ID or -1
	 */
	int dllID() const;
	
	/*!
	 *	\author Craig Bradney
	 *	\date Feb 2005
	 *	\brief Connect the internal toggle connections. Fake toggle actions are toggle actions
	 *         but we connect activated() only, eg itemLock. This means they can be setOn()
	 *         to the status of an item's bool, eg isLocked(), without toggling anything.
	 */
	void setToggleAction(bool isToggle, bool m_fakeToggle = false);
	
	/*!
		\author Craig Bradney
		\date Feb 2005
		\brief Store the shortcut in a private variable as these must be removed in editmode
	 */		
	void saveShortcut();
	/*!
		\author Craig Bradney
		\date Feb 2005
		\brief Restore the saved shortcut to be the active one
	 */		
	void restoreShortcut();	
	/*!
		\author Craig Bradney
		\date Apr 2005
		\brief Return the action data type
	 */		
	ActionType actionType();
	/*!
		\author Craig Bradney
		\date Apr 2005
		\brief Return the action data of int action type
	 */		
	int actionInt() const;
	double actionDouble() const;
	const QString actionQString();
	void setActionQString(const QString &);
	
	/*! \brief Set up text and menuText at the same time */
	void setTexts(const QString &newText);//#9114, qt3-qt4 change of behaviour bug: , bool setTextToo = true);
	
public slots:
	void toggle();
	
signals:
	void triggeredData(QVariant);
	void triggeredData(int);
	void triggeredData(double);
	void triggeredData(QString);
	void triggeredData(ScribusDoc*);
	void triggeredData(QWidget*, ScribusDoc*);
	void toggledData(bool, int);
	void toggledData(bool, double);
	void toggledData(bool, QString);
	void triggeredUnicodeShortcut(int);
	
protected:
	int m_menuIndex;
	ActionType m_actionType;
	QMenu *m_popupMenuAddedTo;
	QKeySequence m_savedKeySequence;
	bool m_shortcutSaved;
	bool m_fakeToggle;

	QString m_iconPath16;
	QString m_iconPath22;

	void initScrAction();
	/*!
		\author Craig Bradney
		\date Jan 2005
		\brief Overridden addedTo QPopupmenu based function to save a pointer to the QPopupMenu we are adding to.
		\param index The saved index within the QPopupMenu
		\param menu The menu we are adding this action to
	*/
	void addedTo(int index, QMenu* menu);
				
private slots:
	/*!
		\author Jean Ghali
		\date Feb 2020
		\brief Reload action icon following an icon set change for eg.
	 */
	void loadIcon();
	/*!
		\author Craig Bradney
		\date Jan 2005
		\brief This passed the triggered() action signal in and back out but with some data, easier than 	overriding menu and menu bar classes for now.
	 */
	void triggeredToTriggeredData();
	/*!
		\author Craig Bradney
		\date Jan 2005
		\brief This passed the toggled() action signal in and back out but with some data, easier than 	overriding menu and menu bar classes for now.
		\param ison true if toggled = on
	 */
	void toggledToToggledData(bool ison);
};

#endif
