#ifndef SWPREFSGUI_H
#define SWPREFSGUI_H

#include "prefspanel.h"

#include <q3textedit.h>
#include <q3syntaxhighlighter.h>
//Added by qt3to4:
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

class QWidget;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QPushButton;
class QLabel;
class QString;
class PrefsPanel;


/*! \brief This is the Scribus Short Words plugin configuration widget.
Used in Preferences dialog.
\author Petr Vanek <petr@yarpen.cz>
*/
class SWPrefsGui : public PrefsPanel
{
	Q_OBJECT

	public:
		SWPrefsGui(QWidget* parent);
		~SWPrefsGui(){};

		QLabel* titleLabel;
		Q3TextEdit* cfgEdit;
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
class SWSyntaxHighlighter : public Q3SyntaxHighlighter
{
	public:
		SWSyntaxHighlighter(Q3TextEdit *textEdit);

		/*! Reimplementation of the Qt highligtion for simple cfg file
		\param text string (one row) provided by text editor via QSyntaxHighlighter inheritance.
		\param endStateOfLastPara how is the syntax left for next paragraph
		 */
		int highlightParagraph(const QString &text, int endStateOfLastPara);
};

#endif
