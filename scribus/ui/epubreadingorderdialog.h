/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program.
*/

#ifndef EPUBREADINGORDERDIALOG_H
#define EPUBREADINGORDERDIALOG_H

#include <QDialog>
#include <QString>
#include <QVector>

class QListWidget;
class QPushButton;

struct EpubStoryEntry
{
	QString name;
	QString preview;
	int page { -1 };
	int savedRank { -1 };
	bool isImage { false };
};

class EpubReadingOrderDialog : public QDialog
{
public:
	explicit EpubReadingOrderDialog(const QVector<EpubStoryEntry>& stories, QWidget* parent = nullptr);
	QVector<int> orderedIndices() const;

private:
	void moveCurrent(int offset);
	void updateButtons();

	QVector<EpubStoryEntry> m_stories;
	QListWidget* m_list { nullptr };
	QPushButton* m_moveUp { nullptr };
	QPushButton* m_moveDown { nullptr };
};

#endif
