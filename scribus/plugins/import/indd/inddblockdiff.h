/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program.
*/

#ifndef INDDBLOCKDIFF_H
#define INDDBLOCKDIFF_H

#include <QVector>
#include <QtGlobal>

class QString;

namespace Indd
{

constexpr quint32 maxComparedDatabaseBlocks = 262144; // 1 GiB per input.

enum class BlockDiffStatus
{
	Compared,
	IoError,
	InvalidDocument,
	IncompatibleDocuments,
	LimitExceeded
};

struct BlockDiffResult
{
	BlockDiffStatus status { BlockDiffStatus::IoError };
	quint32 firstPageCount { 0 };
	quint32 secondPageCount { 0 };
	quint32 comparedPageCount { 0 };
	QVector<quint32> changedPageIndices;

	bool compared() const { return status == BlockDiffStatus::Compared; }
};

// Read-only comparison of common physical 4096-byte database blocks, including
// the two headers. Counts expose blocks unique to either file. Changed blocks
// do not identify live records or editable page content.
BlockDiffResult compareDatabaseBlocks(const QString& firstPath, const QString& secondPath);

} // namespace Indd

#endif
