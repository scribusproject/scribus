/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef _LOREMIPSUM_H_
#define _LOREMIPSUM_H_

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QListView;
class QListViewItem;
class QLabel;
class QSpinBox;
class QCheckBox;
class QPushButton;
class QString;
class QStringList;

/*! \brief This module provides simple interface to the various Lorem Ipsum variants.
Scribus LI are stored in the set of XML files.
The file has followind structure:
				   <?xml version="1.0" encoding="utf8"?>
				   <lorem>

				   <name>name to display</name>
				   <author>person name</author>
				   <url>web interface</url>

				   <p>text</p>
				   <p>...</p>
				   </lorem>
Files are in UTF encoding.
\author Petr Vanek <petr@yarpen.cz>
*/

/*! \brief XML parser for Lorem Ipsum file.
This helper class reads one ile
\author Petr Vanek <petr@yarpen.cz>
 */
class LoremParser
{
	public:
		/*! parse a XML file with given name
		\param filename string fith the file name with full/relative path */
		LoremParser(QString filename);
		~LoremParser(){};

		//! Is the LI correct?
		bool correct;
		//! Name to display
		QString name;
		//! Author of the LI
		QString author;
		//! Website of the LI
		QString url;
		//! Paragraphs of the LI
		QStringList loremIpsum;

		/*! Construct a LI
		\param parCount count of the paragraphs */
		QString createLorem(uint parCount);
};


class LoremManager : public QDialog
{
	Q_OBJECT

	public:
		/*! Reads all XML files in cfg directory. */
		LoremManager(QWidget* parent = 0, const char* name = 0, bool modal = true, WFlags fl = 0);
		~LoremManager(){};

		//! all lorems with Public Name -> filename structure
		QMap<QString,QString> availableLorems;

		/*! Apply created LI into a frame
		\param name filename of the selected LI
		\param paraCount count of the paragraphs
		*/
		void insertLoremIpsum(QString name, int paraCount);

	public slots:
		virtual void okButton_clicked();
		virtual void cancelButton_clicked();

	protected:
		QListView* loremList;
		QLabel* paraLabel;
		QSpinBox* paraBox;
		QPushButton* okButton;
		QPushButton* cancelButton;

		QGridLayout* LoremManagerLayout;
		QVBoxLayout* layout3;
		QHBoxLayout* layout2;
		QSpacerItem* paraSpacer;
		QHBoxLayout* layout1;
		QSpacerItem* buttonSpacer;

	protected slots:
		virtual void languageChange();

};

#endif
