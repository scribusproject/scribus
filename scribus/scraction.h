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

#include <qaction.h>


/**
@author Craig Bradney
*/
class ScrAction : public QAction
{
	Q_OBJECT;
public:
	enum MenuType {Normal, DataInt, DataDouble, DataQString, RecentFile, DLL, Window, RecentScript };
	
	/*!
		\fn ScrAction::ScrAction( QObject * parent, const char * name )
		\author Craig Bradney
		\date Jan 2005
		\brief Constructor from QAction, sets menuType to Normal
		\param parent Parent object of this action
		\param name Name of the action
		\retval None
	*/
	ScrAction ( QObject *parent, const char *name = 0 ) ;
		
	/*!
		\fn ScrAction::ScrAction( const QString & menuText, QKeySequence accel, QObject * parent, const char * name )
		\author Craig Bradney
		\date Jan 2005
		\brief Constructor from QAction, sets menuType to normal
		\param menuText Text to be in the menus for this action
		\param accel Accelerator QKeySequence
		\param parent Parent object of this action
		\param name Name of the action
		\retval None
	*/
	ScrAction( const QString &menuText, QKeySequence accel, QObject *parent, const char *name = 0 );
		
	/*!
		\fn ScrAction::ScrAction( MenuType mType, const QIconSet & icon, const QString & menuText, QKeySequence accel, QObject * parent, const char * name, int extraInt = 0, double extraDouble = 0.0, QString extraQString = QString::null )
		\author Craig Bradney
		\date Jan 2005
		\brief Constructor for an action that may require a specific menu type, such as a DLL menu
		\param mType menuType, of Normal, RecentFile or DLL
					\param icon Iconset for the action
		\param menuText Text to be in the menus for this action
		\param accel Accelerator QKeySequence
		\param parent Parent of this action
		\param name Name of the action
		\retval None
	 */
	ScrAction( MenuType mType, const QIconSet & icon, const QString &menuText, QKeySequence accel, QObject *parent, const char *name = 0, int extraInt = 0, double extraDouble = 0.0, QString extraQString = QString::null );		
	/*!
		\fn ScrAction::ScrAction( const QIconSet & icon, const QString & menuText, QKeySequence accel, QObject * parent, const char * name )
		\author Craig Bradney
		\date Jan 2005
		\brief Constructor for a normal action. Stores iconset.
		\param icon Iconset for the action
		\param menuText Text to be in the menus for this action
		\param accel Accelerator QKeySequence
		\param parent Parent of this action
		\param name Name of the action
		\retval None
	*/			
	ScrAction( const QIconSet & icon, const QString & menuText, QKeySequence accel, QObject *parent, const char * name = 0 );
	
	
	/*!
		\fn ScrAction::~ScrAction()
		\author Craig Bradney
		\date Jan 2005
		\brief Default destructor
		\param None
		\retval None
	*/
    ~ScrAction();
	
	/*!
		\fn ScrAction::getMenuIndex()
		\author Craig Bradney
		\date Jan 2005
		\brief Return the stored menuIndex as QActions cannot deal with these well
		\param None
		\retval int Index of menu the action was inserted into
	*/	
	const int getMenuIndex();
			
	/*!
		\fn ScrAction::addTo ( QWidget * w )
		\author Craig Bradney
		\date Jan 2005
		\brief Overriden addTo() call for storing addedTo widget
		\param w Widget to add the action to
		\retval bool Success or failure of addition
	 */
	bool addTo(QWidget *w);
	
	/*!
		\fn ScrAction::getWidgetAddedTo()
		\author Craig Bradney
		\date Jan 2005
		\brief Return a pointer to the widget we added this action too. Given actions can be added to many, this might need some controls.
		\param None
		\retval QWidget * Pointer to widget
	*/
	QWidget *getWidgetAddedTo();
			
	/*!
		\fn ScrAction::cleanMenuText()
		\author Craig Bradney
		\date Jan 2005
		\brief Return a copy of the menu text, without the &s, possibility to remove ellipses in future.
		\param None
		\retval QString Stripped copy of the menu text
	*/
	QString cleanMenuText();

	/*!
		\fn ScrAction::isDLLAction()
		\author Craig Bradney
		\date Jan 2005
		\brief Return true if action is a DLL
		\param None
		\retval bool True if action is from a DLL
	 */
	const bool isDLLAction();
	
	/*!
		\fn ScrAction::dllID()
		\author Craig Bradney
		\date Jan 2005
		\brief Return DLL ID if the action is from a DLL, otherwise return -1
		\param None
		\retval int DLL ID or -1
	 */
	const int dllID();
	
signals:
	void activatedData(int);
	void activatedData(double);
	void activatedData(QString);
	void toggledData(bool, int);
	void toggledData(bool, double);
	void toggledData(bool, QString);
	
	protected:
	int menuIndex;
	int pluginID;
	int windowID;
	int _dataInt;
	double _dataDouble;
	QString _dataQString;
	MenuType menuType;
	QWidget *widgetAddedTo;
	QWidget *containerWidgetAddedTo;
	QPopupMenu *popupMenuAddedTo;
	/*!
		\fn ScrAction::addedTo ( int index, QPopupMenu * menu )
		\author Craig Bradney
		\date Jan 2005
		\brief Overridden addedTo QPopupmenu based function to save a pointer to the QPopupMenu we are adding to.
		\param index The saved index within the QPopupMenu
		\param menu The menu we are adding this action to
		\retval None
	*/
	void addedTo( int index, QPopupMenu * menu );
			
	/*!
		\fn ScrAction::addedTo( QWidget * actionWidget, QWidget * container )
		\author Craig Bradney
		\date Jan 2005
		\brief Overridden addedTo QWidget based function to save a pointer to the widget we are adding to and its container widget.
		\param actionWidget Widget action is added to
		\param container Container widget of actionWidget
		\retval None
	*/
	void addedTo( QWidget * actionWidget, QWidget * container );
	
	/*!
		\fn ScrAction::setIconSizes()
		\author Craig Bradney
		\date Jan 2005
		\brief Set the icon size for the actions that have them assigned. 16x16 for menus (small), 22x22 for toolbars.
		\param None
		\retval None
	*/
	void setIconSizes();
			
private slots:
	/*!
		\fn ScrAction::activatedtoactivatedData()
		\author Craig Bradney
		\date Jan 2005
		\brief This passed the activated() action signal in and back out but with some data, easier than 	overriding menu and menu bar classes for now.
		\param None
		\retval None
	 */
	void activatedToActivatedData();
	/*!
		\fn ScrAction::toggledToToggledData()
		\author Craig Bradney
		\date Jan 2005
		\brief This passed the toggled() action signal in and back out but with some data, easier than 	overriding menu and menu bar classes for now.
		\param None
		\retval None
	 */
	void toggledToToggledData();
};

#endif
