/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program.
*/

#ifndef INDDRECORDPROBE_H
#define INDDRECORDPROBE_H

#include <array>

#include <QString>
#include <QVector>
#include <QtGlobal>

namespace Indd
{

enum class RecordPageStatus
{
	Parsed,
	IoError,
	InvalidDocument,
	InvalidPageIndex,
	NotRecordPage
};

// Opaque envelope fields only. Neither header word identifies live content.
struct RecordEnvelope
{
	quint16 offset { 0 };
	quint16 length { 0 };
	quint16 wordAt2 { 0 };
	quint32 wordAt4 { 0 };
	// A matching page-local footer slot does not establish document-wide liveness.
	bool slotIndexEntryMatches { false };
};

struct RecordPageResult
{
	RecordPageStatus status { RecordPageStatus::IoError };
	QVector<RecordEnvelope> records;
	bool footerSlotIndexValidated { false };

	bool parsed() const { return status == RecordPageStatus::Parsed; }
};

enum class RecordCatalogStatus
{
	Scanned,
	IoError,
	InvalidDocument,
	LimitExceeded
};

struct RecordReference
{
	quint32 databasePageIndex { 0 };
	RecordEnvelope envelope;
};

// Observed footer type 9 and its opaque token. Neither proves that the page
// is reachable from the active database root.
struct RecordPageFooterReference
{
	quint32 databasePageIndex { 0 };
	quint32 opaqueToken { 0 };
	bool envelopeChainParsed { false };
};

struct RecordCatalogResult
{
	RecordCatalogStatus status { RecordCatalogStatus::IoError };
	quint32 databasePageCount { 0 };
	quint32 candidatePageCount { 0 };
	quint32 skippedPageCount { 0 };
	quint32 footerValidatedPageCount { 0 };
	quint32 duplicateFooterTokenCount { 0 };
	QVector<RecordReference> records;
	QVector<RecordPageFooterReference> observedType9Footers;

	bool scanned() const { return status == RecordCatalogStatus::Scanned; }
};

enum class PageDirectoryStatus
{
	Validated,
	IoError,
	InvalidDocument,
	UnsupportedLayout,
	InvalidMapping,
	LimitExceeded
};

// A narrowly observed header -> type-4 page -> type-5 directory chain.
// This is a diagnostic candidate, not a general INDD root specification.
enum class CandidateDirectoryRootStatus
{
	Candidate,
	IoError,
	InvalidDocument,
	UnsupportedLayout,
	InputChanged
};

struct CandidateDirectoryRootResult
{
	CandidateDirectoryRootStatus status { CandidateDirectoryRootStatus::IoError };
	quint32 headerRootPageIndex { 0 };
	quint32 directoryPageIndex { 0 };

	bool candidate() const { return status == CandidateDirectoryRootStatus::Candidate; }
};

struct PageDirectoryEntry
{
	quint32 opaqueToken { 0 };
	quint32 databasePageIndex { 0 };
	quint32 footerType { 0 };
};

struct PageDirectoryResult
{
	PageDirectoryStatus status { PageDirectoryStatus::IoError };
	quint32 headerRootPageIndex { 0 };
	quint32 directoryPageIndex { 0 };
	quint32 declaredEmptySlots { 0 };
	QVector<PageDirectoryEntry> entries;

	bool validated() const { return status == PageDirectoryStatus::Validated; }
};

struct PageDirectoryAuditResult
{
	PageDirectoryStatus status { PageDirectoryStatus::IoError };
	quint32 mappedType9PageCount { 0 };
	quint32 unmappedType9PageCount { 0 };
	quint32 mappedParsedPageCount { 0 };
	quint32 unmappedParsedPageCount { 0 };

	bool audited() const { return status == PageDirectoryStatus::Validated; }
};

// A page-directory token plus a validated page-local slot. This is a
// diagnostic address, not a document object ID or proof of liveness.
struct DirectoryMappedRecordSlotReference
{
	quint32 opaquePageToken { 0 };
	quint16 pageLocalSlot { 0 };
	quint32 databasePageIndex { 0 };
	quint16 envelopeOffset { 0 };
	quint16 envelopeLength { 0 };
};

struct DirectoryMappedRecordCatalogResult
{
	PageDirectoryStatus status { PageDirectoryStatus::IoError };
	quint32 mappedType9PageCount { 0 };
	quint32 parsedPageCount { 0 };
	quint32 unsupportedPageCount { 0 };
	quint32 footerValidatedPageCount { 0 };
	quint32 unindexedEnvelopeCount { 0 };
	QVector<RecordReference> records;
	QVector<DirectoryMappedRecordSlotReference> validatedSlots;

	bool cataloged() const { return status == PageDirectoryStatus::Validated; }
};

// Raw words in one narrowly observed 124-byte record shape. Correlation with
// IDML identifiers is not proof of a live frame, story, or ownership link.
struct CandidateFrameStoryReference
{
	quint32 opaquePageToken { 0 };
	quint16 pageLocalSlot { 0 };
	quint32 frameCandidateWord { 0 };
	quint32 storyCandidateWord { 0 };
};

struct CandidateFrameStoryCatalogResult
{
	PageDirectoryStatus status { PageDirectoryStatus::IoError };
	quint32 scannedValidatedSlotCount { 0 };
	quint32 rejectedShapeCount { 0 };
	quint32 unsupportedPageCount { 0 };
	quint32 unindexedEnvelopeCount { 0 };
	QVector<CandidateFrameStoryReference> references;

	bool cataloged() const { return status == PageDirectoryStatus::Validated; }
};

// A matching raw key in an observed type-6 row whose packed address and
// length agree with a mapped, page-local-indexed type-9 envelope. Neither
// the key's type nor the row's liveness or ownership is established.
struct CandidateType6RowTarget
{
	quint32 sourcePageToken { 0 };
	quint16 sourceRowOffset { 0 };
	quint32 targetPageToken { 0 };
	quint16 targetPageLocalSlot { 0 };
	quint16 targetEnvelopeLength { 0 };
	quint32 targetRecordWordAt4 { 0 };
	quint32 targetRecordWordAt12 { 0 };
	bool targetHasWordAt12 { false };
};

struct CandidateType6RowTargetResult
{
	PageDirectoryStatus status { PageDirectoryStatus::IoError };
	quint32 scannedType6PageCount { 0 };
	quint32 unsupportedType6PageCount { 0 };
	quint32 rejectedMatchingRowCount { 0 };
	quint32 unsupportedType9PageCount { 0 };
	QVector<CandidateType6RowTarget> targets;

	bool searched() const { return status == PageDirectoryStatus::Validated; }
};

enum class RecordSlotDiffStatus
{
	Compared,
	IoError,
	InvalidDocument,
	IncompatibleDocuments,
	UnsupportedLayout,
	InvalidMapping,
	LimitExceeded,
	InputChanged
};

// Only diagnostic addresses with a validated page-local back-pointer are
// compared. A matching address may be reused for a different object.
struct RecordSlotDiffEntry
{
	quint32 opaquePageToken { 0 };
	quint16 pageLocalSlot { 0 };
	bool presentInFirst { false };
	bool presentInSecond { false };
	bool envelopeBytesChanged { false };
	bool physicalPageChanged { false };
	quint16 firstEnvelopeLength { 0 };
	quint16 secondEnvelopeLength { 0 };
	// Shared byte runs after the opaque 8-byte envelope prefix. These are
	// byte-level diagnostics, not decoded text or a semantic edit span.
	quint16 sharedAfterHeaderPrefixBytes { 0 };
	quint16 sharedAfterHeaderSuffixBytes { 0 };
};

struct RecordSlotDiffResult
{
	RecordSlotDiffStatus status { RecordSlotDiffStatus::IoError };
	quint32 firstValidatedSlotCount { 0 };
	quint32 secondValidatedSlotCount { 0 };
	quint32 firstUnsupportedPageCount { 0 };
	quint32 secondUnsupportedPageCount { 0 };
	quint32 firstUnindexedEnvelopeCount { 0 };
	quint32 secondUnindexedEnvelopeCount { 0 };
	quint32 firstOnlyCount { 0 };
	quint32 secondOnlyCount { 0 };
	quint32 commonCount { 0 };
	quint32 identicalEnvelopeCount { 0 };
	quint32 changedEnvelopeCount { 0 };
	quint32 changedEnvelopeLengthCount { 0 };
	quint32 relocatedPhysicalPageCount { 0 };
	QVector<RecordSlotDiffEntry> differences;

	bool compared() const { return status == RecordSlotDiffStatus::Compared; }
};

// An exact envelope-byte match after omitting only the two-byte page-local
// slot field. Even a unique match cannot establish object identity/liveness.
struct ExactRecordMove
{
	quint32 firstOpaquePageToken { 0 };
	quint16 firstPageLocalSlot { 0 };
	quint32 secondOpaquePageToken { 0 };
	quint16 secondPageLocalSlot { 0 };
};

struct ExactRecordRelocationResult
{
	RecordSlotDiffStatus status { RecordSlotDiffStatus::IoError };
	quint32 firstValidatedSlotCount { 0 };
	quint32 secondValidatedSlotCount { 0 };
	quint32 firstUnsupportedPageCount { 0 };
	quint32 secondUnsupportedPageCount { 0 };
	quint32 firstUnindexedEnvelopeCount { 0 };
	quint32 secondUnindexedEnvelopeCount { 0 };
	quint32 uniqueExactMatchCount { 0 };
	quint32 sameAddressCount { 0 };
	quint32 movedAddressCount { 0 };
	quint32 firstUnmatchedCount { 0 };
	quint32 secondUnmatchedCount { 0 };
	quint32 firstAmbiguousCount { 0 };
	quint32 secondAmbiguousCount { 0 };
	QVector<ExactRecordMove> moves;

	bool compared() const { return status == RecordSlotDiffStatus::Compared; }
};

enum class CandidateStoryStatus
{
	Decoded,
	IoError,
	InvalidDocument,
	UnsupportedLayout,
	InvalidMapping,
	LimitExceeded,
	NotIndexed,
	UnsupportedRecord,
	InvalidEncoding,
	InputChanged
};

struct CandidateStoryResult
{
	CandidateStoryStatus status { CandidateStoryStatus::IoError };
	quint32 opaquePageToken { 0 };
	quint16 pageLocalSlot { 0 };
	QString text; // Includes any terminal carriage return observed in the record.

	bool decoded() const { return status == CandidateStoryStatus::Decoded; }
};

enum class CandidateAffineStatus
{
	Decoded,
	IoError,
	InvalidDocument,
	UnsupportedLayout,
	InvalidMapping,
	LimitExceeded,
	NotIndexed,
	UnsupportedRecord,
	InvalidEncoding,
	InputChanged
};

// The six values are an observed affine tail, not a verified live frame or
// evidence of how that record links to a story.
struct CandidateAffineResult
{
	CandidateAffineStatus status { CandidateAffineStatus::IoError };
	quint32 opaquePageToken { 0 };
	quint16 pageLocalSlot { 0 };
	std::array<double, 6> matrix { 0, 0, 0, 0, 0, 0 };

	bool decoded() const { return status == CandidateAffineStatus::Decoded; }
};

// Raw byte coincidences in validated envelopes only. These are not decoded
// object references or evidence of frame/story ownership.
enum class RecordWordSearchStatus
{
	Searched,
	IoError,
	InvalidDocument,
	UnsupportedLayout,
	InvalidMapping,
	LimitExceeded,
	InputChanged
};

struct RecordWordOccurrence
{
	quint32 opaquePageToken { 0 };
	quint16 pageLocalSlot { 0 };
	quint16 envelopeByteOffset { 0 };
};

struct RecordWordSearchResult
{
	RecordWordSearchStatus status { RecordWordSearchStatus::IoError };
	quint32 unsupportedPageCount { 0 };
	quint32 unindexedEnvelopeCount { 0 };
	quint32 scannedValidatedSlotCount { 0 };
	QVector<RecordWordOccurrence> occurrences;

	bool searched() const { return status == RecordWordSearchStatus::Searched; }
};

constexpr quint32 maxCatalogDatabasePages = 65536; // 256 MiB of database blocks.
constexpr int maxCatalogRecords = 100000;

// Experimental read-only diagnostic for one database page. A parsed envelope
// chain is not an object graph and must not be used to import document content.
RecordPageResult inspectCandidateRecordPage(const QString& filePath, quint32 pageIndex);

// Scans all physical database pages, but catalogs only unambiguous candidate
// envelope chains. Skipped pages may still contain document objects.
RecordCatalogResult catalogCandidateRecordPages(const QString& filePath,
	quint32 pageLimit = maxCatalogDatabasePages, int recordLimit = maxCatalogRecords);

// Read-only tracing of the limited root chain observed in controlled saves.
// The candidate directory table itself is not validated by this diagnostic.
CandidateDirectoryRootResult inspectCandidateDirectoryRoot(const QString& filePath);

// Read-only validation of the directory selected by the guarded root chain.
// A self-consistent map does not establish individual object liveness.
PageDirectoryResult inspectCandidatePageDirectory(const QString& filePath);

// Compares type-9 physical pages with the validated candidate map. "Mapped"
// does not mean live; "unmapped" does not mean safely discardable.
PageDirectoryAuditResult auditCandidatePageDirectory(const QString& filePath);

// Catalogs only type-9 pages referenced by the validated candidate map.
// It also reports only slots with validated page-local back-pointers, keyed
// by the directory's opaque token. Neither mapping establishes liveness.
DirectoryMappedRecordCatalogResult catalogDirectoryMappedRecordPages(const QString& filePath,
	int recordLimit = maxCatalogRecords);

// Catalog only two guarded 124-byte candidate shapes on root-selected,
// page-local-indexed records. Missing pairs are explicit coverage gaps.
CandidateFrameStoryCatalogResult catalogCandidateFrameStoryReferences(const QString& filePath,
	int recordLimit = maxCatalogRecords);

// Read-only, exact-key search in the bounded row layout observed on mapped
// type-6 pages. A row is returned only when its target is a validated slot
// and its packed length agrees; no import or object-ID inference follows.
CandidateType6RowTargetResult findCandidateType6RowTargets(const QString& filePath,
	quint32 rawKeyWord, int matchLimit = 4096, int recordLimit = maxCatalogRecords);

// Compares raw envelopes at validated diagnostic addresses in two compatible
// saves. Unsupported and unindexed coverage is reported separately. This
// cannot determine whether an object is live, retired, or the same object.
RecordSlotDiffResult compareDirectoryMappedRecordSlots(const QString& firstPath,
	const QString& secondPath, int recordLimit = maxCatalogRecords);

// Compare at most 10,000 validated mapped envelopes per saved file by exact
// bytes excluding only the page-local slot word. Duplicate payloads remain
// ambiguous; changed payloads remain unmatched. This is not UID matching.
ExactRecordRelocationResult compareExactCandidateRecordRelocations(const QString& firstPath,
	const QString& secondPath, int recordLimit = 10000);

// Experimental read-only decoder for the bounded 0x262/0x202 record shape
// observed in controlled saves. It does not establish story ownership,
// liveness, text-frame geometry, or a general native INDD text format.
CandidateStoryResult inspectCandidateStoryText(const QString& filePath,
	quint32 opaquePageToken, quint16 pageLocalSlot, int recordLimit = maxCatalogRecords);

// Read-only decoder for one 952-byte indexed envelope with the observed
// 0x151/48-byte affine tail. Other variants are rejected, not guessed.
CandidateAffineResult inspectCandidateAffineTransform(const QString& filePath,
	quint32 opaquePageToken, quint16 pageLocalSlot, int recordLimit = maxCatalogRecords);

// Read-only correlation aid. Finds an exact little-endian 32-bit word at any
// byte offset after the opaque eight-byte envelope header. A match is not a
// typed UID or link. Revalidates each mapped envelope before scanning.
RecordWordSearchResult findCandidateRecordWords(const QString& filePath, quint32 word,
	int matchLimit = 4096, int recordLimit = maxCatalogRecords);

} // namespace Indd

#endif
