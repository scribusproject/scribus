#include "marksmanager.h"
#include "notesstyles.h"
#include "prefsmanager.h"
#include "prefsfile.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "undomanager.h"
#include "util.h"
#include <QStandardItemModel>

MarksManager::MarksManager(QWidget *parent, const char *name)
	: ScrPaletteBase(parent, name), m_Doc(nullptr)
{
	setupUi(this);
	listView->setSelectionMode(QAbstractItemView::SingleSelection);
	listView->setSortingEnabled(true);
	listView->setHeaderHidden(true);
	listView->setColumnCount(1);
	QString pname(name);
	if (pname.isEmpty())
		pname = "marksManager";
	m_prefs = PrefsManager::instance().prefsFile->getContext(pname);
	setDoc(nullptr);
	languageChange();
	EditButton->setEnabled(false);
	DeleteButton->setEnabled(false);
	UpdateButton->setEnabled(false);
}

MarksManager::~MarksManager()
{
	storeVisibility(this->isVisible());
	storePosition();
	storeSize();
}

void MarksManager::addListItem(MarkType typeMrk, const QString& typeStr, const QList<Mark*> &marks, int &index)
{
	bool noSuchMarks = true;
	QTreeWidgetItem *listItem = new QTreeWidgetItem(listView);
	listItem->setText(0,typeStr);
	listItem->setFlags(listItem->flags() & (~Qt::ItemIsSelectable));
	listItem->setBackground(0, this->palette().color(QPalette::AlternateBase));
	for (int i = 0; i < marks.size(); ++i)
	{
		if (marks[i]->isType(typeMrk))
		{
			QTreeWidgetItem *listItem2 = new QTreeWidgetItem(listItem);
			listItem2->setText(0, marks[i]->label);
			listItem2->setData(1, Qt::UserRole, QVariant::fromValue<void*>(marks[i]));
			index++;
			noSuchMarks = false;
		}
	}
	if (noSuchMarks)
	{
		listView->removeItemWidget(listItem,0);
		delete listItem;
	}
	else
		listItem->sortChildren(0, Qt::AscendingOrder);
}

void MarksManager::storeColaption()
{
	m_expandedItems.clear();
	for (int i=0; i < listView->topLevelItemCount(); ++i)
	{
		QTreeWidgetItem *item = listView->topLevelItem(i);
		if (item->isExpanded())
			m_expandedItems.append(item->text(0));
	}
}

void MarksManager::restoreColaption()
{
	listView->collapseAll();
	if (m_expandedItems.isEmpty())
		return;

	for (int i=0; i < listView->topLevelItemCount(); ++i)
	{
		QTreeWidgetItem *item = listView->topLevelItem(i);
		if (m_expandedItems.contains(item->text(0)))
			item->setExpanded(true);
	}
}

void MarksManager::updateListView()
{
	storeColaption();
	listView->clear();
	if (m_Doc == nullptr)
		return;
	if (m_Doc->marksList().isEmpty())
		UpdateButton->setEnabled(false);
	else
	{
		UpdateButton->setEnabled(true);
		int index = 0;
		addListItem(MARKAnchorType, tr("Anchors"), m_Doc->marksList(), index);
		addListItem(MARKVariableTextType, tr("Variable Text"), m_Doc->marksList(), index);
		addListItem(MARK2ItemType, tr("Marks to Items"), m_Doc->marksList(), index);
		addListItem(MARK2MarkType, tr("Marks to Anchors"), m_Doc->marksList(), index);
		addListItem(MARKNoteMasterType, tr("Notes marks"), m_Doc->marksList(), index);
	//	addListItem(MARKIndexType, tr("Index entries");, marks, index);
		listView->sortByColumn(0, Qt::AscendingOrder);
	}
	restoreColaption();
	m_Doc->flag_updateMarksLabels = false;
	m_Doc->flag_updateEndNotes = false;
}

void MarksManager::setDoc(ScribusDoc *doc)
{
	if (m_Doc != nullptr)
		disconnect(m_Doc->scMW(), SIGNAL(UpdateRequest(int)), this , SLOT(handleUpdateRequest(int)));

	UpdateButton->setEnabled(false);
	listView->setEnabled(false);

	m_Doc = doc;
	if (!m_Doc)
	{
		listView->clear();
		return;
	}

	UpdateButton->setEnabled(true);
	listView->setEnabled(true);
	updateListView();
	connect(m_Doc->scMW(), SIGNAL(UpdateRequest(int)), this , SLOT(handleUpdateRequest(int)));
}

void MarksManager::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
		return;
	}
	if (e->type() == QEvent::PaletteChange)
		paletteChange();
	ScrPaletteBase::changeEvent(e);
}

void MarksManager::languageChange()
{
	retranslateUi(this);

	listView->setToolTip(tr("Double click to find mark in text"));
	UpdateButton->setToolTip(tr("Update all reference texts for all marks"));
	EditButton->setToolTip(tr("Edit selected mark"));
	if (m_Doc != nullptr)
		updateListView();
}

void MarksManager::paletteChange()
{
	QColor listItemColor = this->palette().color(QPalette::AlternateBase);
	for (int i = 0; i < listView->topLevelItemCount(); ++i)
	{
		QTreeWidgetItem *listItem = listView->topLevelItem(i);
		listItem->setBackground(0, listItemColor);
	}
}

void MarksManager::handleUpdateRequest(int updateFlags)
{
	if (updateFlags & reqMarksUpdate) {
		updateListView();
	}
	m_Doc->flag_updateMarksLabels = false;
}

Mark* MarksManager::getMarkFromListView()
{
	QTreeWidgetItem* selectedItem = listView->currentItem();
	if (selectedItem == nullptr)
		return nullptr;
	Mark* mrk = reinterpret_cast<Mark*>(selectedItem->data(1, Qt::UserRole).value<void*>());
	return mrk;
}

void MarksManager::on_UpdateButton_clicked()
{
	m_Doc->flag_updateMarksLabels = true;
	m_Doc->flag_updateEndNotes = true;
	m_Doc->setNotesChanged(true);
	if (m_Doc->updateMarks(true))
	{
		m_Doc->changed();
		m_Doc->regionsChanged()->update(QRectF());
		updateListView();
	}

	//update labels for "lost" marks (marks not in any text)
	QList<Mark*> notUsed;
	for (int a=0; a < m_Doc->marksList().count(); ++a)
	{
		Mark* mrk = m_Doc->marksList().at(a);
		if (mrk->isUnique() && !mrk->label.startsWith("UNVISIBLE*") && !m_Doc->isMarkUsed(mrk, true))
			notUsed.append(mrk);
	}

	if (notUsed.isEmpty())
		return;

	for (int a=0; a < notUsed.count(); ++a)
	{
		Mark* mrk = notUsed.at(a);
		QString l = "UNVISIBLE*" + mrk->label;
		getUniqueName(l, m_Doc->marksLabelsList(mrk->getType()), "_");
		mrk->label = l;
	}
	updateListView();
}

void MarksManager::on_EditButton_clicked()
{
	Mark* mrk = getMarkFromListView();
	if (mrk == nullptr)
		return;

	if (m_Doc->scMW()->editMarkDlg(mrk))
	{
		if (mrk->isType(MARKVariableTextType))
			m_Doc->flag_updateMarksLabels = true;
//		else
//			currItem->invalid = true;
		m_Doc->changed();
		m_Doc->regionsChanged()->update(QRectF());
		updateListView();
	}
}

void MarksManager::on_DeleteButton_clicked()
{
	Mark* mrk = getMarkFromListView();
	if (mrk == nullptr)
		return;

	if (mrk->isType(MARKNoteMasterType))
		m_Doc->setUndoDelNote(mrk->getNotePtr());
	else
		m_Doc->setUndoDelMark(mrk);
	m_Doc->eraseMark(mrk, true, nullptr, true);
	m_Doc->changed();
	m_Doc->regionsChanged()->update(QRectF());
	updateListView();
}

void MarksManager::on_listView_doubleClicked(const QModelIndex &index)
{
	Mark* mrk = getMarkFromListView();
	if (mrk != nullptr)
		m_Doc->setCursor2MarkPos(mrk);
}

void MarksManager::on_listView_itemSelectionChanged()
{
	bool isMark = (getMarkFromListView() != nullptr);
	EditButton->setEnabled(isMark);
	DeleteButton->setEnabled(isMark);
}
