#ifndef SWPREFSGUI_H
#define SWPREFSGUI_H

#include "prefspanel.h"

class QWidget;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QTextEdit;
class QPushButton;
class QLabel;
class QString;
class PrefsPanel;


/*! \brief This is the Scribus Short Words plugin configuration widget.
Used in Preferences dialog.
TODO: expanding cfgEdit. "Syntax Highlighting"
\author Petr Vanek <petr@yarpen.cz>
*/
class SWPrefsGui : public PrefsPanel
{
	Q_OBJECT

	public:
		SWPrefsGui(QWidget* parent);
		~SWPrefsGui(){};

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


#endif
