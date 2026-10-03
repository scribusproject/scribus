/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program.
*/

#include "epubreadingorderdialog.h"

#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>

EpubReadingOrderDialog::EpubReadingOrderDialog(const QVector<EpubStoryEntry>& stories, QWidget* parent) :
	QDialog(parent),
	m_stories(stories)
{
	setWindowTitle(tr("EPUB Reading Order"));
	resize(560, 420);
	auto* layout = new QVBoxLayout(this);
	auto* introduction = new QLabel(tr("Arrange text stories and images in the order readers should encounter them."), this);
	introduction->setWordWrap(true);
	layout->addWidget(introduction);

	bool completeSavedOrder = !m_stories.isEmpty();
	QVector<int> byRank(m_stories.size(), -1);
	for (int index = 0; index < m_stories.size(); ++index)
	{
		const int rank = m_stories.at(index).savedRank;
		if (rank < 1 || rank > m_stories.size() || byRank.at(rank - 1) != -1)
		{
			completeSavedOrder = false;
			break;
		}
		byRank[rank - 1] = index;
	}
	if (!completeSavedOrder)
	{
		auto* warning = new QLabel(tr("Saved ranks are incomplete. This list starts in document item order, which may not be the reading order. Review it before assigning."), this);
		warning->setWordWrap(true);
		layout->addWidget(warning);
	}

	m_list = new QListWidget(this);
	m_list->setSelectionMode(QAbstractItemView::SingleSelection);
	for (int rowIndex = 0; rowIndex < m_stories.size(); ++rowIndex)
	{
		const int index = completeSavedOrder ? byRank.at(rowIndex) : rowIndex;
		const EpubStoryEntry& story = m_stories.at(index);
		const QString location = story.page >= 0
			? tr("Page %1").arg(story.page + 1) : tr("Pasteboard");
		QString label = tr("%1 — %2 — %3").arg(location,
			story.isImage ? tr("Image") : tr("Text"), story.name);
		if (!story.preview.isEmpty())
			label += tr(" — %1").arg(story.preview);
		auto* row = new QListWidgetItem(label, m_list);
		row->setData(Qt::UserRole, index);
	}
	layout->addWidget(m_list);

	auto* moveLayout = new QHBoxLayout;
	m_moveUp = new QPushButton(tr("Move Up"), this);
	m_moveDown = new QPushButton(tr("Move Down"), this);
	m_moveUp->setObjectName(QStringLiteral("epubMoveUp"));
	m_moveDown->setObjectName(QStringLiteral("epubMoveDown"));
	m_list->setObjectName(QStringLiteral("epubStoryList"));
	moveLayout->addWidget(m_moveUp);
	moveLayout->addWidget(m_moveDown);
	moveLayout->addStretch();
	layout->addLayout(moveLayout);

	auto* limitation = new QLabel(tr("Text stories and independent linked images can be ordered here, including those inside groups. Tables, shapes, inline images, and other unsupported content still prevent EPUB export. Image descriptions and linked PNG/JPEG files are required."), this);
	limitation->setWordWrap(true);
	layout->addWidget(limitation);
	auto* buttons = new QDialogButtonBox(this);
	auto* assign = buttons->addButton(tr("Assign Displayed Order"), QDialogButtonBox::AcceptRole);
	buttons->addButton(QDialogButtonBox::Cancel);
	assign->setEnabled(!m_stories.isEmpty());
	layout->addWidget(buttons);

	connect(m_moveUp, &QPushButton::clicked, this, [this]() { moveCurrent(-1); });
	connect(m_moveDown, &QPushButton::clicked, this, [this]() { moveCurrent(1); });
	connect(m_list, &QListWidget::currentRowChanged, this, [this](int) { updateButtons(); });
	connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
	if (!m_stories.isEmpty())
		m_list->setCurrentRow(0);
	updateButtons();
}

QVector<int> EpubReadingOrderDialog::orderedIndices() const
{
	QVector<int> indices;
	for (int row = 0; row < m_list->count(); ++row)
		indices.append(m_list->item(row)->data(Qt::UserRole).toInt());
	return indices;
}

void EpubReadingOrderDialog::moveCurrent(int offset)
{
	const int from = m_list->currentRow();
	const int to = from + offset;
	if (from < 0 || to < 0 || to >= m_list->count())
		return;
	QListWidgetItem* item = m_list->takeItem(from);
	m_list->insertItem(to, item);
	m_list->setCurrentRow(to);
	updateButtons();
}

void EpubReadingOrderDialog::updateButtons()
{
	const int row = m_list->currentRow();
	m_moveUp->setEnabled(row > 0);
	m_moveDown->setEnabled(row >= 0 && row < m_list->count() - 1);
}
