/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "datamergedialog.h"

#include <QComboBox>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QFile>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPushButton>
#include <QProgressDialog>
#include <QSaveFile>
#include <QSet>
#include <QSignalBlocker>
#include <QSpinBox>
#include <QTableWidget>
#include <QVBoxLayout>

#include <algorithm>

#include "dynamicvariable.h"
#include "scribus.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "undomanager.h"

DataMergeDialog::DataMergeDialog(ScribusDoc* doc, QWidget* parent)
	: QDialog(parent), m_doc(doc)
{
	setWindowTitle(tr("Data Merge"));
	setMinimumSize(760, 650);
	auto* layout = new QVBoxLayout(this);
	auto* introduction = new QLabel(tr("Choose a CSV or JSON file and match its fields to user-defined variables. Apply one record to the document, or export a selected range as separate PDFs using the document's PDF settings."), this);
	introduction->setWordWrap(true);
	layout->addWidget(introduction);

	auto* sourceRow = new QHBoxLayout;
	m_path = new QLineEdit(this);
	m_path->setReadOnly(true);
	m_path->setPlaceholderText(tr("No data source selected"));
	auto* browse = new QPushButton(tr("Choose Source..."), this);
	connect(browse, &QPushButton::clicked, this, [this]() { chooseSource(); });
	sourceRow->addWidget(m_path, 1);
	sourceRow->addWidget(browse);
	layout->addLayout(sourceRow);

	auto* previewGroup = new QGroupBox(tr("Records"), this);
	auto* previewLayout = new QVBoxLayout(previewGroup);
	auto* recordRow = new QHBoxLayout;
	m_status = new QLabel(tr("Select a source to preview its records."), this);
	m_recordNumber = new QSpinBox(this);
	m_recordNumber->setMinimum(1);
	m_recordNumber->setEnabled(false);
	recordRow->addWidget(m_status, 1);
	recordRow->addWidget(new QLabel(tr("Record:"), this));
	recordRow->addWidget(m_recordNumber);
	previewLayout->addLayout(recordRow);
	m_preview = new QTableWidget(this);
	m_preview->setEditTriggers(QAbstractItemView::NoEditTriggers);
	m_preview->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_preview->setSelectionMode(QAbstractItemView::SingleSelection);
	m_preview->setAlternatingRowColors(true);
	m_preview->verticalHeader()->setVisible(false);
	m_preview->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	previewLayout->addWidget(m_preview);
	layout->addWidget(previewGroup, 1);

	auto* mappingGroup = new QGroupBox(tr("Field Mapping"), this);
	auto* mappingLayout = new QVBoxLayout(mappingGroup);
	m_mapping = new QTableWidget(this);
	m_mapping->setColumnCount(3);
	m_mapping->setHorizontalHeaderLabels({tr("Source Field"), tr("Selected Value"), tr("Document Variable")});
	m_mapping->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
	m_mapping->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
	m_mapping->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
	m_mapping->verticalHeader()->setVisible(false);
	m_mapping->setEditTriggers(QAbstractItemView::NoEditTriggers);
	mappingLayout->addWidget(m_mapping);
	auto* presetRow = new QHBoxLayout;
	auto* loadPreset = new QPushButton(tr("Load Mapping..."), this);
	auto* savePreset = new QPushButton(tr("Save Mapping..."), this);
	connect(loadPreset, &QPushButton::clicked, this, [this]() { loadMapping(); });
	connect(savePreset, &QPushButton::clicked, this, [this]() { saveMapping(); });
	presetRow->addWidget(loadPreset);
	presetRow->addWidget(savePreset);
	presetRow->addStretch();
	mappingLayout->addLayout(presetRow);
	layout->addWidget(mappingGroup, 1);

	auto* outputGroup = new QGroupBox(tr("Batch Output"), this);
	auto* outputLayout = new QHBoxLayout(outputGroup);
	m_firstRecord = new QSpinBox(this);
	m_lastRecord = new QSpinBox(this);
	m_firstRecord->setMinimum(1);
	m_lastRecord->setMinimum(1);
	m_firstRecord->setEnabled(false);
	m_lastRecord->setEnabled(false);
	m_fileNameField = new QComboBox(this);
	m_fileNameField->addItem(tr("Record number only"), QString());
	m_fileNamePrefix = new QLineEdit(this);
	m_fileNamePrefix->setPlaceholderText(tr("Document name"));
	outputLayout->addWidget(new QLabel(tr("From:"), this));
	outputLayout->addWidget(m_firstRecord);
	outputLayout->addWidget(new QLabel(tr("To:"), this));
	outputLayout->addWidget(m_lastRecord);
	outputLayout->addWidget(new QLabel(tr("Filename field:"), this));
	outputLayout->addWidget(m_fileNameField, 1);
	outputLayout->addWidget(new QLabel(tr("Prefix:"), this));
	outputLayout->addWidget(m_fileNamePrefix, 1);
	layout->addWidget(outputGroup);

	auto* buttons = new QDialogButtonBox(QDialogButtonBox::Close, this);
	m_applyButton = buttons->addButton(tr("Apply Selected Record"), QDialogButtonBox::ActionRole);
	m_applyButton->setEnabled(false);
	connect(m_applyButton, &QPushButton::clicked, this, [this]() { applyRecord(); });
	m_exportButton = buttons->addButton(tr("Export Selected PDFs..."), QDialogButtonBox::ActionRole);
	m_exportButton->setEnabled(false);
	connect(m_exportButton, &QPushButton::clicked, this, [this]() { exportAllPdfs(); });
	connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
	connect(m_recordNumber, qOverload<int>(&QSpinBox::valueChanged), this, [this](int number) {
		if (number <= m_preview->rowCount())
			m_preview->selectRow(number - 1);
		else
			m_preview->clearSelection();
		updateSample();
	});
	connect(m_preview, &QTableWidget::itemSelectionChanged, this, [this]() {
		if (m_preview->currentRow() >= 0 && m_preview->currentRow() < m_preview->rowCount())
			m_recordNumber->setValue(m_preview->currentRow() + 1);
	});
	layout->addWidget(buttons);
}

void DataMergeDialog::chooseSource()
{
	const QString path = QFileDialog::getOpenFileName(this, tr("Choose Data Source"), QString(),
		tr("Data Sources (*.csv *.json);;CSV Files (*.csv);;JSON Files (*.json)"));
	if (!path.isEmpty())
		showSource(path);
}

void DataMergeDialog::showSource(const QString& path)
{
	QString error;
	if (!m_source.load(path, QString(), &error))
	{
		m_path->clear();
		m_preview->clear();
		m_preview->setRowCount(0);
		m_preview->setColumnCount(0);
		m_mapping->setRowCount(0);
		m_recordNumber->setEnabled(false);
		m_firstRecord->setEnabled(false);
		m_lastRecord->setEnabled(false);
		m_fileNameField->clear();
		m_fileNameField->addItem(tr("Record number only"), QString());
		m_status->setText(tr("Select a valid source to preview its records."));
		updateApplyButton();
		QMessageBox::warning(this, tr("Data Source Error"), error);
		return;
	}
	m_path->setText(path);
	const int count = m_source.recordCount();
	const int previewCount = qMin(count, 100);
	m_status->setText(count > previewCount
		? tr("%1 records; showing the first %2.").arg(count).arg(previewCount)
		: tr("%1 records.").arg(count));
	m_recordNumber->setEnabled(count > 0);
	m_recordNumber->setMaximum(qMax(1, count));
	m_firstRecord->setEnabled(count > 0);
	m_lastRecord->setEnabled(count > 0);
	m_firstRecord->setMaximum(qMax(1, count));
	m_lastRecord->setMaximum(qMax(1, count));
	m_firstRecord->setValue(1);
	m_lastRecord->setValue(qMax(1, count));
	m_fileNameField->clear();
	m_fileNameField->addItem(tr("Record number only"), QString());
	for (const QString& field : m_source.fields())
		m_fileNameField->addItem(field, field);
	m_preview->clear();
	m_preview->setColumnCount(m_source.fields().size());
	m_preview->setHorizontalHeaderLabels(m_source.fields());
	m_preview->setRowCount(previewCount);
	for (int row = 0; row < previewCount; ++row)
	{
		const QStringList& values = m_source.row(row);
		for (int column = 0; column < values.size(); ++column)
			m_preview->setItem(row, column, new QTableWidgetItem(values.at(column)));
	}

	m_mapping->setRowCount(m_source.fields().size());
	QVector<DynamicVariable> variables;
	for (const DynamicVariable& variable : m_doc->dynamicVariables())
	{
		if (variable.type == DynamicVariableResolver::UserDefined)
			variables.append(variable);
	}
	std::sort(variables.begin(), variables.end(), [](const DynamicVariable& left, const DynamicVariable& right) {
		return QString::localeAwareCompare(left.name, right.name) < 0;
	});
	for (int row = 0; row < m_source.fields().size(); ++row)
	{
		const QString field = m_source.fields().at(row);
		m_mapping->setItem(row, 0, new QTableWidgetItem(field));
		m_mapping->setItem(row, 1, new QTableWidgetItem());
		auto* target = new QComboBox(m_mapping);
		target->addItem(tr("Ignore field"), QString());
		for (const DynamicVariable& variable : variables)
		{
			target->addItem(variable.name, variable.id);
			if (variable.name == field)
				target->setCurrentIndex(target->count() - 1);
		}
		connect(target, qOverload<int>(&QComboBox::currentIndexChanged), this, [this](int) { updateApplyButton(); });
		m_mapping->setCellWidget(row, 2, target);
	}
	if (count > 0)
	{
		m_recordNumber->setValue(1);
		m_preview->selectRow(0);
	}
	updateSample();
	updateApplyButton();
}

void DataMergeDialog::updateSample()
{
	const int recordIndex = m_recordNumber->value() - 1;
	if (recordIndex < 0 || recordIndex >= m_source.recordCount())
		return;
	const QStringList& values = m_source.row(recordIndex);
	for (int row = 0; row < m_mapping->rowCount(); ++row)
		m_mapping->item(row, 1)->setText(values.value(row));
}

void DataMergeDialog::updateApplyButton()
{
	bool mapped = false;
	for (int row = 0; row < m_mapping->rowCount(); ++row)
	{
		const auto* target = qobject_cast<QComboBox*>(m_mapping->cellWidget(row, 2));
		if (target && !target->currentData().toString().isEmpty())
			mapped = true;
	}
	m_applyButton->setEnabled(m_doc && m_source.recordCount() > 0 && mapped);
	m_exportButton->setEnabled(m_doc && m_source.recordCount() > 0 && mapped);
}

bool DataMergeDialog::collectBindings(QVector<Binding>& bindings)
{
	bindings.clear();
	if (!m_doc)
		return false;
	QSet<QString> targets;
	for (int row = 0; row < m_mapping->rowCount(); ++row)
	{
		const auto* target = qobject_cast<QComboBox*>(m_mapping->cellWidget(row, 2));
		const QString id = target ? target->currentData().toString() : QString();
		if (id.isEmpty())
			continue;
		const DynamicVariable* variable = m_doc->dynamicVariable(id);
		if (!variable || variable->type != DynamicVariableResolver::UserDefined || targets.contains(id))
		{
			QMessageBox::warning(this, tr("Invalid Mapping"), tr("Map each field to a different user-defined variable."));
			return false;
		}
		targets.insert(id);
		bindings.append({id, variable->name, row});
	}
	return !bindings.isEmpty();
}

void DataMergeDialog::applyRecord()
{
	if (!m_doc || m_source.recordCount() == 0)
		return;
	const int recordIndex = m_recordNumber->value() - 1;
	if (recordIndex < 0 || recordIndex >= m_source.recordCount())
		return;
	QVector<Binding> bindings;
	if (!collectBindings(bindings))
		return;
	UndoTransaction transaction;
	if (bindings.size() > 1 && UndoManager::undoEnabled())
		transaction = UndoManager::instance()->beginTransaction(m_doc->getUName(), Um::IDocument, tr("Apply Data Record"));
	const QStringList& values = m_source.row(recordIndex);
	for (const Binding& binding : bindings)
		m_doc->updateDynamicVariable(binding.id, binding.name, values.value(binding.sourceColumn));
	if (transaction)
		transaction.commit();
	m_doc->changed();
	m_doc->regionsChanged()->update(QRectF());
	m_status->setText(tr("Applied record %1 of %2.").arg(recordIndex + 1).arg(m_source.recordCount()));
}

void DataMergeDialog::exportAllPdfs()
{
	if (!m_doc || m_source.recordCount() == 0)
		return;
	QVector<Binding> bindings;
	if (!collectBindings(bindings))
		return;
	ScribusMainWindow* mainWindow = ScCore->primaryMainWindow();
	if (!mainWindow || mainWindow->doc != m_doc || !mainWindow->view)
	{
		QMessageBox::warning(this, tr("Data Merge"), tr("The document is no longer available for PDF export."));
		return;
	}

	const QString directory = QFileDialog::getExistingDirectory(this, tr("Choose Folder for Merged PDFs"));
	if (directory.isEmpty())
		return;
	QMap<QString, QString> mapping;
	for (const Binding& binding : bindings)
		mapping.insert(m_source.fields().at(binding.sourceColumn), binding.id);
	DataMergeBatchOptions options;
	options.firstRecord = m_firstRecord->value();
	options.lastRecord = m_lastRecord->value();
	options.fileNameField = m_fileNameField->currentData().toString();
	const int selectedCount = qMax(0, options.lastRecord - options.firstRecord + 1);
	QProgressDialog progress(tr("Exporting merged PDFs..."), tr("Cancel"), 0, selectedCount, this);
	progress.setWindowModality(Qt::WindowModal);
	progress.setMinimumDuration(0);
	DataMergeBatchResult result;
	DataMergeBatchExporter::exportPdfs(mainWindow, m_source, mapping, directory, m_fileNamePrefix->text(), result,
		[&progress](int index, int) {
			progress.setValue(index);
			return !progress.wasCanceled();
		}, options);
	progress.setValue(selectedCount);
	const int exported = result.files.size();
	m_status->setText(tr("Exported %1 of %2 selected records.").arg(exported).arg(selectedCount));
	if (!result.error.isEmpty())
		QMessageBox::warning(this, tr("Data Merge Export Failed"),
			tr("Exported %1 of %2 PDFs. Completed files remain in:\n%3\n\n%4")
				.arg(exported).arg(selectedCount).arg(directory, result.error));
	else if (result.cancelled)
		QMessageBox::information(this, tr("Data Merge Cancelled"),
			tr("Exported %1 of %2 PDFs before cancellation. Completed files remain in:\n%3")
				.arg(exported).arg(selectedCount).arg(directory));
	else
		QMessageBox::information(this, tr("Data Merge Complete"),
			tr("Exported %1 PDFs to:\n%2").arg(exported).arg(directory));
}

void DataMergeDialog::saveMapping()
{
	QVector<Binding> bindings;
	if (!collectBindings(bindings))
		return;
	const QString path = QFileDialog::getSaveFileName(this, tr("Save Data Merge Mapping"), QString(),
		tr("Data Merge Mappings (*.json)"));
	if (path.isEmpty())
		return;
	QJsonObject fields;
	for (const Binding& binding : bindings)
		fields.insert(m_source.fields().at(binding.sourceColumn), binding.name);
	QJsonObject mapping;
	mapping.insert(QStringLiteral("version"), 1);
	mapping.insert(QStringLiteral("fields"), fields);
	QSaveFile file(path);
	const QByteArray data = QJsonDocument(mapping).toJson();
	if (!file.open(QIODevice::WriteOnly) || file.write(data) != data.size() || !file.commit())
		QMessageBox::warning(this, tr("Data Merge"), tr("Could not save the mapping preset."));
}

void DataMergeDialog::loadMapping()
{
	if (m_source.fields().isEmpty())
		return;
	const QString path = QFileDialog::getOpenFileName(this, tr("Load Data Merge Mapping"), QString(),
		tr("Data Merge Mappings (*.json)"));
	if (path.isEmpty())
		return;
	QFile file(path);
	QJsonParseError parseError;
	if (!file.open(QIODevice::ReadOnly))
	{
		QMessageBox::warning(this, tr("Data Merge"), tr("Could not read the mapping preset."));
		return;
	}
	const QJsonDocument document = QJsonDocument::fromJson(file.readAll(), &parseError);
	if (parseError.error != QJsonParseError::NoError || !document.isObject()
		|| document.object().value(QStringLiteral("version")).toInt() != 1
		|| !document.object().value(QStringLiteral("fields")).isObject())
	{
		QMessageBox::warning(this, tr("Data Merge"), tr("The mapping preset is not valid."));
		return;
	}
	const QJsonObject fields = document.object().value(QStringLiteral("fields")).toObject();
	QVector<int> choices(m_source.fields().size(), 0);
	QSet<QString> targets;
	for (auto it = fields.constBegin(); it != fields.constEnd(); ++it)
	{
		const int row = m_source.fields().indexOf(it.key());
		if (row < 0 || !it.value().isString())
		{
			QMessageBox::warning(this, tr("Data Merge"), tr("A preset field is not present in this data source."));
			return;
		}
		auto* target = qobject_cast<QComboBox*>(m_mapping->cellWidget(row, 2));
		const int choice = target ? target->findText(it.value().toString()) : -1;
		if (choice <= 0 || targets.contains(target->itemData(choice).toString()))
		{
			QMessageBox::warning(this, tr("Data Merge"), tr("A preset variable is missing or used more than once."));
			return;
		}
		choices[row] = choice;
		targets.insert(target->itemData(choice).toString());
	}
	for (int row = 0; row < choices.size(); ++row)
	{
		auto* target = qobject_cast<QComboBox*>(m_mapping->cellWidget(row, 2));
		const QSignalBlocker blocker(target);
		target->setCurrentIndex(choices.at(row));
	}
	updateApplyButton();
}
