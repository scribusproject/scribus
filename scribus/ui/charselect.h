/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef CHARSELECT_H
#define CHARSELECT_H

#include "scribusapi.h"
#include "scrpalettebase.h"
#include "chartablemodel.h"
#include "unicodesearch.h"
#include "ui_charselect.h"

class PageItem;
class CharSelectEnhanced;


/*! \brief Character Palette for direct glyphs inserting.
The "classical" one is opened as CharSelectEnhanced instance.
Both CharSelect/CharSelectEnhanced palettes are split due:
1) initial and usage performace. The CharSelectEnhanced is too
slow to load on startup/doc opening etc. The memory footprint
is much smaller too.
2) It was one dialog before split. To show/hide "enhanced" part
caused serious mess with layouts and dialog sizes and positioning.
Now it's handled in standard way.

\note This palette is used as standard Scribus palette for ScribusDoc mainly.
\note There is 2nd usage as well - in the StoryEditor. See its docstrings.
*/
class SCRIBUS_API CharSelect : public ScrPaletteBase, public Ui::CharSelect
{
	Q_OBJECT

public:
	CharSelect(QWidget* parent);
	~CharSelect();

	void hide();
	void show();
	void setDoc(ScribusDoc* doc);
	void setEnabled(bool state, PageItem* item=0);

	const QString & getCharacters();

	//! Used for StoryEditor
	CharTableModel * userTableModel() { return m_userTableModel; }

	virtual void changeEvent(QEvent *e);

signals:
	/*! \brief A signall emittedthis palette tries to insert glyphs.
	It does not have sense in standard "palette" mode, but it's used
	in StoryEditor workaround. */
	void insertSpecialChar();
	/*! Internal signal for one glyph only */
	void insertUserSpecialChar(QChar, QString);


private:
	ScribusDoc* m_doc {nullptr};
	//! \brief m_userTable model
	CharTableModel * m_userTableModel {nullptr};
	UnicodeSearchModel *m_unicodeSearchModel {nullptr};
	CharSelectEnhanced * m_enhanced {nullptr};

	//! Hold the "glyphs to insert" here.
	QString chToIns;
	QString paletteFileMask;
	PageItem *m_Item {nullptr};

	void saveUserContent(const QString& f);
	void loadUserContent(const QString& f);

	void openEnhanced();
	void closeEnhanced();

private slots:
	void iconSetChange();
	void enhancedDialogButton_toggled(bool);
	void slot_insertSpecialChar();
	void slot_insertSpecialChars(const QVector<uint> & chars);
	void slot_insertUserSpecialChar(QChar, const QString& font);
	void slot_selectionChanged( const QItemSelection&, const QItemSelection& );
	void uniClearButton_clicked();
	void uniLoadButton_clicked();
	void uniSaveButton_clicked();
	void userNewChar(uint i, const QString& font);

};

#endif
