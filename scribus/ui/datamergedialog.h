/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef DATAMERGEDIALOG_H
#define DATAMERGEDIALOG_H

#include <QDialog>
#include <QString>
#include <QVector>

#include "datamergesource.h"

class QLabel;
class QLineEdit;
class QComboBox;
class QPushButton;
class QSpinBox;
class QTableWidget;
class ScribusDoc;

class DataMergeDialog : public QDialog
{
public:
	explicit DataMergeDialog(ScribusDoc* doc, QWidget* parent = nullptr);

private:
	struct Binding
	{
		QString id;
		QString name;
		int sourceColumn {0};
	};

	void chooseSource();
	void showSource(const QString& path);
	void updateSample();
	void updateApplyButton();
	bool collectBindings(QVector<Binding>& bindings);
	void applyRecord();
	void exportAllPdfs();
	void saveMapping();
	void loadMapping();

	ScribusDoc* m_doc {nullptr};
	DataMergeSource m_source;
	QLineEdit* m_path {nullptr};
	QLabel* m_status {nullptr};
	QSpinBox* m_recordNumber {nullptr};
	QSpinBox* m_firstRecord {nullptr};
	QSpinBox* m_lastRecord {nullptr};
	QComboBox* m_fileNameField {nullptr};
	QLineEdit* m_fileNamePrefix {nullptr};
	QTableWidget* m_preview {nullptr};
	QTableWidget* m_mapping {nullptr};
	QPushButton* m_applyButton {nullptr};
	QPushButton* m_exportButton {nullptr};
};

#endif
