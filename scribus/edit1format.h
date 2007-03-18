/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef EDITSTYLE_H
#define EDITSTYLE_H

#include <qdialog.h>
#include <q3buttongroup.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qcheckbox.h>
#include <qcombobox.h>
//Added by qt3to4:
#include <Q3GridLayout>
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>
#include <Q3PopupMenu>

#include "scribusapi.h"
#include "scribusstructs.h"
#include "scrspinbox.h"
#include "fontcombo.h"
#include "colorcombo.h"
#include "alignselect.h"
#include "shadebutton.h"
#include "styles/styleset.h"

class ScribusDoc;
class StyleSelect;
class Tabruler;
class SampleItem;
class PrefsContext;


/*! \brief GUI dialog for Paragraph Style setting.
*/
class SCRIBUS_API EditStyle : public QDialog
{
	Q_OBJECT

public:
	EditStyle( QWidget* parent, ParagraphStyle *vor, const StyleSet<ParagraphStyle>& v, bool neu,
	           double au, int dEin, ScribusDoc *doc);
	~EditStyle();

	QLabel* TextLabel1;
	QLabel* TextLabel1_2_2;
	QLabel* TextLabel1_2_3;
	QLabel* pixmapLabel3;
	QLabel* pixmapLabel3_3;
	QLabel* pixmapLabel2;
	QLabel* pixmapLabel3_2;
	QLabel* pixmapLabel3_20;
	QLabel* pixmapLabel3_20_3;
	QLabel* pixmapLabel3_19;
	QLineEdit* Name;
	ScrSpinBox* fontHScale;
	ScrSpinBox* fontVScale;
	ScrSpinBox* LineSpVal;
	ScrSpinBox* fontBase;
	ScrSpinBox* fontKern;
	Q3PopupMenu* lineSpacingPop;
	QToolButton* linespacingButton;
	Tabruler* TabList;
	ScrSpinBox* AboveV;
	ScrSpinBox* BelowV;
	StyleSelect* EffeS;
	AlignSelect* AligS;
	Q3GroupBox* GroupBox10;
	Q3GroupBox* AbstandV;
	Q3GroupBox* GroupFont;
	FontComboH* FontC;
	ScrSpinBox* SizeC;
	QLabel* TextF2;
	QLabel* StrokeIcon;
	ColorCombo* TxStroke;
	ShadeButton *PM1;
	QLabel* FillIcon;
	ColorCombo* TxFill;
	ShadeButton *PM2;
	Q3GroupBox* DropCaps;
	QLabel* CapLabel;
	QSpinBox* DropLines;
	QLabel* CapLabel2;
	ScrSpinBox* DropDist;
	QPushButton* Cancel;
	QPushButton* OkButton;
	ParagraphStyle *werte;
	const StyleSet<ParagraphStyle>& allV;
	QString OldName;
	bool IsNew;
	double AutoVal;
	int DocsEin;
	PrefsContext* prefs;
	//! \brief Label for holding "style preview" bitmap 12/30/2004 petr vanek
	QLabel *previewText;
	QCheckBox *previewCaption;
	//! \brief Preview background
	QPushButton *previewBgColor;
	//! \brief Reference to the own ScribusDoc (preferences etc.) 12/30/2004 petr vanek
	ScribusDoc *parentDoc;

private slots:
	void toggleLsp(int id);
	void Verlassen();
	void FontChange();
	void ColorChange();

	/*! \brief Enables or disables the style previewer */
	void togglePreview();
	/*! \brief Paragraph style preview generator.
	It takes values from the UI form, creates a temporary style.
	This style is appended into global styles list and removed from there
	after finishing. Then is created an item to draw QPixmap with it.
	\author Petr Vanek
	\date 4/7/2005
	 */
	void updatePreview();

	/*! \brief Sets the preview bg with user chosen color. */
	void setPreviewBackground();
	
	/*! \brief Copies the chosen settings into a ParagraphStyle object */
	void copyStyleSettings(ParagraphStyle& parstyle);

protected:
	/*! \brief sample pixmap generator */
	SampleItem *sampleItem;

	Q3HBoxLayout* Layout17;
	Q3HBoxLayout* layout5;
	Q3HBoxLayout* layout6;
	Q3HBoxLayout* layout7;
	Q3VBoxLayout* layout8;
	Q3HBoxLayout* layout9;
	Q3HBoxLayout* layout9a;
	Q3HBoxLayout* layout9b;
	Q3VBoxLayout* layoutPreview;
	Q3HBoxLayout* layoutPrevSet;
	Q3VBoxLayout* EditStyleLayout;
	Q3VBoxLayout* GroupBox10Layout;
	Q3GridLayout* AbstandVLayout;
	Q3GridLayout* DropCapsLayout;
	Q3VBoxLayout* GroupFontLayout;
};

#endif // EDITSTYLE_H
