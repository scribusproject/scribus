#include <QMessageBox>

#include "notesstyleseditor.h"
#include "pageitem_noteframe.h"
#include "prefsmanager.h"
#include "prefsfile.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "scribus.h"
#include "undomanager.h"

NotesStylesEditor::NotesStylesEditor(QWidget *parent, const char *name)
	: ScrPaletteBase(parent, name), m_Doc(NULL)
{
	setupUi(this);
	QString pname(name);
	if (pname.isEmpty())
		pname = "notesStylesEditor";
	m_prefs = PrefsManager::instance()->prefsFile->getContext(pname);

	setBlockSignals(true);
	
	setDoc(0);
	languageChange();
	NSlistBox->setInsertPolicy(QComboBox::InsertAlphabetically);

	NumberingBox->addItem("1 2 3");
	NumberingBox->addItem("i ii iii");
	NumberingBox->addItem("I II III");
	NumberingBox->addItem("a b c");
	NumberingBox->addItem("A B C");
	NumberingBox->addItem("*");

	RangeBox->addItem(tr("Document"));
	RangeBox->addItem(tr("Section"));
	RangeBox->addItem(tr("Story"));
	RangeBox->addItem(tr("Page"));
	RangeBox->addItem(tr("Frame"));

	StartSpinBox->setMinimum(1);
	StartSpinBox->setMaximum(99999);
	changesMap.clear();
	addNewNsMode = false;

	setBlockSignals(isVisible());
}

NotesStylesEditor::~NotesStylesEditor()
{
	storeVisibility(this->isVisible());
	storePosition();
	storeSize();
}

void NotesStylesEditor::languageChange()
{
	bool wasSignalsBlocked = signalsBlocked();
	setBlockSignals(true);

	if (addNewNsMode)
	{
		OKButton->setText(tr("Cancel"));
		OKButton->setToolTip(tr("Dialog is in adding new notes style mode. After pressing Cancel button dialog will be switched into normal notes styles edit mode."));
		ApplyButton->setText(tr("Add Style"));
	}
	else
	{
		OKButton->setText("OK");
		OKButton->setToolTip("");
		ApplyButton->setText(tr("Apply"));
	}
	NewButton->setText(tr("Add New Style"));
    NewButton->setToolTip(tr("New notes style will be add to document only after pressing Apply button.\nYou cannot switch to different notes style before you apply that new one or press Cancel button and exit from adding mode."));
	NewNameLabel->setText(tr("New Style Name"));
	FootRadio->setText(tr("Footnotes"));
	EndRadio->setText(tr("Endnotes"));
	NumberingLabel->setText("Numbering");
	RangeLabel->setText(tr("Range"));
	StartLabel->setText(tr("Start Number"));
	PrefixLabel->setText(tr("Prefix"));
	SuffixLabel->setText(tr("Suffix"));
	SuperMasterLabel->setText(tr("Superscript in text"));
	SuperNoteLabel->setText(tr("Superscript in notes"));
	charStyleComboLabel->setText(tr("Mark`s Character Style"));
	paraStyleComboLabel->setText(tr("Note`s Paragraph Style"));
	AutoHLabel->setText(tr("Auto Height"));
	AutoWLabel->setText(tr("Auto Width"));
	AutoWeldLabel->setText(tr("Auto Welding"));
	AutoRemoveLabel->setText(tr("Remove if empty"));
	DeleteButton->setText(tr("Delete"));
	setBlockSignals(wasSignalsBlocked);
}

void NotesStylesEditor::setDoc(ScribusDoc *doc)
{
	bool wasSignalsBlocked = signalsBlocked();
	setBlockSignals(true);
	if (m_Doc != NULL)
		disconnect(m_Doc->scMW(), SIGNAL(UpdateRequest(int)), this , SLOT(handleUpdateRequest(int)));
	m_Doc = doc;
	paraStyleCombo->setDoc(m_Doc);
	charStyleCombo->setDoc(m_Doc);
	if (m_Doc != NULL)
	{
		updateNSList();
		NSlistBox->setCurrentIndex(0);
		readNotesStyle(NSlistBox->currentText());
		setEnabled(true);
		ApplyButton->setEnabled(false);
		connect(m_Doc->scMW(), SIGNAL(UpdateRequest(int)), this , SLOT(handleUpdateRequest(int)));
	}
	else
	{
		changesMap.clear();
		NewNameEdit->clear();
		setEnabled(false);
	}
	setBlockSignals(wasSignalsBlocked);
}

void NotesStylesEditor::handleUpdateRequest(int updateFlags)
{
	bool wasSignalsBlocked = signalsBlocked();
	setBlockSignals(true);
	if ((updateFlags & reqCharStylesUpdate) || (updateFlags & reqTextStylesUpdate))
		charStyleCombo->updateFormatList();
	if ((updateFlags & reqParaStylesUpdate) || (updateFlags & reqTextStylesUpdate))
		paraStyleCombo->updateFormatList();
	readNotesStyle(NSlistBox->currentText());
	setBlockSignals(wasSignalsBlocked);
}

void NotesStylesEditor::updateNSList()
{
	bool wasSignalsBlocked = signalsBlocked();
	NSlistBox->blockSignals(true);
	if (m_Doc == NULL)
		NSlistBox->setEnabled(false);
	else
	{
		NSlistBox->clear();
		changesMap.clear();
		for (int a = 0; a < m_Doc->m_docNotesStylesList.count(); ++a)
		{
			NSlistBox->addItem(m_Doc->m_docNotesStylesList.at(a)->name());
			changesMap.insert(m_Doc->m_docNotesStylesList.at(a)->name(), *(m_Doc->m_docNotesStylesList.at(a)));
		}
		if (!m_Doc->m_docNotesStylesList.isEmpty())
			NSlistBox->setEnabled(true);
		if (NSlistBox->currentText() != tr("default"))
			DeleteButton->setEnabled(true);
		else
			DeleteButton->setEnabled(false);
	}
	NSlistBox->blockSignals(wasSignalsBlocked);
	
	DeleteButton->setEnabled(NSlistBox->currentText() != tr("default"));
}

void NotesStylesEditor::setBlockSignals(bool block)
{
	foreach (QWidget* obj, findChildren<QWidget *>())
	{
		obj->blockSignals(block);
	}
	paraStyleCombo->blockSignals(block);
	charStyleCombo->blockSignals(block);
}

void NotesStylesEditor::setNotesStyle(NotesStyle * NS)
{
	if (NS == NULL)
		return;
	bool wasSignalsBlocked = signalsBlocked();
	setBlockSignals(true);
	NSlistBox->setCurrentIndex(NSlistBox->findText(NS->name()));
	NewNameEdit->setText(NS->name());
	if (NS->name() == tr("default"))
		NewNameEdit->setEnabled(false);
	else
		NewNameEdit->setEnabled(true);
	FootRadio->setChecked(!NS->isEndNotes());
	if (NS->range() == NSRframe)
		EndRadio->setEnabled(false);
	else
		EndRadio->setEnabled(true);
	EndRadio->setChecked(NS->isEndNotes());
	NumberingBox->setCurrentIndex((int) NS->getType());
	RangeBox->setCurrentIndex((int) NS->range());
	StartSpinBox->setValue(NS->start());
	PrefixEdit->setText(NS->prefix());
	SuffixEdit->setText(NS->suffix());
	SuperMasterCheck->setChecked(NS->isSuperscriptInMaster());
	SuperNoteCheck->setChecked(NS->isSuperscriptInNote());
	if (!NS->notesParStyle().isEmpty() && (NS->notesParStyle() != tr("No Style")))
		paraStyleCombo->setCurrentIndex(paraStyleCombo->findText(NS->notesParStyle()));
	if (!NS->marksChStyle().isEmpty() && (NS->marksChStyle() != tr("No Style")))
		charStyleCombo->setCurrentIndex(charStyleCombo->findText(NS->marksChStyle()));
	AutoH->setChecked(NS->isAutoNotesHeight());
	AutoW->setChecked(NS->isAutoNotesWidth());
	AutoWeld->setChecked(NS->isAutoWeldNotesFrames());
	//for endnotes remove FRAME range (same effect as footnotes for frame)
	//and disable autofixing size of notes frames
	int rangeFrameIndex = RangeBox->findText(tr("Frame"));
	if (NS->isEndNotes())
	{
		AutoW->setEnabled(false);
		AutoWeld->setEnabled(false);
		if (rangeFrameIndex >= 0)
			RangeBox->removeItem(rangeFrameIndex);
}
	else
	{
		AutoW->setEnabled(true);
		AutoWeld->setEnabled(true);
		if (rangeFrameIndex == -1)
			RangeBox->addItem(tr("Frame"));
	}
	AutoRemove->setChecked(NS->isAutoRemoveEmptyNotesFrames());

	ApplyButton->setEnabled(false);
	setBlockSignals(wasSignalsBlocked);
}

void NotesStylesEditor::readNotesStyle(QString nsName)
{
	NotesStyle * NS = m_Doc->getNotesStyle(nsName);
	setNotesStyle(NS);
}

void NotesStylesEditor::on_NSlistBox_currentIndexChanged(const QString &arg1)
{
	if (arg1 != tr("default"))
		DeleteButton->setEnabled(true);
	else
		DeleteButton->setEnabled(false);
	readNotesStyle(arg1);
}

void NotesStylesEditor::on_ApplyButton_clicked()
{
	if (addNewNsMode)
	{
		QString newName = NSlistBox->currentText();
		NotesStyle newNS = changesMap.value(newName);
		if (m_Doc->validateNSet(newNS))
		{
			addNewNsMode = false;
			OKButton->setText(tr("OK"));
			OKButton->setToolTip("");
			ApplyButton->setText(tr("Apply"));
			m_Doc->newNotesStyle(newNS);
			updateNSList();
			NSlistBox->setCurrentIndex(NSlistBox->findText(newNS.name()));
		}
		else
			return;
	}
	else
	{
		//remeber current NStyle
		QString currNS = NSlistBox->currentText();
		NotesStyle* NS = NULL;
		
		foreach (const QString &nsName, changesMap.keys())
		{
			NotesStyle n = changesMap.value(nsName);

			//validate settings
			if (!m_Doc->validateNSet(n))
			{
				NSlistBox->setCurrentIndex(NSlistBox->findText(n.name()));
				break;
			}
			//rename
			if (nsName != n.name())
			{
				//new name for existing set
				QString newName = n.name();
				getUniqueName(newName, changesMap.keys(),"=");
				n.setName(newName);
				NewNameEdit->setText(newName);
				//current NSet name change
				if (currNS == nsName)
					currNS = newName;
				NS = m_Doc->getNotesStyle(nsName);
				m_Doc->renameNotesStyle(NS, newName);
				m_Doc->setNotesChanged(true);
			}
			//change settings and update marks
			NS = m_Doc->getNotesStyle(n.name());
			Q_ASSERT(NS != NULL);
			if (*NS != n)
			{
				SimpleState* ss = NULL;
				if (UndoManager::instance()->undoEnabled())
				{
					ss = new SimpleState(UndoManager::EditNotesStyle);
					ss->set("NSTYLE", QString("edit"));
					m_Doc->undoSetNotesStyle(ss, NS);
				}
				//converting foot <--> end notes or changing footnotes range
				if ((NS->isEndNotes() != n.isEndNotes()) || (NS->isEndNotes() && n.isEndNotes() && NS->range() != n.range()))
				{
					foreach (PageItem_NoteFrame* nF, m_Doc->listNotesFrames(NS))
						m_Doc->delNoteFrame(nF, false);
					if (n.isEndNotes())
						m_Doc->flag_updateEndNotes = true;
				}
				m_Doc->setNotesChanged(true); //notesframes width must be updated
				*NS = n;
				if (ss)
				{
					ss->set("NEWname", NS->name());
					ss->set("NEWstart", NS->start());
					ss->set("NEWendNotes", NS->isEndNotes());
					ss->set("NEWnumFormat", (int) NS->getType());
					ss->set("NEWrange", (int) NS->range());
					ss->set("NEWprefix", NS->prefix());
					ss->set("NEWsuffix", NS->suffix());
					ss->set("NEWautoH", NS->isAutoNotesHeight());
					ss->set("NEWautoW", NS->isAutoNotesWidth());
					ss->set("NEWautoWeld", NS->isAutoWeldNotesFrames());
					ss->set("NEWautoRemove", NS->isAutoRemoveEmptyNotesFrames());
					ss->set("NEWsuperMaster", NS->isSuperscriptInMaster());
					ss->set("NEWsuperNote", NS->isSuperscriptInNote());
					ss->set("NEWmarksChStyle", NS->marksChStyle());
					ss->set("NEWnotesParStyle", NS->notesParStyle());
					UndoManager::instance()->action(m_Doc, ss);
				}
				//invalidate all text frames with marks from current changed notes style
				foreach (PageItem* item, m_Doc->DocItems)
				{
					if (item->isTextFrame() && !item->isNoteFrame() && item->asTextFrame()->hasNoteMark(NS))
						item->invalid = true;
				}
				m_Doc->updateNotesNums(NS);
				m_Doc->updateNotesFramesSettings(NS);
				if (m_Doc->flag_updateEndNotes)
					m_Doc->updateEndnotesFrames(NS);
				m_Doc->updateNotesFramesStyles(NS);
			}
		}
		if (m_Doc->notesChanged())
		{
			updateNSList();
			m_Doc->flag_updateMarksLabels = true;
			m_Doc->changed();
			m_Doc->regionsChanged()->update(QRectF());
		}
		//restore NStyle index
		readNotesStyle(currNS);
	}

	ApplyButton->setEnabled(false);
	NSlistBox->setEnabled(true);
	NewButton->setEnabled(true);
}

void NotesStylesEditor::on_DeleteButton_clicked()
{
	QString nsName = NSlistBox->currentText();
	int t = QMessageBox::warning(m_Doc->scMW(), tr("Warning! Deleting Notes Style"), "<qt>" +
								 tr("You are going to delete notes style %1. All notes and marks using that style are also going to be deleted.").arg(nsName) + "</qt>",
								 QMessageBox::Ok, QMessageBox::Abort | QMessageBox::Default);
	if (t == QMessageBox::Ok)
	{
		m_Doc->deleteNotesStyle(nsName);
		m_Doc->changed();
		m_Doc->regionsChanged()->update(QRectF());
		setDoc(m_Doc);
	}
}

void NotesStylesEditor::on_NewButton_clicked()
{
	QString oldName = NSlistBox->currentText();
	NotesStyle newNS = changesMap.value(oldName);
	QString newName = oldName;
	getUniqueName(newName, changesMap.keys(), "_");
	newNS.setName(newName);
	changesMap.insert(newName, newNS);
	setNotesStyle(&newNS);
	
	NewNameEdit->setEnabled(true);
	NSlistBox->addItem(newName);
	NSlistBox->setCurrentIndex(NSlistBox->findText(newName));
	NSlistBox->setEnabled(false);
	ApplyButton->setText(tr("Add Style"));
	ApplyButton->setEnabled(true);
	DeleteButton->setEnabled(false);
	NewButton->setEnabled(false);
	addNewNsMode = true;
	OKButton->setText(tr("Cancel Adding"));
	OKButton->setToolTip(tr("Notes Styles Editor is in adding new notes style mode. After pressing Cancel button Notes Styles Editor switch into normal notes styles edit mode."));
}

void NotesStylesEditor::on_OKButton_clicked()
{
	if (OKButton->text() != tr("OK"))
	{
		//in adding new style mode go back to normal editing mode
		OKButton->setText(tr("OK"));
		NewButton->setEnabled(true);
		addNewNsMode = false;
		QString newName = NSlistBox->currentText();
		changesMap.remove(newName);
		int index = NSlistBox->findText(newName);
		NSlistBox->removeItem(index);
		NSlistBox->setCurrentIndex(index-1);
		on_NSlistBox_currentIndexChanged(NSlistBox->currentText());
	}
	else
	{
		if (ApplyButton->isEnabled())
			//apply changes
			on_ApplyButton_clicked();

		//in normal mode close
		close();
	}
}

void NotesStylesEditor::on_NewNameEdit_textChanged(const QString &arg1)
{
	NotesStyle ns = changesMap.value(NSlistBox->currentText());
	ns.setName(arg1);
	changesMap.insert(NSlistBox->currentText(), ns);
	ApplyButton->setEnabled(true);
}

void NotesStylesEditor::on_FootRadio_toggled(bool checked)
{
	bool wasSignalsBlocked = signalsBlocked();
	setBlockSignals(true);

	NotesStyle ns = changesMap.value(NSlistBox->currentText());
	ns.setEndNotes(!checked);
	changesMap.insert(NSlistBox->currentText(), ns);
	EndRadio->setChecked(!checked);
	if (checked)
	{
		ns.setAutoNotesWidth(true);
		AutoW->setEnabled(true);
		AutoW->setChecked(true);
		ns.setAutoWeldNotesFrames(true);
		AutoWeld->setEnabled(true);
		AutoWeld->setChecked(true);
	}
	
	changesMap.insert(NSlistBox->currentText(), ns);
	ApplyButton->setEnabled(true);
	setBlockSignals(wasSignalsBlocked);
}

void NotesStylesEditor::on_EndRadio_toggled(bool checked)
{
	bool wasSignalsBlocked = signalsBlocked();
	setBlockSignals(true);

	NotesStyle ns = changesMap.value(NSlistBox->currentText());
	ns.setEndNotes(checked);
	FootRadio->setChecked(!checked);
	if (checked)
	{
		ns.setAutoNotesWidth(false);
		AutoW->setChecked(false);
		AutoW->setEnabled(false);
		ns.setAutoWeldNotesFrames(false);
		AutoWeld->setChecked(false);
		AutoWeld->setEnabled(false);
	}
	
	changesMap.insert(NSlistBox->currentText(), ns);
	ApplyButton->setEnabled(true);
	setBlockSignals(wasSignalsBlocked);
}

void NotesStylesEditor::on_NumberingBox_currentIndexChanged(int index)
{
	NotesStyle ns = changesMap.value(NSlistBox->currentText());
	ns.setType((NumFormat) index);

	changesMap.insert(NSlistBox->currentText(), ns);
	ApplyButton->setEnabled(true);
}

void NotesStylesEditor::on_RangeBox_currentIndexChanged(int index)
{
	NotesStyle ns = changesMap.value(NSlistBox->currentText());
	ns.setRange((NumerationRange) index);

	changesMap.insert(NSlistBox->currentText(), ns);
	ApplyButton->setEnabled(true);
}

void NotesStylesEditor::on_StartSpinBox_valueChanged(int arg1)
{
	NotesStyle ns = changesMap.value(NSlistBox->currentText());
	ns.setStart(arg1);

	changesMap.insert(NSlistBox->currentText(), ns);
	ApplyButton->setEnabled(true);
}

void NotesStylesEditor::on_PrefixEdit_textChanged(const QString &arg1)
{
	NotesStyle ns = changesMap.value(NSlistBox->currentText());
	ns.setPrefix(arg1);

	changesMap.insert(NSlistBox->currentText(), ns);
	ApplyButton->setEnabled(true);
}

void NotesStylesEditor::on_SuffixEdit_textChanged(const QString &arg1)
{
	NotesStyle ns = changesMap.value(NSlistBox->currentText());
	ns.setSuffix(arg1);

	changesMap.insert(NSlistBox->currentText(), ns);
	ApplyButton->setEnabled(true);
}

void NotesStylesEditor::on_SuperMasterCheck_toggled(bool checked)
{
	NotesStyle ns = changesMap.value(NSlistBox->currentText());
	ns.setSuperscriptInMaster(checked);

	changesMap.insert(NSlistBox->currentText(), ns);
	ApplyButton->setEnabled(true);
}

void NotesStylesEditor::on_SuperNoteCheck_toggled(bool checked)
{
	NotesStyle ns = changesMap.value(NSlistBox->currentText());
	ns.setSuperscriptInNote(checked);

	changesMap.insert(NSlistBox->currentText(), ns);
	ApplyButton->setEnabled(true);
}

void NotesStylesEditor::on_AutoH_toggled(bool checked)
{
	NotesStyle ns = changesMap.value(NSlistBox->currentText());
	ns.setAutoNotesHeight(checked);

	changesMap.insert(NSlistBox->currentText(), ns);
	ApplyButton->setEnabled(true);
}

void NotesStylesEditor::on_AutoW_toggled(bool checked)
{
	NotesStyle ns = changesMap.value(NSlistBox->currentText());
	ns.setAutoNotesWidth(checked);

	changesMap.insert(NSlistBox->currentText(), ns);
	ApplyButton->setEnabled(true);
}

void NotesStylesEditor::on_AutoWeld_toggled(bool checked)
{
	NotesStyle ns = changesMap.value(NSlistBox->currentText());
	ns.setAutoWeldNotesFrames(checked);

	changesMap.insert(NSlistBox->currentText(), ns);
	ApplyButton->setEnabled(true);
}

void NotesStylesEditor::on_AutoRemove_toggled(bool checked)
{
	NotesStyle ns = changesMap.value(NSlistBox->currentText());
	ns.setAutoRemoveEmptyNotesFrames(checked);

	changesMap.insert(NSlistBox->currentText(), ns);
	ApplyButton->setEnabled(true);
}

void NotesStylesEditor::on_paraStyleCombo_currentIndexChanged(const int &arg1)
{
	NotesStyle ns = changesMap.value(NSlistBox->currentText());
	if (arg1 == 0)
		ns.setNotesParStyle("");
	else
		ns.setNotesParStyle(paraStyleCombo->itemText(arg1));
	changesMap.insert(NSlistBox->currentText(), ns);
	ApplyButton->setEnabled(true);
}

void NotesStylesEditor::on_charStyleCombo_currentIndexChanged(const int &arg1)
{
	NotesStyle ns = changesMap.value(NSlistBox->currentText());
	if (arg1 == 0)
		ns.setMarksCharStyle("");
	else
		ns.setMarksCharStyle(charStyleCombo->itemText(arg1));
	changesMap.insert(NSlistBox->currentText(), ns);
	ApplyButton->setEnabled(true);
}
