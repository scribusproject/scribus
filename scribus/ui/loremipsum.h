/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
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

#include <QDialog>
#include <QMap>

#include "scribusapi.h"

#include <QDialog>
#include <QMap>
#include <QString>
#include <QStringList>
class QCheckBox;
class QEvent;
class QGridLayout;
class QHBoxLayout;
class QLabel;
class QPushButton;
class QSpacerItem;
class QSpinBox;
class QTreeWidget;
class QVBoxLayout;

class ScribusDoc;
class UndoManager;
// class LanguageManager;


/*! \brief XML parser for Lorem Ipsum file.
This helper class reads one file
\author Petr Vanek <petr@yarpen.cz>
 */
class SCRIBUS_API LoremParser
{
public:
	/*! parse a XML file with given name
	\param filename string of the file name with full/relative path */
	LoremParser(const QString& filename);
	~LoremParser(){};

	//! Is the LI correct?
	bool correct {false};
	//! Name to display
	QString name {"n/a"};
	//! Author of the LI
	QString author {"n/a"};
	//! Website of the LI
	QString url {"n/a"};
	//! Paragraphs of the LI
	QStringList loremIpsum;

	/*! Construct a LI
	\param parCount count of the paragraphs */
	QString createLorem(uint parCount, bool random = true);
};

/*! \brief This module provides simple interface to the various Lorem Ipsum variants.
Scribus LI are stored in the set of XML files.
The file has the following structure:
\<?xml version="1.0" encoding="utf8"?\>
\<lorem\>
\<name\>name to display\</name\>
\<author\>person name\</author\>
\<url\>web interface\</url\>
\<p\>text\</p\>
\<p\>...\</p\>
\</lorem\>
Files are in UTF8 encoding.
\author Petr Vanek <petr@yarpen.cz>
*/
class SCRIBUS_API LoremManager : public QDialog
{
	Q_OBJECT

public:
	/*! Reads all XML files in cfg directory. */
	LoremManager(ScribusDoc* doc, QWidget* parent = nullptr);

	/*! Apply created LI into a frame
	\param name filename of the selected LI
	\param paraCount count of the paragraphs
	*/
	void insertLoremIpsum(const QString& name, int paraCount, bool random = true);

	//! Lorem Ipsum as QString
	QString loremIpsum() const;

	//! Count of the paragraphs selected in GUI
	int paragraphCount() const;

	//! Name of the Lorem Ipsum file including extension.
	QString getName() const;

	//! Flag if use "random" paragraphs, taken from GUI.
	bool randomize() const;

protected:
	void changeEvent(QEvent *e) override;

private:
	QCheckBox* appendCheckBox {nullptr};
	QCheckBox* randomCheckBox {nullptr};
	QGridLayout* LoremManagerLayout {nullptr};
	QHBoxLayout* layout1 {nullptr};
	QHBoxLayout* layout2 {nullptr};
	QLabel* paraLabel {nullptr};
	QPushButton* cancelButton {nullptr};
	QPushButton* okButton {nullptr};
	QSpacerItem* buttonSpacer {nullptr};
	QSpacerItem* paraSpacer {nullptr};
	QSpinBox* paraBox {nullptr};
	QString standardloremtext;
	QTreeWidget* loremList {nullptr};
	QVBoxLayout* layout3 {nullptr};
	ScribusDoc* m_Doc {nullptr};
	UndoManager* undoManager {nullptr};

	//! all lorems with Public Name -> filename structure
	QMap<QString, QString> availableLorems;

protected slots:
	void languageChange();

};

#endif
