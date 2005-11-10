#ifndef SCRIPTERPREFSGUI_H
#define SCRIPTERPREFSGUI_H

#include "cmdvar.h"
#include "qvariant.h"
//Added by qt3to4:
#include <QLabel>
#include <QVBoxLayout>
#include <Q3Frame>
#include <QHBoxLayout>
#include <QGridLayout>
#include "prefspanel.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class Q3GroupBox;
class QCheckBox;
class Q3Frame;
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
		Q3GroupBox* extGroup;
		QCheckBox* extensionScriptsChk;
		Q3Frame* startupScriptFrame;
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
