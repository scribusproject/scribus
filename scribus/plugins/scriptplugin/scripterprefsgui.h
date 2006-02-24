/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCRIPTERPREFSGUI_H
#define SCRIPTERPREFSGUI_H

#include "cmdvar.h"
#include "qvariant.h"
#include "prefspanel.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QTabWidget;
class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QToolButton;

class PrefsPanel;


/*! \brief Subclass of PrefsPanel that's supplied to the prefs
dialog for use when showing plugin preferences. */
class ScripterPrefsGui : public PrefsPanel
{
	Q_OBJECT

	public:
		ScripterPrefsGui(QWidget* parent);
		~ScripterPrefsGui();

	public slots:
		void apply();

	protected:
		QTabWidget* tabWidget;
		QWidget* startupTab;
		QWidget* consoleTab;
		QCheckBox* extensionScriptsChk;
		QLabel* startupScriptEditLabel;
		QLineEdit* startupScriptEdit;

		QGridLayout* ScripterPrefsGuiBaseLayout;
		QVBoxLayout* extLayout;
		QSpacerItem* extScriptSpacer;
		QHBoxLayout* startupScriptLayout;

		// console colors
		QLabel* errorLabel;
		QLabel* commentLabel;
		QLabel* keywordLabel;
		QLabel* signLabel;
		QLabel* numberLabel;
		QLabel* stringLabel;
		QLabel* textLabel;
		QPushButton* errorButton;
		QPushButton* commentButton;
		QPushButton* keywordButton;
		QPushButton* signButton;
		QPushButton* numberButton;
		QPushButton* stringButton;
		QPushButton* textButton;
		QToolButton* startupScriptChangeButton;
		QGridLayout* colorLayout;

		void setupSyntaxColors();
	protected slots:
		virtual void languageChange();
		/*! \brief All requests for color change are handled here.
		\author Petr Vanek
		\warning I'm trying to handle multiple signals via single slot here. sender() Returns a pointer to the object that sent the signal, if called in a slot activated by a signal; otherwise the return value is undefined. This function will return something apparently correct in other cases as well. However, its value may change during any function call, depending on what signal-slot connections are activated during that call. In Qt 3.0 the value will change more often than in 2.x. This function violates the object-oriented principle of modularity. However, getting access to the sender might be useful when many signals are connected to a single slot. The sender is undefined if the slot is called as a normal C++ function. */
		virtual void setColor();
		virtual void changeStartupScript();
};

#endif
