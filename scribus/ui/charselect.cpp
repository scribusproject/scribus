/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QFileDialog>
#include <QMessageBox>

#include "appmodes.h"
#include "charselect.h"
#include "charselectenhanced.h"
#include "commonstrings.h"
#include "pageitem_table.h"
#include "pageitem_textframe.h"
#include "prefsmanager.h"
#include "scpaths.h"
#include "scribusapp.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "ui/scmessagebox.h"
#include "util.h"
#include "iconmanager.h"

CharSelect::CharSelect(QWidget* parent) : ScrPaletteBase(parent, "CharSelect")
{
	setupUi(this);

	paletteFileMask = tr("Scribus Char Palette (*.ucp);;All Files (*)");

	iconSetChange();

	m_userTableModel = new CharTableModel(this, 6, m_doc, PrefsManager::instance().appPrefs.itemToolPrefs.textFont);
	loadUserContent(ScPaths::applicationDataDir() + "charpalette.ucp");

	m_unicodeSearchModel = new UnicodeSearchModel(this);

	m_userTable->setModel(m_userTableModel);
	m_userTable->setAcceptDrops(true);

	// signals and slots connections
	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));

	connect(m_userTable, SIGNAL(selectChar(uint,QString)), this, SLOT(userNewChar(uint,QString)));
	connect(m_userTable->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&)), this, SLOT(slot_selectionChanged(const QItemSelection&,const QItemSelection&)));
	connect(m_userTableModel, SIGNAL(rowAppended()),m_userTable, SLOT(resizeLastRow()));
	connect(unicodeButton, SIGNAL(chosenUnicode(const QString&)), m_userTableModel, SLOT(appendUnicode(const QString&)));
	connect(enhancedDialogButton, SIGNAL(toggled(bool)), this, SLOT(enhancedDialogButton_toggled(bool)));
	connect(this, SIGNAL(insertUserSpecialChar(QString,QString)), this, SLOT(slot_insertUserSpecialChar(QString,QString)));
	connect(uniLoadButton, SIGNAL(clicked()), this, SLOT(uniLoadButton_clicked()));
	connect(uniSaveButton, SIGNAL(clicked()), this, SLOT(uniSaveButton_clicked()));
	connect(uniClearButton, SIGNAL(clicked()), this, SLOT(uniClearButton_clicked()));
}

CharSelect::~CharSelect()
{
	delete m_unicodeSearchModel;
}

void CharSelect::iconSetChange()
{
	enhancedDialogButton->setIcon(IconManager::instance().loadIcon("16/insert-table.png"));
	unicodeButton->setIcon(IconManager::instance().loadIcon("find.png"));
	uniLoadButton->setIcon(IconManager::instance().loadIcon("16/document-open.png"));
	uniSaveButton->setIcon(IconManager::instance().loadIcon("16/document-save.png"));
	uniClearButton->setIcon(IconManager::instance().loadIcon("16/document-new.png"));
}

void CharSelect::setDoc(ScribusDoc* doc)
{
//     tDebug("setDoc start");
	if (m_doc != doc)
	{
		m_doc = doc;
		m_userTableModel->setDoc(m_doc);
	}

	if (!m_doc)
	{
		saveUserContent(ScPaths::applicationDataDir() + "charpalette.ucp");
		return;
	}

	m_userTableModel->setFontInUse(m_doc->currentStyle.charStyle().font().scName());
//     tDebug("setDoc end");
}

const QString& CharSelect::getCharacters() const
{
	return chToIns;
}

void CharSelect::userNewChar(uint i, const QString& font)
{
	QString str = QString::fromUcs4(&i, 1);
	emit insertUserSpecialChar(str, font);
}

void CharSelect::slot_insertSpecialChars(const QVector<uint> & chars)
{
	chToIns = QString::fromUcs4(chars.data(), chars.length());
	slot_insertSpecialChar();
}

void CharSelect::slot_insertSpecialChar()
{
	emit insertSpecialChar();

	if (!m_Item)
		return;
	PageItem_TextFrame *cItem;
	if (m_doc->appMode == modeEditTable)
		cItem = m_Item->asTable()->activeCell().textFrame();
	else
		cItem = m_Item->asTextFrame();
	if (cItem->HasSel)
		cItem->deleteSelectedTextFromFrame();
	cItem->invalidateLayout(false);
	//CB: Avox please make text->insertchar(char) so none of this happens in gui code, and item can tell doc its changed so the view and mainwindow slotdocch are not necessary
	QString fontName = m_doc->currentStyle.charStyle().font().scName();
	if (m_enhanced)
		fontName = m_enhanced->getUsedFont();
	for (QChar ch : chToIns)
	{
		if (ch == QChar(10))
			ch = QChar(13);
		if (ch == QChar(9))
			ch = QChar(32);
		int pos = cItem->itemText.cursorPosition();
		cItem->itemText.insertChars(ch, true);
		CharStyle nstyle = m_Item->itemText.charStyle(pos);
		nstyle.setFont((*m_doc->AllFonts)[fontName]);
		cItem->itemText.applyCharStyle(pos, 1, nstyle);
	}
	m_doc->view()->DrawNew();
	m_doc->changed();
}

void CharSelect::slot_insertUserSpecialChar(QString str, const QString& font)
{
	if (!m_Item)
		return;
	PageItem_TextFrame *cItem;
	if (m_doc->appMode == modeEditTable)
		cItem = m_Item->asTable()->activeCell().textFrame();
	else
		cItem = m_Item->asTextFrame();
	if (cItem->HasSel)
		cItem->deleteSelectedTextFromFrame();
	cItem->invalidateLayout(false);
// 	//CB: Avox please make text->insertchar(char) so none of this happens in gui code, and item can tell doc its changed so the view and mainwindow slotdocch are not necessary
	for (QChar ch : str)
	{
		if (ch == QChar(10))
			ch = QChar(13);
		if (ch == QChar(9))
			ch = QChar(32);
		int pos = cItem->itemText.cursorPosition();
		cItem->itemText.insertChars(ch, true);
		CharStyle nstyle = m_Item->itemText.charStyle(pos);
		nstyle.setFont((*m_doc->AllFonts)[font]);
		cItem->itemText.applyCharStyle(pos, 1, nstyle);
	}
	m_doc->view()->DrawNew();
	m_doc->changed();
}

void CharSelect::slot_selectionChanged( const QItemSelection& sel, const QItemSelection& )
{
	QModelIndexList indexes = sel.indexes();
	if ( indexes.isEmpty() )
		return;

	QModelIndex index = indexes.at(0);
	if ( !index.isValid() )
		return;

	// we retrieve a QStringList of two elements from data() encapsulated in a
	// QVariant, so we should make sure first that we can convert to QStringList.
	// Otherwise clear the status label
	QVariant tmp = index.data(Qt::StatusTipRole);
	if ( !tmp.canConvert(QMetaType::QStringList) )
	{
		m_statusLabel->setText(QString());
		return;
	}

	QStringList lst = tmp.toStringList();
	m_statusLabel->setText("<p><b>" + QString("%1").arg(m_unicodeSearchModel->descriptionFromHex(lst.at(0))).toUpper() +
												 "</b><br>" + QString(tr("Font: %2")).arg(lst.at(1)) + "</p>");
}

void CharSelect::openEnhanced()
{
	if (m_enhanced)
		return;

	QApplication::changeOverrideCursor(QCursor(Qt::WaitCursor));
	m_enhanced = new CharSelectEnhanced(this);
	connect(m_enhanced, SIGNAL(insertSpecialChars(const QVector<uint> &)), this, SLOT(slot_insertSpecialChars(const QVector<uint> &)));
	connect(m_enhanced, SIGNAL(paletteShown(bool)), enhancedDialogButton, SLOT(setChecked(bool)));
	m_enhanced->setDoc(m_doc);
	m_enhanced->setEnabled(this->isEnabled());
	m_enhanced->show();
	QApplication::changeOverrideCursor(Qt::ArrowCursor);
}

void CharSelect::closeEnhanced()
{
	if (!m_enhanced)
		return;

	enhancedDialogButton->blockSignals(true);
	enhancedDialogButton->setChecked(false);
	enhancedDialogButton->blockSignals(false);

	disconnect(m_enhanced, SIGNAL(insertSpecialChars(const QVector<uint> &)), this, SLOT(slot_insertSpecialChars(const QVector<uint> &)));
	disconnect(m_enhanced, SIGNAL(paletteShown(bool)), enhancedDialogButton, SLOT(setChecked(bool)));
	m_enhanced->close();
	m_enhanced->deleteLater();
	m_enhanced = nullptr;
}

void CharSelect::enhancedDialogButton_toggled(bool state)
{
//     tDebug("enhancedDialogButton_toggled start");
	if (!m_doc)
		return;

	if (m_enhanced && !state)
		closeEnhanced();
	else
		openEnhanced();

//     tDebug("enhancedDialogButton_toggled end");
}

void CharSelect::hide()
{
	closeEnhanced();
	ScrPaletteBase::hide();
}

void CharSelect::show()
{
	ScrPaletteBase::show();
	m_userTable->resize(m_userTable->size());
}

void CharSelect::setEnabled(bool state, PageItem* item)
{
	ScrPaletteBase::setEnabled(state);
	m_Item = item;
	if (state)
		setDoc(m_doc);

	if (m_enhanced)
		m_enhanced->setEnabled(state);
}

void CharSelect::uniLoadButton_clicked()
{
	QString f = QFileDialog::getOpenFileName(this, tr("Open Character Palette"), QDir::currentPath(), paletteFileMask);
	if (!f.isEmpty())
		loadUserContent(f);
}

void CharSelect::loadUserContent(const QString& f)
{
	QFile file(f);
	if (!file.exists())
		return;
	if (!file.open(QIODevice::ReadOnly))
		return;

	QTextStream stream(&file);
	QString line = stream.readLine();
	if (line != "# Character palette file for Scribus")
	{
		file.close();
		return;
	}

	m_userTableModel->setCharacters(CharClassDef());
	while (!stream.atEnd())
	{
		bool ok = false;
		line = stream.readLine();
		if (line.left(1) == "#")
			continue; // don't mess with a comment
		int a = line.indexOf(" ");
		QString si = line.left(a);
		si.toInt(&ok, 10);
		if (ok)
			m_userTableModel->addCharacter(line);
		else
		{
			ScMessageBox::warning(this, tr("Error"),
				                    "<qt>" + tr("Error reading file %1 - file is probably corrupted.").arg(f) + "</qt>",
				                    QMessageBox::Ok, QMessageBox::NoButton);
			break;
		}
	}
	file.close();
}

void CharSelect::uniSaveButton_clicked()
{
	if (m_userTableModel->characters().count() == 0)
		return;
	QString f = QFileDialog::getSaveFileName(this, tr("Save Quick Character Palette"), QDir::currentPath(), paletteFileMask);
	if (f.isEmpty())
		return;
	if (!f.endsWith(".ucp"))
		f += ".ucp";
//#9832: Qt does this for us now in getSaveFileName
//	if (!overwrite(this, f))
//		return;
	saveUserContent(f);
}

void CharSelect::saveUserContent(const QString& f)
{
	QFile file(f);
	if (!file.open(QIODevice::WriteOnly))
	{
		ScMessageBox::warning(this, tr("Error"),
			"<qt>" + tr("Cannot write file %1").arg(f) + "</qt>",
			QMessageBox::Ok, QMessageBox::NoButton);
		return;
	}

	QTextStream stream(&file);
	CharClassDef chars = m_userTableModel->characters();
	QStringList fonts = m_userTableModel->fonts();
	stream << "# Character palette file for Scribus\n";
	for (int i = 0; i < chars.count(); ++i)
	{
		stream << chars[i] << " " << fonts[i] << "\n";
	}
	file.close();
}

void CharSelect::uniClearButton_clicked()
{
	if (m_userTableModel->characters().count() > 0
	        &&
	        ScMessageBox::question(this, tr("Empty the Palette?"),
	                               "<qt>" + tr("You will remove all characters from this palette. Are you sure?") + "</qt>",
	                               QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes
	   )
	{
		m_userTableModel->setCharacters(CharClassDef());
		m_statusLabel->setText(QString());
	}
}

void CharSelect::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
		Ui::CharSelect::retranslateUi(this);
	else
		QWidget::changeEvent(e);
}
