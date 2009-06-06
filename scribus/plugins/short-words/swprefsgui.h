/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SWPREFSGUI_H
#define SWPREFSGUI_H

#include <QSyntaxHighlighter>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QPushButton;
class QLabel;
class QTextEdit;
class QSpacerItem;

#include "ui/prefspanel.h"

/*! \brief This is the Scribus Short Words plugin configuration widget.
Used in Preferences dialog.
\author Petr Vanek <petr@yarpen.cz>
*/
class SWPrefsGui : public PrefsPanel
{
	Q_OBJECT

	public:
		SWPrefsGui(QWidget* parent);
		~SWPrefsGui();

		QLabel* titleLabel;
		QTextEdit* cfgEdit;
		QPushButton* okButton;
		QPushButton* resetButton;

	public slots:
		//! \brief Apply changes to prefs. Auto connected.
		void apply();
		/*! \brief Save the content into user file. */
		virtual void okButton_pressed();
		/*! \brief Re-reads system wide config file. */
		virtual void resetButton_pressed();
		/*! \brief Text editor changed. */
		virtual void cfgEdit_changed();

	protected:
		/*! \brief Load cfg file.
		\param filename string with full path and name.*/
		bool loadCfgFile(QString filename);

		QGridLayout* SWPrefsGuiLayout;
		QVBoxLayout* editLayout;
		QHBoxLayout* buttonLayout;
		QSpacerItem* buttonSpacer;

	protected slots:
		virtual void languageChange();
};

/*! Simple syntax highlighting for configuration editor (QTextEdit).
\author Petr Vanek, <petr@yarpen.cz>
 */
class SWSyntaxHighlighter : public QSyntaxHighlighter
{
	public:
		SWSyntaxHighlighter(QTextEdit *textEdit);

		/*! Reimplementation of the Qt highligtion for simple cfg file
		\param text string (one row) provided by text editor via QSyntaxHighlighter inheritance.
		 */
		void highlightBlock(const QString &text);
};

#endif
