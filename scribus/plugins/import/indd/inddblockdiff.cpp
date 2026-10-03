/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program.
*/

#include "inddblockdiff.h"

#include "inddprobe.h"

#include <QFile>

#include <cstring>

namespace Indd
{
namespace
{
constexpr qint64 blockSize = 4096;
} // namespace

BlockDiffResult compareDatabaseBlocks(const QString& firstPath, const QString& secondPath)
{
	BlockDiffResult result;
	const ProbeResult firstProbe = probeFile(firstPath);
	const ProbeResult secondProbe = probeFile(secondPath);
	if (firstProbe.status == ProbeStatus::IoError || secondProbe.status == ProbeStatus::IoError)
		return result;
	if (!firstProbe.valid() || !secondProbe.valid())
	{
		result.status = BlockDiffStatus::InvalidDocument;
		return result;
	}
	if (firstProbe.formatVersion != secondProbe.formatVersion ||
		firstProbe.streamByteOrder != secondProbe.streamByteOrder)
	{
		result.status = BlockDiffStatus::IncompatibleDocuments;
		return result;
	}
	const quint32 commonPageCount = qMin(firstProbe.databasePageCount, secondProbe.databasePageCount);
	if (commonPageCount > maxComparedDatabaseBlocks)
	{
		result.status = BlockDiffStatus::LimitExceeded;
		return result;
	}

	QFile first(firstPath);
	QFile second(secondPath);
	if (!first.open(QIODevice::ReadOnly) || !second.open(QIODevice::ReadOnly))
		return result;
	char firstBlock[blockSize];
	char secondBlock[blockSize];
	for (quint32 page = 0; page < commonPageCount; ++page)
	{
		if (first.read(firstBlock, blockSize) != blockSize || second.read(secondBlock, blockSize) != blockSize)
		{
			result.changedPageIndices.clear();
			return result;
		}
		if (std::memcmp(firstBlock, secondBlock, blockSize) != 0)
			result.changedPageIndices.append(page);
	}
	result.firstPageCount = firstProbe.databasePageCount;
	result.secondPageCount = secondProbe.databasePageCount;
	result.comparedPageCount = commonPageCount;
	result.status = BlockDiffStatus::Compared;
	return result;
}

} // namespace Indd
