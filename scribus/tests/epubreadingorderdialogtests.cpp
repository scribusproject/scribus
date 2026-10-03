/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program.
*/

#include "ui/epubreadingorderdialog.h"

#include <QAbstractButton>
#include <QDialogButtonBox>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QtTest>

class EpubReadingOrderDialogTests : public QObject
{
	Q_OBJECT

private slots:
	void loadsCompleteSavedOrder();
	void incompleteRanksDoNotImplyOrder();
	void moveControlsAndExplicitAccept();
	void mixedTextAndImageOrder();
	void emptyListCannotAssign();
};

void EpubReadingOrderDialogTests::loadsCompleteSavedOrder()
{
	EpubReadingOrderDialog dialog({ { QStringLiteral("Later"), {}, 2, 2 },
		{ QStringLiteral("Earlier"), {}, 0, 1 } });
	QCOMPARE(dialog.orderedIndices(), (QVector<int> { 1, 0 }));
	auto* list = dialog.findChild<QListWidget*>(QStringLiteral("epubStoryList"));
	QVERIFY(list);
	QVERIFY(list->item(0)->text().contains(QStringLiteral("Earlier")));
}

void EpubReadingOrderDialogTests::incompleteRanksDoNotImplyOrder()
{
	EpubReadingOrderDialog dialog({ { QStringLiteral("First"), {}, 0, 2 },
		{ QStringLiteral("Second"), {}, 1, -1 } });
	QCOMPARE(dialog.orderedIndices(), (QVector<int> { 0, 1 }));
	bool warningShown = false;
	for (const QLabel* label : dialog.findChildren<QLabel*>())
		warningShown |= label->text().contains(QStringLiteral("document item order"));
	QVERIFY(warningShown);
}

void EpubReadingOrderDialogTests::moveControlsAndExplicitAccept()
{
	EpubReadingOrderDialog dialog({ { QStringLiteral("A"), {}, 0, 1 },
		{ QStringLiteral("B"), {}, 0, 2 }, { QStringLiteral("C"), {}, 0, 3 } });
	auto* up = dialog.findChild<QPushButton*>(QStringLiteral("epubMoveUp"));
	auto* down = dialog.findChild<QPushButton*>(QStringLiteral("epubMoveDown"));
	QVERIFY(up);
	QVERIFY(down);
	QVERIFY(!up->isEnabled());
	QVERIFY(down->isEnabled());
	dialog.show();
	QTest::mouseClick(down, Qt::LeftButton);
	QCOMPARE(dialog.orderedIndices(), (QVector<int> { 1, 0, 2 }));
	QVERIFY(up->isEnabled());
	QTest::mouseClick(down, Qt::LeftButton);
	QCOMPARE(dialog.orderedIndices(), (QVector<int> { 1, 2, 0 }));
	QVERIFY(!down->isEnabled());
	auto* buttons = dialog.findChild<QDialogButtonBox*>();
	QVERIFY(buttons);
	QPushButton* assign = nullptr;
	for (QAbstractButton* button : buttons->buttons())
	{
		if (buttons->buttonRole(button) == QDialogButtonBox::AcceptRole)
			assign = qobject_cast<QPushButton*>(button);
	}
	QVERIFY(assign);
	QTest::mouseClick(assign, Qt::LeftButton);
	QCOMPARE(dialog.result(), int(QDialog::Accepted));
}

void EpubReadingOrderDialogTests::mixedTextAndImageOrder()
{
	EpubReadingOrderDialog dialog({
		{ QStringLiteral("Ending"), QStringLiteral("Last paragraph"), 2, 3 },
		{ QStringLiteral("Portrait"), QStringLiteral("A portrait"), 1, 2, true },
		{ QStringLiteral("Opening"), QStringLiteral("First paragraph"), 0, 1 }
	});
	QCOMPARE(dialog.orderedIndices(), (QVector<int> { 2, 1, 0 }));
	auto* list = dialog.findChild<QListWidget*>(QStringLiteral("epubStoryList"));
	QVERIFY(list);
	QVERIFY(list->item(0)->text().contains(QStringLiteral("Text")));
	QVERIFY(list->item(1)->text().contains(QStringLiteral("Image")));
	QVERIFY(list->item(1)->text().contains(QStringLiteral("A portrait")));
	list->setCurrentRow(1);
	QTest::mouseClick(dialog.findChild<QPushButton*>(QStringLiteral("epubMoveUp")), Qt::LeftButton);
	QCOMPARE(dialog.orderedIndices(), (QVector<int> { 1, 2, 0 }));
	EpubReadingOrderDialog incomplete({
		{ QStringLiteral("Opening"), {}, 0, 1 },
		{ QStringLiteral("Portrait"), {}, 0, -1, true },
		{ QStringLiteral("Ending"), {}, 0, 2 }
	});
	QCOMPARE(incomplete.orderedIndices(), (QVector<int> { 0, 1, 2 }));
}

void EpubReadingOrderDialogTests::emptyListCannotAssign()
{
	EpubReadingOrderDialog dialog({});
	QCOMPARE(dialog.orderedIndices().size(), 0);
	auto* buttons = dialog.findChild<QDialogButtonBox*>();
	QVERIFY(buttons);
	for (QAbstractButton* button : buttons->buttons())
	{
		if (buttons->buttonRole(button) == QDialogButtonBox::AcceptRole)
			QVERIFY(!button->isEnabled());
	}
}

QTEST_MAIN(EpubReadingOrderDialogTests)
#include "epubreadingorderdialogtests.moc"
