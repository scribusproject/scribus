/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef PREFS_TABLEOFCONTENTS_H
#define PREFS_TABLEOFCONTENTS_H

#include <QStringList>
#include <QListWidget>
#include <QListWidgetItem>

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
		Prefs_TableOfContents(QWidget* parent, ScribusDoc* doc = nullptr);
		~Prefs_TableOfContents();

		void restoreDefaults(struct ApplicationPrefs *prefsData) override;
		void saveGuiToPrefs(struct ApplicationPrefs *prefsData) const override;

		void changeEvent(QEvent *e) override;
		virtual void enableGUIWidgets();

	public slots:
		void languageChange();

//		virtual void setup( ToCSetupVector * tocsetups, ScribusDoc * doc );
		virtual void generatePageItemList();
		virtual void setupItemAttrs(const QStringList& newNames );
		virtual void selectToC( int numberSelected );
		virtual void addToC();
		virtual void updateToCListBox();
		virtual void updateParagraphStyleComboBox();
		virtual void updateDocParagraphStyleComboBox();
		virtual void deleteToC();
		virtual void tocSourceSelected( const QString & tocSource );
		virtual void itemAttributeSelected( const QString & itemAttributeName );
		virtual void itemFrameSelected( const QString & frameName );
		virtual void itemPageNumberPlacedSelected( const QString & pageLocation );
		virtual void itemParagraphStyleSelected( const QString & itemStyle );
		virtual void setToCName( const QString & newName );
		void nonPrintingFramesSelected(bool showNonPrinting);
		void removeLineBreaksSelected(bool removeLineBreaks);
		void styleListUpdate();
		void styleListTOCUpdate();
		void addStyleClicked();
		void removeStyleClicked();
		void addStyleTOCClicked();
		void removeStyleTOCClicked();
		void tocListWidgetItemEdited(QListWidgetItem *qlwi);
		void tocStyleMoveUp();
		void tocStyleMoveDown();
		void tocEntryStyleMoveUp();
		void tocEntryStyleMoveDown();
		void styleListWidgetClicked();
		void styleListTOCWidgetClicked();


	protected:
		int numSelected { 999 };
		QString strPNNotShown;
		QString strPNEnd;
		QString strPNBeginning;
		QString strTOCSrcStyle;
		QString strTOCSrcAttribute;
		QString trStrPNNotShown;
		QString trStrPNEnd;
		QString trStrPNBeginning;
		QString trStrTOCSrcStyle;
		QString trStrTOCSrcAttribute;
		ToCSetupVector localToCSetupVector;
		ScribusDoc* m_Doc { nullptr };
		QString selectedTOCAttrName;
		QStringList paragraphStyleList;

	private:
		void destroy();

};

#endif // PREFS_TABLEOFCONTENTS_H
