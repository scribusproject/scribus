#ifndef SCRIPTERPREFSGUI_H
#define SCRIPTERPREFSGUI_H

#include "cmdvar.h"
#include "qvariant.h"
#include "prefspanel.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QGroupBox;
class QCheckBox;
class QFrame;
class QLabel;
class QLineEdit;
class PrefsPanel;

// Subclass of PrefsPanel that's supplied to the prefs
// dialog for use when showing plugin preferences.
class ScripterPrefsGui : public PrefsPanel
{
	Q_OBJECT

	public:
		ScripterPrefsGui(QWidget* parent);
		~ScripterPrefsGui();

	public slots:
		void apply();

	protected:
		QGroupBox* extGroup;
		QCheckBox* extensionScriptsChk;
		QFrame* startupScriptFrame;
		QLabel* startupScriptEditLabel;
		QLineEdit* startupScriptEdit;

		QGridLayout* ScripterPrefsGuiBaseLayout;
		QVBoxLayout* extGroupLayout;
		QSpacerItem* extScriptSpacer;
		QHBoxLayout* startupScriptFrameLayout;

	protected slots:
		virtual void languageChange();
};

#endif
