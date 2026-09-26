/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef DATAMERGESOURCE_H
#define DATAMERGESOURCE_H

#include <QMap>
#include <QStringList>
#include <QVector>

#include <functional>

class ScribusMainWindow;

class DataMergeSource
{
public:
	bool load(const QString& path, const QString& format = QString(), QString* error = nullptr);
	const QStringList& fields() const { return m_fields; }
	int recordCount() const { return m_rows.size(); }
	const QStringList& row(int index) const { return m_rows.at(index); }
	QMap<QString, QString> record(int index) const;

private:
	QStringList m_fields;
	QVector<QStringList> m_rows;
};

struct DataMergeBatchResult
{
	QStringList files;
	QString error;
	bool cancelled {false};
};

struct DataMergeBatchOptions
{
	int firstRecord {1};
	int lastRecord {-1}; // -1 includes the final source record.
	QString fileNameField; // Optional source field appended to the record number.
	bool failOnPreflight {false}; // Headless jobs can reject critical defects per record.
};

class DataMergeBatchExporter
{
public:
	// Mappings pair source-field names with stable user-variable IDs. Existing
	// files are never intentionally overwritten, and the document is restored.
	static bool exportPdfs(ScribusMainWindow* mainWindow, const DataMergeSource& source,
		const QMap<QString, QString>& mapping, const QString& directory, const QString& prefix,
		DataMergeBatchResult& result, const std::function<bool(int, int)>& progress = {},
		const DataMergeBatchOptions& options = {});
};

#endif
