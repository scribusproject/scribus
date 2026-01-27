#include <QHeaderView>
#include <QInputDialog>
#include <QMessageBox>

#include "iconmanager.h"
#include "opticalmargineditor.h"
#include "scribusapp.h"
#include "scrspinbox.h"
#include "util_text.h"

OpticalMarginEditor::OpticalMarginEditor(QWidget *parent)
	: QWidget(parent)
{
	setupUi(this);

	tableWidget->clear();
	tableWidget->setShowGrid(false);
	tableWidget->verticalHeader()->hide();
	tableWidget->setColumnCount(5);

	QHeaderView *headerView = new QHeaderView(Qt::Horizontal, tableWidget);
	tableWidget->setHorizontalHeader(headerView);
	headerView->setSectionResizeMode(0, QHeaderView::ResizeToContents);
	headerView->setSectionResizeMode(1, QHeaderView::ResizeToContents);
	headerView->setSectionResizeMode(2, QHeaderView::ResizeToContents);
	headerView->setSectionResizeMode(3, QHeaderView::Stretch);
	headerView->setSectionResizeMode(4, QHeaderView::ResizeToContents);

	auto &oml = OpticalMarginLookup::instance();
	m_sets = oml.defaultSets();
	m_setId = oml.defaultSetId();
	createTable();
	iconSetChange();
	languageChange();

	// remove this code to support more than 1 set
	// missing implementation: import of optical margin sets from imported paragraph styles in style manager
	buttonAddNewSet->setVisible(false);
	buttonRemoveSet->setVisible(false);
	buttonResetAllSets->setVisible(false);
	buttonLoadMissingDefaults->setVisible(false);
	label_2->setVisible(false);

	connect(comboSets, SIGNAL(currentIndexChanged(int)), this, SLOT(loadSet()));
	connect(buttonResetSet, SIGNAL(clicked()), this, SLOT(resetCurrentSet()));
	connect(buttonAddNewSet, SIGNAL(clicked()), this, SLOT(addNewSet()));
	connect(buttonRemoveSet, SIGNAL(clicked()), this, SLOT(removeCurrentSet()));
	connect(buttonResetAllSets, SIGNAL(clicked()), this, SLOT(resetAllSets()));
	connect(buttonLoadMissingDefaults, SIGNAL(clicked()), this, SLOT(addMissingDefaults()));
	connect(buttonAddNewRow, SIGNAL(clicked()), this, SLOT(addNewRow()));

	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));
}

void OpticalMarginEditor::setOpticalMarginSets(const OpticalMarginSets& sets)
{
	m_sets = sets;
	setOpticalMarginSetId(opticalMarginSetId());
}

void OpticalMarginEditor::setOpticalMarginSetId(const QString& id)
{
	if (m_sets.size() <= 0)
		return;

	m_setId = m_sets.contains(id) ? id : m_sets.firstKey();
	buttonResetSet->setVisible(m_sets.value(m_setId).type == "preset" ? true : false);

	updateSetSelector();
	createTable();
}

void OpticalMarginEditor::addRow(const QString &s, double left, double right, int unit)
{
	IconManager &im = IconManager::instance();

	ScrSpinBox *spinL = new ScrSpinBox(this);
	spinL->setMinimum(0.0);
	spinL->setMaximum(100.0);
	spinL->setNewUnit(unit);
	if (unit == SC_PERCENT)
	{
		spinL->setDecimals(0);
		spinL->setValue(left * 100);
	}
	else
	{
		spinL->setValue(left, SC_PT);
	}
	connect(spinL, SIGNAL(valueChanged(double)), this, SLOT(updateMarginRules()));

	ScrSpinBox *spinR = new ScrSpinBox(this);
	spinR->setMinimum(0.0);
	spinR->setMaximum(100.0);
	spinR->setNewUnit(unit);
	if (unit == SC_PERCENT)
	{
		spinR->setDecimals(0);
		spinR->setValue(right * 100);
	}
	else
	{
		spinR->setValue(right, SC_PT);
	}
	connect(spinR, SIGNAL(valueChanged(double)), this, SLOT(updateMarginRules()));

	QComboBox *comboUnit = new QComboBox();
	comboUnit->addItem("%", scUnit::SC_PERCENT);
	comboUnit->addItem("pt", scUnit::SC_PT);
	comboUnit->addItem("mm", scUnit::SC_MM);
	comboUnit->addItem("in", scUnit::SC_IN);
	comboUnit->setCurrentIndex(comboUnit->findData(unit));
	connect(comboUnit, SIGNAL(currentIndexChanged(int)), this, SLOT(updateUnit()));

	QLineEdit *lineStr = new QLineEdit();
	lineStr->setText(s);
	lineStr->setMinimumWidth(50);
	connect(lineStr, SIGNAL(textChanged(QString)), this, SLOT(updateMarginRules()));
	connect(lineStr, SIGNAL(selectionChanged()), this, SLOT(updatePreview()));

	QToolButton *buttonRemove = new QToolButton();
	buttonRemove->setText("");
	buttonRemove->setIconSize(QSize(16, 16));
	buttonRemove->setIcon(im.loadIcon("delete"));
	buttonRemove->adjustSize();
	connect(buttonRemove, SIGNAL(clicked()), this, SLOT(removeCurrentRow()));

	int row = tableWidget->rowCount();
	tableWidget->setRowCount(tableWidget->rowCount() + 1);
	tableWidget->setCellWidget(row, 0, spinL);
	tableWidget->setCellWidget(row, 1, spinR);
	tableWidget->setCellWidget(row, 2, comboUnit);
	tableWidget->setCellWidget(row, 3, lineStr);
	tableWidget->setCellWidget(row, 4, buttonRemove);
}

void OpticalMarginEditor::updateSetSelector()
{
	QSignalBlocker blockSets(comboSets);
	comboSets->clear();

	for (auto [name, set] : m_sets.asKeyValueRange())
		comboSets->addItem(set.name, name);

	int index = comboSets->findData(m_setId);
	comboSets->setCurrentIndex(index);
}

void OpticalMarginEditor::createTable()
{
	tableWidget->clearContents();
	tableWidget->setRowCount(0);

	if (m_sets.isEmpty())
		qWarning() << Q_FUNC_INFO << "Empty optical margin sets.";

	OpticalMarginSet set = (m_sets.contains(m_setId)) ? m_sets.value(m_setId) : m_sets.first();
	OpticalMarginRules rules = OpticalMarginLookup::instance().mergeCharsToRules(set.rules);

	for (const auto &rule : rules)
		addRow(rule.Chars, rule.Left, rule.Right, rule.Unit);
}

void OpticalMarginEditor::resetCurrentSet()
{
	QMessageBox msgBox;
	msgBox.setWindowTitle(tr("Reset Set"));
	msgBox.setText(tr("Do you want to reset the current set to default?"));
	msgBox.setInformativeText(tr("This will revert all changes. This cannot be undone."));
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	msgBox.setDefaultButton(QMessageBox::No);

	if (msgBox.exec() != QMessageBox::Yes)
		return;

	OpticalMarginLookup &oml = OpticalMarginLookup::instance();

	if (oml.defaultSets().contains(m_setId))
	{
		m_sets.insert(m_setId, oml.defaultSets().value(m_setId));
		createTable();
		emit setChanged();
	}
}

void OpticalMarginEditor::addMissingDefaults()
{
	OpticalMarginLookup &oml = OpticalMarginLookup::instance();

	for (auto [id, set] : oml.defaultSets().asKeyValueRange())
		if (!m_sets.contains(id))
			m_sets.insert(id, set);

	updateSetSelector();
	emit setChanged();
}

void OpticalMarginEditor::loadSet()
{
	QString id = comboSets->currentData().toString();

	if (id.isEmpty())
		return;

	m_setId = id;
	createTable();

	if (m_sets.contains(m_setId))
		buttonResetSet->setVisible(m_sets.value(m_setId).type == "preset" ? true : false);

	emit setChanged();
}

void OpticalMarginEditor::removeCurrentSet()
{
	if (!m_sets.contains(m_setId) || m_sets.size() == 1)
		return;

	OpticalMarginLookup &oml = OpticalMarginLookup::instance();
	QString oldId = m_setId;
	QString oldName = oml.trName(m_sets, m_setId);

	QMessageBox msgBox;
	msgBox.setWindowTitle(tr("Remove Current Set"));
	msgBox.setText(QString(tr("Are you sure you want to delete set '%1'?")).arg(oldName));
	msgBox.setInformativeText(tr("This cannot be undone."));
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	msgBox.setDefaultButton(QMessageBox::No);

	if (msgBox.exec() != QMessageBox::Yes)
		return;

	bool ok;
	OpticalMarginSets tmpSets = m_sets;
	tmpSets.remove(oldId);
	QMap<QString, QString> trNames = oml.trNames(tmpSets);
	QString newName = QInputDialog::getItem(this, tr("Set Replacement"), QString(tr("New set for '%1'.")).arg(oldName), trNames.values(), 0, false, &ok);
	QString newId = trNames.key(newName);

	if (ok && !newId.isEmpty())
	{
		m_sets = tmpSets;
		setOpticalMarginSetId(newId);
		emit setChanged();
		emit setDeleted(oldId, newId);
	}
}

void OpticalMarginEditor::addNewSet()
{
	bool ok;
	QString name = QInputDialog::getText(this, tr("New Set Name"), tr("Set Name:"), QLineEdit::Normal, tr("New Set"), &ok);

	if (!ok || name.isEmpty())
		return;

	OpticalMarginLookup &oml = OpticalMarginLookup::instance();

	if (oml.hasTrName(m_sets, name))
	{
		QMessageBox msgBox;
		msgBox.setWindowTitle(tr("Add New Set"));
		msgBox.setText(QString(tr("Set name '%1' already exists. Please select another name.")).arg(name));
		msgBox.exec();
		return;
	}

	QString id = oml.nextAvailableUserId(m_sets);

	OpticalMarginSet set;
	set.id = id;
	set.name = name;
	set.type = "user";

	m_sets.insert(id, set);
	setOpticalMarginSetId(id);
	emit setChanged();
}

void OpticalMarginEditor::resetAllSets()
{
	QMessageBox msgBox;
	msgBox.setWindowTitle(tr("Load Default Sets"));
	msgBox.setText(tr("Do you want to load all default sets?"));
	msgBox.setInformativeText(tr("This will replace all current sets. This cannot be undone."));
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	msgBox.setDefaultButton(QMessageBox::No);

	if (msgBox.exec() != QMessageBox::Yes)
		return;

	OpticalMarginLookup &oml = OpticalMarginLookup::instance();

	// Add all missing sets and override existing sets
	for (auto [id, set] : oml.defaultSets().asKeyValueRange())
		m_sets.insert(id, set);

	OpticalMarginSets tmpSets = m_sets;
	bool ok;

	QMap<QString, QString> trNames = oml.trNames(oml.defaultSets());

	for (auto &name : tmpSets)
	{
		// We need a replacement for all custom user sets
		if (!oml.defaultSets().contains(name.id))
		{
			QString oldName = oml.trName(tmpSets, name.id);
			QString newName = QInputDialog::getItem(this, tr("Set Replacement"), QString(tr("New set for '%1'.")).arg(oldName), trNames.values(), 0, false, &ok);
			QString newId = trNames.key(newName);

			// We replace custom user sets only if user select a replacement
			if (ok && !newId.isEmpty())
			{
				m_sets.remove(name.id);
				emit setDeleted(name.id, newId);
			}
		}
	}

	setOpticalMarginSetId(m_sets.firstKey());
	emit setChanged();
}

void OpticalMarginEditor::removeCurrentRow()
{
	if (!sender())
		return;

	QToolButton *button = qobject_cast<QToolButton *>(sender());

	if (!button)
		return;

	tableWidget->removeRow(tableWidget->indexAt(button->pos()).row());
	updateMarginRules();
}

void OpticalMarginEditor::addNewRow()
{
	addRow("", 0.5, 0.5, SC_PERCENT);
}

void OpticalMarginEditor::updateMarginRules()
{
	OpticalMarginRules rules;

	for (int i = 0; i < tableWidget->rowCount(); i++)
	{
		ScrSpinBox *spinL = qobject_cast<ScrSpinBox *>(tableWidget->cellWidget(i, 0));
		double left = (spinL->unitIndex() == SC_PERCENT) ? spinL->value() / 100.0 : spinL->getValue(SC_PT);

		ScrSpinBox *spinR = qobject_cast<ScrSpinBox *>(tableWidget->cellWidget(i, 1));
		double right = (spinR->unitIndex() == SC_PERCENT) ? spinR->value() / 100.0 : spinR->getValue(SC_PT);

		QComboBox *combo = qobject_cast<QComboBox *>(tableWidget->cellWidget(i, 2));
		int unit = combo->currentData().toInt();

		QLineEdit *lineEdit = qobject_cast<QLineEdit *>(tableWidget->cellWidget(i, 3));
		QString s = lineEdit->text().trimmed();

		if (s.isEmpty())
			continue;

		rules.append(OpticalMarginRule(s, left, right, unit));
	}

	if (m_sets.size() <= 0)
		qWarning() << Q_FUNC_INFO << "Empty optical margin sets.";

	OpticalMarginSet set = (m_sets.contains(m_setId)) ? m_sets.value(m_setId) : m_sets.first();
	set.rules = OpticalMarginLookup::instance().splitRulesToChars(rules);

	m_sets.insert(set.id, set);

	emit setChanged();
}

void OpticalMarginEditor::updatePreview()
{
	if (!sender())
		return;

	QLineEdit *lineEdit = qobject_cast<QLineEdit *>(sender());

	if (!lineEdit)
		return;

	QString str = lineEdit->selectedText();

	if (str.length() < 1)
	{
		labelUnicodePreview->setText("");
		return;
	}

	labelUnicodePreview->setText(stringToUnicode(str[str.length() - 1]));
}

void OpticalMarginEditor::updateUnit()
{
	if (!sender())
		return;

	QComboBox *combo = qobject_cast<QComboBox *>(sender());

	if (!combo)
		return;

	int row = tableWidget->indexAt(combo->pos()).row();
	int unit = combo->currentData().toInt();

	ScrSpinBox *spinL = qobject_cast<ScrSpinBox *>(tableWidget->cellWidget(row, 0));
	if (spinL)
	{
		QSignalBlocker blockRight(spinL);
		spinL->setNewUnit(unit);
		if (unit == SC_PERCENT)
			spinL->setDecimals(0);
	}

	ScrSpinBox *spinR = qobject_cast<ScrSpinBox *>(tableWidget->cellWidget(row, 1));
	if (spinR)
	{
		QSignalBlocker blockRight(spinR);
		spinR->setNewUnit(unit);
		if (unit == SC_PERCENT)
			spinR->setDecimals(0);
	}

	updateMarginRules();
}

void OpticalMarginEditor::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
		return;
	}
	QWidget::changeEvent(e);
}

void OpticalMarginEditor::languageChange()
{
	retranslateUi(this);

	tableWidget->setHorizontalHeaderLabels(QStringList {tr("Left"), tr("Right"), tr("Unit"), tr("Characters"), ""});

	updateSetSelector();
}

void OpticalMarginEditor::iconSetChange()
{
	IconManager &im = IconManager::instance();
	buttonResetSet->setIcon(im.loadIcon("reset"));
	buttonAddNewSet->setIcon(im.loadIcon("add"));
	buttonRemoveSet->setIcon(im.loadIcon("remove"));
	buttonResetAllSets->setIcon(im.loadIcon("reset"));
	buttonLoadMissingDefaults->setIcon(im.loadIcon("add"));
	buttonAddNewRow->setIcon(im.loadIcon("add"));
}
