/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef PREFS_TABLEOFCONTENTS_H
#define PREFS_TABLEOFCONTENTS_H

#include <QStringList>

#include "ui_prefs_tableofcontentsbase.h"
//#include "page.h"
#include "pagestructs.h"
#include "prefs_pane.h"
#include "prefsstructs.h"
#include "scribusapi.h"

class ScribusDoc;

class SCRIBUS_API Prefs_TableOfContents : public Prefs_Pane, Ui::Prefs_TableOfContents
{
	Q_OBJECT

	public:
		Prefs_TableOfContents(QWidget* parent, ScribusDoc* doc=NULL);
		~Prefs_TableOfContents();
		virtual void restoreDefaults(struct ApplicationPrefs *prefsData);
		virtual void saveGuiToPrefs(struct ApplicationPrefs *prefsData) const;
		virtual void changeEvent(QEvent *e);
		virtual void enableGUIWidgets();

	public slots:
		void languageChange();

//		virtual void setup( ToCSetupVector * tocsetups, ScribusDoc * doc );
		virtual void generatePageItemList();
		virtual void setupItemAttrs( QStringList newNames );
		virtual void selectToC( int numberSelected );
		virtual void addToC();
		virtual void updateToCListBox();
		virtual void updateParagraphStyleComboBox();
		virtual void deleteToC();
		virtual void itemAttributeSelected( const QString & itemAttributeName );
		virtual void itemFrameSelected( const QString & frameName );
		virtual void itemPageNumberPlacedSelected( const QString & pageLocation );
		virtual void itemParagraphStyleSelected( const QString & itemStyle );
		virtual void setToCName( const QString & newName );
		virtual void nonPrintingFramesSelected( bool showNonPrinting );

	protected:
		int numSelected;
		QString strPNNotShown;
		QString strPNEnd;
		QString strPNBeginning;
		QString trStrPNNotShown;
		QString trStrPNEnd;
		QString trStrPNBeginning;
		ToCSetupVector localToCSetupVector;
		ScribusDoc* m_Doc;
		QString selectedTOCAttrName;
		QStringList paragraphStyleList;

	private:
		void destroy();

};

#endif // PREFS_TABLEOFCONTENTS_H
