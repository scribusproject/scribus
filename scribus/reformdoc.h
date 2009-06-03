/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef REFORMDOC_H
#define REFORMDOC_H

#include <QLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QRadioButton>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QToolButton>
#include <QColor>

#include "scribusapi.h"
#include "scribusstructs.h"
#include "prefsdialogbase.h"

class QSpinBox;
class DocSections;
class ScribusDoc;
class ScribusMainWindow;
class TabTypograpy;
class HySettings;
class CMSPrefs;
class ScrSpinBox;
class DocInfos;
class TabGuides;
class TabTools;
class TabCheckDoc;
class TabPDFOptions;
class FontPrefs;
class DocumentItemAttributes;
class TOCIndexPrefs;
class MarginWidget;
class PageLayouts;
class TabDisplay;
class TabDocument;


class SCRIBUS_API ReformDoc : public PrefsDialogBase
{
	Q_OBJECT

public:
	ReformDoc( QWidget* parent, ScribusDoc* doc );
	~ReformDoc() {};
	
	const int getSelectedUnit();
	const bool imageResolutionChanged();
	const bool colorManagementSettingsChanged();
	void updateDocumentSettings();	

	ScribusMainWindow *ScMW;
	ScribusDoc* currDoc;
	int docUnitIndex;
	int decimals;
	double unitRatio;

protected:
	QStringList docAttributesList;
	
	TabDocument* tabPage;
	DocInfos* docInfos;
	DocSections* tabDocSections;
	TabGuides* tabGuides;
	TabDisplay* tabView;
	TabTypograpy* tabTypo;
	TabTools* tabTools;
	HySettings* tabHyphenator;
	FontPrefs* tabFonts;
	TabCheckDoc* tabDocChecker;
	CMSPrefs* tabColorManagement;
	TabPDFOptions* tabPDF;
	DocumentItemAttributes* tabDocItemAttributes;
	TOCIndexPrefs* tabTOCIndexPrefs;
	
	bool viewToRecalcPictureRes;

protected slots:
	virtual void restoreDefaults();
	virtual void unitChange();
	virtual void setDS(int layout);
	virtual void switchCMS(bool enable);
	virtual void showWidgetInStack(QWidget *widgetToShow);
	virtual void applyChangesButton_clicked();
};

#endif // REFORMDOC_H
